// GEGUIButton.h: interface for the GEGUIButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEGUIBUTTON_H__D89F7173_3855_4754_A94A_CF534BBA5EB7__INCLUDED_)
#define AFX_GEGUIBUTTON_H__D89F7173_3855_4754_A94A_CF534BBA5EB7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGUIButtonBase.h"
#include "MyEventHandler.h"
//#include "GE2DSound.h"

const int MAXFUC = 10;

class My2DSound;

/** 按钮类 */
class MyGUIButton : public MyGUIButtonBase 
{
private:

	Position _pPosBegin;			///< 刚选中时的位置
	Size _pSize;					///< 刚选中时的大小
	Position _pMouseBegin;			///< 刚选中时鼠标的位置

	My2DSound*	m_pBtnSound;		///< 按钮滑过的时候的声音

	MyEventHandler*	m_pEventHandler;///< 消息处理器	

	bool _bOnBtn;					///< 鼠标是否移到按钮上，用于声音提示
public:
	bool CheckMousePosition();
	
	// 绑定响应函数
	void BindFunction(void (*func)());
	template<typename T>
	void BindFunction(T *object, void (T::*func)())
	{
		SAFE_DELETE(m_pEventHandler);
		m_pEventHandler = CreateInstanceEventHandler(object, func);
	}

	MyGUIButton(MyGUIElementBase * parent, int id, const string& name,
		int offsetX = 0, int offsetY = 0, int width = 110, int height = 40, 
		const string& imageNormal = "Texture/Button_NLPD.tga", const string& imageOver= "Texture/Button_NLPD.tga", 
		const string& imageButtonDown = "Texture/Button_NLPD.tga", const string& imageDisabled = "Texture/Button_NLPD.tga", 
		const string& strText = "New Button", bool visible = true, 
		int tabindex = 0, bool tabenable = false, bool bPushed = false);
	virtual ~MyGUIButton();

	virtual HRESULT Update(float deltaTime);

	/** 重新设置按钮滑过声音 */
	void SetBtnSound(const string& sound);
	void SetBtnSoundNull();
};
#endif // !defined(AFX_GEGUIBUTTON_H__D89F7173_3855_4754_A94A_CF534BBA5EB7__INCLUDED_)
