// GELight.cpp: implementation of the GELight class.
//
//////////////////////////////////////////////////////////////////////

#include "MyLight.h"
#include "DxGraphicLayer.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyLight::MyLight(const string& name, MyObject * father, const Vector3& offset,
		float radianRight, float radianUp, float radianDir, bool isVisible)
		: MyMovable(name , father, offset, radianRight, radianUp, radianDir, isVisible)
{
	InitLight(true, D3DLIGHT_DIRECTIONAL);
	m_uIndex = _GetIndex();
}

MyLight::~MyLight()
{

}


void MyLight::InitLight(bool enable, D3DLIGHTTYPE Type, MyColor& Diffuse, MyColor& Specular, MyColor& Ambient,
		float Range, float Falloff, float Attenuation0, float Attenuation1, float Attenuation2, 
		float Theta, float Phi)
{
	m_Light.Type = Type;
	m_Light.Diffuse = Diffuse.GetAsD3DCOLOR();
	m_Light.Specular = Specular.GetAsD3DCOLOR();
	m_Light.Ambient = Ambient.GetAsD3DCOLOR();
	m_Light.Range = Range;
	m_Light.Falloff = Falloff;
	m_Light.Attenuation0 = Attenuation0;
	m_Light.Attenuation1 = Attenuation1;
	m_Light.Attenuation2 = Attenuation2;
	m_Light.Theta = Theta;
	m_Light.Phi = Phi;

	m_Light.Position.x = GetPosition().x;
	m_Light.Position.y = GetPosition().y;
	m_Light.Position.z = GetPosition().z;
	m_Light.Direction.x = GetDir().x;
	m_Light.Direction.y = GetDir().y;
	m_Light.Direction.z = GetDir().z;

	m_bEnable = enable;
}

DWORD MyLight::_GetIndex()
{
	static DWORD index = 0;

	return index++;
}

D3DLIGHT9 * MyLight::GetLight()
{
	return &m_Light;
}

HRESULT MyLight::LightEnable(bool bEnable)
{
	m_bEnable = bEnable;
	return DxGraphicLayer::GetSingleton()->LightEnable(m_uIndex, m_bEnable);
}

HRESULT MyLight::RestoreDeviceObjects()
{
	DxGraphicLayer::GetSingleton()->SetLight(m_uIndex, this);
	DxGraphicLayer::GetSingleton()->LightEnable(m_uIndex, m_bEnable);

	return MyMovable::RestoreDeviceObjects();
}


HRESULT MyLight::FrameMove(float deltTime)
{
	HRESULT r = MyMovable::FrameMove(deltTime);
	m_Light.Position.x = GetPosition().x;
	m_Light.Position.y = GetPosition().y;
	m_Light.Position.z = GetPosition().z;
	m_Light.Direction.x = GetDir().x;
	m_Light.Direction.y = GetDir().y;
	m_Light.Direction.z = GetDir().z;

	return r;
}

