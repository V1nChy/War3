#include "MyUtil.h"
#include "GEVertexBuffer.h"
//#include "GEBone.h"
//#include "GESkeleton.h"
//#include "GEBoneAssignment.h"
#include <string>
using namespace std;

/**
* @param autoBackupForRestore 如果为true的话，RestoreDeviceObjects时必须传入数据来源，否则自动从system buffer中获得数据填充m_DX9VB
*/
GEVertexBuffer::GEVertexBuffer(UINT size, DWORD FVF, UINT stride, 
							   GEPrimitiveType primitiveType, bool managed, bool dynamic, bool autoBackupForRestore)
							   : m_stride(stride), m_size(size), m_FVF(FVF), m_primitiveType(primitiveType),
							   m_managed(managed), m_dynamic(dynamic), 
							   m_autoBackupForRestore( (managed)?false:autoBackupForRestore ), 
							   m_DX9VB(NULL), m_ShadowVB(NULL),m_sysBuffer(NULL), m_vertexCount(0), m_dataSize(0)

{
	try 
	{
		if(!m_managed && m_autoBackupForRestore)
		{
			m_sysBuffer=new BYTE[size];
			//UINT v=D3D_SDK_VERSION;
			if(!m_sysBuffer)
			{
				LOGINFO("Out of memory when create sysbuffer for vertex buffer.");
			}
		}
	}
	catch(...)
	{
		LOGINFO("Out of memory when create sysbuffer for vertex buffer.");
	}	

}

GEVertexBuffer::~GEVertexBuffer()
{ 
	SAFE_RELEASE(m_DX9VB);
	SAFE_RELEASE(m_ShadowVB);
	SAFE_DELETE(m_sysBuffer);		
}

/*
* 要填充缓冲区的数据比缓冲区小时调用此函数。
* @param sourceData 填充顶点缓冲区的数据源
* @param sizeToFeed 要填充的数据的字节数，不能超过m_size
*/
void GEVertexBuffer::PrepareSysBuffer(void* sourceData, UINT sizeToFeed)
{
	if(sizeToFeed>m_size)
	{
		//GELog::GetSingleton()->AppendLog("Attempt to feed VertexBuffer with data more than m_size.");
		LOGINFO("Attempt to feed VertexBuffer with data more than m_size.");
	}		
	if(m_autoBackupForRestore && m_sysBuffer)	
		memcpy(m_sysBuffer, sourceData, sizeToFeed);
	else
		LOGINFO("VertexBuffer::PrepareSysBuffer error!, Try to feed system buffer but set m_autoBackupForRestore false.");
	m_dataSize=sizeToFeed;

	if(m_stride) m_vertexCount=sizeToFeed/m_stride;
}
/**
* sourceData里包含的数据大小恰为m_size时调用此函数
*/
void GEVertexBuffer::PrepareSysBuffer(void* sourceData)
{		
	m_dataSize=m_size;
	if(m_autoBackupForRestore && m_sysBuffer)
		memcpy(m_sysBuffer, sourceData, m_dataSize);		
	else
		LOGINFO("PrepareSysBuffer error!, Try to feed system buffer but set m_autoBackupForRestore false.");
	if(m_stride) m_vertexCount=m_dataSize/m_stride;
}

void GEVertexBuffer::_CreateDXVB()
{
	HRESULT r;
	if(m_managed)
	{
		if(!m_dynamic)
			r= MyGraphicLayer::GetSingleton()->GetD3DDevice()->CreateVertexBuffer(m_size, 0x00, m_FVF, D3DPOOL_MANAGED, &m_DX9VB, NULL); // D3DUSAGE_WRITEONLY
		else
			r= MyGraphicLayer::GetSingleton()->GetD3DDevice()->CreateVertexBuffer(m_size, D3DUSAGE_DYNAMIC, m_FVF, D3DPOOL_MANAGED, &m_DX9VB, NULL);
	}
	else
	{
		if(!m_dynamic)
			r= MyGraphicLayer::GetSingleton()->GetD3DDevice()->CreateVertexBuffer(m_size, 0x00, m_FVF, D3DPOOL_DEFAULT, &m_DX9VB, NULL);
		else
			r=MyGraphicLayer::GetSingleton()->GetD3DDevice()->CreateVertexBuffer(m_size, D3DUSAGE_DYNAMIC, m_FVF, D3DPOOL_DEFAULT, &m_DX9VB, NULL);
	}


	if(FAILED(r))
	{
		string err;
		if(r==D3DERR_INVALIDCALL)
		{
			err="D3D9 CreateVertexBuffer failed:Invalid call, e.g. parameter has invalid value.";
		}
		else if(r==D3DERR_OUTOFVIDEOMEMORY)
		{
			err="D3D9 CreateVertexBuffer failed:Out of video memory.";
		}
		else if(r==E_OUTOFMEMORY)
		{
			err="D3D9 CreateVertexBuffer failed:Out of memory.";
		}
		else 
		{
			err="D3D9 CreateVertexBuffer failed:Unknown type error.";
		}

		LOGINFO(err.c_str());
	}
}

