//--------------------------------------------------------------------------------------
// File: MeshLoader.h
//
// Wrapper class for ID3DXMesh interface. Handles loading mesh data from an .obj file
// and resource management for material textures.
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------
#ifndef _MESHLOADER_H_
#define _MESHLOADER_H_
#pragma once
#include "d3dUtility.h"
#include <assert.h>
#ifndef V
#define V(x)           { hr = (x); if( FAILED(hr) ) { MessageBox(0, "´íÎó", 0, 0); } }
#endif
#ifndef V_RETURN
#define V_RETURN(x)    { hr = (x); if( FAILED(hr) ) { return hr; } }
#endif

//--------------------------------------------------------------------------------------
// A growable array
//--------------------------------------------------------------------------------------
template<typename TYPE> class CGrowableArray
{
public:
	CGrowableArray()
	{
		m_pData = NULL; m_nSize = 0; m_nMaxSize = 0;
	}
	CGrowableArray(const CGrowableArray <TYPE>& a)
	{
		for (int i = 0; i < a.m_nSize; i++) Add(a.m_pData[i]);
	}
	~CGrowableArray()
	{
		RemoveAll();
	}

	const TYPE& operator[](int nIndex) const
	{
		return GetAt(nIndex);
	}
	TYPE& operator[](int nIndex)
	{
		return GetAt(nIndex);
	}

	CGrowableArray& operator=(const CGrowableArray <TYPE>& a)
	{
		if (this == &a) return *this; RemoveAll(); for (int i = 0; i < a.m_nSize;
		i++) Add(a.m_pData[i]); return *this;
	}

	HRESULT SetSize(int nNewMaxSize);
	HRESULT Add(const TYPE& value);
	HRESULT Insert(int nIndex, const TYPE& value);
	HRESULT SetAt(int nIndex, const TYPE& value);
	TYPE& GetAt(int nIndex) const
	{
		assert(nIndex >= 0 && nIndex < m_nSize); return m_pData[nIndex];
	}
	int     GetSize() const
	{
		return m_nSize;
	}
	TYPE* GetData()
	{
		return m_pData;
	}
	bool    Contains(const TYPE& value)
	{
		return (-1 != IndexOf(value));
	}

	int     IndexOf(const TYPE& value)
	{
		return (m_nSize > 0) ? IndexOf(value, 0, m_nSize) : -1;
	}
	int     IndexOf(const TYPE& value, int iStart)
	{
		return IndexOf(value, iStart, m_nSize - iStart);
	}
	int     IndexOf(const TYPE& value, int nIndex, int nNumElements);

	int     LastIndexOf(const TYPE& value)
	{
		return (m_nSize > 0) ? LastIndexOf(value, m_nSize - 1, m_nSize) : -1;
	}
	int     LastIndexOf(const TYPE& value, int nIndex)
	{
		return LastIndexOf(value, nIndex, nIndex + 1);
	}
	int     LastIndexOf(const TYPE& value, int nIndex, int nNumElements);

	HRESULT Remove(int nIndex);
	void    RemoveAll()
	{
		SetSize(0);
	}
	void    Reset()
	{
		m_nSize = 0;
	}

protected:
	TYPE* m_pData;      // the actual array of data
	int m_nSize;        // # of elements (upperBound - 1)
	int m_nMaxSize;     // max allocated

	HRESULT SetSizeInternal(int nNewMaxSize);  // This version doesn't call ctor or dtor.
};
//--------------------------------------------------------------------------------------
// Implementation of CGrowableArray
//--------------------------------------------------------------------------------------

