
//---------------------------------------------------------------------------
//	k.chen
//	peterchenkai@163.com
//
//---------------------------------------------------------------------------

#ifndef _MVECTOR
#define _MVECTOR

#include "MyMath.h"
#include <assert.h>
#include <iostream>
using namespace std;

/** 
 * ��ά����, (x,y), Ҳ���Ա�ʾ��ά�ռ�ĵ�
 */
class Vector2
{
public:
	float x, y;

	/** Ĭ�Ϲ��캯���������� */
	inline Vector2() :	x(0), y(0)
	{}

	/** ���캯�� */
	inline Vector2( float InX, float InY )
	:	x(InX), y(InY)
	{}

	/** �������캯�� */
	inline Vector2(const Vector2& v) : x(v.x), y(v.y)
	{}

	/** ��������0��1������ɱ��Ķ� */
	inline float& operator [] (int i)				
	{ return (reinterpret_cast<float*>(this))[i]; }

	/** ��������0��1��������ɱ��Ķ� */
	inline float operator [] (int i) const			
	{ return (reinterpret_cast<const float*>(this))[i]; }

	/** ��ʼ���������������ֱ𸳸�x��y */
	inline void operator() (float nx, float ny)	
	{x = nx; y = ny;}	

	/** �Ƚ�������ά�����Ƿ���� \n
	 * @param [const Vector2&]: �Ƚ�����������
	 * @return [bool]: �Ƿ�һ��
	 */
	inline bool operator == (const Vector2& w) const
	{return RealEqual(x, w.x) && RealEqual(y, w.y);}

	
	/** �������� */
	inline Vector2 operator - () const
	{
		Vector2 vNeg;
		vNeg.x = -x; vNeg.y = -y;
		return vNeg;
	}

	/** ��ά�������ţ�this *= float; \n
	 * @return [Vector2&] : ����Ǹ�����,���Զ��������޸�
	 */
	inline Vector2& operator *= (float s)			
	{x *= s; y *= s; return *this;}	
	
	/** ��ά������ӣ�Result = this + Vector2; */
	inline Vector2 operator+( const Vector2& V ) const	
	{return Vector2( x + V.x, y + V.y);}

	/** ��ά���������Result = this - Vector2; */
	inline Vector2 operator-( const Vector2& V ) const 
	{return Vector2( x - V.x, y - V.y);}

	/** ��ά�������ţ�this = this / float; \n
	* @return [Vector2&] : ����Ǹ�����,���Զ��������޸�
	*/
	inline Vector2& operator /= (float s)
	{
		assert( fabsf(s) > SMALL_NUMBER );
		x /= s; y /= s;
		return *this;
	}	

	/** ��ά�������ţ�result = this / float; */
	inline Vector2 operator/( float s ) const 
	{
		assert( fabsf(s) > SMALL_NUMBER );
		return Vector2( x / s, y / s);
	}

	/** ����������: return = float * Vector2; */
	inline friend Vector2 operator*( float Scale, const Vector2& V )
	{
		return Vector2( V.x * Scale, V.y * Scale);
	}

	/** ����x�������y�������ƽ��ֵ */
	inline float Average() const	
	{return (x + y) * 0.5f;}	

	/** ������� */
	inline float  Dot(const Vector2& v) const 
	{return x*v.x + y*v.y;}

	/** ������ģ*/
	inline float Length() const 
	{ return sqrtf( x*x + y*y ); }

	/** ����ģ��ƽ�� \n
	 *  @note: ���������Ƚ����������ĳ���
	 */
	inline float LengthSquared() const 
	{ return ( x*x + y*y ); }

	/** ��λ������ \n
	 *	@return [bool] : �Ƿ�ɹ���λ��
	 *	@note: ���������ܱ���λ��������false
	 */
	inline bool Normalize()
	{
		float squareSum = LengthSquared();
		if( squareSum >= SMALL_NUMBER )
		{
			float scale = 1.f/sqrtf(squareSum);
			x *= scale; y *= scale;
			return true;
		}
		else return false;
	}

	/** �õ��������ĵ�λ������
	 *	@return [Vector2] : ����λ���������
	 *	@note: ���������ܱ���λ�������� Vector2(0, 0)
	 */
	inline Vector2 GetNormalized() const
	{
		float squareSum = LengthSquared();
		if( squareSum >= SMALL_NUMBER )
		{
			float scale = 1.f/sqrtf(squareSum);
			return Vector2( x*scale, y*scale );
		}
		else return Vector2( 0, 0 );
	}


