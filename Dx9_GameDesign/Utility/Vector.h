
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
 * 二维向量, (x,y), 也可以表示二维空间的点
 */
class Vector2
{
public:
	float x, y;

	/** 默认构造函数，零向量 */
	inline Vector2() :	x(0), y(0)
	{}

	/** 构造函数 */
	inline Vector2( float InX, float InY )
	:	x(InX), y(InY)
	{}

	/** 拷贝构造函数 */
	inline Vector2(const Vector2& v) : x(v.x), y(v.y)
	{}

	/** 参数限制0，1，结果可被改动 */
	inline float& operator [] (int i)				
	{ return (reinterpret_cast<float*>(this))[i]; }

	/** 参数限制0，1，结果不可被改动 */
	inline float operator [] (int i) const			
	{ return (reinterpret_cast<const float*>(this))[i]; }

	/** 初始化，传入两个数分别赋给x，y */
	inline void operator() (float nx, float ny)	
	{x = nx; y = ny;}	

	/** 比较两个二维向量是否相等 \n
	 * @param [const Vector2&]: 比较向量的引用
	 * @return [bool]: 是否一样
	 */
	inline bool operator == (const Vector2& w) const
	{return RealEqual(x, w.x) && RealEqual(y, w.y);}

	
	/** 负号重载 */
	inline Vector2 operator - () const
	{
		Vector2 vNeg;
		vNeg.x = -x; vNeg.y = -y;
		return vNeg;
	}

	/** 二维向量缩放，this *= float; \n
	 * @return [Vector2&] : 结果是个引用,可以对他进行修改
	 */
	inline Vector2& operator *= (float s)			
	{x *= s; y *= s; return *this;}	
	
	/** 二维向量相加，Result = this + Vector2; */
	inline Vector2 operator+( const Vector2& V ) const	
	{return Vector2( x + V.x, y + V.y);}

	/** 二维向量相减，Result = this - Vector2; */
	inline Vector2 operator-( const Vector2& V ) const 
	{return Vector2( x - V.x, y - V.y);}

	/** 二维向量缩放，this = this / float; \n
	* @return [Vector2&] : 结果是个引用,可以对他进行修改
	*/
	inline Vector2& operator /= (float s)
	{
		assert( fabsf(s) > SMALL_NUMBER );
		x /= s; y /= s;
		return *this;
	}	

	/** 二维向量缩放，result = this / float; */
	inline Vector2 operator/( float s ) const 
	{
		assert( fabsf(s) > SMALL_NUMBER );
		return Vector2( x / s, y / s);
	}

	/** 操作符重载: return = float * Vector2; */
	inline friend Vector2 operator*( float Scale, const Vector2& V )
	{
		return Vector2( V.x * Scale, V.y * Scale);
	}

	/** 计算x轴分量与y轴分量的平均值 */
	inline float Average() const	
	{return (x + y) * 0.5f;}	

	/** 向量点乘 */
	inline float  Dot(const Vector2& v) const 
	{return x*v.x + y*v.y;}

	/** 向量的模*/
	inline float Length() const 
	{ return sqrtf( x*x + y*y ); }

	/** 向量模的平方 \n
	 *  @note: 可以用来比较两个向量的长度
	 */
	inline float LengthSquared() const 
	{ return ( x*x + y*y ); }

	/** 单位化向量 \n
	 *	@return [bool] : 是否成功单位化
	 *	@note: 零向量不能被单位化，返回false
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

	/** 得到此向量的单位化向量
	 *	@return [Vector2] : 被单位化后的向量
	 *	@note: 零向量不能被单位化，返回 Vector2(0, 0)
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


	/** 求原向量的正交向量 */
	inline Vector2 Perpendicular(void) const 
	{ return Vector2 (-y, x); }

	/** 判断原向量是否长度为0 */
	inline bool IsZeroLength(void) const
	{
		if (LengthSquared() < SMALL_NUMBER)
			return true;
		else return false;
	}

