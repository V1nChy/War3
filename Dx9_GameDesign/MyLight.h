// GELight.h: interface for the GELight class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GELIGHT_H__080FD8F2_D62D_47FD_BC51_A049496E86CB__INCLUDED_)
#define AFX_GELIGHT_H__080FD8F2_D62D_47FD_BC51_A049496E86CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <d3d9.h>
#include <d3d9types.h>
#include "MyColor.h"
#include "MyMovable.h"


/** 光,封装D3DLIGHT,场景管理器的一部分 */
class MyLight : public MyMovable  
{
public:
	HRESULT RestoreDeviceObjects();
	D3DLIGHT9 * GetLight(void);
	HRESULT LightEnable(bool bEnable);

	MyLight(const string& name, MyObject * father, const Vector3& offset = Vector3::ZERO,
		float radianPitch = 0, float radianYaw = 0, float radianRoll = 0, bool isVisible = true);

	virtual ~MyLight();
    
	void InitLight(bool enable, D3DLIGHTTYPE Type, MyColor& Diffuse = MyColor::White, MyColor& Specular = MyColor::White, MyColor& Ambient = MyColor::White,
		float Range = 1000, float Falloff = 1.f, float Attenuation0 = 0, float Attenuation1 = 1.f, float Attenuation2 = 0, 
		float Theta = PI/3, float Phi = HALF_PI);

	HRESULT FrameMove(float deltTime);


protected:
	D3DLIGHT9 m_Light;
	DWORD m_uIndex; 
	static DWORD _GetIndex();
	bool m_bEnable;

};

#endif // !defined(AFX_GELIGHT_H__080FD8F2_D62D_47FD_BC51_A049496E86CB__INCLUDED_)
