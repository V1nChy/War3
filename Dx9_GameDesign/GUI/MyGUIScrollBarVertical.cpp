// MyGUIScrollBarVertical.cpp: implementation of the MyGUIScrollBarVertical class.
//
//////////////////////////////////////////////////////////////////////

#include "MyGUIScrollBarVertical.h"
#include "MyUtil.h"
#include "MyMouse.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyGUIScrollBarVertical::MyGUIScrollBarVertical(MyGUIElementBase * parent, int myid, string myname,
		int offsetX, int offsetY, int width, int height, bool enable, bool visible)
		:MyGUIScrollBar(parent, myid, myname, offsetX, offsetY, width, height, enable, visible)
{
	m_eType = GUI_ScrollBarVerti;

	int id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	string name = MyUTIL::IntToStr(id);
	m_pPicBarBegin  = new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", false, MyColor::White, 0, 0, 6, 6);
	m_pPicBarBegin->SetUVMap(113.f/256, 19.f/256, 119.f/256, 25.f/256);

	id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	name = MyUTIL::IntToStr(id);
	m_pPicBarEnd	= new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", false, MyColor::White, 0, 0, 6, 6);
	m_pPicBarEnd->SetUVMap(113.f/256, 19.f/256, 119.f/256, 25.f/256);
	
	id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	name = MyUTIL::IntToStr(id);
	m_pPicBarMiddle	= new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", false, MyColor::White, 0, 0, 2, height-m_pPicBarBegin->GetSize().y-m_pPicBarEnd->GetSize().y);
	m_pPicBarMiddle->SetUVMap(6.f/256, 94.f/256, 7.f/256, 116.f/256);


	id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	name = MyUTIL::IntToStr(id);
	m_pPicTarget = new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", true, MyColor::White, 0, 0, 7, 22);
	m_pPicTarget->SetUVMap(214.f/256, 60.f/256, 221.f/256, 82.f/256);

	SetSize(width, height);
}

MyGUIScrollBarVertical::~MyGUIScrollBarVertical()
{
	
}



void MyGUIScrollBarVertical::SetSize(int width, int height)
{
	MyGUIScrollBar::SetSize(width, height);
	
	m_pPicBarBegin->SetPosition( (width - m_pPicBarBegin->GetSize().x)/2, 0 );
	m_pPicBarEnd->SetPosition( (width - m_pPicBarBegin->GetSize().x)/2, 
		height - m_pPicBarEnd->GetSize().y );
	m_pPicBarMiddle->SetPosition( (width - m_pPicBarMiddle->GetSize().x)/2,
		m_pPicBarBegin->GetSize().y ); 
	m_pPicBarMiddle->SetSize( m_pPicBarMiddle->GetSize().x,
		height - m_pPicBarBegin->GetSize().y - m_pPicBarEnd->GetSize().y );

	m_pPicTarget->SetPosition( (width - m_pPicTarget->GetSize().x)/2,
		m_pPicBarBegin->GetSize().y );

	if (m_pLabelValue)
	{
		m_pLabelValue->SetPosition( 0, height + 5 );
		m_pLabelValue->SetSize(600, 10);
	}
}


HRESULT MyGUIScrollBarVertical::Update(float deltaTime)
{
	MyGUIScrollBar::Update(deltaTime);

	if (!m_bVisible || !m_bEnable) return S_FALSE;

	
	// 如果刚点到此区域
	if (MyMouse::GetSingleton()->IsMouseKeyDown(0, true))
	{
		if (MyUTIL::DotInRect(MyMouse::GetSingleton()->GetPos().x,
			MyMouse::GetSingleton()->GetPos().y,
							m_pPicTarget->GetPosition().x, m_pPicTarget->GetPosition().y,
							m_pPicTarget->GetSize().x, m_pPicTarget->GetSize().y))
		{
			_bInControl = true;
			_pPosBegin = MyMouse::GetSingleton()->GetPos();
			_fValueBegin = m_fValue;
			return S_OK;
		}
	}
	else if (MyMouse::GetSingleton()->IsMouseKeyDown(0, false))
	{
		if (_bInControl)	// 之前按下，且一直没松开的情况下
		{
			int ynow = MyMouse::GetSingleton()->GetPos().y;
			float lengthOfBar = m_pPicBarMiddle->GetSize().y - m_pPicTarget->GetSize().y;
			float increased = (ynow - _pPosBegin.y)/lengthOfBar;
			float value = _fValueBegin + increased;
			Clamp(value, 0, 1);
			increased = value - _fValueBegin;
			m_fValue = value;	// Update value
			m_pPicTarget->SetPosition( (GetSize().x- m_pPicTarget->GetSize().x)/2, 
				m_pPicBarBegin->GetSize().y + m_fValue * lengthOfBar );	// update target pic
			return S_OK;
		}
	}
	else	// 鼠标左键没按下，那么一定失去控制了
		_bInControl = false;

	return S_FALSE;
}

void MyGUIScrollBarVertical::SetHeight(int height)
{
	SetSize(m_Size.x, height);
}

void MyGUIScrollBarVertical::_UpdateTarget()
{
	int lengthOfBar = m_pPicBarMiddle->GetSize().y - m_pPicTarget->GetSize().y;
	m_pPicTarget->SetPosition(m_pPicTarget->GetOffsetFromParent().x, m_pPicBarMiddle->GetOffsetFromParent().y + lengthOfBar*m_fValue);
}

