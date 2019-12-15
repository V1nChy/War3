#include"PSystem.h"
#include"MyUtil.h"
#include<d3dx9.h>

IDirect3DVertexDeclaration9* Particle::decl = NULL;
/**
* Constructor
*/
PSystem::PSystem(const char* fxName,
	const char* techName,
	const char* texName,
	const D3DXVECTOR3& accel,
	const BoundingBox& box,
	int maxNumParticles,
	float timePerParticle,
	LPDIRECT3DDEVICE9 device,
	Camera* camera)
{
	//Save the device
	m_pDevice = device;

	//Create error buffer
	ID3DXBuffer* _error = NULL;
	D3DXCreateBuffer(128, &_error);

	//Create the Effect
	D3DXCreateEffectFromFileA(m_pDevice, fxName,
		NULL, NULL, D3DXSHADER_DEBUG, NULL, &m_FX, &_error);

	//If error 
	if (_error)
	{
		MessageBoxA(NULL, (char*)_error->GetBufferPointer(), "Error", MB_OK);
		return;
	}

	//Get the technique handle
	m_hTech = m_FX->GetTechniqueByName(techName);

	//Get all the handle
	m_hWVP = m_FX->GetParameterByName(0, "gWVP");
	m_hEyePosL = m_FX->GetParameterByName(0, "gEyePosL");
	m_hTex = m_FX->GetParameterByName(0, "gTex");
	m_hTime = m_FX->GetParameterByName(0, "gTime");
	m_hAccel = m_FX->GetParameterByName(0, "gAccel");
	m_hViewportHeight = m_FX->GetParameterByName(0, "gViewportHeight");

	//Create the texture
	D3DXCreateTextureFromFileA(m_pDevice, texName, &m_Tex);

	//Set parameters
	m_FX->SetTechnique(m_hTech);
	m_FX->SetTexture(m_hTex, m_Tex);
	m_FX->SetVector(m_hAccel, &D3DXVECTOR4(m_Accel, 0.0f));

	//Save the time per particles
	m_fTimePerParticle = timePerParticle;

	m_Time = 0.0f;

	//Save the AABB
	m_AABB = box;

	//Save the camera
	m_pCamera = camera;

	//Allocate the memory for the particle
	m_MaxNumParticles = maxNumParticles;
	m_Particles.resize(m_MaxNumParticles);
	m_AliveParticles.reserve(m_MaxNumParticles);
	m_DeadParticles.reserve(m_MaxNumParticles);

	//They start all dead
	for (int i = 0; i < m_MaxNumParticles; i++)
	{
		m_Particles[i].initTime = 0.0f;
		m_Particles[i].lifeTime = -1.0f;
	}

	//Create the vertex buffer
	m_pDevice->CreateVertexBuffer(m_MaxNumParticles * sizeof(Particle), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY |
		D3DUSAGE_POINTS, 0, D3DPOOL_DEFAULT, &m_VB, NULL);
}// end for constructor

 /**
 * Destructor
 */
PSystem::~PSystem()
{
	m_AliveParticles.clear();
	m_DeadParticles.clear();
	m_Particles.clear();
	m_FX->Release();
	m_Tex->Release();
	m_VB->Release();
}// end for destructor

void PSystem::setTime(float fTime)
{
	m_Time = fTime;
}// end for setTime

void PSystem::setWorldMtx(const D3DXMATRIX& world)
{
	m_World = world;
	D3DXMatrixInverse(&m_World, NULL, &m_World);
}// end for setWorldMtx

void PSystem::addParticle()
{
	if (m_DeadParticles.size() > 0)
	{
		Particle* p = m_DeadParticles.back();
		initParticles(*p);

		m_DeadParticles.pop_back();
		m_AliveParticles.push_back(p);
	}
}// end for addParticle

void PSystem::onLostDevice()
{
	//OnlostDevice for fx
	m_FX->OnLostDevice();

	if (m_VB)
	{
		m_VB->Release();
		m_VB = NULL;
	}
}// end for onLostDevice

void PSystem::onResetDevice()
{
	//OnResetDevice for fx
	m_FX->OnResetDevice();

	if (m_VB)
	{
		//Recreate the vertex buffer
		m_pDevice->CreateVertexBuffer(m_MaxNumParticles*sizeof(Particle),
			D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY | D3DUSAGE_POINTS, 0,
			D3DPOOL_DEFAULT,
			&m_VB,
			NULL);
	}
}// end for onResetDevice

void PSystem::update(float dt)
{
	m_Time += dt;

	//Rebuild the dead list and alive list
	m_DeadParticles.resize(0);
	m_AliveParticles.resize(0);

	//For each particle
	for (int i = 0; i < m_Particles.size(); i++)
	{
		if ((m_Time - m_Particles[i].initTime) > m_Particles[i].lifeTime)
		{
			m_DeadParticles.push_back(&m_Particles[i]);
		}
		else
		{
			m_AliveParticles.push_back(&m_Particles[i]);
		}
	}// end for 

	 //Check if it is the time to emit one another particle
	if (m_fTimePerParticle > 0.0f)
	{
		static float timeDelay = 0.0f;
		timeDelay += dt;

		if (timeDelay > m_fTimePerParticle)
		{
			addParticle();
			timeDelay = 0.0f;
		}
	}
}// end for update

void PSystem::draw()
{
	//Get the camera's position in the world and make it relative to the particle system's local system
	D3DXVECTOR3 eyeW;
	m_pCamera->getPosition(&eyeW);
	D3DXVECTOR3 eyeL;
	D3DXVec3TransformCoord(&eyeL, &eyeW, &m_InvWorld);

	//Set the FX parameter
	m_FX->SetValue(m_hEyePosL, &eyeL, sizeof(D3DXVECTOR3));
	m_FX->SetFloat(m_hTime, m_Time);
	m_FX->SetMatrix(m_hWVP, &(m_World* m_pCamera->GetViewProjMatrix()));
	m_FX->SetInt(m_hViewportHeight, 600);

	//Draw
	//set the vertex buffer
	m_pDevice->SetStreamSource(0, m_VB, 0, sizeof(Particle));

	//set the vertex declaration
	m_pDevice->SetVertexDeclaration(Particle::decl);

	Particle* p = 0;
	m_VB->Lock(0, 0, (void**)&p, D3DLOCK_DISCARD);
	UINT vIndex = 0;
	for (int i = 0; i < m_AliveParticles.size(); i++)
	{
		p[vIndex] = *m_AliveParticles[i];
		vIndex++;
	}// end for

	m_VB->Unlock();

	UINT numPass = 0;
	m_FX->Begin(&numPass, 0);
	m_FX->BeginPass(0);

	if (vIndex > 0)
	{
		m_pDevice->DrawPrimitive(D3DPT_POINTLIST, 0, vIndex);
	}

	m_FX->EndPass();
	m_FX->End();
}// end for draw