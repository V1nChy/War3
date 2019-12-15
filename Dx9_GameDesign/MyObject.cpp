

#include "MyObject.h"
#include "MyUtil.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MyObject* MyObject::c_Root = new MyObject("ROOT", NULL);


MyObject::MyObject(const string& name, MyObject* father, const Vector3& offsetFromParent,
				   float radianX, float radianY, float radianZ, bool isVisible )
				   :m_bDirtyBB(true), m_bDirty(true), m_strName(name), m_bIsVisible(isVisible),
				   m_fScale(1), m_pMesh(NULL), m_pFather(NULL), m_pShadowLight(NULL)
{
	if (!father)
	{
		m_mTransformation = Matrix4::IDENTITY;
		m_vPositionRelative = Vector3::ZERO;
		m_mRotateRelative = Matrix3::IDENTITY;
		return;
	}

	m_mScale = Matrix4::IDENTITY;

	Matrix4::GetRotaionMatrixXYZ(radianX, radianY, radianZ).Extract3x3Matrix(m_mRotateRelative);
	m_vPositionRelative = offsetFromParent;

	father->UpdateTransformation();
	m_mFinalTransformMatrix = m_mTransformation = GetTransformMatrixRelative() * father->m_mTransformation;

	Attach(father);

	m_SelfBoundingSphere.SetRadius(0);
}

MyObject::~MyObject()
{
	FinalCleanup();
}

//-----------------------------------------------------------------------------
// Name: UpdateSubSceneGraph()
// Desc: ���½ڵ�����ձ任����
//-----------------------------------------------------------------------------
void MyObject::UpdateSubSceneGraph(bool compulsive)
{
	// �ȸ����Լ�
	bool dirty = m_bDirty || compulsive;
	if (dirty)
	{
		m_mTransformation = GetTransformMatrixRelative();
		if (m_pFather)
			m_mTransformation *= m_pFather->m_mTransformation;

		m_mFinalTransformMatrix = m_mScale * m_mTransformation;

		if (m_pMesh)
			//m_OBB.UpdateOBB();

		m_bDirty = false;
		m_bDirtyBB = true;
	}

	// ���¶��� ���Լ����࣬�����ܶ����ࣩ
	for (list<MyObject*>::iterator ptr = m_pChildList.begin(); ptr != m_pChildList.end(); ++ptr)
	{
		(*ptr)->UpdateSubSceneGraph(dirty);
	}
}

//-----------------------------------------------------------------------------
// Name: GetTransformMatrixRelative()
// Desc: ��������븸�����λ�þ���
//-----------------------------------------------------------------------------
const Matrix4& MyObject::GetTransformMatrixRelative() const
{
	static Matrix4 m;

	m = m_mRotateRelative;
	m.SetOffset(m_vPositionRelative);
	m.SetScale(1);
	m.SetProj(Vector3::ZERO);

	return m;
}

//-----------------------------------------------------------------------------
// Name: UpdateTransformation()
// Desc: ��������븸�����λ�þ���λ����ת�����������ձ任����
//-----------------------------------------------------------------------------
const Matrix4& MyObject::UpdateTransformation()
{
	m_mTransformation = GetTransformMatrixRelative();

	//���������󣬷������վ�����������ϵ�µ�λ�þ���
	MyObject* pNode = m_pFather;
	while (pNode)
	{
		m_mTransformation = m_mTransformation * pNode->GetTransformMatrixRelative();
		pNode = pNode->m_pFather;
	}

	m_mFinalTransformMatrix = m_mScale * m_mTransformation;

	if (m_pMesh)
		//m_OBB.UpdateOBB();

	m_bDirty = false;

	return m_mTransformation;
}

//-----------------------------------------------------------------------------
// Name: SetDirectionRelative()
// Desc: ������Ը��ڵ�ķ���
//-----------------------------------------------------------------------------
void MyObject::SetDirectionRelative( const Vector3& vDir, const Vector3& vUp)
{
	Matrix4::LookTowardsUp(vDir, vUp).Extract3x3Matrix(m_mRotateRelative);
	m_bDirty = true;
	m_bDirtyBB = true;
}

