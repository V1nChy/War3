//#include"GEIndexBuffer.h"
#include"MyUtil.h"



GEIndexBuffer::GEIndexBuffer(UINT size, UINT stride, 
							 GEVertexBuffer* VBassociated, UINT baseIndex,  bool managed, bool dynamic,
							 bool autoBackupForRestore)
							 : m_size(size), m_format(GEFormat_Index16), m_stride(stride), m_indexCount(0), 
							 m_managed(managed), m_dynamic(dynamic), 
							 m_autoBackupForRestore( (m_managed)?false:autoBackupForRestore ), 
							 m_DX9IB(NULL), m_sysbuffer(NULL),
							 m_VBassociated(VBassociated), m_baseIndex(baseIndex)
{		
	/*
	D3DFORMAT d3dformat;
	d3dformat=TranslateToD3DFormat(m_format);
	*/

	try
	{
		if(!m_managed && m_autoBackupForRestore)
		{
			m_sysbuffer=new BYTE[size];
			if(!m_sysbuffer)
			{
				LOGINFO("Out of memory when create sysbuffer for index buffer.");
			}
		}
	}
	catch(...)
	{
		LOGINFO("Out of memory when create sysbuffer for index buffer.");
	}	
}

GEIndexBuffer::~GEIndexBuffer()
{
	SAFE_RELEASE(m_DX9IB);
	SAFE_DELETE(m_sysbuffer);
	//m_indexCount=0;
	//m_size=0;
	//m_stride=0;
	//m_format=GEFormat_UNKNOWN;	
	//m_VBassociated=NULL;
}

void GEIndexBuffer::PrepareSysBuffer(void* pSource, UINT sizeToFeed)
{
	if(sizeToFeed>m_size)
	{			
		LOGINFO("Attempt to feed IndexBuffer with data more than m_size.");
	}		

	m_dataSize=sizeToFeed;
	if(m_autoBackupForRestore && m_sysbuffer)
		memcpy(m_sysbuffer, pSource, m_dataSize);
	else
		LOGINFO("IndexBuffer::PrepareSysBuffer error!, Try to feed system buffer but set m_autoBackupForRestore false.");

	if(m_stride) m_indexCount+=m_dataSize/m_stride;	// set the index count		
}

void GEIndexBuffer::PrepareSysBuffer(void* pSource)
{		
	m_dataSize=m_size;
	if(m_autoBackupForRestore && m_sysbuffer)
		memcpy(m_sysbuffer, pSource, m_dataSize);
	else
		LOGINFO("IndexBuffer::PrepareSysBuffer error!, Try to feed system buffer but set m_autoBackupForRestore false.");
	if(m_stride) m_indexCount+=m_dataSize/m_stride;	// set the index count		
}

void GEIndexBuffer::_CreateDXIB()
{
	HRESULT r;
	if(m_managed)
	{
		if(!m_dynamic)
			r=GEGraphicLayer::GetSingleton()->GetD3DDevice()->CreateIndexBuffer(m_size, D3DUSAGE_WRITEONLY, TranslateToD3DFormat(m_format), D3DPOOL_MANAGED, &m_DX9IB, NULL);
		else
			r=GEGraphicLayer::GetSingleton()->GetD3DDevice()->CreateIndexBuffer(m_size, D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, TranslateToD3DFormat(m_format), D3DPOOL_MANAGED, &m_DX9IB, NULL);
	}
	else
	{
		if(!m_dynamic)
			r=GEGraphicLayer::GetSingleton()->GetD3DDevice()->CreateIndexBuffer(m_size, D3DUSAGE_WRITEONLY, TranslateToD3DFormat(m_format), D3DPOOL_DEFAULT, &m_DX9IB, NULL);
		else
			r=GEGraphicLayer::GetSingleton()->GetD3DDevice()->CreateIndexBuffer(m_size, D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC, TranslateToD3DFormat(m_format), D3DPOOL_DEFAULT, &m_DX9IB, NULL);
	}

	//r=GEGraphicLayer::GetSingleton()->GetDevice()->CreateIndexBuffer(m_size, D3DUSAGE_DYNAMIC, TranslateToD3DFormat(m_format), D3DPOOL_DEFAULT, &m_DX9IB, NULL);
	if(FAILED(r))
	{
		string err;
		if(r==D3DERR_INVALIDCALL)
		{
			err="D3D9 CreateIndexBuffer failedInvalid: call, e.g. parameter has invalid value.";
		}
		else if(r==D3DERR_OUTOFVIDEOMEMORY)
		{
			err="D3D9 CreateIndexBuffer failedInvalid:Out of video memory.";
		}
		else if(r==E_OUTOFMEMORY)
		{
			err="D3D9 CreateIndexBuffer failedInvalid:Out of memory.";
		}
		else if(r==D3DXERR_INVALIDDATA)
		{
			err="D3D9 CreateIndexBuffer failedInvalid:The data is invalid.";
		}
		else 
		{
			err="D3D9 CreateIndexBuffer failedInvalid:Unknown type error.";
		}

		LOGINFO( err.c_str());
	}
}

