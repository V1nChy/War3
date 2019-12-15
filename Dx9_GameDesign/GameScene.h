#ifndef _GAMESCENE_H_
#define _GAMESCENE_H_
#pragma once

#include "SceneContent.h"
#include "MyUtil.h"

#include "camera.h"
#include "SkyBox.h"
#include "terrain.h"
#include "Sprite.h"

#include "Model.h"
#include "particleSystem.h"
#include "SpriteManager.h"
#define MAX_INTERSECTIONS 16

class GameScene :public SceneContent
{
	LPDIRECT3DDEVICE9 m_pDevice;

	Camera* m_pCamera;
	CSkyBox* m_pSkyBox;
	Terrain* m_pTerrain;
	Sprite* m_pRole;
	
	Model* m_MovePlane;
	Model* m_Build;
	Model* m_RedCastle;
	Model* m_BlueCastle;
	Tower* m_RedTower;
	Tower* m_BlueTower;
	float detalTime;
	DrawTriangle m_DrawTriangle;

	ParticleSystem* m_Snow;

	SpriteManager* m_EnemyManager;

	string text;

	bool m_bOver;
public:
	HRESULT OneTimeSceneInit();
	HRESULT LoadContent();
	void UnloadContent();
	void Update(float dt);
	void Render();

	D3DXVECTOR3* RayIntersectFace(const D3DXVECTOR3& orig, const D3DXVECTOR3& dir);
	D3DXVECTOR3* MousePick();
};


#endif