	/** 特殊向量 */
	static const Vector2 ZERO;			/// (0,0)
	static const Vector2 X_AXIS;		/// (1,0)
	static const Vector2 Y_AXIS;		/// (0,1)
 	static const Vector2 NEG_X_AXIS;	/// (-1,0)
	static const Vector2 NEG_Y_AXIS;	/// (0,-1)
	static const Vector2 UNIT_SCALE;	/// (1,1)

	/** 重载<<运算符，可以将二维向量按照Vector2(x,y)的格式输出 */
    inline friend std::ostream& operator <<	( std::ostream& o, const Vector2& v )
	{
		o << "Vector2(" << v.x << ", " << v.y <<  ")";
		return o;
	}


	/** 得到原向量旋转一定角度后的新向量 \n
	 *	@param [float] radian: 旋转的角度，单位是弧度
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

	/** 得到原向量旋转一定角度后的新向量 \n
	*	@param [float] degree: 旋转的角度，单位是角度
	*/
	inline Vector2 GetRotatedInDegree(float degree)
	{
		float radian = DEG_TO_RAD(degree);
		return GetRotatedInRadian(radian);
	}

	/** 得到两个向量之间的夹角，从src到dest \n
	*	@param [Vector2] src: 原方向
	*	@param [Vector2] dest: 目标方向
	*	@param [float] : 旋转角度，(-PI,PI],左手法则
	*/
	static float GetRadianBetween2Vector2(Vector2 src, Vector2 dest);

};



/** 
 * 三维向量, (x,y,z), 也可以表示三维空间的点
 */
class Vector3
{
public:
	float x,y,z;
	
	/** 按下标取向量的各分量, 参数限制0~2 */
	inline float& operator [] (int i)			{return ((float*)this)[i];}

	/** 按下标取向量的各分量, 参数限制0~2 */
	inline float operator [] (int i) const		{return ((float*)this)[i];}

	/** 构造函数.*/
	inline Vector3( float InX, float InY, float InZ )
	:	x(InX), y(InY), z(InZ)
	{}

	/** 默认构造函数.*/
	inline Vector3() :	x(0), y(0), z(0)
	{}

	/** 3维向量缩放\n
	 *	@param [float]: 缩放因子
	 *	@param [const Vector3&]: 要缩放向量的常量引用
	 *	@return [vector3]: result = float * vector3
	 */
	inline friend Vector3 operator*( float Scale, const Vector3& V )
	{ return Vector3( V.x * Scale, V.y * Scale, V.z * Scale ); }

	/** 3维向量加号重载 \n
	 *	@param [const Vector3&]: 相加向量的常量引用
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

	/** 3维向量减号重载 \n
	 *	@param [const Vector3&]: 相减向量的常量引用
	 *	@return [vector3]: result = this - vector3
	 */
	inline Vector3 operator-( const Vector3& V ) const
	{
		return Vector3( x - V.x, y - V.y, z - V.z );
	}


	/** 3维向量缩放，乘号重载 \n
	 *	@param [float]: 缩放因子
	 *	@return [vector3]: result = vector3 * float
	 */
	inline Vector3 operator*( float Scale ) const
	{
		return Vector3( x * Scale, y * Scale, z * Scale );
	}

	/** 3维向量缩放，除号重载 \n
	 *	@param [float]: 缩放因子
	 *	@return [vector3]: result = vector3 / float
	 *	@note:	参数不能为0
	 */
	inline Vector3 operator/( float Scale ) const
	{
		assert( fabsf(Scale) > SMALL_NUMBER );
		float RScale = 1.f/Scale;
		return Vector3( x * RScale, y * RScale, z * RScale );
	}
	
