//---------------------------------------------------------------------------
//	k.chen
//	peterchenkai@163.com
//
//---------------------------------------------------------------------------

#ifndef _MMATRIX
#define _MMATRIX

#include "MyMath.h"
#include "Quaternion.h"
#include "Vector.h"
#include <assert.h>
#include <iostream>
using namespace std;

class Matrix3
{
public:
    /** 默认构造函数
        @note: 未被初始化,效率考虑
    */
	inline Matrix3 () {};
	/** 构造函数 */
    inline explicit Matrix3 (const float arr[3][3])
	{
		memcpy(m,arr,9*sizeof(float));
	}
	/** 拷贝构造函数 */
   inline Matrix3 (const Matrix3& rkMatrix)
	{
		memcpy(m,rkMatrix.m,9*sizeof(float));
	}
	/** 构造函数 */
    Matrix3 (float fEntry00, float fEntry01, float fEntry02,
                float fEntry10, float fEntry11, float fEntry12,
                float fEntry20, float fEntry21, float fEntry22)
	{
		m[0][0] = fEntry00;
		m[0][1] = fEntry01;
		m[0][2] = fEntry02;
		m[1][0] = fEntry10;
		m[1][1] = fEntry11;
		m[1][2] = fEntry12;
		m[2][0] = fEntry20;
		m[2][1] = fEntry21;
		m[2][2] = fEntry22;
	}

    /** []操作符, 可以使用 mat[r][c] */
    inline float* operator[] (int iRow) const
	{
		assert(iRow>=0 && iRow<3);
		return (float*)m[iRow];
	}

	/** 得到对应行 */
	Vector3 GetRow(int iRow) const;
	/** 设置行 */
	void SetRow(int iRow, const Vector3& vec);

	/** 得到对应列，返回一个vector3 */
    Vector3 GetColumn (int iCol) const;
	/** 对对应列进行设置 */
    void SetColumn(int iCol, const Vector3& vec);
	/** 根据三个坐标轴上的投影得到一个矩阵 */
    void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

    /** 把已知3*3矩阵的值赋给3*3矩阵对象 */
    inline Matrix3& operator= (const Matrix3& rkMatrix)
	{	memcpy(m,rkMatrix.m,9*sizeof(float));	return *this;	}
	/** 判断两个3*3矩阵是否相等 */
	bool operator== (const Matrix3& rkMatrix) const;
	/** 判断两个3*3矩阵是否不相等 */
	inline bool operator!= (const Matrix3& rkMatrix) const
	{	return !operator==(rkMatrix);	}


    // arithmetic operations
	/** 两个3*3矩阵相加 */
	Matrix3 operator+ (const Matrix3& rkMatrix) const;
	/** 两个3*3矩阵相减 */
	Matrix3 operator- (const Matrix3& rkMatrix) const;
	/** 两个3*3矩阵相乘 */
    Matrix3 operator* (const Matrix3& rkMatrix) const;
	/** 3*3矩阵乘以－1 */
    Matrix3 operator- () const;

    /** 3*3矩阵和列向量相乘 */ 
    Vector3 operator* (const Vector3& rkVector) const;

    /** 行向量与3*3矩阵相乘 */
    friend inline Vector3 operator* (const Vector3& rkVector,const Matrix3& rkMatrix)
	{
		Vector3 kProd;
		for (int iRow = 0; iRow < 3; iRow++)
		{
			kProd[iRow] =
				rkVector[0]*rkMatrix.m[0][iRow] +
				rkVector[1]*rkMatrix.m[1][iRow] +
				rkVector[2]*rkMatrix.m[2][iRow];
		}
		return kProd;
	}

    /** 3*3矩阵与标量相乘 */
    Matrix3 operator* (float fScalar) const;

    /** 标量与3*3矩阵相乘 */
    friend inline Matrix3 operator* (float fScalar, const Matrix3& rkMatrix)
	{
		Matrix3 kProd;
		for (int iRow = 0; iRow < 3; iRow++)
		{
			for (int iCol = 0; iCol < 3; iCol++)
				kProd[iRow][iCol] = fScalar*rkMatrix.m[iRow][iCol];
		}
		return kProd;
	}

