#ifndef _MY_APP_H_
#define _MY_APP_H_
#pragma once

#include "MyEngineFrame.h"
#include "MainMenu.h"
#include "GameScene.h"

class MyApp :public EngineFrame
{
	MyGUIManager* m_Load;
	MyGUIManager* m_CurretGUI;
	MySceneManager* m_CurretSM;
	SceneContent* m_CurCont;
	MainMenu m_MainMenu;
	GameScene m_GameScene;
public:
	MyApp();
	~MyApp();

	bool change=false;

	HRESULT OneTimeSceneInit();
	HRESULT InitDeviceObjects();
	HRESULT RestoreDeviceObjects();
	HRESULT FrameMove();
	HRESULT Render();
	HRESULT InvalidateDeviceObjects();
	HRESULT DeleteDeviceObjects();
	void FinalCleanup();

	void RenderAll();
	void RenderLoading(bool isEnd=false);
};

#endif