	/** 3维向量点除，除号重载 \n
	 *	@param [const Vector3&]: 要除的向量的常量引用
	 *	@return [Vector3]: result = this / Vector3
	 *	@note: 计算过程就是原向量的各分量分别和参数向量的各分量相除，得到新的一个向量
	 */
    inline Vector3 operator / ( const Vector3& rhs) const
    {
        Vector3 kDiv;

        kDiv.x = x / rhs.x;
        kDiv.y = y / rhs.y;
        kDiv.z = z / rhs.z;

        return kDiv;
    }
		
	/** 3维向量点乘，乘号重载 \n
	 *	@param [const Vector3&]: 相乘向量的常量引用
	 *	@return [Vector3]: result = this 点乘 Vector3
	 *	@note: 计算过程就是原向量的各分量分别和参数向量的各分量相乘，得到新的一个向量
	 */    
	inline Vector3 operator * ( const Vector3& rhs) const
    {
        Vector3 kProd;

        kProd.x = rhs.x * x;
        kProd.y = rhs.y * y;
        kProd.z = rhs.z * z;

        return kProd;
    }

	/** 3维向量和一个4×4变换矩阵相乘，乘号重载 \n
	 *	@param [const Matrix4&]: 4×4变换矩阵的常量引用
	 *	@return [Vector3]: result = this × Matrix4
	 *	@note: 这里先将原Vector3变换为Vector4(w = 1),变换后将Vector3的各分量都除以w'
	 */    
    inline Vector3 operator * ( const Matrix4 &m ) const;


	/** 比较两个3维向量是否相等，等号重载 \n
	 *	@param [const Vector3&]: 与之比较的向量的常量引用
	 *	@return [bool]: 是否相等
	 *	@note: 判断标准是两个3维向量各分量都相等
	 */
	inline bool operator==( const Vector3& V ) const
	{
		return x==V.x && y==V.y && z==V.z;
	}

	/** 比较两个3维向量是否不等，不等号重载 \n
	 *	@param [const Vector3&]: 与之比较的向量的常量引用
	 *	@return [bool]: 是否不等
	 *	@note: 如果两个3维向量的任一对应分量不等，则两个向量不等
	 */
	inline bool operator!=( const Vector3& V ) const
	{
		return x!=V.x || y!=V.y || z!=V.z;
	}

	/** 3维向量求负，负号重载 \n
	 *	@note: 返回值就是与原点对称的向量
	 */
	inline Vector3 operator-() const
	{
		return Vector3( -x, -y, -z );
	}

	/** 3维向量自加，this += Vector3; */
	inline void operator+=( const Vector3& V )
	{
		x += V.x; y += V.y; z += V.z;
	}
	/** 3维向量赋值，this = Vector3; */
	inline void operator=( const Vector3& V )
	{
		x = V.x; y = V.y; z = V.z;
	}
	/** 3维向量自减，this -= Vector3; */
	inline void operator-=( const Vector3& V )
	{
		x -= V.x; y -= V.y; z -= V.z;
	}
	/** 3维向量缩放，this *= Scale; */
	inline void operator*=( float Scale )
	{
		x *= Scale; y *= Scale; z *= Scale;
	}
	/** 3维向量缩放，this /= Scale; 
	 *	@note: 不能传入0
	 */
	inline void operator/=( float V )
	{
		assert(V != 0);
		float RV = 1.f/V;
		x *= RV; y *= RV; z *= RV;
	}
	
	/** 得到3维向量的模 */
	inline float Length() const
	{
		return sqrtf( x*x + y*y + z*z );
	}

	/** 得到3维向量模的平方，用来做长度比较 */
	inline float LengthSquared() const
	{
		return ( x*x + y*y + z*z );
	}

	/** 判断该向量是否是个非常小的向量，接近于0 /n
	 *	@note: 判断法则是向量的各分量都非常小
	 */
	inline int IsNearlyZero() const
	{
		return
			(	fabsf(x)<KINDA_SMALL_NUMBER
			&&	fabsf(y)<KINDA_SMALL_NUMBER
			&&	fabsf(z)<KINDA_SMALL_NUMBER);
	}