	/** ��ԭ�������������� */
	inline Vector2 Perpendicular(void) const 
	{ return Vector2 (-y, x); }

	/** �ж�ԭ�����Ƿ񳤶�Ϊ0 */
	inline bool IsZeroLength(void) const
	{
		if (LengthSquared() < SMALL_NUMBER)
			return true;
		else return false;
	}

	/** �������� */
	static const Vector2 ZERO;			/// (0,0)
	static const Vector2 X_AXIS;		/// (1,0)
	static const Vector2 Y_AXIS;		/// (0,1)
 	static const Vector2 NEG_X_AXIS;	/// (-1,0)
	static const Vector2 NEG_Y_AXIS;	/// (0,-1)
	static const Vector2 UNIT_SCALE;	/// (1,1)

	/** ����<<����������Խ���ά��������Vector2(x,y)�ĸ�ʽ��� */
    inline friend std::ostream& operator <<	( std::ostream& o, const Vector2& v )
	{
		o << "Vector2(" << v.x << ", " << v.y <<  ")";
		return o;
	}


	/** �õ�ԭ������תһ���ǶȺ�������� \n
	 *	@param [float] radian: ��ת�ĽǶȣ���λ�ǻ���
	 */
	inline Vector2 GetRotatedInRadian(float radian)
	{
		float cosD = cosf(radian);
		float sinD = sinf(radian);

		Vector2 result;
		result.x = x*cosD + y*sinD;
		result.y = y*cosD + x*sinD;

		return result;
	}

	/** �õ�ԭ������תһ���ǶȺ�������� \n
	*	@param [float] degree: ��ת�ĽǶȣ���λ�ǽǶ�
	*/
	inline Vector2 GetRotatedInDegree(float degree)
	{
		float radian = DEG_TO_RAD(degree);
		return GetRotatedInRadian(radian);
	}

	/** �õ���������֮��ļнǣ���src��dest \n
	*	@param [Vector2] src: ԭ����
	*	@param [Vector2] dest: Ŀ�귽��
	*	@param [float] : ��ת�Ƕȣ�(-PI,PI],���ַ���
	*/
	static float GetRadianBetween2Vector2(Vector2 src, Vector2 dest);

};



/** 
 * ��ά����, (x,y,z), Ҳ���Ա�ʾ��ά�ռ�ĵ�
 */
class Vector3
{
public:
	float x,y,z;
	
	/** ���±�ȡ�����ĸ�����, ��������0~2 */
	inline float& operator [] (int i)			{return ((float*)this)[i];}

	/** ���±�ȡ�����ĸ�����, ��������0~2 */
	inline float operator [] (int i) const		{return ((float*)this)[i];}

	/** ���캯��.*/
	inline Vector3( float InX, float InY, float InZ )
	:	x(InX), y(InY), z(InZ)
	{}

	/** Ĭ�Ϲ��캯��.*/
	inline Vector3() :	x(0), y(0), z(0)
	{}

	/** 3ά��������\n
	 *	@param [float]: ��������
	 *	@param [const Vector3&]: Ҫ���������ĳ�������
	 *	@return [vector3]: result = float * vector3
	 */
	inline friend Vector3 operator*( float Scale, const Vector3& V )
	{ return Vector3( V.x * Scale, V.y * Scale, V.z * Scale ); }

	/** 3ά�����Ӻ����� \n
	 *	@param [const Vector3&]: ��������ĳ�������
	 *	@return [vector3]: result = this + vector3
	 */
	inline Vector3 operator+( const Vector3& V ) const
	{
		// for debug
		Vector3 kSum;
		kSum.x = x + V.x;
		kSum.y = y + V.y;
		kSum.z = z + V.z;
		return kSum;


		return Vector3( x + V.x, y + V.y, z + V.z );
	}

	/** 3ά������������ \n
	 *	@param [const Vector3&]: ��������ĳ�������
	 *	@return [vector3]: result = this - vector3
	 */
	inline Vector3 operator-( const Vector3& V ) const
	{
		return Vector3( x - V.x, y - V.y, z - V.z );
	}


	/** 3ά�������ţ��˺����� \n
	 *	@param [float]: ��������
	 *	@return [vector3]: result = vector3 * float
	 */
	inline Vector3 operator*( float Scale ) const
	{
		return Vector3( x * Scale, y * Scale, z * Scale );
	}

	/** 3ά�������ţ��������� \n
	 *	@param [float]: ��������
	 *	@return [vector3]: result = vector3 / float
	 *	@note:	��������Ϊ0
	 */
	inline Vector3 operator/( float Scale ) const
	{
		assert( fabsf(Scale) > SMALL_NUMBER );
		float RScale = 1.f/Scale;
		return Vector3( x * RScale, y * RScale, z * RScale );
	}
	