void GEIndexBuffer::_CopySysBuffeToDXIB()
{
	void* tmp;
	HRESULT r;

	r=m_DX9IB->Lock(0, m_dataSize, &tmp, D3DLOCK_DISCARD);

	if(FAILED(r))
	{
		LOGINFO("D3D9 IndexBuffer lock failed.");
	}

	memcpy(tmp, m_sysbuffer, m_dataSize);

	r=m_DX9IB->Unlock();
	if(FAILED(r))	LOGINFO("D3D9 IndexBuffer unlock error.");

	// 以下计算 m_minIndex 和 m_maxIndex:		
	if(m_format==GEFormat_Index16)
	{
		m_minIndex=65535;
		m_maxIndex=0;
		unsigned short* p=static_cast<unsigned short*>(m_sysbuffer);	// 两个字节
		for(size_t i=0;i<m_indexCount;i++)
		{
			if(*p<m_minIndex) m_minIndex=*p;
			if(*p>m_maxIndex) m_maxIndex=*p;
			++p;
		}
	}
	else if(m_format==GEFormat_Index32)
	{
		m_minIndex=4294967295L;
		m_maxIndex=0;
		unsigned int* p=static_cast<unsigned int*>(m_sysbuffer);	// 四个字节
		for(size_t i=0;i<m_indexCount;i++)
		{
			if(*p<m_minIndex) m_minIndex=*p;
			if(*p>m_maxIndex) m_maxIndex=*p;
			++p;
		}
	}			
}

void GEIndexBuffer::_FeedDXIB(void* sourceData)
{
	void* tmp;
	HRESULT r;
	r=m_DX9IB->Lock(0, m_dataSize, &tmp, D3DLOCK_DISCARD);		

	if(FAILED(r))
	{
		LOGINFO("D3D9 IndexBuffer lock failed.");
	}

	memcpy(tmp, sourceData, m_dataSize);

	r=m_DX9IB->Unlock();

	if(FAILED(r))
	{
		LOGINFO("D3D9 IndexBuffer unlock error");
	}

	// 以下计算 m_minIndex 和 m_maxIndex:		
	if(m_format==GEFormat_Index16)
	{
		m_minIndex=0xffff;
		m_maxIndex=0;
		unsigned short* p=static_cast<unsigned short*>(sourceData);	// 两个字节
		for(size_t i=0;i<m_indexCount;i++)
		{
			if(*p<m_minIndex) m_minIndex=*p;
			if(*p>m_maxIndex) m_maxIndex=*p;
			p++;
		}
	}
	else if(m_format==GEFormat_Index32)
	{
		m_minIndex=0xffffffff;
		m_maxIndex=0;
		unsigned int* p=static_cast<unsigned int*>(sourceData);	// 四个字节
		for(size_t i=0;i<m_indexCount;i++)
		{
			if(*p<m_minIndex) m_minIndex=*p;
			if(*p>m_maxIndex) m_maxIndex=*p;
			++p;
		}
	}
}


