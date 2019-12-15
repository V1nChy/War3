#ifndef __VERTEXFORMAT__
#define __VERTEXFORMAT__

#include <d3dx9.h>
#include "MyColor.h"
#include "Vector.h"

/** 有光照的顶点 */
struct My_LVERTEX
{
    FLOAT    x, y, z;
    D3DCOLOR diffuse, specular;
    FLOAT    tu, tv;
	static DWORD FVF;

	void Init(Vector3 position, MyColor pDiffuse,
		MyColor pSpecular, float pu, float pv)
	{
		x = position.x;
		y = position.y;
		z = position.z;
		
		diffuse = pDiffuse.GetAsARGB();
		specular = pSpecular.GetAsARGB();

		tu = pu;
		tv = pv;
	}
	void SetPos(Vector3 position)
	{
		x = position.x;
		y = position.y;
		z = position.z;	
	}
};


/** 经过变换的有光照的顶点 */
struct My_TLVERTEX
{
    float    x, y, z, rhw;
    D3DCOLOR diffuse;
    float    tu, tv;

	static DWORD FVF;

	void Init(Vector3 position, float pRhw, MyColor pDiffuse,
				float pu, float pv)
	{
		x = position.x;
		y = position.y;
		z = position.z;

		rhw = pRhw;

		diffuse = pDiffuse.GetAsARGB();
		
		tu = pu;
		tv = pv;
	}

	void Init(float px, float py, float pu, float pv)
	{
		x = px;
		y = py;
		z = 0;

		rhw = 1.f;

		diffuse = 0xFFFFFFFF;
		
		tu = pu;
		tv = pv;
	}

	void SetPos(D3DXVECTOR3 position)
	{
		x = position.x;
		y = position.y;
		z = position.z;	
	}
};


/** 没有光照的3D顶点 */
struct My_NLVERTEX
{
	float    x, y, z;
	D3DCOLOR diffuse;
	float    tu, tv;
	static DWORD FVF;

	void Init(Vector3 position, float pu, float pv)
	{
		diffuse = MyColor::White.GetAsARGB();
		x = position.x;
		y = position.y;
		z = position.z;
		tu = pu;
		tv = pv;
	}
};


/** 没有光照、纹理的3D顶点 */
struct My_NLNTVERTEX
{
	float    x, y, z;
	D3DCOLOR diffuse;
	static DWORD FVF;

	void Init(const Vector3& position, const MyColor& color = MyColor::White)
	{
		diffuse = color.GetAsARGB();
		x = position.x;
		y = position.y;
		z = position.z;
	}
	const My_NLNTVERTEX& operator = (const My_NLNTVERTEX& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
		diffuse = v.diffuse;
		return *this;
	}
};



#endif


