
#include "Vector.h"
#include "Matrix.h"

const Vector2 Vector2::ZERO( 0, 0);
const Vector2 Vector2::X_AXIS( 1, 0);
const Vector2 Vector2::Y_AXIS( 0, 1);
const Vector2 Vector2::NEG_X_AXIS( -1,  0);
const Vector2 Vector2::NEG_Y_AXIS(  0, -1);
const Vector2 Vector2::UNIT_SCALE(1, 1);

const Vector3 Vector3::ZERO( 0, 0, 0 );
const Vector3 Vector3::X_AXIS( 1, 0, 0 );
const Vector3 Vector3::Y_AXIS( 0, 1, 0 );
const Vector3 Vector3::Z_AXIS( 0, 0, 1 );
const Vector3 Vector3::NEG_X_AXIS( -1,  0,  0 );
const Vector3 Vector3::NEG_Y_AXIS(  0, -1,  0 );
const Vector3 Vector3::NEG_Z_AXIS(  0,  0, -1 );
const Vector3 Vector3::UNIT_SCALE(1, 1, 1);


float Vector2::GetRadianBetween2Vector2(Vector2 src, Vector2 dest)
{
	float srcR, destR;
	if (src.x == 0){
		if (src.y >= 0)
			srcR = PI/2;
		else
			srcR = -PI/2;
	}
	else
	{
		srcR = atanf(src.y/src.x);
		if (src.x < 0)
			srcR += PI;
	}


	if (dest.x == 0){
		if (dest.y >= 0)
			destR = PI/2;
		else
			destR = -PI/2;
	}
	else
	{
		destR = atanf(dest.y/dest.x);
		if (dest.x < 0)
			destR += PI;
	}


	float btwR = srcR - destR;
	if (btwR > PI)
		btwR -= 2 * PI;
	else if (btwR < -PI)
		btwR += 2 * PI;

	return btwR;
}



/*
float  Vector3::CalcAngle(Vector3& rotation, Vector3& target)
{
	Vector3 xzDir = rotation;
	xzDir.y = 0;
	xzDir.Normalize();
	Vector3 xzLocation = target - *this;
	xzLocation.y = 0;
	xzLocation.Normalize();
	float angle = atan2f(xzDir.z,xzDir.x) - atan2f(xzLocation.z,xzLocation.x);
	if(angle < 0)angle += 2*PI;
	return angle * RAD_TO_DEG;
}
*/


Vector3 Vector3::GetNormal()
{
	float fx = fabsf(x); 
	float fy = fabsf(y); 
	float fz = fabsf(z); 
	if(fx > fy && fx > fz) return Vector3(1,0,0); 
	if(fy > fx && fy > fz) return Vector3(0,1,0); 
	if(fz > fx && fz > fy) return Vector3(0,0,1); 

	return Vector3(0,0,0);
}


//Functions--------------------------------------------------------------------
Vector3 Vector3::Lerp(const Vector3& v1, float t)
{
	//Linearly interpolate between vectors by an amount t and return the resulting vector. 			
	//v = v0 + (v1 - v0)t = v0 + t(v1) - t(v0) = v0 - t(v0) + t(v1) = (1 - t)v0 + t(v1)
	float omt = 1.f - t;	
	Vector3 v = *this;
	v.x = omt * x + t * v1.x;
	v.y = omt * y + t * v1.y;
	v.z = omt * z + t * v1.z;
	return v;
}

Vector3 Vector3::NormalFromTriangle(const Vector3& v0, const Vector3& v1, const Vector3& v2)
{
	Vector3 n1, n2;
	n1 = v1-v0;n2 = v2-v0;
	return Cross(n1,n2);
}
	


Vector3 Vector3::Cross(const Vector3& v, const Vector3& v2)
{
	Vector3 n;
	n.x = (v.y * v2.z) - (v.z * v2.y);
	n.y = (v.z * v2.x) - (v.x * v2.z);
	n.z = (v.x * v2.y) - (v.y * v2.x);
	return n.GetNormalized();
}

Vector3 Vector3::CrossNoNorm(const Vector3& v, const Vector3& v2)
{
	Vector3 n;
	n.x = (v2.z * v.y) - (v2.y * v.z);
	n.y = (v2.x * v.z) - (v2.z * v.x);
	n.z = (v2.y * v.x) - (v2.x * v.y);
	return n;
}


Vector3 Vector3::operator * ( const Matrix4 &m ) const
{
    Vector3 r;

    float fInvW = 1.f / ( m[0][3] * x + m[1][3] * y + m[2][3] * z + m[3][3] );

    r.x = ( m[0][0] * x + m[1][0] * y + m[2][0] * z + m[3][0] ) * fInvW;
    r.y = ( m[0][1] * x + m[1][1] * y + m[2][1] * z + m[3][1] ) * fInvW;
    r.z = ( m[0][2] * x + m[1][2] * y + m[2][2] * z + m[3][2] ) * fInvW;

    return r;
}



/// FIXME: Hacky and slow
Vector3 Vector3::MakeDirection(float yaw, float pitch, float roll)
{
	//Convert from degrees to radians
	roll  *=0.0174532f;
	pitch *=0.0174532f;
	yaw   *=0.0174532f;

	double X = (sinf(pitch)*sinf(yaw)*sinf(roll))+ (cosf(pitch)*cosf(roll));
	double Y = cosf(yaw)*sinf(roll);
	double Z = (sinf(pitch)*cosf(roll))- (cosf(roll)*sinf(pitch)*sinf(yaw));

	return Vector3((float)X,(float)Y,(float)Z).GetNormalized();
}




float Vector3::DistanceToLine(const Vector3& p0, const Vector3& p1) const
{
	Vector3 p0_to_p = *this - p0;
	Vector3 p0_to_p1 = p1 - p0;

	Vector3 p_on_line = p0_to_p.ProjectionOntoVector(p0_to_p1);
	Vector3 perp	  = p0_to_p - p_on_line;

	return perp.Length();
}


Vector4 Vector4::Lerp(const Vector4& v1, float t)
{
	float _w = 1/w;
	Vector3 _v1(x*_w, y*_w, z*_w);
	_w = 1/v1.w;
	Vector3 _v2(v1.x*_w, v1.y*_w, v1.z*_w);

	Vector3 _r = _v1.Lerp(_v2, t);
	return Vector4(_r.x, _r.y, _r.z, 1);
}


Vector4 Vector4::operator *(const Matrix4& mat)
{
	Vector4 v;
	v.x = mat[0][0] * x + mat[1][0] * y + mat[2][0] * z + mat[3][0] * w;
	v.y = mat[0][1] * x + mat[1][1] * y + mat[2][1] * z + mat[3][1] * w;
	v.z = mat[0][2] * x + mat[1][2] * y + mat[2][2] * z + mat[3][2] * w;
	v.w = mat[0][3] * x + mat[1][3] * y + mat[2][3] * z + mat[3][3] * w;
	return v;
}

