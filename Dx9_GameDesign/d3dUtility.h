#pragma once

#include <d3dx9.h>
#include <string>
#include <limits>
#include <String>
#include <List>
#include <Map>
using namespace std;
//
// 常量
//
const float FINFINITY = FLT_MAX;
const float EPSILON = 0.001f;

//屏幕大小
const int ScreenWidth = GetSystemMetrics(SM_CXSCREEN);
const int ScreenHeight = GetSystemMetrics(SM_CYSCREEN);

//测试按键是否按下
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEYUP(vk_code)   ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

//安全删除
#define SAFE_RELEASE(object) {if(object){object->Release();object=NULL;}}
#define SAFE_DELETE(object) {if(object){delete object;object=NULL;}}
#define SAFE_DELARRAY(object) {if(object){delete[] object;object=NULL;}}

//按键
#define		UIT01		0
#define		UIT02		1
#define		UIT03		2
#define		UIT04		3
#define		MINIMAP		4
#define		BUTTONROLE		5
#define		BUTTONCENTER		6
#define		BTNATTACK		7
#define		BTNHOLDPOSITION		8
#define		BTNMOVE		9
#define		BTNNONE		10
#define		BTNPATROL		11
#define		BTNSTOP		12
#define		BTNSKILLZ		13
#define		BTNSKILL1		14

//
// Cleanup
//
template<class T> void Release(T t)
{
	if( t )
	{
		t->Release();
		t = 0;
	}
}
	
template<class T> void Delete(T t)
{
	if( t )
	{
		delete t;
		t = 0;
	}
}

//
// Colors
//
const D3DXCOLOR      WHITE( D3DCOLOR_XRGB(255, 255, 255) );
const D3DXCOLOR      BLACK( D3DCOLOR_XRGB(  0,   0,   0) );
const D3DXCOLOR        RED( D3DCOLOR_XRGB(255,   0,   0) );
const D3DXCOLOR      GREEN( D3DCOLOR_XRGB(  0, 255,   0) );
const D3DXCOLOR       BLUE( D3DCOLOR_XRGB(  0,   0, 255) );
const D3DXCOLOR     YELLOW( D3DCOLOR_XRGB(255, 255,   0) );
const D3DXCOLOR       CYAN( D3DCOLOR_XRGB(  0, 255, 255) );
const D3DXCOLOR    MAGENTA( D3DCOLOR_XRGB(255,   0, 255) );

const D3DXCOLOR BEACH_SAND( D3DCOLOR_XRGB(255, 249, 157) );
const D3DXCOLOR DESERT_SAND( D3DCOLOR_XRGB(250, 205, 135) );

const D3DXCOLOR LIGHTGREEN( D3DCOLOR_XRGB( 60, 184, 120) );
const D3DXCOLOR  PUREGREEN( D3DCOLOR_XRGB(  0, 166,  81) );
const D3DXCOLOR  DARKGREEN( D3DCOLOR_XRGB(  0, 114,  54) );

const D3DXCOLOR LIGHT_YELLOW_GREEN( D3DCOLOR_XRGB(124, 197, 118) );
const D3DXCOLOR  PURE_YELLOW_GREEN( D3DCOLOR_XRGB( 57, 181,  74) );
const D3DXCOLOR  DARK_YELLOW_GREEN( D3DCOLOR_XRGB( 25, 123,  48) );

const D3DXCOLOR LIGHTBROWN(D3DCOLOR_XRGB(198, 156, 109));
const D3DXCOLOR DARKBROWN( D3DCOLOR_XRGB(115, 100,  87));

//
// Lights
//

D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color);
D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color);
D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color);

//
// Materials
//

D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p);

const D3DMATERIAL9 WHITE_MTRL  = InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f);
const D3DMATERIAL9 RED_MTRL    = InitMtrl(RED, RED, RED, BLACK, 2.0f);
const D3DMATERIAL9 GREEN_MTRL  = InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f);
const D3DMATERIAL9 BLUE_MTRL   = InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f);
const D3DMATERIAL9 YELLOW_MTRL = InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f);


//
// Vertex Structures
//

struct Vertex
{
	Vertex(){}
	Vertex(float x, float y, float z, 
		float nx, float ny, float nz,
		float u, float v)
	{
		_x  = x;  _y  = y;  _z  = z;
		_nx = nx; _ny = ny; _nz = nz;
		_u  = u;  _v  = v;
	}
	float _x, _y, _z;
	float _nx, _ny, _nz;
	float _u, _v;

