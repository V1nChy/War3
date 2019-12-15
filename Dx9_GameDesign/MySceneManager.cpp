// GESceneManager.cpp: implementation of the GESceneManager class.
//
//////////////////////////////////////////////////////////////////////

#include "MySceneManager.h"
#include "MyApp.h"
#include "MyMouse.h"
#include "Ray.h"
#include <algorithm>

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MySceneManager MySceneManager::c_SceneManager;

MySceneManager::MySceneManager()
{

}

MySceneManager::~MySceneManager()
{

}

//-----------------------------------------------------------------------------
// Name: _FrameMoveNode()
// Desc: 更新节点
// root: 开始更新的节点
//-----------------------------------------------------------------------------
HRESULT MySceneManager::_FrameMoveNode(float deltTime, MyObject* root)
{
	//遍历root节点下的所有子节点...孙子节点...孙孙。。
	list<MyObject *>::iterator p, temp;
	for (p = root->GetChildList().begin(); p != root->GetChildList().end(); )
	{
		temp = p; ++temp;
		_FrameMoveNode(deltTime, *p);
		p = temp;
	}
	return root->FrameMove(deltTime);
}

//-----------------------------------------------------------------------------
// Name: IfNameConflicted()
// Desc: 是否重命名
//-----------------------------------------------------------------------------
bool MySceneManager::IfNameConflicted(const string& name) const
{
	if (GetNode(name) == NULL)
		return false;
	else
		return true;
}

//-----------------------------------------------------------------------------
// Name: GetNode()
// Desc: 得到场景图中的特定结点，只能查找子链表中的节点
// root: 开始查找的父节点
//-----------------------------------------------------------------------------
MyObject * MySceneManager::GetNode(const string& name, const MyObject* root)
{
	return root->CheckHasChild( name );
}
const MyObject * const MySceneManager::GetNode(const string& name, const MyObject* root)const
{
	return root->CheckHasChild(name);
}

//-----------------------------------------------------------------------------
// Name: UpdateBoundingSphere()
// Desc: 更新包围球
// root: 开始的节点（默认为根节点）
//-----------------------------------------------------------------------------
void MySceneManager::UpdateBoundingSphere(MyObject* root)
{
	//遍历root节点下的所有子节点...孙子节点...孙孙。。
	for (list<MyObject*>::iterator p = root->GetChildList().begin();
		p != root->GetChildList().end(); ++p)
	{
		UpdateBoundingSphere(*p);
	}

	root->UpdateBoundingSphere();
}

//-----------------------------------------------------------------------------
// Name: Render()
// Desc: 将节点放入渲染队列
// root: 开始的节点（默认为根节点）
// camera: 当前场景的相机，做场景剔除操作
//-----------------------------------------------------------------------------
HRESULT MySceneManager::Render(const MyCamera& camera, MyObject* root)
{
	//检查节点可见
	HRESULT r = root->Render(camera);
	if (r != S_OK)	// 说明此结点不可见，则其子结点亦不可见
		return r;

	//遍历root节点下的所有子节点...孙子节点...孙孙。。
	list<MyObject *>::iterator p;
	for ( p = root->GetChildList().begin(); p != root->GetChildList().end(); ++p )
	{
		Render(camera, *p);
	}
		
	return r;
}

//-----------------------------------------------------------------------------
// Name: OneTimeSceneInit()
// Desc: 初始化节点对象，updat之前，与设备无关
// root: 开始的节点（默认为根节点）
//-----------------------------------------------------------------------------
HRESULT MySceneManager::OneTimeSceneInit(MyObject* root)
{
	//遍历root节点下的所有子节点...孙子节点...孙孙。。
	list<MyObject *>::iterator p;
	for ( p = root->GetChildList().begin(); p != root->GetChildList().end(); ++p )
	{
		OneTimeSceneInit(*p);
	}

	return root->OneTimeSceneInit();
}

//-----------------------------------------------------------------------------
// Name: InitDeviceObjects()
// Desc: 初始化设备对象，与设备无关
// root: 开始的节点（默认为根节点）
//-----------------------------------------------------------------------------
HRESULT MySceneManager::InitDeviceObjects(MyObject* root)
{
	list<MyObject *>::iterator p;
	for (p = root->GetChildList().begin(); p != root->GetChildList().end(); ++p)
	{
		InitDeviceObjects(*p);
	}
	return root->InitDeviceObjects();
}

//-----------------------------------------------------------------------------
// Name: FinalCleanup()
// Desc: 销毁对象
// root: 开始的节点（默认为根节点）
//-----------------------------------------------------------------------------
void MySceneManager::FinalCleanup(MyObject* root)
{
	SAFE_DELETE(root);
}

