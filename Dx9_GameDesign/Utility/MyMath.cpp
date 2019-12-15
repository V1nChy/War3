#include "MyMath.h" 
#include "plane.h"
#include "vector.h"
#include "matrix.h"
#include "sphere.h"
#include "ray.h"
#include "Frustum.h"

#include <limits>
#include <algorithm>
using namespace std;


Angle *Angle::m_pAngle = new Angle;

int Angle::c_TrigTableSize;

float  Angle::c_TrigTableFactor;
float *Angle::c_SinTable = NULL;
float *Angle::c_CosTable = NULL;
float *Angle::c_TanTable = NULL;

Angle::Angle( unsigned int trigTableSize )
{
    c_TrigTableSize = trigTableSize;
    c_TrigTableFactor = c_TrigTableSize / TWO_PI;

    c_SinTable = new float[c_TrigTableSize];
	c_CosTable = new float[c_TrigTableSize];
    c_TanTable = new float[c_TrigTableSize];

    BuildTrigTables();
}

//-----------------------------------------------------------------------
Angle::~Angle()
{
}
void Angle::FinalCleanUp()
{
	delete[] c_SinTable;
	delete[] c_CosTable;
	delete[] c_TanTable;
}
//-----------------------------------------------------------------------
void Angle::BuildTrigTables(void)
{
    // Build trig lookup tables
    // Could get away with building only PI sized Sin table but simpler this 
    // way. Who cares, it'll ony use an extra 8k of memory anyway and I like 
    // simplicity.
    float angle;
    for (int i = 0; i < c_TrigTableSize; ++i)
    {
        angle = TWO_PI * i / c_TrigTableSize;
        c_SinTable[i] = sinf(angle);
		c_CosTable[i] = cosf(angle);
        c_TanTable[i] = tanf(angle);
    }
}
//-----------------------------------------------------------------------	
float Angle::SinTable (float fValue)
{
    // Convert range to index values, wrap if required
    int idx;
    if (fValue >= 0)
    {
        idx = int(fValue * c_TrigTableFactor) % c_TrigTableSize;
    }
    else
    {
        idx = c_TrigTableSize - (int(-fValue * c_TrigTableFactor) % c_TrigTableSize) - 1;
    }

    return c_SinTable[idx];
}

//-----------------------------------------------------------------------	
float Angle::CosTable (float fValue)
{
	// Convert range to index values, wrap if required
	int idx;
	if (fValue >= 0)
	{
		idx = int(fValue * c_TrigTableFactor) % c_TrigTableSize;
	}
	else
	{
		idx = c_TrigTableSize - (int(-fValue * c_TrigTableFactor) % c_TrigTableSize) - 1;
	}

	return c_CosTable[idx];
}

//-----------------------------------------------------------------------
float Angle::TanTable (float fValue)
{
    // Convert range to index values, wrap if required
	int idx = int(fValue *= c_TrigTableFactor) % c_TrigTableSize;
	return c_TanTable[idx];
}

//-----------------------------------------------------------------------
namespace Math {
	std::pair<bool, float> Intersects(const Ray& ray, const Plane& plane)
	{

		float denom = plane.m_vNormal.DotProduction(ray.GetDirection());
		if (fabsf(denom) < std::numeric_limits<float>::epsilon())
		{
			// Parallel
			return std::pair<bool, float>(false, 0);
		}
		else
		{
			float nom = plane.m_vNormal.DotProduction(ray.GetOrigin()) + plane.m_fD;
			float t = -(nom / denom);
			return std::pair<bool, float>(t >= 0, t);
		}

	}
	//-----------------------------------------------------------------------
	std::pair<bool, float> Intersects(const Ray& ray,
		const std::vector<Plane>& planes, bool normalIsOutside)
	{
		std::vector<Plane>::const_iterator planeit, planeitend;
		planeitend = planes.end();
		bool allInside = true;
		std::pair<bool, float> ret;
		ret.first = false;
		ret.second = 0.0f;

		// derive side
		// NB we don't pass directly since that would require Plane::Side in 
		// interface, which results in recursive includes since Math is so fundamental
		Plane::Side outside = normalIsOutside ? Plane::POSITIVE_SIDE : Plane::NEGATIVE_SIDE;

		for (planeit = planes.begin(); planeit != planeitend; ++planeit)
		{
			const Plane& plane = *planeit;
			// is origin outside?
			if (plane.GetSide(ray.GetOrigin()) == outside)
			{
				allInside = false;
				// Test single plane
				std::pair<bool, float> planeRes =
					ray.Intersects(plane);
				if (planeRes.first)
				{
					// Ok, we intersected
					ret.first = true;
					// Use the most distant result since convex volume
					ret.second = GetMax(ret.second, planeRes.second);
				}
			}
		}

		if (allInside)
		{
			// Intersecting at 0 distance since inside the volume!
			ret.first = true;
			ret.second = 0.0f;
		}

		return ret;
	}

