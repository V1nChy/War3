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

/** ��ť�� */
class MyGUIButton : public MyGUIButtonBase 
{
private:

	Position _pPosBegin;			///< ��ѡ��ʱ��λ��
	Size _pSize;					///< ��ѡ��ʱ�Ĵ�С
	Position _pMouseBegin;			///< ��ѡ��ʱ����λ��

	My2DSound*	m_pBtnSound;		///< ��ť������ʱ�������

	MyEventHandler*	m_pEventHandler;///< ��Ϣ������	

	bool _bOnBtn;					///< ����Ƿ��Ƶ���ť�ϣ�����������ʾ
public:
	bool CheckMousePosition();
	
	// ����Ӧ����
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

	/** �������ð�ť�������� */
	void SetBtnSound(const string& sound);
	void SetBtnSoundNull();
};
#endif // !defined(AFX_GEGUIBUTTON_H__D89F7173_3855_4754_A94A_CF534BBA5EB7__INCLUDED_)
