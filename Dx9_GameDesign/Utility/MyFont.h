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

/** 对齐方式 */
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
//字体样式
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

/** 写字符串 \n
*	@params [DWORD] Format:
*		DT_NOCLIP		: 无裁减, 会快些
*		DT_TOP			: 上对齐
*		DT_BOTTOM		: 下对齐, 必须和 DT_SINGLELINE 合用
*		DT_LEFT			: 左对齐
*		DT_RIGHT		: 右对齐
*		DT_CENTER		: 水平居中
*		DT_VCENTER		: 垂直居中
*		DT_SINGLELINE	: 只显示单行
*		DT_END_ELLIPSIS or DT_PATH_ELLIPSIS : 裁减字符使之能在矩形中显示. 不会改变字符本身, 除非和 DT_MODIFYSTRING 一起用
*											  DT_END_ELLIPSIS 是裁减末端字符, DT_PATH_ELLIPSIS则裁减中间的字符
*------------------------------------------------------------------------------------------------------------------------
*		DT_CALCRECT		: 计算矩形的宽高, 不绘制. 返回高度. 传入的rightBottom会改变
*		DT_EDITCONTROL	: ?? 重复显示
*		DT_EXPANDTABS	: 是字符充满矩形(用tab扩充), 不能和 DT_WORD_ELLIPSIS, DT_PATH_ELLIPSIS, DT_END_ELLIPSIS 合用
*		DT_EXTERNALLEADING : ?? 将字符的多头包括进行高
*		DT_HIDEPREFIX, DT_NOPREFIX, DT_PREFIXONLY : 前缀有关, 没什么用
*		DT_INTERNAL		: ?? 用系统默认字符去计算"字律"
*		DT_MODIFYSTRING : 修改字符串, 必须和 DT_END_ELLIPSIS or DT_PATH_ELLIPSIS 合用
*/

/** 字体类,封装了D3DXFONT */
class MyFont
{
protected:
	LPD3DXFONT	m_pFont;		//DX字体接口
	string		m_strFontName;	//字体名
	DWORD		m_dwFontHeight;	//字体的高度
	FONT_FLAG   m_eFontFlags;	//字体样式
	int m_ID;					//字体ID

public:

	MyFont(int id, string strFontName, DWORD hwHeight, FONT_FLAG eFlags = FF_NORMAL);
	virtual ~MyFont();
	//初始化字体
	HRESULT DeleteDeviceObjects();
	HRESULT InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT InvalidateDeviceObjects() { return m_pFont->OnLostDevice(); }
	HRESULT RestoreDeviceObjects() { return m_pFont->OnResetDevice(); }
	HRESULT OneTimeSceneInit() { return S_OK; }
	//渲染
	int DrawText(const string& str, const MyColor& Color, int left, int top, int right = 0, int bottom = 0, ALIGN_FORMAT format = AF_DEFAULT);

	int GetID() const { return m_ID; }
	const string& GetName() const { return m_strFontName; }
	int GetFontHeight() const { return m_dwFontHeight; }
	FONT_FLAG GetFlags() const { return m_eFontFlags; }
};

#endif // !defined(AFX_GEFONT_H__D67961B2_C11E_488A_8EB2_23D5BF42ACC6__INCLUDED_)