	//-----------------------------------------------------------------------
	std::pair<bool, float> Intersects(const Ray& ray, const Sphere& sphere,
		bool discardInside)
	{
		const Vector3& raydir = ray.GetDirection();
		// Adjust ray origin relative to sphere center
		const Vector3& rayorig = ray.GetOrigin() - sphere.GetCenter();
		float radius = sphere.GetRadius();

		// Check origin inside first
		if (rayorig.LengthSquared() <= radius*radius && discardInside)
		{
			return std::pair<bool, float>(true, 0);
		}

		// Mmm, quadratics
		// Build coeffs which can be used with std quadratic solver
		// ie t = (-b +/- sqrt(b*b + 4ac)) / 2a
		float a = raydir.DotProduction(raydir);
		float b = 2 * rayorig.DotProduction(raydir);
		float c = rayorig.DotProduction(rayorig) - radius*radius;

		// Calc determinant
		float d = (b*b) - (4 * a * c);
		if (d < 0)
		{
			// No intersection
			return std::pair<bool, float>(false, 0);
		}
		else
		{
			// BTW, if d=0 there is one intersection, if d > 0 there are 2
			// But we only want the closest one, so that's ok, just use the 
			// '-' version of the solver
			float t = (-b - sqrtf(d)) / (2 * a);
			if (t < 0)
				t = (-b + sqrtf(d)) / (2 * a);
			return std::pair<bool, float>(true, t);
		}


	}


	std::pair<bool, float> Intersects(const Ray& ray, const Vector3& vOBBCenter,
		const Vector3& vOBBRight, const Vector3& vOBBUp, const Vector3& vOBBDir,
		float xLength, float yHeight, float zDepth)
	{
		const Vector3* directXYZ[3];
		float lenXYZ[3];
		float t_Min = numeric_limits<float>::min();
		float t_Max = numeric_limits<float>::max();

		Vector3 tempP = vOBBCenter - ray.GetOrigin();
		Vector3 lineD = ray.GetDirection();
		directXYZ[0] = &vOBBRight;
		directXYZ[1] = &vOBBUp;
		directXYZ[2] = &vOBBDir;
		lenXYZ[0] = xLength*0.5f;
		lenXYZ[1] = yHeight*0.5f;
		lenXYZ[2] = zDepth *0.5f;

		float tempE;
		float tempF;
		float t1;
		float t2;
		for (int i = 0; i < 3; ++i)
		{
			tempE = directXYZ[i]->DotProduction(tempP);
			tempF = directXYZ[i]->DotProduction(lineD);
			if (!RealEqual(tempF, 0.f, SMALL_NUMBER))
			{
				t1 = (tempE + lenXYZ[i]) / tempF;
				t2 = (tempE - lenXYZ[i]) / tempF;
				if (t1 > t2)
					swap(t1, t2);
				if (t1 > t_Min) t_Min = t1;
				if (t2 < t_Max) t_Max = t2;
				if (t_Min > t_Max || t_Max < 0)
					return make_pair(false, 0.f);
			}
			else if ((-tempE - lenXYZ[i] > 0) || (-tempE + lenXYZ[i]) < 0)
				return make_pair(false, 0.f);
		}

		if (t_Min > 0)
		{
			return make_pair(true, (ray.GetOrigin() + t_Min * lineD).LengthSquared());
		}
		else
		{
			return make_pair(true, (ray.GetOrigin() + t_Max * lineD).LengthSquared());
		}

	}


	//-----------------------------------------------------------------------
	bool Intersects(const Sphere& sphere, const Plane& plane)
	{
		return (
			fabsf(plane.m_vNormal.DotProduction(sphere.GetCenter()))
			<= sphere.GetRadius());
	}


	bool Intersects(const Sphere& sphere, const Frustum& frustum)
	{
		if (frustum.GetPlane(PLANE_LEFT)->GetDistance(sphere.GetCenter()) > sphere.GetRadius())
			return false;
		if (frustum.GetPlane(PLANE_RIGHT)->GetDistance(sphere.GetCenter()) > sphere.GetRadius())
			return false;
		if (frustum.GetPlane(PLANE_TOP)->GetDistance(sphere.GetCenter()) > sphere.GetRadius())
			return false;
		if (frustum.GetPlane(PLANE_BOTTOM)->GetDistance(sphere.GetCenter()) > sphere.GetRadius())
			return false;
		if (frustum.GetPlane(PLANE_FRONT)->GetDistance(sphere.GetCenter()) > sphere.GetRadius())
			return false;
		if (frustum.GetPlane(PLANE_BACK)->GetDistance(sphere.GetCenter()) > sphere.GetRadius())
			return false;

		return true;
	}


