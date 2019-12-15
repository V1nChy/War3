#include "d3dUtility.h"

// vertex formats
const DWORD  Vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

////////////////////////////////////////////////////////////////////////////

D3DLIGHT9  InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_DIRECTIONAL;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Direction = *direction;

	return light;
}

D3DLIGHT9  InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_POINT;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;

	return light;
}

D3DLIGHT9  InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
{
	D3DLIGHT9 light;
	::ZeroMemory(&light, sizeof(light));

	light.Type      = D3DLIGHT_SPOT;
	light.Ambient   = *color * 0.4f;
	light.Diffuse   = *color;
	light.Specular  = *color * 0.6f;
	light.Position  = *position;
	light.Direction = *direction;
	light.Range        = 1000.0f;
	light.Falloff      = 1.0f;
	light.Attenuation0 = 1.0f;
	light.Attenuation1 = 0.0f;
	light.Attenuation2 = 0.0f;
	light.Theta        = 0.5f;
	light.Phi          = 0.7f;

	return light;
}

D3DMATERIAL9  InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s, D3DXCOLOR e, float p)
{
	D3DMATERIAL9 mtrl;
	mtrl.Ambient  = a;
	mtrl.Diffuse  = d;
	mtrl.Specular = s;
	mtrl.Emissive = e;
	mtrl.Power    = p;
	return mtrl;
}

 BoundingBox::BoundingBox()
{
	// infinite small 
	_min.x = FINFINITY;
	_min.y = FINFINITY;
	_min.z = FINFINITY;

	_max.x = -FINFINITY;
	_max.y = -FINFINITY;
	_max.z = -FINFINITY;
}

bool  BoundingBox::isPointInside(D3DXVECTOR3& p)
{
	if( p.x >= _min.x && p.y >= _min.y && p.z >= _min.z &&
		p.x <= _max.x && p.y <= _max.y && p.z <= _max.z )
	{
		return true;
	}
	else
	{
		return false;
	}
}

 BoundingSphere::BoundingSphere()
{
	isRender = true;
	_mesh = NULL;
	_radius = 0.0f;

	_center = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_worldPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	_ColliID = -1;
	_ID = -1;
}
 bool BoundingSphere::ComputeBoundingSphere(IDirect3DDevice9* d3d,ID3DXMesh* mesh, float scale)
 {
	 _d3d = d3d;
	 HRESULT hr = 0;

	 BYTE* v = 0;
	 mesh->LockVertexBuffer(0, (void**)&v);

	 hr = D3DXComputeBoundingSphere(
		 (D3DXVECTOR3*)v,
		 mesh->GetNumVertices(),
		 D3DXGetFVFVertexSize(mesh->GetFVF()),
		 &_center,
		 &_radius);

	 mesh->UnlockVertexBuffer();

	 _radius *= scale;

	 if (FAILED(hr))
		 return false;

	 return true;
 }
 void BoundingSphere::CreateMesh(float scale, D3DXVECTOR3 adjustCenter)
 {
	 _radius *= scale;
	 _center += adjustCenter;
	 //_center= D3DXVECTOR3(0.0f, _radius, 0.0f);
	 D3DXCreateSphere(_d3d, _radius, 10, 10, &_mesh, 0);
 }

 bool BoundingSphere::RaySphereIntTest(sRay* ray)
 {
	 D3DXVECTOR3 v = ray->_origin - (_center + _worldPos);

	 float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
	 float c = D3DXVec3Dot(&v, &v) - (_radius * _radius);

	 // find the discriminant
	 float discriminant = (b * b) - (4.0f * c);

	 // test for imaginary number
	 if (discriminant < 0.0f)
		 return false;

	 discriminant = sqrtf(discriminant);

	 float s0 = (-b + discriminant) / 2.0f;
	 float s1 = (-b - discriminant) / 2.0f;

	 // if a solution is >= 0, then we intersected the sphere
	 if (s0 >= 0.0f || s1 >= 0.0f)
		 return true;

	 return false;
 }
 bool BoundingSphere::CollisionShere(BoundingSphere& other)
 {
	 D3DXVECTOR3 pos1 = _worldPos + _center;
	 D3DXVECTOR3 pos2 = other._worldPos + other._center;
	
	 float dis = sqrtf((pos1.x - pos2.x)*(pos1.x - pos2.x) + (pos1.y - pos2.y)*(pos1.y - pos2.y) + (pos1.z - pos2.z)*(pos1.z - pos2.z));
	 if (dis < _radius + other._radius)
	 {
		 /*if (_ColliID == other._ID)
			 return false;
		 _ColliID = other._ID;*/
		 return true;
	 }
	 else
	 {
		 //_ColliID = -1;
		 return false;
	 }
 }
 POINT BoundingSphere::GetPoint()
 {
	 POINT temp;
	 temp.x = _center.x;
	 temp.y = _center.z;
	 return temp;
 }
 void BoundingSphere::Render()
 {
	 if (!isRender || _mesh==NULL)
		 return;
	 D3DXMATRIX world;
	 D3DXMatrixTranslation(&world, _worldPos.x + _center.x, _worldPos.y + _center.y, _worldPos.z + _center.z);
	 _d3d->SetTransform(D3DTS_WORLD, &world);

	 _d3d->SetTexture(0, NULL);
	 _d3d->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	 _mesh->DrawSubset(0);
	 _d3d->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
 }
 void BoundingSphere::Release()
 {
	 SAFE_RELEASE(_mesh);
 }
