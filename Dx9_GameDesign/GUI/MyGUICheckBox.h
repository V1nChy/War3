#ifndef _MY_CHECK_BOX_H_
#define _MY_CHECK_BOX_H_
#pragma once 

#include "MyGUIButtonBase.h"

const int MINCHECKBOXWIDTH = 30;
const int MINCHECKBOXHEIGHT = 30;
const int INTERVAL = 5;


/** µãÑ¡¿ò */
class MyGUICheckBox:public MyGUIButtonBase
{
protected:
	bool m_bChecked;

public:
	bool CheckMousePosition();
	
	bool IsChecked();

	void SetCheck(bool b);

	MyGUICheckBox(MyGUIElementBase * parent, int id, const string& name,
		int offsetX = 0, int offsetY = 0, int width = 150, int height = 20, 
		const string& imageNormal = "Texture/GUI-OGRE.tga", const string& imageButtonDown = "Texture/GUI-OGRE.tga", 
		const string& imageOver = "Texture/GUI-OGRE.tga", const string& imageDisabled = "Texture/GUI-OGRE.tga", const string& strText = "New CheckBox", 
		bool visible = true, int tabindex = 0, bool tabenable = false, bool bChecked = false);

	void SetCheckBoxElementPosition();

	void SetCheckBoxElementSize();

	void Move(int offwetx, int offwety);

	void SetPosition(int offsetFromParentX, int offsetFromParentY);

	void SetSize(int width, int height);
	
	virtual HRESULT Render();

	virtual HRESULT Update(float deltaTime);


};

#endif