void GEVertexBuffer::_CopySysBufferToDXVB()
{
	void* tmp;
	HRESULT r;
	r=m_DX9VB->Lock(0, m_dataSize, &tmp, D3DLOCK_DISCARD);		

	if(FAILED(r))
	{
		LOGINFO("D3D9 VertexBuffer lock failed.");
	}

	memcpy(tmp, m_sysBuffer, m_dataSize);

	r=m_DX9VB->Unlock();

	if(FAILED(r))
	{
		LOGINFO("D3D9 VertexBuffer unlock error.");
	}
}

void GEVertexBuffer::_FeedDXVB(void* sourceData)
{
	void* tmp;
	HRESULT r;
	r=m_DX9VB->Lock(0, m_dataSize, &tmp, D3DLOCK_DISCARD);		

	if(FAILED(r))
	{
		LOGINFO("D3D9 VertexBuffer lock failed.");
	}

	memcpy(tmp, sourceData, m_dataSize);

	r=m_DX9VB->Unlock();

	if(FAILED(r))
	{
		LOGINFO("D3D9 VertexBuffer unlock error.");
	}

}

UINT GEVertexBuffer::GetPrimitiveCount() const
{
	UINT count;
	switch(m_primitiveType)
	{
	case GE_PRIMITIVETYPE_UNKNOWN:
		count=0;
		break;
	case GE_POINTLIST:
		count=m_vertexCount;
		break; 
	case GE_LINELIST:
		count=m_vertexCount/2;
		break;
	case GE_LINESTRIP:
		count=m_vertexCount-1;
		break;
	case GE_TRIANGLELIST:
		count=m_vertexCount/3;
		break;
	case GE_TRIANGLESTRIP:
		count=m_vertexCount-2;
		break;
	case GE_TRIANGLEFAN:
		count=m_vertexCount-2;
		break;
	}
	return count;
}



// 非managed的vertex buffer调用此版本，无需在此提供顶点数据	
void GEVertexBuffer::InitDeviceObjects()
{
	if(m_managed) 
		LOGINFO("VertexBuffer::InitDeviceObjects() error!, Try to init a managed vertex buffer without providing vertex data source");
	// 如果是managed，必须提供数据源。
	// 如果不是managed，那么在InitDeviceObjects不需要做任何事情，而是在Restore时创建
	// 并填充顶点缓冲。
}


// managed的vertex buffer调用此版本(除非是GEMesh的VB)，必须提供顶点数据
void GEVertexBuffer::InitDeviceObjects(void* pSource, UINT sizeToFeed)
{
	if(m_managed) 
	{
		_CreateDXVB();

		if(sizeToFeed<=m_size)
		{
			m_dataSize=sizeToFeed;								
			if(m_stride) m_vertexCount=m_dataSize/m_stride;
			_FeedDXVB(pSource);
		}
		else
		{
			LOGINFO("GEVertexBuffer::InitDeviceObjects(void* pSource, UINT sizeToFeed) error! , sizeToFeed is larger than buffer size!");
		}
	}
	else
		LOGINFO("GEVertexBuffer::InitDeviceObjects(void* pSource, UINT sizeToFeed) error!, Try to provide data to a NONMANAGED vertex buffer when InitDeviceObjects().");
}

// GEMesh的VB调用此版本
void GEVertexBuffer::InitDeviceObjectsFromMesh()
{
	_CreateDXVB();
	m_dataSize=m_size;
	if(m_stride) m_vertexCount=m_dataSize/m_stride;
}


