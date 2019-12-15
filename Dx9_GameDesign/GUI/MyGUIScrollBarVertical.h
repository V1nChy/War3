// MyGUIScrollBarVertical.h: interface for the MyGUIScrollBarVertical class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _MY_SCROLLBAR_VERTICAL_H_
#define _MY_SCROLLBAR_VERTICAL_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyGUIScrollBar.h"


/** ´¹Ö±¹ö¶¯Ìõ */
class MyGUIScrollBarVertical : public MyGUIScrollBar  
{
public:
	void SetHeight(int height);
	MyGUIScrollBarVertical(MyGUIElementBase * parent, int id, string name, int offsetX = 0, int offsetY = 0,
		int width = 10, int height = 200, bool enable = true, bool visible = true);
	virtual ~MyGUIScrollBarVertical();

	void SetSize(int width, int height);
	HRESULT Update(float deltaTime);

protected:
	void _UpdateTarget();
};

#endif