	/** 3ά����������������� \n
	 *	@param [const Vector3&]: Ҫ���������ĳ�������
	 *	@return [Vector3]: result = this / Vector3
	 *	@note: ������̾���ԭ�����ĸ������ֱ�Ͳ��������ĸ�����������õ��µ�һ������
	 */
    inline Vector3 operator / ( const Vector3& rhs) const
    {
        Vector3 kDiv;

        kDiv.x = x / rhs.x;
        kDiv.y = y / rhs.y;
        kDiv.z = z / rhs.z;

        return kDiv;
    }
		
	/** 3ά������ˣ��˺����� \n
	 *	@param [const Vector3&]: ��������ĳ�������
	 *	@return [Vector3]: result = this ��� Vector3
	 *	@note: ������̾���ԭ�����ĸ������ֱ�Ͳ��������ĸ�������ˣ��õ��µ�һ������
	 */    
	inline Vector3 operator * ( const Vector3& rhs) const
    {
        Vector3 kProd;

        kProd.x = rhs.x * x;
        kProd.y = rhs.y * y;
        kProd.z = rhs.z * z;

        return kProd;
    }

	/** 3ά������һ��4��4�任������ˣ��˺����� \n
	 *	@param [const Matrix4&]: 4��4�任����ĳ�������
	 *	@return [Vector3]: result = this �� Matrix4
	 *	@note: �����Ƚ�ԭVector3�任ΪVector4(w = 1),�任��Vector3�ĸ�����������w'
	 */    
    inline Vector3 operator * ( const Matrix4 &m ) const;


	/** �Ƚ�����3ά�����Ƿ���ȣ��Ⱥ����� \n
	 *	@param [const Vector3&]: ��֮�Ƚϵ������ĳ�������
	 *	@return [bool]: �Ƿ����
	 *	@note: �жϱ�׼������3ά���������������
	 */
	inline bool operator==( const Vector3& V ) const
	{
		return x==V.x && y==V.y && z==V.z;
	}

	/** �Ƚ�����3ά�����Ƿ񲻵ȣ����Ⱥ����� \n
	 *	@param [const Vector3&]: ��֮�Ƚϵ������ĳ�������
	 *	@return [bool]: �Ƿ񲻵�
	 *	@note: �������3ά��������һ��Ӧ�������ȣ���������������
	 */
	inline bool operator!=( const Vector3& V ) const
	{
		return x!=V.x || y!=V.y || z!=V.z;
	}

	/** 3ά�����󸺣��������� \n
	 *	@note: ����ֵ������ԭ��ԳƵ�����
	 */
	inline Vector3 operator-() const
	{
		return Vector3( -x, -y, -z );
	}

	/** 3ά�����Լӣ�this += Vector3; */
	inline void operator+=( const Vector3& V )
	{
		x += V.x; y += V.y; z += V.z;
	}
	/** 3ά������ֵ��this = Vector3; */
	inline void operator=( const Vector3& V )
	{
		x = V.x; y = V.y; z = V.z;
	}
	/** 3ά�����Լ���this -= Vector3; */
	inline void operator-=( const Vector3& V )
	{
		x -= V.x; y -= V.y; z -= V.z;
	}
	/** 3ά�������ţ�this *= Scale; */
	inline void operator*=( float Scale )
	{
		x *= Scale; y *= Scale; z *= Scale;
	}
	/** 3ά�������ţ�this /= Scale; 
	 *	@note: ���ܴ���0
	 */
	inline void operator/=( float V )
	{
		assert(V != 0);
		float RV = 1.f/V;
		x *= RV; y *= RV; z *= RV;
	}
	
	/** �õ�3ά������ģ */
	inline float Length() const
	{
		return sqrtf( x*x + y*y + z*z );
	}

	/** �õ�3ά����ģ��ƽ�������������ȱȽ� */
	inline float LengthSquared() const
	{
		return ( x*x + y*y + z*z );
	}

	/** �жϸ������Ƿ��Ǹ��ǳ�С���������ӽ���0 /n
	 *	@note: �жϷ����������ĸ��������ǳ�С
	 */
	inline int IsNearlyZero() const
	{
		return
			(	fabsf(x)<KINDA_SMALL_NUMBER
			&&	fabsf(y)<KINDA_SMALL_NUMBER
			&&	fabsf(z)<KINDA_SMALL_NUMBER);
	}

