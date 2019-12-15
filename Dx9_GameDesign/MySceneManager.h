// GESceneManager.h: interface for the GESceneManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GESCENEMANAGER_H__C9908845_C920_4BB7_8DD5_2DF39269DFF4__INCLUDED_)
#define AFX_GESCENEMANAGER_H__C9908845_C920_4BB7_8DD5_2DF39269DFF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable:4786)

#include "MyObject.h"
#include "MyCamera.h"
#include "MyLight.h"

//#include "GEParticleSystem.h"
//#include "GECuboidPS.h"
//#include "GENumberPS.h"

//#include "GEParticleSystemGroup.h"
//#include "GEMathPSGroup.h"

//#include "GERenderQueue.h"
//#include "GESkybox.h"

//#include "Ray.h"

#include <windows.h>
#include <string>
using namespace std;

class MySceneManager  
{
protected:
	static MySceneManager c_SceneManager;
	MySceneManager();
	virtual ~MySceneManager();

	HRESULT _FrameMoveNode(float deltTime, MyObject* root);

	vector<pair<float, MyObject*> > mMousePickedObjects;
	void _MousePick(MyObject* obj, const Ray& mousePickRay);
	//pair<bool, float> _MeshIntersect(GEObject* obj, GEMesh* mesh, const Ray& mousePickRay);
	//pair<bool, float> _SubMeshIntersect(GEObject* obj, GESubmesh* submesh, const Ray& mousePickRay);
public:
	bool IfNameConflicted(const string& name) const;
	/*----------------------------------------------------------------------------------------------*/
	//创建对象
	/*GEParticleSystem * CreateParticleSystem(const string& name, string scriptsFile, GEObject * father, 
		const Vector3& offset = Vector3::ZERO, float radianPitch = 0, float radianYaw = 0, float radianRoll = 0, bool isVisible = true );*/
	
	/*GEParticleSystemGroup * CreateParticleSystemGroup(const string& name, string scriptsFile, GEObject * father, 
		const Vector3& offset = Vector3::ZERO, float radianPitch = 0, float radianYaw = 0, float radianRoll = 0, bool isVisible = true );*/

	/*GEMathPSGroup * CreateMathPSGroup(const string& name, GEObject * father, const Vector3& offset = Vector3::ZERO, 
		float radianPitch = 0, float radianYaw = 0, float radianRoll = 0, bool isVisible = true );*/

	MyLight * CreateLight(const string& name, MyObject * father = MyObject::c_Root, const Vector3& offset = Vector3::ZERO,
		float radianX = 0.f, float radianY = 0.f, float radianZ = 0.f);

	MyCamera * CreateCamera(const string& name, MyObject * father = MyObject::c_Root, const Vector3& offset = Vector3::ZERO,
		float radianX = 0.f, float radianY = 0.f, float radianZ = 0.f, 
		float fov = PI/2, float nearclip = 0.17f, float farclip = 1000.f);

	/*GESkybox * CreateSkybox(const string& name, const string& cubeTextureFile, size_t size, bool visible);*/

	/*----------------------------------------------------------------------------------------------*/
	//销毁对象
	void FinalCleanup(MyObject* root = MyObject::c_Root);

    HRESULT DeleteDeviceObjects(MyObject* root = MyObject::c_Root);

	/*----------------------------------------------------------------------------------------------*/
	//初始化对象
	HRESULT OneTimeSceneInit(MyObject* root = MyObject::c_Root);
	HRESULT InitDeviceObjects(MyObject* root = MyObject::c_Root);

	HRESULT InvalidateDeviceObjects(MyObject* root = MyObject::c_Root);
	HRESULT RestoreDeviceObjects(MyObject* root = MyObject::c_Root);

	/*----------------------------------------------------------------------------------------------*/
	//更新对象
	HRESULT FrameMove(float deltTime, MyObject* root = MyObject::c_Root);
	static MySceneManager * GetSingleton() { return &c_SceneManager; }

	/** 更新场景图的包围球，每帧都要调用 */
	void UpdateBoundingSphere(MyObject* root = MyObject::c_Root);

	/** 更新场景图中root节点及其子节点（受移动，旋转，缩放影响） */
	void UpdateSceneGraph(MyObject* root = MyObject::c_Root, bool compulsive = false);


	/*----------------------------------------------------------------------------------------------*/
	//渲染对象
	HRESULT Render(const MyCamera& camera, MyObject* root = MyObject::c_Root);


	/** 得到场景图中的特定结点
	 * @return [GEObject *] : 该结点。NULL 说明无此结点
	 */
	MyObject* GetNode( const string& name, const MyObject* root = MyObject::c_Root );
	const MyObject* const GetNode( const string& name, const MyObject* root = MyObject::c_Root ) const;

	/** 得到鼠标点选的射线 */
	Ray GetMousePickRay(MyCamera* pCamera) const;

	/** 鼠标拾取 
	 *  @parm obj 子树的根节点
	 */
	MyObject* MousePick(MyCamera* pCamera, MyObject* obj = MyObject::c_Root);


	/** 鼠标精确拾取 
	*  @parm obj 子树的根节点
	*  @return [pair<GEObject*, Vector3>]: 第一项是相交物体指针,如无相交则为NULL, 第二项是交点座标
	*/
	std::pair<MyObject*, Vector3> MouseAccuratePick(MyCamera* pCamera, MyObject* obj = MyObject::c_Root);

	bool static _SortMode(const pair<float, MyObject*>& itr1, const pair<float, MyObject*>& itr2);



};

#endif // !defined(AFX_GESCENEMANAGER_H__C9908845_C920_4BB7_8DD5_2DF39269DFF4__INCLUDED_)