// This version doesn't call ctor or dtor.
template<typename TYPE> HRESULT CGrowableArray <TYPE>::SetSizeInternal(int nNewMaxSize)
{
	if (nNewMaxSize < 0 || (nNewMaxSize > INT_MAX / sizeof(TYPE)))
	{
		assert(false);
		return E_INVALIDARG;
	}

	if (nNewMaxSize == 0)
	{
		// Shrink to 0 size & cleanup
		if (m_pData)
		{
			free(m_pData);
			m_pData = NULL;
		}

		m_nMaxSize = 0;
		m_nSize = 0;
	}
	else if (m_pData == NULL || nNewMaxSize > m_nMaxSize)
	{
		// Grow array
		int nGrowBy = (m_nMaxSize == 0) ? 16 : m_nMaxSize;

		// Limit nGrowBy to keep m_nMaxSize less than INT_MAX
		if ((UINT)m_nMaxSize + (UINT)nGrowBy > (UINT)INT_MAX)
			nGrowBy = INT_MAX - m_nMaxSize;

		nNewMaxSize = __max(nNewMaxSize, m_nMaxSize + nGrowBy);

		// Verify that (nNewMaxSize * sizeof(TYPE)) is not greater than UINT_MAX or the realloc will overrun
		if (sizeof(TYPE) > UINT_MAX / (UINT)nNewMaxSize)
			return E_INVALIDARG;

		TYPE* pDataNew = (TYPE*)realloc(m_pData, nNewMaxSize * sizeof(TYPE));
		if (pDataNew == NULL)
			return E_OUTOFMEMORY;

		m_pData = pDataNew;
		m_nMaxSize = nNewMaxSize;
	}

	return S_OK;
}

//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::SetSize(int nNewMaxSize)
{
	int nOldSize = m_nSize;

	if (nOldSize > nNewMaxSize)
	{
		assert(m_pData);
		if (m_pData)
		{
			// Removing elements. Call dtor.

			for (int i = nNewMaxSize; i < nOldSize; ++i)
				m_pData[i].~TYPE();
		}
	}

	// Adjust buffer.  Note that there's no need to check for error
	// since if it happens, nOldSize == nNewMaxSize will be true.)
	HRESULT hr = SetSizeInternal(nNewMaxSize);

	if (nOldSize < nNewMaxSize)
	{
		assert(m_pData);
		if (m_pData)
		{
			// Adding elements. Call ctor.

			for (int i = nOldSize; i < nNewMaxSize; ++i)
				::new (&m_pData[i]) TYPE;
		}
	}

	return hr;
}

//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::Add(const TYPE& value)
{
	HRESULT hr;
	if (FAILED(hr = SetSizeInternal(m_nSize + 1)))
		return hr;

	assert(m_pData != NULL);

	// Construct the new element
	::new (&m_pData[m_nSize]) TYPE;

	// Assign
	m_pData[m_nSize] = value;
	++m_nSize;

	return S_OK;
}

//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::Insert(int nIndex, const TYPE& value)
{
	HRESULT hr;

	// Validate index
	if (nIndex < 0 ||
		nIndex > m_nSize)
	{
		assert(false);
		return E_INVALIDARG;
	}

	// Prepare the buffer
	if (FAILED(hr = SetSizeInternal(m_nSize + 1)))
		return hr;

	// Shift the array
	MoveMemory(&m_pData[nIndex + 1], &m_pData[nIndex], sizeof(TYPE) * (m_nSize - nIndex));

	// Construct the new element
	::new (&m_pData[nIndex]) TYPE;

	// Set the value and increase the size
	m_pData[nIndex] = value;
	++m_nSize;

	return S_OK;
}

//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::SetAt(int nIndex, const TYPE& value)
{
	// Validate arguments
	if (nIndex < 0 ||
		nIndex >= m_nSize)
	{
		assert(false);
		return E_INVALIDARG;
	}

	m_pData[nIndex] = value;
	return S_OK;
}

//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the first occurrence
// within the section of the data array that extends from iStart and contains the 
// specified number of elements. Returns -1 if value is not found within the given 
// section.
//--------------------------------------------------------------------------------------
template<typename TYPE> int CGrowableArray <TYPE>::IndexOf(const TYPE& value, int iStart, int nNumElements)
{
	// Validate arguments
	if (iStart < 0 ||
		iStart >= m_nSize ||
		nNumElements < 0 ||
		iStart + nNumElements > m_nSize)
	{
		assert(false);
		return -1;
	}

	// Search
	for (int i = iStart; i < (iStart + nNumElements); i++)
	{
		if (value == m_pData[i])
			return i;
	}

	// Not found
	return -1;
}

