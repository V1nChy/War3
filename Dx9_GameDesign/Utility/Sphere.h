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

/** 球,用圆心和半径表示 */
class Sphere
{
protected:
    float m_Radius;
    Vector3 m_Center;
public:
    /** 默认构造函数，创建一个以原点为圆心的单位圆 */
    Sphere() : m_Radius(1.0), m_Center(Vector3::ZERO) {}
	
    /** 构造函数 
        @param [const Vector3&] center: 圆心
        @param [float] radius: 半径
    */
    Sphere(const Vector3& center, float radius)
        : m_Radius(radius), m_Center(center) {}

    /** 得到球的半径 */
    float GetRadius(void) const { return m_Radius; }

    /** 改变球的半径 */
    void SetRadius(float radius) { m_Radius = radius; }

    /** 得到球的圆心坐标 */
    const Vector3& GetCenter(void) const { return m_Center; }

    /** 改变球的圆心坐标 */
    void SetCenter(const Vector3& center) { m_Center = center; }

	/** 判断当前球是否与传入的球有相交，利用球心距离和两球半径和相比 */
	bool Intersects(const Sphere& s) const
	{
		return (s.m_Center - m_Center).LengthSquared() <= ((s.m_Radius + m_Radius) * (s.m_Radius + m_Radius));
	}

	/** 判断该球是否在传入的球的内部，利用球心距离和半径差相比 */
	bool IfInSphere(const Sphere& s) const
	{
		float d = s.m_Radius - m_Radius;
		if ( d < 0 )
			return false;
		return ( (m_Center - s.m_Center).LengthSquared() < d * d );
	}

	/** 判断该球是否在传入的球的外部，利用球心距离和半径差相比 */
	bool IfOutOfSphere(const Sphere& s) const
	{
		float d = s.m_Radius - m_Radius;
		if ( d > 0 )
			return false;
		return ( (m_Center - s.m_Center).LengthSquared() < d * d );
	}

	/** 判断该球是否与传入的平面相交，用球心到平面的距离和球的半径相比较 */
	bool Intersects(const Plane& plane) const
	{
		return Math::Intersects(*this, plane);
	}
	/** 判断点向量是否在该球的内部，利用点和球心的距离与半径比较. */
	bool Intersects(const Vector3& v) const
	{
		return ((v - m_Center).LengthSquared() <= m_Radius * m_Radius);
	}

	/** 判断圆是否和六面体相交 */
	bool Intersects(const Frustum& frustum) const
	{ return Math::Intersects(*this, frustum); }

	/** 判断两个球是否相同，半径和圆心均相同 */
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
