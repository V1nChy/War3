#pragma once

#include"PSystem.h"

class FireRing : public PSystem
{
public:
	FireRing(const char* fxName,
		const char* techName,
		const char* texName,
		const D3DXVECTOR3& accel,
		const BoundingBox& box,
		int maxNumParticles,
		float timePerParticle,
		LPDIRECT3DDEVICE9 device,
		Camera* camera)
		:PSystem(fxName, techName, texName, accel,
			box, maxNumParticles, timePerParticle, device, camera)
	{

	};

	void initParticles(Particle& out)
	{
		//Save the init time
		out.initTime = m_Time;

		//Calculate the life time from 2.0s to 4.0s
		out.lifeTime = 20.0f + 2 * (rand() % 10001 * 0.0001);

		//Calculate the initialize size in pixel
		out.initSize = 50.0f + 10 * (rand() % 10001 * 0.0001);

		//Calculate the a very small velocity
		static float angle = 0.0f;
		D3DXVECTOR3 vel(0.5f, 1.0f, 0.5f);
		D3DXMATRIX m;

		D3DXMatrixRotationY(&m, angle);
		D3DXVec3TransformCoord(&vel, &vel, &m);

		out.initVelocity = vel;
		D3DXVec3Normalize(&out.initVelocity, &out.initVelocity);
		angle += 1.0f;

		//Calculate the mass
		out.mass = 1.0f + (rand() % 10001 * 0.0001);

		//Calculate the color
		float t = (0.5f + 0.5*(rand() % 10001 * 0.0001));
		out.initColor = D3DXCOLOR(0.0f, 0.0f, t * 1.0f, t * 1.0f);

		//Calculate the pos
		out.initPos.x = 0.0f;
		out.initPos.y = 0.0f;
		out.initPos.z = 0.0f;

	}// end for initParticle
};