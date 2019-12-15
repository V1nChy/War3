

#include "Plane.h"
		
//-----------------------------------------------------------------------
Plane::Plane ()
{
	m_vNormal = Vector3::ZERO;
	m_fD = 0.0;
}
//-----------------------------------------------------------------------
void Plane::Init (const Plane& rhs)
{
	m_vNormal = rhs.m_vNormal;
	m_fD = rhs.m_fD;
}
//-----------------------------------------------------------------------
void Plane::Init (const Vector3& rkNormal, float fConstant)
{
	m_vNormal = rkNormal;
	m_fD = fConstant;
}
//-----------------------------------------------------------------------
void Plane::Init (const Vector3& rkNormal, const Vector3& rkPoint)
{
	m_vNormal = rkNormal;
	m_fD = -rkNormal.DotProduction(rkPoint);
}
//-----------------------------------------------------------------------
void Plane::Init (const Vector3& rkPoint0, const Vector3& rkPoint1,
	const Vector3& rkPoint2)
{
	Vector3 kEdge1 = rkPoint1 - rkPoint0;
	Vector3 kEdge2 = rkPoint2 - rkPoint0;
	m_vNormal = kEdge1.CrossProduction(kEdge2);
	m_vNormal.Normalize();
	m_fD = -m_vNormal.DotProduction(rkPoint0);
}
//-----------------------------------------------------------------------
float Plane::GetDistance (const Vector3& rkPoint) const
{
	return m_vNormal.DotProduction(rkPoint) + m_fD;
}
//-----------------------------------------------------------------------
Plane::Side Plane::GetSide (const Vector3& rkPoint) const
{
	float fDistance = GetDistance(rkPoint);

	if ( fDistance < 0.0 )
		return Plane::NEGATIVE_SIDE;

	if ( fDistance > 0.0 )
		return Plane::POSITIVE_SIDE;

	return Plane::NO_SIDE;
}
