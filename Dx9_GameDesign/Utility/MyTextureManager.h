// MyTextureManager.h: interface for the MyTextureManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_TEXTURE_MANAGER_H_
#define _MY_TEXTURE_MANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "My2DTexture.h"
//#include "GECubeTexture.h"
//#include "GEVolumnTexture.h"
//#include "GETextureList.h"
#include "MyResourceManager.h"

#include <vector>
using namespace std;

class GETextureList;

class MyTextureManager : public MyResourceManager  
{
public:
	MyTextureManager();
	virtual ~MyTextureManager();

	static MyTextureManager * GetSingleton();

    HRESULT InitDeviceObjects();
    HRESULT DeleteDeviceObjects();
	HRESULT OneTimeInit();

	My2DTexture * Create2DTexture(const string& strName, const string& strOrigin);
	My2DTexture * Create2DTexture(const string& strOrigin);


protected:
	static MyTextureManager* c_pTextureManager;
};

#endif // !defined(AFX_GETEXTUREMANAGER_H__5E71446F_9F9B_40C3_8C90_C87887106E71__INCLUDED_)