//-----------------------------------------------------------------------------
// Name: Attach()
// Desc: ���ص�ĳ���ڵ���
//-----------------------------------------------------------------------------
bool MyObject::Attach(MyObject *father)
{
	if (father == m_pFather || !father)
		return false;

	// ��ȷ��attach, ��һ������
	// �ȸ���ԭ����㣬detach�����
	if (m_pFather)
		m_pFather->Detach(m_strName);

	// Ȼ�����ø����
	m_pFather = father;
	// ���¸��ı任����
	m_pFather->UpdateTransformation();
	// ���¶��ӵ���Ա任��λ�ơ���ת��
	Matrix4 mChildRelative = m_mTransformation * m_pFather->m_mTransformation.Inverse();
	mChildRelative.Extract3x3Matrix(m_mRotateRelative);
	m_vPositionRelative = mChildRelative.GetOffset();

	// �����¸����϶��� :) �����°�Χ��
	m_pFather->m_pChildList.push_back(this);
	if (!m_BoundingSphere.IfInSphere(m_pFather->m_BoundingSphere))
		m_pFather->m_bDirtyBB = true;

	return true;
}

//-----------------------------------------------------------------------------
// Name: Detach()
// Desc: ж��ĳ���ӽڵ�name ��δɾ����
//-----------------------------------------------------------------------------
bool MyObject::Detach(const string& name)
{
	list<MyObject *>::iterator p;
	for ( p = m_pChildList.begin(); p != m_pChildList.end(); ++p )
	{
		// ���߶��ӣ��Լ���Ҫ���ˣ�Ȼ�󣬶���Ҳ��û�и�����
		if ( (*p)->GetName() == name )
		{
			(*p)->UpdateTransformation();	// ��������ı任����

			(*p)->m_mRotateRelative = (*p)->GetRotationMatrix();
			(*p)->m_vPositionRelative = (*p)->GetPosition();
			(*p)->m_pFather = NULL;	

			m_pChildList.erase(p);

			m_bDirtyBB = true;

			return true;
		}
	}
	string err = "Detach error.Child name :";
	err += name;
	LOGINFO(err.c_str());

	return false;
}

//-----------------------------------------------------------------------------
// Name: DetachAll()
// Desc: ж��ȫ���ӽڵ�name ��ɾ����
//-----------------------------------------------------------------------------
void MyObject::DetachAll()
{
	m_bDirtyBB = true;

	list<MyObject *>::iterator p;
	for ( p = m_pChildList.begin(); p != m_pChildList.end(); ++p )
	{
		SAFE_DELETE(*p);
	}

	m_pChildList.clear();
}

//-----------------------------------------------------------------------------
// Name: AddAttach()
// Desc: �����ӽڵ�
//-----------------------------------------------------------------------------
bool MyObject::AddAttach(MyObject * child)
{
	if (child == NULL)
		return false;
	else
		return child->Attach(this);
}

bool MyObject::SetMesh(MyMesh *mesh)
{
	assert(mesh);
	m_pMesh = mesh;

	return true;
}

//-----------------------------------------------------------------------------
// Name: UpdateBoundingSphere()
// Desc: ���¼���˽�㼰���ӽ��İ�Χ��
//-----------------------------------------------------------------------------
void MyObject::UpdateBoundingSphere()
{
	// ��������ӽڵ�İ�Χ�������µģ�ȷʵҲ�ǣ���Ϊ�Ǹ�����ǰ��������
	if (m_bDirtyBB == false)
		return;

	bool ifSelfBBChanged = false;	// ����Χ���Ƿ�������޸�

	// �����Χ��ĸ���
	UpdateSelfBoundingSphere();
	Sphere boundingSphere = m_SelfBoundingSphere;

	// ������ӽڵ�İ�Χ��ı䣬���Ǹý�㱾�����ⲿ���Ķ�����ý���Χ����Ҫ���е���
	for (list<MyObject*>::iterator p = m_pChildList.begin(); p != m_pChildList.end(); ++p)
	{
		// ��������, ��������°�Χ��
		if ( boundingSphere.IfOutOfSphere( (*p)->m_BoundingSphere ) )
			continue;

		// ��������, �����İ�Χ����³��Ӱ�Χ��
		if ( boundingSphere.IfInSphere((*p)->m_BoundingSphere) )
		{	
			boundingSphere.SetCenter( (*p)->m_BoundingSphere.GetCenter() );
			boundingSphere.SetRadius( (*p)->m_BoundingSphere.GetRadius() );	
		}

		// �����ཻ��������
		// �õ���Բ�ĵ������;���
		Vector3 v = (*p)->m_BoundingSphere.GetCenter() - boundingSphere.GetCenter(); 
		float d = v.Length();

		boundingSphere.SetCenter( boundingSphere.GetCenter() + v.GetNormalized() * 
			( d-boundingSphere.GetRadius()+(*p)->m_BoundingSphere.GetRadius() ) * 0.5f );
		boundingSphere.SetRadius( ( d + boundingSphere.GetRadius() + (*p)->m_BoundingSphere.GetRadius() ) * 0.5f );
	}


	// ����µõ��İ�Χ���ԭ��Χ��ͬ
	if ( !( boundingSphere == m_BoundingSphere ) )
	{
		ifSelfBBChanged = true;

		m_BoundingSphere = boundingSphere;
		if(m_pFather)
			m_pFather->m_bDirtyBB = true;
	}

	// ������ӽ��㶨
	// ֻ�����ʼ��ʱ��, �������š��ƶ���ʱ�����Ҫ���øı�
	m_bDirtyBB = false;
}

