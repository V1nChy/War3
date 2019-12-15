// MyGUIScrollBarHori.cpp: implementation of the MyGUIScrollBarHori class.
//
//////////////////////////////////////////////////////////////////////

#include "MyGUIScrollBarHori.h"
#include "MyUtil.h"
#include "MyMouse.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyGUIScrollBarHori::MyGUIScrollBarHori(MyGUIElementBase * parent, int myid, string myname,
		int offsetX, int offsetY, int width, int height, bool enable, bool visible)
		:MyGUIScrollBar(parent, myid, myname, offsetX, offsetY, width, height, enable, visible)
{
	m_eType = GUI_ScrollBarHori;
	
	int id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	string name = MyUTIL::IntToStr(id);
	m_pPicBarBegin  = new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", false, MyColor::White, 0, 0, 6, 6);
	m_pPicBarBegin->SetUVMap(113.f/256, 19.f/256, 119.f/256, 25.f/256);

	id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	name = MyUTIL::IntToStr(id);
	m_pPicBarEnd  = new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", false, MyColor::White, 0, 0, 6, 6);
	m_pPicBarEnd->SetUVMap(113.f/256, 19.f/256, 119.f/256, 25.f/256);
	
	id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	name = MyUTIL::IntToStr(id);
	m_pPicBarMiddle	= new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", false, MyColor::White, 0, 0, width-m_pPicBarBegin->GetSize().x-m_pPicBarEnd->GetSize().x , 2);
	m_pPicBarMiddle->SetUVMap(154.f/256, 68.f/256, 162.f/256, 69.f/256);


	id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	name = MyUTIL::IntToStr(id);
	m_pPicTarget = new MyGUIImage(this, id, name, "Texture/GUI-OGRE.tga", true, MyColor::White, 0, 0, 22, 7);
	m_pPicTarget->SetUVMap(233.f/256, 96.f/256, 255.f/256, 103.f/256);

	SetSize(width, height);
}

MyGUIScrollBarHori::~MyGUIScrollBarHori()
{
	
}



void MyGUIScrollBarHori::SetSize(int width, int height)
{
	MyGUIScrollBar::SetSize(width, height);
	
	m_pPicBarBegin->SetPosition( 0, (height - m_pPicBarBegin->GetSize().y)/2 );
	m_pPicBarEnd->SetPosition( width - m_pPicBarEnd->GetSize().x, 
		(height - m_pPicBarEnd->GetSize().y)/2 );
	m_pPicBarMiddle->SetPosition( m_pPicBarBegin->GetSize().x, (height - m_pPicBarMiddle->GetSize().y)/2 ); 
	m_pPicBarMiddle->SetSize(width - m_pPicBarBegin->GetSize().x - m_pPicBarEnd->GetSize().x, 
		m_pPicBarMiddle->GetSize().y );

	m_pPicTarget->SetPosition( m_pPicBarBegin->GetSize().x, 
		(height - m_pPicTarget->GetSize().y)/2 );

	if (m_pLabelValue)
	{
		m_pLabelValue->SetPosition( width + 5, 0 );
		m_pLabelValue->SetSize(800, height);
	}
}


HRESULT MyGUIScrollBarHori::Update(float deltaTime)
{
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
			_fValueBegin= m_fValue;
			return S_OK;
		}
	}
	else if (MyMouse::GetSingleton()->IsMouseKeyDown(0, false))
	{
		if (_bInControl)	// 之前按下，且一直没松开的情况下
		{
			int xnow = MyMouse::GetSingleton()->GetPos().x;
			float lengthOfBar = m_pPicBarMiddle->GetSize().x - m_pPicTarget->GetSize().x;
			float increased = (xnow - _pPosBegin.x)/lengthOfBar;
			float value = _fValueBegin + increased;
			Clamp(value, 0, 1);
			increased = value - _fValueBegin;
			m_fValue = value;	// Update value
			m_pPicTarget->SetPosition(m_pPicBarBegin->GetSize().x + m_fValue * lengthOfBar, 
				(GetSize().y - m_pPicTarget->GetSize().y)/2);	// update target pic
			return S_OK;
		}
	}
	else	// 鼠标左键没按下，那么一定失去控制了
		_bInControl = false;

	return S_FALSE;
}

void MyGUIScrollBarHori::SetWidth(int width)
{
	SetSize(width, m_Size.y);
}

void MyGUIScrollBarHori::_UpdateTarget()
{
	int lengthOfBar = m_pPicBarMiddle->GetSize().x - m_pPicTarget->GetSize().x;
	m_pPicTarget->SetPosition(m_pPicBarMiddle->GetOffsetFromParent().x + lengthOfBar*m_fValue, m_pPicTarget->GetOffsetFromParent().y);
}

