#pragma once
#include <assert.h>
#include <d3dx9.h>
#include <math.h>
#define safe_delete(p)			{ if(p) { delete (p);		(p)=0; } }
#define safe_delete_array(p)	{ if(p) { delete[] (p);	(p)=0; } }
#define safe_release(p)			{ if(p) { (p)->Release();	(p)=0; } }


//简陋的常用几何
namespace LODmath
{
//一个2维的位标记
class CBit
{

private:
	UINT*		m_pBits;				//存储位标志的指针
	UINT		m_Row;					//行数
	UINT		m_Col;					//列数

public:
	CBit(); 
	~CBit();

	bool Create(UINT Row, UINT Col);							//创建标志数组
	inline void Set(UINT Row, UINT Col, bool bFlog = true);		//设置标志位			
	void Reset();												//标志清零
	inline bool IsTrue(UINT Row, UINT Col);						//查询该标志

};
//成员函数实现...
inline void CBit::Set(UINT R, UINT C, bool bFlog)
{
	assert(m_pBits);

	bFlog ? m_pBits[(R*m_Col+C)/32] |= (1<<(R*m_Col+C)%32)  
		:m_pBits[(R*m_Col+C)/32] &= ~(1<<(R*m_Col+C)%32);

}
inline bool CBit::IsTrue(UINT R, UINT C)
{
	assert(m_pBits);

	return  (m_pBits[(R*m_Col+C)/32] & (1<<(R*m_Col+C)%32)) != 0;
}


struct Rect : public RECT
{
	Rect();
	Rect(int l, int t, int r, int b);

	long GetWidth() const{return right - left;}
	long GetHeight() const{return bottom - top;}

	void SetRect(long _left, long _top, long _right, long _bottom);
	void SetSize(long lWidth,long lHeight);
	void SetPos(const POINT& pt);
	bool In(const POINT& pt) const;
};
inline Rect::Rect()
{ 
	left = top = right = bottom = 0;
}


inline Rect::Rect(int l, int t, int r, int b)
{
	left	= l;
	top		= t;
	right	= r;
	bottom	= b; 
}
inline void Rect::SetRect(long _left, long _top, long _right, long _bottom)
{
	left	= _left;
	top		= _top;
	right	= _right;
	bottom	= _bottom; 
}
inline void Rect::SetSize(long lWidth,long lHeight)
{
	right = lWidth; 
	bottom = lHeight;
}
inline void Rect::SetPos(const POINT& pt)
{
	left += pt.x; top += pt.y;
	right += pt.x; bottom += pt.y;
}
inline bool Rect::In(const POINT& pt) const
{ 
	return (pt.x<right && pt.x>left &&  pt.y>top && pt.y<bottom);
}

//3d矩形，在空间中有z深度
struct Rect3d 
{
public:
	D3DXVECTOR3 Pos0;
	D3DXVECTOR3 Pos1;

	Rect3d(){};
	Rect3d(float _x0, float _x1, float _y0, float _y1, float _z);
	Rect3d(const D3DXVECTOR2& pos0,const D3DXVECTOR2& pos1, float _z);
	Rect3d(const Rect3d& src);
	Rect3d(float _xSize, float _ySize);
	Rect3d(const D3DXVECTOR3& pos,float _xSize, float _ySize);
	explicit Rect3d(const D3DXVECTOR2& size);
	~Rect3d(void){};

	void set(const Rect3d& v);
	void set(float _x0, float _x1, float _y0, float _y1, float _z); 
	void setSize(float _xSize, float _ySize);
	void setSize(const D3DXVECTOR2& size); 
	void setPos(const D3DXVECTOR3& pos);
	D3DXVECTOR2 size()const;
private:

