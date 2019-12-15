#include "Sprite.h"

Sprite::Sprite(IDirect3DDevice9* device, const char* wfilename, D3DXVECTOR3 size, 
			   string action, double actiontime, float fheight, int maxlive)
{
	m_SkinMesh = new CSkinMesh(device);
	m_SkinMesh->create(wfilename);
	m_SkinMesh->SetAnimationSet(action, actiontime);
	m_SkinMesh->SetTransHeight(fheight);
	m_SkinMesh->SetSize(size);
	m_V3Speed = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_V3EndPoint = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Action = A_STOP;
	m_fMaxMoveStep = 0.1;
	m_fView = 2000.0f;
	m_iTime = 65;
    m_iAttackTime = 0;
	m_iMaxAttackCD = 150;
	m_iMaxHp = m_iNowHp = maxlive;
	m_iMaxMp = 500;
	m_iNowMp = 500;
	m_iMaxAttackLen = 100;
	m_iMaxAttackNum = 100;

	m_bMoving = false;
	m_isFoundEnemy = false;
	m_sphere.ComputeBoundingSphere(device, m_SkinMesh->GetOrgMesh(), size.x);
}

void Sprite::Render(double app_elapsed_time)
{
	m_sphere.Render();
	m_SkinMesh->render(app_elapsed_time);
}

void Sprite::UpDate()
{
	if (m_Action == A_MOVE)
	{	
		if (Move())
		{
			//移动停止
			SetAction(A_STOP);
			SetSpeed(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		}
	}
	if (m_Action == A_DEATH && m_iTime == 150)
	{
		m_Action = A_HIDE;
	}
	if (m_Action == A_ATTACK || m_Action == A_ATTACK2 || m_Action == A_SPELL || m_Action == A_SPELL2)
	{
		(++ m_iAttackTime) %= m_iMaxAttackCD;
	}
	else
	{
		m_iAttackTime = 0;
	}
	//移动
	//D3DXMATRIX matStep;
	//D3DXMatrixTranslation(&matStep, m_V3Speed.x, 0, m_V3Speed.z);
	//m_SkinMesh->m_matMoveMatrices = m_SkinMesh->m_matMoveMatrices * matStep;
	
	(++m_iTime) %= 300;//控制时间更新
	//魔法自动回复
	if(m_iTime % 32 == 0 && m_iNowMp < m_iMaxMp)
	{
		m_iNowMp ++;
	}
}

bool Sprite::Move()
{
	bool EndMove = false;//移动标记
	//计算精灵到目标点的距离
	D3DXVECTOR3 Len = m_V3EndPoint - 
		D3DXVECTOR3(m_SkinMesh->m_matMoveMatrices._41, 0.0f, m_SkinMesh->m_matMoveMatrices._43);
	//计算但前速度
	//当长度大于最大移动距离时
	float length = (Len.x * Len.x + Len.z * Len.z);
	length = sqrt(length);
	if (length >= m_fMaxMoveStep)
	{
		m_V3Speed.x = m_fMaxMoveStep * cosf(2 * D3DX_PI - m_SkinMesh->m_fRotateAngle);
		m_V3Speed.z = m_fMaxMoveStep * sinf(2 * D3DX_PI - m_SkinMesh->m_fRotateAngle); 
		m_V3Speed.y = 0;
	}
	else 
	{
		m_V3Speed.x = length * cosf(2 * D3DX_PI - m_SkinMesh->m_fRotateAngle);
		m_V3Speed.z = length * sinf(2 * D3DX_PI - m_SkinMesh->m_fRotateAngle); 
		m_V3Speed.y = 0;
		EndMove = true;
	}
	////移动
	D3DXMATRIX matStep;
	D3DXMatrixTranslation(&matStep, m_V3Speed.x, 0, m_V3Speed.z);
	m_SkinMesh->m_matMoveMatrices = m_SkinMesh->m_matMoveMatrices * matStep;
	m_SkinMesh->m_matMoveMatrices._41 += m_V3Speed.x;
	m_SkinMesh->m_matMoveMatrices._43 += m_V3Speed.z;

	m_sphere._worldPos = GetVecPos();
	return EndMove;
}

bool Sprite::FieldOfView(POINT pos)
{
	double length = sqrt((pos.y - m_SkinMesh->m_matMoveMatrices._43) * (pos.y - m_SkinMesh->m_matMoveMatrices._43) + 
		(pos.x - m_SkinMesh->m_matMoveMatrices._41) * (pos.x - m_SkinMesh->m_matMoveMatrices._41));

	if (length < m_fView / 2)
	{
		if(length < m_sphere._radius * 2)
			return false;
		return true;
	}
	if (length > m_fView)
	{
		return false;
	}

	float fAngle = 2 * D3DX_PI - atan2( pos.y - m_SkinMesh->m_matMoveMatrices._43, 
		pos.x - m_SkinMesh->m_matMoveMatrices._41);
	if (fabs(fAngle - m_SkinMesh->m_fRotateAngle) < D3DX_PI / 3.0f)
	{
		return true;
	}
	else 
	{
		return false;
	}
}

bool Sprite::AttackTest(POINT pos)
{
	double length = sqrt((pos.y - m_SkinMesh->m_matMoveMatrices._43) * (pos.y - m_SkinMesh->m_matMoveMatrices._43) + 
		(pos.x - m_SkinMesh->m_matMoveMatrices._41) * (pos.x - m_SkinMesh->m_matMoveMatrices._41));
	if (length > m_iMaxAttackLen)
	{

		return false;
	}
	if (length < m_iMaxAttackLen / 2)
	{
		m_isFoundEnemy = true;
		return true;
	}
	//计算攻击的角度
	float fAngle = 2 * D3DX_PI - atan2( pos.y - m_SkinMesh->m_matMoveMatrices._43, 
		pos.x - m_SkinMesh->m_matMoveMatrices._41);
	if (fabs(fAngle - m_SkinMesh->m_fRotateAngle) < D3DX_PI / 3.0f)
	{
		m_isFoundEnemy = true;
		return true;
	}
	else 
	{
		return false;
	}

}
bool Sprite::HitTest(D3DXVECTOR3 pos)
{
	D3DXVECTOR3 temp = GetVecPos();
	float dis = (temp.x - pos.x)*(temp.x - pos.x) + (temp.y - pos.y)*(temp.y - pos.y) + \
		(temp.z - pos.z)*(temp.z - pos.z);
	dis = sqrtf(dis);
	if (dis <= m_sphere._radius)
	{
		return true;
	}
	return false;
}
Sprite::~Sprite()
{
	Delete(m_SkinMesh);
	m_sphere.Release();
}
