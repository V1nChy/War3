#include "MyGUIContainer.h"
#include "MyUtil.h"


MyGUIContainer::MyGUIContainer(MyGUIContainer * parentContainer, int id, string name,
							   int offsetX, int offsetY, int width, int height, bool visible)
				: MyGUIElementBase(parentContainer, id, name, offsetX, offsetY, width, height, true, visible, 0, false)
{
	m_eType = GUI_Container;
}

MyGUIContainer::~MyGUIContainer()
{
	DeleteAllElement();
}
void MyGUIContainer::DeleteAllElement()
{
	int size = m_pElementVector.size();
	for (int i = 0; i < size; ++i)
	{
		SAFE_DELETE(m_pElementVector[i]);
	}
	m_pElementVector.clear();
}

void MyGUIContainer::SortElementInOrder()
{
	bool ordered;

	for (int i = 0; i < static_cast<int>(m_pElementVector.size()) - 1; ++i)
	{
		ordered = true;
		for (int j = 0; j < static_cast<int>(m_pElementVector.size()) - 1 - i; ++j)
		{
			if (m_pElementVector[j]->GetShowLevel() > m_pElementVector[j + 1]->GetShowLevel())
			{
				ordered = false;
				MyGUIElementBase* p = m_pElementVector[j];
				m_pElementVector[j] = m_pElementVector[j + 1];
				m_pElementVector[j + 1] = p;
			}
		}
		if (ordered)
			break;
	}
}

HRESULT MyGUIContainer::Render()
{
	HRESULT r = S_OK;

	if (m_bVisible)
	{
		SortElementInOrder();

		for(int i=0; i < m_pElementVector.size(); ++i)
		{
			r &= m_pElementVector[i]->Render();
		}
	}
	return r;
}

HRESULT MyGUIContainer::Update(float deltaTime)
{
	if (!m_bVisible)
		return S_FALSE;

	HRESULT r = S_OK;
	for (size_t i = 0; i < m_pElementVector.size(); ++i)
	{
		r &= m_pElementVector[i]->Update(deltaTime);
	}

	return r& MyGUIElementBase::Update(deltaTime);
}

HRESULT MyGUIContainer::OneTimeInit()
{
	HRESULT r = S_OK;
	int size = m_pElementVector.size();
	for (int i = 0; i < size; ++i)
	{
		r &= m_pElementVector[i]->OneTimeInit();
	}
	return r & MyGUIElementBase::OneTimeInit();
}


MyGUIElementBase * MyGUIContainer::CreateElement(MyGUIElementType type)
{
	MyGUIElementBase * pElement = NULL;

	int id = MyGUIManager::GetCurrentGUIManager()->GetNextID();
	string name = MyUTIL::IntToStr(id);

	switch (type) {
	case GUI_Label:
		pElement = new MyGUILabel(this, id, name, "GE Game Engine ^o^");
		break;
		case GUI_Image:
		pElement = new MyGUIImage(this, id, name);
		break;
		case GUI_Container:
		pElement = new MyGUIContainer(this, id, name);
		break;
		case GUI_Button:
		pElement = new MyGUIButton(this, id, name);
		break;
		case GUI_CheckBox:
			pElement = new MyGUICheckBox(this, id, name);
		break;
		case GUI_ScrollBarVerti:
			pElement = new MyGUIScrollBarVertical(this, id, name);
			break;
		case GUI_ScrollBarHori:
		pElement = new MyGUIScrollBarHori(this, id, name);
		break;
		/*case GUI_TextBox:
		pElement = new GEGUITextBox(this, id, name, false);
		break;
		case GUI_RadioButton:
		pElement = new GEGUIRadioBox(this, id, name, "DEFAULT_GROUP");
		break;
		case GUI_DropdownList:
		pElement = new GEGUIDropdownList(this, id, name);
		break;
		case GUI_ListBox:
		pElement = new GEGUIListBox(this, id, name);
		break;
		case GUI_Animation:
		pElement = new GEGUIAnimation(this, name, 100);
		break;*/
	default:
		break;
	}

	if (pElement != NULL)
		m_pElementVector.push_back(pElement);

	return pElement;
}
void MyGUIContainer::AddElement(MyGUIElementBase *element)
{
	if (element)
		m_pElementVector.push_back(element);
}