	virtual void Update(){}

};
inline Rect3d::Rect3d(float _x0, float _x1, float _y0, float _y1, float _z)
{
	Pos0.x = _x0;Pos0.y = _y0;Pos0.z = _z;
	Pos1.x = _x1;Pos1.y = _y1;Pos1.z = _z;
	Update();
}
inline Rect3d::Rect3d(const D3DXVECTOR2& pos0,const D3DXVECTOR2& pos1, float _z)
{
	Pos0.x = pos0.x;Pos0.y = pos0.y;Pos0.z = _z;
	Pos1.x = pos1.x;Pos1.y = pos1.y;Pos1.z = _z;
	Update();
}
inline Rect3d::Rect3d(const Rect3d& src)
{
	Pos0 = src.Pos0;
	Pos1 = src.Pos1;
	Update();
}
inline Rect3d::Rect3d(float _xSize, float _ySize)
{
	Pos0.x = Pos0.y  = Pos0.z = Pos1.z = 0.0f;
	Pos1.x = _xSize;Pos1.y = _ySize;
	Update();
}
inline Rect3d::Rect3d(const D3DXVECTOR3& pos,float _xSize, float _ySize)
{
	Pos0 = pos; Pos1.x = Pos0.x + _xSize; Pos1.y = Pos0.y + _ySize;Pos1.z = Pos0.z;
	Update();
}
inline Rect3d::Rect3d(const D3DXVECTOR2& size)
{
	Pos0.x = Pos0.y  = Pos0.z = Pos1.z = 0.0f;
	Pos1.x = size.x;Pos1.y = size.y;
	Update();
}
inline void Rect3d::set(const Rect3d& src)
{
	Pos0 = src.Pos0;
	Pos1 = src.Pos1;
	Update();
}
inline void Rect3d::set(float _x0, float _x1, float _y0, float _y1, float _z)
{
	Pos0.x = _x0;Pos0.y = _y0;Pos0.z = _z;
	Pos1.x = _x1;Pos1.y = _y1;Pos1.z = _z;
	Update();
}
inline void Rect3d::setSize(float _xSize, float _ySize)
{
	Pos1.x = _xSize;Pos1.y = _ySize;
	Update();
}
inline void Rect3d::setSize(const D3DXVECTOR2& size)
{
	Pos1.x = size.x;Pos1.y = size.y;
	Update();
}
inline void Rect3d::setPos(const D3DXVECTOR3& pos)
{
	Pos1 = Pos1 - Pos0 + pos;Pos0 = pos;
	Update();
}
inline D3DXVECTOR2 Rect3d::size()const
{
	return D3DXVECTOR2(Pos1 - Pos0);
}

//3d矩形扩展，增加了矩形中心点和2条对角线的中心向量
//增加以上信息可在剔除上面提高效率
struct Rect3dEx: public Rect3d 
{
	Rect3dEx(){}
	Rect3dEx(float _x0, float _x1, float _y0, float _y1, float _z);
	Rect3dEx(const D3DXVECTOR2& pos0,const D3DXVECTOR2& pos1, float _z);
	Rect3dEx(const Rect3d& src);
	Rect3dEx(float _xSize, float _ySize);
	Rect3dEx(const D3DXVECTOR3& pos,float _xSize, float _ySize);
	explicit Rect3dEx(const D3DXVECTOR2& size);

	
	D3DXVECTOR3 Center;
	D3DXVECTOR3 Normal0;
	D3DXVECTOR3 Normal1;
private:
	void Update();

};
inline Rect3dEx::Rect3dEx(float _x0, float _x1, float _y0, float _y1, float _z)
:Rect3d(_x0,  _x1,  _y0,  _y1, _z){}

inline Rect3dEx::Rect3dEx(const D3DXVECTOR2& pos0,const D3DXVECTOR2& pos1, float _z)
:Rect3d(pos0,pos1,_z){}
inline Rect3dEx::Rect3dEx(const Rect3d& src)
:Rect3d(src){}
inline Rect3dEx::Rect3dEx(float _xSize, float _ySize)
:Rect3d(_xSize,_ySize){}
inline Rect3dEx::Rect3dEx(const D3DXVECTOR3& pos,float _xSize, float _ySize)
:Rect3d(pos,_xSize,_ySize){}
inline Rect3dEx::Rect3dEx(const D3DXVECTOR2& size)
:Rect3d(size){}
inline void Rect3dEx::Update()
{
	Center = (Pos1 - Pos0)/2.0f + Pos0;
	Normal0 = Pos1 - Center;
	Normal1 = D3DXVECTOR3(Pos1.x,Pos0.y,Pos0.z) - Center;
}
//立方体
struct Cube 
{
public:
	D3DXVECTOR3 Pos0;
	D3DXVECTOR3 Pos1;

