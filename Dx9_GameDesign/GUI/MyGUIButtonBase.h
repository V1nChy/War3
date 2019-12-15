// MyGUIButtonBase.h: interface for the MyGUIButtonBase class.
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_GEGUIBUTTONBASE_H__3B271D33_E041_4EA8_81B8_7CD059831BB1__INCLUDED_)
#define AFX_GEGUIBUTTONBASE_H__3B271D33_E041_4EA8_81B8_7CD059831BB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGUIElementBase.h"
#include "MyColor.h"

class MyGUIImage;
class MyGUILabel;
enum ALIGN_FORMAT;


//基类按钮类
//由四种Image和一个Label组成

class MyGUIButtonBase : public MyGUIElementBase
{
public:
	//枚举Button的状态
	enum ButtonState{
		BTS_NORMAL,
		BTS_MOUSEOVER,
		BTS_BUTTONDOWN,
		BTS_DISABLE,
	};

protected:	
	MyGUIImage* m_pImgNormal;			//正常状态的图片
	MyGUIImage* m_pImgMouseOver;		//鼠标移上去时的图片
	MyGUIImage* m_pImgButtonDown;		//鼠标按下时的图片
	MyGUIImage* m_pImgButtonDisable;	//按钮被禁止使用后的图片
	MyGUILabel* m_pLabel;					//显示文字的Label

	ButtonState m_ButtonState;//按钮状态

	bool m_bChanged;//判断按钮是否被改变过，预留属性

public:
	static ButtonState Str2ButtonState(const string& str);
	static string ButtonState2Str(const ButtonState& state);

	MyGUIButtonBase(MyGUIElementBase * parent, int id, string name, string imageNormal,
		string imageOver, string imageButtonDown, string imageDisabled, string strText,  
		int offsetX = 0, int offsetY = 0, int width = 100, int height = 100, bool visible = true, 
		int tabindex = 0, bool tabenable = false);

	virtual ~MyGUIButtonBase();

	/* 虚函数需要被重载,调整控件定位方式 */
	virtual void SetLocation(sLocation lt);

	/* 虚函数需要被重载,调整控件位置 */
	virtual void SetPosition(int offestx, int offesty);

	/* 虚函数需要被重载,控制控件大小 */
	virtual void SetSize(int width, int height);

	/* 虚函数需要被重载，用来移动控件 */
	virtual void Move(int offsetx, int offsety);

	bool GetChanged() const			{ return m_bChanged;	}
	void SetChanged(bool bChanged)	{ m_bChanged = bChanged;}

	const ButtonState& GetButtonState() const		{ return m_ButtonState;  }
	void SetButtonStatus(const ButtonState& state)	{ m_ButtonState = state; }

	/* 得到Label */
	MyGUILabel* GetLabel() const	{ return m_pLabel; }
	void SetLabel(MyGUILabel * label);

	void SetLabelText(const string& text);
	void SetLabelPosition(int lefttopx, int lefttopy);
	void SetLabelSize(int width, int height);
	void SetLabelAlignMode(const ALIGN_FORMAT& format);
	void SetLabelTextColor(const MyColor& colorkey);

	/** 得到符合当前按钮状态的图片框控件指针 */
	MyGUIImage* GetImage() const;
	/** 得到相应按钮状态对应的图片框控件指针 */
	MyGUIImage* GetImage(const ButtonState& state) const;

	/** 设置相应按钮状态对应的图片框控件 */
	void SetImage(const ButtonState& state, const string& image, const MyColor& colorKey=MyColor::NoColor);
	void SetImage(const ButtonState& state, MyGUIImage *pic);

	/** 设置相应按钮状态对应的图片框控件 
	 *  @param image : 包含四张按序排列按钮图片的大图片
	 *	@param imageLength : 大图片的长
	 *	@param imageHeight : 大图片的宽
	 *	@param btnLength : 按钮图片的长
	 *	@param btnHeight : 按钮图片的宽
	 */
	void SetImage(const string& image, float imageLength, float imageHeight,
		float btnLength, float btnHeight, const MyColor& colorKey = MyColor::NoColor);


	virtual HRESULT Render();

private:
	void FinalCleanUp();
};

#endif // !defined(AFX_GEGUIBUTTONBASE_H__3B271D33_E041_4EA8_81B8_7CD059831BB1__INCLUDED_)
