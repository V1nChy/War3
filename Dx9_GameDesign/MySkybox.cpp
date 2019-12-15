// GESkybox.cpp: implementation of the GESkybox class.
//
//////////////////////////////////////////////////////////////////////

#include "MySkybox.h"
#include "MyUTIL.h"
#include "MyRenderQueue.h"
#include "DxGraphicLayer.h"
#pragma warning(disable:4267)
#pragma warning(disable:4244)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MySkybox::MySkybox(const string& name, const string& cubeTexFilePath, size_t size, bool visible)
: MyObject(name, MyObject::c_Root, Vector3::ZERO, 0, 0, 0, visible)
{
	m_Material.SetDepthCheck(false);
	m_Material.SetDepthWrite(false);
	m_Material.AddTextureLayer();
	m_Material.AddTexture(cubeTexFilePath);

	SetSize(size);

	m_pCamera =	NULL;
}

MySkybox::~MySkybox()
{

}



void MySkybox::SetSize(int size)
{
	m_uSize = size;

	// +x
	m_Vertex[0].Init(Vector3(m_uSize, m_uSize, m_uSize),	0.f, 0.f);
	m_Vertex[1].Init(Vector3(m_uSize, -m_uSize, -m_uSize),	1.f/6, 1.f);
	m_Vertex[2].Init(Vector3(m_uSize, -m_uSize, m_uSize),	0.f, 1.f);
	m_Vertex[3] = m_Vertex[0];
	m_Vertex[4].Init(Vector3(m_uSize, m_uSize, -m_uSize),	1.f/6, 0.f);
	m_Vertex[5] = m_Vertex[1];

	// -x
	m_Vertex[6].Init(Vector3(-m_uSize, m_uSize, -m_uSize),	1.f/6, 0.f);
	m_Vertex[7].Init(Vector3(-m_uSize, -m_uSize, m_uSize),	2.f/6, 1.f);
	m_Vertex[8].Init(Vector3(-m_uSize, -m_uSize, -m_uSize),	1.f/6, 1.f);
	m_Vertex[9] = m_Vertex[6];
	m_Vertex[10].Init(Vector3(-m_uSize, m_uSize, m_uSize),	2.f/6, 0.f);
	m_Vertex[11] = m_Vertex[7];

	// +y
	m_Vertex[12].Init(Vector3(-m_uSize, m_uSize, -m_uSize),	2.f/6, 0.f);
	m_Vertex[13].Init(Vector3(m_uSize, m_uSize, m_uSize),	3.f/6, 1.f);
	m_Vertex[14].Init(Vector3(-m_uSize, m_uSize, m_uSize),	2.f/6, 1.f);
	m_Vertex[15] = m_Vertex[12];
	m_Vertex[16].Init(Vector3(m_uSize, m_uSize, -m_uSize),	3.f/6, 0.f);
	m_Vertex[17] = m_Vertex[13];
	
	// -y
	m_Vertex[18].Init(Vector3(-m_uSize, -m_uSize, m_uSize),	3.f/6, 0.f);
	m_Vertex[19].Init(Vector3(m_uSize, -m_uSize, -m_uSize),	4.f/6, 1.f);
	m_Vertex[20].Init(Vector3(-m_uSize, -m_uSize, -m_uSize),3.f/6, 1.f);
	m_Vertex[21] = m_Vertex[18];
	m_Vertex[22].Init(Vector3(m_uSize, -m_uSize, m_uSize),	4.f/6, 0.f);
	m_Vertex[23] = m_Vertex[19];

	// +z
	m_Vertex[24].Init(Vector3(-m_uSize, m_uSize, m_uSize),	4.f/6, 0.f);
	m_Vertex[25].Init(Vector3(m_uSize, -m_uSize, m_uSize),	5.f/6, 1.f);
	m_Vertex[26].Init(Vector3(-m_uSize, -m_uSize, m_uSize),	4.f/6, 1.f);
	m_Vertex[27] = m_Vertex[24];
	m_Vertex[28].Init(Vector3(m_uSize, m_uSize, m_uSize),	5.f/6, 0.f);
	m_Vertex[29] = m_Vertex[25];

	// -z
	m_Vertex[30].Init(Vector3(m_uSize, m_uSize, -m_uSize),	5.f/6, 0.f);
	m_Vertex[31].Init(Vector3(-m_uSize, -m_uSize, -m_uSize),1.f, 1.f);
	m_Vertex[32].Init(Vector3(m_uSize, -m_uSize, -m_uSize),	5.f/6, 1.f);
	m_Vertex[33] = m_Vertex[30];
	m_Vertex[34].Init(Vector3(-m_uSize, m_uSize, -m_uSize),	1.f, 0.f);
	m_Vertex[35] = m_Vertex[31];
}


bool MySkybox::SetCamera(MyCamera* camera, int height)
{
	if (!camera)
		return false;

	m_pCamera = camera;
	m_uOffsetFromGround = height;
	return true;
}

HRESULT MySkybox::Render(const MyCamera& camera)
{
	if (IsVisible() == false)
		return E_FAIL;
	else
		MyRenderQueue::GetSingleton()->SetSkybox(this);

	return S_OK;
}

HRESULT MySkybox::Render()
{
	m_Material.SetFromMaterial();

	Vector3 p = m_pCamera->GetPosition();
	p += (m_uOffsetFromGround - m_uSize)*Vector3::Y_AXIS;
	MoveTo(p);

	DxGraphicLayer::GetSingleton()->SetFVF(My_NLVERTEX::FVF);
	DxGraphicLayer::GetSingleton()->SetWorldMatrix(GetTransformMatrix());
	return DxGraphicLayer::GetSingleton()->DrawPrimitiveUp( D3DPT_TRIANGLELIST, 12, m_Vertex, sizeof(My_NLVERTEX));
}