	Cube(){};
	Cube(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1);
	Cube(const Cube& src);
	Cube(const D3DXVECTOR3& pos0,const D3DXVECTOR3& pos1);
	Cube(float _xSize, float _ySize, float _zSize);
	Cube(const D3DXVECTOR3& pos,float _xSize, float _ySize, float _zSize);
	explicit Cube(const D3DXVECTOR3& size);
	~Cube(void){};

	void set(const Cube& v);
	void set(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1); 
	void setSize(const D3DXVECTOR3& size); 
	void setPos(const D3DXVECTOR3& pos);
	D3DXVECTOR3 size()const;

private:
	
	virtual void Update(){}

};

inline Cube::Cube(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1)
{
	Pos0.x = _x0;Pos0.y = _y0;Pos0.z = _z0;
	Pos1.x = _x1;Pos1.y = _y1;Pos1.z = _z1;
	Update();
}
inline Cube::Cube(const Cube& src)
{
	Pos0 = src.Pos0;
	Pos1 = src.Pos1;
	Update();
}
inline Cube::Cube(const D3DXVECTOR3& pos0,const D3DXVECTOR3& pos1)
{
	Pos0 = pos0; Pos1 = pos1;
	Update();
}
inline Cube::Cube(float _xSize, float _ySize, float _zSize)
{
	Pos0.x = Pos0.y = Pos0.z = 0.0f;
	Pos1.x = _xSize;Pos1.y = _ySize;Pos1.z = _zSize;
	Update();
}
inline Cube::Cube(const D3DXVECTOR3& pos,float _xSize, float _ySize, float _zSize)
{
	Pos0 = pos;
	Pos1.x = Pos0.x + _xSize;Pos1.y = Pos0.y + _ySize;Pos1.z = Pos0.z + _zSize;
	Update();
}
inline Cube::Cube(const D3DXVECTOR3& size)
{
	Pos0.x = Pos0.y = Pos0.z = 0.0f;
	Pos1.x = size.x;Pos1.y = size.y;Pos1.z = size.z;
	Update();
}
inline void Cube::set(const Cube& src)
{
	Pos0 = src.Pos0;
	Pos1 = src.Pos1;
	Update();
}
inline void Cube::set(float _x0, float _x1, float _y0, float _y1, float _z0, float _z1)
{
	Pos0.x = _x0;Pos0.y = _y0;Pos0.z = _z0;
	Pos1.x = _x1;Pos1.y = _y1;Pos1.z = _z1;
	Update();
}
inline void Cube::setSize(const D3DXVECTOR3& size)
{
	Pos1 = Pos0 + size;
	Update();
}
inline void Cube::setPos(const D3DXVECTOR3& pos)
{
	Pos1 = Pos1 - Pos0 + pos;Pos0 = pos;
	Update();
}
inline D3DXVECTOR3 Cube::size()const
{
	return D3DXVECTOR3(Pos1 - Pos0);
}
//立方体扩展，增加了立方体中心点和4条对角线的中心向量
//增加以上信息可在剔除上面提高效率
struct CubeEx: public Cube 
{
	CubeEx(){}
	CubeEx(float _x0, float _x1, float _y0, float _y1,float _z0, float _z1);
	CubeEx(const D3DXVECTOR3& pos0,const D3DXVECTOR3& pos1);
	CubeEx(const Cube& src);
	CubeEx(float _xSize, float _ySize, float _zSize);
	CubeEx(const D3DXVECTOR3& pos,float _xSize, float _ySize, float _zSize);
	explicit CubeEx(const D3DXVECTOR3& size);

