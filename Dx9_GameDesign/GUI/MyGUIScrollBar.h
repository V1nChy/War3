// MyGUIScrollBar.h: interface for the MyGUIScrollBar class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_SCROLLBAR_H_
#define _MY_SCROLLBAR_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGUIElementBase.h"

class MyGUIImage;
class MyGUILabel;


/** 滚动条,基类 */
class MyGUIScrollBar : public MyGUIElementBase
{
public:
	enum PicIndex
	{
		BAR_BEGIN,
		BAR_END,
		BAR_MIDDLE,
		BAR_TARGET,
	};
	
	void SetPictureBox(PicIndex index, MyGUIImage* pic);
	MyGUIImage* GetImage(PicIndex index);

	float GetValue();
	void SetValue(float value);

	virtual void SetPosition(int offsetXFromParent, int offsetYFromParent);
	virtual void Move(int offsetx, int offsety);
	
	virtual HRESULT Update(float deltaTime);


	MyGUIScrollBar(MyGUIElementBase * parent, int id, string name, int offsetX = 0, int offsetY = 0,
		int width = 100, int height = 100, bool enable = true, bool visible = true);
	virtual ~MyGUIScrollBar();
	
	virtual HRESULT Render();


protected:
	virtual void _UpdateTarget() = 0;

	MyGUIImage* m_pPicBarBegin;
	MyGUIImage* m_pPicBarEnd;
	MyGUIImage* m_pPicBarMiddle;
	MyGUIImage* m_pPicTarget;

	MyGUILabel*		 m_pLabelValue;

	float m_fValue;		///< 0 ~ 1 

	bool _bInControl;				// 在非edit模式下用

	Position _pPosBegin;			// 刚选中时的位置
	Size _pSize;					// 刚选中时的大小
	Position _pMouseBegin;			// 刚选中时鼠标的位置
	float _fValueBegin;				// 刚选中移动靶时的值

private:
	void FinalCleanUp();	

};

#endif