HRESULT MyObject::Render(const MyCamera& camera)
{
	if (IsVisible() == false)
		return S_FALSE;

	if (m_pMesh)
	{
		//if (m_SelfBoundingSphere.Intersects(camera.GetFrustum()))
		//{
		//	if (m_Material.GetShadowType() == ST_SHADOWED)
		//	{
		//		// Peter: һ��ֻ��һ����ͶӰ��, ������ȱ����׳��, 
		//		// ���ж�������ʱ��ͻ����,��ʱ��Ҫ�����е�ע��ȥ��, ͬʱ��RenderInStencil��setFromMaterialȥ��
		//		//	GERenderQueue::GetSingleton().PushToNormalRQ(this);
		//		GERenderQueue::GetSingleton().PushToShadowDestRQ(this);
		//	}
		//	else if (m_Material.GetShadowType() == ST_PLANAR)
		//	{
		//		GERenderQueue::GetSingleton().PushToShadowSrcRQ(this);
		//		GERenderQueue::GetSingleton().PushToNormalRQ(this);
		//	}
		//	else if (m_Material.GetTransparent())
		//		GERenderQueue::GetSingleton().PushToTransRQ(this);
		//	else 
		//		GERenderQueue::GetSingleton().PushToNormalRQ(this);
		//}
	}

	return S_OK;
}

HRESULT MyObject::Render()
{
//#ifdef _DEBUG
//	m_OBB.Render();
//	_RenderBoundingSphere();
//#endif
//
//	m_Material.SetFromMaterial();
//
//	GEGraphicLayer::GetSingleton()->SetWorldMatrix(GetFinalTransformMatrix());
//
//	return m_pMesh->Render();
	return S_OK;
}

HRESULT MyObject::RenderInStencil()
{
	//if (m_Material.GetShadowType() == ST_SHADOWED)
	//{
	//	m_Material.SetFromMaterial();	// Peter : �ڶ��ͶӰ������ڵ�ʱ��, ����Ҫȥ��
	//	GEGraphicLayer::GetSingleton()->SetWorldMatrix(GetFinalTransformMatrix());
	//	return m_pMesh->Render();
	//}
	//else if (m_Material.GetShadowType() == ST_PLANAR)
	//{
	//	Vector3 vl = m_pShadowLight->GetPosition();	// Ŀǰ֧�ֵ��, ��ʵƽ�й�Ҳ���
	//	Vector3 vn = m_vPlanarShadowDestNormal;		// ƽ�淨��

	//	float d = 0;
	//	Matrix4 mat;	// ƽ��ͶӰ����
	//	mat[0][0] = vn.y * vl.y + vn.z * vl.z + d;
	//	mat[1][0] = -vn.y * vl.x;
	//	mat[2][0] = -vn.z * vl.x;
	//	mat[3][0] = -d * vl.x;
	//	mat[0][1] = -vn.x * vl.y;
	//	mat[1][1] = vn.x * vl.x + vn.z * vl.z + d;
	//	mat[2][1] = -vn.z * vl.y;
	//	mat[3][1] = -d * vl.y;
	//	mat[0][2] = -vn.x * vl.z;
	//	mat[1][2] = -vn.y * vl.z;
	//	mat[2][2] = vn.x * vl.x + vn.y * vl.y + d;
	//	mat[3][2] = -d * vl.z;
	//	mat[0][3] = -vn.x;
	//	mat[1][3] = -vn.y;
	//	mat[2][3] = -vn.z;
	//	mat[3][3] = vn.DotProduction(vl);

	//	GEGraphicLayer::GetSingleton()->SetWorldMatrix( GetFinalTransformMatrix() * mat );
	//	return m_pMesh->Render();
	//}

	return E_FAIL;
}



