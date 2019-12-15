// MyGUIScrollBar.cpp: implementation of the MyGUIScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#include "MyGUIScrollBar.h"
#include "MyUtil.h"
#include "MyMouse.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyGUIScrollBar::MyGUIScrollBar(MyGUIElementBase * parent, int myid, string myname,
		int offsetX, int offsetY, int width, int height, bool enable, bool visible)
		: MyGUIElementBase(parent, myid, myname, offsetX, offsetY, width, height, enable, visible)
{
	m_pLabelValue = NULL;
	m_fValue = 0;

	_bInControl = false;
}

MyGUIScrollBar::~MyGUIScrollBar()
{
	FinalCleanUp();	
}


void MyGUIScrollBar::FinalCleanUp()
{
	SAFE_DELETE(m_pPicBarBegin);
	SAFE_DELETE(m_pPicBarMiddle);
	SAFE_DELETE(m_pPicBarEnd);
	SAFE_DELETE(m_pPicTarget);
	
	SAFE_DELETE(m_pLabelValue);
}




void MyGUIScrollBar::SetPosition(int offsetXFromParent, int offsetYFromParent)
{
	MyGUIElementBase::SetPosition(offsetXFromParent, offsetYFromParent);
	m_pPicBarMiddle->Move(0,0);
	m_pPicBarBegin->Move(0,0);
	m_pPicBarEnd->Move(0,0);
	m_pPicTarget->Move(0,0);
	if (m_pLabelValue)
		m_pLabelValue->Move(0,0);
}



void MyGUIScrollBar::Move(int offsetx, int offsety)
{
	MyGUIElementBase::Move(offsetx,offsety);
	m_pPicBarMiddle->Move(0,0);
	m_pPicBarBegin->Move(0,0);
	m_pPicBarEnd->Move(0,0);
	m_pPicTarget->Move(0,0);
	if (m_pLabelValue)
		m_pLabelValue->Move(0,0);
}


HRESULT MyGUIScrollBar::Render()
{
	if(m_bVisible)
	{
		HRESULT r = S_OK;
		r |= m_pPicBarBegin->Render();
		r |= m_pPicBarEnd->Render();
		r |= m_pPicBarMiddle->Render();
		r |= m_pPicTarget->Render();

		if (m_pLabelValue)
			r |= m_pLabelValue->Render();

		return r;
	}
	else
		return S_FALSE;
}




float MyGUIScrollBar::GetValue()
{
	return m_fValue;
}


HRESULT MyGUIScrollBar::Update(float deltaTime)
{
	return S_OK;
}


void MyGUIScrollBar::SetPictureBox(PicIndex index, MyGUIImage *pic)
{
	switch(index) {
	case BAR_BEGIN:
		SAFE_DELETE(m_pPicBarBegin);
		m_pPicBarBegin = pic;
		break;
	case BAR_END:
		SAFE_DELETE(m_pPicBarEnd);
		m_pPicBarEnd = pic;
		break;
	case BAR_MIDDLE:
		SAFE_DELETE(m_pPicBarMiddle);
		m_pPicBarMiddle = pic;
		break;
	case BAR_TARGET:
		SAFE_DELETE(m_pPicTarget);
		m_pPicTarget = pic;
		break;
	}
}

MyGUIImage* MyGUIScrollBar::GetImage(PicIndex index)
{
	switch (index) {
	case BAR_BEGIN:
		return m_pPicBarBegin;
		break;
	case BAR_END:
		return m_pPicBarEnd;
		break;
	case BAR_MIDDLE:
		return m_pPicBarMiddle;
		break;
	case BAR_TARGET:
		return m_pPicTarget;
		break;
	default:
		return NULL;
	}
}

void MyGUIScrollBar::SetValue(float value)
{
	Clamp(value, 0, 1);
	m_fValue = value;
	_UpdateTarget();
}

