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
    /** Ĭ�Ϲ��캯��
        @note: δ����ʼ��,Ч�ʿ���
    */
	inline Matrix3 () {};
	/** ���캯�� */
    inline explicit Matrix3 (const float arr[3][3])
	{
		memcpy(m,arr,9*sizeof(float));
	}
	/** �������캯�� */
   inline Matrix3 (const Matrix3& rkMatrix)
	{
		memcpy(m,rkMatrix.m,9*sizeof(float));
	}
	/** ���캯�� */
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

    /** []������, ����ʹ�� mat[r][c] */
    inline float* operator[] (int iRow) const
	{
		assert(iRow>=0 && iRow<3);
		return (float*)m[iRow];
	}

	/** �õ���Ӧ�� */
	Vector3 GetRow(int iRow) const;
	/** ������ */
	void SetRow(int iRow, const Vector3& vec);

	/** �õ���Ӧ�У�����һ��vector3 */
    Vector3 GetColumn (int iCol) const;
	/** �Զ�Ӧ�н������� */
    void SetColumn(int iCol, const Vector3& vec);
	/** ���������������ϵ�ͶӰ�õ�һ������ */
    void FromAxes(const Vector3& xAxis, const Vector3& yAxis, const Vector3& zAxis);

    /** ����֪3*3�����ֵ����3*3������� */
    inline Matrix3& operator= (const Matrix3& rkMatrix)
	{	memcpy(m,rkMatrix.m,9*sizeof(float));	return *this;	}
	/** �ж�����3*3�����Ƿ���� */
	bool operator== (const Matrix3& rkMatrix) const;
	/** �ж�����3*3�����Ƿ���� */
	inline bool operator!= (const Matrix3& rkMatrix) const
	{	return !operator==(rkMatrix);	}


    // arithmetic operations
	/** ����3*3������� */
	Matrix3 operator+ (const Matrix3& rkMatrix) const;
	/** ����3*3������� */
	Matrix3 operator- (const Matrix3& rkMatrix) const;
	/** ����3*3������� */
    Matrix3 operator* (const Matrix3& rkMatrix) const;
	/** 3*3������ԣ�1 */
    Matrix3 operator- () const;

    /** 3*3�������������� */ 
    Vector3 operator* (const Vector3& rkVector) const;

    /** ��������3*3������� */
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

    /** 3*3������������ */
    Matrix3 operator* (float fScalar) const;

    /** ������3*3������� */
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
	/** ��3*3������ת�þ��� */
    Matrix3 Transpose () const;
	/** ��3*3���������������������ڣ��ѽ��������һ������������bool 1��
		�������󲻴��ڣ��򷵻�bool 0��������ԭ�������ΪO�ľ��󸳸���һ������ 
	 */
    bool Inverse (Matrix3& rkInverse, float fTolerance = 1e-06) const;
	/** ��3*3�����������,����������ڣ��򷵻�������������󲻴��ڣ��򷵻��������ԭ�������ΪO�ľ���.
	 * @note: ������const�� 
	 */
    Matrix3 Inverse (float fTolerance = 1e-06) const;
	/** ��3*3���������ʽֵ */
    float Determinant () const;

	/** ����ֵ�ֽ⣬�Ѿ���ֽ��[U��S��V]������֮��������U��VΪ��������SΪ�ԽǾ���
	 *	@note: �Ȳ�Ҫ��,��δ��������
	 */
    void SingularValueDecomposition (Matrix3& rkL, Vector3& rkS,
        Matrix3& rkR) const;
	/** ����ֵ���
	 *	@note: �Ȳ�Ҫ��,��δ��������
	 */
    void SingularValueComposition (const Matrix3& rkL,
        const Vector3& rkS, const Matrix3& rkR);

    /** ��3*3���������� Gram-Schmidt orthonormalization (applied to columns of rotation matrix) */
    void Orthonormalize ();

	/** QDU�ֽ⣺��3*3����ֽ��һ����������һ���ԽǾ���һ�������Ǿ���01 02 12λ�ã�
		����ֱ𴫸��������������ں���������ֻ���������֣�����vector3����ʾ��
	 */
    void QDUDecomposition (Matrix3& rkQ, Vector3& rkD,
        Vector3& rkU) const;

	/** �����ķ��� */
    float SpectralNorm () const;

	/** һ��3*3����任Ϊ��rkAxis��תrfAngle����
	 *	@note: matrix must be orthonormal
	 */
    void ToAxisRadian (Vector3& rkAxis, float& rfAngle) const;
	/** һ��3*3����任Ϊ��rkAxis��תrfAngle�Ƕ�
	 *	@note: matrix must be orthonormal
	 */
	inline void ToAxisDegree (Vector3& rkAxis, float& rfAngle) const {
		float r;
		ToAxisRadian ( rkAxis, r );
		rfAngle = RAD_TO_DEG(r);
	}
	/** һ��3ά������������ת�ᣬfloat������Ϊ��ת�Ƕȣ���ת��һ��3*3�����ֵ�������ö������ */
    void FromAxisRadian (const Vector3& rkAxis, const float fRadians);


 	/** ������ŷ���任�зֱ���XYZ���ٻ����ܵõ����ö�����󣬰ѽǶȷ��ظ�����������
		�����Ψһ�⺯������ֵΪ�棬����ⲻΨһ�򷵻�ֵΪ�١�
	 *	@note: �����Ǹ���ת����(����Ҫ����)
	 */
    bool ToEulerAnglesXYZ (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesXZY (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesYXZ (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesYZX (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesZXY (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;
    bool ToEulerAnglesZYX (float& rfYAngle, float& rfPAngle, float& rfRAngle) const;

	/** ��ŷ���任����ϵ�У��ֱ���XYZ�����ֵ�ĽǶȣ����ȣ����ѵõ��ľ��󸳸����ö������ */
    void FromEulerAnglesXYZ (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesXZY (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesYXZ (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesYZX (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesZXY (const float fYAngle, const float fPAngle, const float fRAngle);
    void FromEulerAnglesZYX (const float fYAngle, const float fPAngle, const float fRAngle);


    /** ����3*3���������ֵ����������,�ֱ𸳸���һ���͵ڶ������� */
    void EigenSolveSymmetric (float afEigenvalue[3],
        Vector3 akEigenvector[3]) const;

	/** ǰ����������������������3*3���󣩸������������� */
    static void TensorProduct (const Vector3& rkU, const Vector3& rkV,
        Matrix3& rkProduct);

	/** �õ�һ������dir����,ͬʱup������y��ľ��� */
	static Matrix3 LookTowards(const Vector3& dir);
	/** ����һ������(����ϵ), y������up����,x������x�� */
	static Matrix3 LookToUp(const Vector3& up); 
	/** ����һ������(����ϵ), ����to,����up */
	static Matrix3 LookTowardsUp(const Vector3& to,const Vector3& up);

 
	// ������ת , ��������Ӧ��Ļ���ֵ
	/** ��X��������ת ����radian��������ת��3*3�����ָ�� */
	const Matrix3& RotationX(float radian);
	/** ��Y��������ת ����radian��������ת��3*3�����ָ�� */
	const Matrix3& RotationY(float radian);
	/** ��Z��������ת ����radian��������ת��3*3�����ָ�� */
	const Matrix3& RotationZ(float radian);
	/** �ֱ���XYZ����������ת �������Ⱥ󣬷�����ת��3*3�����ָ�� */
	const Matrix3& RotationXYZ(float radianX, float radianY,  float radianZ);

	// �õ�����Ӧ�����ת����
	/** �õ�����һ��3*3���������Զ��������ö������õ���X����תradian���ȵ�Ч�� */
	static Matrix3 GetRotaionMatrixX(float radian);
	/** �õ�����һ��3*3���������Զ��������ö������õ���Y����תradian���ȵ�Ч�� */
	static Matrix3 GetRotaionMatrixY(float radian);
	/** �õ�����һ��3*3���������Զ��������ö������õ���Z����תradian���ȵ�Ч�� */
	static Matrix3 GetRotaionMatrixZ(float radian);
	/** �õ�����һ��3*3���������Զ��������ö������õ��ֱ���XYZ������ת�������Ⱥ��Ч�� */
	static Matrix3 GetRotaionMatrixXYZ(float radianX, float radianY,  float radianZ);	
	
	static const float EPSILON;
    static const Matrix3 ZERO;			/// �����
    static const Matrix3 IDENTITY;		/// ��λ����


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
	/** �õ�һ������dir����,ͬʱup������y��ľ��� */
	static Matrix4 LookTowards(const Vector3& dir);
	/** ����һ������(����ϵ), ����to,����up */
	static Matrix4 LookTowardsUp(const Vector3& to,const Vector3& up);


	/** �õ�4*4�����ҷ���ı�׼��,����Ǹ���λ���� */
	Vector3 GetRight() const { return Vector3(m[0][0], m[0][1], m[0][2]).GetNormalized(); }
	/** �õ�4*4�����Ϸ���ı�׼��,����Ǹ���λ���� */
	Vector3 GetUp() const { return Vector3(m[1][0], m[1][1], m[1][2]).GetNormalized(); }
	/** �õ�4*4����ǰ����ı�׼��,����Ǹ���λ���� */
	Vector3 GetDir() const { return Vector3(m[2][0], m[2][1], m[2][2]).GetNormalized(); }

	/** �õ�4*4�����ҷ���ķǱ�׼��,���δ����λ�� */
	Vector3 GetRightUnnormalized() const { return Vector3(m[0][0], m[0][1], m[0][2]); }
	/** �õ�4*4�����Ϸ���ķǱ�׼��,���δ����λ�� */
	Vector3 GetUpUnnormalized() const { return Vector3(m[1][0], m[1][1], m[1][2]); }
	/** �õ�4*4����ǰ����ķǱ�׼��,���δ����λ�� */
	Vector3 GetDirUnnormalized() const { return Vector3(m[2][0], m[2][1], m[2][2]); }


	/** �õ���Ӧ�У�����һ��vector3 */
    Vector3 GetColumn (int iCol) const;
	/** �Զ�Ӧ�н������� */
    void SetColumn(int iCol, const Vector3& vec);

	/** �õ���Ӧ�� */
	Vector3 GetRow(int iRow) const;
	/** ������ */
	void SetRow(int iRow, const Vector3& vec);


	// ������ת , ��������Ӧ��Ļ���ֵ
	/** ��X��������ת ����radian��������ת��4*4�����ָ�� */
	const Matrix4& RotationX(float radian);
	/** ��Y��������ת ����radian��������ת��4*4�����ָ�� */
	const Matrix4& RotationY(float radian);
	/** ��Z��������ת ����radian��������ת��4*4�����ָ�� */
	const Matrix4& RotationZ(float radian);
	/** �ֱ���XYZ����������ת �������Ⱥ󣬷�����ת��4*4�����ָ�� */
	const Matrix4& RotationXYZ(float radianX, float radianY,  float radianZ);

	// �õ�����Ӧ�����ת����
	/** �õ�����һ��4*4���������Զ��������ö������õ���X����תradian���ȵ�Ч�� */
	static Matrix4 GetRotaionMatrixX(float radian);
	/** �õ�����һ��4*4���������Զ��������ö������õ���Y����תradian���ȵ�Ч�� */
	static Matrix4 GetRotaionMatrixY(float radian);
	/** �õ�����һ��4*4���������Զ��������ö������õ���Z����תradian���ȵ�Ч�� */
	static Matrix4 GetRotaionMatrixZ(float radian);
	/** �õ�����һ��4*4���������Զ��������ö������õ��ֱ���XYZ������ת�������Ⱥ��Ч�� */
	static Matrix4 GetRotaionMatrixXYZ(float radianX, float radianY,  float radianZ);

	/** Ĭ�Ϲ��캯��,��ʼ������Ϊ4*4��λ���� */
    inline Matrix4()
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }

	/** ��16��������ʼ������ */
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

    /** ͨ��3*3������һ��4*4����ת/���ž���,��ƫ������0  */
    inline Matrix4(const Matrix3& m3x3)
    {
      operator=(IDENTITY);
      operator=(m3x3);
    }

    /** ͨ��һ����Ԫ����ʼ��һ��4*4����ת/���ž��� */
    inline Matrix4(const Quaternion& rot)
    {
      Matrix3 m3x3;
      rot.ToRotationMatrix(m3x3);
      operator=(IDENTITY);
      operator=(m3x3);
    }
    
	/** []���أ��õ�4*4�����һ�е��׵�ַ�������ڱ��� */
    inline float* operator [] ( int iRow )
    {
        assert( iRow < 4 );
        return m[iRow];
    }

	/** []���أ��õ�4*4�����һ�е��׵�ַ�������ڳ��� */
   inline const float *const operator [] ( int iRow ) const
    {
        assert( iRow < 4 );
        return m[iRow];
    }

	/** ������ */
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

	/** *���������أ�����4*4������� */
    inline Matrix4 operator * ( const Matrix4 &m2 ) const
    {
        return Concatenate( m2 );
    }
	/** *=���������أ�����4*4�������,�ѽ���������ö��� */
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

    /** +���������أ�����4*4������� */
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

    /** �����������أ�����4*4������������ö����ȥ������ */
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

    /** �������������أ��ж�����4*4�����Ƿ���� */
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

    /** �������������أ��ж�����4*4�����Ƿ���� */
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

    /** �����������أ��Ѳ���3*3����ֵ��4*4���󣬽��Ϊ4*4��λ�����ǰ�������м��ϲ���3*3���� */
    inline void operator = ( const Matrix3& mat3 )
    {
        m[0][0] = mat3.m[0][0]; m[0][1] = mat3.m[0][1]; m[0][2] = mat3.m[0][2]; //m[0][3] = 0;
        m[1][0] = mat3.m[1][0]; m[1][1] = mat3.m[1][1]; m[1][2] = mat3.m[1][2]; //m[1][3] = 0;
        m[2][0] = mat3.m[2][0]; m[2][1] = mat3.m[2][1]; m[2][2] = mat3.m[2][2]; //m[2][3] = 0;
	//	m[3][0] = 0;			m[3][1] = 0	;			m[3][2] = 0;			//m[3][3] = 1;
   }

	/** ������������ */
	inline const Matrix4& operator = ( const Matrix4& mat4)
	{	memcpy(m, mat4.m,16*sizeof(float));	return *this; }

	/** �õ�4*4�����ת�þ��� */
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

    /** ��һ��3ά�������趨4*4�����ͶӰ�任��ֵ�����趨[0][3]��[1][3]��[2][3]λ�õ�ֵ�� */
    inline Matrix4 SetProj( const Vector3& v )
    {
        m[0][3] = v.x;
        m[1][3] = v.y;
        m[2][3] = v.z;
		return *this;
    }

	/** ��һ��3ά�������趨4*4�����λ�Ʊ任��ֵ�����趨[3][0]��[3][1]��[3][2]λ�õ�ֵ�� */
    inline Matrix4 SetOffset( const Vector3& v )
    {
        m[3][0] = v.x;
        m[3][1] = v.y;
        m[3][2] = v.z;
		return *this;
    }

    /** ���4*4�����ͶӰ�任��ֵ����[0][3]��[1][3]��[2][3]λ�õ�ֵ�� */
    inline Vector3 GetProj() const
    {
      return Vector3(m[0][3], m[1][3], m[2][3]);
    }
    
	/** ���4*4�����λ�Ʊ任��ֵ����[0][3]��[1][3]��[2][3]λ�õ�ֵ�� */
    inline Vector3 GetOffset() const
    {
      return Vector3(m[3][0], m[3][1], m[3][2]);
    }

    /** ��һ��3ά�����ѵ��ö���������۾���ԭ������ô���ģ����һ��ͶӰת������ */
    inline void MakeProj( const Vector3& v )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = v.x;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = v.y;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = v.z;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }

	/** ��һ��3ά�����ѵ��ö���������۾���ԭ������ô���ģ����һ��λ��ת������ */
    inline void MakeOffset( const Vector3& v )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = v.x; m[3][1] = v.y; m[3][2] = v.z; m[3][3] = 1.0;
    }


	/** ��3��float�����ѵ��ö���������۾���ԭ������ô���ģ����һ��ͶӰת������ */
    inline void MakeProj( float tx, float ty, float tz )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = tx;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = ty;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = tz;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
    }

	/** ��3��float�����ѵ��ö���������۾���ԭ������ô���ģ����һ��λ��ת������ */
	inline void MakeOffset( float tx, float ty, float tz )
    {
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = tx ; m[3][1] = ty ; m[3][2] = tz ; m[3][3] = 1.0;
    }

    /** ��һ��3ά�����ѵ��ö���������۾���ԭ������ô���ģ����һ��ͶӰת�����󣬲������������Ϊ��������ֵ  */
    inline static Matrix4 GetProj( const Vector3& v )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = v.x;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = v.y;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = v.z;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

        return r;
    }

	/** ��һ��3ά�����ѵ��ö���������۾���ԭ������ô���ģ����һ��ƽ��ת�����󣬲������������Ϊ��������ֵ  */
    inline static Matrix4 GetOffset( const Vector3& v )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = 0.0;
        r.m[3][0] = v.x; r.m[3][1] = v.y; r.m[3][2] = v.z; r.m[3][3] = 1.0;

        return r;
    }


	/** ��3��float�����ѵ��ö���������۾���ԭ������ô���ģ����һ��ͶӰ��ת�����󣬲������������Ϊ��������ֵ */
    inline static Matrix4 GetProj( float t_x, float t_y, float t_z )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = t_x;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = t_y;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = t_z;
        r.m[3][0] = 0.0; r.m[3][1] = 0.0; r.m[3][2] = 0.0; r.m[3][3] = 1.0;

        return r;
    }

	/** ��3��float�����ѵ��ö���������۾���ԭ������ô���ģ����һ��ƽ�Ƶ�ת�����󣬲������������Ϊ��������ֵ */
    inline static Matrix4 GetOffset( float t_x, float t_y, float t_z )
    {
        Matrix4 r;

        r.m[0][0] = 1.0; r.m[0][1] = 0.0; r.m[0][2] = 0.0; r.m[0][3] = 0.0;
        r.m[1][0] = 0.0; r.m[1][1] = 1.0; r.m[1][2] = 0.0; r.m[1][3] = 0.0;
        r.m[2][0] = 0.0; r.m[2][1] = 0.0; r.m[2][2] = 1.0; r.m[2][3] = 0.0;
        r.m[3][0] = t_x; r.m[3][1] = t_y; r.m[3][2] = t_z; r.m[3][3] = 1.0;

        return r;
    }

	/** �õ�4��4������ָ�� */
	inline const float * const Get4_4Array() const
	{ return _m; }

	inline void SetMatrix(const float * const p4_4Array)
	{
		memcpy(_m, p4_4Array, sizeof(_m));
	}

	/** ��4*4�����ʼ������Ϊ4*4��λ���� */
	inline void Identify()
	{
        m[0][0] = 1.0; m[0][1] = 0.0; m[0][2] = 0.0; m[0][3] = 0.0;
        m[1][0] = 0.0; m[1][1] = 1.0; m[1][2] = 0.0; m[1][3] = 0.0;
        m[2][0] = 0.0; m[2][1] = 0.0; m[2][2] = 1.0; m[2][3] = 0.0;
        m[3][0] = 0.0; m[3][1] = 0.0; m[3][2] = 0.0; m[3][3] = 1.0;
	}

	/** ��4*4�����ʼ������Ϊ4*4����� */
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

    /** �ò���3ά�������ö���4*4�����scaleֵ��[0][0],[1][1],[2][2]λ�õ�ֵ��    */
    inline void SetScale( const Vector3& v )
    {
        m[0][0] = v.x;
        m[1][1] = v.y;
        m[2][2] = v.z;
    }

	/** �������ţ�[3][3]λ�õ�ֵ��    */
	inline void SetScale( const float& scale )
	{
		m[3][3] = 1.f/scale;
	}


    /** �õ�һ�����ž���
	 *	@param [const Vector&] v : x��, y��, z�� ������ϵ��
	 *	@return Matrix4 : �õ���4��4���ž���
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

	/** �õ�һ�����ž���
	 *	@param [float] x y z : x��, y��, z�� ������ϵ��
	 *	@return Matrix4 : �õ���4��4���ž���
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

    /** ���4ά�����б�ʾ��ת,���ŵ��Ǹ���ά���� 
     *	@param [Matrix3&] : �������,Matrix3
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

    /** ͨ��һ��4*4����õ�һ����Ԫ��(��ת) */
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

	/** ͨ������3ά��������һ��������ϵ����ϵ���4*4����ͨ������1��� \n
	 *	@param [Matrix4&] pOut: ����ľ���
	 *	@param [const Vector3&] pEye: �۾���λ��
	 *	@param [const Vector3&] pAt : ����λ��,ע����λ��,���Ƿ���
	 *	@param [const Vector3&] pUp : �Ϸ���
	 */
	static Matrix4* MatrixLookAtLH( Matrix4 &pOut,
									const Vector3 &pEye,
									const Vector3 &pAt,
									const Vector3 &pUp );

	/** ͨ������3ά��������һ��������ϵ����ϵ���4*4����ͨ������1��� 
	 *	@param [Matrix4&] pOut: ����ľ���
	 *	@param [const Vector3&] pEye: �۾���λ��
	 *	@param [const Vector3&] pAt : ����λ��,ע����λ��,���Ƿ���
	 *	@param [const Vector3&] pUp : �Ϸ���
	 */
	static Matrix4* MatrixLookAtRH( Matrix4 &pOut,
									const Vector3 &pEye,
									const Vector3 &pAt,
									const Vector3 &pUp );


	/** *���������أ�4*4�����ÿ��ֵ�������������ˣ�������˺�Ľ������ */
    inline Matrix4 operator*(float scalar)
    {
        return Matrix4(
            scalar*m[0][0], scalar*m[0][1], scalar*m[0][2], scalar*m[0][3],
            scalar*m[1][0], scalar*m[1][1], scalar*m[1][2], scalar*m[1][3],
            scalar*m[2][0], scalar*m[2][1], scalar*m[2][2], scalar*m[2][3],
            scalar*m[3][0], scalar*m[3][1], scalar*m[3][2], scalar*m[3][3]);
    }

    /** <<���������أ��������4*4���� */
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
	
	/** ��4*4����İ�����󣬲���Ϊ��������ֵ���� */
	Matrix4 Adjoint() const;

	/** ��4*4������������ʽֵ */
	float Determinant() const;

	/** ��4*4����������󣬷���ֵΪ����� */
	Matrix4 Inverse() const;

    /* *���������أ�4ά������4*4������˷���һ��4ά���� */
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