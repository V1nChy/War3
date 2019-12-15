#ifndef __GUILABLE
#define __GUILABLE

//标签类
//功能:
//（１）能够设置字体 ，包括字体 、字号 、样式（粗 、斜 、普通）。
//（２）能够设置文本在文本框内的对齐形式 。
//（３）能够设置文本的颜色 。
//（４）能够设置文本的内容 。


#include "MyGUIElementBase.h"
#include "MyColor.h"
#include "MyFont.h"

/** 静态标签类 */
class MyGUILabel: public MyGUIElementBase
{
private:
	string m_sText;		//显示的字符	
	int m_iFontID;		//编号
	MyColor m_Color;	//颜色
	ALIGN_FORMAT m_Format;	//对齐方式
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