// managed的vertex buffer以及设m_autoBackupForRestore为真的非managed的vertex buffer调用此版本。
void GEVertexBuffer::RestoreDeviceObjects()
{
	if(!m_managed)
	{
		_CreateDXVB();		

		if(m_autoBackupForRestore)
			_CopySysBufferToDXVB();
		else
			LOGINFO("Failed to restore VertexBuffer. A nonmanaged vertex buffer didn't not set m_autoBackupForRestore, but try to restore without providing data source!");
	}
	// managed的vertex buffer不需要做任何事情。

}

// 非managed并且未指明自动备份数据(m_autoBakcupForRestore为false)的vertex buffer调用此函数。
void GEVertexBuffer::RestoreDeviceObjects(void* backSource, UINT sizeToFeed)
{		
	if(sizeToFeed>m_size)
	{
		//GELog::GetSingleton()->AppendLog("Attempt to feed VertexBuffer with data more than m_size.");
		LOGINFO("Attempt to feed VertexBuffer with data more than m_size");
	}

	if(m_autoBackupForRestore==true)
	{
		LOGINFO("VertexBuffer::RestoreDeviceObjects error!, Try to restore with a external data source but set m_autoBackupForRestore true.");
	}

	m_dataSize=sizeToFeed;
	if(m_stride) m_vertexCount=m_dataSize/m_stride;

	_CreateDXVB();

	if(!m_autoBackupForRestore)
		_FeedDXVB(backSource);
	else
		LOGINFO("Failed to restore VertexBuffer. Have not set m_autoBackupForRestore false, but try to restore without providing data source!");

}

void GEVertexBuffer::InvalidateDeviceObjects()
{
	if(!m_managed)	
	{ 
		SAFE_RELEASE(m_DX9VB);	
		SAFE_RELEASE(m_ShadowVB);
	}

}

void GEVertexBuffer::DeleteDeviceObjects()
{
	if(m_managed)
	{ 
		SAFE_RELEASE(m_DX9VB);	
		SAFE_RELEASE(m_ShadowVB);
	}
}


bool GEVertexBuffer::createShadowBuffer()
{	
	SAFE_RELEASE(m_ShadowVB);

	HRESULT r=MyGraphicLayer::GetSingleton()->GetD3DDevice()->CreateVertexBuffer(m_size, 0/*D3DUSAGE_DYNAMIC*/, m_FVF, D3DPOOL_MANAGED, &m_ShadowVB, NULL);
	if(r==D3DERR_INVALIDCALL)	assert("zhen qi guai");


	if(FAILED(r)) return false;

	// 创建后要从原始buffer拷贝过来
	void *p,*sp;
	if(m_dynamic)
		r=m_DX9VB->Lock(0,0,&p,D3DLOCK_READONLY|D3DLOCK_NOOVERWRITE);
	else
		r=m_DX9VB->Lock(0,0,&p,D3DLOCK_READONLY);	
	if(FAILED(r)) return false;
	r=m_ShadowVB->Lock(0,0,&sp, D3DLOCK_DISCARD);
	if(FAILED(r)) return false;

	memcpy(sp, p, m_dataSize);

	m_DX9VB->Unlock();
	m_ShadowVB->Unlock();

	return true;
}

