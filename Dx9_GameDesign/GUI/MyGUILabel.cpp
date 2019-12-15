#include "MyGUILabel.h"
#include "MyUTIL.h"
//#include "GEMouse.h"

MyGUILabel::MyGUILabel(MyGUIElementBase * parent, int id, string name, string text,
					   int offsetX, int offsetY, int width, int height, MyColor color, bool visible)
					   : MyGUIElementBase(parent, id, name, offsetX, offsetY, width, height, false, visible)
{
	m_eType = GUI_Label;

	m_Color = color;
	m_iFontID = MyFontManager::GetSingleton()->m_pNormalFont->GetID();
	m_sText = text;
	m_Format = AF_VMD_LEFT;
}


void MyGUILabel::SetText(const string& text)
{
	m_sText = text;
}

void MyGUILabel::SetTextColor(const MyColor& color)
{
	m_Color = color;
}

void MyGUILabel::SetFont(const string& fontName, UINT height, FONT_FLAG eflags)
{
	if ( MyFontManager::GetSingleton()->IsFontExist(fontName, height, eflags) )
		m_iFontID = MyFontManager::GetSingleton()->GetFont(fontName, height, eflags)->GetID();
	else
	{
		m_iFontID = MyFontManager::GetSingleton()->AddNewFont(fontName, height, eflags);
	}
}

void MyGUILabel::SetFont(int fontid)
{
	if (MyFontManager::GetSingleton()->IsFontExist(fontid))
		m_iFontID = fontid;
}

HRESULT MyGUILabel::Render()
{
	if (m_bVisible)
	{
		MyFont * font = MyFontManager::GetSingleton()->GetFont(m_iFontID);
		font->DrawText(m_sText, m_Color, m_Position.x, m_Position.y, m_Position.x+m_Size.x, m_Position.y+m_Size.y, m_Format);
	}

	return S_OK;
}



void MyGUILabel::SetFormat(ALIGN_FORMAT format)
{
	m_Format = format;
}


HRESULT MyGUILabel::Update(float deltaTime)
{
	

	return MyGUIElementBase::Update(deltaTime);
}

const string& MyGUILabel::GetText() const
{
	return m_sText;
}
ALIGN_FORMAT MyGUILabel::GetFormat() const
{
	return this->m_Format;
}
const MyColor& MyGUILabel::GetTextColor() const
{
	return m_Color;
}
const string& MyGUILabel::GetFontName() const
{ 
	return MyFontManager::GetSingleton()->GetFont(m_iFontID)->GetName();
}
int MyGUILabel::GetFontHeight() const
{
	return MyFontManager::GetSingleton()->GetFont(m_iFontID)->GetFontHeight();
}
FONT_FLAG MyGUILabel::GetFontFlags() const
{
	return MyFontManager::GetSingleton()->GetFont(m_iFontID)->GetFlags();
}


void MyGUILabel::SetAlpha(float alpha)
{
	assert(alpha >=0 && alpha <= 1 && "alpha必须在0～1之间");
	m_Color.a = alpha;
}
