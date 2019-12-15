// MyGUIButtonBase.cpp: implementation of the MyGUIButtonBase class.
//
//////////////////////////////////////////////////////////////////////

#include "MyGUIButtonBase.h"
#include "MyGUIManager.h"
#include "MyGUIImage.h"
#include "MyGUILabel.h"
#include "MyUtil.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
MyGUIButtonBase::ButtonState MyGUIButtonBase::Str2ButtonState(const string& str)
{
	if (str == "BTS_NORMAL")
		return BTS_NORMAL;
	else if (str == "BTS_MOUSEOVER")
		return BTS_MOUSEOVER;
	else if (str == "BTS_BUTTONDOWN")
		return BTS_BUTTONDOWN;
	else if (str == "BTS_DISABLE")
		return BTS_DISABLE;
	else 
		return BTS_NORMAL;
}

string MyGUIButtonBase::ButtonState2Str(const ButtonState& state)
{
	if (state == BTS_NORMAL)
		return "BTS_NORMAL";
	else if (state == BTS_MOUSEOVER)
		return "BTS_MOUSEOVER";
	else if (state == BTS_BUTTONDOWN)
		return "BTS_BUTTONDOWN";
	else if (state == BTS_DISABLE)
		return "BTS_DISABLE";
	else 
		return "BTS_NORMAL";
}


MyGUIButtonBase::MyGUIButtonBase(MyGUIElementBase * parent, int myid, string myname,
		string imageNormal, string imageOver, string imageButtonDown, string imageDisabled, 
		string strText, int offsetX, int offsetY, int width, int height, 
		 bool visible, 	int tabindex, bool tabenable) 
		: MyGUIElementBase(parent, myid, myname, offsetX, offsetY, 
		width, height, true, visible, tabindex, tabenable)
		, m_pLabel(NULL)
{
	assert(!imageNormal.empty());

	int id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	string name = MyUTIL::IntToStr(id);
	m_pImgNormal = new MyGUIImage(this, id, name, imageNormal, true, MyColor::White, 0, 0, width, height, MyColor::White);

	if (!imageOver.empty())
	{
		id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
		name = MyUTIL::IntToStr(id);
		m_pImgMouseOver = new MyGUIImage(this, id, name, imageOver, true, MyColor::White, 0, 0, width, height,MyColor::White);
	}
	else
		m_pImgMouseOver= NULL;

	if (!imageButtonDown.empty())
	{
		id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
		name = MyUTIL::IntToStr(id);
		m_pImgButtonDown = new MyGUIImage(this, id, name, imageButtonDown, true, MyColor::White, 0, 0, width, height, MyColor::White);
	}
	else
		m_pImgButtonDown = NULL;

	if (!imageDisabled.empty())
	{
		id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
		name = MyUTIL::IntToStr(id);
		m_pImgButtonDisable = new MyGUIImage(this, id, name, imageDisabled, true, MyColor::White, 0, 0, width, height, MyColor::White);
	}
	else
		m_pImgButtonDisable = NULL;
	
	SetLabelText(strText);

	m_ButtonState = BTS_NORMAL;
}


MyGUIButtonBase::~MyGUIButtonBase()
{
	FinalCleanUp();
}

void MyGUIButtonBase::SetLocation(sLocation lt)
{
	MyGUIElementBase::SetLocation(lt);
	m_pImgNormal->SetLocation(lt);
	if (m_pImgButtonDown)
		m_pImgButtonDown->SetLocation(lt);
	if (m_pImgButtonDisable)
		m_pImgButtonDisable->SetLocation(lt);
	if (m_pImgMouseOver)
		m_pImgMouseOver->SetLocation(lt);

	if (m_pLabel)
		m_pLabel->SetLocation(lt);
}

void MyGUIButtonBase::SetPosition(int offestfromparentx, int offestfromparenty)
{
	MyGUIElementBase::SetPosition(offestfromparentx, offestfromparenty);
	m_pImgNormal->Move(0, 0);
	if (m_pImgButtonDown)
		m_pImgButtonDown->Move(0, 0);
	if (m_pImgButtonDisable)
		m_pImgButtonDisable->Move(0, 0);
	if (m_pImgMouseOver)
		m_pImgMouseOver->Move(0, 0);

	if (m_pLabel)
		m_pLabel->Move(0, 0);
}
void MyGUIButtonBase::Move(int offsetfromparentx, int offsetfromparenty)
{
	MyGUIElementBase::Move(offsetfromparentx, offsetfromparenty);
	m_pImgNormal->Move(0, 0);
	if (m_pImgButtonDown)
		m_pImgButtonDown->Move(0, 0);
	if (m_pImgButtonDisable)
		m_pImgButtonDisable->Move(0, 0);
	if (m_pImgMouseOver)
		m_pImgMouseOver->Move(0, 0);

	if (m_pLabel)
		m_pLabel->Move(0, 0);
}

void MyGUIButtonBase::SetSize(int width, int height)
{
	MyGUIElementBase::SetSize(width, height);
	m_pImgNormal->SetSize(width, height);
	if (m_pImgButtonDown)
		m_pImgButtonDown->SetSize(width, height);
	if (m_pImgButtonDisable)
		m_pImgButtonDisable->SetSize(width, height);
	if (m_pImgMouseOver)
		m_pImgMouseOver->SetSize(width, height);

	if (m_pLabel)
		m_pLabel->SetSize(width, height);
}

HRESULT MyGUIButtonBase::Render()
{
	if (m_bVisible)
	{
		HRESULT r = GetImage()->Render();
		if (m_pLabel != NULL)
			r |= m_pLabel->Render();
		return r;
	}
	else
		return S_FALSE;
}