	D3DXVECTOR3 Center;
	D3DXVECTOR3 Normal0;
	D3DXVECTOR3 Normal1;
	D3DXVECTOR3 Normal2;
	D3DXVECTOR3 Normal3;
private:
	void Update();

};
inline CubeEx::CubeEx(float _x0, float _x1, float _y0, float _y1,float _z0, float _z1)
:Cube(_x0,  _x1,  _y0,  _y1, _z0, _z1){}

inline CubeEx::CubeEx(const D3DXVECTOR3& pos0,const D3DXVECTOR3& pos1)
:Cube(pos0,pos1){}
inline CubeEx::CubeEx(const Cube& src)
:Cube(src){}
inline CubeEx::CubeEx(float _xSize, float _ySize, float _zSize)
:Cube(_xSize,_ySize,_zSize){}
inline CubeEx::CubeEx(const D3DXVECTOR3& pos,float _xSize, float _ySize, float _zSize)
:Cube(pos,_xSize,_ySize, _zSize){}
inline CubeEx::CubeEx(const D3DXVECTOR3& size)
:Cube(size){}
inline void CubeEx::Update()
{
	Center = (Pos1 - Pos0)/2.0f + Pos0;
	Normal0 = Pos1 - Center;
	Normal1 = D3DXVECTOR3(Pos1.x,Pos0.y,Pos1.z) - Center;
	Normal2 = D3DXVECTOR3(Pos0.x,Pos0.y,Pos1.z) - Center;
	Normal3 = D3DXVECTOR3(Pos0.x,Pos1.y,Pos1.z) - Center;
}

//平面
struct Plane3d: public D3DXPLANE 
{
	//构造函数
	Plane3d();
	Plane3d( CONST FLOAT* );
	Plane3d( CONST D3DXFLOAT16* );
	Plane3d( FLOAT a, FLOAT b, FLOAT c, FLOAT d );
	Plane3d(const D3DXVECTOR3& Normal,float d);
	Plane3d(const D3DXVECTOR4& V);

