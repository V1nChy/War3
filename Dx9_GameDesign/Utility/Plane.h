//---------------------------------------------------------------------------
//	k.chen
//	peterchenkai@163.com
//
//---------------------------------------------------------------------------


#ifndef _PLANE__
#define _PLANE__

#include "Vector.h"
#include "Quaternion.h"

/** 3D空间中的平面.
	@remarks
		3D空间中的平面方程如下所示
		Ax + By + Cz + D = 0
	@par
		其中Vector3（A,B,C）是平面的法向，D是原点到平面的距离
*/
class Plane
{	
public:
	Vector3 m_vNormal;
	float m_fD;

	/** 缺省的构造函数初始化，将元素都设为 0.*/
	Plane ();
	/** 拷贝构造函数 */
	void Init (const Plane& rhs);
	/** 用一个向量rkNormal作为平面的法向量，fConstant为原点到平面的距离初始化平面.*/
	void Init (const Vector3& rkNormal, float fConstant);
	/** 用一个向量rkNormal作为平面的法向量，rkPoint为另外一点初始化平面 */
	void Init (const Vector3& rkNormal, const Vector3& rkPoint);
	/** 利用平面上的三点初始化平面，用其中两个向量确定平面法向量，另外一点和法向量的点积为原点到平面的距离 */
	void Init (const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2);

	/** "positive side" 表示与平面法向同向的半个空间。 
		"negative side" 表示与平面法向反向的半个空间 
		"no side" 在平面上
	*/
	enum Side
	{
	NO_SIDE,
	POSITIVE_SIDE,
	NEGATIVE_SIDE
	};

	/** 判定rkPoint在平面的那一侧，对于平面的侧定义了正，负，无，分别表示与法向量同向，反向，在平面上 */
	Side GetSide (const Vector3& rkPoint) const;

	/** 伪距离，如果在面的正面则返回正值，反面则返回负值，0 说明在面上	*/
	float GetDistance (const Vector3& rkPoint) const;

	/** 重载运算符==,判断两个平面是否相等 */
	bool operator==(const Plane& rhs) const
	{	return (rhs.m_fD == m_fD && rhs.m_vNormal == m_vNormal);	}

	/** 重载运算符 << 输出平面 */
	inline friend std::ostream& operator<< (std::ostream& o, Plane& p)
	{
		o << "Plane(normal=" << p.m_vNormal << ", d=" << p.m_fD << ")";
		return o;
	}
};



#endif