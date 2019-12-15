// GEFontManager.cpp: implementation of the GEFontManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MyFontManager.h"
#include "../DxGraphicLayer.h"
#include <assert.h>

#include "MyUTIL.H"

MyFontManager* MyFontManager::c_pFontManager = new MyFontManager;

MyFontManager::MyFontManager():
	m_uMaxFontNums(0)
{
	m_ID = 0;
}

MyFontManager::~MyFontManager()
{

}

MyFontManager * MyFontManager::GetSingleton()
{
	return c_pFontManager;
}


int MyFontManager::AddNewFont(const string& strFont, DWORD dwHeight, FONT_FLAG eFlags)
{
	if (m_vUserFont.size() >= m_uMaxFontNums)
		return 0;

	if (IsFontExist(strFont, dwHeight, eFlags))
		return GetFont(strFont, dwHeight, eFlags)->GetID();

	MyFont * newFont = NULL;
	newFont = new MyFont(m_ID, strFont, dwHeight, eFlags);

	if (newFont == NULL)
		return -1;
	//获得当前图形设备接口
	LPDIRECT3DDEVICE9 lpdevice = DxGraphicLayer::GetSingleton()->GetD3DDevice();
	if (lpdevice)
	{
		newFont->InitDeviceObjects(lpdevice);
	}

	m_ID ++;

	m_vUserFont.push_back(newFont);

	return newFont->GetID();
}

HRESULT MyFontManager::OneTimeSceneInit()
{
	m_pSystemFont = new MyFont(m_ID++, "SIMLI", 18, FF_BOLD);
	m_pSmallFont = new MyFont(m_ID++, "宋体", 8);
	m_pNormalFont = new MyFont(m_ID++, "宋体", 18);
	m_pBigFont = new MyFont(m_ID++, "宋体", 24, FF_BOLD);

	m_vUserFont.push_back(m_pSystemFont);
	m_vUserFont.push_back(m_pSmallFont);
	m_vUserFont.push_back(m_pNormalFont);
	m_vUserFont.push_back(m_pBigFont);

	if (!m_pSmallFont || !m_pSystemFont || !m_pBigFont || !m_pNormalFont)
		return E_FAIL;

	return S_OK;

}

HRESULT MyFontManager::InitDeviceObjects()
{
	HRESULT hr = S_OK;
	LPDIRECT3DDEVICE9 lpdevice = DxGraphicLayer::GetSingleton()->GetD3DDevice();
	for (size_t i = 0; i < m_vUserFont.size(); ++i)
	{
		hr |= m_vUserFont[i]->InitDeviceObjects(lpdevice);
	}

	return hr;
}

HRESULT MyFontManager::InvalidateDeviceObjects()
{
	HRESULT hr = S_OK;
	for (size_t i = 0; i < m_vUserFont.size(); ++i)
	{
		hr |= m_vUserFont[i]->InvalidateDeviceObjects();
	}

	return hr;
}


HRESULT MyFontManager::DeleteDeviceObjects()
{
	HRESULT hr = S_OK;
	for (size_t i = 0; i < m_vUserFont.size(); ++i)
	{
		hr |= m_vUserFont[i]->DeleteDeviceObjects();
	}

	return hr;
}

void MyFontManager::FinalCleanup()
{
	for ( size_t i = 0; i < m_vUserFont.size(); ++i )
	{
		SAFE_DELETE(m_vUserFont[i]);
	}
	m_vUserFont.clear();

	SAFE_DELETE(c_pFontManager);
}

MyFont * MyFontManager::GetFont(int id) const
{
	if (id >= 0 && id< static_cast<int>(m_vUserFont.size()))
		return m_vUserFont[id];
	else
		return m_pSystemFont;
}

MyFont * MyFontManager::GetFont(const string& fontName, UINT height, FONT_FLAG eFlags) const
{
	for (size_t i = 0; i < m_vUserFont.size(); ++i)
	{
		if (m_vUserFont[i]->GetName() == fontName 
			&& m_vUserFont[i]->GetFontHeight() == height
			&& m_vUserFont[i]->GetFlags() == eFlags)
			return m_vUserFont[i];
	}

	//默认字体
	return m_pSystemFont;
}

bool MyFontManager::IsFontExist(const string& fontName, int height, FONT_FLAG eFlags) const
{
	for (size_t i = 0; i < m_vUserFont.size(); ++i)
	{
		if (m_vUserFont[i]->GetName() == fontName 
			&& m_vUserFont[i]->GetFontHeight() == height
			&& m_vUserFont[i]->GetFlags() == eFlags)
			return true;
	}

	return false;
}

bool MyFontManager::IsFontExist(int id)
{
	if (id >= 0 && id < static_cast<int>(m_vUserFont.size()))
		return true;
	else
		return false;
}