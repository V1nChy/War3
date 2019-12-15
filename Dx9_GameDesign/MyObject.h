
#pragma once
#pragma warning(disable:4786)

#include <Windows.h>
#include "Vector.h"
#include "Matrix.h"
#include "Sphere.h"
#include "Frustum.h"
//#include "OrientBoundingBox.h"
//#include "GEMesh.h"
#include "MyMaterial.h"

#include <list>
#include <string>
using namespace std;

class MyLight;
class MyCamera;
class MyMaterial;
class MyMesh;

/** ����, ���� \n
* ��Ϸ�����е����嶼�Ǽ̳��Դ���:
* Entity, Light, Camera, SceneNode ... 
* ���б����λ�ú���ת�����Ǿ��Եģ����������������ϵ��˵��
*/
class MyObject
{
private:
	void _RenderBoundingSphere();		//��Ⱦ��Χ��
	void FinalCleanup();				//�������
protected:
	string  m_strName;				// ����, Ψһ
	bool    m_bIsVisible;			// �Ƿ�ɼ�

	MyObject* m_pFather;			// �����ָ��
	list<MyObject*> m_pChildList;	// �ӽڵ�����

	MyMesh	* m_pMesh;				// Mesh
	MyMaterial m_Material;			// ����


	//OrientBoundingBox m_OBB;		// �����Χ��
	Sphere m_SelfBoundingSphere;	// ֻ��Χ����İ�Χ��	
	Sphere m_BoundingSphere;		// �ý��İ�Χ��, ��̬����

	bool m_bDirtyBB;				// �Ƿ���Ҫ���¼��������Χ��
	bool m_bDirty;					// �Ƿ���Ҫ���¼������ձ任������Ϊ�ƶ�����ת�����ţ�

	/** ��������İ�Χ�򣬴˺���������Ҫ��Ϊ�˽����������ϵͳһ�������ʾ��û��OBB�Ľڵ�,
	*  ����ڵ�Ҫ���ش˺���
	*/
	virtual void UpdateSelfBoundingSphere();

	const MyLight* m_pShadowLight;		// ��Ӱ���õĵƹ�
	Vector3 m_vPlanarShadowDestNormal;	// ƽ����ӰͶӰ��ķ���			

	Vector3 m_vPositionRelative;	// ��Ը��ڵ�λ��
	Matrix3 m_mRotateRelative;		// ��Ը��ڵ����ת����

	float	m_fScale;				// ģ����������ű���������Ӱ���ӽڵ㣩
	Matrix4 m_mScale;				// ģ����������ž��󣨲���Ӱ���ӽڵ㣩

	Matrix4 m_mTransformation;		// ����任���� (ֻ����ƽ�ƺ���ת������������)
	Matrix4 m_mFinalTransformMatrix;// ���ձ任���� (����ƽ�ƣ���ת�����ţ�

public:
	/* ���캯��	\n
	* @param [string]	name: �����Ψһ����
	* @param [GEObject*] father: �ҽ����ĸ���������, һ��Ҫ������NULL�����ǳ���ͼ�ĸ��ڵ㣩
	* @param [Vector3]	offset: ����ڸ��׵�λ��
	* @param [float]	radianRight: ���������X�����ת
	* @param [float]	radianUp: ���������Y�����ת
	* @param [float]	radianDir: ���������Z�����ת
	* @param [bool]	isVisible: �����Ƿ�ɼ�, �粻�ɼ�, �����軭����ҵĽ��, Ĭ���ǿɼ�
	*/
	MyObject(const string& name, MyObject* father, const Vector3& offsetFromParent = Vector3::ZERO,
		float radianX = 0.f, float radianY = 0.f, float radianZ = 0.f, bool isVisible = true);

	virtual ~MyObject();
public:
	static MyObject* c_Root;				// �����

	/** ������Ӱ�� */
	void InitPlanarShadowSrc(const MyLight* const light, const Vector3& vNormal);
	/** ��Ч����Ӱ */
	void InvalidateShadow(); 		
	/** ������ӰͶӰ�� */
	void InitPlanarShadowDest();

	/** ���ز��� */
	/*inline void LoadMaterial(const string& fileName) 
	{ m_Material.Load("Material/"+fileName); }*/

	/** �õ���ǰλ��(��������ϵ��) */
	inline Vector3	GetPosition() const	{ return m_mTransformation.GetOffset(); }
	/** �õ�ǰ������ z ����һ����������*/
	inline Vector3	GetDir() const		{ return m_mTransformation.GetDirUnnormalized(); }	
	/** �õ��������� x ����һ����������*/
	inline Vector3	GetRight() const	{ return m_mTransformation.GetRightUnnormalized();}	
	/** �õ��������� y ����һ����������*/
	inline Vector3	GetUp() const		{ return m_mTransformation.GetUpUnnormalized(); }	
	/** �õ��������ת���� */
	const Matrix3& GetRotationMatrix() const;		
	/** �õ�����任����(����ƽ��,��ת������������) */
	inline const Matrix4& GetTransformMatrix() const { return m_mTransformation; }
	/** �õ���������任���󣨰���ƽ�ơ���ת�����ţ� */
	inline const Matrix4& GetFinalTransformMatrix() const { return m_mFinalTransformMatrix; }


