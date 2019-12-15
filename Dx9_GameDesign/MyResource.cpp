// GEResource.cpp: implementation of the GEResource class.
//
//////////////////////////////////////////////////////////////////////

#include "MyResource.h"
#include "GEFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyResource::MyResource()
{
	m_bIsLoaded = false;
	m_iTimesRef = 1;
}

MyResource::~MyResource()
{
	Delete();
}

//-----------------------------------------------------------------------------
// Name: InitParms()
// Desc: 初始化参数
//-----------------------------------------------------------------------------
bool MyResource::InitParms(ResHandle handle, string strName, string strOrigin, MyResourceManager *pResManager, MyResourceLoader * pResLoader)
{
	if (m_bIsLoaded)
	{
		string err = "Resource File not exist: ";
		err += m_strName;
		LOGINFO(err.c_str());
		return false;
	}

	m_hHandle = handle;
	m_pResManager = pResManager;
	m_pResLoader = pResLoader;
	m_strName = strName;
	m_strOrigin = strOrigin;

	m_iSize = 0;
	m_bIsLoaded = false;
	m_bLocked = false;

	m_iTimesRef = 1;

	return true;
}

//-----------------------------------------------------------------------------
// Name: Load()
// Desc: 加载，两种方式①资源加载器②LoadImpl（）
//-----------------------------------------------------------------------------
bool MyResource::Load()
{
	if (!m_bIsLoaded)
	{
		#ifdef _DEBUG
			if (!GEFile::IsFileExist(m_strOrigin))
			{
				string err = "Resource File not exist: "; 
				err += m_strOrigin;
				LOGINFO(err.c_str());
				//GELog::GetSingleton()->AppendLog(err, __FILE__, __LINE__);
				return false;
			}
		#endif


		if (m_pResLoader != NULL)
		{
			if (!m_pResLoader->LoadResouce(this))
			{
				string err = "Resource can not be load successfully: "; 
				err += m_strOrigin;
				LOGINFO(err.c_str());
				//GELog::GetSingleton()->AppendLog(err, __FILE__, __LINE__);

				return false;
			}
		}
		else
		{
			if ( !LoadImpl() )
			{
				string err = "Resource can not be load successfully: "; 
				err += m_strOrigin;
				LOGINFO(err.c_str());
				//GELog::GetSingleton()->AppendLog(err, __FILE__, __LINE__);

				return false;
			}
		}

		// Calculate resource size
		m_iSize = CalculateSize();
		// Now loaded
		m_bIsLoaded = true;
	}

	return true;
}

bool MyResource::Reload()
{
	if (m_bIsLoaded)
	{
		bool r = Unload();
		r &= Load();
		if (r == false)
			return r;
	}

#ifdef _DEBUG
	string err = "Can't reload resource: "; 
	err += m_strOrigin;
	err += ". Because it has not been loaded yet. ";
	LOGINFO(err.c_str());
	//GELog::GetSingleton()->AppendLog(err, __FILE__, __LINE__);
#endif

	return false;
}

bool MyResource::Unload()
{
	if (m_bIsLoaded)
	{
		if (UnloadImpl())
		{
			m_bIsLoaded = false;
			return true;
		}
	}

#ifdef _DEBUG
	string err = "Can't unload resource: "; 
	err += m_strOrigin;
	err += ". Because it has not been loaded yet or something wrong. ";
	LOGINFO(err.c_str());
	//GELog::GetSingleton()->AppendLog(err, __FILE__, __LINE__);
#endif

	return false;
}

bool MyResource::Delete()
{
	if (m_bIsLoaded)
	{
		 Unload();
	}

	MinusTimesRef();

	return true;
}
