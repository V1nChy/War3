
#include "MyGUIContainer.h"
#include "MyGUIElementBase.h"

MyGUIElementBase::MyGUIElementBase(MyGUIElementBase * parent, int id, string name, int offsetX, int offsetY, int width, int height,
	bool enable, bool visible, int tabindex, bool tabEnable, sLocation location)
{
	m_pParent = parent;
	m_nID = id;
	m_strName = name;

	SetPosition(offsetX, offsetY);
	SetSize(width, height);

	m_bEnable = enable;
	m_bVisible = visible;

	m_iShowLevel = 0;
	m_eType = GUI_Default;
	m_eLocatType = location;

	m_pBehaviour = NULL;
}

MyGUIElementBase::~MyGUIElementBase()
{
	if (m_pBehaviour)
		delete m_pBehaviour;
	m_pBehaviour = NULL;
}

HRESULT MyGUIElementBase::OneTimeInit()
{
	if(m_pBehaviour)
		m_pBehaviour->OneTimeInit();
	//	SetSize(m_Size.x, m_Size.y); 
	return S_OK;
}
HRESULT MyGUIElementBase::Update(float deltaTime)
{
	if (m_pBehaviour)
		m_pBehaviour->Update();
	return S_OK;
}

void MyGUIElementBase::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

void MyGUIElementBase::SetPosition(int offsetX, int offsetY)
{
	m_OffsetFromParent.x = offsetX*m_eLocatType.sX;
	m_OffsetFromParent.y = offsetY*m_eLocatType.sY;
	if (m_pParent)
		m_Position = m_pParent->GetPosition() + m_OffsetFromParent;
	else
		m_Position = m_OffsetFromParent;
}

void MyGUIElementBase::Move(int offsetx, int offsety)
{
	m_OffsetFromParent.x += offsetx*m_eLocatType.sX;
	m_OffsetFromParent.y += offsety*m_eLocatType.sY;
	if (m_pParent)
		m_Position = m_pParent->GetPosition() + m_OffsetFromParent;
	else
		m_Position = m_OffsetFromParent;
}

void MyGUIElementBase::SetSize(int width, int height)
{
	m_Size.x = width*m_eLocatType.sX;
	m_Size.y = height*m_eLocatType.sY;
}

void MyGUIElementBase::SetName(const string& name)
{
	m_strName = name;
}

void MyGUIElementBase::SetID(int id)
{
	m_nID = id;
}

void MyGUIElementBase::SetEnable(bool bEnable)
{
	m_bEnable = bEnable;
}

void MyGUIElementBase::SetParent(MyGUIElementBase* pParent)
{
	m_pParent = pParent;
	Move(0,0);
}

void MyGUIElementBase::SetShowLevel(int level)
{
	m_iShowLevel = level;
}


int MyGUIElementBase::GetShowLevel()
{
	return m_iShowLevel;
}

MyGUIElementType MyGUIElementBase::GetType()
{
	return m_eType;
}

bool MyGUIElementBase::IsVisible() const
{
	return m_bVisible;
}

bool MyGUIElementBase::IsEnable() const
{
	return m_bEnable;
}

const Position& MyGUIElementBase::GetPosition() const
{
	return m_Position;
}

const Position& MyGUIElementBase::GetOffsetFromParent() const
{
	return m_OffsetFromParent;
}

const Size& MyGUIElementBase::GetSize() const
{
	return m_Size;
}

const string& MyGUIElementBase::GetName() const
{
	return m_strName;
}

int MyGUIElementBase::GetID() const
{
	return m_nID;

}

MyGUIElementBase * MyGUIElementBase::GetParent()
{
	return m_pParent;
}

sLocation MyGUIElementBase::GetLocation() const
{
	return m_eLocatType;
}
void MyGUIElementBase::SetLocation(sLocation lt)
{
	m_eLocatType = lt;
}

MyBeHaviour* MyGUIElementBase::SetBehaviour(MyBeHaviour* pB)
{
	m_pBehaviour = pB;
	m_pBehaviour->pHandleObj = this;

	return m_pBehaviour;
}
MyBeHaviour* MyGUIElementBase::GetBehaviour()
{
	return m_pBehaviour;
}