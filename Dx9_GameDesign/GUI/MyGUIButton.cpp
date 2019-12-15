// MyGUIButton.cpp: implementation of the MyGUIButton class.
//
//////////////////////////////////////////////////////////////////////

#include "MyGUIButton.h"
#include "MyUTIL.h"
#include "MyMouse.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyGUIButton::MyGUIButton(MyGUIElementBase * parent, int id, const string& name, int offsetX, int offsetY, int width, int height,
						 const string& imageNormal, const string& imageOver, const string& imageButtonDown, 
						 const string& imageDisable, const string& strText, 
						 bool visible, int tabindex, bool tabenable, bool bPushed) 
		 : MyGUIButtonBase (parent, id, name, imageNormal, imageOver, imageButtonDown,
		 imageDisable, strText, offsetX, offsetY, width, height, visible, tabindex, tabenable)
		 ,m_pEventHandler(NULL), m_pBtnSound(NULL), _bOnBtn(false)
{
	m_eType = GUI_Button;

	m_pImgNormal->SetUVMap(0, 0, 1, 0.25f);
	m_pImgMouseOver->SetUVMap(0, 0.25f, 1, 0.5f);
	m_pImgButtonDown->SetUVMap(0, 0.5f, 1, 0.75f);
	m_pImgButtonDisable->SetUVMap(0, 0.75f, 1, 1);
}


MyGUIButton::~MyGUIButton()
{
	SAFE_DELETE(m_pEventHandler);
	SetBtnSoundNull();
}


void MyGUIButton::SetBtnSound(const string& sound)
{
	if (m_pBtnSound != NULL)
	{
		/*if (m_pBtnSound->GetOrigin() != sound)
		{
			GESoundManager::GetSingleton().RemoveResource(m_pBtnSound->GetHandle());
		}
		else
		{
			return;
		}*/
	}

	//m_pBtnSound = GESoundManager::GetSingleton().Create2DSound(sound);
}


void MyGUIButton::SetBtnSoundNull()
{
	if (m_pBtnSound != NULL)
	{
		//GESoundManager::GetSingleton().RemoveResource(m_pBtnSound->GetHandle());
		m_pBtnSound = NULL;
	}
}


HRESULT MyGUIButton::Update(float deltaTime)
{
	if (!m_bVisible)
		return S_OK;

	if(GetButtonState() != BTS_DISABLE)
	{
		if(CheckMousePosition() || m_ButtonState== BTS_BUTTONDOWN)
		{
			switch(GetButtonState())
			{
			case BTS_NORMAL:
				if ( !MyMouse::GetSingleton()->IsMouseKeyDown(0, false) )
				{
					SetButtonStatus(BTS_MOUSEOVER);
					if (!_bOnBtn)
					{
						_bOnBtn = true;
						/*if (m_pBtnSound)
							m_pBtnSound->PlayMusic(GESound::RESUME_ONCE);*/
					}
				}
				break;
			case BTS_MOUSEOVER:
				if (MyMouse::GetSingleton()->IsMouseKeyDown(0, true))
				{
					SetButtonStatus(BTS_BUTTONDOWN);

					if (m_pBehaviour)
						m_pBehaviour->Execute1();
				}
				break;
			case BTS_BUTTONDOWN:
				if (MyMouse::GetSingleton()->IsMouseKeyUp(0, true) )
				{
					SetButtonStatus(BTS_MOUSEOVER);

					if (m_pBehaviour)
						m_pBehaviour->Execute2();
				}
				break;
			default:
				break;
			}
		}
		else
		{
			this->SetButtonStatus(BTS_NORMAL);
			_bOnBtn = false;
		}
	}
	
	return MyGUIElementBase::Update(deltaTime);

}

bool MyGUIButton::CheckMousePosition()
{
	if	(MyUTIL::DotInRect(MyMouse::GetSingleton()->GetX(), MyMouse::GetSingleton()->GetY(),
		GetPosition().x, GetPosition().y, GetSize().x, GetSize().y))
		return true;
	else
		return false;

}

void MyGUIButton::BindFunction(void (*func)())
{
	SAFE_DELETE(m_pEventHandler);
	m_pEventHandler = CreateStaticEventHandler(func);
}


