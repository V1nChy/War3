// GEMovable.h: interface for the GEMovable class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEMOVABLE_H__645FC162_7D25_4F2E_9069_C2400007ED1F__INCLUDED_)
#define AFX_GEMOVABLE_H__645FC162_7D25_4F2E_9069_C2400007ED1F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MyObject.h"

/** �˶����� */
class MyMovable : public MyObject
{
protected:
	float m_fSpeed;			///< �ٶȣ�����
	Vector3 m_vVelocity;	///< �ٶȷ���,ʸ��	
	float m_fRotationPitch;	///< �������x����ת���ٶ�
	float m_fRotationYaw;	///< �������y����ת���ٶ�
	float m_fRotationRoll;	///< �������z����ת���ٶ�

	Vector3 m_vAccl;		///< ���ٶȷ���,ʸ��
	float   m_fAccl;		///< ���ٶȱ���

	bool m_bAuto;			///< �Ƿ����Զ��ģ������˶�������

public:
	MyMovable(const string& name, MyObject * father, const Vector3& offset = Vector3::ZERO,
		float radianX = 0.f, float radianY = 0.f, float radianZ = 0.f, bool isVisible = true );

	virtual ~MyMovable();

	/** �����ٶȣ�������ע�⣬����ٶ����൱�븸�����ٶ� */
	inline void SetSpeed(float speed) { m_fSpeed = speed; }
	/** �õ��ٶȣ� ע�⣬����ٶ����൱�븸�����ٶ� */
	inline float GetSpeed() const { return m_fSpeed; }

	/** ���ü��ٶ� \n
	 *	@param [float] fAccl : ���ٶȱ���
	 *	@param [const Vector3&] vAccl : ���ٶȷ���, Ҳ������ڸ�����
	 */
	inline void SetAccel(float fAccl, const Vector3& vAccl)
	{ m_fAccl = fAccl; 	m_vAccl = vAccl.GetNormalized(); }
	/** �õ����ٶȵ�ֵ,���� */
	inline float GetAccelValue() const { return m_fAccl; }
	/** �õ����ٶȷ���,���ص��Ǹ���λ���� */
	inline Vector3 GetAccelDirection() const { return m_vAccl; }



	/** �����ٶȵķ���, ����Ĳ�Ҫ��һ���ǵ�λ���� 
	 *	@note: �ٶȵķ���������������ϵ��
	 */
	void SetVelocityDirectionInWorldAxis( const Vector3& direction );
	/** �����ٶȵķ���, ����Ĳ�Ҫ��һ���ǵ�λ���� 
	*	@note: �ٶȵķ��������û�����ϵ��
	*/
	void SetVelocityDirectionInUserAxis(const Vector3& direction );
	/** �õ��ٶȷ���, ʸ��, ���������µ� */
	inline Vector3 GetVelocityDirection() const { return m_vVelocity; }
	/** �õ��ٶ�, ʸ��, ������ʹ�С��Ϣ, ���ص��ٶ��������������µ� */
	inline Vector3 GetVelocity() const { return m_vVelocity * m_fSpeed; }


	/** ������ת, ��Χ�����������ת */
	inline void SetRotationPitch(float pitch) { m_fRotationPitch = pitch; }
	inline void SetRotationYaw(float yaw) { m_fRotationYaw = yaw; }
	inline void SetRotationRoll(float roll) { m_fRotationRoll = roll; }
	inline void SetRotation(float pitch, float yaw, float roll)

	{
		m_fRotationPitch = pitch;
		m_fRotationYaw = yaw;
		m_fRotationRoll = roll;
	}

	/** ��������, ���������ٶȺ��������ٶ�
	 *	@note: ǰ�����ҵķ��򲻱�
	 */
	virtual void JumpBegin(float fSpeedUp, float fAccl = -9.8f);
	/** ������Ծ, ����� */
	virtual void JumpEnd();
	/** ����ڸ��׾�ֹ */
	void StopMoving();
	/** ������Ծ, ����� */
	void StopRotate();

	virtual HRESULT FrameMove(float deltTime);


	/** �Ƿ����Զ������� */
	void SetAuto(bool isAuto) { m_bAuto = isAuto; }
};

#endif // !defined(AFX_GEMOVABLE_H__645FC162_7D25_4F2E_9069_C2400007ED1F__INCLUDED_)
