/*********************************************************
*           class  SPRITE
*           author 詹伟龙
*               V 1.0
**********************************************************/
#pragma once

#ifndef SPRITE_H
#define SPRITE_H

#include "SkinMesh.h"
#include "d3dUtility.h"

typedef WORD        ACTION;//边界动作
const ACTION  A_STOP      = 0,//停止
			  A_MOVE      = 1,//移动		
			  A_ATTACK2   = 2,//攻击2
			  A_ATTACK    = 3,//攻击1	
			  A_SPELL     = 4,//施法1
			  A_SPELL2    = 5,//施法2
			  A_DEATH     = 6,//死亡
			  A_HIDE      = 7;//隐藏

const char ACTNAME[][15] = { "Stand_1","Walk","Attack_2","Attack_1","Spell","Stand_Victory","Death","Dissipate" };

class Sprite
{
public:
	CSkinMesh*  m_SkinMesh;      //角色骨骼对象
	ACTION      m_Action;        //动作类型
	D3DXVECTOR3 m_V3Speed;       //速度
	D3DXVECTOR3 m_V3EndPoint;    //目标点
	float       m_fMaxMoveStep;  //移动最大步长
	float       m_fView;         //视线的范围
	int         m_iTime;         //时间用于控制AI 或 魔法回复
	int         m_iMaxHp;        //最大生命值
	int         m_iNowHp;        //当前生命值
	int         m_iMaxAttackLen; //最大攻击范围
	int         m_iMaxAttackNum; //最大伤害值
	int      	m_iAttackTime;   //攻击时间控制
	int         m_iMaxMp;        //魔法的最大值
	int         m_iNowMp;        //魔法的当前值
	int         m_iKind;         //敌人的类型

	int			m_iMaxAttackCD;
	BoundingSphere m_sphere;     //边界球
	//D3DXMATRIX g_mFirePDMoveMatrix;
	//float g_fFirePDAngle = 0.0f;

	bool m_bMoving;			   //是否移动
	bool m_isFoundEnemy;
public:
	Sprite(IDirect3DDevice9* device, const char* wfilename, D3DXVECTOR3 size, 
		string action, double actiontime, float fheight, int maxlive);
	~Sprite();

	void SetSpeed(D3DXVECTOR3 NewSpeed);
	void SetAction(WORD action);
	void SetAnimSign( bool bAnim );
	void SetRotateAngle( float fAngle );
	void SetRotateAngle( D3DXVECTOR3 point );
	void SetEndPoint(D3DXVECTOR3 endpoint);
	void SetTransHeight( float fheigh );
	void SetAnimationSet( std::string set, double times );//设置角色的当前动画
	void SetPosition(POINT pPosition);//设置角色位置
	void SetHeight(float h);
	void SetView(float view);
	float GetView(){return m_fView;}
	D3DXVECTOR3 GetSpeed(){return m_V3Speed;}
	WORD GetAction(){return m_Action;}
	float GetRotateAngle(){return m_SkinMesh->m_fRotateAngle;}
	void Render(double app_elapsed_time);
	POINT GetPosition();
	D3DXVECTOR3 GetVecPos();
	void UpDate();
	bool Move();
	//可视范围
	bool FieldOfView(POINT pos);
	//碰撞检测
	bool AttackTest(POINT pos);
	bool HitTest(D3DXVECTOR3 pos);

	bool GetAttackRound();
	bool GetIsHide();
};

inline void Sprite::SetSpeed(D3DXVECTOR3 NewSpeed)
{
	m_V3Speed = NewSpeed;
}
inline void Sprite::SetRotateAngle( float fAngle )
{
	m_SkinMesh->SetRotateAngle(fAngle);
}
inline void Sprite::SetRotateAngle(D3DXVECTOR3 point )
{
	//计算旋转的角度
	float fAngle = 2 * D3DX_PI - atan2( point.z - m_SkinMesh->m_matMoveMatrices._43, 
		point.x - m_SkinMesh->m_matMoveMatrices._41);
	m_SkinMesh->SetRotateAngle(fAngle);
}
inline void Sprite::SetTransHeight( float fheigh )
{
	m_SkinMesh->SetTransHeight(fheigh);
}
inline void Sprite::SetAnimSign( bool bAnim )
{
	m_bMoving = bAnim;
	m_SkinMesh->SetAnim(bAnim);
}
inline void Sprite::SetAnimationSet( std::string set, double times )
{
	//设置角色的当前动画
	m_SkinMesh->SetAnimationSet(set, times);
}
inline void Sprite::SetAction(WORD action)
{
	if (m_Action >= A_DEATH )
		return;

	if(action == A_DEATH)
		m_iTime = 0;

	if (m_bMoving && action != A_MOVE)
	{
		m_Action = action;
		SetSpeed(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
		m_SkinMesh->SetAnimationSet(ACTNAME[action], 1);
		m_bMoving = false;
	}
	else
	{
		m_Action = action;
		m_SkinMesh->SetAnimationSet(ACTNAME[action], 1);
	}
}
inline void Sprite::SetEndPoint(D3DXVECTOR3 endpoint)
{
	m_bMoving = true;
	m_V3EndPoint = endpoint;
	//设置旋转的角度
	SetRotateAngle(m_V3EndPoint);
	SetAction(A_MOVE);
}
inline void Sprite::SetPosition(POINT pPosition)
{
	m_SkinMesh->m_matMoveMatrices._41 = pPosition.x;
	m_SkinMesh->m_matMoveMatrices._43 = pPosition.y;

	m_sphere._worldPos = GetVecPos();
}

inline void Sprite::SetHeight(float h)
{
	m_SkinMesh->SetTransHeight(h);
}

inline POINT Sprite::GetPosition()
{
	POINT newpoint;
	newpoint.x = m_SkinMesh->m_matMoveMatrices._41;
	newpoint.y = m_SkinMesh->m_matMoveMatrices._43;
	return newpoint;
}
inline D3DXVECTOR3 Sprite::GetVecPos()
{
	D3DXVECTOR3 newpoint;
	newpoint.x = m_SkinMesh->m_matMoveMatrices._41;
	newpoint.y = m_SkinMesh->m_matMoveMatrices._42;
	newpoint.z = m_SkinMesh->m_matMoveMatrices._43;
	return newpoint;
}
inline void Sprite::SetView(float view)
{
	m_fView = view;
}
inline bool Sprite::GetAttackRound()
{
	if ((m_Action == A_ATTACK || m_Action== A_ATTACK2 || m_Action == A_SPELL || m_Action == A_SPELL2) && m_iAttackTime == m_iMaxAttackCD/2)
		return true;
	return false;
}
inline bool Sprite::GetIsHide()
{
	if (m_Action == A_HIDE)
		return true;
	return false;
}

#endif