	//成员方法
	void Set(float a, float b, float c, float d );
	void Set(const D3DXVECTOR3& Normal,float d);
	void SetNormal(const D3DXVECTOR3& Normal);
	void Setd(float d);
	float GetDistance(const D3DXVECTOR3& v)const;
	void Normalize();
	D3DXVECTOR3 ProjectPointToPlane(const D3DXVECTOR3& Point);
	//转换
	//operator D3DXVECTOR4();	
	//operator D3DXVECTOR3();
	operator float*();
	operator const float* ()const;

};
inline Plane3d::Plane3d()						
{
	a = b = c = d = 0.0f;
}
inline Plane3d::Plane3d( CONST FLOAT* f):D3DXPLANE(f){}
inline Plane3d::Plane3d( CONST D3DXFLOAT16* f):D3DXPLANE(f){}
inline Plane3d::Plane3d( FLOAT a, FLOAT b, FLOAT c, FLOAT d )
:D3DXPLANE(a,b,c,d){}

inline Plane3d::Plane3d(const D3DXVECTOR3& Normal,float d)
{
	Set(Normal,d);
}
inline Plane3d::Plane3d(const D3DXVECTOR4& V)
{
	Set(V.x,V.y,V.z,V.w);
}
inline void Plane3d::Set(float a, float b, float c, float d )
{
	this->a = a;this->b = b;this->c = c;this->d = d;
}
inline void Plane3d::Set(const D3DXVECTOR3& Normal,float d)
{
	a = Normal.x;b = Normal.y; c = Normal.z;this->d = d; 
}
inline void Plane3d::SetNormal(const D3DXVECTOR3& Normal)
{
	a = Normal.x;b = Normal.y; c = Normal.z;
}
inline void Plane3d::Setd(float d)
{
	this->d = d; 
}
inline float Plane3d::GetDistance(const D3DXVECTOR3& v)const
{
	//Normalize();
	//调用前先要规范化平面,在法向量那一侧为正，反之为负。
	return a*v.x + b*v.y + c*v.z + d;
}
inline void Plane3d::Normalize()
{
	D3DXPlaneNormalize(this,this);
}
inline D3DXVECTOR3 Plane3d::ProjectPointToPlane(const D3DXVECTOR3& Point)
{
	float fd = GetDistance(Point);

	return D3DXVECTOR3(Point + D3DXVECTOR3(&a)*(-fd));
}
//inline Plane3d::operator D3DXVECTOR4()
//{
//	return D3DXVECTOR4(&a);
//}
//
//inline Plane3d::operator D3DXVECTOR3()
//{
//	return D3DXVECTOR3(&a);
//}
inline Plane3d::operator float*()
{
	return &a;
}
inline Plane3d::operator const float* ()const
{
	return (const float*)&a;
}


//矩形是否与平面相切或在平面上。
//若是，返回真，否则返回假
inline bool Plane3dTestRect3d(const Plane3d& P,const Rect3d R)
{
	if ((	P.GetDistance(R.Pos0)	> 0.0f)
		|| (P.GetDistance(R.Pos1)	> 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos0.x,R.Pos1.y,R.Pos1.z)) > 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos1.x,R.Pos0.y,R.Pos1.z)) > 0.0f))
	{
		return true;
	}

	return false;

}
//立方体最多需要测试8个顶点
inline bool Plane3dTestCube(const Plane3d& P,const Cube R)
{

	if ((	P.GetDistance(R.Pos0)	> 0.0f)
		|| (P.GetDistance(R.Pos1)	> 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos0.x,R.Pos1.y,R.Pos0.z))	> 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos0.x,R.Pos1.y,R.Pos1.z))	> 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos1.x,R.Pos0.y,R.Pos1.z))	> 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos1.x,R.Pos0.y,R.Pos0.z))	> 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos0.x,R.Pos0.y,R.Pos1.z))	> 0.0f)
		|| (P.GetDistance(D3DXVECTOR3(R.Pos1.x,R.Pos1.y,R.Pos0.z))	> 0.0f))
	{
		return true;
	}

	return false;

}
//矩形是否与平面相切或在平面上。
//若是，返回真，否则返回假
//用的是扩展类型，测试效率要高些
inline bool Plane3dTestRect3dEx(const Plane3d& P,const Rect3dEx& R)
{
	float dCenter = P.GetDistance(R.Center);

	if (dCenter >= 0)	return true;

	dCenter = fabs(dCenter);

	if (	fabs(P.a*R.Normal0.x + P.b*R.Normal0.y + P.c*R.Normal0.z) > dCenter
		||	fabs(P.a*R.Normal1.x + P.b*R.Normal1.y + P.c*R.Normal1.z) > dCenter)
	{
		return true;
	}
	
	return false;

}
//扩展立方体最多需要测试5个顶点
inline bool Plane3dTestCubeEx(const Plane3d& P,const CubeEx& R)
{

	float dCenter = P.GetDistance(R.Center);

	if (dCenter >= 0)	return true;

	dCenter = fabs(dCenter);

	if (	fabs(P.a*R.Normal0.x + P.b*R.Normal0.y + P.c*R.Normal0.z) > dCenter
		||	fabs(P.a*R.Normal1.x + P.b*R.Normal1.y + P.c*R.Normal1.z) > dCenter
		||	fabs(P.a*R.Normal2.x + P.b*R.Normal2.y + P.c*R.Normal2.z) > dCenter
		||	fabs(P.a*R.Normal3.x + P.b*R.Normal3.y + P.c*R.Normal3.z) > dCenter)
	{
		return true;
	}

	return false;

}
//平截头体，用6个平面表示并且含有一个包围盒和平截头体的八个顶点
struct Frustum
{

	Plane3d LeftPlane;
	Plane3d RightPlane;
	Plane3d TopPlane;
	Plane3d BottomPlane;
	Plane3d NearPlane;
	Plane3d FarPlane;

	CubeEx	m_Bound;

	D3DXVECTOR3 m_v[8];

