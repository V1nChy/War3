#ifndef __GUIBASE_H_
#define __GUIBASE_H_

#pragma warning(disable:4786)
#pragma warning(disable:4267)
#pragma warning(disable:4244)

#include <WINDOWS.H>

#include "Point2D.h"
#include "BeHaviour.h"
#include <fstream>
#include <string>
using namespace std;

class MyGUIContainer;

typedef Point2D<int> Position;
typedef Point2D<int> Size;

//枚举控件类型
enum MyGUIElementType
{
	GUI_Label, 
	GUI_Image, 
	GUI_Button, 
	GUI_CheckBox, 
	GUI_RadioButton, 
	GUI_TextBox, 
	GUI_ScrollBarHori, 
	GUI_ScrollBarVerti, 
	GUI_ListBox, 
	GUI_DropdownList,
	GUI_Container,
	GUI_FileFinder,
	GUI_PictureSet,
	GUI_Animation,
	GUI_Default
};


struct sLocation
{
	enum LocationType
	{
		RelativeType,
		AbsoluteType
	};
	float sX;
	float sY;
	LocationType sLT;
	sLocation() 
	{
		sX = 1;
		sY = 1;
		sLT = AbsoluteType;
	}
	sLocation(float x,float y)
	{
		sX = x;
		sY = y;
		sLT = RelativeType;
	}
};

/** GUI组件基类 */
class MyGUIElementBase
{	
protected:
	int m_iShowLevel;				// 在显示时的层次，0在最下
	MyGUIElementType m_eType;		// 控件类型
	sLocation m_eLocatType;

	string m_strName;				// 控件名字
	int m_nID;						// 控件ID,唯一
	
	Position m_OffsetFromParent;	// 控件相对父物体的位置
	Position m_Position;			// 控件的世界坐标系下位置	
	Size m_Size;					// 大小
		
	bool m_bEnable;					// 是否可编辑
	bool m_bVisible;				// 是否可见

	MyGUIElementBase * m_pParent;	// 父控件指针

	MyBeHaviour* m_pBehaviour;
public:

	MyGUIElementBase(MyGUIElementBase * parent, int id, string name , int offsetX = 0, int offsetY = 0, int width = 200, int height = 200,
		bool enable = true, bool visible = true, int tabindex = 0, bool tabEnable = false, sLocation location = sLocation());
	virtual ~MyGUIElementBase();
	
	MyGUIElementType GetType();
	void SetShowLevel(int level);
	int GetShowLevel();

	void SetParent(MyGUIElementBase* pParent);
	MyGUIElementBase * GetParent();

	virtual bool IsEnable() const;
	virtual void SetEnable(bool bEnable); 

	virtual sLocation GetLocation() const;
	virtual void SetLocation(sLocation lt);

	virtual int GetID() const;
	virtual void SetID(int id);

	virtual const string& GetName() const;
	virtual void SetName(const string& name);

	virtual void SetPosition(int offsetFromParentX, int offsetFromParentY);
	virtual void Move(int offsetx, int offsety);
	virtual const Position& GetPosition() const;
	virtual const Position& GetOffsetFromParent() const;

	virtual void SetSize(int width, int height);
	virtual const Size& GetSize() const;

	virtual bool IsVisible() const;
	virtual void SetVisible(bool bVisible);

	MyBeHaviour* SetBehaviour(MyBeHaviour* pB);
	MyBeHaviour* GetBehaviour();

	virtual HRESULT Render() = 0;
	virtual HRESULT Update(float deltaTime);

	virtual HRESULT OneTimeInit();

};

#endif