//--------------------------------------------------------------------------------------
// Searches for the specified value and returns the index of the last occurrence
// within the section of the data array that contains the specified number of elements
// and ends at iEnd. Returns -1 if value is not found within the given section.
//--------------------------------------------------------------------------------------
template<typename TYPE> int CGrowableArray <TYPE>::LastIndexOf(const TYPE& value, int iEnd, int nNumElements)
{
	// Validate arguments
	if (iEnd < 0 ||
		iEnd >= m_nSize ||
		nNumElements < 0 ||
		iEnd - nNumElements < 0)
	{
		assert(false);
		return -1;
	}

	// Search
	for (int i = iEnd; i >(iEnd - nNumElements); i--)
	{
		if (value == m_pData[i])
			return i;
	}

	// Not found
	return -1;
}

//--------------------------------------------------------------------------------------
template<typename TYPE> HRESULT CGrowableArray <TYPE>::Remove(int nIndex)
{
	if (nIndex < 0 ||
		nIndex >= m_nSize)
	{
		assert(false);
		return E_INVALIDARG;
	}

	// Destruct the element to be removed
	m_pData[nIndex].~TYPE();

	// Compact the array and decrease the size
	MoveMemory(&m_pData[nIndex], &m_pData[nIndex + 1], sizeof(TYPE) * (m_nSize - (nIndex + 1)));
	--m_nSize;

	return S_OK;
}

// Vertex format
struct VERTEX
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 texcoord;

	static DWORD FVF;
};


// Used for a hashtable vertex cache when creating the mesh from a .obj file
struct CacheEntry
{
	UINT index;
	CacheEntry* pNext;
};


// Material properties per mesh subset
struct Material
{
	WCHAR   strName[MAX_PATH];

	D3DXVECTOR3 vAmbient;
	D3DXVECTOR3 vDiffuse;
	D3DXVECTOR3 vSpecular;

	int nShininess;
	float fAlpha;

	bool bSpecular;

	WCHAR   strTexture[MAX_PATH];
	IDirect3DTexture9* pTexture;
	D3DXHANDLE hTechnique;
};


class CMeshLoader
{
public:
	CMeshLoader();
	~CMeshLoader();

	HRESULT Create(IDirect3DDevice9* pd3dDevice, const WCHAR* strFilename);
	void    Destroy();


	UINT    GetNumMaterials() const
	{
		return m_Materials.GetSize();
	}
	Material* GetMaterial(UINT iMaterial)
	{
		return m_Materials.GetAt(iMaterial);
	}

	ID3DXMesh* GetMesh()
	{
		return m_pMesh;
	}
	WCHAR* GetMediaDirectory()
	{
		return m_strMediaDir;
	}
private:

	HRESULT LoadGeometryFromOBJ(const WCHAR* strFilename);
	HRESULT LoadMaterialsFromMTL(const WCHAR* strFileName);
	void    InitMaterial(Material* pMaterial);

	DWORD   AddVertex(UINT hash, VERTEX* pVertex);
	void    DeleteCache();

	IDirect3DDevice9* m_pd3dDevice;    // Direct3D Device object associated with this mesh
	ID3DXMesh* m_pMesh;         // Encapsulated D3DX Mesh

	CGrowableArray <CacheEntry*> m_VertexCache;   // Hashtable cache for locating duplicate vertices
	CGrowableArray <VERTEX> m_Vertices;      // Filled and copied to the vertex buffer
	CGrowableArray <DWORD> m_Indices;       // Filled and copied to the index buffer
	CGrowableArray <DWORD> m_Attributes;    // Filled and copied to the attribute buffer
	CGrowableArray <Material*> m_Materials;     // Holds material properties per subset

	WCHAR   m_strMediaDir[MAX_PATH];               // Directory where the mesh was found
};

#endif // _MESHLOADER_H_