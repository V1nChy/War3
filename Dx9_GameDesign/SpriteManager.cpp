#include "SpriteManager.h"
#include "Model.h"
#include "MyApp.h"

SpriteManager::SpriteManager(Sprite* role, LPDIRECT3DDEVICE9 device):
	m_pDevice(device),
	m_pRole(role),
	m_iMaxEnemyNum(4),
	m_iNowEnemyNum(0),
	m_iMaxRound(4),
	m_iNowRound(1),
	m_fTime(0)
{}
SpriteManager::~SpriteManager()
{
	DeleteAll();
}
void SpriteManager::Add(Sprite* sp)
{
	m_iNowEnemyNum++;
	m_vSpriteCt.push_back(sp);
}
void SpriteManager::AddOnePart()
{
	for (int i = 0; i < 4; i++)
	{
		Sprite* temp = m_vPrefab.back();
		Add(temp);
		m_vPrefab.pop_back();
	}
}
void SpriteManager::AddPrefab()
{
	MyApp* pApp = (MyApp*)EngineFrame::GetSingleton();
	pApp->RenderLoading();
	/*——————————————————————————————————————————————*/
	Sprite* sprite;
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-200 });
	sprite->SetRotateAngle(-PI / 2);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));
	sprite->m_sphere.isRender = false;
	sprite->SetView(100);			//发现敌人范围
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-200 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	/*——————————————————————————————————————————————*/
	/*——————————————————————————————————————————————*/
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-200 });
	sprite->SetRotateAngle(-PI / 2);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));
	sprite->m_sphere.isRender = false;
	sprite->SetView(100);			//发现敌人范围
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-200 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	/*——————————————————————————————————————————————*/
	/*——————————————————————————————————————————————*/
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-200 });
	sprite->SetRotateAngle(-PI / 2);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));
	sprite->m_sphere.isRender = false;
	sprite->SetView(100);			//发现敌人范围
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-200 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	/*——————————————————————————————————————————————*/
	/*——————————————————————————————————————————————*/
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-200 });
	sprite->SetRotateAngle(-PI / 2);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));
	sprite->m_sphere.isRender = false;
	sprite->SetView(100);			//发现敌人范围
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 70,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(70, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-250 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	sprite = new Sprite(m_pDevice, "Res\\Ent.X", D3DXVECTOR3(0.2, 0.2, 0.2), "Stand_1", 1, -66, 10);
	sprite->SetPosition(POINT{ 0,-200 });
	sprite->SetEndPoint(D3DXVECTOR3(0, 100, 550));
	sprite->SetRotateAngle(-PI / 2);
	sprite->SetView(100);
	sprite->m_sphere.CreateMesh(4, D3DXVECTOR3(40, -82, 23));	//创建并调整包围球位置
	sprite->m_sphere.isRender = false;
	m_vPrefab.push_back(sprite);
	pApp->RenderLoading();
	/*——————————————————————————————————————————————*/
}
void SpriteManager::Update(float dt)
{
	m_fTime += dt;

	vector<Sprite *>::iterator p,next;
	p = m_vSpriteCt.begin();
	while (p != m_vSpriteCt.end())
	{
		if ((*p)->GetIsHide())
		{
			//delete (*p);
			m_iNowEnemyNum--;
			p = m_vSpriteCt.erase(p);
			continue;
		}

		(*p)->UpDate();
		if ((*p)->m_iNowHp < 0)
		{
			(*p)->SetAction(A_DEATH);
			p++;
			continue;
		}

		if ((*p)->m_sphere.CollisionShere(target->m_sphere))
		{
			(*p)->SetAction(A_ATTACK);
			if ((*p)->GetAttackRound())
				target->m_iNowHp -= (*p)->m_iMaxAttackNum;
			p++;
			continue;
		}
		else if ((*p)->m_sphere.CollisionShere(m_pRole->m_sphere))
		{
			(*p)->SetAction(A_ATTACK);
			if((*p)->GetAttackRound())
				m_pRole->m_iNowHp -= (*p)->m_iMaxAttackNum;
			if(m_pRole->GetAttackRound())
				(*p)->m_iNowHp-= m_pRole->m_iMaxAttackNum;
			p++;
			continue;
		}

		if ((*p)->AttackTest(target->m_sphere.GetPoint()))
		{
			(*p)->SetEndPoint(target->m_sphere._center);
		}
		else if ((*p)->AttackTest(m_pRole->GetPosition()))
		{
			(*p)->SetEndPoint(m_pRole->GetVecPos());
		}
		else if ((*p)->m_isFoundEnemy)
		{
			(*p)->m_isFoundEnemy = false;
			(*p)->SetEndPoint(D3DXVECTOR3(35, 100, 550));
		}
		p++;
	}

	if (m_fTime > 20.0f && m_iNowRound < m_iMaxRound && m_iNowEnemyNum<m_iMaxEnemyNum)
	{
		m_fTime = 0.0f;
		m_iNowRound++;
		AddOnePart();
	}
}
void SpriteManager::Render()
{
	for (int i = 0; i < m_vSpriteCt.size(); i++)
	{
		m_vSpriteCt[i]->Render(0.015f);
	}
}
void SpriteManager::DeleteAll()
{
	for (int i = 0; i < m_vSpriteCt.size(); i++)
	{
		SAFE_DELETE(m_vSpriteCt[i]);
	}
	m_vSpriteCt.clear();
}
bool SpriteManager::isAllDeath()
{
	if (m_iNowRound == m_iMaxRound && m_iNowEnemyNum == 0)
		return true;
	return false;
}