	/** �жϸ������Ƿ��Ǹ������� /n
	 *	@remark: x==y==z==0
	 */
	inline bool IsZero() const
	{
		return (x==0.f && y==0.f && z==0.f);
	}

	/** ��λ������ \n
	 *	@return [bool] : �Ƿ�ɹ���λ��
	 *	@note: ���������ܱ���λ��������false
	 */
	inline bool Normalize()
	{
		float squareSum = LengthSquared();
		if( squareSum >= SMALL_NUMBER )
		{
			float scale = 1.f/sqrtf(squareSum);
			x *= scale; y *= scale; z *= scale;
			return true;
		}
		else return false;
	}

	/** �õ��������ĵ�λ������
	 *	@return [Vector3] : ����λ���������
	 *	@note: ���������ܱ���λ�������� Vector3(0, 0, 0)
	 */	
	inline Vector3 GetNormalized() const
	{
		float squareSum = LengthSquared();
		if( squareSum >= SMALL_NUMBER )
		{
			float scale = 1.f/sqrtf(squareSum);
			return Vector3( x*scale, y*scale, z*scale );
		}
		else return Vector3( 0, 0, 0 );
	}

	//---------------------------------
	/// Operator replacement functions
	//---------------------------------
	/** ��ֵ��Ԫ�� */
	inline void Set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	/** �ж�����3ά�����Ƿ���� */
	inline bool Cmp(const Vector3& v){ return *this == v; }
	/** *this += Vector3	*/
	inline void Add(const Vector3& v){ *this += v; }
	/** *this ��= Vector3	*/	
	inline void Sub(const Vector3& v){ *this -= v; }
	/** 3ά�������	*/
	inline float DotProduction(const Vector3& v) const{ return v.x*x + v.y*y + v.z*z; }
	/** 3ά�������
	 * @note: ���ص���һ����<b>��λ��<\b>���������������ͱ��˵�����������ֱ
	 */
	inline Vector3 CrossProduction(const Vector3& v) const{ return Vector3::Cross(*this, v); }




	/** ��������������� */
	inline void Zero()										{x = y = z = 0;}
	/** �����ˣ����������ȵ�ƽ�� */
	inline float  DotSelf() const							{return x*x + y*y + z*z;}
	// �õ������������������ᣬ���� x_axis/y_axis/z_axis
	inline Vector3 GetNormal();

	/** �õ�ԭ�����ڲ���������ͶӰ�ĳ��� */ 
	inline float  ScalarProjectionOntoVector(Vector3& v1)	{return DotProduction(v1)/v1.Length();}
	
	/** �õ�ԭ�����ڲ��������ϵ�ͶӰ���� */ 
	inline Vector3 ProjectionOntoVector(const Vector3& v1)	{return v1*(DotProduction(v1)/v1.DotSelf());}

	/** ����ԭ�����Ͳ���������ƫ��Ļ��� */ 
	inline float  RadAngle(const Vector3& v1) const		{return acosf(DotProduction(v1)/(Length()*v1.Length()));}
	/** ����ԭ�����Ͳ���������ƫ��Ļ��ȵ�cosֵ */ 
	inline float  CosAngle(const Vector3& v1) const		{return DotProduction(v1)/(Length()*v1.Length());}
	/** ��ԭ���������Բ�������Ϊ��������ƽ��ķ������� */
	inline Vector3 Reflected(const Vector3& n)			{Vector3 r = n*(2.f * DotProduction(n));r-=*this;return r;}
	
	/** ����ԭ������������������֮������(P0-P1)�ľ��� */
	inline float  DistanceToLine(const Vector3& p0, const Vector3& p1) const;

	/** (x + y + z)/3 */
	inline float  Average() const						{return (x + y + z) * 0.333333f;}

	/** �����������е����� */
	inline Vector3 HalfWay(const Vector3& dest) const	{return Vector3(0.5f * (x + dest.x),0.5f * (y + dest.y),0.5f * (z + dest.z));}
	
	// special points
	static const Vector3 ZERO;
	static const Vector3 X_AXIS;
	static const Vector3 Y_AXIS;
	static const Vector3 Z_AXIS;
 	static const Vector3 NEG_X_AXIS;
	static const Vector3 NEG_Y_AXIS;
	static const Vector3 NEG_Z_AXIS;
	static const Vector3 UNIT_SCALE;

	/** ���������<<,ʹ֮��װ��ʽVector3(vx,y,z)��� */
	inline friend std::ostream& operator <<
		( std::ostream& o, const Vector3& v )
	{
		o << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
		return o;
	}

