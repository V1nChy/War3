

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
// Desc: 更新节点的最终变换矩阵
//-----------------------------------------------------------------------------
void MyObject::UpdateSubSceneGraph(bool compulsive)
{
	// 先更新自己
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

	// 更新儿子 （自己不脏，但可能儿子脏）
	for (list<MyObject*>::iterator ptr = m_pChildList.begin(); ptr != m_pChildList.end(); ++ptr)
	{
		(*ptr)->UpdateSubSceneGraph(dirty);
	}
}

//-----------------------------------------------------------------------------
// Name: GetTransformMatrixRelative()
// Desc: 返回相对与父对象的位置矩阵
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
// Desc: 返回相对与父对象的位置矩阵（位置旋转）。更新最终变换矩阵
//-----------------------------------------------------------------------------
const Matrix4& MyObject::UpdateTransformation()
{
	m_mTransformation = GetTransformMatrixRelative();

	//遍历父对象，返回最终矩阵（世界坐标系下的位置矩阵）
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
// Desc: 设置相对父节点的方向
//-----------------------------------------------------------------------------
void MyObject::SetDirectionRelative( const Vector3& vDir, const Vector3& vUp)
{
	Matrix4::LookTowardsUp(vDir, vUp).Extract3x3Matrix(m_mRotateRelative);
	m_bDirty = true;
	m_bDirtyBB = true;
}

//-----------------------------------------------------------------------------
// Name: Attach()
// Desc: 挂载到某个节点上
//-----------------------------------------------------------------------------
bool MyObject::Attach(MyObject *father)
{
	if (father == m_pFather || !father)
		return false;

	// 正确的attach, 认一个父亲
	// 先告诉原父结点，detach本结点
	if (m_pFather)
		m_pFather->Detach(m_strName);

	// 然后设置父结点
	m_pFather = father;
	// 更新父的变换矩阵
	m_pFather->UpdateTransformation();
	// 更新儿子的相对变换（位移、旋转）
	Matrix4 mChildRelative = m_mTransformation * m_pFather->m_mTransformation.Inverse();
	mChildRelative.Extract3x3Matrix(m_mRotateRelative);
	m_vPositionRelative = mChildRelative.GetOffset();

	// 让其新父亲认儿子 :) 并更新包围球
	m_pFather->m_pChildList.push_back(this);
	if (!m_BoundingSphere.IfInSphere(m_pFather->m_BoundingSphere))
		m_pFather->m_bDirtyBB = true;

	return true;
}

//-----------------------------------------------------------------------------
// Name: Detach()
// Desc: 卸载某个子节点name （未删除）
//-----------------------------------------------------------------------------
bool MyObject::Detach(const string& name)
{
	list<MyObject *>::iterator p;
	for ( p = m_pChildList.begin(); p != m_pChildList.end(); ++p )
	{
		// 告诉儿子，自己不要他了，然后，儿子也就没有父亲了
		if ( (*p)->GetName() == name )
		{
			(*p)->UpdateTransformation();	// 更新自身的变换矩阵

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
// Desc: 卸载全部子节点name （删除）
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
// Desc: 增加子节点
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
// Desc: 重新计算此结点及其子结点的包围球
//-----------------------------------------------------------------------------
void MyObject::UpdateBoundingSphere()
{
	// 这里假设子节点的包围球都是最新的（确实也是，因为是个树的前续遍历）
	if (m_bDirtyBB == false)
		return;

	bool ifSelfBBChanged = false;	// 结点包围球是否进行了修改

	// 自身包围球的更新
	UpdateSelfBoundingSphere();
	Sphere boundingSphere = m_SelfBoundingSphere;

	// 如果有子节点的包围球改变，或是该结点本身在外部被改动，则该结点包围球需要进行调整
	for (list<MyObject*>::iterator p = m_pChildList.begin(); p != m_pChildList.end(); ++p)
	{
		// 父在子外, 则无需更新包围球
		if ( boundingSphere.IfOutOfSphere( (*p)->m_BoundingSphere ) )
			continue;

		// 父在子内, 将父的包围球更新成子包围球
		if ( boundingSphere.IfInSphere((*p)->m_BoundingSphere) )
		{	
			boundingSphere.SetCenter( (*p)->m_BoundingSphere.GetCenter() );
			boundingSphere.SetRadius( (*p)->m_BoundingSphere.GetRadius() );	
		}

		// 父子相交或是相离
		// 得到两圆心的向量和距离
		Vector3 v = (*p)->m_BoundingSphere.GetCenter() - boundingSphere.GetCenter(); 
		float d = v.Length();

		boundingSphere.SetCenter( boundingSphere.GetCenter() + v.GetNormalized() * 
			( d-boundingSphere.GetRadius()+(*p)->m_BoundingSphere.GetRadius() ) * 0.5f );
		boundingSphere.SetRadius( ( d + boundingSphere.GetRadius() + (*p)->m_BoundingSphere.GetRadius() ) * 0.5f );
	}


	// 如果新得到的包围球和原包围球不同
	if ( !( boundingSphere == m_BoundingSphere ) )
	{
		ifSelfBBChanged = true;

		m_BoundingSphere = boundingSphere;
		if(m_pFather)
			m_pFather->m_bDirtyBB = true;
	}

	// 自身和子结点搞定
	// 只有在最开始的时候, 或是缩放、移动的时候才需要设置改变
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
		//		// Peter: 一般只有一个被投影面, 但这里缺乏健壮性, 
		//		// 当有多个表面的时候就会出错,那时需要将下行的注释去掉, 同时将RenderInStencil的setFromMaterial去掉
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
	//	m_Material.SetFromMaterial();	// Peter : 在多个投影表面存在的时候, 这样要去掉
	//	GEGraphicLayer::GetSingleton()->SetWorldMatrix(GetFinalTransformMatrix());
	//	return m_pMesh->Render();
	//}
	//else if (m_Material.GetShadowType() == ST_PLANAR)
	//{
	//	Vector3 vl = m_pShadowLight->GetPosition();	// 目前支持点光, 其实平行光也差不多
	//	Vector3 vn = m_vPlanarShadowDestNormal;		// 平面法向

	//	float d = 0;
	//	Matrix4 mat;	// 平面投影矩阵
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
// Desc: 检测是否有这个子结点
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
	m_bDirty = true;	// 节点的最终变换矩阵需要更新

	m_bDirtyBB = true;	// 节点的包围球需要更新
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
		(*ptr)->m_vPositionRelative *= scale;	// 儿子和父亲的相对距离发生改变
		(*ptr)->Scale(scale);
	}
}