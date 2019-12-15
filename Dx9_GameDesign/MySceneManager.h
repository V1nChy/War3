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
	//��������
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
	//���ٶ���
	void FinalCleanup(MyObject* root = MyObject::c_Root);

    HRESULT DeleteDeviceObjects(MyObject* root = MyObject::c_Root);

	/*----------------------------------------------------------------------------------------------*/
	//��ʼ������
	HRESULT OneTimeSceneInit(MyObject* root = MyObject::c_Root);
	HRESULT InitDeviceObjects(MyObject* root = MyObject::c_Root);

	HRESULT InvalidateDeviceObjects(MyObject* root = MyObject::c_Root);
	HRESULT RestoreDeviceObjects(MyObject* root = MyObject::c_Root);

	/*----------------------------------------------------------------------------------------------*/
	//���¶���
	HRESULT FrameMove(float deltTime, MyObject* root = MyObject::c_Root);
	static MySceneManager * GetSingleton() { return &c_SceneManager; }

	/** ���³���ͼ�İ�Χ��ÿ֡��Ҫ���� */
	void UpdateBoundingSphere(MyObject* root = MyObject::c_Root);

	/** ���³���ͼ��root�ڵ㼰���ӽڵ㣨���ƶ�����ת������Ӱ�죩 */
	void UpdateSceneGraph(MyObject* root = MyObject::c_Root, bool compulsive = false);


	/*----------------------------------------------------------------------------------------------*/
	//��Ⱦ����
	HRESULT Render(const MyCamera& camera, MyObject* root = MyObject::c_Root);


	/** �õ�����ͼ�е��ض����
	 * @return [GEObject *] : �ý�㡣NULL ˵���޴˽��
	 */
	MyObject* GetNode( const string& name, const MyObject* root = MyObject::c_Root );
	const MyObject* const GetNode( const string& name, const MyObject* root = MyObject::c_Root ) const;

	/** �õ�����ѡ������ */
	Ray GetMousePickRay(MyCamera* pCamera) const;

	/** ���ʰȡ 
	 *  @parm obj �����ĸ��ڵ�
	 */
	MyObject* MousePick(MyCamera* pCamera, MyObject* obj = MyObject::c_Root);


	/** ��꾫ȷʰȡ 
	*  @parm obj �����ĸ��ڵ�
	*  @return [pair<GEObject*, Vector3>]: ��һ�����ཻ����ָ��,�����ཻ��ΪNULL, �ڶ����ǽ�������
	*/
	std::pair<MyObject*, Vector3> MouseAccuratePick(MyCamera* pCamera, MyObject* obj = MyObject::c_Root);

	bool static _SortMode(const pair<float, MyObject*>& itr1, const pair<float, MyObject*>& itr2);



};

#endif // !defined(AFX_GESCENEMANAGER_H__C9908845_C920_4BB7_8DD5_2DF39269DFF4__INCLUDED_)
