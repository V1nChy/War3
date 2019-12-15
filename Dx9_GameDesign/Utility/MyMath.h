#ifndef _MATHUTIL
#define _MATHUTIL

#include <math.h>
#include <vector>
#include <list>
using namespace std;

class Vector2;
class Vector3;
class Vector4;
class Matrix3;
class Matrix4;
class Plane;
class Ray;
class Sphere;
class Frustum;
template <class T> class Point2D;


#ifndef __GEDL__
#define __GEDL__

#ifdef ENGINE_NONCLIENT_BUILD
#define DLL __declspec( dllexport )
#else
#define DLL __declspec( dllimport )
#endif

#endif


// �꼰����.
#undef  PI
#define	PI					3.141592653589793238462643383279f	
#undef  TWO_PI
#define TWO_PI				6.283185307179586476925286766558f
#undef  HALF_PI
#define HALF_PI				1.5707963267948966192313216916395f

#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER		    (1.0e+10f)

#ifndef RAD_TO_DEG
#define RAD_TO_DEG(r)		(r*57.295779505601046646705075978956f)	 //(180.0f / 3.141592654f)
#endif 

#ifndef DEG_TO_RAD
#define DEG_TO_RAD(d)		(d*0.017453292522222222222222222222222f) //(3.141592654f / 180.0f)
#endif


#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER		    (1.0e+10f)

#define Clamp(x, min, max)  x = (x<min  ? min : x<max ? x : max)

typedef unsigned char uint8;
typedef unsigned int  uint32;


/** �õ��������н�С���Ǹ� */
template <class T>
T GetMin(T a, T b)
{
	return (a < b) ? a : b;
}

/** �õ��������нϴ���Ǹ� */
template <class T>
T GetMax(T a, T b)
{
	return (a > b) ? a : b;
}


/** ʵ���������
 *	@note: �����������Ǹ����̶�,float
 */
template <class T>
bool RealEqual(T a, T b, float tolerance = 1.e-6f)
{
	return (fabsf(a-b) < tolerance ? true: false);
}

namespace Math {

	/** ƽ�����ĵ���
	 *	@remarks: result = 1 / a while a * a = r
	 */
	inline float InvSqrt(float r) { return (1.f / sqrtf(r)); }

	/** �жϹ��������ƽ��p�Ƿ��ཻ
	*	@returns [pair<bool, float>] : ��һ������ֵ��ʾ�Ƿ��ཻ, �ڶ�������ֵ����㵽����ľ���
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Plane& plane);

	/** �жϹ������������p�Ƿ��ཻ
	*	@returns [pair<bool, float>] : ��һ������ֵ��ʾ�Ƿ��ཻ, �ڶ�������ֵ����㵽����ľ���
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Sphere& sphere,
		bool discardInside = true);

	/** �ж������Ƿ��OBB�ཻ
	*  @returns [pair<bool, float>] : ��һ������ֵ��ʾ�Ƿ��ཻ, �ڶ�������ֵ����㵽����ľ���ƽ��
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Vector3& vOBBCenter,
		const Vector3& vOBBRight, const Vector3& vOBBUp, const Vector3& vOBBDir,
		float xLength, float yHeight, float zDepth);

	/** �ж������Ƿ���������ཻ
	*  @returns [pair<bool, float>] : ��һ������ֵ��ʾ�Ƿ��ཻ, �ڶ�������ֵ����㵽����ľ���ƽ��
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Vector3& p1, const Vector3& p2, const Vector3& p3);

	/** ���ߺ�һ������ཻ���
	@param ray : Ҫ��������
	@param plaeList : ������ƽ���б�
	@param normalIsOutside: ƽ��ķ������ڻ��������
	*/
	static std::pair<bool, float> Intersects(
		const Ray& ray, const std::vector<Plane>& planeList,
		bool normalIsOutside);

	Vector3 GetIntersectPointof3Planes(const Plane& plane1, const Plane& plane2, const Plane& plane3);

	/** ���ƽ����ཻ���
	@remarks: �μ� plane.getDistance(sphere.getCenter())
	*/
	bool Intersects(const Sphere& sphere, const Plane& plane);

	/** �����������ཻ��� */
	bool Intersects(const Sphere& sphere, const Frustum& frustum);

	/** ����ռ�����<b>??<\b> */
	Vector3 CalculateTangentSpaceVector(
		const Vector3& position1, const Vector3& position2, const Vector3& position3,
		float u1, float v1, float u2, float v2, float u3, float v3);

	/** ���㴫��ƽ��ķ������ */
	Matrix4 BuildReflectionMatrix(const Plane& p);
	/** ����ƽ�淨��
	*	@note: ���ص�Vector4�е�w�Ǵ�ԭ�㵽ƽ��ľ���,�ҷ��򱻵�λ���� */
	Vector4 CalculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	/** ����ƽ�淨��
	*	@note: �޴�ԭ�㵽ƽ��ľ�����Ϣ,�ҷ��򱻵�λ���� */
	Vector3 CalculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	/** ����ƽ�淨��
	*	@note: ���ص�Vector4�е�w�Ǵ�ԭ�㵽ƽ��ľ���,�ҷ���δ����λ�� */
	Vector4 CalculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	/** ����ƽ�淨��
	*	@note: �޴�ԭ�㵽ƽ��ľ�����Ϣ,�ҷ���δ����λ�� */
	Vector3 CalculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);

}
class Angle 
{
private:
	static Angle* m_pAngle;
	Angle(unsigned int trigTableSize = 4096);
protected:
	
    /// Size of the trig tables as determined by constructor.
    static int c_TrigTableSize;

    /// Radian -> index factor value ( mTrigTableSize / 2 * PI )
    static float c_TrigTableFactor;
    static float* c_SinTable;
	static float* c_CosTable;
    static float* c_TanTable;

    /** ���Ǻ�����ֵ��Ľ���,˽�к���
    */
    void BuildTrigTables();

public:

    /** ���캯��
        @param [unsigned int] trigTableSize : ���Ǻ�����ֵ��Ĵ�С
    */
	static Angle* GetSingleton() {
		return m_pAngle;
	}
	void FinalCleanUp();
    ~Angle();

	/** ���Ǻ�����ֵ��,sin */
	static float SinTable (float fValue);
	/** ���Ǻ�����ֵ��,cos */
	static float CosTable (float fValue);
	/** ���Ǻ�����ֵ��,tan */
	static float TanTable (float fValue);

};


#endif