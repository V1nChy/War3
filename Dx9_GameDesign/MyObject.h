
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

/** 物体, 基类 \n
* 游戏中所有的物体都是继承自此类:
* Entity, Light, Camera, SceneNode ... 
* 所有保存的位置和旋转矩阵都是绝对的，是相对于世界坐标系来说的
*/
class MyObject
{
private:
	void _RenderBoundingSphere();		//渲染包围球
	void FinalCleanup();				//清理对象
protected:
	string  m_strName;				// 名字, 唯一
	bool    m_bIsVisible;			// 是否可见

	MyObject* m_pFather;			// 父结点指针
	list<MyObject*> m_pChildList;	// 子节点链表

	MyMesh	* m_pMesh;				// Mesh
	MyMaterial m_Material;			// 材质


	//OrientBoundingBox m_OBB;		// 有向包围盒
	Sphere m_SelfBoundingSphere;	// 只包围自身的包围球	
	Sphere m_BoundingSphere;		// 该结点的包围球, 动态调整

	bool m_bDirtyBB;				// 是否需要重新计算自身包围球
	bool m_bDirty;					// 是否需要重新计算最终变换矩阵（因为移动、旋转或缩放）

	/** 更新自身的包围球，此函数设置主要是为了解决类似粒子系统一般的有显示但没有OBB的节点,
	*  此类节点要重载此函数
	*/
	virtual void UpdateSelfBoundingSphere();

	const MyLight* m_pShadowLight;		// 阴影作用的灯光
	Vector3 m_vPlanarShadowDestNormal;	// 平面阴影投影面的法向			

	Vector3 m_vPositionRelative;	// 相对父节点位置
	Matrix3 m_mRotateRelative;		// 相对父节点的旋转矩阵

	float	m_fScale;				// 模型整体的缩放比例（不会影响子节点）
	Matrix4 m_mScale;				// 模型整体的缩放矩阵（不会影响子节点）

	Matrix4 m_mTransformation;		// 世界变换矩阵 (只包括平移和旋转，不包括缩放)
	Matrix4 m_mFinalTransformMatrix;// 最终变换矩阵 (包括平移，旋转和缩放）

public:
	/* 构造函数	\n
	* @param [string]	name: 物体的唯一名字
	* @param [GEObject*] father: 挂接在哪个物体上面, 一定要给出（NULL表明是场景图的根节点）
	* @param [Vector3]	offset: 相对于父亲的位移
	* @param [float]	radianRight: 相对于自身X轴的旋转
	* @param [float]	radianUp: 相对于自身Y轴的旋转
	* @param [float]	radianDir: 相对于自身Z轴的旋转
	* @param [bool]	isVisible: 自身是否可见, 如不可见, 则无需画下面挂的结点, 默认是可见
	*/
	MyObject(const string& name, MyObject* father, const Vector3& offsetFromParent = Vector3::ZERO,
		float radianX = 0.f, float radianY = 0.f, float radianZ = 0.f, bool isVisible = true);

	virtual ~MyObject();
public:
	static MyObject* c_Root;				// 根结点

	/** 设置阴影物 */
	void InitPlanarShadowSrc(const MyLight* const light, const Vector3& vNormal);
	/** 无效化阴影 */
	void InvalidateShadow(); 		
	/** 设置阴影投影体 */
	void InitPlanarShadowDest();

	/** 加载材质 */
	/*inline void LoadMaterial(const string& fileName) 
	{ m_Material.Load("Material/"+fileName); }*/

	/** 得到当前位置(世界坐标系中) */
	inline Vector3	GetPosition() const	{ return m_mTransformation.GetOffset(); }
	/** 得到前向向量 z （规一化的向量）*/
	inline Vector3	GetDir() const		{ return m_mTransformation.GetDirUnnormalized(); }	
	/** 得到右向向量 x （规一化的向量）*/
	inline Vector3	GetRight() const	{ return m_mTransformation.GetRightUnnormalized();}	
	/** 得到上向向量 y （规一化的向量）*/
	inline Vector3	GetUp() const		{ return m_mTransformation.GetUpUnnormalized(); }	
	/** 得到物体的旋转矩阵 */
	const Matrix3& GetRotationMatrix() const;		
	/** 得到世界变换矩阵(经过平移,旋转，不包括缩放) */
	inline const Matrix4& GetTransformMatrix() const { return m_mTransformation; }
	/** 得到最终世界变换矩阵（包括平移、旋转和缩放） */
	inline const Matrix4& GetFinalTransformMatrix() const { return m_mFinalTransformMatrix; }


