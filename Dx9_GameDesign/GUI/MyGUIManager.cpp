#include "MyGUIManager.h"
#include "MyUTIL.h"
#include "MyMouse.h"
#include "MyMaterial.h"
#include <string>
using namespace std;

MyGUIManager * MyGUIManager::m_pCurrentGUIManager = NULL;

MyGUIManager::MyGUIManager()
{

	m_bVisible = true;
	m_bEnable = true;

	m_iNextID = 0;
}

MyGUIManager::~MyGUIManager()
{
	FinalCleanUp();
}

void MyGUIManager::OneTimeInit()
{
	for(size_t i = 0; i < m_BackgroundGUIContainers.size(); ++i)
	{
		m_BackgroundGUIContainers[i]->OneTimeInit();
	}
	for(size_t i = 0; i < m_ForwardGUIContainers.size(); ++i)
	{
		m_ForwardGUIContainers[i]->OneTimeInit();
	}
}

void MyGUIManager::FrameMove(float deltaTime)
{
	for (size_t i = 0; i < m_BackgroundGUIContainers.size(); ++i)
	{
		m_BackgroundGUIContainers[i]->Update(deltaTime);
	}

	for(size_t i = 0; i < m_ForwardGUIContainers.size(); ++i)
	{
		m_ForwardGUIContainers[i]->Update(deltaTime);
	}
}

void MyGUIManager::FinalCleanUp()
{
	for (size_t i = 0; i < m_BackgroundGUIContainers.size(); ++i)
	{
		SAFE_DELETE(m_BackgroundGUIContainers[i]);
	}
	for(size_t i = 0; i < m_ForwardGUIContainers.size(); ++i)
	{
		SAFE_DELETE(m_ForwardGUIContainers[i]);
	}
	m_ForwardGUIContainers.clear();
	m_BackgroundGUIContainers.clear();
	/*SAFE_DELETE(m_pCursorSystem);
	m_pCursorSystem = m_pCursorNormal = NULL;*/
}

void MyGUIManager::RenderBackgroundGUI()
{
	SortContainersInOrder(m_BackgroundGUIContainers);

	MyMaterial::GUIMaterial.SetFromMaterial();
	for(size_t i = 0; i < m_BackgroundGUIContainers.size(); ++i)
	{
		m_BackgroundGUIContainers[i]->Render();
	}
}

void MyGUIManager::RenderForwardGUI()
{

	MyMaterial::GUIMaterial.SetFromMaterial();

	SortContainersInOrder(m_ForwardGUIContainers);

	for(size_t i = 0; i < m_ForwardGUIContainers.size(); ++i)
	{
		m_ForwardGUIContainers[i]->Render();
	}

}


void MyGUIManager::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}
bool MyGUIManager::IsVisible()
{
	return m_bVisible;
}
bool MyGUIManager::IsEnable() const
{
	if (m_bEnable)
		return true;
	return false;
}
void MyGUIManager::SetEnable(const bool bEnable)
{
	m_bEnable = bEnable;
}

int MyGUIManager::GetNextID()
{
	return m_iNextID++;
}

MyGUIContainer* MyGUIManager::CreateContainer(const string& strName, bool isForwardGUI)
{
	int id = GetNextID();
	MyGUIContainer* temp = new MyGUIContainer(NULL, id, strName);

	if (isForwardGUI)
		m_ForwardGUIContainers.push_back(temp);
	else
		m_BackgroundGUIContainers.push_back(temp);

	return temp;
}
bool MyGUIManager::DeleteContainer(int iID)
{
	vector<MyGUIContainer *>::iterator p;
	for (p = m_ForwardGUIContainers.begin(); p != m_ForwardGUIContainers.end(); ++p)
	{
		if ((*p)->GetID() == iID)
		{
			delete (*p);
			m_ForwardGUIContainers.erase(p);
			return true;
		}
	}

	for (p = m_BackgroundGUIContainers.begin(); p != m_BackgroundGUIContainers.end(); ++p)
	{
		if ((*p)->GetID() == iID)
		{
			delete (*p);
			m_BackgroundGUIContainers.erase(p);
			return true;
		}
	}

	return false;
}
bool MyGUIManager::DeleteContainer(const string& sName)
{
	vector<MyGUIContainer *>::iterator p;
	for (p = m_ForwardGUIContainers.begin(); p != m_ForwardGUIContainers.end(); ++p)
	{
		if ((*p)->GetName() == sName)
		{
			delete (*p);
			m_ForwardGUIContainers.erase(p);
			return true;
		}
	}

	for (p = m_BackgroundGUIContainers.begin(); p != m_BackgroundGUIContainers.end(); ++p)
	{
		if ((*p)->GetName() == sName)
		{
			delete (*p);
			m_BackgroundGUIContainers.erase(p);
			return true;
		}
	}

	return false;
}
void MyGUIManager::RemoveAllContainer()
{
	for (size_t i = 0; i < m_ForwardGUIContainers.size(); ++i)
	{
		SAFE_DELETE(m_ForwardGUIContainers[i]);
	}
	for (size_t i = 0; i < m_BackgroundGUIContainers.size(); ++i)
	{
		SAFE_DELETE(m_BackgroundGUIContainers[i]);
	}

	m_ForwardGUIContainers.clear();
	m_BackgroundGUIContainers.clear();
}

