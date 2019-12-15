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


/** GUI管理器 */
class MyGUIManager
{
private:
	static MyGUIManager* m_pCurrentGUIManager;

	void SortContainersInOrder(vector<MyGUIContainer*>& containers);

	bool m_bVisible;	//GUI可见性的属性
	bool m_bEnable;		//GUI可用性的属性

	vector<MyGUIContainer*> m_BackgroundGUIContainers;
	vector<MyGUIContainer*> m_ForwardGUIContainers;

	int m_iNextID;

public:
	MyGUIManager();		//构造函数
	virtual ~MyGUIManager();//析构函数 
	void OneTimeInit();
	void FrameMove(float deltaTime);

	void RenderBackgroundGUI();
	void RenderForwardGUI();

	void FinalCleanUp();

	/* 返回有多少container被加载了 */
	int GetContainerCounts() const;

	/* 得到单例模式 */
	static MyGUIManager* GetCurrentGUIManager() { return m_pCurrentGUIManager; };
	static void SetCurrentGUIManager(MyGUIManager* pGUI) { m_pCurrentGUIManager = pGUI; };

	//创建Container
	MyGUIContainer* CreateContainer(const string& strName, bool isForwardGUI = true);

	//删除一个Container的重载
	bool DeleteContainer(int nContainerIndex);
	bool DeleteContainer(const string& sName);

	//移除所有Container
	void RemoveAllContainer();

	//得到Container的Visible的属性
	bool IsContainerVisible(const string& sName) const;
	bool IsContainerVisible(int nID) const;

	//设置Container的Visible的属性
	bool SetContainerVisible(const string& sName, bool bVisible);
	bool SetContainerVisible(int nID, bool bVisible);

	//设置Container的位置
	bool SetContainerPosition(const string& sName, int x, int y);
	bool SetContainerPosition(int nID, int x, int y);

	//设置Container的大小
	bool SetContainerSize(const string& sName, int width, int height);
	bool SetContainerSize(int nID, int width, int height);

	//得到某一Container的Enabled属性
	bool IsContainerEnabled(const string& sName) const;
	bool IsContainerEnabled(int nID) const;

	//设置某一Container的Enabled属性
	bool SetContainerEnabled(int nID, bool bEnable);
	bool SetContainerEnabled(const string& sName, bool bEnable);

	//得到GUI的Enable属性
	bool IsEnable() const;
	//设置GUI的Enable属性
	void SetEnable(bool bEnable);

	//得到GUI的Visible属性
	bool IsVisible();	
	//设置GUI的Visible属性
	void SetVisible(bool bVisible);
	
	
	//得到某一Container
	MyGUIContainer* GetContainer(int nContainerID) const;
	MyGUIContainer* GetContainer(const string& sName) const;

	int GetNextID();
};

#endif // !defined(AFX_GEGUIMANAGER_H__FAA5C368_D6F7_4A53_8C47_EDCE3CF9E1F0__INCLUDED_)
