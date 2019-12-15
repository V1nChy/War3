#ifndef _MAINMENU_H_
#define _MAINMENU_H_
#pragma once

#include "SceneContent.h"
#include "MyUtil.h"

class MainMenu :public SceneContent
{
public:
	HRESULT OneTimeSceneInit();
	HRESULT LoadContent();
	void UnloadContent();
	void Update(float dt);
	void Render();
};


#endif