//-----------------------------------------------------------------------------
// Name: CheckHasChild()
// Desc: ����Ƿ�������ӽ��
//-----------------------------------------------------------------------------
MyObject * MyObject::CheckHasChild(const string& childName) const
{
	list<MyObject*>::const_iterator p;
	for ( p = m_pChildList.begin(); p != m_pChildList.end(); ++p )
	{
		if ( (*p)->GetName() == childName )
			return (*p);			
	}

	MyObject * r = NULL;
	for ( p = m_pChildList.begin(); p != m_pChildList.end(); ++p )
	{
		r = (*p)->CheckHasChild( childName );	
		if ( r != NULL )
			return r;
	}

	return NULL;
}


HRESULT MyObject::OneTimeSceneInit()
{ 
	return S_OK;
}

void MyObject::FinalCleanup()				
{ 
	if (m_pMesh) 
	{
		SAFE_DELETE(m_pMesh);
	}

	for (list<MyObject*>::iterator p = m_pChildList.begin(); p != m_pChildList.end(); ++p)
	{
		delete (*p);
	}
}

HRESULT MyObject::InitDeviceObjects()			
{
	/*if(m_pMesh) 
	{
		m_pMesh->InitDeviceObjects();

		m_OBB.Init(this, m_pMesh->GetRightDist(), m_pMesh->GetLeftDist(), m_pMesh->GetTopDist(), m_pMesh->GetBottomDist(),
			m_pMesh->GetFrontDist(), m_pMesh->GetBackDist());
		m_OBB.UpdateOBB();

		UpdateSelfBoundingSphere();

		m_BoundingSphere = m_SelfBoundingSphere;
		m_bDirtyBB = false;
	}*/

	return S_OK;
}

HRESULT MyObject::DeleteDeviceObjects()
{
	/*if(m_pMesh) 
	{
		m_pMesh->DeleteDeviceObjects();
		m_pMesh = NULL;
	}*/
	return S_OK;
}

HRESULT MyObject::InvalidateDeviceObjects()
{ 
	/*if (m_pMesh) 
		m_pMesh->InvalidateDeviceObjects(); */
	return S_OK;
}
HRESULT MyObject::RestoreDeviceObjects()
{ 
	/*if (m_pMesh) 
		m_pMesh->RestoreDeviceObjects();*/
	return S_OK;
}


void MyObject::UpdateSelfBoundingSphere()
{
	if (m_pMesh){
		//m_SelfBoundingSphere.SetCenter(m_OBB.GetCenter());
		//m_SelfBoundingSphere.SetRadius(m_OBB.GetRadius());
	}
	else{
		m_SelfBoundingSphere.SetCenter(GetPosition());
		m_SelfBoundingSphere.SetRadius(0);
	}
}


