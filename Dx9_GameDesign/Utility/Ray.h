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
	/** ȱʡ�Ĺ��캯����ʼ�� */
    inline Ray():m_Origin(Vector3::ZERO), m_Direction(Vector3::Z_AXIS) {}

	/** ������������ʼ�����ߵ����ͷ��� */
    inline Ray(const Vector3& origin, const Vector3& direction)
        :m_Origin(origin), m_Direction(direction) {}
	
    /** �ı���ߵ���㣬��������Ϊ�����е�origin������. */
    inline void SetOrigin(const Vector3& origin) {m_Origin = origin;} 
    /** �õ����ߵ��������. */
    inline const Vector3& GetOrigin(void) const {return m_Origin;} 

    /** �ı���ߵķ��򣬽�������Ϊ�µķ���dir. */
    inline void SetDirection(const Vector3& dir) {m_Direction = dir;} 
    /** �õ����ߵķ�������. */
    inline const Vector3& GetDirection(void) const {return m_Direction;} 

	/** �õ���ǰ�����Ͼ���������Ϊt�������� */
	inline Vector3 GetPoint(float t) const { 
		return Vector3(m_Origin + (m_Direction * t));
	}
	
	/** ���������*,�õ�������ͬ������Ϊԭ����t���¹��ߵ��յ�����,��GetPoint��ͬ���� */
	inline Vector3 operator*(float t) const { 
		return GetPoint(t);
	};

	/** �жϹ��������ƽ��p�Ƿ��ཻ
	 *	@returns [pair<bool, float>] : ��һ������ֵ��ʾ�Ƿ��ཻ, �ڶ�������ֵ����㵽����ľ���
	 */
	inline std::pair<bool, float> Intersects(const Plane& p) const
	{
		return Math::Intersects(*this, p);
	}

	/** �жϹ������������p�Ƿ��ཻ
	 *	@returns [pair<bool, float>] : ��һ������ֵ��ʾ�Ƿ��ཻ, �ڶ�������ֵ����㵽����ľ���
	 */
	inline std::pair<bool, float> Intersects(const Sphere& s) const
	{
		return Math::Intersects(*this, s);
	}

};

#endif