	/** 得到相对位置 */
	inline const Vector3& GetPositionRelative() const	{ return m_vPositionRelative; }
	/** 得到相对前向向量 z （规一化的向量）*/
	inline Vector3	GetDirRelative() const				{ return m_mRotateRelative.GetRow(2); }	
	/** 得到相对右向向量 x （规一化的向量）*/
	inline Vector3	GetRightRelative() const			{ return m_mRotateRelative.GetRow(0); }	
	/** 得到相对上向向量 y （规一化的向量）*/
	inline Vector3	GetUpRelative() const				{ return m_mRotateRelative.GetRow(1); }	
	/** 得到物体的相对旋转矩阵 */
	inline const Matrix3& GetRotationMatrixRelative() const	{ return m_mRotateRelative; }	
	/** 得到物体的相对变换矩阵（只有旋转和位移信息，无缩放信息） */
	const Matrix4& GetTransformMatrixRelative() const;



	void Move(const Vector3& offset);		///< 移动距离(x,y,z), 返回当前位置
	void MoveTo(const Vector3& newPos);		///< 移动到位置, 返回当前位置
	void Scale( float scale );				///< 缩放，儿子也跟着缩放

	float GetScale() const { return m_fScale; }

	/** 可见性问题 */
	inline bool IsVisible() const	{ return m_bIsVisible; }	
	inline void SetVisible( bool visible ) { m_bIsVisible = visible; }

	inline const string& GetName() const { return m_strName; }	///< 取得名字

	void RotationPitch( float radian );		///< 绕自己的x轴转－点头
	void RotationYaw( float radian );		///< 绕自己的y轴转－摇头
	void RotationRoll( float radian );		///< 绕自己的z轴转－翻转
	void RotationPitchYawRoll( float radianX,	
		float radianY, float radianZ );		///< 绕自身的xyz旋转	

	/** 设置相对父节点的方向 */
	void SetDirectionRelative( const Vector3& vDir, const Vector3& vUp = Vector3::Y_AXIS);	
						
	/** 更新自己的变换矩阵（从自身开始寻根）
	 * @return [const Matrix4&] : 计算后的最终变换矩阵(不包括缩放)
	 * @note : 不会更新父节点或子节点
	 */
	const Matrix4& UpdateTransformation();		
	/** 更新以自己为根的一棵子树的变换矩阵 
	 * @param [bool] compulsive: 是否强制更新
	 */
	void UpdateSubSceneGraph(bool compulsive);

	const Sphere& GetBoundingSphere() const { return m_BoundingSphere; }
	const Sphere& GetSelfBoundingSphere() const { return m_SelfBoundingSphere; }
	//const OrientBoundingBox& GetOBB() const { return m_OBB; }

	/* 重新计算此结点及其子结点的包围球	*/
	void UpdateBoundingSphere();			

	inline MyObject* GetParent() const	{ return m_pFather; }
	bool Attach(MyObject * father);			///< 挂接到一个object上, 会自己调用原父节点的Detach(), 及新父结点的AddAttached()
	bool Detach(const string& name);		///< 卸载一个child
	bool AddAttach(MyObject * child);		///< 增加一个新的附着物体	
	void DetachAll();						///< 甩掉所有儿子	

	bool SetMesh(MyMesh * mesh);					///< 给一个Mesh
	MyMesh* GetMesh() const		{ return m_pMesh; }	///< 得到该OBJ对应的Mesh

	inline bool operator == (const MyObject& object) const
	{ return m_strName == object.GetName(); }

	inline bool operator != (const MyObject& object) const 
	{ return m_strName != object.GetName(); }


	MyObject * CheckHasChild( const string& childName ) const;		///< 检测是否有这个子结点
	inline list<MyObject*>& GetChildList() { return m_pChildList; }	///< 得到子结点列表

	virtual HRESULT OneTimeSceneInit();

	virtual HRESULT InitDeviceObjects()	;
	virtual HRESULT DeleteDeviceObjects();

	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();	

	virtual HRESULT FrameMove(float deltTime)	{ return S_OK;}

	virtual HRESULT Render(const MyCamera& camera);		///< 放入RenderQueue
	virtual HRESULT Render();							///< 真正的画

	HRESULT RenderInStencil();							///< render到模板缓冲区


};