	//提取平截头体6个平面
	void extractFromMatrix(
		const D3DXMATRIX& matrix,
		bool normalizePlanes = true);

	
	bool TestPoint(const D3DXVECTOR3& point) const;
	bool TestRect3d(const Rect3d& rect) const;
	bool TestCube(const Cube& rect) const;
	bool TestRect3dEx(const Rect3dEx& rect) const;
	bool TestCubeEx(const CubeEx& rect) const;
	bool TestSphere(const D3DXVECTOR3& Pos,float r) const;


};


//推导过程可参阅潘李亮《largeLod》文中的第5章
inline void Frustum::extractFromMatrix(
					   const D3DXMATRIX& matrix,
					   bool normalizePlanes )
{
	
	LeftPlane.a = matrix._14 + matrix._11; 
	LeftPlane.b = matrix._24 + matrix._21; 
	LeftPlane.c = matrix._34 + matrix._31; 
	LeftPlane.d = matrix._44 + matrix._41;

	
	RightPlane.a = matrix._14 - matrix._11; 
	RightPlane.b = matrix._24 - matrix._21; 
	RightPlane.c = matrix._34 - matrix._31; 
	RightPlane.d = matrix._44 - matrix._41;


	TopPlane.a = matrix._14 - matrix._12; 
	TopPlane.b = matrix._24 - matrix._22; 
	TopPlane.c = matrix._34 - matrix._32; 
	TopPlane.d = matrix._44 - matrix._42;

	
	BottomPlane.a = matrix._14 + matrix._12; 
	BottomPlane.b = matrix._24 + matrix._22; 
	BottomPlane.c = matrix._34 + matrix._32; 
	BottomPlane.d = matrix._44 + matrix._42;


	NearPlane.a = matrix._13; 
	NearPlane.b = matrix._23; 
	NearPlane.c = matrix._33; 
	NearPlane.d = matrix._43;


	FarPlane.a = matrix._14 - matrix._13; 
	FarPlane.b = matrix._24 - matrix._23; 
	FarPlane.c = matrix._34 - matrix._33; 
	FarPlane.d = matrix._44 - matrix._43; 

	
	if (normalizePlanes)
	{
		LeftPlane.Normalize();
		RightPlane.Normalize();
		TopPlane.Normalize();
		BottomPlane.Normalize();
		NearPlane.Normalize();
		FarPlane.Normalize();
	}

}


