#include "MyUtil.h"
#include "GEVertexBuffer.h"
//#include "GEBone.h"
//#include "GESkeleton.h"
//#include "GEBoneAssignment.h"
#include <string>
using namespace std;

/**
* @param autoBackupForRestore ���Ϊtrue�Ļ���RestoreDeviceObjectsʱ���봫��������Դ�������Զ���system buffer�л���������m_DX9VB
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
* Ҫ��仺���������ݱȻ�����Сʱ���ô˺�����
* @param sourceData ��䶥�㻺����������Դ
* @param sizeToFeed Ҫ�������ݵ��ֽ��������ܳ���m_size
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
* sourceData����������ݴ�СǡΪm_sizeʱ���ô˺���
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



// ��managed��vertex buffer���ô˰汾�������ڴ��ṩ��������	
void GEVertexBuffer::InitDeviceObjects()
{
	if(m_managed) 
		LOGINFO("VertexBuffer::InitDeviceObjects() error!, Try to init a managed vertex buffer without providing vertex data source");
	// �����managed�������ṩ����Դ��
	// �������managed����ô��InitDeviceObjects����Ҫ���κ����飬������Restoreʱ����
	// ����䶥�㻺�塣
}


// managed��vertex buffer���ô˰汾(������GEMesh��VB)�������ṩ��������
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

// GEMesh��VB���ô˰汾
void GEVertexBuffer::InitDeviceObjectsFromMesh()
{
	_CreateDXVB();
	m_dataSize=m_size;
	if(m_stride) m_vertexCount=m_dataSize/m_stride;
}


// managed��vertex buffer�Լ���m_autoBackupForRestoreΪ��ķ�managed��vertex buffer���ô˰汾��
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
	// managed��vertex buffer����Ҫ���κ����顣

}

// ��managed����δָ���Զ���������(m_autoBakcupForRestoreΪfalse)��vertex buffer���ô˺�����
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

	// ������Ҫ��ԭʼbuffer��������
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

	////�����Ӱ��buffer������ͷ��򲿷֣������������꣩��Ȼ���ԭʼbufferȡ�����ꡢ���򣬱任��ӵ�Ӱ��buffer��
 //   if(m_ShadowVB==NULL) this->createShadowBuffer();

	//assert( m_ShadowVB && "There is no shadow VB.");

	//unsigned char *p, *sp;
	//if(FAILED(m_DX9VB->Lock(0,0,(void**)&p,0)) || FAILED(m_ShadowVB->Lock(0,0,(void**)&sp,0))) 
	//	return false;


	//// ���, ������
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
	//unsigned short vertexindex;				// ��������	
	//GEBoneAssignment::BoneIndexWeight iw;	// Ȩ��		
	//unsigned int memOffset;					// �����ڴ�ƫ��
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
	//	// peter: ����ļ���Ϊʲô��Ҫ����, scale?
	//	rot[0][0]=rot[1][1]=rot[2][2]=1;
	//	Matrix4 rot4(rot);
	//	*(Vector3*)(sp+memOffset+sizeofVector3) += *(Vector3*)(p+memOffset+sizeofVector3)  * rot4 * iw.second;
	//	asgn=assignment->GetNextAssignment();
	//}

	//// ����ģ�͵ı߽�
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
	//}// ���½���


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

	//// ��ֵ, ������
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
	//}// ���½���

	//m_DX9VB->Unlock();
	
	return true;
}