bool GEVertexBuffer::updateFromSkeleton(GESkeleton* skeleton, GEBoneAssignment* assignment)
{
	//static unsigned int sizeofVector3 = sizeof(Vector3);

	////先清空影子buffer的坐标和法向部分（保留纹理坐标），然后从原始buffer取出坐标、法向，变换后加到影子buffer里
 //   if(m_ShadowVB==NULL) this->createShadowBuffer();

	//assert( m_ShadowVB && "There is no shadow VB.");

	//unsigned char *p, *sp;
	//if(FAILED(m_DX9VB->Lock(0,0,(void**)&p,0)) || FAILED(m_ShadowVB->Lock(0,0,(void**)&sp,0))) 
	//	return false;


	//// 清空, 点距递增
	//unsigned char* offsetSPP = sp;
	//unsigned char* offsetSPN = sp+sizeofVector3;
	//for(unsigned short i=0; i<m_vertexCount; ++i)
	//{	
	//	// position:
	//	if(m_FVF & D3DFVF_XYZ)
	//		*(Vector3*)(offsetSPP) = Vector3::ZERO;

	//	// normal:
	//	if(m_FVF & D3DFVF_NORMAL) 
	//		*(Vector3*)(offsetSPN) = Vector3::ZERO;

	//	offsetSPP += this->m_stride;
	//	offsetSPN += this->m_stride;
	//}

	//// update:
	//GEBoneAssignment::Assignment asgn=assignment->GetFirstAssignment();
	//unsigned short vertexindex;				// 顶点索引	
	//GEBoneAssignment::BoneIndexWeight iw;	// 权重		
	//unsigned int memOffset;					// 顶点内存偏移
	//GEBone* bone;
	//while(!assignment->IsEnd())
	//{		
	//	vertexindex	= asgn.first;
	//	iw			= asgn.second;
	//	bone		= skeleton->getBone(iw.first);
	//	memOffset	= m_stride*vertexindex;

	//	// position:
	//	*(Vector3*)(sp+memOffset) += *(Vector3*)(p+memOffset) * bone->getFullTransform() * iw.second;

	//	// normal:
	//	Matrix3 rot;
	//	bone->getFullTransform().Extract3x3Matrix(rot);
	//	// peter: 法向的计算为什么需要这样, scale?
	//	rot[0][0]=rot[1][1]=rot[2][2]=1;
	//	Matrix4 rot4(rot);
	//	*(Vector3*)(sp+memOffset+sizeofVector3) += *(Vector3*)(p+memOffset+sizeofVector3)  * rot4 * iw.second;
	//	asgn=assignment->GetNextAssignment();
	//}

	//// 计算模型的边界
	//m_fXMin	= BIG_NUMBER;
	//m_fXMax	= -BIG_NUMBER;
	//m_fYMin	= BIG_NUMBER;
	//m_fYMax	= -BIG_NUMBER;
	//m_fZMin	= BIG_NUMBER;
	//m_fZMax	= -BIG_NUMBER;

	//for (unsigned int i = 0; i < m_vertexCount; ++i)
	//{
	//	const Vector3& v = *(Vector3*)sp;
	//	if (m_fXMin > v.x)
	//		m_fXMin = v.x;
	//	if (m_fXMax < v.x)
	//		m_fXMax = v.x;
	//	if (m_fYMin > v.y)
	//		m_fYMin = v.y;
	//	if (m_fYMax < v.y)
	//		m_fYMax = v.y;
	//	if (m_fZMin > v.z)
	//		m_fZMin = v.z;
	//	if (m_fZMax < v.z)
	//		m_fZMax = v.z;

	//	sp += m_stride;
	//}// 更新结束


	//m_DX9VB->Unlock();
	//m_ShadowVB->Unlock();

	return true;
}


bool GEVertexBuffer::updateFromMorphAnimation(GEMorphAnimationKey* currentPose)
{
	//static unsigned int sizeofVector3 = sizeof(Vector3);

	//unsigned char *p;
	//if(FAILED(m_DX9VB->Lock(0,0,(void**)&p,0))) 
	//	return false;

	//// 赋值, 点距递增
	//unsigned char* offsetPP = p;
	//unsigned char* offsetPN = p+sizeofVector3;
	//for(unsigned short i=0; i < m_vertexCount; ++i)
	//{	
	//	// position:
	//	if(m_FVF & D3DFVF_XYZ)
	//		*(Vector3*)(offsetPP) = currentPose->GetPosition(i);

	//	// normal:
	//	if(m_FVF & D3DFVF_NORMAL) 
	//		*(Vector3*)(offsetPN) = currentPose->GetPosition(i);

	//	offsetPP += this->m_stride;
	//	offsetPN += this->m_stride;
	//}

	//m_fXMin	= BIG_NUMBER;
	//m_fXMax	= -BIG_NUMBER;
	//m_fYMin	= BIG_NUMBER;
	//m_fYMax	= -BIG_NUMBER;
	//m_fZMin	= BIG_NUMBER;
	//m_fZMax	= -BIG_NUMBER;

	//for (unsigned int i = 0; i < m_vertexCount; ++i)
	//{
	//	const Vector3& v = *(Vector3*)p;
	//	if (m_fXMin > v.x)
	//		m_fXMin = v.x;
	//	if (m_fXMax < v.x)
	//		m_fXMax = v.x;
	//	if (m_fYMin > v.y)
	//		m_fYMin = v.y;
	//	if (m_fYMax < v.y)
	//		m_fYMax = v.y;
	//	if (m_fZMin > v.z)
	//		m_fZMin = v.z;
	//	if (m_fZMax < v.z)
	//		m_fZMax = v.z;

	//	p += m_stride;
	//}// 更新结束

	//m_DX9VB->Unlock();
	
	return true;
}


