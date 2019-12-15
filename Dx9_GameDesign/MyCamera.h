// GECamera.h: interface for the GECamera class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GECAMERA_H__8688325A_4B41_4F44_A2BB_57C6C3BFCAFD__INCLUDED_)
#define AFX_GECAMERA_H__8688325A_4B41_4F44_A2BB_57C6C3BFCAFD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "MyMovable.h"
#include "Frustum.h"


/** 摄像机,被挂接在场景图中 */
class MyCamera : public MyMovable 
{
private:
	float m_fFov, m_fNearClip, m_fFarClip;	// Projection参数

	Frustum m_sCameraFrustum;				// 照相机的平截头体
	// 创建相机的平截头体
	void CreateClipPlanes(Frustum& frustum, Matrix4& vp);
	
	bool m_bNeedResetPro;						//重置标志
public:
	MyCamera(const string& name, MyObject * father, const Vector3& offset = Vector3::ZERO,
		float radianPitch = 0, float radianYaw = 0, float radianRoll = 0,
		float fov = PI / 2, float nearclip = 0.17f, float farclip = 1000.f);

	virtual ~MyCamera();

	void MakeProjectionTransform();
	void MakeViewTransform();


	virtual HRESULT FrameMove(float deltTime);

	HRESULT InvalidateDeviceObjects();

	void UpdateViewMatrix();
	void SetProjectionData(float fov, float nearclip, float farclip);
	void SetProjectionMatrix(float fov, float nearclip, float farclip);
	void SetProjectionMatrix();

	const Frustum& GetFrustum() const;

	Matrix4 m_mProjection;
	Matrix4 m_mView;
	Matrix4 m_mViewProjection;
	Matrix4	m_mInvView;
	Matrix4	m_mInvProj;
	Matrix4	m_mInvViewProj;
	
	
};

#endif // !defined(AFX_GECAMERA_H__8688325A_4B41_4F44_A2BB_57C6C3BFCAFD__INCLUDED_)