void MyObject::_RenderBoundingSphere()
{
	// this func is for test of bounding sphere
	//static GEMaterial material;
	//static bool inited = false; 
	//static GE_NLNTVERTEX vertex[13];
	//if (!inited)
	//{
	//	inited = true;
	//	material.SetLighting(false);
	//}

	//static const float sqrt3 = sqrtf(3.f)*0.5f;
	//const float r = m_BoundingSphere.GetRadius();
	//vertex[0].Init(Vector3(-0.5f * r, sqrt3 * r, 0), GEColor::Red);
	//vertex[1].Init(Vector3::Y_AXIS * r, GEColor::Red);
	//vertex[2].Init(Vector3( 0.5f * r, sqrt3 * r, 0), GEColor::Red);
	//vertex[3].Init(Vector3( sqrt3 * r, 0.5f * r, 0), GEColor::Red);
	//vertex[4].Init(Vector3::X_AXIS * r, GEColor::Red);
	//vertex[5].Init(Vector3(sqrt3 * r, -0.5f * r, 0), GEColor::Red);
	//vertex[6].Init(Vector3( 0.5f * r, -sqrt3 * r, 0), GEColor::Red);
	//vertex[7].Init(Vector3::NEG_Y_AXIS * r, GEColor::Red);
	//vertex[8].Init(Vector3(-0.5f * r, -sqrt3 * r, 0), GEColor::Red);
	//vertex[9].Init(Vector3(-sqrt3 * r, -0.5f * r, 0), GEColor::Red);
	//vertex[10].Init(Vector3::NEG_X_AXIS * r, GEColor::Red);
	//vertex[11].Init(Vector3(-sqrt3 * r, 0.5f * r, 0), GEColor::Red);
	//vertex[12] = vertex[0];


	//// billboard
	//GECamera* camera = (GECamera*)(GESceneManager::GetSingleton()->GetNode("CAMERA"));
	//if (camera == NULL)
	//	return;
	//Vector3 normalPlan = (m_BoundingSphere.GetCenter() - camera->GetPosition());
	//Matrix4 m = Matrix4::LookTowards(normalPlan);
	//m.SetOffset(m_BoundingSphere.GetCenter());
	//GEGraphicLayer::GetSingleton()->SetWorldMatrix(m);

	//material.SetFromMaterial();
	//GEGraphicLayer::GetSingleton()->SetFVF(GE_NLNTVERTEX::FVF);

	//GEGraphicLayer::GetSingleton()->DrawPrimitiveUp(D3DPT_LINESTRIP, 12, vertex, sizeof(GE_NLNTVERTEX));
}

void MyObject::InitPlanarShadowSrc(const MyLight* const light, const Vector3& vNormal)
{
	/*m_pShadowLight = light;
	m_vPlanarShadowDestNormal = vNormal;
	m_Material.SetShadow(ST_PLANAR);*/
}

void MyObject::InvalidateShadow()
{
	/*m_Material.SetShadow(ST_DISABLE);*/
}

void MyObject::InitPlanarShadowDest()
{
	/*m_Material.SetShadow(ST_SHADOWED);*/
}

const Matrix3& MyObject::GetRotationMatrix() const
{
	static Matrix3 mRot;
	mRot.SetRow(0, GetRight());
	mRot.SetRow(1, GetUp());
	mRot.SetRow(2, GetDir());
	return mRot;
}

void MyObject::RotationPitch(float radian)
{
	if (radian == 0)
		return;

	m_mRotateRelative.RotationX(radian);

	m_bDirty = true;
	m_bDirtyBB = true;

	return;
}

void MyObject::RotationYaw(float radian)
{
	if (radian == 0)
		return;

	m_mRotateRelative.RotationY(radian);

	m_bDirty = true;
	m_bDirtyBB = true;

	return;
}

void MyObject::RotationRoll(float radian)
{
	if (radian == 0)
		return;

	m_mRotateRelative.RotationZ(radian);

	m_bDirty = true;
	m_bDirtyBB = true;

	return;
}

void MyObject::RotationPitchYawRoll(float radianX, float radianY, float radianZ)
{
	if ((radianX == radianY) && (radianY == radianZ) && radianX == 0)
		return;

	m_mRotateRelative.RotationXYZ(radianX, radianY, radianZ);

	m_bDirty = true;
	m_bDirtyBB = true;

	return;
}


void MyObject::Move(const Vector3& offset)
{
	if (offset.IsZero())
		return;

	static Matrix3 _mrv;
	_mrv = Matrix3::IDENTITY;
	if (m_pFather)
		m_pFather->m_mTransformation.Extract3x3Matrix(_mrv);
	_mrv = _mrv.Transpose();

	m_vPositionRelative += offset * _mrv;
	m_bDirty = true;	// �ڵ�����ձ任������Ҫ����

	m_bDirtyBB = true;	// �ڵ�İ�Χ����Ҫ����
}

void MyObject::MoveTo(const Vector3& newPos)
{
	Move(newPos - GetPosition());
}

void MyObject::Scale(float scale)
{
	m_fScale *= scale;
	m_mScale = Matrix4::GetScale(m_fScale, m_fScale, m_fScale);

	m_bDirty = true;
	m_bDirtyBB = true;

	for (list<MyObject*>::iterator ptr = m_pChildList.begin(); ptr != m_pChildList.end(); ++ptr)
	{
		(*ptr)->m_vPositionRelative *= scale;	// ���Ӻ͸��׵���Ծ��뷢���ı�
		(*ptr)->Scale(scale);
	}
}