	/** ���Բ�ֵ����������������֮�������ϱ���Ϊt��������ʾ������ֵ */
	Vector3 Lerp(const Vector3& v1, float t);

	//---------------------------------
	/// Static functions
	//---------------------------------
	/** ��z_axis���ղ����еĽǶ���ת�����ҵ�λ����� */
	static Vector3 MakeDirection(float yaw, float pitch, float roll);
	/** ��v0,v1,v2��������ɵ�ƽ��ķ����� */
	static Vector3 NormalFromTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);

	/** ������������ˣ����ҵ�λ�� */
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	/** ���������Ĳ����δ��λ�� */
	static Vector3 CrossNoNorm(const Vector3& v1, const Vector3& v2);

};





/** 
 * ��ά����, (x,y,z,w), ����������ά�ռ�ĵ�, ����任��
 */
class Vector4: public Vector3
{
public:
	float w;

	/** ȱʡ�Ĺ��캯������ʼ�� */
	inline Vector4() : w(0)				{}
	/** ���ع��캯������һ����СΪ4�������ʼ�� */
	inline Vector4(const float v[4])	{x=v[0];y=v[1];z=v[2];w=v[3];}
	/** ���ع��캯�������ĸ���������ʼ�� */
	inline Vector4(float nx, float ny, float nz, float nw)	
	{	x = nx; y = ny; z = nz; w = nw;	}
	/** �������ع��캯���������ж����ʼ������ */
	inline Vector4(const Vector4& w)	{*this = w;}	


	// linear lerp
	/** ���Բ�ֵ��������ԭ�������������֮�������ϱ���Ϊt��������ʾ������ֵ */
	inline Vector4 Lerp(const Vector4& v1, float t);

	/** ���������*,������ά�����볣���ĳ˻� */
	inline Vector4 operator * (float s) const			{return Vector4(x*s, y*s, z*s, w*s);}
	/** ����*,����������ά�����ĵ������Ӧ�������˻� */
	inline Vector4 operator * (const Vector4& a) const	{return Vector4(x*a.x, y*a.y, z*a.z, w*a.w);}

	/** ���������*=,������ά�����볣���ĳ˻����Ҹ�ֵ��ԭ���� */
	inline Vector4& operator *= (float s)				{x *= s; y *= s; z *= s; w *= s; return *this;}
	/** ����*=����������ά�����ĳ˻�����Ӧ�������˻����Ҹ�����ǰ���� */
	inline Vector4& operator *= (const Vector4& a)		{x *= a.x; y *= a.y; z *= a.z; w *= a.w; return *this;}	

	/** ���أ�����������ά�����ĺͣ���Ӧ�������ӷ� */
	inline Vector4 operator + (const Vector4& a) const	{return Vector4(x+a.x, y+a.y, z+a.z, w+a.w);}
	/** ����+=����������ά�����ĺͣ���Ӧ�������ӷ����ҽ��������ǰ���� */
	inline Vector4& operator += (const Vector4& a)		{x += a.x; y += a.y; z += a.z; w += a.w; return *this;}

	/** ����-����������ά�����Ĳ� */
	inline Vector4 operator - (const Vector4& a) const	{return Vector4(x-a.x, y-a.y, z-a.z, w-a.w);}
	/** ����-=����������ά�����Ĳ��Ӧ�������������ҽ��������ǰ���� */
	inline Vector4& operator -= (const Vector4& a)		{x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this;}

	/** ����/������ά������һ������s���� */
	inline Vector4 operator / (float s) const			{assert(s!=0);s = 1.f/s; return Vector4(x*s, y*s, z*s, w*s);}	
	/** ����/=������ά������һ������s����,���������������ǰ���� */
	inline Vector4& operator /= (float s)				{assert(s!=0);s = 1.f/s; x *= s; y *= s; z *= s; w *= s; return *this;}	


	/** ������ά������ģ */ 
	inline float Length() const							{return sqrtf(x*x+y*y+z*z+w*w);}
	/** ������ά�����ĸ�������ƽ��ֵ */
	inline float Average() const						{return (x + y + z + w) * 0.25f;}	
	/** ��һ����ά����λ���������������� */	
	inline Vector4 Normalized() const					{float s = 1.f/Length(); Vector4 n = *this * s; return n;}

	/** ������ά��������ά����ĳ˻�
	 * @remark: ���������ǽ���������1��4�ľ���Ȼ�����m���󣬰��վ���˷���������Ϊ1��4�ľ��󣬼��µ���ά���� 
	 */
	inline Vector4 operator * (const Matrix4& mat);
};



#endif