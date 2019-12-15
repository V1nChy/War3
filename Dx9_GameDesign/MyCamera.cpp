// GECamera.cpp: implementation of the GECamera class.
//
//////////////////////////////////////////////////////////////////////

#include "MyCamera.h"
#include "DxGraphicLayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyCamera::MyCamera(const string& name, MyObject * father, const Vector3& offset,
		float radianRight, float radianUp, float radianDir,
		float fov, float nearclip, float farclip)
		: MyMovable(name , father, offset, radianRight, radianUp, radianDir, true)
{
	m_bNeedResetPro = true;
	m_fFov = fov;
	m_fNearClip = nearclip;
	m_fFarClip = farclip;

	m_mProjection = DxGraphicLayer::GetSingleton()->SetProjectionData(fov, nearclip, farclip);
}



MyCamera::~MyCamera()
{

}

HRESULT MyCamera::FrameMove(float deltTime)
{
	if (m_bNeedResetPro)
		SetProjectionMatrix(m_fFov, m_fNearClip, m_fFarClip);

	UpdateViewMatrix();

	m_mViewProjection = m_mView * m_mProjection;

	CreateClipPlanes(m_sCameraFrustum, m_mViewProjection);

	return MyMovable::FrameMove(deltTime);
}



void MyCamera::CreateClipPlanes(Frustum& frustum, Matrix4& vp)
{
	Vector3 nor;
	float d;
	
	// Left clipping plane
	nor.x = -( vp[0][3] + vp[0][0] );
	nor.y = -( vp[1][3] + vp[1][0] );
	nor.z = -( vp[2][3] + vp[2][0] );
	d     = -( vp[3][3] + vp[3][0] );
	frustum.m_Plane[PLANE_LEFT].Init( nor, d );

	// Right clipping plane
	nor.x = -( vp[0][3] - vp[0][0] );
	nor.y = -( vp[1][3] - vp[1][0] );
	nor.z = -( vp[2][3] - vp[2][0] );
	d     = -( vp[3][3] - vp[3][0] );
	frustum.m_Plane[PLANE_RIGHT].Init( nor, d );

	// Bottom clipping plane
	nor.x = -( vp[0][3] + vp[0][1] );
	nor.y = -( vp[1][3] + vp[1][1] );
	nor.z = -( vp[2][3] + vp[2][1] );
	d     = -( vp[3][3] + vp[3][1] );
	frustum.m_Plane[PLANE_BOTTOM].Init( nor, d );

	// Top clipping plane
	nor.x = -( vp[0][3] - vp[0][1] );
	nor.y = -( vp[1][3] - vp[1][1] );
	nor.z = -( vp[2][3] - vp[2][1] );
	d     = -( vp[3][3] - vp[3][1] );
	frustum.m_Plane[PLANE_TOP].Init( nor, d );

	// Near clipping plane
	nor.x = -vp[0][2];
	nor.y = -vp[1][2];
	nor.z = -vp[2][2];
	d     = -vp[3][2];
	frustum.m_Plane[PLANE_BACK].Init( nor, d );

	// Far clipping plane
	nor.x = -( vp[0][3] - vp[0][2] );
	nor.y = -( vp[1][3] - vp[1][2] );
	nor.z = -( vp[2][3] - vp[2][2] );
	d     = -( vp[3][3] - vp[3][2] );
	frustum.m_Plane[PLANE_FRONT].Init( nor, d );
}

//-----------------------------------------------------------------------------
// Name: SetProjectionData()
// Desc: 设置投影矩阵参数
//-----------------------------------------------------------------------------
void MyCamera::SetProjectionData(float fov, float nearclip, float farclip)
{
	if (RealEqual(m_fFov, fov) && RealEqual(m_fNearClip, nearclip) && RealEqual(m_fFarClip, farclip))
		return;

	m_fFov = fov;
	m_fNearClip = nearclip;
	m_fFarClip = farclip;

	m_mProjection = DxGraphicLayer::GetSingleton()->SetProjectionData(fov, nearclip, farclip);

	m_bNeedResetPro = true;
}

//-----------------------------------------------------------------------------
// Name: SetProjectionMatrix()
// Desc: 设置投影矩阵，只能在初始化设备后调用
//-----------------------------------------------------------------------------
void MyCamera::SetProjectionMatrix(float fov, float nearclip, float farclip)
{
	SetProjectionData(fov, nearclip, farclip);
	
	DxGraphicLayer::GetSingleton()->SetProjectionMatrix(m_mProjection);

	m_bNeedResetPro = false;
}
void MyCamera::SetProjectionMatrix()
{
	DxGraphicLayer::GetSingleton()->SetProjectionMatrix(m_mProjection);

	m_bNeedResetPro = false;
}

void MyCamera::UpdateViewMatrix()
{
    Vector3 vEyePt      = GetPosition();
    Vector3 vLookatPt   = vEyePt + GetDir();
    Vector3 vUpVec      = GetUp();

	Matrix4::MatrixLookAtLH( m_mView, vEyePt, vLookatPt, vUpVec );
}


HRESULT MyCamera::InvalidateDeviceObjects()
{
	m_bNeedResetPro = true;
	return MyMovable::InvalidateDeviceObjects();
}

void MyCamera::MakeViewTransform()
{
	DxGraphicLayer::GetSingleton()->SetViewMatrix( m_mView );
}


const Frustum& MyCamera::GetFrustum() const
{
	return m_sCameraFrustum;
}
