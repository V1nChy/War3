// GETextureManager.cpp: implementation of the GETextureManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MyTextureManager.h"
#include "MyTexture.h"
//#include "MyTextureList.h"
#include "GEFile.h"
#include "MyUtil.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyTextureManager* MyTextureManager::c_pTextureManager = new MyTextureManager;

MyTextureManager::MyTextureManager()
{
	static bool IfHasSingleton = false;

	assert(!IfHasSingleton);

	IfHasSingleton = true;
}


MyTextureManager::~MyTextureManager()
{

}


MyTextureManager * MyTextureManager::GetSingleton()
{
	return c_pTextureManager;
}

HRESULT MyTextureManager::InitDeviceObjects()
{
	LoadAll();
	return S_OK;
}

HRESULT MyTextureManager::DeleteDeviceObjects()
{
	UnloadAll();
	return S_OK;
}




My2DTexture * MyTextureManager::Create2DTexture(const string& strName,const string& strOrigin)
{
	return (My2DTexture*)(MyTextureManager::GetSingleton()->CreateResource(strName,strOrigin,new My2DTexture));
}

My2DTexture * MyTextureManager::Create2DTexture(const string& strOrigin)
{
	return (My2DTexture*)(MyTextureManager::GetSingleton()->CreateResource(strOrigin,new My2DTexture));
}



HRESULT MyTextureManager::OneTimeInit()
{
	Create2DTexture("WhiteTxtr", "Texture/white.bmp");
	Create2DTexture("BLACK", "Texture/black.bmp");

	return S_OK;
}
