// GEFont.cpp: implementation of the GEFont class.
//
//////////////////////////////////////////////////////////////////////

#include "MyFont.h"
#include "MyUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


ALIGN_FORMAT Str2AlignFmt(const string& str)
{
	if (str == "AF_TOP_LEFT")
		return AF_TOP_LEFT;
	else if (str == "AF_TOP_CTR")
		return AF_TOP_CTR;
	else if (str == "AF_TOP_RT")
		return AF_TOP_RT;
	else if (str == "AF_VMD_LEFT")
		return AF_VMD_LEFT;
	else if (str == "AF_VMD_CTR")
		return AF_VMD_CTR;
	else if (str == "AF_VMD_RT")
		return AF_VMD_RT;
	else if (str == "AF_BTM_LEFT")
		return AF_BTM_LEFT;
	else if (str == "AF_BTM_CTR")
		return AF_BTM_CTR;
	else if (str == "AF_BTM_RT")
		return AF_BTM_RT;
	else
		return AF_DEFAULT;
}

string AlignFmt2Str(const ALIGN_FORMAT& format)
{
	if (format == AF_TOP_LEFT)
		return "AF_TOP_LEFT";
	else if (format == AF_TOP_CTR)
		return "AF_TOP_CTR";
	else if (format == AF_TOP_RT)
		return "AF_TOP_RT";
	else if (format == AF_VMD_LEFT)
		return "AF_VMD_LEFT";
	else if (format == AF_VMD_CTR)
		return "AF_VMD_CTR";
	else if (format == AF_VMD_RT)
		return "AF_VMD_RT";
	else if (format == AF_BTM_LEFT)
		return "AF_BTM_LEFT";
	else if (format == AF_BTM_CTR)
		return "AF_BTM_CTR";
	else if (format == AF_BTM_RT)
		return "AF_BTM_RT";
	else
		return "AF_DEFAULT";
}

FONT_FLAG Str2FontFlags(const string& str)
{
	if ("FF_NORMAL" == str)
		return FF_NORMAL;
	else if ("FF_BOLD" == str)
		return FF_BOLD;
	else if ("FF_ITALIC" == str)
		return FF_ITALIC;
	else if ("FF_BOLD_ITALIC" == str)
		return FF_BOLD_ITALIC;
	else
		return FF_NORMAL;

}

string FontFlags2Str(const FONT_FLAG& flag)
{
	switch (flag)
	{
	case FF_NORMAL:
		return "FF_NORMAL";
		break;
	case FF_BOLD:
		return "FF_BOLD";
		break;
	case FF_ITALIC:
		return "FF_ITALIC";
		break;
	case FF_BOLD_ITALIC:
		return "FF_BOLD_ITALIC";
		break;
	default:
		return "FF_NORMAL";
	}
}


MyFont::MyFont(int id, string strFontName, DWORD hwHeight, FONT_FLAG eFlags)
{
	m_strFontName = strFontName;
	m_dwFontHeight = hwHeight;
	m_eFontFlags = eFlags;

	m_ID = id;
	m_pFont = NULL;
}

MyFont::~MyFont()
{
	SAFE_RELEASE(m_pFont);
}

HRESULT MyFont::InitDeviceObjects(LPDIRECT3DDEVICE9 pd3dDevice)
{
	HDC hDC = NULL;

	hDC = CreateCompatibleDC(NULL);
	SetMapMode(hDC, MM_TEXT);

	// Create a font.  By specifying ANTIALIASED_QUALITY, we might get an
	// antialiased font, but this is not guaranteed.
	int nHeight = -MulDiv(m_dwFontHeight,
		(INT)(GetDeviceCaps(hDC, LOGPIXELSY)),
		72);
	DWORD dwBold = (m_eFontFlags & D3DFONT_BOLD) ? FW_BOLD : FW_NORMAL;
	DWORD dwItalic = (m_eFontFlags & D3DFONT_ITALIC) ? TRUE : FALSE;

	D3DXFONT_DESC ft;
	ft.Height = nHeight;
	ft.Width = 0;
	ft.Weight = dwBold;
	ft.Italic = dwItalic;
	ft.CharSet = DEFAULT_CHARSET;
	ft.Quality = DEFAULT_QUALITY;
	ft.PitchAndFamily = 0;
	strcpy(ft.FaceName, "Times New Roman");

	DeleteDC(hDC);
	SAFE_RELEASE(m_pFont);
	if (FAILED(D3DXCreateFontIndirect(pd3dDevice, &ft, &m_pFont)))
	{
		MessageBox(0, "D3DXCreateFontIndirect() - FAILED", 0, 0);
		PostQuitMessage(0);
	}

	return S_OK;
}

HRESULT MyFont::DeleteDeviceObjects()
{
	m_pFont->OnLostDevice();
	SAFE_RELEASE(m_pFont);

	return S_OK;
}

int MyFont::DrawText(const string& str, const MyColor& Color, int left, int top, int right, int bottom, ALIGN_FORMAT format)
{
	RECT rect;
	rect.left = left;
	rect.top = top;
	rect.right = right;
	rect.bottom = bottom;

	return m_pFont->DrawTextA(NULL, str.c_str(), -1, &rect, format, Color.GetAsARGB());

}