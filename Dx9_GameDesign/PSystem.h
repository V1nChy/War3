#pragma once

#include <d3dx9.h>
#include "d3dUtility.h"
#include "camera.h"
#include <vector>

using namespace std;

struct Particle
{
	D3DXVECTOR3 initPos;
	D3DXVECTOR3 initVelocity;
	float		initSize;		//in pixel
	float       initTime;
	float		lifeTime;
	float		mass;
	D3DXCOLOR	initColor;

	static IDirect3DVertexDeclaration9*	decl;
};

class PSystem
{
public:
	PSystem(
		const char* fxName,
		const char* techName,
		const char* texName,
		const D3DXVECTOR3& accel,
		const BoundingBox& box,
		int maxNumParticles,
		float timePerParticle,
		LPDIRECT3DDEVICE9 device,
		Camera* camera);
	virtual ~PSystem();

public:
	float getTime();
	void setTime(float fTime);
	const BoundingBox& getAABB() const;
	void setWorldMtx(const D3DXMATRIX& world);
	void addParticle();

	virtual void onLostDevice();
	virtual void onResetDevice();

	virtual void initParticles(Particle& out) = 0;
	virtual void update(float dt);
	virtual void draw();

protected:
	LPDIRECT3DDEVICE9 m_pDevice;				// Device
	ID3DXEffect		*m_FX;					// Effect
	D3DXHANDLE		 m_hTech;					// Technique
	D3DXHANDLE		 m_hWVP;					// WVP matrix
	D3DXHANDLE		 m_hEyePosL;				// 
	D3DXHANDLE		 m_hTex;					// Texture
	D3DXHANDLE		 m_hTime;					// Time
	D3DXHANDLE		 m_hAccel;					// Accel
	D3DXHANDLE		 m_hViewportHeight;			// Viewport's height

	IDirect3DTexture9* m_Tex;					// Texture
	IDirect3DVertexBuffer9* m_VB;			// Vertex buffer
	D3DXMATRIX       m_World;					// World matrix
	D3DXMATRIX		 m_InvWorld;				// Inverse matrix
	float			 m_Time;					// Time
	D3DXVECTOR3		 m_Accel;					// Accelerate
	BoundingBox		 m_AABB;					// Bounding box
	int				 m_MaxNumParticles;			// Max number particles
	float			 m_fTimePerParticle;		// Delay time to emit one particle

	Camera			*m_pCamera;				// Camera

	std::vector<Particle>	m_Particles;		// Particles list
	std::vector<Particle*>  m_AliveParticles;	// Alive particles list
	std::vector<Particle*>	m_DeadParticles;	// Dead particles list
};// end for PSystem