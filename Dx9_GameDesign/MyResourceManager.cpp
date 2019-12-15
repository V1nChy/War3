// MyResourceManager.cpp: implementation of the MyResourceManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MyResourceManager.h"
#include "MyUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyResourceManager::MyResourceManager()
{
	m_NextHandle = 0;
	m_hasLoadAll = false;
}

MyResourceManager::~MyResourceManager()
{

}

bool MyResourceManager::HasLoadAll()
{
	return m_hasLoadAll;
}

MyResource * MyResourceManager::GetResource(const string& strResName)
{
	map<string, MyResource*>::iterator pRes;

	pRes = m_pResMapByName.find(strResName);
	
	if (pRes != m_pResMapByName.end())
		return pRes->second;
	else 
	{
		string err = "Can find resource: ";
		err += strResName;
		//LOGINFO(err.c_str());
		//GELog::GetSingleton()->AppendLog(err);
		return NULL;
	}
}

MyResource * MyResourceManager::GetResource(ResHandle handle)
{
	map<ResHandle, MyResource*>::iterator pRes;

	pRes = m_pResMapByHandle.find(handle);
	
	if (pRes != m_pResMapByHandle.end())
		return pRes->second;
	else 
	{
		string err = "Can find resource";
		LOGINFO(err.c_str());
		//GELog::GetSingleton()->AppendLog(err);
		return NULL;
	}
}

bool MyResourceManager::Load(const string& strResName)
{
	map<string, MyResource*>::iterator pRes;

	pRes = m_pResMapByName.find(strResName);

	if (pRes != m_pResMapByName.end())
		if (pRes->second->Load())
			return true;
		
	return false;
}

bool MyResourceManager::Load(ResHandle handle)
{
	map<ResHandle, MyResource*>::iterator pRes;

	pRes = m_pResMapByHandle.find(handle);

	if (pRes != m_pResMapByHandle.end())	
		if (pRes->second->Load())
			return true;
		
	return false;
}

bool MyResourceManager::Unload(const string& strResName)
{
	map<string, MyResource*>::iterator pRes;

	pRes = m_pResMapByName.find(strResName);
	
	if (pRes != m_pResMapByName.end())
		if (pRes->second->Unload())
			return true;
		
	return false;
}

bool MyResourceManager::Unload(ResHandle handle)
{
	map<ResHandle, MyResource*>::iterator pRes;

	pRes = m_pResMapByHandle.find(handle);
	
	if (pRes != m_pResMapByHandle.end())
		if (pRes->second->Unload())
			return true;
		
	return false;
}

bool MyResourceManager::Reload(const string& strResName)
{
	map<string, MyResource*>::iterator pRes;

	pRes = m_pResMapByName.find(strResName);
	
	if (pRes != m_pResMapByName.end())
		if (pRes->second->Reload())
			return true;
		
	return false;
}

bool MyResourceManager::Reload(ResHandle handle)
{
	map<ResHandle, MyResource*>::iterator pRes;

	pRes = m_pResMapByHandle.find(handle);
	
	if (pRes != m_pResMapByHandle.end())
		if (pRes->second->Reload())
			return true;
		
	return false;
}

