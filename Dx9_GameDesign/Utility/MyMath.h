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


// 宏及常量.
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


/** 得到两个数中较小的那个 */
template <class T>
T GetMin(T a, T b)
{
	return (a < b) ? a : b;
}

/** 得到两个数中较大的那个 */
template <class T>
T GetMax(T a, T b)
{
	return (a > b) ? a : b;
}


/** 实数级的相等
 *	@note: 第三个参数是个容忍度,float
 */
template <class T>
bool RealEqual(T a, T b, float tolerance = 1.e-6f)
{
	return (fabsf(a-b) < tolerance ? true: false);
}

namespace Math {

	/** 平方根的倒数
	 *	@remarks: result = 1 / a while a * a = r
	 */
	inline float InvSqrt(float r) { return (1.f / sqrtf(r)); }

	/** 判断光线与给定平面p是否相交
	*	@returns [pair<bool, float>] : 第一个返回值表示是否相交, 第二个返回值是起点到交点的距离
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Plane& plane);

	/** 判断光线与给定球面p是否相交
	*	@returns [pair<bool, float>] : 第一个返回值表示是否相交, 第二个返回值是起点到交点的距离
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Sphere& sphere,
		bool discardInside = true);

	/** 判断射线是否和OBB相交
	*  @returns [pair<bool, float>] : 第一个返回值表示是否相交, 第二个返回值是起点到交点的距离平方
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Vector3& vOBBCenter,
		const Vector3& vOBBRight, const Vector3& vOBBUp, const Vector3& vOBBDir,
		float xLength, float yHeight, float zDepth);

	/** 判断射线是否和三角形相交
	*  @returns [pair<bool, float>] : 第一个返回值表示是否相交, 第二个返回值是起点到交点的距离平方
	*/
	std::pair<bool, float> Intersects(const Ray& ray, const Vector3& p1, const Vector3& p2, const Vector3& p3);

	/** 射线和一个体的相交检测
	@param ray : 要检测的射线
	@param plaeList : 组成体的平面列表
	@param normalIsOutside: 平面的法向向内还是向外的
	*/
	static std::pair<bool, float> Intersects(
		const Ray& ray, const std::vector<Plane>& planeList,
		bool normalIsOutside);

	Vector3 GetIntersectPointof3Planes(const Plane& plane1, const Plane& plane2, const Plane& plane3);

	/** 球和平面的相交检测
	@remarks: 参见 plane.getDistance(sphere.getCenter())
	*/
	bool Intersects(const Sphere& sphere, const Plane& plane);

	/** 球和六面体的相交检测 */
	bool Intersects(const Sphere& sphere, const Frustum& frustum);

	/** 计算空间向量<b>??<\b> */
	Vector3 CalculateTangentSpaceVector(
		const Vector3& position1, const Vector3& position2, const Vector3& position3,
		float u1, float v1, float u2, float v2, float u3, float v3);

	/** 计算传入平面的反射矩阵 */
	Matrix4 BuildReflectionMatrix(const Plane& p);
	/** 计算平面法向
	*	@note: 返回的Vector4中的w是从原点到平面的距离,且法向被单位化了 */
	Vector4 CalculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	/** 计算平面法向
	*	@note: 无从原点到平面的距离信息,且法向被单位化了 */
	Vector3 CalculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	/** 计算平面法向
	*	@note: 返回的Vector4中的w是从原点到平面的距离,且法向未被单位化 */
	Vector4 CalculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	/** 计算平面法向
	*	@note: 无从原点到平面的距离信息,且法向未被单位化 */
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

    /** 三角函数查值表的建立,私有函数
    */
    void BuildTrigTables();

public:

    /** 构造函数
        @param [unsigned int] trigTableSize : 三角函数查值表的大小
    */
	static Angle* GetSingleton() {
		return m_pAngle;
	}
	void FinalCleanUp();
    ~Angle();

	/** 三角函数查值表,sin */
	static float SinTable (float fValue);
	/** 三角函数查值表,cos */
	static float CosTable (float fValue);
	/** 三角函数查值表,tan */
	static float TanTable (float fValue);

};


#endif