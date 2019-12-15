#pragma once
#ifndef _SPRITE_MANAGER_H_
#define _SPRITE_MANAGER_H_

#include "Sprite.h"
#include <vector.h>

class Model;

class SpriteManager
{
	LPDIRECT3DDEVICE9 m_pDevice;
	Sprite* m_pRole;
	vector<Sprite*> m_vSpriteCt;
	vector<Sprite*> m_vPrefab;
	int m_iMaxEnemyNum;
	int m_iNowEnemyNum;

	int m_iMaxRound;
	int m_iNowRound;

	float m_fTime;
public:
	Model* target;
	SpriteManager(Sprite* role, LPDIRECT3DDEVICE9 device);
	~SpriteManager();
	void Add(Sprite* sp);
	void AddOnePart();
	void AddPrefab();
	void Update(float dt);
	void Render();
	void DeleteAll();
	bool isAllDeath();
	inline vector<Sprite*>* GetContent() {
		return &m_vSpriteCt;
	}
};

#endif
