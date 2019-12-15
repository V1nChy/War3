//---------------------------------------------------------------------------
//	k.chen
//	peterchenkai@163.com
//
//---------------------------------------------------------------------------
 
#ifndef _RAY__
#define _RAY__

#include "Vector.h"

/** Representation of a ray in space, ie a line with an origin and direction. */
class Ray
{
protected:
    Vector3 m_Origin;
    Vector3 m_Direction;
public:
	/** 缺省的构造函数初始化 */
    inline Ray():m_Origin(Vector3::ZERO), m_Direction(Vector3::Z_AXIS) {}

	/** 用两个向量初始化光线的起点和方向 */
    inline Ray(const Vector3& origin, const Vector3& direction)
        :m_Origin(origin), m_Direction(direction) {}
	
    /** 改变光线的起点，将其设置为参数中的origin的向量. */
    inline void SetOrigin(const Vector3& origin) {m_Origin = origin;} 
    /** 得到光线的起点向量. */
    inline const Vector3& GetOrigin(void) const {return m_Origin;} 

    /** 改变光线的方向，将其设置为新的方向dir. */
    inline void SetDirection(const Vector3& dir) {m_Direction = dir;} 
    /** 得到光线的方向向量. */
    inline const Vector3& GetDirection(void) const {return m_Direction;} 

	/** 得到当前光线上距离起点比例为t的向量点 */
	inline Vector3 GetPoint(float t) const { 
		return Vector3(m_Origin + (m_Direction * t));
	}
	
	/** 重载运算符*,得到方向相同，长度为原光线t的新光线的终点向量,与GetPoint相同功能 */
	inline Vector3 operator*(float t) const { 
		return GetPoint(t);
	};

	/** 判断光线与给定平面p是否相交
	 *	@returns [pair<bool, float>] : 第一个返回值表示是否相交, 第二个返回值是起点到交点的距离
	 */
	inline std::pair<bool, float> Intersects(const Plane& p) const
	{
		return Math::Intersects(*this, p);
	}

	/** 判断光线与给定球面p是否相交
	 *	@returns [pair<bool, float>] : 第一个返回值表示是否相交, 第二个返回值是起点到交点的距离
	 */
	inline std::pair<bool, float> Intersects(const Sphere& s) const
	{
		return Math::Intersects(*this, s);
	}

};

#endif