	pair<bool, float> Intersects(const Ray& ray, const Vector3& v0, const Vector3& v1, const Vector3& v2)
	{
		Vector3 e1 = v1 - v0;
		Vector3 e2 = v2 - v0;
		Vector3 rDir = ray.GetDirection();
		Vector3 rOrigin = ray.GetOrigin();

		// 排除对背向三角形计算，rDir点乘三角形法向>0 则是背向  (实际上是-e2 * e1)
		if (rDir.DotProduction(Vector3::CrossNoNorm(e1, e2)) > 0.f)
		{
			return pair<bool, float>(false, 0);
		}

		Vector3 p = Vector3::CrossNoNorm(rDir, e2);
		float a = e1.DotProduction(p);
		if (a > -SMALL_NUMBER && a < SMALL_NUMBER)
		{
			return pair<bool, float>(false, 0);
		}

		float f = 1 / a;
		Vector3 s = rOrigin - v0;
		float u = f * (s.DotProduction(p));
		if (u < 0.f || u > 1.f)
		{
			return pair<bool, float>(false, 0);
		}

		Vector3 q = Vector3::CrossNoNorm(s, e1);
		float v = f * (rDir.DotProduction(q));
		if (v < 0.f || u + v > 1.f)
		{
			return pair<bool, float>(false, 0);
		}

		float t = f * (e2.DotProduction(q));

		// 排除背后的三角形
		if (t < 0.f)
			return pair<bool, float>(false, 0);

		// 返回的第两个参数是距离的平方,主要用来做比较
		float len = (t * rDir).LengthSquared();
		return pair<bool, float>(true, len);

	}


	Vector3 GetIntersectPointof3Planes(const Plane& plane1, const Plane& plane2, const Plane& plane3)
	{
		Matrix3 mat;
		Vector3 returnVec;
		Vector3 vecD = Vector3(-plane1.m_fD, -plane2.m_fD, -plane3.m_fD);
		mat.SetRow(0, plane1.m_vNormal);
		mat.SetRow(1, plane2.m_vNormal);
		mat.SetRow(2, plane3.m_vNormal);

		returnVec = mat.Inverse() * vecD;

		return returnVec;
	}

	//-----------------------------------------------------------------------
	Vector3 CalculateTangentSpaceVector(
		const Vector3& position1, const Vector3& position2, const Vector3& position3,
		float u1, float v1, float u2, float v2, float u3, float v3)
	{
		//side0 is the vector along one side of the triangle of vertices passed in, 
		//and side1 is the vector along another side. Taking the cross product of these returns the normal.
		Vector3 side0 = position1 - position2;
		Vector3 side1 = position3 - position1;
		//Calculate face normal
		Vector3 normal = side1.CrossProduction(side0);
		normal.Normalize();
		//Now we use a formula to calculate the tangent. 
		float deltaV0 = v1 - v2;
		float deltaV1 = v3 - v1;
		Vector3 tangent = deltaV1 * side0 - deltaV0 * side1;
		tangent.Normalize();
		//Calculate binormal
		float deltaU0 = u1 - u2;
		float deltaU1 = u3 - u1;
		Vector3 binormal = deltaU1 * side0 - deltaU0 * side1;
		binormal.Normalize();
		//Now, we take the cross product of the tangents to get a vector which 
		//should point in the same direction as our normal calculated above. 
		//If it points in the opposite direction (the dot product between the normals is less than zero), 
		//then we need to reverse the s and t tangents. 
		//This is because the triangle has been mirrored when going from tangent space to object space.
		//reverse tangents if necessary
		Vector3 tangentCross = tangent.CrossProduction(binormal);
		if (tangentCross.DotProduction(normal) < 0.0f)
		{
			tangent = -tangent;
			binormal = -binormal;
		}

		return tangent;

	}

	//-----------------------------------------------------------------------
	Matrix4 BuildReflectionMatrix(const Plane& p)
	{
		return Matrix4(
			-2 * p.m_vNormal.x * p.m_vNormal.x + 1, -2 * p.m_vNormal.x * p.m_vNormal.y, -2 * p.m_vNormal.x * p.m_vNormal.z, -2 * p.m_vNormal.x * p.m_fD,
			-2 * p.m_vNormal.y * p.m_vNormal.x, -2 * p.m_vNormal.y * p.m_vNormal.y + 1, -2 * p.m_vNormal.y * p.m_vNormal.z, -2 * p.m_vNormal.y * p.m_fD,
			-2 * p.m_vNormal.z * p.m_vNormal.x, -2 * p.m_vNormal.z * p.m_vNormal.y, -2 * p.m_vNormal.z * p.m_vNormal.z + 1, -2 * p.m_vNormal.z * p.m_fD,
			0, 0, 0, 1);
	}
	//-----------------------------------------------------------------------
	Vector4 CalculateFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = CalculateBasicFaceNormal(v1, v2, v3);
		// Now set up the w (distance of tri from origin
		return Vector4(normal.x, normal.y, normal.z, -(normal.DotProduction(v1)));
	}
	//-----------------------------------------------------------------------
	Vector3 CalculateBasicFaceNormal(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = (v2 - v1).CrossProduction(v3 - v1);
		normal.Normalize();
		return normal;
	}
	//-----------------------------------------------------------------------
	Vector4 CalculateFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = CalculateBasicFaceNormalWithoutNormalize(v1, v2, v3);
		// Now set up the w (distance of tri from origin)
		return Vector4(normal.x, normal.y, normal.z, -(normal.DotProduction(v1)));
	}
	//-----------------------------------------------------------------------
	Vector3 CalculateBasicFaceNormalWithoutNormalize(const Vector3& v1, const Vector3& v2, const Vector3& v3)
	{
		Vector3 normal = (v2 - v1).CrossProduction(v3 - v1);
		return normal;
	}
}