float  GetRandomFloat(float lowBound, float highBound)
{
	if( lowBound >= highBound ) // bad input
		return lowBound;

	// get random float in [0, 1] interval
	float f = (rand() % 10000) * 0.0001f; 

	// return float in [lowBound, highBound] interval. 
	return (f * (highBound - lowBound)) + lowBound; 
}

void  GetRandomVector(
	  D3DXVECTOR3* out,
	  D3DXVECTOR3* min,
	  D3DXVECTOR3* max)
{
	out->x = GetRandomFloat(min->x, max->x);
	out->y = GetRandomFloat(min->y, max->y);
	out->z = GetRandomFloat(min->z, max->z);
}

DWORD  FtoDw(float f)
{
	return *((DWORD*)&f);
}

float  Lerp(float a, float b, float t)
{
	return a - (a*t) + (b*t);
}

bool ComputeBoundingSphere(ID3DXMesh* mesh, BoundingSphere* sphere)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingSphere(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&sphere->_center,
		&sphere->_radius);

	mesh->UnlockVertexBuffer();

	if( FAILED(hr) )
		return false;

	return true;
}

bool ComputeBoundingBox(ID3DXMesh* mesh, BoundingBox* box)
{
	HRESULT hr = 0;

	BYTE* v = 0;
	mesh->LockVertexBuffer(0, (void**)&v);

	hr = D3DXComputeBoundingBox(
		(D3DXVECTOR3*)v,
		mesh->GetNumVertices(),
		D3DXGetFVFVertexSize(mesh->GetFVF()),
		&box->_min,
		&box->_max);

	mesh->UnlockVertexBuffer();

	if( FAILED(hr) )
		return false;

	return true;
}

bool RaySphereIntTest(sRay* ray, BoundingSphere* sphere)
{
	D3DXVECTOR3 v = ray->_origin - sphere->_center;

	float b = 2.0f * D3DXVec3Dot(&ray->_direction, &v);
	float c = D3DXVec3Dot(&v, &v) - (sphere->_radius * sphere->_radius);

	// find the discriminant
	float discriminant = (b * b) - (4.0f * c);

	// test for imaginary number
	if (discriminant < 0.0f)
		return false;

	discriminant = sqrtf(discriminant);

	float s0 = (-b + discriminant) / 2.0f;
	float s1 = (-b - discriminant) / 2.0f;

	// if a solution is >= 0, then we intersected the sphere
	if (s0 >= 0.0f || s1 >= 0.0f)
		return true;

	return false;
}