	/** 判断该向量是否是个零向量 /n
	 *	@remark: x==y==z==0
	 */
	inline bool IsZero() const
	{
		return (x==0.f && y==0.f && z==0.f);
	}

	/** 单位化向量 \n
	 *	@return [bool] : 是否成功单位化
	 *	@note: 零向量不能被单位化，返回false
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

	/** 得到此向量的单位化向量
	 *	@return [Vector3] : 被单位化后的向量
	 *	@note: 零向量不能被单位化，返回 Vector3(0, 0, 0)
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
	/** 赋值各元素 */
	inline void Set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	/** 判断两个3维向量是否相等 */
	inline bool Cmp(const Vector3& v){ return *this == v; }
	/** *this += Vector3	*/
	inline void Add(const Vector3& v){ *this += v; }
	/** *this －= Vector3	*/	
	inline void Sub(const Vector3& v){ *this -= v; }
	/** 3维向量点乘	*/
	inline float DotProduction(const Vector3& v) const{ return v.x*x + v.y*y + v.z*z; }
	/** 3维向量叉乘
	 * @note: 返回的是一个被<b>单位化<\b>的向量，此向量和被乘的两个向量垂直
	 */
	inline Vector3 CrossProduction(const Vector3& v) const{ return Vector3::Cross(*this, v); }




	/** 将向量变成零向量 */
	inline void Zero()										{x = y = z = 0;}
	/** 自身点乘，就是自身长度的平方 */
	inline float  DotSelf() const							{return x*x + y*y + z*z;}
	// 得到与此向量最近的坐标轴，返回 x_axis/y_axis/z_axis
	inline Vector3 GetNormal();

	/** 得到原向量在参数向量上投影的长度 */ 
	inline float  ScalarProjectionOntoVector(Vector3& v1)	{return DotProduction(v1)/v1.Length();}
	
	/** 得到原向量在参数向量上的投影向量 */ 
	inline Vector3 ProjectionOntoVector(const Vector3& v1)	{return v1*(DotProduction(v1)/v1.DotSelf());}

	/** 计算原向量和参数向量的偏离的弧度 */ 
	inline float  RadAngle(const Vector3& v1) const		{return acosf(DotProduction(v1)/(Length()*v1.Length()));}
	/** 计算原向量和参数向量的偏离的弧度的cos值 */ 
	inline float  CosAngle(const Vector3& v1) const		{return DotProduction(v1)/(Length()*v1.Length());}
	/** 求原向量关于以参数向量为法向量的平面的反射向量 */
	inline Vector3 Reflected(const Vector3& n)			{Vector3 r = n*(2.f * DotProduction(n));r-=*this;return r;}
	
	/** 计算原向量到另外两个向量之间连线(P0-P1)的距离 */
	inline float  DistanceToLine(const Vector3& p0, const Vector3& p1) const;

	/** (x + y + z)/3 */
	inline float  Average() const						{return (x + y + z) * 0.333333f;}

	/** 两个向量的中点向量 */
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

	/** 重载运算符<<,使之安装格式Vector3(vx,y,z)输出 */
	inline friend std::ostream& operator <<
		( std::ostream& o, const Vector3& v )
	{
		o << "Vector3(" << v.x << ", " << v.y << ", " << v.z << ")";
		return o;
	}

	/** 线性插值，即求在两个向量之间连线上比例为t处点所表示的向量值 */
	Vector3 Lerp(const Vector3& v1, float t);

	//---------------------------------
	/// Static functions
	//---------------------------------
	/** 将z_axis按照参数中的角度旋转，并且单位化输出 */
	static Vector3 MakeDirection(float yaw, float pitch, float roll);
	/** 求v0,v1,v2三点所组成的平面的法向量 */
	static Vector3 NormalFromTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2);

	/** 将两个向量叉乘，并且单位化 */
	static Vector3 Cross(const Vector3& v1, const Vector3& v2);
	/** 两个向量的叉积，未单位化 */
	static Vector3 CrossNoNorm(const Vector3& v1, const Vector3& v2);

};