	static const DWORD FVF;
};

//
// Custom Vertex
//

struct CUSTOMVERTEX
{
	FLOAT x, y, z, rhw;
	DWORD color;
};

#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW|D3DFVF_DIFFUSE)  //顶点格式

//
// Randomness
//

// Desc: Return random float in [lowBound, highBound] interval.
float GetRandomFloat(float lowBound, float highBound);


// Desc: Returns a random vector in the bounds specified by min and max.
void GetRandomVector(
	D3DXVECTOR3* out,
	D3DXVECTOR3* min,
	D3DXVECTOR3* max);

//
// Conversion
//float转换无符号长整型
DWORD FtoDw(float f);

//
// Interpolation
//插值
float Lerp(float a, float b, float t);

//射线
struct sRay
{
	D3DXVECTOR3 _origin;
	D3DXVECTOR3 _direction;
};

//边界盒
struct BoundingBox
{
	BoundingBox();

	bool isPointInside(D3DXVECTOR3& p);

	D3DXVECTOR3 _min;
	D3DXVECTOR3 _max;
};
//边界球
struct BoundingSphere
{
	BoundingSphere();
	bool ComputeBoundingSphere(IDirect3DDevice9* d3d,ID3DXMesh* mesh,float scale);
	void CreateMesh(float scale, D3DXVECTOR3 adjustCenter);
	bool CollisionShere(BoundingSphere& other);
	bool RaySphereIntTest(sRay* ray);
	POINT GetPoint();
	void Render();
	void Release();

	D3DXVECTOR3 _center;
	D3DXVECTOR3 _worldPos;
	float       _radius;
	ID3DXMesh* _mesh;
	IDirect3DDevice9* _d3d;
	bool isRender;
	int _ColliID;
	int _ID;
};

bool ComputeBoundingSphere(ID3DXMesh* mesh, BoundingSphere* sphere);
bool ComputeBoundingBox(ID3DXMesh* mesh, BoundingBox* box);
bool RaySphereIntTest(sRay* ray, BoundingSphere* sphere);

struct DrawRay
{
	struct Vertex
	{
		Vertex() {}
		Vertex(
			float x, float y, float z,
			DWORD c)
		{
			_x = x;  _y = y;  _z = z;
			_color = c;
		}
		float _x, _y, _z;
		DWORD _color;

		static const DWORD FVF_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	};

	bool Create(IDirect3DDevice9* d3d);
	void Release();
	bool Draw();
	void Updata(sRay& ray);

private:
	IDirect3DDevice9*       _d3d;
	IDirect3DVertexBuffer9* _vb;
	IDirect3DIndexBuffer9*  _ib;
};

struct DrawTriangle
{
	struct Vertex
	{
		Vertex() {}
		Vertex(
			float x, float y, float z,
			DWORD c)
		{
			_x = x;  _y = y;  _z = z;
			_color = c;
		}
		float _x, _y, _z;
		DWORD _color;

		static const DWORD FVF_VERTEX = D3DFVF_XYZ | D3DFVF_DIFFUSE;
	};

	bool Create(IDirect3DDevice9* d3d);
	void Release();
	bool Draw();
	void Updata(D3DXVECTOR3* vec);

private:
	IDirect3DDevice9*       _d3d;
	IDirect3DVertexBuffer9* _vb;
};

struct sTextMsg
{
	string _str;
	RECT _rect;
	D3DCOLOR _color;

	sTextMsg();
};
class cTextOut
{
private:
	ID3DXFont* pFont_;
	IDirect3DDevice9* pDevice_;
	map<string, sTextMsg*> mText;
public:
	cTextOut();
	~cTextOut();
	void Init(IDirect3DDevice9* d3d);
	void Draw();
	void AddText(string ID,sTextMsg* text);
	void SetText(string ID, string text);
};

struct INTERSECTION
{
	BOOL bPick;
	DWORD dwFace;                 // mesh face that was intersected
	FLOAT fBary1, fBary2;         // barycentric coords of intersection
	FLOAT fDist;                  // distance from ray origin to intersection
	FLOAT tu, tv;                 // texture coords of intersection
	D3DXVECTOR3 v[3];
};