    /// utilities
	/** 对3*3矩阵求转置矩阵 */
    Matrix3 Transpose () const;
	/** 对3*3矩阵求逆矩阵，如果逆矩阵存在，把结果赋给第一个参数，返回bool 1。
		如果逆矩阵不存在，则返回bool 0，并把与原矩阵相乘为O的矩阵赋给第一个参数 
	 */
    bool Inverse (Matrix3& rkInverse, float fTolerance = 1e-06) const;
	/** 对3*3矩阵求逆矩阵,如果逆矩阵存在，则返回逆矩阵；如果逆矩阵不存在，则返回零矩阵与原矩阵相乘为O的矩阵.
	 * @note: 函数是const的 
	 */
    Matrix3 Inverse (float fTolerance = 1e-06) const;
	/** 求3*3矩阵的行列式值 */
    float Determinant () const;

	/** 奇异值分解，把矩阵分解成[U，S，V]三矩阵之积，其中U、V为正交矩阵，S为对角矩阵
	 *	@note: 先不要用,还未经过测试
	 */
    void SingularValueDecomposition (Matrix3& rkL, Vector3& rkS,
        Matrix3& rkR) const;
	/** 奇异值组合
	 *	@note: 先不要用,还未经过测试
	 */
    void SingularValueComposition (const Matrix3& rkL,
        const Vector3& rkS, const Matrix3& rkR);

    /** 对3*3矩阵正交化 Gram-Schmidt orthonormalization (applied to columns of rotation matrix) */
    void Orthonormalize ();

	/** QDU分解：把3*3矩阵分解成一个正交矩阵、一个对角矩阵、一个上三角矩阵（01 02 12位置）
		结果分别传给三个参数（由于后两个矩阵只有三个数字，故用vector3来表示）
	 */
    void QDUDecomposition (Matrix3& rkQ, Vector3& rkD,
        Vector3& rkU) const;

	/** 求矩阵的范数 */
    float SpectralNorm () const;

	/** 一个3*3矩阵变换为绕rkAxis轴转rfAngle弧度
	 *	@note: matrix must be orthonormal
	 */
    void ToAxisRadian (Vector3& rkAxis, float& rfAngle) const;
	/** 一个3*3矩阵变换为绕rkAxis轴转rfAngle角度
	 *	@note: matrix must be orthonormal
	 */
	inline void ToAxisDegree (Vector3& rkAxis, float& rfAngle) const {
		float r;
		ToAxisRadian ( rkAxis, r );
		rfAngle = RAD_TO_DEG(r);
	}
	/** 一个3维参数向量做旋转轴，float参数作为旋转角度，旋转后到一个3*3矩阵的值赋给调用对象矩阵 */
    void FromAxisRadian (const Vector3& rkAxis, const float fRadians);


 	/** 计算在欧拉变换中分别绕XYZ多少弧度能得到调用对象矩阵，把角度返回给三个参数。
		如果有唯一解函数返回值为真，如果解不唯一则返回值为假。
	 *	@note: 必须是个旋转矩阵(矩阵要正交)
	 */
    bool ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;