/** 
 * 四维向量, (x,y,z,w), 用来描述三维空间的点, 坐标变换用
 */
class Vector4: public Vector3
{
public:
	float w;

	/** 缺省的构造函数，初始化 */
	inline Vector4() : w(0)				{}
	/** 重载构造函数，用一个大小为4的数组初始化 */
	inline Vector4(const float v[4])	{x=v[0];y=v[1];z=v[2];w=v[3];}
	/** 重载构造函数，用四个浮点数初始化 */
	inline Vector4(float nx, float ny, float nz, float nw)	
	{	x = nx; y = ny; z = nz; w = nw;	}
	/** 拷贝重载构造函数，用已有对象初始化向量 */
	inline Vector4(const Vector4& w)	{*this = w;}	


	// linear lerp
	/** 线性插值，即求在原向量与参数向量之间连线上比例为t处点所表示的向量值 */
	inline Vector4 Lerp(const Vector4& v1, float t);

	/** 重载运算符*,计算四维向量与常量的乘积 */
	inline Vector4 operator * (float s) const			{return Vector4(x*s, y*s, z*s, w*s);}
	/** 重载*,计算两个四维向量的点积，对应分量做乘积 */
	inline Vector4 operator * (const Vector4& a) const	{return Vector4(x*a.x, y*a.y, z*a.z, w*a.w);}

	/** 重载运算符*=,计算四维向量与常量的乘积并且赋值给原向量 */
	inline Vector4& operator *= (float s)				{x *= s; y *= s; z *= s; w *= s; return *this;}
	/** 重载*=计算两个四维向量的乘积，对应分量做乘积并且赋给当前向量 */
	inline Vector4& operator *= (const Vector4& a)		{x *= a.x; y *= a.y; z *= a.z; w *= a.w; return *this;}	

	/** 重载＋计算两个四维向量的和，对应分量做加法 */
	inline Vector4 operator + (const Vector4& a) const	{return Vector4(x+a.x, y+a.y, z+a.z, w+a.w);}
	/** 重载+=计算两个四维向量的和，对应分量做加法并且结果赋给当前向量 */
	inline Vector4& operator += (const Vector4& a)		{x += a.x; y += a.y; z += a.z; w += a.w; return *this;}

	/** 重载-计算两个四维向量的差 */
	inline Vector4 operator - (const Vector4& a) const	{return Vector4(x-a.x, y-a.y, z-a.z, w-a.w);}
	/** 重载-=计算两个四维向量的差，对应分量做减法并且结果赋给当前向量 */
	inline Vector4& operator -= (const Vector4& a)		{x -= a.x; y -= a.y; z -= a.z; w -= a.w; return *this;}

	/** 重载/计算四维向量与一个常数s的商 */
	inline Vector4 operator / (float s) const			{assert(s!=0);s = 1.f/s; return Vector4(x*s, y*s, z*s, w*s);}	
	/** 重载/=计算四维向量与一个常数s的商,并将将结果赋给当前向量 */
	inline Vector4& operator /= (float s)				{assert(s!=0);s = 1.f/s; x *= s; y *= s; z *= s; w *= s; return *this;}	


	/** 计算四维向量的模 */ 
	inline float Length() const							{return sqrtf(x*x+y*y+z*z+w*w);}
	/** 计算四维向量四个分量的平均值 */
	inline float Average() const						{return (x + y + z + w) * 0.25f;}	
	/** 求一个四维被单位化后的向量，并输出 */	
	inline Vector4 Normalized() const					{float s = 1.f/Length(); Vector4 n = *this * s; return n;}

	/** 计算四维向量与四维矩阵的乘积
	 * @remark: 具体做法是将向量看成1×4的矩阵，然后乘以m矩阵，按照矩阵乘法法则结果仍为1×4的矩阵，即新的四维向量 
	 */
	inline Vector4 operator * (const Matrix4& mat);
};



#endif