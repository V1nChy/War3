// MyGUIScrollBarHori.h: interface for the MyGUIScrollBarHori class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_SCROLLBAR_HORI_H_
#define _MY_SCROLLBAR_HORI_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGUIScrollBar.h"


/** 水平滚动条 */
class MyGUIScrollBarHori : public MyGUIScrollBar  
{
public:
	void SetWidth(int width);
	MyGUIScrollBarHori(MyGUIElementBase * parent, int id, string name, int offsetX = 0, int offsetY = 0,
		int width = 200, int height = 10, bool enable = true, bool visible = true);
	virtual ~MyGUIScrollBarHori();

	void SetSize(int width, int height);
	HRESULT Update(float deltaTime);

protected:
	void _UpdateTarget();
};

#endif