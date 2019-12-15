// GEMovable.cpp: implementation of the GEMovable class.
//
//////////////////////////////////////////////////////////////////////

#include "MyMovable.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyMovable::MyMovable(const string& name, MyObject * father, const Vector3& offset,
					 float radianRight, float radianUp, float radianDir, bool isVisible )
					 : MyObject(name , father, offset, radianRight, radianUp, radianDir, isVisible)
{
	m_bAuto = false;
	m_fSpeed = 0;
	m_vVelocity.Zero();
	m_vAccl.Zero();
	m_fAccl = 0;
	m_fRotationPitch = m_fRotationYaw = m_fRotationRoll = 0;
}



MyMovable::~MyMovable()
{

}


HRESULT MyMovable::FrameMove(float deltTime)
{
	if (m_bAuto)
	{
		// 自动的情况下会旋转和移动
		RotationPitchYawRoll(m_fRotationPitch * deltTime, m_fRotationYaw * deltTime, m_fRotationRoll * deltTime);
		Move(m_vVelocity * m_fSpeed * deltTime);
	}
	else
	{
		// 非自动
		if ( m_fSpeed != 0 )
			Move(m_vVelocity * m_fSpeed * deltTime);

		if ( m_fAccl != 0 )
		{
			m_vVelocity = m_vVelocity * m_fSpeed + m_vAccl * m_fAccl * deltTime;	// 速度改变
			m_fSpeed = m_vVelocity.Length();
			if (m_fSpeed != 0)
				m_vVelocity /= m_fSpeed;
		}
	}

	return MyObject::FrameMove(deltTime);
}





void MyMovable::JumpBegin(float fSpeedUp, float fAccl)
{
	//	m_vVelocity = (vDirection * m_mTransformation.SetOffset(Vector3::ZERO)).GetNormalized();
	m_vVelocity = m_vVelocity * m_fSpeed + fSpeedUp * Vector3::Y_AXIS;
	m_fSpeed = m_vVelocity.Length();
	m_vVelocity.Normalize();

	m_fAccl = fAccl;
	m_vAccl = Vector3::Y_AXIS;
}

void MyMovable::JumpEnd()
{
	m_vVelocity.y = 0;
	m_fSpeed = (m_vVelocity * m_fSpeed).Length();

	m_vVelocity.Normalize();

	m_fAccl = 0;
	//m_vAccl.Zero();
}


void MyMovable::StopMoving()
{
	m_fSpeed = 0;
	m_fAccl = 0;
	//m_vVelocity.Zero();
	//m_vAccl.Zero();
}


void MyMovable::StopRotate()
{
	m_fRotationPitch = m_fRotationYaw = m_fRotationRoll = 0;
}


void MyMovable::SetVelocityDirectionInWorldAxis( const Vector3& direction )
{ 
	m_vVelocity = direction.GetNormalized(); 
}

void MyMovable::SetVelocityDirectionInUserAxis( const Vector3& direction )
{
	m_vVelocity = direction.GetNormalized() * GetRotationMatrix();
}