HRESULT MySceneManager::DeleteDeviceObjects(MyObject* root)
{
	list<MyObject *>::iterator p;
	for ( p = root->GetChildList().begin(); p != root->GetChildList().end(); ++p )
	{
		DeleteDeviceObjects(*p);
	}
	return root->DeleteDeviceObjects();
}

HRESULT MySceneManager::InvalidateDeviceObjects(MyObject* root)
{
	list<MyObject *>::iterator p;
	for ( p = root->GetChildList().begin(); p != root->GetChildList().end(); ++p )
	{
		InvalidateDeviceObjects(*p);
	}
	return root->InvalidateDeviceObjects();
}

HRESULT MySceneManager::RestoreDeviceObjects(MyObject* root)
{
	list<MyObject *>::iterator p;
	for ( p = root->GetChildList().begin(); p != root->GetChildList().end(); ++p )
	{
		RestoreDeviceObjects(*p);
	}
	return root->RestoreDeviceObjects();
}

//-----------------------------------------------------------------------------
// Name: FrameMove()
// Desc: 更新节点，包围盒，UpdateSceneGrap()
// root: 开始的节点（默认为根节点）
//-----------------------------------------------------------------------------
HRESULT MySceneManager::FrameMove(float deltTime, MyObject* root)
{
	_FrameMoveNode(deltTime, root);

	UpdateSceneGraph(root, false);
	UpdateBoundingSphere(root);

	return S_OK;
}

//-----------------------------------------------------------------------------
// Name: UpdateSceneGraph()
// Desc: 更新节点的最终变换矩阵
// root: 开始的节点（默认为根节点）
//compulsive: 默认为false，不更新
//-----------------------------------------------------------------------------
void MySceneManager::UpdateSceneGraph(MyObject* root, bool compulsive)
{
	root->UpdateSubSceneGraph(compulsive);
}


MyCamera * MySceneManager::CreateCamera(const string& name, MyObject * father, const Vector3& offset,
		float radianX, float radianY, float radianZ, 
		float fov, float nearclip, float farclip)
{
	assert(!IfNameConflicted(name));

	MyCamera * camera = new MyCamera(name, father, offset, radianX, radianY, radianZ,
		fov, nearclip, farclip);

	return camera;
}

MyLight * MySceneManager::CreateLight(const string& name, MyObject * father, const Vector3& offset,
		float radianX, float radianY, float radianZ)
{
	assert(!IfNameConflicted(name));

	MyLight * light = new MyLight(name, father, offset, radianX, radianY, radianZ, true);

	return light;
}

//GESkybox * GESceneManager::CreateSkybox(const string& name, const string& cubeTextureFile, size_t size, bool visible)
//{
//	GESkybox * skybox = new GESkybox(name, cubeTextureFile, size, visible);
//	return skybox;
//}


//GEParticleSystem * GESceneManager::CreateParticleSystem(const string& name, string scripfile, GEObject * father, 
//				const Vector3& offset, float radianX, float radianY, float radianZ, bool isVisible)
//{
//	assert(!IfNameConflicted(name));
//	
//	scripfile = GEParticleSystem::c_strParticleSystemFilePath + scripfile;
//	PSTYPE type = GEParticleSystem::GetPSTypeInScript(scripfile);
//
//	GEParticleSystem * ps;	
//	switch(type) {
//	case PS_CUBOID:
//		ps= new GECuboidPS(name, scripfile, father, offset, radianX, radianY, radianZ, isVisible);
//		break;
//	case PS_NUM:
//		ps= new GENumberPS(name, scripfile, father, offset, radianX, radianY, radianZ, isVisible);
//		break;
//	default:
//		break;
//	}
//
//	return ps;
//}


//GEParticleSystemGroup * GESceneManager::CreateParticleSystemGroup(const string& name, string scripfile, GEObject * father, 
//												const Vector3& offset, float radianX, float radianY, float radianZ, bool isVisible)
//{
//	assert(!IfNameConflicted(name));
//
//	scripfile = GEParticleSystem::c_strParticleSystemFilePath + scripfile;
//
//	return new GEParticleSystemGroup(name, scripfile, father, offset, radianX, radianY, radianZ, isVisible);
//
//}
//


//GEMathPSGroup * GESceneManager::CreateMathPSGroup(const string& name, GEObject * father, const Vector3& offset, 
//								  float radianPitch, float radianYaw, float radianRoll, bool isVisible)
//{
//	return new GEMathPSGroup(name, father, offset, radianPitch, radianYaw, radianRoll, isVisible);
//}



