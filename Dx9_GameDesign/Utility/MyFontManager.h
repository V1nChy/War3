// GEFontManager.h: interface for the GEFontManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEFONTMANAGER_H__E5E1FED0_6EED_4239_A16B_68ED3615379A__INCLUDED_)
#define AFX_GEFONTMANAGER_H__E5E1FED0_6EED_4239_A16B_68ED3615379A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "MyFont.h"

#include <vector>
using namespace std;


/** 字体管理器, 有统一接口 */
class MyFontManager  
{
private:
	MyFontManager();
	static MyFontManager* c_pFontManager;
	const UINT m_uMaxFontNums;		//最多字体数

	int m_ID;		//字体编号
public:
	virtual ~MyFontManager();
	static MyFontManager* GetSingleton();

	/** 因为字体库很少删除,所以采用 vector 来存储*/
	vector<MyFont *> m_vUserFont;

	/** 通过字体的编号,找到字体
	 * @note: 如没有对应字体则返回默认字体
	 */
	MyFont * GetFont(int id) const ;
	MyFont * GetFont(const string& fontName, UINT height, FONT_FLAG eFlags = FF_NORMAL) const;
	bool IsFontExist(const string& fontName, int height, FONT_FLAG eFlags = FF_NORMAL) const;
	bool IsFontExist(int id);

	/** 往UserFontList中增加新的字体, 返回字体的编号, 如错误则返回 -1 */
	int AddNewFont(const string& strFont, DWORD dwHeight, FONT_FLAG eFlags = FF_NORMAL);

	HRESULT OneTimeSceneInit();
	HRESULT InitDeviceObjects();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	HRESULT RestoreDeviceObjects();
	void FinalCleanup();

	MyFont * m_pSmallFont;
	MyFont * m_pBigFont;
	MyFont * m_pNormalFont;
	MyFont * m_pSystemFont;
};

#endif // !defined(AFX_GEFONTMANAGER_H__E5E1FED0_6EED_4239_A16B_68ED3615379A__INCLUDED_)
