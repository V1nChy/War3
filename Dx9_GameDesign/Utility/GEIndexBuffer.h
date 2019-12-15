#ifndef _GE_INDEX_BUFFER_H_
#define _GE_INDEX_BUFFER_H_

//#include "GEUtil.h" // some frequently used header
#include "GEFormat.h"
#include "GEVertexBuffer.h"

// IndexBuffer �� primitive type ���������󶨵�VB�� primitive type
/**
* ע�⣺IndexBuffer�ڵ�����������16λ��(unsigned short)
*/
class GEIndexBuffer
{
protected:
	UINT m_indexCount;	///< �����ŵ�����
	UINT m_size;		///< ��������������
	UINT m_dataSize;	///< �������е����ݵĴ�С������С�ڵ���m_size
	UINT m_stride;		///< ����ֵ����ÿ������ռ�õ��ֽ���
	GEFormat m_format;	///< �������������ĸ�ʽ
	IDirect3DIndexBuffer9* m_DX9IB;	///< DirectX�ĵײ�����������
	void* m_sysbuffer;	///< ��ϵͳ�ڴ��еı��ݣ��������豸�豸��ʧ��restoreʱ�ָ��Դ��е�IDirect3DIndexBuffer�����ݡ��������ã���������ʹ�ô˳�Ա��
	GEVertexBuffer* m_VBassociated; ///< ��������������Ӧ�Ķ��㻺����

	bool m_managed;		///< �Ƿ�����D3DPOOL_MANAGED������
	bool m_dynamic;		///< �Ƿ���ҪƵ�����£������Ҫ���Բ�ͬ�ķ�ʽ�����Լ�����
	bool m_autoBackupForRestore;	///< �Ƿ��Զ���ϵͳ�ڴ汸�����ݵı�־

	// ���³�Ա���ڻ��ƣ�����minIndex,maxIndex���Զ����㣺
	UINT m_minIndex;
	UINT m_maxIndex;	// used for calculation of NumVertices in DrawIndexedPrimitive();
	UINT m_baseIndex;

public:
	/**
	 * ���캯��.
	 * @param size Ҫ�����Ļ����С
	 * @param stride �����������Ĳ���
	 * @param VBassociated ��Ӧ�Ķ��㻺����
	 * @param baseIndex �����������Ļ�ֵ������ʵ�ʵ������ǻ����е�ֵ���������ֵ
	 * @param managed ��managed��ʽ��������DirectX�ײ��Զ������豸��ʧ������������ϵͳ�ڴ���
	 * �������ݡ�
	 * @param dynamic ���������Ҫ��Ƶ�����£����Ҹ���ʱԭ��������ȫ��������ֻ��ԭ���ݺ���
	 * ��ӣ���ʹ�ô˱�־���ǲ���Ч�ʸ��ߡ��������˵����
	 * @param autoBackupForRestore ����Է�managed��ʽ������������ѡ��˱�־����ʹ������ϵͳ
	 * �ڴ��Զ��������������豸��ʧ�Ĵ������ʹ��managed��ʽ����δѡ��˱�־������
	 * Ҫ�ڴ����豸��ʧʱ�ṩ����Դ��
	 */
	GEIndexBuffer(UINT size, UINT stride, GEVertexBuffer* VBassociated, 
		UINT baseIndex=0, bool managed=true, bool dynamic=false, 
		bool autoBackupForRestore=true); // ʵ�ʻ���ʱIndexBuffer�ڵ�index��ȫ������baseIndex
	virtual ~GEIndexBuffer();

	/**
	 * ׼��ϵͳ�ڴ��е����ݱ��ݣ���Ҫ�ṩ����Դ��
	 * ���Է�managed��ʽ��������Ҫ��������ϵͳ�ڴ汸������ʱ��Ҫ�ڴ�������ô˺����� 
	 */
	void PrepareSysBuffer(void* sourceData, UINT sizeToFeed);
	void PrepareSysBuffer(void* pSource);	

	///< ��managed��index buffer���ô˰汾�������ڴ��ṩ��������
	void InitDeviceObjects();
	///< managed��index buffer���ô˰汾��������GEMesh��IB���������ṩ��������
	void InitDeviceObjects(void* pSource, UINT sizeToFeed);
	///< GEMesh��IB���ô˰汾�������lock���������������
	void GEIndexBuffer::InitDeviceObjectsFromMesh();
	///< managed��index buffer�Լ���m_autoBackupForRestoreΪ��ķ�managed��index buffer���ô˰汾
	void RestoreDeviceObjects();
	///< ��managed����δָ���Զ���������(m_autoBakcupForRestoreΪfalse)��index buffer���ô˺��������ṩ����Դ��
	void RestoreDeviceObjects(void* backSource, UINT sizeToFeed);
	void InvalidateDeviceObjects();
	void DeleteDeviceObjects();

	// ���ڻ��Ƶļ���������

	///< ��ȡ�����������Ӧ�Ķ��㻺�����еĶ��㲽��
	inline UINT GetVBStride() const						{ return m_VBassociated->GetStride(); }
	///< ��ö�Ӧ��DirectX�ĵײ㶥�㻺����
	inline IDirect3DVertexBuffer9* GetVBSource() const	{ return m_VBassociated->GetSource(); }
	///< ���DierectX�ĵײ�����������
	inline IDirect3DIndexBuffer9*  GetSource() const	{ return m_DX9IB; }	
	///< ���ͼԪ���ͣ��������������߶��б�ȣ�
	inline GEPrimitiveType GetPrimitiveType() const		{ return m_VBassociated->GetPrimitiveType(); }
	///< ��ö�Ӧ�Ķ��㻺����
	inline GEVertexBuffer* GetVertexBufferAssociated() const { return m_VBassociated; }
	/**
	 * ���������Ļ�ֵ��//
	 * ������ͬһ��IndexBuffer��ͬһ��VertexBuffer�Ĳ�ͬ���ֻ��ƣ�ֻҪ�򵥵ĸı�baseIndex 
	 */
	inline UINT  SetBaseVertexIndex	(UINT baseIndex)	{ m_baseIndex=baseIndex; } 

	///< ��ö�Ӧ�Ķ��㻺�����ģ�D3D�ģ������ʽ
	inline DWORD GetFVF() const				{ return m_VBassociated->GetFVF(); }
	///< ��������Ļ�ֵ
	inline UINT  GetBaseVertexIndex() const	{ return m_baseIndex; }
	///< ������������е���С����ֵ
	inline UINT  GetMinIndex() const		{ return m_minIndex; }
	///< ��ô��������������漰�Ķ���������Ǿ�ȷ���㣬�Ǵ��Թ���ֵ��
	inline UINT  GetNumVertices() const		{ return m_maxIndex-m_minIndex+1; }
    ///< ���ͼԪ�ĸ���
	UINT   GetPrimitiveCount() const;	

	///< ��û������ĸ�ʽ
	inline GEFormat GetFormat()	const	{ return m_format; }
	///< ��û������Ĵ�С
	inline UINT GetSize() const			{ return m_size;   }		// get total size of the buffer
	///< ��û�����������������
	inline UINT GetIndexNum() const		{ return m_indexCount; }

protected:
	void _CreateDXIB();					///< ����D3D�ײ�����������
	void _CopySysBuffeToDXIB();			///< ��ϵͳ�ڴ��н����ݵ��������ݿ���D3D����
	void _FeedDXIB(void* sourceData);	///< �����ṩ������Դ���DX����

	friend GEMesh;
};


#endif