Ray MySceneManager::GetMousePickRay(MyCamera* pCamera) const
{
	int screenWidth = EngineFrame::GetSingleton()->GetWidth();
	int screenHeight = EngineFrame::GetSingleton()->GetHeight();

	Vector3 camPosition = pCamera->GetPosition();
	Point2D<int> mousePosition = MyMouse::GetSingleton()->GetPos();

	const Plane* nearPlane   = pCamera->GetFrustum().GetPlane(PLANE_BACK);
	const Plane* leftPlane	 = pCamera->GetFrustum().GetPlane(PLANE_LEFT);
	const Plane* rightPlane  = pCamera->GetFrustum().GetPlane(PLANE_RIGHT);
	const Plane* topPlane	 = pCamera->GetFrustum().GetPlane(PLANE_TOP);
	const Plane* bottomPlane = pCamera->GetFrustum().GetPlane(PLANE_BOTTOM);

	Vector3 lefttopPoint    = Math::GetIntersectPointof3Planes(*nearPlane, *leftPlane, *topPlane);
	Vector3 leftbottomPoint = Math::GetIntersectPointof3Planes(*nearPlane, *leftPlane, *bottomPlane);
	Vector3 righttopPoint   = Math::GetIntersectPointof3Planes(*nearPlane, *rightPlane, *topPlane);

	const float yRate = (float)mousePosition.y/screenHeight;
	const float xRate = (float)mousePosition.x/screenWidth;
	Vector3 tempP = (1-yRate)*lefttopPoint + yRate*leftbottomPoint;
	Vector3 tempQ = xRate*righttopPoint + (1-xRate)*lefttopPoint;

	Vector3 dir = tempP + tempQ - lefttopPoint - camPosition;
	return Ray(camPosition, dir.GetNormalized());
}


MyObject* MySceneManager::MousePick(MyCamera* pCamera, MyObject* obj)
{
	mMousePickedObjects.clear();
	const Ray& mousPickRay = GetMousePickRay(pCamera);
	_MousePick(obj, mousPickRay);
	std::sort(mMousePickedObjects.begin(),mMousePickedObjects.end(),MySceneManager::_SortMode);
	if (mMousePickedObjects.size() > 0)
        return (mMousePickedObjects.begin()->second);
	else
		return NULL;
}


void MySceneManager::_MousePick(MyObject* obj, const Ray& mousePickRay)
{
	float objRadius = (obj->GetBoundingSphere()).GetRadius();			
	if (RealEqual(objRadius, 0.f))
	{
		return;
	}

	// 节点包围球相交
	pair<bool, float> itstBSResult = Math::Intersects(mousePickRay, obj->GetBoundingSphere());
	if (itstBSResult.first == true)
	{
		float objSelfRadius = (obj->GetSelfBoundingSphere()).GetRadius();
		if (!RealEqual(objSelfRadius, 0.f))
		{
			/*pair<bool, float> itstSBSResult = Math::Intersects(mousePickRay, obj->GetSelfBoundingSphere());
			if (itstSBSResult.first == true)
			{
				const OrientBoundingBox& obb = obj->GetOBB();

				pair<bool, float> itstOBBResult = Math::Intersects(mousePickRay, obb.GetCenter(), 
					obj->GetRight(), obj->GetUp(), obj->GetDir(),
					obb.GetWidth(), obb.GetHeight(), obb.GetDepth());
				if (itstOBBResult.first == true)
				{
					mMousePickedObjects.push_back(make_pair(itstOBBResult.second, obj));
				}
			}*/
		}

		for (list<MyObject*>::iterator itr = obj->GetChildList().begin(); itr != obj->GetChildList().end(); ++itr)
		{
			_MousePick((*itr), mousePickRay);
		}
	}

}



pair<MyObject*, Vector3> MySceneManager::MouseAccuratePick(MyCamera* pCamera, MyObject* root)
{
	mMousePickedObjects.clear();
	Ray mousePickRay = MySceneManager::GetSingleton()->GetMousePickRay(pCamera);
	_MousePick(root, mousePickRay);
	if (mMousePickedObjects.size() > 0)
	{
		MyObject* obj = NULL;
		float minDis = BIG_NUMBER;
		/*for (vector<pair<float, MyObject*> >::iterator itr = mMousePickedObjects.begin(); itr != mMousePickedObjects.end(); ++itr)
		{
			pair<bool, float> result = _MeshIntersect(itr->second, itr->second->GetMesh(), mousePickRay);
			if (result.first)
			{
				if (minDis > result.second)
				{
					obj = itr->second;
					minDis = result.second;
				}
			}
		}*/
		
		if (obj != NULL)
		{
			Vector3 p = mousePickRay.GetOrigin() + mousePickRay.GetDirection() * sqrtf(minDis);
			return pair<MyObject*, Vector3>(obj, p);
		}
	}

	return pair<MyObject*, Vector3>(NULL, Vector3::ZERO);
}


