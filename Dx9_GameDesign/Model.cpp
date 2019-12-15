#include "Model.h"
#include "particleSystem.h"
#include "Sprite.h"

Model::Model(IDirect3DDevice9* pd3dDevice, const WCHAR* strFilename):
	m_pd3dDevice(pd3dDevice),
	m_pEffect(NULL)
{
	m_iMaxHp = m_iNowHp = 100;

	m_OffsetPos = D3DXVECTOR3(0, 0, 0);
	m_Scale = D3DXVECTOR3(1,1,1);

	m_MeshLoader.Create(pd3dDevice, strFilename);
	m_sphere.ComputeBoundingSphere(pd3dDevice, m_MeshLoader.GetMesh(), m_Scale.x);

	m_pEffect = new sEffect(pd3dDevice);

	// Store the correct technique handles for each material
	for (UINT i = 0; i < m_MeshLoader.GetNumMaterials(); i++)
	{
		Material* pMaterial = m_MeshLoader.GetMaterial(i);

		const char* strTechnique = NULL;

		if (pMaterial->pTexture && pMaterial->bSpecular)
			strTechnique = "TexturedSpecular";
		else if (pMaterial->pTexture && !pMaterial->bSpecular)
			strTechnique = "TexturedNoSpecular";
		else if (!pMaterial->pTexture && pMaterial->bSpecular)
			strTechnique = "Specular";
		else if (!pMaterial->pTexture && !pMaterial->bSpecular)
			strTechnique = "NoSpecular";

		pMaterial->hTechnique = m_pEffect->pEffect->GetTechniqueByName(strTechnique);
	}
}

Model::~Model()
{
	delete m_pEffect;
}
void Model::Update(float t)
{}
void Model::Render(Camera* p, float t)
{
	D3DXVECTOR3 pos;
	p->getPosition(&pos);

	D3DXMATRIXA16 posMat;
	D3DXMatrixTranslation(&posMat, m_OffsetPos.x, m_OffsetPos.y, m_OffsetPos.z);

	D3DXMATRIXA16 scaleMat;
	D3DXMatrixScaling(&scaleMat, m_Scale.x, m_Scale.y, m_Scale.z);

	D3DXMATRIXA16 mWorld;
	D3DXMATRIXA16 mView;
	D3DXMATRIXA16 mProj;
	D3DXMATRIXA16 mWorldViewProjection;
	// Get the projection & view matrix from the camera class

	D3DXMatrixMultiply(&mWorld, &posMat, &scaleMat);
	mView = p->GetViewMatrix();
	mProj = p->GetProjMatrix();

	mWorldViewProjection = mWorld * mView * mProj;

	// Update the effect's variables. 
	m_pEffect->pEffect->SetMatrix(m_pEffect->hWorldViewProjection, &mWorldViewProjection);
	m_pEffect->pEffect->SetMatrix(m_pEffect->hWorld, &mWorld);
	m_pEffect->pEffect->SetFloat(m_pEffect->hTime, t);
	m_pEffect->pEffect->SetValue(m_pEffect->hCameraPosition, pos, sizeof(D3DXVECTOR3));

	//m_pd3dDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	// Iterate through subsets, changing material properties for each
	for (UINT iSubset = 0; iSubset < m_MeshLoader.GetNumMaterials(); iSubset++)
	{
		RenderSubset(iSubset);
	}

	m_sphere.Render();
}
void Model::RenderSubset(UINT iSubset)
{
	HRESULT hr;
	UINT iPass, cPasses;

	// Retrieve the ID3DXMesh pointer and current material from the MeshLoader helper
	ID3DXMesh* pMesh = m_MeshLoader.GetMesh();
	Material* pMaterial = m_MeshLoader.GetMaterial(iSubset);

	// Set the lighting variables and texture for the current material
	V(m_pEffect->pEffect->SetValue(m_pEffect->hAmbient, pMaterial->vAmbient, sizeof(D3DXVECTOR3)));
	V(m_pEffect->pEffect->SetValue(m_pEffect->hDiffuse, pMaterial->vDiffuse, sizeof(D3DXVECTOR3)));
	V(m_pEffect->pEffect->SetValue(m_pEffect->hSpecular, pMaterial->vSpecular, sizeof(D3DXVECTOR3)));
	V(m_pEffect->pEffect->SetTexture(m_pEffect->hTexture, pMaterial->pTexture));
	V(m_pEffect->pEffect->SetFloat(m_pEffect->hOpacity, pMaterial->fAlpha));
	V(m_pEffect->pEffect->SetInt(m_pEffect->hSpecularPower, pMaterial->nShininess));

	V(m_pEffect->pEffect->SetTechnique(pMaterial->hTechnique));
	V(m_pEffect->pEffect->Begin(&cPasses, 0));

	for (iPass = 0; iPass < cPasses; iPass++)
	{
		V(m_pEffect->pEffect->BeginPass(iPass));

		// The effect interface queues up the changes and performs them 
		// with the CommitChanges call. You do not need to call CommitChanges if 
		// you are not setting any parameters between the BeginPass and EndPass.
		// V( g_pEffect->CommitChanges() );

		// Render the mesh with the applied technique
		V(pMesh->DrawSubset(iSubset));

		V(m_pEffect->pEffect->EndPass());
	}
	V(m_pEffect->pEffect->End());
}

Tower::Tower(IDirect3DDevice9* pd3dDevice, const WCHAR* strFilename, ParticleGun* gun):
	Model(pd3dDevice, strFilename),
	m_gun(gun)
{
	m_iMaxAttackLen = 125;
	m_fAttackCD = 1;
}
Tower::~Tower()
{
	
}
void Tower::Render(Camera* p, float t)
{
	if(m_gun)
		m_gun->render();

	Model::Render(p, t);
}
void Tower::Update(float t)
{
	if(m_fAttackCD>0.0f)
		m_fAttackCD -= t;
	if (m_gun)
	{
		m_gun->update(t);
	}
}
bool Tower::AttackTest(D3DXVECTOR3 pos)
{
	if (m_fAttackCD > 0.0f)
		return false;
	float dis = (m_gun->m_begin.x - pos.x)*(m_gun->m_begin.x - pos.x) + (m_gun->m_begin.y - pos.y)*(m_gun->m_begin.y - pos.y) + \
		(m_gun->m_begin.z - pos.z)*(m_gun->m_begin.z - pos.z);
	dis = sqrtf(dis);
	if (dis <= m_iMaxAttackLen)
	{
		m_fAttackCD = 1.0;
		m_gun->m_end = pos;
		if (m_gun)
		{
			m_gun->addParticle();
		}
		return true;
	}
	return false;
}
void Tower::AttackTest(vector<Sprite*>* pv)
{
	vector<Sprite *>::iterator p, next;
	p = pv->begin();
	while (p != pv->end())
	{
		if (AttackTest((*p)->GetVecPos()))
			break;
		p++;
	}

	p = pv->begin();
	list<Attribute>* particle = m_gun->GetListMap();
	list<Attribute>::iterator i;
	while (p != pv->end())
	{
		i = particle->begin();
		while (i != particle->end())
		{
			if ((*p)->HitTest(i->_position))
			{
				(*p)->m_iNowHp -= 10;
				i->_isAlive = false;
			}
			i++; 
		}
		p++;
	}
}