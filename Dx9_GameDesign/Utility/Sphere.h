//---------------------------------------------------------------------------
//	k.chen
//	peterchenkai@163.com
//
//---------------------------------------------------------------------------
#pragma once
#ifndef _SPHERE__
#define _SPHERE__

#include "MyMath.h"
#include "Vector.h"
//#include "Frustum.h"

/** ��,��Բ�ĺͰ뾶��ʾ */
class Sphere
{
protected:
    float m_Radius;
    Vector3 m_Center;
public:
    /** Ĭ�Ϲ��캯��������һ����ԭ��ΪԲ�ĵĵ�λԲ */
    Sphere() : m_Radius(1.0), m_Center(Vector3::ZERO) {}
	
    /** ���캯�� 
        @param [const Vector3&] center: Բ��
        @param [float] radius: �뾶
    */
    Sphere(const Vector3& center, float radius)
        : m_Radius(radius), m_Center(center) {}

    /** �õ���İ뾶 */
    float GetRadius(void) const { return m_Radius; }

    /** �ı���İ뾶 */
    void SetRadius(float radius) { m_Radius = radius; }

    /** �õ����Բ������ */
    const Vector3& GetCenter(void) const { return m_Center; }

    /** �ı����Բ������ */
    void SetCenter(const Vector3& center) { m_Center = center; }

	/** �жϵ�ǰ���Ƿ��봫��������ཻ���������ľ��������뾶����� */
	bool Intersects(const Sphere& s) const
	{
		return (s.m_Center - m_Center).LengthSquared() <= ((s.m_Radius + m_Radius) * (s.m_Radius + m_Radius));
	}

	/** �жϸ����Ƿ��ڴ��������ڲ����������ľ���Ͱ뾶����� */
	bool IfInSphere(const Sphere& s) const
	{
		float d = s.m_Radius - m_Radius;
		if ( d < 0 )
			return false;
		return ( (m_Center - s.m_Center).LengthSquared() < d * d );
	}

	/** �жϸ����Ƿ��ڴ��������ⲿ���������ľ���Ͱ뾶����� */
	bool IfOutOfSphere(const Sphere& s) const
	{
		float d = s.m_Radius - m_Radius;
		if ( d > 0 )
			return false;
		return ( (m_Center - s.m_Center).LengthSquared() < d * d );
	}

	/** �жϸ����Ƿ��봫���ƽ���ཻ�������ĵ�ƽ��ľ������İ뾶��Ƚ� */
	bool Intersects(const Plane& plane) const
	{
		return Math::Intersects(*this, plane);
	}
	/** �жϵ������Ƿ��ڸ�����ڲ������õ�����ĵľ�����뾶�Ƚ�. */
	bool Intersects(const Vector3& v) const
	{
		return ((v - m_Center).LengthSquared() <= m_Radius * m_Radius);
	}

	/** �ж�Բ�Ƿ���������ཻ */
	bool Intersects(const Frustum& frustum) const
	{ return Math::Intersects(*this, frustum); }

	/** �ж��������Ƿ���ͬ���뾶��Բ�ľ���ͬ */
	bool operator == (const Sphere& s) const
	{
		if (!RealEqual(m_Radius, s.m_Radius))
			return false;
		if ( !RealEqual(m_Center.x, s.m_Center.x) || !RealEqual(m_Center.y, s.m_Center.y)
			|| !RealEqual(m_Center.z, s.m_Center.z))
			return false;

		return true;
	}

};


#endif