bool MyGUIContainer::DeleteElement(int nID)
{
	vector<MyGUIElementBase *>::iterator p;
	for (p = m_pElementVector.begin(); p != m_pElementVector.end(); ++p)
	{
		if ((*p)->GetID() == nID)
		{
			delete (*p);
			*p = NULL;
			m_pElementVector.erase(p);
			return true;
		}
	}
	return false;
}
bool MyGUIContainer::DeleteElement(const string& sName)
{
	vector<MyGUIElementBase *>::iterator p;
	for (p = m_pElementVector.begin(); p != m_pElementVector.end(); ++p)
	{
		if ((*p)->GetName() == sName)
		{
			delete (*p);
			*p = NULL;
			m_pElementVector.erase(p);
			return true;
		}
	}
	return false;
}

void MyGUIContainer::SetPosition(int offsetFromParentX, int offsetFromParentY)
{
	MyGUIElementBase::SetPosition(offsetFromParentX, offsetFromParentY);
	for (size_t i = 0; i < m_pElementVector.size(); ++i)
	{
		m_pElementVector[i]->Move(0, 0);
	}
}
void MyGUIContainer::Move(int offsetx, int offsety)
{
	MyGUIElementBase::Move(offsetx, offsety);
	for (size_t i = 0; i < m_pElementVector.size(); ++i)
	{
		m_pElementVector[i]->Move(0, 0);
	}
}

bool MyGUIContainer::SetElementPosition(const string& sName, int offsetX, int offsetY)
{
	MyGUIElementBase * pElement = GetElement(sName);
	if (pElement)
	{
		pElement->SetPosition(offsetX, offsetY);
		return true;
	}

	return false;
}
bool MyGUIContainer::SetElementPosition(int nID, int offsetX, int offsetY)
{
	MyGUIElementBase * pElement = GetElement(nID);
	if (pElement)
	{
		pElement->SetPosition(offsetX, offsetY);
		return true;
	}

	return false;
}

bool MyGUIContainer::SetElementEnable(int nID, bool bEnable)
{
	MyGUIElementBase * pElement = GetElement(nID);
	if (pElement)
	{
		pElement->SetEnable(bEnable);
		return true;
	}

	return false;
}
bool MyGUIContainer::SetElementEnable(const string& sName, bool bEnable)
{
	MyGUIElementBase * pElement = GetElement(sName);
	if (pElement){
		pElement->SetEnable(bEnable);
		return true;
	}

	return false;
}

bool MyGUIContainer::SetElementVisible(const string& sName, bool bVisible)
{
	MyGUIElementBase * pElement = GetElement(sName);
	if (pElement){
		pElement->SetVisible(bVisible);
		return true;
	}

	return false;
}
bool MyGUIContainer::SetElementVisible(int nID, bool bVisible)
{
	MyGUIElementBase * pElement = GetElement(nID);
	if (pElement){
		pElement->SetVisible(bVisible);
		return true;
	}

	return false;
}

bool MyGUIContainer::IsElementEnable(int nID) const
{	
	MyGUIElementBase * pElement = GetElement(nID);
	if (pElement)
		return pElement->IsEnable();
	else
		return false;
}
bool MyGUIContainer::IsElementEnable(const string& sName) const
{	
	MyGUIElementBase * pElement = GetElement(sName);
	if (pElement)
		return pElement->IsEnable();
	else
		return false;
}

bool MyGUIContainer::IsElementVisible(int nID) const
{
	MyGUIElementBase * pElement = GetElement(nID);
	if (pElement)
		return pElement->IsVisible();
	else
		return false;
}
bool MyGUIContainer::IsElementVisible(const string& sName) const
{
	MyGUIElementBase * pElement = GetElement(sName);
	if (pElement)
		return pElement->IsVisible();
	else
		return false;
}

bool MyGUIContainer::SetElementSize(const string& sName, int width, int height)
{
	MyGUIElementBase * pElement = GetElement(sName);
	if (pElement)
	{
		pElement->SetSize(width, height);
		return true;
	}

	return false;
}
bool MyGUIContainer::SetElementSize(int nID, int width, int height)
{
	MyGUIElementBase * pElement = GetElement(nID);
	if (pElement)
	{
		pElement->SetSize(width, height);
		return true;
	}

	return false;
}

MyGUIElementBase * MyGUIContainer::GetElement(const string& sName) const
{
	for (unsigned int i = 0; i < m_pElementVector.size(); ++i)
	{
		if (m_pElementVector[i]->GetName() == sName)
			return m_pElementVector[i];
	}

	return NULL;
}
MyGUIElementBase * MyGUIContainer::GetElement(int nID) const
{
	for (unsigned int i = 0; i < m_pElementVector.size(); ++i)
	{
		if (m_pElementVector[i]->GetID() == nID)
			return m_pElementVector[i];
	}

	return NULL;
}