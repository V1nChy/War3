// MyResourceManager.h: interface for the MyResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_RESOURCE_MANAGER_H_
#define _MY_RESOURCE_MANAGER_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include <windows.h>

#include "MyResource.h"
#include <map>
using namespace std;


typedef pair<string, MyResource *> PairResName;
typedef pair<ResHandle, MyResource *> PairResHandle;

/** 资源管理器 */
class MyResourceManager  
{
protected:
	map<ResHandle, MyResource *> m_pResMapByHandle;
	map<string, MyResource *> m_pResMapByName;

	ResHandle m_NextHandle;

	bool m_hasLoadAll;
public:
	bool HasLoadAll();

	void ReloadAll();
	void UnloadAll();
	void RemoveAll();
	void LoadAll();

	MyResource* CreateResource(const string& strName, const string& strOrigin, MyResource* pRes, MyResourceLoader * pResLoader = 0);
	MyResource* CreateResource(const string& strOrigin, MyResource* pRes, MyResourceLoader* pResLoader = 0);

	bool	RemoveResource(const string& strName);
	bool	RemoveResource(ResHandle handle);

	virtual bool Unload(ResHandle handle);
	virtual bool Unload(const string& strResName);

	virtual bool Load(ResHandle handle);
	virtual bool Load(const string& strResName);

	virtual bool Reload(ResHandle handle);
	virtual bool Reload(const string& strResName);

	MyResource * GetResource(const string& strResName);
	MyResource * GetResource(ResHandle handle);

	MyResourceManager();
	virtual ~MyResourceManager();

    virtual HRESULT OneTimeInit()				{ return S_OK;}
    virtual HRESULT InitDeviceObjects()			{ return S_OK;}
    virtual HRESULT InvalidateDeviceObjects()	{ return S_OK;}
    virtual HRESULT DeleteDeviceObjects()		{ return S_OK;}
	virtual HRESULT RestoreDeviceObjects()		{ return S_OK;}
	virtual void FinalCleanup()					{ RemoveAll();}

};

#endif
