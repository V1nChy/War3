#ifndef _GE_VERTEX_BUFFER_H_
#define _GE_VERTEX_BUFFER_H_

#include "GEPrimitiveType.h"

class GEMesh;
class GESkeleton;
class GEMorphAnimationKey;
class GEBoneAssignment;

/**	 	 
* 顶点缓冲区，封装底层顶点缓冲区的创建、填充数据、设备丢失的处理等。.
* 顶点缓冲区提供了几种创建方式：
* 1．以managed方式创建
*   这是最常用的方式。数据存放与显存中，底层图形库自动在系统内存中备份、处理设备丢失。
* 2．以default方式创建
*   这种方式将不在内存中保存数据备份，需要在设备丢失时重新从硬盘中装载数据。当mesh很大时，在系统内存中备份数据将占用大量空间，而设备丢失并不经常发生，所以这种情况下可以以default方式创建缓冲区，节省内存空间。这种做法的缺点是设备丢失时重新读取硬盘、解析mesh文件，用户停顿感比较明显，但设备丢失在游戏运行过程中并不频繁。
* 3．附加dynamic方式创建
*   当缓冲区需要频繁锁定时（如用于骨骼动画），以dynamic方式创建将极大地提高程序运行效率。如果缓冲区以dynamic方式创建，当需要：
*   a） 重新填充缓冲区中的数据：
*     以discard方式锁定缓冲区，这时，如果显卡正在读取该缓存，DirectX将立刻从锁定函数返回，并指向一块新的缓冲区，原来的缓冲区将在显卡读取完毕后释放掉。这样，程序就不用在锁定时挂起直至显卡读取完数据。
*   b）只添加新的数据：
*     以nooverwrite方式锁定缓冲区。这时，如果显卡帧在读取该缓存，DirectX同样将立刻从锁定函数返回，并指向该缓冲区的尾部，程序可以添加新的数据在缓冲区末尾，而不影响显卡读取缓存中前面的数据。
*  \n\n
* 大部分情况下，客户程序员应该让GEVertexBuffer以managed方式创建，但两种情况下
* 客户程序员可能不希望以managed方式创建：
* 1、该顶点缓冲区用于骨骼动画，要频繁更新缓冲区数据，这种情况下非managed效率
*    较高（内部使用D3DPOOL_DEFAULT）；
* 2、模型很大，不希望在内存中备份而占用过多内存（代价是RestoreDeviceObjects()时要从硬盘
*    读入数据）
*   在这两种情况下，请在构造函数里指明managed为false。此时将以非managed方式创建缓冲区。
* 并且如果客户程序员选择让GEVertexBuffer在系统内存中备份顶点数据、自动处理restore，那么
* 在new了一个GEVertexBuffer对象之后必须调用PrepareSysBuffer()；反之，如果客户程序员出于
* 某些原因选择让GEVertexBuffer不要在系统内存中备份数据（如：因为该顶点缓冲区很大，在
* 系统内存中备份将浪费极大空间），那么就无需调用PrepareSysBuffer()，但在Restore时需要
* 提供外来的数据源，即调用RestoreDeviceObjects(void* backSource, UINT sizeToFeed).
*   如果顶点缓冲区需要大量更新并且更新方式是完全丢弃原数据或只添加新数据，请在构造函数
* 指定dynamic为true。
* 注意：并非所有需要频繁更新的顶点缓冲区都可以以dynamic方式创建。
*/	
class GEVertexBuffer
{
protected:		
	UINT m_vertexCount;	///< 缓冲区中的顶点数量
	UINT m_stride;		///< 顶点的步长
	UINT m_size;		///< 缓冲区的总容量
	UINT m_dataSize;	///< 缓冲区中实际保存的数据的字节数
	DWORD m_FVF;		///< D3D的灵活顶点格式
	GEPrimitiveType m_primitiveType;	///< 图元格式	
	IDirect3DVertexBuffer9* m_DX9VB;	///< 底层D3D的顶点缓冲区
	IDirect3DVertexBuffer9* m_ShadowVB; ///< 用于骨骼动画的影子缓存。如果使用该顶点缓冲区的mesh无骨骼动画，则无需
	BYTE* m_sysBuffer;	///< 在系统内存中的备份，用于在设备设备丢失后restore时恢复显存中的IDirect3DIndexBuffer的数据。根据设置，可能无需使用此成员。
	bool m_managed;		///< 是否是以D3DPOOL_MANAGED创建的
	bool m_dynamic;		///< 是否需要频繁更新的
	bool m_autoBackupForRestore; ///< 是否自动在系统内存备份数据的标志
	
public:
	/**
	* 构造函数.
	* @param size 要创建的缓存大小
	* @param FVF 缓冲区的顶点格式
	* @param stride 缓存内顶点的步长
	* @param primitiveType 图元类型（三角形条带、线段列表等）	
	* @param managed 以managed方式创建，由DirectX底层自动管理设备丢失，无需引擎在系统内存中
	* 备份数据。
	* @param dynamic 如果缓存需要被频繁更新，并且更新时原来的数据全部丢弃或只在原数据后面
	* 添加，则使用此标志将是操作效率更高。具体见类说明。
	* @param autoBackupForRestore 如果以非managed方式创建缓冲区，选择此标志可以使引擎在系统
	* 内存自动备份数据用于设备丢失的处理；如果使非managed方式而且未选择此标志，则需
	* 要在处理设备丢失时提供数据源。
	*/
	GEVertexBuffer(UINT size, DWORD FVF, UINT stride, GEPrimitiveType primitiveType, 
		bool managed=true, bool dynamic=false, bool autoBackupForRestore=true);
	virtual ~GEVertexBuffer();

