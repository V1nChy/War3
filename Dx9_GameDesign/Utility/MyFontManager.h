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


/** ���������, ��ͳһ�ӿ� */
class MyFontManager  
{
private:
	MyFontManager();
	static MyFontManager* c_pFontManager;
	const UINT m_uMaxFontNums;		//���������

	int m_ID;		//������
public:
	virtual ~MyFontManager();
	static MyFontManager* GetSingleton();

	/** ��Ϊ��������ɾ��,���Բ��� vector ���洢*/
	vector<MyFont *> m_vUserFont;

	/** ͨ������ı��,�ҵ�����
	 * @note: ��û�ж�Ӧ�����򷵻�Ĭ������
	 */
	MyFont * GetFont(int id) const ;
	MyFont * GetFont(const string& fontName, UINT height, FONT_FLAG eFlags = FF_NORMAL) const;
	bool IsFontExist(const string& fontName, int height, FONT_FLAG eFlags = FF_NORMAL) const;
	bool IsFontExist(int id);

	/** ��UserFontList�������µ�����, ��������ı��, ������򷵻� -1 */
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
