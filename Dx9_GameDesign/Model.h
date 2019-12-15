#pragma once
#ifndef _MODEL_H_
#define _MODEL_H_

#include "meshloader.h"
#include "camera.h"
#include <vector>

struct sEffect
{
	sEffect(IDirect3DDevice9* pd3dDevice)
	{
		HRESULT hr;
		// If this fails, there should be debug output as to 
		// they the .fx file failed to compile
		hr = D3DXCreateEffectFromFile(pd3dDevice, "media\\MeshFromOBJ.fx", NULL, NULL, NULL,
			NULL, &pEffect, NULL);

		// Cache the effect handles
		hAmbient = pEffect->GetParameterBySemantic(0, "Ambient");
		hDiffuse = pEffect->GetParameterBySemantic(0, "Diffuse");
		hSpecular = pEffect->GetParameterBySemantic(0, "Specular");
		hOpacity = pEffect->GetParameterBySemantic(0, "Opacity");
		hSpecularPower = pEffect->GetParameterBySemantic(0, "SpecularPower");
		hLightColor = pEffect->GetParameterBySemantic(0, "LightColor");
		hLightPosition = pEffect->GetParameterBySemantic(0, "LightPosition");
		hCameraPosition = pEffect->GetParameterBySemantic(0, "CameraPosition");
		hTexture = pEffect->GetParameterBySemantic(0, "Texture");
		hTime = pEffect->GetParameterBySemantic(0, "Time");
		hWorld = pEffect->GetParameterBySemantic(0, "World");
		hWorldViewProjection = pEffect->GetParameterBySemantic(0, "WorldViewProjection");
	}
	~sEffect()
	{
		SAFE_RELEASE(pEffect);
	}
	ID3DXEffect* pEffect;

	D3DXHANDLE                  hAmbient = NULL;
	D3DXHANDLE                  hDiffuse = NULL;
	D3DXHANDLE                  hSpecular = NULL;
	D3DXHANDLE                  hOpacity = NULL;
	D3DXHANDLE                  hSpecularPower = NULL;
	D3DXHANDLE                  hLightColor = NULL;
	D3DXHANDLE                  hLightPosition = NULL;
	D3DXHANDLE                  hCameraPosition = NULL;
	D3DXHANDLE                  hTexture = NULL;
	D3DXHANDLE                  hTime = NULL;
	D3DXHANDLE                  hWorld = NULL;
	D3DXHANDLE                  hWorldViewProjection = NULL;
};

class Model
{
protected:
	IDirect3DDevice9* m_pd3dDevice;

	CMeshLoader m_MeshLoader;
	
	sEffect* m_pEffect;

	D3DXVECTOR3 m_OffsetPos;
	D3DXVECTOR3 m_Scale;
public:
	int         m_iMaxHp;        //最大生命值
	int         m_iNowHp;        //当前生命值

	BoundingSphere m_sphere;
	Model(IDirect3DDevice9* pd3dDevice, const WCHAR* strFilename);
	~Model();
	virtual void Render(Camera* p,float t);
	virtual void Update(float t);
	void RenderSubset(UINT iSubset);

	inline void SetOffset(D3DXVECTOR3 offset) {
		m_OffsetPos = offset;
	}
	inline void SetScale(D3DXVECTOR3 scale) {
		m_Scale = scale;
	}

	inline ID3DXMesh* GetMesh() {
		return m_MeshLoader.GetMesh();
	}
};

class ParticleGun;

class Sprite;
class Tower :public Model
{
	ParticleGun* m_gun;
	int m_iMaxAttackLen; //最大攻击范围
	float m_fAttackCD;
public:
	Tower(IDirect3DDevice9* pd3dDevice, const WCHAR* strFilename, ParticleGun* gun=NULL);
	~Tower();
	void Render(Camera* p, float t);
	void Update(float t);
	bool AttackTest(D3DXVECTOR3 pos);
	void AttackTest(vector<Sprite*>* pv);
};

#endif
