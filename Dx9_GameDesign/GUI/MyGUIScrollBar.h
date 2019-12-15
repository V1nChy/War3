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


/** ������,���� */
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

	bool _bInControl;				// �ڷ�editģʽ����

	Position _pPosBegin;			// ��ѡ��ʱ��λ��
	Size _pSize;					// ��ѡ��ʱ�Ĵ�С
	Position _pMouseBegin;			// ��ѡ��ʱ����λ��
	float _fValueBegin;				// ��ѡ���ƶ���ʱ��ֵ

private:
	void FinalCleanUp();	

};

#endif
