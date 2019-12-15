#ifndef __GUILABLE
#define __GUILABLE

//��ǩ��
//����:
//�������ܹ��������� ���������� ���ֺ� ����ʽ���� ��б ����ͨ����
//�������ܹ������ı����ı����ڵĶ�����ʽ ��
//�������ܹ������ı�����ɫ ��
//�������ܹ������ı������� ��


#include "MyGUIElementBase.h"
#include "MyColor.h"
#include "MyFont.h"

/** ��̬��ǩ�� */
class MyGUILabel: public MyGUIElementBase
{
private:
	string m_sText;		//��ʾ���ַ�	
	int m_iFontID;		//���
	MyColor m_Color;	//��ɫ
	ALIGN_FORMAT m_Format;	//���뷽ʽ
public:
	MyGUILabel(MyGUIElementBase * parent, int id, string name, string text, int offsetX = 0, int offsetY = 0,
		int width = 200, int height = 30, MyColor color = MyColor::Black,
		bool visible = true);

	virtual ~MyGUILabel(){}
	
	void SetFormat(ALIGN_FORMAT format);
	void SetText(const string& text);
	void SetTextColor(const MyColor& color);
	void SetAlpha(float alpha);
	void SetFont(const string& fontName, UINT height, FONT_FLAG eFlags);
	void SetFont(int fontid);

	ALIGN_FORMAT GetFormat()		const;
	const string& GetText()			const;
	const MyColor& GetTextColor()	const;
	const string& GetFontName()		const;
	int GetFontHeight()				const;
	FONT_FLAG GetFontFlags()		const;
	
	virtual HRESULT Render();
	virtual HRESULT Update(float deltaTime);

};


#endif