bool DrawRay::Create(IDirect3DDevice9* d3d)
{
	// save a ptr to the device
	_d3d = d3d;

	_d3d->CreateVertexBuffer(
		4 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);


	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	// build box
	// fill in the front face vertex data
	v[0] = Vertex(-1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = Vertex(-1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[2] = Vertex(1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[3] = Vertex(1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));

	_vb->Unlock();

	_d3d->CreateIndexBuffer(
		6 * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&_ib,
		0);

	WORD* i = 0;
	_ib->Lock(0, 0, (void**)&i, 0);

	// fill in the front face index data
	i[0] = 0; i[1] = 1; i[2] = 2;
	i[3] = 0; i[4] = 2; i[5] = 3;

	_ib->Unlock();


	return true;
}
void DrawRay::Release()
{
	SAFE_RELEASE(_vb);
	SAFE_RELEASE(_ib);
}
bool DrawRay::Draw()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	_d3d->SetTransform(D3DTS_WORLD, &world);

	_d3d->SetIndices(_ib);
	_d3d->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_d3d->SetFVF(Vertex::FVF_VERTEX);
	_d3d->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	_d3d->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	_d3d->SetRenderState(D3DRS_LIGHTING, FALSE);

	_d3d->DrawIndexedPrimitive(
		D3DPT_TRIANGLELIST,
		0,
		0,
		4,
		0,
		2);


	//_d3d->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	/*_d3d->DrawIndexedPrimitive(
	D3DPT_TRIANGLELIST,
	0,
	0,
	24,
	0,
	12);*/

	_d3d->SetIndices(0);

	_d3d->SetPixelShader(0);

	_d3d->SetVertexShader(0);

	_d3d->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	_d3d->SetRenderState(D3DRS_LIGHTING, TRUE);

	return true;
}
void DrawRay::Updata(sRay& ray)
{
	D3DXVECTOR3 l;
	l.x = ray._origin.x + ray._direction.x * 1000;
	l.y = ray._origin.y + ray._direction.y * 1000;
	l.z = ray._origin.z + ray._direction.z * 1000;
	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(ray._origin.x, ray._origin.y, ray._origin.z, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = Vertex(ray._origin.x, ray._origin.y + 0.01f, ray._origin.z, D3DCOLOR_XRGB(255, 0, 0));
	v[2] = Vertex(l.x, l.y + 0.01f, l.z, D3DCOLOR_XRGB(255, 0, 0));
	v[3] = Vertex(l.x, l.y, l.z, D3DCOLOR_XRGB(255, 0, 0));


	_vb->Unlock();
}

bool DrawTriangle::Create(IDirect3DDevice9* d3d)
{
	// save a ptr to the device
	_d3d = d3d;

	_d3d->CreateVertexBuffer(
		3 * sizeof(Vertex),
		D3DUSAGE_WRITEONLY,
		Vertex::FVF_VERTEX,
		D3DPOOL_MANAGED,
		&_vb,
		0);


	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	// build box
	// fill in the front face vertex data
	v[0] = Vertex(-1.0f, -1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = Vertex(-1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));
	v[2] = Vertex(1.0f, 1.0f, 0.0f, D3DCOLOR_XRGB(255, 0, 0));

	_vb->Unlock();
	return true;
}
void DrawTriangle::Release()
{
	SAFE_RELEASE(_vb);
}
bool DrawTriangle::Draw()
{
	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);
	_d3d->SetTransform(D3DTS_WORLD, &world);

	_d3d->SetStreamSource(0, _vb, 0, sizeof(Vertex));
	_d3d->SetFVF(Vertex::FVF_VERTEX);
	_d3d->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	//_d3d->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	_d3d->SetRenderState(D3DRS_LIGHTING, FALSE);

	_d3d->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	_d3d->SetIndices(0);

	_d3d->SetPixelShader(0);

	_d3d->SetVertexShader(0);

	_d3d->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	_d3d->SetRenderState(D3DRS_LIGHTING, TRUE);

	return true;
}
void DrawTriangle::Updata(D3DXVECTOR3* vec)
{
	Vertex* v;
	_vb->Lock(0, 0, (void**)&v, 0);

	v[0] = Vertex(vec[0].x, vec[0].y, vec[0].z, D3DCOLOR_XRGB(255, 0, 0));
	v[1] = Vertex(vec[1].x, vec[1].y, vec[1].z, D3DCOLOR_XRGB(255, 0, 0));
	v[2] = Vertex(vec[2].x, vec[2].y, vec[2].z, D3DCOLOR_XRGB(255, 0, 0));

	_vb->Unlock();
}

sTextMsg::sTextMsg()
{
	_str = " ";
	_rect = RECT{ 0,0,ScreenWidth,ScreenWidth };
	_color = D3DCOLOR_XRGB(255,255,255);
}

cTextOut::cTextOut()
{}
cTextOut::~cTextOut()
{
	SAFE_RELEASE(pFont_);
}
void cTextOut::Init(IDirect3DDevice9* d3d)
{
	pDevice_ = d3d;
	//实例化一个D3DXFONT_DESC结构体，描述我们创建的字体
	//ID3DXFont		支持中英文
	D3DXFONT_DESC ft;
	ft.Height = 25;
	ft.Width = 12;
	ft.Weight = 500;
	ft.Italic = false;
	ft.CharSet = DEFAULT_CHARSET;
	ft.Quality = 0;
	ft.PitchAndFamily = 0;
	strcpy(ft.FaceName, "Times New Roman");

	if (FAILED(D3DXCreateFontIndirect(pDevice_, &ft, &pFont_)))
	{
		MessageBox(0, "D3DXCreateFontIndirect() - FAILED", 0, 0);
		PostQuitMessage(0);
	}
}

void cTextOut::Draw()
{
	map<string, sTextMsg*>::iterator it;
	for (it = mText.begin(); it != mText.end(); ++it)
	{
		sTextMsg* temp = (*it).second;
		pFont_->DrawText(
			NULL,
			temp->_str.c_str(),
			-1, // size of string or -1 indicates null terminating string
			&temp->_rect,            // rectangle text is to be formatted to in windows coords
			DT_TOP | DT_LEFT, // draw in the top left corner of the viewport
			temp->_color);      // black text
	}
}
void cTextOut::AddText(string ID,sTextMsg* text)
{
	map<string, sTextMsg* >::iterator it;;
	it = mText.find(ID);//返回的是一个指针
	if (it == mText.end())
	{
		mText[ID] = text;
	}
}
void cTextOut::SetText(string ID, string text)
{
	map<string, sTextMsg* >::iterator it;;
	it = mText.find(ID);//返回的是一个指针
	if (it != mText.end())
	{
		sTextMsg* temp = (*it).second;
		temp->_str = text;
	}
}