// OrientBoundingBox.cpp: implementation of the OrientBoundingBox class.
//
//////////////////////////////////////////////////////////////////////

#include "OrientBoundingBox.h"
#include "MyUtil.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

OrientBoundingBox::OrientBoundingBox()
					:m_Object(NULL), m_fRight(0), m_fLeft(0), m_fFront(0), m_fBack(0), m_fUp(0), m_fDown(0),
					m_fRadiusOriginal(0), m_fRadius(0)
{}


OrientBoundingBox::~OrientBoundingBox()
{};

void OrientBoundingBox::Init(GEObject * object, float fRight, float fLeft, 
							 float fUp, float fDown, float fFront, float fBack)
{
	m_Object = object;

	m_fRight = fRight;
	m_fLeft = fLeft;
	m_fFront = fFront;
	m_fBack = fBack;
	m_fUp = fUp;
	m_fDown = fDown;

	m_fRadiusOriginal = 0.5f * sqrtf( (m_fRight - m_fLeft) * (m_fRight - m_fLeft) + 
		(m_fFront - m_fBack) * (m_fFront - m_fBack) + (m_fUp - m_fDown) * (m_fUp - m_fDown) );

	m_fRadius = m_fRadiusOriginal * object->GetScale();
}


void OrientBoundingBox::UpdateOBB()
{
	if (!m_Object)
		return;

	Vector3 p;
	// Left clipping plane
	p = Vector3(m_fLeft, 0, 0) * m_Object->GetFinalTransformMatrix();
	m_OBB.m_Plane[PLANE_LEFT].Init( -m_Object->GetRight(), p );

	// Right clipping plane
	p = Vector3(m_fRight, 0, 0) * m_Object->GetFinalTransformMatrix();
	m_OBB.m_Plane[PLANE_RIGHT].Init( m_Object->GetRight(), p );

	// Top clipping plane
	p = Vector3(0, m_fUp, 0) * m_Object->GetFinalTransformMatrix();
	m_OBB.m_Plane[PLANE_TOP].Init( m_Object->GetUp(), p );

	// Bottom clipping plane
	p = Vector3(0, m_fDown, 0) * m_Object->GetFinalTransformMatrix();
	m_OBB.m_Plane[PLANE_BOTTOM].Init( -m_Object->GetUp(), p );

	// Far clipping plane
	p = Vector3(0, 0, m_fFront) * m_Object->GetFinalTransformMatrix();
	m_OBB.m_Plane[PLANE_FRONT].Init( m_Object->GetDir(), p );

	// Near clipping plane
	p = Vector3(0, 0, m_fBack) * m_Object->GetFinalTransformMatrix();
	m_OBB.m_Plane[PLANE_BACK].Init( -m_Object->GetDir(), p );	

	m_fRadius = m_fRadiusOriginal * m_Object->GetScale();

	m_vCenter = Vector3((m_fLeft + m_fRight) * 0.5f, (m_fUp + m_fDown) * 0.5f, (m_fFront + m_fBack) * 0.5f)
		* m_Object->GetFinalTransformMatrix();
}


void OrientBoundingBox::Render()
{
	static GEMaterial material;
	static bool inited = false; 
	static GE_NLNTVERTEX vertex[24];
	if (!inited)
	{
		inited = true;
		material.SetLighting(false);
	}

	vertex[0].Init(Vector3(m_fLeft, m_fUp, m_fBack));		vertex[8]  = vertex[7]  = vertex[0];
	vertex[1].Init(Vector3(m_fLeft, m_fUp, m_fFront));		vertex[10] = vertex[2]  = vertex[1];
	vertex[3].Init(Vector3(m_fRight, m_fUp, m_fFront));		vertex[12] = vertex[4]  = vertex[3];
	vertex[5].Init(Vector3(m_fRight, m_fUp, m_fBack));		vertex[14] = vertex[6]  = vertex[5];
	vertex[23].Init(Vector3(m_fLeft, m_fDown, m_fBack));	vertex[9]  = vertex[16] = vertex[23];
	vertex[18].Init(Vector3(m_fLeft, m_fDown, m_fFront));	vertex[11] = vertex[17] = vertex[18];
	vertex[20].Init(Vector3(m_fRight, m_fDown, m_fFront));	vertex[13] = vertex[19] = vertex[20];
	vertex[22].Init(Vector3(m_fRight, m_fDown, m_fBack));	vertex[15] = vertex[21] = vertex[22];

	material.SetFromMaterial();
	GEGraphicLayer::GetSingleton()->SetFVF(GE_NLNTVERTEX::FVF);
	GEGraphicLayer::GetSingleton()->SetWorldMatrix(m_Object->GetFinalTransformMatrix());
	GEGraphicLayer::GetSingleton()->DrawPrimitiveUp(D3DPT_LINELIST, 12, vertex, sizeof(GE_NLNTVERTEX));
}

float OrientBoundingBox::GetWidth () const
{
	return fabsf(m_fRight - m_fLeft) * m_Object->GetScale();
}
float OrientBoundingBox::GetHeight() const
{
	return fabsf(m_fUp - m_fDown) * m_Object->GetScale();
}
float OrientBoundingBox::GetDepth () const
{
	return fabsf(m_fFront - m_fBack) * m_Object->GetScale();
}