//pair<bool, float> MySceneManager::_SubMeshIntersect(GEObject* obj, GESubmesh* submesh, const Ray& mousePickRay)
//{
//	GEIndexBuffer* pIndexBf = submesh->getIndexBuffer();
//	unsigned int iFaceNum = pIndexBf->GetPrimitiveCount();
//	int iVBStride = pIndexBf->GetVBStride();
//
//	IDirect3DIndexBuffer9* indexSource = pIndexBf->GetSource();
//	IDirect3DVertexBuffer9* vertexSource = pIndexBf->GetVBSource();
//
//	float flength = BIG_NUMBER;
//
//	void* pi;
//	void* pv;
//
//
//	if (SUCCEEDED(indexSource->Lock(0,0,&pi,0)) && SUCCEEDED(vertexSource->Lock(0,0,&pv,0)))
//	{
//		for (unsigned int i = 0; i < iFaceNum; ++i)
//		{
//			unsigned short indexVertex0;
//			unsigned short indexVertex1;
//			unsigned short indexVertex2;
//			Vector3 v0, v1, v2;
//
//			// 得到面顶点的index
//			unsigned short* ptempIndex = reinterpret_cast<unsigned short*>(pi) + i * 3;
//			indexVertex0 = *ptempIndex;
//			indexVertex1 = *(ptempIndex + 1);
//			indexVertex2 = *(ptempIndex + 2);
//
//			// 得到面顶点座标
//			float* tempVertex = reinterpret_cast<float*>(static_cast<char*>(pv) + indexVertex0 * iVBStride);
//			v0.x = *tempVertex;
//			v0.y = *(tempVertex + 1);
//			v0.z = *(tempVertex + 2);
//
//			tempVertex = reinterpret_cast<float*>(static_cast<char*>(pv) + indexVertex1 * iVBStride);
//			v1.x = *tempVertex;
//			v1.y = *(tempVertex + 1);
//			v1.z = *(tempVertex + 2);
//
//			tempVertex = reinterpret_cast<float*>(static_cast<char*>(pv) + indexVertex2 * iVBStride);
//			v2.x = *tempVertex;
//			v2.y = *(tempVertex + 1);
//			v2.z = *(tempVertex + 2);
//
//			// 变换面顶点
//			v0 = v0 * obj->GetFinalTransformMatrix();
//			v1 = v1 * obj->GetFinalTransformMatrix();
//			v2 = v2 * obj->GetFinalTransformMatrix();
//
//			// 相交测试
//			pair<bool, float> tempresult = Math::Intersects(mousePickRay, v0, v1, v2);
//			if(tempresult.first == true && tempresult.second < flength)
//			{	
//				flength = tempresult.second;
//
//				// 简化,提高效率,只要相交了就返回,但不一定是最近点
//				//vertexSource->Unlock();
//				//indexSource->Unlock();
//				//return pair<bool, float>(true, flength);	
//			}
//
//		}
//	}
//
//	vertexSource->Unlock();
//	indexSource->Unlock();
//	return pair<bool, float>(!RealEqual(flength, BIG_NUMBER), flength);
//}

//pair<bool, float> GESceneManager::_MeshIntersect(GEObject* obj, GEMesh* mesh, const Ray& mousePickRay)
//{
//	float fresult = BIG_NUMBER;
//
//	vector<GESubmesh*>& submesh = mesh->GetSubMeshes();
//	for (vector<GESubmesh*>::iterator itr = submesh.begin(); itr != submesh.end(); ++itr)
//	{
//		pair<bool, float> tempResult = _SubMeshIntersect(obj, *itr, mousePickRay);
//		if (tempResult.first == true && tempResult.second < fresult)
//		{
//			fresult = tempResult.second;
//		}
//	}
//
//	return pair<bool, float>(fresult < BIG_NUMBER, fresult);
//}

bool MySceneManager::_SortMode(const pair<float, MyObject*>& itr1, const pair<float, MyObject*>& itr2)
{
	return (itr1.first) < (itr2.first);
}
