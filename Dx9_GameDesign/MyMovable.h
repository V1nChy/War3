// GEMovable.h: interface for the GEMovable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEMOVABLE_H__645FC162_7D25_4F2E_9069_C2400007ED1F__INCLUDED_)
#define AFX_GEMOVABLE_H__645FC162_7D25_4F2E_9069_C2400007ED1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"

/** 运动物体 */
class MyMovable : public MyObject
{
protected:
	float m_fSpeed;			///< 速度，标量
	Vector3 m_vVelocity;	///< 速度方向,矢量	
	float m_fRotationPitch;	///< 绕自身的x轴旋转的速度
	float m_fRotationYaw;	///< 绕自身的y轴旋转的速度
	float m_fRotationRoll;	///< 绕自身的z轴旋转的速度

	Vector3 m_vAccl;		///< 加速度方向,矢量
	float   m_fAccl;		///< 加速度标量

	bool m_bAuto;			///< 是否是自动的，规律运动的物体

public:
	MyMovable(const string& name, MyObject * father, const Vector3& offset = Vector3::ZERO,
		float radianX = 0.f, float radianY = 0.f, float radianZ = 0.f, bool isVisible = true );

	virtual ~MyMovable();

	/** 设置速度，标量。注意，这个速度是相当与父结点的速度 */
	inline void SetSpeed(float speed) { m_fSpeed = speed; }
	/** 得到速度， 注意，这个速度是相当与父结点的速度 */
	inline float GetSpeed() const { return m_fSpeed; }

	/** 设置加速度 \n
	 *	@param [float] fAccl : 加速度标量
	 *	@param [const Vector3&] vAccl : 加速度方向, 也是相对于父结点的
	 */
	inline void SetAccel(float fAccl, const Vector3& vAccl)
	{ m_fAccl = fAccl; 	m_vAccl = vAccl.GetNormalized(); }
	/** 得到加速度的值,标量 */
	inline float GetAccelValue() const { return m_fAccl; }
	/** 得到加速度方向,返回的是个单位向量 */
	inline Vector3 GetAccelDirection() const { return m_vAccl; }



	/** 设置速度的方向, 传入的不要求一定是单位向量 
	 *	@note: 速度的方向是在世界坐标系中
	 */
	void SetVelocityDirectionInWorldAxis( const Vector3& direction );
	/** 设置速度的方向, 传入的不要求一定是单位向量 
	*	@note: 速度的方向是在用户坐标系中
	*/
	void SetVelocityDirectionInUserAxis(const Vector3& direction );
	/** 得到速度方向, 矢量, 世界坐标下的 */
	inline Vector3 GetVelocityDirection() const { return m_vVelocity; }
	/** 得到速度, 矢量, 带方向和大小信息, 返回的速度是在世界坐标下的 */
	inline Vector3 GetVelocity() const { return m_vVelocity * m_fSpeed; }


	/** 设置旋转, 是围绕自身的轴旋转 */
	inline void SetRotationPitch(float pitch) { m_fRotationPitch = pitch; }
	inline void SetRotationYaw(float yaw) { m_fRotationYaw = yaw; }
	inline void SetRotationRoll(float roll) { m_fRotationRoll = roll; }
	inline void SetRotation(float pitch, float yaw, float roll)

	{
		m_fRotationPitch = pitch;
		m_fRotationYaw = yaw;
		m_fRotationRoll = roll;
	}

	/** 向上跳起, 传入起跳速度和重力加速度
	 *	@note: 前后左右的方向不变
	 */
	virtual void JumpBegin(float fSpeedUp, float fAccl = -9.8f);
	/** 结束跳跃, 即落地 */
	virtual void JumpEnd();
	/** 相对于父亲静止 */
	void StopMoving();
	/** 结束跳跃, 即落地 */
	void StopRotate();

	virtual HRESULT FrameMove(float deltTime);


	/** 是否是自动的物体 */
	void SetAuto(bool isAuto) { m_bAuto = isAuto; }
};

#endif // !defined(AFX_GEMOVABLE_H__645FC162_7D25_4F2E_9069_C2400007ED1F__INCLUDED_)