	/**
	* 准备系统内存中的数据备份，需要提供数据源。
	* 当以非managed方式创建，并要求引擎在系统内存备份数据时需要在创建后调用此函数。 
	*/
	void PrepareSysBuffer(void* sourceData);
	void PrepareSysBuffer(void* sourceData, UINT sizeToFeed);

	//< 非managed的vertex buffer调用此版本，无需在此提供顶点数据
	void InitDeviceObjects();
	///< managed的vertex buffer调用此版本（除非是GEMesh的VB），必须提供顶点数据
	void InitDeviceObjects(void* pSource, UINT sizeToFeed);
	///< GEMesh的VB调用此版本，随后需lock缓冲区并填充数据
	void InitDeviceObjectsFromMesh();
	///< managed的vertex buffer以及设m_autoBackupForRestore为真的非managed的vertex buffer调用此版本
	void RestoreDeviceObjects();
	///< 非managed并且未指明自动备份数据(m_autoBakcupForRestore为false)的vertex buffer调用此函数
	void RestoreDeviceObjects(void* backSource, UINT sizeToFeed);		
	void InvalidateDeviceObjects();
	void DeleteDeviceObjects();

	///< 获得D3D灵活顶点格式
	inline DWORD GetFVF() const	  { return m_FVF;	 }
	///< 获得顶点步长
	inline UINT GetStride()	const { return m_stride; }		// get size of each vertex
	///< 获得缓冲区大小
	inline UINT GetSize() const	  { return m_size;	 }		// get total size of the buffer
	///< 获得顶点数量
	inline UINT GetVertexNum() const { return m_vertexCount; }
	///< 获得基本图元的数量
	UINT   GetPrimitiveCount() const ;

	///< 获得是否创建了影子缓存
	inline bool hasShadowBuffer() { return m_ShadowVB?true:false; }
	///< 获得图元类型
	inline GEPrimitiveType GetPrimitiveType() { return m_primitiveType; }
	///< 获得D3D底层顶点缓存
	inline IDirect3DVertexBuffer9* GetSource() 
	{  
		if(hasShadowBuffer())
		{
			return m_ShadowVB;			
		}
		else
		{
			return m_DX9VB;			
		}
		
	}

	///< 填充数据
	void feed(void* pSource, UINT size);
	///< 创建影子缓存
	bool createShadowBuffer();
	///< 根据骨架更新影子缓冲区
	bool updateFromSkeleton(GESkeleton* skeleton, GEBoneAssignment* assignment);

	bool updateFromMorphAnimation(GEMorphAnimationKey* currentPose);

	inline float GetXMin() const	{ return m_fXMin; }
	inline float GetXMax() const	{ return m_fXMax; }
	inline float GetYMin() const	{ return m_fYMin; }
	inline float GetYMax() const	{ return m_fYMax; }
	inline float GetZMin() const	{ return m_fZMin; }
	inline float GetZMax() const	{ return m_fZMax; }

protected:
	void _CreateDXVB();				///< 创建m_DX9VB
	void _CopySysBufferToDXVB();	///< 把系统内存中的数据备份拷到m_DX9VB
	void _FeedDXVB(void* sourceData);	///< 填充D3D底层缓冲区

	friend GEMesh;

	float m_fXMin;
	float m_fXMax;
	float m_fYMin;
	float m_fYMax;
	float m_fZMin;
	float m_fZMax;
};


#endif