#ifndef _GE_INDEX_BUFFER_H_
#define _GE_INDEX_BUFFER_H_

//#include "GEUtil.h" // some frequently used header
#include "GEFormat.h"
#include "GEVertexBuffer.h"

// IndexBuffer 的 primitive type 就是它所绑定的VB的 primitive type
/**
* 注意：IndexBuffer内的索引必须是16位的(unsigned short)
*/
class GEIndexBuffer
{
protected:
	UINT m_indexCount;	///< 索引号的数量
	UINT m_size;		///< 缓冲区的总容量
	UINT m_dataSize;	///< 缓冲区中的数据的大小，必须小于等于m_size
	UINT m_stride;		///< 步长值，即每个索引占用的字节数
	GEFormat m_format;	///< 缓冲区中索引的格式
	IDirect3DIndexBuffer9* m_DX9IB;	///< DirectX的底层索引缓冲区
	void* m_sysbuffer;	///< 在系统内存中的备份，用于在设备设备丢失后restore时恢复显存中的IDirect3DIndexBuffer的数据。根据设置，可能无需使用此成员。
	GEVertexBuffer* m_VBassociated; ///< 此索引缓冲区对应的顶点缓冲区

	bool m_managed;		///< 是否是以D3DPOOL_MANAGED创建的
	bool m_dynamic;		///< 是否需要频繁更新，如果需要将以不同的方式创建以及锁定
	bool m_autoBackupForRestore;	///< 是否自动在系统内存备份数据的标志

	// 以下成员用于绘制，其中minIndex,maxIndex会自动计算：
	UINT m_minIndex;
	UINT m_maxIndex;	// used for calculation of NumVertices in DrawIndexedPrimitive();
	UINT m_baseIndex;

public:
	/**
	 * 构造函数.
	 * @param size 要创建的缓存大小
	 * @param stride 缓存内索引的步长
	 * @param VBassociated 对应的顶点缓冲区
	 * @param baseIndex 缓存中索引的基值，即：实际的索引是缓存中的值加上这个基值
	 * @param managed 以managed方式创建，由DirectX底层自动管理设备丢失，无需引擎在系统内存中
	 * 备份数据。
	 * @param dynamic 如果缓存需要被频繁更新，并且更新时原来的数据全部丢弃或只在原数据后面
	 * 添加，则使用此标志将是操作效率更高。具体见类说明。
	 * @param autoBackupForRestore 如果以非managed方式创建缓冲区，选择此标志可以使引擎在系统
	 * 内存自动备份数据用于设备丢失的处理；如果使非managed方式而且未选择此标志，则需
	 * 要在处理设备丢失时提供数据源。
	 */
	GEIndexBuffer(UINT size, UINT stride, GEVertexBuffer* VBassociated, 
		UINT baseIndex=0, bool managed=true, bool dynamic=false, 
		bool autoBackupForRestore=true); // 实际绘制时IndexBuffer内的index会全部加上baseIndex
	virtual ~GEIndexBuffer();

	/**
	 * 准备系统内存中的数据备份，需要提供数据源。
	 * 当以非managed方式创建，并要求引擎在系统内存备份数据时需要在创建后调用此函数。 
	 */
	void PrepareSysBuffer(void* sourceData, UINT sizeToFeed);
	void PrepareSysBuffer(void* pSource);	

	///< 非managed的index buffer调用此版本，无需在此提供顶点数据
	void InitDeviceObjects();
	///< managed的index buffer调用此版本（除非是GEMesh的IB），必须提供顶点数据
	void InitDeviceObjects(void* pSource, UINT sizeToFeed);
	///< GEMesh的IB调用此版本，随后需lock缓冲区并填充数据
	void GEIndexBuffer::InitDeviceObjectsFromMesh();
	///< managed的index buffer以及设m_autoBackupForRestore为真的非managed的index buffer调用此版本
	void RestoreDeviceObjects();
	///< 非managed并且未指明自动备份数据(m_autoBakcupForRestore为false)的index buffer调用此函数，需提供数据源。
	void RestoreDeviceObjects(void* backSource, UINT sizeToFeed);
	void InvalidateDeviceObjects();
	void DeleteDeviceObjects();

	// 用于绘制的几个函数：

	///< 获取该索引缓存对应的顶点缓冲区中的顶点步长
	inline UINT GetVBStride() const						{ return m_VBassociated->GetStride(); }
	///< 获得对应的DirectX的底层顶点缓冲区
	inline IDirect3DVertexBuffer9* GetVBSource() const	{ return m_VBassociated->GetSource(); }
	///< 获得DierectX的底层索引缓冲区
	inline IDirect3DIndexBuffer9*  GetSource() const	{ return m_DX9IB; }	
	///< 获得图元类型（三角形条带、线段列表等）
	inline GEPrimitiveType GetPrimitiveType() const		{ return m_VBassociated->GetPrimitiveType(); }
	///< 获得对应的顶点缓冲区
	inline GEVertexBuffer* GetVertexBufferAssociated() const { return m_VBassociated; }
	/**
	 * 设置索引的基值。//
	 * 可以用同一个IndexBuffer对同一个VertexBuffer的不同部分绘制，只要简单的改变baseIndex 
	 */
	inline UINT  SetBaseVertexIndex	(UINT baseIndex)	{ m_baseIndex=baseIndex; } 

	///< 获得对应的顶点缓冲区的（D3D的）灵活顶点格式
	inline DWORD GetFVF() const				{ return m_VBassociated->GetFVF(); }
	///< 获得索引的基值
	inline UINT  GetBaseVertexIndex() const	{ return m_baseIndex; }
	///< 获得整个缓存中的最小索引值
	inline UINT  GetMinIndex() const		{ return m_minIndex; }
	///< 获得此索引缓冲区所涉及的顶点个数（非精确计算，是粗略估计值）
	inline UINT  GetNumVertices() const		{ return m_maxIndex-m_minIndex+1; }
    ///< 获得图元的个数
	UINT   GetPrimitiveCount() const;	

	///< 获得缓冲区的格式
	inline GEFormat GetFormat()	const	{ return m_format; }
	///< 获得缓冲区的大小
	inline UINT GetSize() const			{ return m_size;   }		// get total size of the buffer
	///< 获得缓冲区中索引的数量
	inline UINT GetIndexNum() const		{ return m_indexCount; }

protected:
	void _CreateDXIB();					///< 创建D3D底层索引缓冲区
	void _CopySysBuffeToDXIB();			///< 从系统内存中将备份的索引数据拷回D3D缓存
	void _FeedDXIB(void* sourceData);	///< 根据提供的数据源填充DX缓存

	friend GEMesh;
};


#endif