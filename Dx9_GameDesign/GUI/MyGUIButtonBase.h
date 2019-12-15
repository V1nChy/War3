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


//���ఴť��
//������Image��һ��Label���

class MyGUIButtonBase : public MyGUIElementBase
{
public:
	//ö��Button��״̬
	enum ButtonState{
		BTS_NORMAL,
		BTS_MOUSEOVER,
		BTS_BUTTONDOWN,
		BTS_DISABLE,
	};

protected:	
	MyGUIImage* m_pImgNormal;			//����״̬��ͼƬ
	MyGUIImage* m_pImgMouseOver;		//�������ȥʱ��ͼƬ
	MyGUIImage* m_pImgButtonDown;		//��갴��ʱ��ͼƬ
	MyGUIImage* m_pImgButtonDisable;	//��ť����ֹʹ�ú��ͼƬ
	MyGUILabel* m_pLabel;					//��ʾ���ֵ�Label

	ButtonState m_ButtonState;//��ť״̬

	bool m_bChanged;//�жϰ�ť�Ƿ񱻸ı����Ԥ������

public:
	static ButtonState Str2ButtonState(const string& str);
	static string ButtonState2Str(const ButtonState& state);

	MyGUIButtonBase(MyGUIElementBase * parent, int id, string name, string imageNormal,
		string imageOver, string imageButtonDown, string imageDisabled, string strText,  
		int offsetX = 0, int offsetY = 0, int width = 100, int height = 100, bool visible = true, 
		int tabindex = 0, bool tabenable = false);

	virtual ~MyGUIButtonBase();

	/* �麯����Ҫ������,�����ؼ���λ��ʽ */
	virtual void SetLocation(sLocation lt);

	/* �麯����Ҫ������,�����ؼ�λ�� */
	virtual void SetPosition(int offestx, int offesty);

	/* �麯����Ҫ������,���ƿؼ���С */
	virtual void SetSize(int width, int height);

	/* �麯����Ҫ�����أ������ƶ��ؼ� */
	virtual void Move(int offsetx, int offsety);

	bool GetChanged() const			{ return m_bChanged;	}
	void SetChanged(bool bChanged)	{ m_bChanged = bChanged;}

	const ButtonState& GetButtonState() const		{ return m_ButtonState;  }
	void SetButtonStatus(const ButtonState& state)	{ m_ButtonState = state; }

	/* �õ�Label */
	MyGUILabel* GetLabel() const	{ return m_pLabel; }
	void SetLabel(MyGUILabel * label);

	void SetLabelText(const string& text);
	void SetLabelPosition(int lefttopx, int lefttopy);
	void SetLabelSize(int width, int height);
	void SetLabelAlignMode(const ALIGN_FORMAT& format);
	void SetLabelTextColor(const MyColor& colorkey);

	/** �õ����ϵ�ǰ��ť״̬��ͼƬ��ؼ�ָ�� */
	MyGUIImage* GetImage() const;
	/** �õ���Ӧ��ť״̬��Ӧ��ͼƬ��ؼ�ָ�� */
	MyGUIImage* GetImage(const ButtonState& state) const;

	/** ������Ӧ��ť״̬��Ӧ��ͼƬ��ؼ� */
	void SetImage(const ButtonState& state, const string& image, const MyColor& colorKey=MyColor::NoColor);
	void SetImage(const ButtonState& state, MyGUIImage *pic);

	/** ������Ӧ��ť״̬��Ӧ��ͼƬ��ؼ� 
	 *  @param image : �������Ű������а�ťͼƬ�Ĵ�ͼƬ
	 *	@param imageLength : ��ͼƬ�ĳ�
	 *	@param imageHeight : ��ͼƬ�Ŀ�
	 *	@param btnLength : ��ťͼƬ�ĳ�
	 *	@param btnHeight : ��ťͼƬ�Ŀ�
	 */
	void SetImage(const string& image, float imageLength, float imageHeight,
		float btnLength, float btnHeight, const MyColor& colorKey = MyColor::NoColor);


	virtual HRESULT Render();

private:
	void FinalCleanUp();
};

#endif // !defined(AFX_GEGUIBUTTONBASE_H__3B271D33_E041_4EA8_81B8_7CD059831BB1__INCLUDED_)