inline bool Frustum::TestPoint(const D3DXVECTOR3& point) const
{
	if ((LeftPlane.GetDistance(point)		< 0.0f)
		|| (RightPlane.GetDistance(point)	< 0.0f)
		|| (TopPlane.GetDistance(point)		< 0.0f)
		|| (BottomPlane.GetDistance(point)	< 0.0f)
		|| (NearPlane.GetDistance(point)	< 0.0f)
		|| (FarPlane.GetDistance(point)		< 0.0f))
	{
		return false;
	}

	return true;
}
inline bool Frustum::TestRect3d(const Rect3d& rect) const
{
	if ( !Plane3dTestRect3d(LeftPlane,rect)
		|| !Plane3dTestRect3d(RightPlane,rect)
		|| !Plane3dTestRect3d(TopPlane,rect)
		|| !Plane3dTestRect3d(BottomPlane,rect)
		|| !Plane3dTestRect3d(NearPlane,rect)
		|| !Plane3dTestRect3d(FarPlane,rect))
	{
		return false;
	}

	return true;
	
}
inline bool Frustum::TestCube(const Cube& rect) const
{
	if ( !Plane3dTestCube(LeftPlane,rect)
		|| !Plane3dTestCube(RightPlane,rect)
		|| !Plane3dTestCube(TopPlane,rect)
		|| !Plane3dTestCube(BottomPlane,rect)
		|| !Plane3dTestCube(NearPlane,rect)
		|| !Plane3dTestCube(FarPlane,rect))
	{
		return false;
	}

	return true;
}
inline bool Frustum::TestRect3dEx(const Rect3dEx& rect) const
{
	if ( !Plane3dTestRect3dEx(LeftPlane,rect)
		|| !Plane3dTestRect3dEx(RightPlane,rect)
		|| !Plane3dTestRect3dEx(TopPlane,rect)
		|| !Plane3dTestRect3dEx(BottomPlane,rect)
		|| !Plane3dTestRect3dEx(NearPlane,rect)
		|| !Plane3dTestRect3dEx(FarPlane,rect))
	{
		return false;
	}

	return true;
}
inline bool Frustum::TestCubeEx(const CubeEx& rect) const
{
	if ( !Plane3dTestCubeEx(LeftPlane,rect)
		|| !Plane3dTestCubeEx(RightPlane,rect)
		|| !Plane3dTestCubeEx(TopPlane,rect)
		|| !Plane3dTestCubeEx(BottomPlane,rect)
		|| !Plane3dTestCubeEx(NearPlane,rect)
		|| !Plane3dTestCubeEx(FarPlane,rect))
	{
		return false;
	}

	return true;
}
inline bool Frustum::TestSphere(const D3DXVECTOR3& Pos,float r) const
{
	if (	(LeftPlane.GetDistance(Pos) + r		< 0.0f)
		|| (RightPlane.GetDistance(Pos) + r		< 0.0f)
		|| (NearPlane.GetDistance(Pos)	+ r		< 0.0f)
		|| (FarPlane.GetDistance(Pos)	+ r		< 0.0f)
		|| (TopPlane.GetDistance(Pos)	+ r		< 0.0f)
		|| (BottomPlane.GetDistance(Pos)+ r		< 0.0f)
		)
	{
		return false;
	}

	return true;
}
//窗口点转近投影面
inline D3DXVECTOR2 ClientToView(IDirect3DDevice9* d,int x, int y)
{

	float px = 0.0f;
	float py = 0.0f;

	static D3DVIEWPORT9 vp;
	d->GetViewport(&vp);

	static D3DXMATRIX proj;
	d->GetTransform(D3DTS_PROJECTION, &proj);

	px = ((( 2.0f*x) / vp.Width)  - 1.0f) / proj(0, 0);
	py = (((-2.0f*y) / vp.Height) + 1.0f) / proj(1, 1);



	return D3DXVECTOR2(px, py);

}

//点是否在平截头体内（CVV空间）
inline bool TestPointInCVV(const D3DXMATRIX& ViewProj,const D3DXVECTOR3& pt)
{
	static float x,y,z,w;

	w = ViewProj(0,3) * pt.x + ViewProj(1,3) * pt.y + ViewProj(2,3) * pt.z + ViewProj(3,3);

	x = ViewProj(0,0) * pt.x + ViewProj(1,0) * pt.y + ViewProj(2,0) * pt.z + ViewProj(3,0);

	x = x/w;

	if (x > 1.0f || x < -1.0f) return false;

	y = ViewProj(0,1) * pt.x + ViewProj(1,1) * pt.y + ViewProj(2,1) * pt.z + ViewProj(3,1);

	y = y/w;

	if (y > 1.0f || y < -1.0f) return false;

	z = ViewProj(0,2) * pt.x + ViewProj(1,2) * pt.y + ViewProj(2,2) * pt.z + ViewProj(3,2);

	z = z/w;

	if (z > 1.0f || z < 0.0f) return false;

	return true;
}

inline bool CreateDefaultID3DXFont(	IDirect3DDevice9*	d3d,
									ID3DXFont**			Font,
								    char*				FaceName,			
									INT					Height,
									UINT				Width,
									UINT				Weight = 500	)

{
	D3DXFONT_DESC d3dFont;
	memset(&d3dFont,0,sizeof(d3dFont));

	d3dFont.Height		= Height;
	d3dFont.Width		= Width;  
	d3dFont.Weight		= Weight;					
	d3dFont.Italic		= FALSE;
	d3dFont.CharSet		= DEFAULT_CHARSET;

	strcpy(d3dFont.FaceName,FaceName);


	if(FAILED(D3DXCreateFontIndirect(d3d,&d3dFont,Font)))
	{
		::MessageBox(0, "D3DXCreateFontIndirect() - FAILED", 0, 0);
		return false;
	}

	return true;
}

}