// GEGUIContainer.h: interface for the GEGUIContainer class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEGUICONTAINER_H__A431D6F1_2908_491C_A712_F0CE52BEB2AC__INCLUDED_)
#define AFX_GEGUICONTAINER_H__A431D6F1_2908_491C_A712_F0CE52BEB2AC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGUIElementBase.h"
#include "MyGUILabel.h"
#include "MyGUIImage.h"
#include "MyGUIButton.h"
#include "MyGUICheckBox.h"
#include "MyGUIScrollBarVertical.h"
#include "MyGUIScrollBarHori.h"
#include <vector>
using namespace std;


//�������, ����panel
 
class MyGUIContainer : public MyGUIElementBase
{
private:
	vector<MyGUIElementBase*> m_pElementVector;
	void SortElementInOrder();
public:
	MyGUIContainer(MyGUIContainer * parentContainer, int id, string name, int offsetx = 0, int offsety = 0, int width = 800, int height = 600, bool visible = true);
	virtual ~MyGUIContainer();

	//�����ӿؼ�
	MyGUIElementBase* CreateElement(MyGUIElementType type);
	void AddElement(MyGUIElementBase * element);

	virtual HRESULT Render();
	virtual HRESULT Update(float deltaTime);
	virtual HRESULT OneTimeInit();

	void SetPosition(int offsetFromParentX, int offsetFromParentY);
	void Move(int offsetx, int offsety);

	// ���ÿؼ�λ��
	bool SetElementPosition(const string& sName, int offsetX, int offsetY);
	bool SetElementPosition(int nID, int offsetX, int offsetY);

	// ���ÿؼ���С
	bool SetElementSize(const string& sName, int wight, int hight);
	bool SetElementSize(int nID, int width, int height);

	// �õ������ÿؼ�Visible����
	bool SetElementVisible(const string& sName, bool bVisible);
	bool SetElementVisible(int nID, bool bVisible);
	bool IsElementVisible(const string& sName) const;
	bool IsElementVisible(int nID) const;

	// �õ������ÿؼ�Enable����
	bool SetElementEnable(int nID, bool bEnable);
	bool SetElementEnable(const string& sName, bool bEnable);
	bool IsElementEnable(int nID) const;
	bool IsElementEnable(const string& sName) const;

	//ɾ���ӿؼ�
	bool DeleteElement(int nID);
	bool DeleteElement(const string& sName);
	void DeleteAllElement();
	
	// �õ���Ӧ�ؼ�
	MyGUIElementBase* GetElement(const string& sName) const;
	MyGUIElementBase* GetElement(int nID) const;
};

#endif // !defined(AFX_GEGUICONTAINER_H__A431D6F1_2908_491C_A712_F0CE52BEB2AC__INCLUDED_)
