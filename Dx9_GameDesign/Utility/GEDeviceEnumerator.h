// GEDeviceEnumerator.h: interface for the GEDeviceEnumerator class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEDEVICEENUMERATOR_H__8BF84F85_3A4A_466C_BA35_E6AF2C1BFC29__INCLUDED_)
#define AFX_GEDEVICEENUMERATOR_H__8BF84F85_3A4A_466C_BA35_E6AF2C1BFC29__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <WINDOWS.H>
#include <d3d9.h>
#include <vector>
using namespace std;

struct D3DDeviceCombo;

/** 枚举顶点处理的格式 */
enum VertexProcessingType
{
    SOFTWARE_VP,
    MIXED_VP,
    HARDWARE_VP,
    PURE_HARDWARE_VP
};


/** D3D 设备的一些信息, including a list of D3DDeviceCombos (see below) that work with the device. */
struct D3DDeviceInfo
{
    int AdapterOrdinal;
    D3DDEVTYPE DevType;
    D3DCAPS9 Caps;
    vector<D3DDeviceCombo> DeviceComboList; // List of D3DDeviceCombo
};



/** 显卡的信息 */
struct D3DAdapterInfo
{
    int AdapterOrdinal;
    D3DADAPTER_IDENTIFIER9 AdapterIdentifier;
    vector<D3DDISPLAYMODE> DisplayModeList; // List of D3DDISPLAYMODEs
    vector<D3DDeviceInfo> DeviceInfoList; // List of D3DDeviceInfo
};


/** A depth/stencil buffer format that is incompatible with a multisample type */
struct D3DDSMSConflict
{
    D3DFORMAT DSFormat;
    D3DMULTISAMPLE_TYPE MSType;
};



/** A combination of adapter format, back buffer format, 
 * and windowed/fullscreen that is compatible with a particular D3D device (and the app). 
 */
struct D3DDeviceCombo
{
    int AdapterOrdinal;
    D3DDEVTYPE DevType;
    D3DFORMAT AdapterFormat;
    D3DFORMAT BackBufferFormat;
    bool IsWindowed;
    vector<D3DFORMAT> DepthStencilFormatList;			// List of D3DFORMATs
    vector<D3DMULTISAMPLE_TYPE> pMultiSampleTypeList;	// List of D3DMULTISAMPLE_TYPEs
    vector<DWORD> pMultiSampleQualityList;				// List of DWORDs (number of quality levels for each multisample type)
    vector<D3DDSMSConflict> pDSMSConflictList;			// List of D3DDSMSConflicts
    vector<VertexProcessingType> pVertexProcessingTypeList;	// List of VertexProcessingTypes
    vector<UINT> pPresentIntervalList;					// List of D3DPRESENT_INTERVALs
};


typedef bool(* CONFIRMDEVICECALLBACK)( D3DCAPS9* pCaps, VertexProcessingType vertexProcessingType, 
									   D3DFORMAT adapterFormat, D3DFORMAT backBufferFormat );


/** 枚举可用的D3D适配器,设备,模式等等 */
class GEDeviceEnumerator  
{
private:
    IDirect3D9* m_pD3D;

private:
    HRESULT EnumerateDevices( D3DAdapterInfo& pAdapterInfo, vector<D3DFORMAT>& pAdapterFormatList );
    HRESULT EnumerateDeviceCombos( D3DDeviceInfo& pDeviceInfo, vector<D3DFORMAT>& pAdapterFormatList );
    void BuildDepthStencilFormatList( D3DDeviceCombo& pDeviceCombo );
    void BuildMultiSampleTypeList( D3DDeviceCombo& pDeviceCombo );
    void BuildDSMSConflictList( D3DDeviceCombo& pDeviceCombo );
    void BuildVertexProcessingTypeList( D3DDeviceInfo& pDeviceInfo, D3DDeviceCombo& pDeviceCombo );
    void BuildPresentIntervalList( const D3DDeviceInfo& pDeviceInfo, D3DDeviceCombo& pDeviceCombo );

public:
    vector<D3DAdapterInfo> m_pAdapterInfoList;
    // The following variables can be used to limit what modes, formats, 
    // etc. are enumerated.  Set them to the values you want before calling
    // Enumerate().
    CONFIRMDEVICECALLBACK ConfirmDeviceCallback;
    UINT AppMinFullscreenWidth;
    UINT AppMinFullscreenHeight;
    UINT AppMinColorChannelBits; // min color bits per channel in adapter format
    UINT AppMinAlphaChannelBits; // min alpha bits per pixel in back buffer format
    UINT AppMinDepthBits;
    UINT AppMinStencilBits;
    bool AppUsesDepthBuffer;
    bool AppUsesMixedVP; // whether app can take advantage of mixed vp mode
    bool AppRequiresWindowed;
    bool AppRequiresFullscreen;
    vector<D3DFORMAT> m_pAllowedAdapterFormatList; // list of D3DFORMATs

    GEDeviceEnumerator();
    ~GEDeviceEnumerator();
    void SetD3D(IDirect3D9* pD3D) { m_pD3D = pD3D; }
    HRESULT Enumerate();
};

#endif // !defined(AFX_GEDEVICEENUMERATOR_H__8BF84F85_3A4A_466C_BA35_E6AF2C1BFC29__INCLUDED_)