	/** �õ����λ�� */
	inline const Vector3& GetPositionRelative() const	{ return m_vPositionRelative; }
	/** �õ����ǰ������ z ����һ����������*/
	inline Vector3	GetDirRelative() const				{ return m_mRotateRelative.GetRow(2); }	
	/** �õ������������ x ����һ����������*/
	inline Vector3	GetRightRelative() const			{ return m_mRotateRelative.GetRow(0); }	
	/** �õ������������ y ����һ����������*/
	inline Vector3	GetUpRelative() const				{ return m_mRotateRelative.GetRow(1); }	
	/** �õ�����������ת���� */
	inline const Matrix3& GetRotationMatrixRelative() const	{ return m_mRotateRelative; }	
	/** �õ��������Ա任����ֻ����ת��λ����Ϣ����������Ϣ�� */
	const Matrix4& GetTransformMatrixRelative() const;



	void Move(const Vector3& offset);		///< �ƶ�����(x,y,z), ���ص�ǰλ��
	void MoveTo(const Vector3& newPos);		///< �ƶ���λ��, ���ص�ǰλ��
	void Scale( float scale );				///< ���ţ�����Ҳ��������

	float GetScale() const { return m_fScale; }

	/** �ɼ������� */
	inline bool IsVisible() const	{ return m_bIsVisible; }	
	inline void SetVisible( bool visible ) { m_bIsVisible = visible; }

	inline const string& GetName() const { return m_strName; }	///< ȡ������

	void RotationPitch( float radian );		///< ���Լ���x��ת����ͷ
	void RotationYaw( float radian );		///< ���Լ���y��ת��ҡͷ
	void RotationRoll( float radian );		///< ���Լ���z��ת����ת
	void RotationPitchYawRoll( float radianX,	
		float radianY, float radianZ );		///< �������xyz��ת	

	/** ������Ը��ڵ�ķ��� */
	void SetDirectionRelative( const Vector3& vDir, const Vector3& vUp = Vector3::Y_AXIS);	
						
	/** �����Լ��ı任���󣨴�����ʼѰ����
	 * @return [const Matrix4&] : ���������ձ任����(����������)
	 * @note : ������¸��ڵ���ӽڵ�
	 */
	const Matrix4& UpdateTransformation();		
	/** �������Լ�Ϊ����һ�������ı任���� 
	 * @param [bool] compulsive: �Ƿ�ǿ�Ƹ���
	 */
	void UpdateSubSceneGraph(bool compulsive);

	const Sphere& GetBoundingSphere() const { return m_BoundingSphere; }
	const Sphere& GetSelfBoundingSphere() const { return m_SelfBoundingSphere; }
	//const OrientBoundingBox& GetOBB() const { return m_OBB; }

	/* ���¼���˽�㼰���ӽ��İ�Χ��	*/
	void UpdateBoundingSphere();			

	inline MyObject* GetParent() const	{ return m_pFather; }
	bool Attach(MyObject * father);			///< �ҽӵ�һ��object��, ���Լ�����ԭ���ڵ��Detach(), ���¸�����AddAttached()
	bool Detach(const string& name);		///< ж��һ��child
	bool AddAttach(MyObject * child);		///< ����һ���µĸ�������	
	void DetachAll();						///< ˦�����ж���	

	bool SetMesh(MyMesh * mesh);					///< ��һ��Mesh
	MyMesh* GetMesh() const		{ return m_pMesh; }	///< �õ���OBJ��Ӧ��Mesh

	inline bool operator == (const MyObject& object) const
	{ return m_strName == object.GetName(); }

	inline bool operator != (const MyObject& object) const 
	{ return m_strName != object.GetName(); }


	MyObject * CheckHasChild( const string& childName ) const;		///< ����Ƿ�������ӽ��
	inline list<MyObject*>& GetChildList() { return m_pChildList; }	///< �õ��ӽ���б�

	virtual HRESULT OneTimeSceneInit();

	virtual HRESULT InitDeviceObjects()	;
	virtual HRESULT DeleteDeviceObjects();

	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();	

	virtual HRESULT FrameMove(float deltTime)	{ return S_OK;}

	virtual HRESULT Render(const MyCamera& camera);		///< ����RenderQueue
	virtual HRESULT Render();							///< �����Ļ�

	HRESULT RenderInStencil();							///< render��ģ�建����


};