int MyGUIManager::GetContainerCounts() const
{
	return (m_ForwardGUIContainers.size() + m_BackgroundGUIContainers.size());
}
MyGUIContainer* MyGUIManager::GetContainer(int nContainerID) const
{
	for (int i = static_cast<int>(m_ForwardGUIContainers.size()-1); i>=0; --i)
	{
		if(m_ForwardGUIContainers[i]->GetID() == nContainerID) 
			return m_ForwardGUIContainers[i];
	}

	for (int i = static_cast<int>(m_BackgroundGUIContainers.size()-1); i>=0; --i)
	{
		if(m_BackgroundGUIContainers[i]->GetID() == nContainerID) 
			return m_BackgroundGUIContainers[i];
	}

	return NULL;	
}
MyGUIContainer* MyGUIManager::GetContainer(const string& sName) const
{
	for (int i = static_cast<int>(m_ForwardGUIContainers.size())-1; i>=0; --i)
	{
		if(m_ForwardGUIContainers[i]->GetName() == sName) 
			return m_ForwardGUIContainers[i];
	}

	for (int i = static_cast<int>(m_BackgroundGUIContainers.size()-1); i>=0; --i)
	{
		if(m_BackgroundGUIContainers[i]->GetName() == sName) 
			return m_BackgroundGUIContainers[i];
	}

	return NULL;	
}

bool MyGUIManager::SetContainerEnabled(int iID, bool bEnable)
{
	MyGUIContainer* container = GetContainer(iID);

	if (container)
	{
		container->SetEnable(bEnable);
		return true;
	}

	return false;
}
bool MyGUIManager::SetContainerEnabled(const string& sName, bool bEnable)
{
	MyGUIContainer* container = GetContainer(sName);

	if (container)
	{
		container->SetEnable(bEnable);
		return true;
	}

	return false;
}
bool MyGUIManager::IsContainerEnabled(int nID) const
{
	MyGUIContainer* container = GetContainer(nID);

	if (container)
		return container->IsEnable();

	return false;
}
bool MyGUIManager::IsContainerEnabled(const string& sName) const
{
	MyGUIContainer* container = GetContainer(sName);

	if (container)
		return container->IsEnable();

	return false;
}

bool MyGUIManager::SetContainerSize(const string& sName, int width, int height)
{
	MyGUIContainer* container = GetContainer(sName);

	if (container)
	{
		container->SetSize(width, height);
		return true;
	}

	return false;
}
bool MyGUIManager::SetContainerSize(int nID, int width, int height)
{
	MyGUIContainer* container = GetContainer(nID);

	if (container)
	{
		container->SetSize(width, height);
		return true;
	}

	return false;
}

bool MyGUIManager::SetContainerPosition(const string& sName, int x, int y)
{
	MyGUIContainer * container = GetContainer(sName);

	if (container)
	{
		container->SetPosition(x, y);
		return true;
	}

	return false;
}
bool MyGUIManager::SetContainerPosition(int nID, int x, int y)
{
	MyGUIContainer * container = GetContainer(nID);

	if (container)
	{
		container->SetPosition(x, y);
		return true;
	}

	return false;
}

bool MyGUIManager::SetContainerVisible(const string& sName, bool bVisible)
{
	MyGUIContainer* container = GetContainer(sName);

	if (container)
	{
		container->SetVisible(bVisible);
		return true;
	}

	return false;
}
bool MyGUIManager::SetContainerVisible(int nID, bool bVisible)
{
	MyGUIContainer* container = GetContainer(nID);

	if (container)
	{
		container->SetVisible(bVisible);
		return true;
	}

	return false;
}

bool MyGUIManager::IsContainerVisible(const string& sName) const
{
	MyGUIContainer * container = GetContainer(sName);

	if (container)
	{
		return container->IsVisible();
	}

	return false;
}
bool MyGUIManager::IsContainerVisible(int nID) const
{
	MyGUIContainer * container = GetContainer(nID);

	if (container)
	{
		return container->IsVisible();
	}

	return false;
}

void MyGUIManager::SortContainersInOrder(vector<MyGUIContainer*>& containers)
{
	bool ordered;

	for (int i = 0; i < static_cast<int>(containers.size())-1; ++i)
	{
		ordered = true;
		for (int j = 0; j < static_cast<int>(containers.size())-1-i; ++j)
		{
			if (containers[j]->GetShowLevel() > containers[j+1]->GetShowLevel())
			{
				ordered = false;
				MyGUIContainer* p = containers[j];
				containers[j] = containers[j+1];
				containers[j+1] = p;
			}
		}
		if (ordered)
			break;
	}
}