	/** 在欧拉变换坐标系中，分别绕XYZ如参数值的角度（弧度），把得到的矩阵赋给调用对象矩阵 */
    void FromEulerAnglesXYZ (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesXZY (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesYXZ (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesYZX (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesZXY (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesZYX (const float fYAngle, const float fPAngle, const float fRAngle);


    /** 计算3*3矩阵的特征值与特征向量,分别赋给第一个和第二个参数 */
    void EigenSolveSymmetric (float afEigenvalue[3],
        Vector3 akEigenvector[3]) const;

	/** 前两个向量参数的张量积（3*3矩阵）赋给第三个参数 */
    static void TensorProduct (const Vector3& rkU, const Vector3& rkV,
        Matrix3& rkProduct);

	/** 得到一个对着dir方向,同时up向量是y轴的矩阵 */
	static Matrix3 LookTowards(const Vector3& dir);
	/** 返回一个矩阵(坐标系), y方向是up向量,x方向是x轴 */
	static Matrix3 LookToUp(const Vector3& up); 
	/** 返回一个矩阵(坐标系), 对着to,上是up */
	static Matrix3 LookTowardsUp(const Vector3& to,const Vector3& up);

 
	// 自身旋转 , 输入绕相应轴的弧度值
	/** 绕X轴自身旋转 弧度radian，返回旋转后3*3矩阵的指针 */
	const Matrix3& RotationX(float radian);
	/** 绕Y轴自身旋转 弧度radian，返回旋转后3*3矩阵的指针 */
	const Matrix3& RotationY(float radian);
	/** 绕Z轴自身旋转 弧度radian，返回旋转后3*3矩阵的指针 */
	const Matrix3& RotationZ(float radian);
	/** 分别绕XYZ三轴自身旋转 参数弧度后，返回旋转后3*3矩阵的指针 */
	const Matrix3& RotationXYZ(float radianX, float radianY,  float radianZ);

	// 得到绕相应轴的旋转矩阵
	/** 得到这样一个3*3矩阵：它乘以对象矩阵后，让对象矩阵得到绕X轴旋转radian弧度的效果 */
	static Matrix3 GetRotaionMatrixX(float radian);
	/** 得到这样一个3*3矩阵：它乘以对象矩阵后，让对象矩阵得到绕Y轴旋转radian弧度的效果 */
	static Matrix3 GetRotaionMatrixY(float radian);
	/** 得到这样一个3*3矩阵：它乘以对象矩阵后，让对象矩阵得到绕Z轴旋转radian弧度的效果 */
	static Matrix3 GetRotaionMatrixZ(float radian);
	/** 得到这样一个3*3矩阵：它乘以对象矩阵后，让对象矩阵得到分别绕XYZ三轴旋转参数弧度后的效果 */
	static Matrix3 GetRotaionMatrixXYZ(float radianX, float radianY,  float radianZ);	
	
	static const float EPSILON;
    static const Matrix3 ZERO;			/// 零矩阵
    static const Matrix3 IDENTITY;		/// 单位矩阵


protected:
    // support for eigensolver
    void Tridiagonal (float afDiag[3], float afSubDiag[3]);
    bool QLAlgorithm (float afDiag[3], float afSubDiag[3]);

    // support for singular value decomposition
    static const float ms_fSvdEpsilon;
    static const unsigned int ms_iSvdMaxIterations;
    static void Bidiagonalize (Matrix3& kA, Matrix3& kL,
        Matrix3& kR);
    static void GolubKahanStep (Matrix3& kA, Matrix3& kL,
        Matrix3& kR);

    // support for spectral norm
    static float MaxCubicRoot (float afCoeff[3]);

	float m[3][3];

    // for faster access
    friend class Matrix4;

};




class Matrix4
{
protected:
	/// The matrix entries, indexed by [row][col].
	union {
		float m[4][4];
		float _m[16];
	};
public:
	/** 得到一个对着dir方向,同时up向量是y轴的矩阵 */
	static Matrix4 LookTowards(const Vector3& dir);
	/** 返回一个矩阵(坐标系), 对着to,上是up */
	static Matrix4 LookTowardsUp(const Vector3& to,const Vector3& up);


	/** 得到4*4矩阵右方向的标准基,结果是个单位向量 */
	Vector3 GetRight() const { return Vector3(m[0][0], m[0][1], m[0][2]).GetNormalized(); }
	/** 得到4*4矩阵上方向的标准基,结果是个单位向量 */
	Vector3 GetUp() const { return Vector3(m[1][0], m[1][1], m[1][2]).GetNormalized(); }
	/** 得到4*4矩阵前方向的标准基,结果是个单位向量 */
	Vector3 GetDir() const { return Vector3(m[2][0], m[2][1], m[2][2]).GetNormalized(); }

	/** 得到4*4矩阵右方向的非标准基,结果未被单位化 */
	Vector3 GetRightUnnormalized() const { return Vector3(m[0][0], m[0][1], m[0][2]); }
	/** 得到4*4矩阵上方向的非标准基,结果未被单位化 */
	Vector3 GetUpUnnormalized() const { return Vector3(m[1][0], m[1][1], m[1][2]); }
	/** 得到4*4矩阵前方向的非标准基,结果未被单位化 */
	Vector3 GetDirUnnormalized() const { return Vector3(m[2][0], m[2][1], m[2][2]); }


	/** 得到对应列，返回一个vector3 */
    Vector3 GetColumn (int iCol) const;
	/** 对对应列进行设置 */
    void SetColumn(int iCol, const Vector3& vec);

	/** 得到对应行 */
	Vector3 GetRow(int iRow) const;
	/** 设置行 */
	void SetRow(int iRow, const Vector3& vec);


	// 自身旋转 , 输入绕相应轴的弧度值
	/** 绕X轴自身旋转 弧度radian，返回旋转后4*4矩阵的指针 */
	const Matrix4& RotationX(float radian);
	/** 绕Y轴自身旋转 弧度radian，返回旋转后4*4矩阵的指针 */
	const Matrix4& RotationY(float radian);
	/** 绕Z轴自身旋转 弧度radian，返回旋转后4*4矩阵的指针 */
	const Matrix4& RotationZ(float radian);
	/** 分别绕XYZ三轴自身旋转 参数弧度后，返回旋转后4*4矩阵的指针 */
	const Matrix4& RotationXYZ(float radianX, float radianY,  float radianZ);

	// 得到绕相应轴的旋转矩阵
	/** 得到这样一个4*4矩阵：它乘以对象矩阵后，让对象矩阵得到绕X轴旋转radian弧度的效果 */
	static Matrix4 GetRotaionMatrixX(float radian);
	/** 得到这样一个4*4矩阵：它乘以对象矩阵后，让对象矩阵得到绕Y轴旋转radian弧度的效果 */
	static Matrix4 GetRotaionMatrixY(float radian);
	/** 得到这样一个4*4矩阵：它乘以对象矩阵后，让对象矩阵得到绕Z轴旋转radian弧度的效果 */
	static Matrix4 GetRotaionMatrixZ(float radian);
	/** 得到这样一个4*4矩阵：它乘以对象矩阵后，让对象矩阵得到分别绕XYZ三轴旋转参数弧度后的效果 */
	static Matrix4 GetRotaionMatrixXYZ(float radianX, float radianY,  float radianZ);

	/** 默认构造函数,初始化矩阵为4*4单位矩阵 */
    inline Matrix4()
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }

	/** 用16个变量初始化矩阵 */
    inline Matrix4(
        float m00, float m01, float m02, float m03,
        float m10, float m11, float m12, float m13,
        float m20, float m21, float m22, float m23,
        float m30, float m31, float m32, float m33 )
    {
        m[0][0] = m00;
        m[0][1] = m01;
        m[0][2] = m02;
        m[0][3] = m03;
        m[1][0] = m10;
        m[1][1] = m11;
        m[1][2] = m12;
        m[1][3] = m13;
        m[2][0] = m20;
        m[2][1] = m21;
        m[2][2] = m22;
        m[2][3] = m23;
        m[3][0] = m30;
        m[3][1] = m31;
        m[3][2] = m32;
        m[3][3] = m33;
    }

    /** 通过3*3矩阵构造一个4*4的旋转/缩放矩阵,其偏移量是0  */
    inline Matrix4(const Matrix3& m3x3)
    {
      operator=(IDENTITY);
      operator=(m3x3);
    }

    /** 通过一个四元数初始化一个4*4的旋转/缩放矩阵 */
    inline Matrix4(const Quaternion& rot)
    {
      Matrix3 m3x3;
      rot.ToRotationMatrix(m3x3);
      operator=(IDENTITY);
      operator=(m3x3);
    }
    
	/** []重载，得到4*4矩阵的一行的首地址，适用于变量 */
    inline float* operator [] ( int iRow )
    {
        assert( iRow < 4 );
        return m[iRow];
    }

	/** []重载，得到4*4矩阵的一行的首地址，适用于常量 */
   inline const float *const operator [] ( int iRow ) const
    {
        assert( iRow < 4 );
        return m[iRow];
    }

	/** 矩阵级连 */
    inline Matrix4 Concatenate(const Matrix4 &m2) const
    {
        Matrix4 r;
        r.m[0][0] = m[0][0] * m2.m[0][0] + m[0][1] * m2.m[1][0] + m[0][2] * m2.m[2][0] + m[0][3] * m2.m[3][0];
        r.m[0][1] = m[0][0] * m2.m[0][1] + m[0][1] * m2.m[1][1] + m[0][2] * m2.m[2][1] + m[0][3] * m2.m[3][1];
        r.m[0][2] = m[0][0] * m2.m[0][2] + m[0][1] * m2.m[1][2] + m[0][2] * m2.m[2][2] + m[0][3] * m2.m[3][2];
        r.m[0][3] = m[0][0] * m2.m[0][3] + m[0][1] * m2.m[1][3] + m[0][2] * m2.m[2][3] + m[0][3] * m2.m[3][3];

        r.m[1][0] = m[1][0] * m2.m[0][0] + m[1][1] * m2.m[1][0] + m[1][2] * m2.m[2][0] + m[1][3] * m2.m[3][0];
        r.m[1][1] = m[1][0] * m2.m[0][1] + m[1][1] * m2.m[1][1] + m[1][2] * m2.m[2][1] + m[1][3] * m2.m[3][1];
        r.m[1][2] = m[1][0] * m2.m[0][2] + m[1][1] * m2.m[1][2] + m[1][2] * m2.m[2][2] + m[1][3] * m2.m[3][2];
        r.m[1][3] = m[1][0] * m2.m[0][3] + m[1][1] * m2.m[1][3] + m[1][2] * m2.m[2][3] + m[1][3] * m2.m[3][3];

        r.m[2][0] = m[2][0] * m2.m[0][0] + m[2][1] * m2.m[1][0] + m[2][2] * m2.m[2][0] + m[2][3] * m2.m[3][0];
        r.m[2][1] = m[2][0] * m2.m[0][1] + m[2][1] * m2.m[1][1] + m[2][2] * m2.m[2][1] + m[2][3] * m2.m[3][1];
        r.m[2][2] = m[2][0] * m2.m[0][2] + m[2][1] * m2.m[1][2] + m[2][2] * m2.m[2][2] + m[2][3] * m2.m[3][2];
        r.m[2][3] = m[2][0] * m2.m[0][3] + m[2][1] * m2.m[1][3] + m[2][2] * m2.m[2][3] + m[2][3] * m2.m[3][3];

        r.m[3][0] = m[3][0] * m2.m[0][0] + m[3][1] * m2.m[1][0] + m[3][2] * m2.m[2][0] + m[3][3] * m2.m[3][0];
        r.m[3][1] = m[3][0] * m2.m[0][1] + m[3][1] * m2.m[1][1] + m[3][2] * m2.m[2][1] + m[3][3] * m2.m[3][1];
        r.m[3][2] = m[3][0] * m2.m[0][2] + m[3][1] * m2.m[1][2] + m[3][2] * m2.m[2][2] + m[3][3] * m2.m[3][2];
        r.m[3][3] = m[3][0] * m2.m[0][3] + m[3][1] * m2.m[1][3] + m[3][2] * m2.m[2][3] + m[3][3] * m2.m[3][3];

        return r;
    }

	/** *操作符重载，两个4*4矩阵相乘 */
    inline Matrix4 operator * ( const Matrix4 &m2 ) const
    {
        return Concatenate( m2 );
    }
	/** *=操作符重载，两个4*4矩阵相乘,把结果赋给调用对象 */
	inline void operator *= ( const Matrix4 &m2 )
	{
		*this = Concatenate( m2 );
	}


/*	inline Plane operator * (const Plane& p) const
    {
        Plane ret;
		Matrix4 invTrans = inverse().transpose();
        ret.normal.x =
            invTrans[0][0] * p.normal.x + invTrans[0][1] * p.normal.y + invTrans[0][2] * p.normal.z;
        ret.normal.y = 
            invTrans[1][0] * p.normal.x + invTrans[1][1] * p.normal.y + invTrans[1][2] * p.normal.z;
        ret.normal.z = 
            invTrans[2][0] * p.normal.x + invTrans[2][1] * p.normal.y + invTrans[2][2] * p.normal.z;
        Vector3 pt = p.normal * -p.d;
        pt = *this * pt;
        ret.d = - pt.dotProduct(ret.normal);
        return ret;
    }
*/

    /** +操作符重载，两个4*4矩阵相加 */
    inline Matrix4 operator + ( const Matrix4 &m2 ) const
    {
        Matrix4 r;

        r.m[0][0] = m[0][0] + m2.m[0][0];
        r.m[0][1] = m[0][1] + m2.m[0][1];
        r.m[0][2] = m[0][2] + m2.m[0][2];
        r.m[0][3] = m[0][3] + m2.m[0][3];

        r.m[1][0] = m[1][0] + m2.m[1][0];
        r.m[1][1] = m[1][1] + m2.m[1][1];
        r.m[1][2] = m[1][2] + m2.m[1][2];
        r.m[1][3] = m[1][3] + m2.m[1][3];

        r.m[2][0] = m[2][0] + m2.m[2][0];
        r.m[2][1] = m[2][1] + m2.m[2][1];
        r.m[2][2] = m[2][2] + m2.m[2][2];
        r.m[2][3] = m[2][3] + m2.m[2][3];

        r.m[3][0] = m[3][0] + m2.m[3][0];
        r.m[3][1] = m[3][1] + m2.m[3][1];
        r.m[3][2] = m[3][2] + m2.m[3][2];
        r.m[3][3] = m[3][3] + m2.m[3][3];

        return r;
    }

    /** －操作符重载，两个4*4矩阵相减（调用对象减去参数） */
    inline Matrix4 operator - ( const Matrix4 &m2 ) const
    {
        Matrix4 r;
        r.m[0][0] = m[0][0] - m2.m[0][0];
        r.m[0][1] = m[0][1] - m2.m[0][1];
        r.m[0][2] = m[0][2] - m2.m[0][2];
        r.m[0][3] = m[0][3] - m2.m[0][3];

        r.m[1][0] = m[1][0] - m2.m[1][0];
        r.m[1][1] = m[1][1] - m2.m[1][1];
        r.m[1][2] = m[1][2] - m2.m[1][2];
        r.m[1][3] = m[1][3] - m2.m[1][3];

        r.m[2][0] = m[2][0] - m2.m[2][0];
        r.m[2][1] = m[2][1] - m2.m[2][1];
        r.m[2][2] = m[2][2] - m2.m[2][2];
        r.m[2][3] = m[2][3] - m2.m[2][3];

        r.m[3][0] = m[3][0] - m2.m[3][0];
        r.m[3][1] = m[3][1] - m2.m[3][1];
        r.m[3][2] = m[3][2] - m2.m[3][2];
        r.m[3][3] = m[3][3] - m2.m[3][3];

        return r;
    }

    /** ＝＝操作符重载，判断两个4*4矩阵是否相等 */
    inline bool operator == ( const Matrix4& m2 ) const
    {
        if( 
            m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
            m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
            m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
            m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
            return false;
        return true;
    }

    /** ！＝操作符重载，判断两个4*4矩阵是否不相等 */
    inline bool operator != ( const Matrix4& m2 ) const
    {
        if( 
            m[0][0] != m2.m[0][0] || m[0][1] != m2.m[0][1] || m[0][2] != m2.m[0][2] || m[0][3] != m2.m[0][3] ||
            m[1][0] != m2.m[1][0] || m[1][1] != m2.m[1][1] || m[1][2] != m2.m[1][2] || m[1][3] != m2.m[1][3] ||
            m[2][0] != m2.m[2][0] || m[2][1] != m2.m[2][1] || m[2][2] != m2.m[2][2] || m[2][3] != m2.m[2][3] ||
            m[3][0] != m2.m[3][0] || m[3][1] != m2.m[3][1] || m[3][2] != m2.m[3][2] || m[3][3] != m2.m[3][3] )
            return true;
        return false;
    }

    /** ＝操作符重载，把参数3*3矩阵赋值给4*4矩阵，结果为4*4单位矩阵的前三行三列加上参数3*3矩阵 */
    inline void operator = ( const Matrix3& mat3 )
    {
        m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2]; //m[0][3] = 0;
        m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2]; //m[1][3] = 0;
        m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2]; //m[2][3] = 0;
	//	m[3][0] = 0;			m[3][1] = 0	;			m[3][2] = 0;			//m[3][3] = 1;
   }

	/** ＝操作符重载 */
	inline const Matrix4& operator = ( const Matrix4& mat4)
	{	memcpy(m, mat4.m,16*sizeof(float));	return *this; }

	/** 得到4*4矩阵的转置矩阵 */
    inline Matrix4 Transpose(void) const
    {
        return Matrix4(m[0][0], m[1][0], m[2][0], m[3][0],
                       m[0][1], m[1][1], m[2][1], m[3][1],
                       m[0][2], m[1][2], m[2][2], m[3][2],
                       m[0][3], m[1][3], m[2][3], m[3][3]);
    }

    /*
    -----------------------------------------------------------------------
    Translation Transformation
    -----------------------------------------------------------------------
    */

    /** 用一个3维向量来设定4*4矩阵的投影变换的值（即设定[0][3]、[1][3]、[2][3]位置的值） */
    inline Matrix4 SetProj( const Vector3& v )
    {
        m[0][3] = v.x;
        m[1][3] = v.y;
        m[2][3] = v.z;
		return *this;
    }

	/** 用一个3维向量来设定4*4矩阵的位移变换的值（即设定[3][0]、[3][1]、[3][2]位置的值） */
    inline Matrix4 SetOffset( const Vector3& v )
    {
        m[3][0] = v.x;
        m[3][1] = v.y;
        m[3][2] = v.z;
		return *this;
    }

    /** 获得4*4矩阵的投影变换的值（即[0][3]、[1][3]、[2][3]位置的值） */
    inline Vector3 GetProj() const
    {
      return Vector3(m[0][3], m[1][3], m[2][3]);
    }
    
	/** 获得4*4矩阵的位移变换的值（即[0][3]、[1][3]、[2][3]位置的值） */
    inline Vector3 GetOffset() const
    {
      return Vector3(m[3][0], m[3][1], m[3][2]);
    }

    /** 用一个3维向量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个投影转换矩阵 */
    inline void MakeProj( const Vector3& v )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }

	/** 用一个3维向量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个位移转换矩阵 */
    inline void MakeOffset( const Vector3& v )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = v.x; m[3][1] = v.y; m[3][2] = v.z; m[3][3] = 1.0;
    }


