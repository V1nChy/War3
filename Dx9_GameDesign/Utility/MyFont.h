// GEFont.h: interface for the GEFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEFONT_H__D67961B2_C11E_488A_8EB2_23D5BF42ACC6__INCLUDED_)
#define AFX_GEFONT_H__D67961B2_C11E_488A_8EB2_23D5BF42ACC6__INCLUDED_

#pragma once

#include <d3dx9core.h>
#include <d3d9.h>
#include "MyColor.h"
#include <string>
using namespace std;

// Font creation flags
const DWORD D3DFONT_BOLD = 0x0001;
const DWORD D3DFONT_ITALIC = 0x0002;

/** ���뷽ʽ */
enum ALIGN_FORMAT
{
	AF_DEFAULT = DT_NOCLIP,

	AF_TOP_LEFT = DT_LEFT | DT_TOP,
	AF_TOP_CTR = DT_CENTER | DT_TOP,
	AF_TOP_RT = DT_RIGHT | DT_RIGHT,

	AF_VMD_LEFT = DT_LEFT | DT_VCENTER,
	AF_VMD_CTR = DT_CENTER | DT_VCENTER,
	AF_VMD_RT = DT_RIGHT | DT_VCENTER,

	AF_BTM_LEFT = DT_LEFT | DT_BOTTOM,
	AF_BTM_CTR = DT_CENTER | DT_BOTTOM,
	AF_BTM_RT = DT_RIGHT | DT_BOTTOM,

};
//������ʽ
enum FONT_FLAG
{
	FF_NORMAL = 0x0,
	FF_BOLD = D3DFONT_BOLD,
	FF_ITALIC = D3DFONT_ITALIC,
	FF_BOLD_ITALIC = D3DFONT_BOLD | D3DFONT_ITALIC
};


ALIGN_FORMAT Str2AlignFmt(const string& str);
string AlignFmt2Str(const ALIGN_FORMAT& format);

FONT_FLAG Str2FontFlags(const string& str);
string FontFlags2Str(const FONT_FLAG& flag);

/** д�ַ��� \n
*	@params [DWORD] Format:
*		DT_NOCLIP		: �޲ü�, ���Щ
*		DT_TOP			: �϶���
*		DT_BOTTOM		: �¶���, ����� DT_SINGLELINE ����
*		DT_LEFT			: �����
*		DT_RIGHT		: �Ҷ���
*		DT_CENTER		: ˮƽ����
*		DT_VCENTER		: ��ֱ����
*		DT_SINGLELINE	: ֻ��ʾ����
*		DT_END_ELLIPSIS or DT_PATH_ELLIPSIS : �ü��ַ�ʹ֮���ھ�������ʾ. ����ı��ַ�����, ���Ǻ� DT_MODIFYSTRING һ����
*											  DT_END_ELLIPSIS �ǲü�ĩ���ַ�, DT_PATH_ELLIPSIS��ü��м���ַ�
*------------------------------------------------------------------------------------------------------------------------
*		DT_CALCRECT		: ������εĿ��, ������. ���ظ߶�. �����rightBottom��ı�
*		DT_EDITCONTROL	: ?? �ظ���ʾ
*		DT_EXPANDTABS	: ���ַ���������(��tab����), ���ܺ� DT_WORD_ELLIPSIS, DT_PATH_ELLIPSIS, DT_END_ELLIPSIS ����
*		DT_EXTERNALLEADING : ?? ���ַ��Ķ�ͷ�������и�
*		DT_HIDEPREFIX, DT_NOPREFIX, DT_PREFIXONLY : ǰ׺�й�, ûʲô��
*		DT_INTERNAL		: ?? ��ϵͳĬ���ַ�ȥ����"����"
*		DT_MODIFYSTRING : �޸��ַ���, ����� DT_END_ELLIPSIS or DT_PATH_ELLIPSIS ����
*/

/** ������,��װ��D3DXFONT */
class MyFont
{
protected:
	LPD3DXFONT	m_pFont;		//DX����ӿ�
	string		m_strFontName;	//������
	DWORD		m_dwFontHeight;	//����ĸ߶�
	FONT_FLAG   m_eFontFlags;	//������ʽ
	int m_ID;					//����ID

public:

	MyFont(int id, string strFontName, DWORD hwHeight, FONT_FLAG eFlags = FF_NORMAL);
	virtual ~MyFont();
	//��ʼ������
	HRESULT DeleteDeviceObjects();
	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT InvalidateDeviceObjects() { return m_pFont->OnLostDevice(); }
	HRESULT RestoreDeviceObjects() { return m_pFont->OnResetDevice(); }
	HRESULT OneTimeSceneInit() { return S_OK; }
	//��Ⱦ
	int DrawText(const string& str, const MyColor& Color, int left, int top, int right = 0, int bottom = 0, ALIGN_FORMAT format = AF_DEFAULT);

	int GetID() const { return m_ID; }
	const string& GetName() const { return m_strFontName; }
	int GetFontHeight() const { return m_dwFontHeight; }
	FONT_FLAG GetFlags() const { return m_eFontFlags; }
};

#endif // !defined(AFX_GEFONT_H__D67961B2_C11E_488A_8EB2_23D5BF42ACC6__INCLUDED_)