UINT GEIndexBuffer::GetPrimitiveCount() const
{
	int count;
	switch(m_VBassociated->GetPrimitiveType())
	{
	case GE_PRIMITIVETYPE_UNKNOWN:
		count=0;
		break;
	case GE_POINTLIST:
		count=m_indexCount;
		break; 
	case GE_LINELIST:
		count=m_indexCount/2;
		break;
	case GE_LINESTRIP:
		count=m_indexCount-1;
		break;
	case GE_TRIANGLELIST:
		count=m_indexCount/3;
		break;
	case GE_TRIANGLESTRIP:
		count=m_indexCount-2;
		break;
	case GE_TRIANGLEFAN:
		count=m_indexCount-2;
		break;
	}
	return count;
}


// 非managed的index buffer调用此版本，无需在此提供顶点数据	
void GEIndexBuffer::InitDeviceObjects()
{
	if(m_managed) 
		LOGINFO("IndexBuffer::InitDeviceObjects() error!, Try to init a managed index buffer without providing index data source");
	// 如果是managed，必须提供数据源。
	// 如果不是managed，那么在InitDeviceObjects不需要做任何事情，而是在Restore时创建
	// 并填充顶点缓冲。
}

// managed的index buffer调用此版本(除非是GEMesh的IB)，必须提供顶点数据
void GEIndexBuffer::InitDeviceObjects(void* pSource, UINT sizeToFeed)
{
	if(m_managed) 
	{
		_CreateDXIB();

		if(sizeToFeed<=m_size)
		{
			m_dataSize=sizeToFeed;
			if(m_stride) m_indexCount=m_dataSize/m_stride;
			_FeedDXIB(pSource);
		}
		else
		{
			LOGINFO("GEIndexBuffer::InitDeviceObjects(void* pSource, UINT sizeToFeed) error! , sizeToFeed is larger than buffer size!");
		}
	}
	else
		LOGINFO("GEIndexexBuffer::InitDeviceObjects(void* pSource, UINT sizeToFeed) error!, Try to provide data to a NONMANAGED index buffer when InitDeviceObjects().");
}

// GEMesh的IB调用此版本
void GEIndexBuffer::InitDeviceObjectsFromMesh()
{
	_CreateDXIB();
	m_dataSize=m_size;
}

// managed的index buffer以及设m_autoBackupForRestore为真的非managed的index buffer调用此版本。	
void GEIndexBuffer::RestoreDeviceObjects()
{
	if(!m_managed)
	{
		_CreateDXIB();		

		if(m_autoBackupForRestore)
			_CopySysBuffeToDXIB();
		else
			LOGINFO("Failed to restore IndexBuffer. Does not set m_autoBackupForRestore, but try to restore without providing data source!");
	}
	// managed的index buffer不需要做任何事情。
}

// 非managed并且未指明自动备份数据(m_autoBakcupForRestore为false)的index buffer调用此函数。
void GEIndexBuffer::RestoreDeviceObjects(void* backSource, UINT sizeToFeed)
{		
	if(sizeToFeed>m_size)
	{			
		LOGINFO("Attempt to feed IndexBuffer with data more than m_size.");
	}

	if(m_autoBackupForRestore==true)
	{
		LOGINFO("IndexBuffer::RestoreDeviceObjects() error! Try to restore with a external data source but set m_autoBackupForRestore true.");
	}

	m_dataSize=sizeToFeed;
	if(m_stride) m_indexCount=m_dataSize/m_stride;

	_CreateDXIB();

	if(!m_autoBackupForRestore)
		_FeedDXIB(backSource);
	else
		LOGINFO("Failed to restore IndexBuffer. Have not set m_autoBackupForRestore false, but try to restore without providing data source!");

}

void GEIndexBuffer::InvalidateDeviceObjects()
{
	if(!m_managed)	SAFE_RELEASE(m_DX9IB);
}

void GEIndexBuffer::DeleteDeviceObjects()
{
	if(m_managed) SAFE_RELEASE(m_DX9IB);
}