	/** 用3个float变量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个投影转换矩阵 */
    inline void MakeProj( float tx, float ty, float tz )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }

	/** 用3个float变量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个位移转换矩阵 */
	inline void MakeOffset( float tx, float ty, float tz )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = tx ; m[3][1] = ty ; m[3][2] = tz ; m[3][3] = 1.0;
    }

    /** 用一个3维向量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个投影转换矩阵，并把这个矩阵作为函数返回值  */
    inline static Matrix4 GetProj( const Vector3& v )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = v.x;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = v.y;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = v.z;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

        return r;
    }

	/** 用一个3维向量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个平移转换矩阵，并把这个矩阵作为函数返回值  */
    inline static Matrix4 GetOffset( const Vector3& v )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = 0.0;
        r.m[3][0] = v.x; r.m[3][1] = v.y; r.m[3][2] = v.z; r.m[3][3] = 1.0;

        return r;
    }


	/** 用3个float变量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个投影的转换矩阵，并把这个矩阵作为函数返回值 */
    inline static Matrix4 GetProj( float t_x, float t_y, float t_z )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = t_x;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = t_y;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = t_z;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

        return r;
    }

	/** 用3个float变量把调用对象矩阵（无论矩阵原来是怎么样的）变成一个平移的转换矩阵，并把这个矩阵作为函数返回值 */
    inline static Matrix4 GetOffset( float t_x, float t_y, float t_z )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = 0.0;
        r.m[3][0] = t_x; r.m[3][1] = t_y; r.m[3][2] = t_z; r.m[3][3] = 1.0;

        return r;
    }

	/** 得到4×4的数组指针 */
	inline const float * const Get4_4Array() const
	{ return _m; }

	inline void SetMatrix(const float * const p4_4Array)
	{
		memcpy(_m, p4_4Array, sizeof(_m));
	}

	/** 把4*4矩阵初始化或设为4*4单位矩阵 */
	inline void Identify()
	{
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
	}

	/** 把4*4矩阵初始化或设为4*4零矩阵 */
	inline void Zero()
	{
        m[0][0] = 0.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 0.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 0.0; m[2][3] = 0.0;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 0.0;
	}

    /*
    -----------------------------------------------------------------------
    Scale Transformation
    -----------------------------------------------------------------------
    */

    /** 用参数3维向量设置对象4*4矩阵的scale值（[0][0],[1][1],[2][2]位置的值）    */
    inline void SetScale( const Vector3& v )
    {
        m[0][0] = v.x;
        m[1][1] = v.y;
        m[2][2] = v.z;
    }

	/** 整体缩放（[3][3]位置的值）    */
	inline void SetScale( const float& scale )
	{
		m[3][3] = 1.f/scale;
	}


    /** 得到一个缩放矩阵
	 *	@param [const Vector&] v : x轴, y轴, z轴 的缩放系数
	 *	@return Matrix4 : 得到的4×4缩放矩阵
	*/
    inline static Matrix4 GetScale( const Vector3& v )
    {
        Matrix4 r;
        r.m[0][0] = v.x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = v.y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = v.z; r.m[2][3] = 0.0;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

        return r;
    }

	/** 得到一个缩放矩阵
	 *	@param [float] x y z : x轴, y轴, z轴 的缩放系数
	 *	@return Matrix4 : 得到的4×4缩放矩阵
	 */
    inline static Matrix4 GetScale( float s_x, float s_y, float s_z )
    {
        Matrix4 r;
        r.m[0][0] = s_x; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = s_y; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = s_z; r.m[2][3] = 0.0;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

        return r;
    }

    /** 输出4维矩阵中表示旋转,缩放的那个三维矩阵 
     *	@param [Matrix3&] : 输出矩阵,Matrix3
     */
    inline void Extract3x3Matrix(Matrix3& m3x3) const
    {
        m3x3.m[0][0] = m[0][0];
        m3x3.m[0][1] = m[0][1];
        m3x3.m[0][2] = m[0][2];
        m3x3.m[1][0] = m[1][0];
        m3x3.m[1][1] = m[1][1];
        m3x3.m[1][2] = m[1][2];
        m3x3.m[2][0] = m[2][0];
        m3x3.m[2][1] = m[2][1];
        m3x3.m[2][2] = m[2][2];

    }

    /** 通过一个4*4矩阵得到一个四元数(旋转) */
    inline Quaternion ExtractQuaternion() const
    {
      Matrix3 m3x3;
      Extract3x3Matrix(m3x3);
      return Quaternion(m3x3);
    }

    static const Matrix4 ZERO;
    static const Matrix4 IDENTITY;

    /** Useful little matrix which takes 2D clipspace {-1, 1} to {0,1}
        and inverts the Y. */
    static const Matrix4 CLIPSPACE2DTOIMAGESPACE;

	/** 通过三个3维向量构造一个在左手系坐标系里的4*4矩阵，通过参数1输出 \n
	 *	@param [Matrix4&] pOut: 输出的矩阵
	 *	@param [const Vector3&] pEye: 眼睛的位置
	 *	@param [const Vector3&] pAt : 看的位置,注意是位置,不是方向
	 *	@param [const Vector3&] pUp : 上方向
	 */
	static Matrix4* MatrixLookAtLH( Matrix4 &pOut,
									const Vector3 &pEye,
									const Vector3 &pAt,
									const Vector3 &pUp );

	/** 通过三个3维向量构造一个在右手系坐标系里的4*4矩阵，通过参数1输出 
	 *	@param [Matrix4&] pOut: 输出的矩阵
	 *	@param [const Vector3&] pEye: 眼睛的位置
	 *	@param [const Vector3&] pAt : 看的位置,注意是位置,不是方向
	 *	@param [const Vector3&] pUp : 上方向
	 */
	static Matrix4* MatrixLookAtRH( Matrix4 &pOut,
									const Vector3 &pEye,
									const Vector3 &pAt,
									const Vector3 &pUp );


	/** *操作符重载，4*4矩阵的每个值都与参数标量相乘，返回相乘后的结果矩阵 */
    inline Matrix4 operator*(float scalar)
    {
        return Matrix4(
            scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
            scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
            scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
            scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
    }

    /** <<操作符重载，按行输出4*4矩阵 */
    inline friend std::ostream& operator <<
        ( std::ostream& o, const Matrix4& m )
    {
        o << "Matrix4(";
		for (int i = 0; i < 4; ++i)
        {
            o << " row" << (unsigned)i << "{";
            for(int j = 0; j < 4; ++j)
            {
                o << m[i][j] << " ";
            }
            o << "}";
        }
        o << ")";
        return o;
    }
	
	/** 求4*4矩阵的伴随矩阵，并作为函数返回值返回 */
	Matrix4 Adjoint() const;

	/** 对4*4矩阵求其行列式值 */
	float Determinant() const;

	/** 对4*4矩阵求逆矩阵，返回值为逆矩阵 */
	Matrix4 Inverse() const;

    /* *操作符重载，4维向量与4*4矩阵相乘返回一个4维向量 */
    inline friend Vector4 operator * (const Vector4& v, const Matrix4& mat)
    {
        return Vector4(
            v.x*mat[0][0] + v.y*mat[1][0] + v.z*mat[2][0] + v.w*mat[3][0],
            v.x*mat[0][1] + v.y*mat[1][1] + v.z*mat[2][1] + v.w*mat[3][1],
            v.x*mat[0][2] + v.y*mat[1][2] + v.z*mat[2][2] + v.w*mat[3][2],
            v.x*mat[0][3] + v.y*mat[1][3] + v.z*mat[2][3] + v.w*mat[3][3]
            );
    }
};



#endif