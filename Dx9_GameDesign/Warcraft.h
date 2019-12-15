#pragma once
#ifndef _WARCRAFT_H_
#define _WARCRAFT_H_

#include "Dx9DemoBase.h"
#include "d3dUtility.h"
#include "terrain.h"
#include "camera.h"
#include "SkyBox.h"
#include "BillBoard.h"
#include "Sprite.h"
#include "sound.h"
#include "MyGUIManager.h"
#include "MyFontManager.h"
#include "MyMath.h"
#include "MyMouse.h"

class WarcraftDemo :public Dx9DemoBase
{
	//单实例设计
	WarcraftDemo();

	Camera* m_pCamera;
	Terrain* m_pTerrain;
	CSkyBox* m_pSkyBox;
	BillBoard* m_BillBoard;
	Sprite* m_pRole;
	list<Sprite*> m_Enemy;
	Sprite* m_pEnt;

	DrawRay	m_DrawRay;
	DrawTriangle m_DrawTriangle;
	cTextOut m_DrawText;

	CDSound m_Sound;

	MyFontManager* m_FontMg;
	MyGUIManager* m_GUI;
public:
	~WarcraftDemo();

	bool LoadContent();
	void UnloadContent();

	void Update(float dt);

	void Render();
	void RenderUI();

	void Pick(int x, int y);

	LRESULT CALLBACK MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static WarcraftDemo* GetObject();

	
};


#endif