void MyGUIButtonBase::FinalCleanUp()
{
	SAFE_DELETE(m_pImgNormal);
	SAFE_DELETE(m_pImgButtonDown);
	SAFE_DELETE(m_pImgButtonDisable);
	SAFE_DELETE(m_pImgMouseOver);
	SAFE_DELETE(m_pLabel);
}


void MyGUIButtonBase::SetImage(const ButtonState& state, const string& image, const MyColor& colorKey)
{
	MyGUIImage* img = NULL;
	switch(state) {
	case BTS_NORMAL:
		img = m_pImgNormal;
		break;
	case BTS_MOUSEOVER:
		img = m_pImgMouseOver;
		break;
	case BTS_DISABLE:
		img = m_pImgButtonDisable;
		break;
	case BTS_BUTTONDOWN:
		img = m_pImgButtonDown;
		break;
	default:
		return;
	}

	if (img)
	{
		img->ResetImage(image, colorKey);
		img->SetUVMap(0,0,1,1);
	}
	else
	{
		int id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
		string name = MyUTIL::IntToStr(id);
		img = new MyGUIImage(this, id, name, image, true, colorKey, 0, 0, m_Size.x, m_Size.y,MyColor::White);
	}
}
void MyGUIButtonBase::SetImage(const ButtonState& state, MyGUIImage *pic)
{
	switch (state) {
	case BTS_NORMAL:
		SAFE_DELETE(m_pImgNormal);
		m_pImgNormal = pic;
		break;
	case BTS_MOUSEOVER:
		SAFE_DELETE(m_pImgMouseOver);
		m_pImgMouseOver = pic;
		break;
	case BTS_BUTTONDOWN:
		SAFE_DELETE(m_pImgButtonDown);
		m_pImgButtonDown = pic;
		break;
	case BTS_DISABLE:
		SAFE_DELETE(m_pImgButtonDisable);
		m_pImgButtonDisable = pic;
		break;
	}
	pic->SetPosition(0, 0);
	pic->SetSize(GetSize().x, GetSize().y);
}
void MyGUIButtonBase::SetImage(const string& image, float imageLength, float imageHeight,
	float btnLength, float btnHeight, const MyColor& colorKey)

{
	SetImage(BTS_NORMAL, image, colorKey);
	SetImage(BTS_MOUSEOVER, image, colorKey);
	SetImage(BTS_BUTTONDOWN, image, colorKey);
	SetImage(BTS_DISABLE, image, colorKey);

	m_pImgNormal->SetUVMap(btnLength / imageLength * 0, btnHeight / imageHeight * 0, btnLength / imageLength * 1, btnHeight / imageHeight * 1);
	m_pImgMouseOver->SetUVMap(btnLength / imageLength * 1, btnHeight / imageHeight * 1, btnLength / imageLength * 2, btnHeight / imageHeight * 2);
	m_pImgButtonDown->SetUVMap(btnLength / imageLength * 2, btnHeight / imageHeight * 2, btnLength / imageLength * 3, btnHeight / imageHeight * 3);
	m_pImgButtonDisable->SetUVMap(btnLength / imageLength * 3, btnHeight / imageHeight * 3, btnLength / imageLength * 4, btnHeight / imageHeight * 4);
}

MyGUIImage* MyGUIButtonBase::GetImage(const ButtonState& state) const
{
	switch(state) {
	case BTS_NORMAL:
		return m_pImgNormal;
		break;
	case BTS_MOUSEOVER:
		return m_pImgMouseOver;
		break;
	case BTS_DISABLE:
		return m_pImgButtonDisable;
		break;
	case BTS_BUTTONDOWN:
		return m_pImgButtonDown;
		break;
	default:
		return NULL;
	}
}
MyGUIImage* MyGUIButtonBase::GetImage() const
{
	switch(m_ButtonState) {
	case BTS_NORMAL:
		return m_pImgNormal;
		break;
	case BTS_MOUSEOVER:
		if (m_pImgMouseOver) return m_pImgMouseOver;
		else return m_pImgNormal;
		break;
	case BTS_DISABLE:
		if (m_pImgButtonDisable) return m_pImgButtonDisable;
		else return m_pImgNormal;
		break;
	case BTS_BUTTONDOWN:
		if (m_pImgButtonDown) return m_pImgButtonDown;
		else return m_pImgNormal;
		break;
	default:
		return NULL;
	}
}


void MyGUIButtonBase::SetLabelText(const string& text)
{
	if (!text.empty())
	{
		if (m_pLabel != NULL)
		{
			m_pLabel->SetText(text);
		}
		else
		{
			int id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
			string name = MyUTIL::IntToStr(id);
			m_pLabel = new MyGUILabel(this, id, name, text, 0, 0, m_Size.x, m_Size.y);
			SetLabelAlignMode(AF_VMD_CTR);
		}
	}
	else
	{
		SAFE_DELETE(m_pLabel);

	}
}
void MyGUIButtonBase::SetLabelTextColor(const MyColor& colorkey)
{
	if(m_pLabel)
		m_pLabel->SetTextColor(colorkey);	
}
void MyGUIButtonBase::SetLabelPosition(int lefttopx, int lefttopy)
{
	if(m_pLabel)
		m_pLabel->SetPosition(lefttopx, lefttopy);
}
void MyGUIButtonBase::SetLabelSize(int width, int height)
{
	
	if(m_pLabel)
		m_pLabel->SetSize(width, height);
}
void MyGUIButtonBase::SetLabelAlignMode(const ALIGN_FORMAT& format)
{
	if(m_pLabel)
		m_pLabel->SetFormat(format);
}
void MyGUIButtonBase::SetLabel(MyGUILabel * label)
{
	SAFE_DELETE(m_pLabel);
	m_pLabel = label;
}