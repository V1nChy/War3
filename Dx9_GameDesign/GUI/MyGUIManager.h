// GEGUIManager.h: interface for the GEGUIManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEGUIMANAGER_H__FAA5C368_D6F7_4A53_8C47_EDCE3CF9E1F0__INCLUDED_)
#define AFX_GEGUIMANAGER_H__FAA5C368_D6F7_4A53_8C47_EDCE3CF9E1F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGUIContainer.h"
using namespace std;


/** GUI������ */
class MyGUIManager
{
private:
	static MyGUIManager* m_pCurrentGUIManager;

	void SortContainersInOrder(vector<MyGUIContainer*>& containers);

	bool m_bVisible;	//GUI�ɼ��Ե�����
	bool m_bEnable;		//GUI�����Ե�����

	vector<MyGUIContainer*> m_BackgroundGUIContainers;
	vector<MyGUIContainer*> m_ForwardGUIContainers;

	int m_iNextID;

public:
	MyGUIManager();		//���캯��
	virtual ~MyGUIManager();//�������� 
	void OneTimeInit();
	void FrameMove(float deltaTime);

	void RenderBackgroundGUI();
	void RenderForwardGUI();

	void FinalCleanUp();

	/* �����ж���container�������� */
	int GetContainerCounts() const;

	/* �õ�����ģʽ */
	static MyGUIManager* GetCurrentGUIManager() { return m_pCurrentGUIManager; };
	static void SetCurrentGUIManager(MyGUIManager* pGUI) { m_pCurrentGUIManager = pGUI; };

	//����Container
	MyGUIContainer* CreateContainer(const string& strName, bool isForwardGUI = true);

	//ɾ��һ��Container������
	bool DeleteContainer(int nContainerIndex);
	bool DeleteContainer(const string& sName);

	//�Ƴ�����Container
	void RemoveAllContainer();

	//�õ�Container��Visible������
	bool IsContainerVisible(const string& sName) const;
	bool IsContainerVisible(int nID) const;

	//����Container��Visible������
	bool SetContainerVisible(const string& sName, bool bVisible);
	bool SetContainerVisible(int nID, bool bVisible);

	//����Container��λ��
	bool SetContainerPosition(const string& sName, int x, int y);
	bool SetContainerPosition(int nID, int x, int y);

	//����Container�Ĵ�С
	bool SetContainerSize(const string& sName, int width, int height);
	bool SetContainerSize(int nID, int width, int height);

	//�õ�ĳһContainer��Enabled����
	bool IsContainerEnabled(const string& sName) const;
	bool IsContainerEnabled(int nID) const;

	//����ĳһContainer��Enabled����
	bool SetContainerEnabled(int nID, bool bEnable);
	bool SetContainerEnabled(const string& sName, bool bEnable);

	//�õ�GUI��Enable����
	bool IsEnable() const;
	//����GUI��Enable����
	void SetEnable(bool bEnable);

	//�õ�GUI��Visible����
	bool IsVisible();	
	//����GUI��Visible����
	void SetVisible(bool bVisible);
	
	
	//�õ�ĳһContainer
	MyGUIContainer* GetContainer(int nContainerID) const;
	MyGUIContainer* GetContainer(const string& sName) const;

	int GetNextID();
};

#endif // !defined(AFX_GEGUIMANAGER_H__FAA5C368_D6F7_4A53_8C47_EDCE3CF9E1F0__INCLUDED_)
