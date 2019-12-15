//---------------------------------------------------------------------------
//	k.chen
//	peterchenkai@163.com
//
//---------------------------------------------------------------------------


#ifndef _PLANE__
#define _PLANE__

#include "Vector.h"
#include "Quaternion.h"

/** 3D�ռ��е�ƽ��.
	@remarks
		3D�ռ��е�ƽ�淽��������ʾ
		Ax + By + Cz + D = 0
	@par
		����Vector3��A,B,C����ƽ��ķ���D��ԭ�㵽ƽ��ľ���
*/
class Plane
{	
public:
	Vector3 m_vNormal;
	float m_fD;

	/** ȱʡ�Ĺ��캯����ʼ������Ԫ�ض���Ϊ 0.*/
	Plane ();
	/** �������캯�� */
	void Init (const Plane& rhs);
	/** ��һ������rkNormal��Ϊƽ��ķ�������fConstantΪԭ�㵽ƽ��ľ����ʼ��ƽ��.*/
	void Init (const Vector3& rkNormal, float fConstant);
	/** ��һ������rkNormal��Ϊƽ��ķ�������rkPointΪ����һ���ʼ��ƽ�� */
	void Init (const Vector3& rkNormal, const Vector3& rkPoint);
	/** ����ƽ���ϵ������ʼ��ƽ�棬��������������ȷ��ƽ�淨����������һ��ͷ������ĵ��Ϊԭ�㵽ƽ��ľ��� */
	void Init (const Vector3& rkPoint0, const Vector3& rkPoint1, const Vector3& rkPoint2);

	/** "positive side" ��ʾ��ƽ�淨��ͬ��İ���ռ䡣 
		"negative side" ��ʾ��ƽ�淨����İ���ռ� 
		"no side" ��ƽ����
	*/
	enum Side
	{
	NO_SIDE,
	POSITIVE_SIDE,
	NEGATIVE_SIDE
	};

	/** �ж�rkPoint��ƽ�����һ�࣬����ƽ��Ĳඨ�������������ޣ��ֱ��ʾ�뷨����ͬ�򣬷�����ƽ���� */
	Side GetSide (const Vector3& rkPoint) const;

	/** α���룬�������������򷵻���ֵ�������򷵻ظ�ֵ��0 ˵��������	*/
	float GetDistance (const Vector3& rkPoint) const;

	/** ���������==,�ж�����ƽ���Ƿ���� */
	bool operator==(const Plane& rhs) const
	{	return (rhs.m_fD == m_fD && rhs.m_vNormal == m_vNormal);	}

	/** ��������� << ���ƽ�� */
	inline friend std::ostream& operator<< (std::ostream& o, Plane& p)
	{
		o << "Plane(normal=" << p.m_vNormal << ", d=" << p.m_fD << ")";
		return o;
	}
};



#endif