MyResource* MyResourceManager::CreateResource(const string& strName, const string& strOrigin, MyResource* pRes, MyResourceLoader * pResLoader)
{
	// if the origin file not exist , then it can not be created
#ifdef _DEBUG
	/*if (!GEFile::IsFileExist(strOrigin))
	{
		string err = "Resource File not exist, so MyResource can not be created.\n\t\tResource Name: "; 
		err += strName;
		err += "\t\tResource File: "; 
		err += strOrigin;
		GELog::GetSingleton()->AppendLog(err, __FILE__, __LINE__);
		SAFE_DELETE(pRes);
		return NULL;
	}*/
#endif


	if (!pRes->InitParms(m_NextHandle, strName, strOrigin, this, pResLoader))
	{
		// 说明该resource已经被加载过了,不能被重新初始化,除非先 unload 
		return pRes;
	}

	// check if already created
	map<string, MyResource*>::iterator pr = m_pResMapByName.find(strName);
	if (pr != m_pResMapByName.end())
	{
		SAFE_DELETE(pRes);
		pRes = pr->second;
		pr->second->AddTimesRef();	// 增加一次引用计数

		/*#ifdef _DEBUG
			string err = "Resource has already in the map, or name conflicted: ";
			err += strName;
			err += "\t; Ref Times : ";
			err += GEUTIL::IntToStr(pr->second->GetTimesRef());
			GEDEBUGINFO(err);
		#endif*/

		return pr->second;
	}
	
	PairResHandle	reshp(m_NextHandle++, pRes);
	PairResName		resnp(strName, pRes);

	m_pResMapByName.insert(resnp);
	m_pResMapByHandle.insert(reshp);

	return pRes;
}


MyResource* MyResourceManager::CreateResource(const string& strOrigin, MyResource* pRes, MyResourceLoader * pResLoader)
{
	return CreateResource(strOrigin, strOrigin, pRes, pResLoader);
}


bool MyResourceManager::RemoveResource(const string& strName)
{
	map<string, MyResource*>::iterator pRes;

	pRes = m_pResMapByName.find(strName);

	if (pRes == m_pResMapByName.end())
		return false;

	int i = pRes->second->GetTimesRef();
	if (pRes->second->GetTimesRef() > 1)	// 说明还有别的对象在用这个资源,不能删除
	{
		pRes->second->MinusTimesRef();
		return true;
	}

	ResHandle hr = pRes->second->GetHandle();
	map<ResHandle, MyResource *>::iterator pRes2 = m_pResMapByHandle.find(hr);
	
	if (!pRes->second->Delete())
		return false;
	
	m_pResMapByName.erase( pRes );
	m_pResMapByHandle.erase( pRes2 );
		
	return true;
}


bool MyResourceManager::RemoveResource(ResHandle handle)
{
	map<ResHandle, MyResource*>::iterator pRes;

	pRes = m_pResMapByHandle.find(handle);

	if (pRes == m_pResMapByHandle.end())
		return false;

	if (pRes->second->GetTimesRef() > 1)	// 说明还有别的对象在用这个资源,不能删除
	{
		pRes->second->MinusTimesRef();
		return true;
	}

	string name = pRes->second->GetName();
	map<string, MyResource *>::iterator pRes2 = m_pResMapByName.find(name);
	
	if (!pRes->second->Delete())
		return false;

	m_pResMapByName.erase( pRes2 );
	m_pResMapByHandle.erase( pRes );

	return true;
}

void MyResourceManager::RemoveAll()
{
	map<ResHandle, MyResource*>::iterator pRes = m_pResMapByHandle.begin();
	for (int i = m_pResMapByHandle.size(); i > 0; ++pRes, --i)
	{
		pRes->second->Delete();
	}
	m_pResMapByHandle.clear();
	m_pResMapByName.clear();

}

void MyResourceManager::UnloadAll()
{
	map<ResHandle, MyResource*>::iterator pRes = m_pResMapByHandle.begin();
	for (int i = m_pResMapByHandle.size(); i > 0; ++pRes, --i)
	{
		pRes->second->Unload();
	}
	m_hasLoadAll = false;
}

void MyResourceManager::ReloadAll()
{
	map<ResHandle, MyResource*>::iterator pRes = m_pResMapByHandle.begin();
	for (int i = m_pResMapByHandle.size(); i > 0; ++pRes, --i)
	{
		pRes->second->Reload();
	}
}


void MyResourceManager::LoadAll()
{
	map<ResHandle, MyResource*>::iterator pRes = m_pResMapByHandle.begin();
	for (int i = m_pResMapByHandle.size(); i > 0; ++pRes, --i)
	{
		pRes->second->Load();
	}
	m_hasLoadAll = true;
}
