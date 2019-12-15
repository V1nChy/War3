#include "MyGUICheckBox.h"
#include "MyUtil.h"
#include "MyMouse.h"
#include "Point2D.h"


MyGUICheckBox::MyGUICheckBox(MyGUIElementBase * parent, int id, const string& name,
    	 int offsetX, int offsetY, int width, int height, 
		const string& imageNormal, const string& imageButtonDown,
		const string& imageOver, const string& imageDisabled, const string& strText, 
		bool visible, int tabindex, bool tabenable, bool bChecked)
		:MyGUIButtonBase (parent, id, name, imageNormal, imageOver, imageButtonDown,
		 imageDisabled, strText, offsetX, offsetY, width, height, visible, tabindex, tabenable)
{
	m_bChecked = bChecked;
	m_eType = GUI_CheckBox;

	m_pImgNormal->SetUVMap(41.f/256, 109.f/256, 57.f/256, 125.f/256);
	m_pImgButtonDown->SetUVMap(41.f/256, 146.f/256, 57.f/256, 162.f/256);
	m_pImgMouseOver->SetUVMap(41.f / 256, 109.f / 256, 57.f / 256, 125.f / 256);
	m_pImgButtonDisable->SetUVMap(41.f / 256, 109.f / 256, 57.f / 256, 125.f / 256);

	SetCheckBoxElementSize();      //The order to call these two function must
	SetCheckBoxElementPosition();  //be like this: first call the "...size"function then the "...position"function.
	
}

void MyGUICheckBox::SetCheckBoxElementPosition()
{
	m_pImgNormal->SetPosition(0, (m_Size.y-MINCHECKBOXHEIGHT)/2);
	m_pImgButtonDown->SetPosition(0, (m_Size.y-MINCHECKBOXHEIGHT)/2);
	m_pImgMouseOver->SetPosition(0, (m_Size.y - MINCHECKBOXHEIGHT) / 2);
	m_pImgButtonDisable->SetPosition(0, (m_Size.y - MINCHECKBOXHEIGHT) / 2);
	m_pLabel->SetPosition(MINCHECKBOXWIDTH+INTERVAL, 0);
	m_pLabel->SetFormat(AF_VMD_LEFT);
}

void MyGUICheckBox::SetCheckBoxElementSize()
{
	if(m_Size.x < MINCHECKBOXWIDTH)
		m_Size.x = MINCHECKBOXWIDTH+INTERVAL;

	if(m_Size.y <MINCHECKBOXHEIGHT)
		m_Size.y = MINCHECKBOXHEIGHT;

	SetCheckBoxElementPosition();

	m_pImgNormal->SetSize(MINCHECKBOXWIDTH, MINCHECKBOXHEIGHT);
	m_pImgButtonDown->SetSize(MINCHECKBOXWIDTH, MINCHECKBOXHEIGHT);
	m_pImgMouseOver->SetSize(MINCHECKBOXWIDTH, MINCHECKBOXHEIGHT);
	m_pImgButtonDisable->SetSize(MINCHECKBOXWIDTH, MINCHECKBOXHEIGHT);
	m_pLabel->SetSize(m_Size.x-MINCHECKBOXWIDTH-INTERVAL, m_Size.y);
}

bool MyGUICheckBox::CheckMousePosition()
{
	if	(MyUTIL::DotInRect(MyMouse::GetSingleton()->GetX(), MyMouse::GetSingleton()->GetY(),
		m_pImgNormal->GetPosition().x, m_pImgNormal->GetPosition().y, m_pImgNormal->GetSize().x, m_pImgNormal->GetSize().y))
		return true;
	else
		return false;

}

bool MyGUICheckBox::IsChecked()
{
	return m_bChecked;
}

void MyGUICheckBox::SetCheck(bool b)
{
	m_bChecked = b;
	if(m_bChecked)
		SetButtonStatus(BTS_BUTTONDOWN);
	else 
		SetButtonStatus(BTS_NORMAL);

}

void MyGUICheckBox::Move(int offsetx, int offsety)
{
	MyGUIButtonBase::Move(offsetx,offsety);
	SetCheckBoxElementPosition();
}

void MyGUICheckBox::SetPosition(int offsetFromParentX, int offsetFromParentY)
{
	MyGUIButtonBase::SetPosition(offsetFromParentX, offsetFromParentY);
	SetCheckBoxElementPosition();
}

void MyGUICheckBox::SetSize(int width, int height)
{
	MyGUIButtonBase::SetSize(width, height);
	SetCheckBoxElementSize();
}



HRESULT MyGUICheckBox::Update(float deltaTime)
{
	if(this->GetButtonState() != BTS_DISABLE)
	{
		if (CheckMousePosition())
		{
			switch (GetButtonState())
			{
			case BTS_NORMAL:
				if (MyMouse::GetSingleton()->IsMouseKeyDown(0, true))
					SetButtonStatus(BTS_MOUSEOVER);
				break;
			case BTS_MOUSEOVER:
				if (MyMouse::GetSingleton()->IsMouseKeyUp(0, true))
				{
					m_bChecked = !m_bChecked;
					if (IsChecked())
						SetButtonStatus(BTS_BUTTONDOWN);
					else
						SetButtonStatus(BTS_NORMAL);
				}
				break;
			case BTS_BUTTONDOWN:
				if (MyMouse::GetSingleton()->IsMouseKeyDown(0, true))
					SetButtonStatus(BTS_MOUSEOVER);
				break;
			default:
				break;
			}
		}
		else
		{
			if (IsChecked())
				SetButtonStatus(BTS_BUTTONDOWN);
			else
				SetButtonStatus(BTS_NORMAL);
		}
			/*if(CheckMousePosition())
			{
				m_bChecked = !m_bChecked;
			}

			if(IsChecked())
				SetButtonStatus(BTS_BUTTONDOWN);
			else 
				SetButtonStatus(BTS_NORMAL);*/
	}

	return S_OK;
}

HRESULT MyGUICheckBox::Render()
{
	if(m_bVisible)
	{
		m_pImgNormal->Render();
		HRESULT r = GetImage()->Render();
		r |= m_pLabel->Render();

		return r;
	}
	else
		return S_OK;
}
