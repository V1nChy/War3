// GEDeviceEnumerator.cpp: implementation of the GEDeviceEnumerator class.
//
//////////////////////////////////////////////////////////////////////

#include "GEDeviceEnumerator.h"

//-----------------------------------------------------------------------------
// Name: ColorChannelBits
// Desc: Returns the number of color channel bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT ColorChannelBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_R8G8B8:
            return 8;
        case D3DFMT_A8R8G8B8:
            return 8;
        case D3DFMT_X8R8G8B8:
            return 8;
        case D3DFMT_R5G6B5:
            return 5;
        case D3DFMT_X1R5G5B5:
            return 5;
        case D3DFMT_A1R5G5B5:
            return 5;
        case D3DFMT_A4R4G4B4:
            return 4;
        case D3DFMT_R3G3B2:
            return 2;
        case D3DFMT_A8R3G3B2:
            return 2;
        case D3DFMT_X4R4G4B4:
            return 4;
        case D3DFMT_A2B10G10R10:
            return 10;
        case D3DFMT_A2R10G10B10:
            return 10;
        default:
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: AlphaChannelBits
// Desc: Returns the number of alpha channel bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT AlphaChannelBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_R8G8B8:
            return 0;
        case D3DFMT_A8R8G8B8:
            return 8;
        case D3DFMT_X8R8G8B8:
            return 0;
        case D3DFMT_R5G6B5:
            return 0;
        case D3DFMT_X1R5G5B5:
            return 0;
        case D3DFMT_A1R5G5B5:
            return 1;
        case D3DFMT_A4R4G4B4:
            return 4;
        case D3DFMT_R3G3B2:
            return 0;
        case D3DFMT_A8R3G3B2:
            return 8;
        case D3DFMT_X4R4G4B4:
            return 0;
        case D3DFMT_A2B10G10R10:
            return 2;
        case D3DFMT_A2R10G10B10:
            return 2;
        default:
            return 0;
    }
}




//-----------------------------------------------------------------------------
// Name: DepthBits
// Desc: Returns the number of depth bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT DepthBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_D16:
            return 16;
        case D3DFMT_D15S1:
            return 15;
        case D3DFMT_D24X8:
            return 24;
        case D3DFMT_D24S8:
            return 24;
        case D3DFMT_D24X4S4:
            return 24;
        case D3DFMT_D32:
            return 32;
        default:
            return 0;
    }
}




//-----------------------------------------------------------------------------
// Name: StencilBits
// Desc: Returns the number of stencil bits in the specified D3DFORMAT
//-----------------------------------------------------------------------------
static UINT StencilBits( D3DFORMAT fmt )
{
    switch( fmt )
    {
        case D3DFMT_D16:
            return 0;
        case D3DFMT_D15S1:
            return 1;
        case D3DFMT_D24X8:
            return 0;
        case D3DFMT_D24S8:
            return 8;
        case D3DFMT_D24X4S4:
            return 4;
        case D3DFMT_D32:
            return 0;
        default:
            return 0;
    }
}




//-----------------------------------------------------------------------------
// Name: CD3DEnumeration constructor
// Desc: 
//-----------------------------------------------------------------------------
GEDeviceEnumerator::GEDeviceEnumerator()
{
    AppMinFullscreenWidth = 640;
    AppMinFullscreenHeight = 480;
    AppMinColorChannelBits = 5;
    AppMinAlphaChannelBits = 0;
    AppMinDepthBits = 15;
    AppMinStencilBits = 1;
    AppUsesDepthBuffer = true;
    AppUsesMixedVP = false;
    AppRequiresWindowed = false;
    AppRequiresFullscreen = false;
}




//-----------------------------------------------------------------------------
// Name: CD3DEnumeration destructor
// Desc: 
//-----------------------------------------------------------------------------
GEDeviceEnumerator::~GEDeviceEnumerator()
{

}




//-----------------------------------------------------------------------------
// Name: SortModesCallback
// Desc: Used to sort D3DDISPLAYMODEs
//-----------------------------------------------------------------------------
static int __cdecl SortModesCallback( const void* arg1, const void* arg2 )
{
    D3DDISPLAYMODE* pdm1 = (D3DDISPLAYMODE*)arg1;
    D3DDISPLAYMODE* pdm2 = (D3DDISPLAYMODE*)arg2;

    if (pdm1->Width > pdm2->Width)
        return 1;
    if (pdm1->Width < pdm2->Width)
        return -1;
    if (pdm1->Height > pdm2->Height)
        return 1;
    if (pdm1->Height < pdm2->Height)
        return -1;
    if (pdm1->Format > pdm2->Format)
        return 1;
    if (pdm1->Format < pdm2->Format)
        return -1;
    if (pdm1->RefreshRate > pdm2->RefreshRate)
        return 1;
    if (pdm1->RefreshRate < pdm2->RefreshRate)
        return -1;
    return 0;
}




//-----------------------------------------------------------------------------
// Name: Enumerate
// Desc: Enumerates available D3D adapters, devices, modes, etc.
//-----------------------------------------------------------------------------
HRESULT GEDeviceEnumerator::Enumerate()
{
    HRESULT hr;
    vector<D3DFORMAT> adapterFormatList;

    if( m_pD3D == NULL )
        return E_FAIL;

    m_pAllowedAdapterFormatList.push_back(D3DFMT_X8R8G8B8);
    m_pAllowedAdapterFormatList.push_back(D3DFMT_X1R5G5B5);
    m_pAllowedAdapterFormatList.push_back(D3DFMT_R5G6B5);
    m_pAllowedAdapterFormatList.push_back(D3DFMT_A2R10G10B10);

    UINT numAdapters = m_pD3D->GetAdapterCount();

    for (UINT adapterOrdinal = 0; adapterOrdinal < numAdapters; ++adapterOrdinal)
    {
		D3DAdapterInfo pAdapterInfo;

        pAdapterInfo.AdapterOrdinal = adapterOrdinal;
        m_pD3D->GetAdapterIdentifier(adapterOrdinal, 0, &pAdapterInfo.AdapterIdentifier);

        // Get list of all display modes on this adapter.  
        // Also build a temporary list of all display adapter formats.
        adapterFormatList.clear();
        for( UINT iaaf = 0; iaaf < m_pAllowedAdapterFormatList.size(); iaaf++ )
        {
            D3DFORMAT allowedAdapterFormat = m_pAllowedAdapterFormatList[iaaf];
            UINT numAdapterModes = m_pD3D->GetAdapterModeCount( adapterOrdinal, allowedAdapterFormat );
            for (UINT mode = 0; mode < numAdapterModes; mode++)
            {
                D3DDISPLAYMODE displayMode;
                m_pD3D->EnumAdapterModes( adapterOrdinal, allowedAdapterFormat, mode, &displayMode );
                if( displayMode.Width < AppMinFullscreenWidth ||
                    displayMode.Height < AppMinFullscreenHeight ||
                    ColorChannelBits(displayMode.Format) < AppMinColorChannelBits )
                {
                    continue;
                }
                pAdapterInfo.DisplayModeList.push_back(displayMode);
				
				bool finded = false;
				for (size_t i = 0; i < adapterFormatList.size(); ++i)
				{
					if (adapterFormatList[i] == displayMode.Format)
					{
						finded = true;
						break;
					}
				}
                if( !finded )
                    adapterFormatList.push_back( displayMode.Format );
            }
        }

        // Sort displaymode list
        //qsort( pAdapterInfo.DisplayModeList.begin(), 
        //    pAdapterInfo.DisplayModeList.size(), sizeof( D3DDISPLAYMODE ),
        //    SortModesCallback );

        // Get info for each device on this adapter
        if( FAILED( hr = EnumerateDevices( pAdapterInfo, adapterFormatList ) ) )
            return hr;

        // If at least one device on this adapter is available and compatible
        // with the app, add the adapterInfo to the list
        if (!pAdapterInfo.DeviceInfoList.empty())
            m_pAdapterInfoList.push_back(pAdapterInfo);
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: EnumerateDevices
// Desc: Enumerates D3D devices for a particular adapter.
//-----------------------------------------------------------------------------
HRESULT GEDeviceEnumerator::EnumerateDevices( D3DAdapterInfo& pAdapterInfo, 
                                           vector<D3DFORMAT>& pAdapterFormatList )
{
    const D3DDEVTYPE devTypeArray[] = { D3DDEVTYPE_HAL, D3DDEVTYPE_SW, D3DDEVTYPE_REF };
    const UINT devTypeArrayCount = sizeof(devTypeArray) / sizeof(devTypeArray[0]);
    HRESULT hr;

    for( UINT idt = 0; idt < devTypeArrayCount; idt++ )
    {
		D3DDeviceInfo pDeviceInfo;

        pDeviceInfo.AdapterOrdinal = pAdapterInfo.AdapterOrdinal;
        pDeviceInfo.DevType = devTypeArray[idt];
        if( FAILED( m_pD3D->GetDeviceCaps( pAdapterInfo.AdapterOrdinal, 
            pDeviceInfo.DevType, &pDeviceInfo.Caps ) ) )
		{
            continue;
        }

        // Get info for each devicecombo on this device
        if( FAILED( hr = EnumerateDeviceCombos(pDeviceInfo, pAdapterFormatList) ) )
            return hr;

        // If at least one devicecombo for this device is found, 
        // add the deviceInfo to the list
        if (pDeviceInfo.DeviceComboList.empty())
            continue;

        pAdapterInfo.DeviceInfoList.push_back(pDeviceInfo);
    }
    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: EnumerateDeviceCombos
// Desc: Enumerates DeviceCombos for a particular device.
//-----------------------------------------------------------------------------
HRESULT GEDeviceEnumerator::EnumerateDeviceCombos( D3DDeviceInfo& pDeviceInfo, 
                                               vector<D3DFORMAT>& pAdapterFormatList )
{
    const D3DFORMAT backBufferFormatArray[] = 
        {   D3DFMT_A8R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_A2R10G10B10, 
            D3DFMT_R5G6B5, D3DFMT_A1R5G5B5, D3DFMT_X1R5G5B5 };
    const UINT backBufferFormatArrayCount = sizeof(backBufferFormatArray) / sizeof(backBufferFormatArray[0]);
    bool isWindowedArray[] = { false, true };

    // See which adapter formats are supported by this device
    D3DFORMAT adapterFormat;
    for( UINT iaf = 0; iaf < pAdapterFormatList.size(); iaf++ )
    {
        adapterFormat = pAdapterFormatList[iaf];
        D3DFORMAT backBufferFormat;
        for( UINT ibbf = 0; ibbf < backBufferFormatArrayCount; ibbf++ )
        {
            backBufferFormat = backBufferFormatArray[ibbf];
            if (AlphaChannelBits(backBufferFormat) < AppMinAlphaChannelBits)
                continue;
            bool isWindowed;
            for( UINT iiw = 0; iiw < 2; iiw++)
            {
                isWindowed = isWindowedArray[iiw];
                if (!isWindowed && AppRequiresWindowed)
                    continue;
                if (isWindowed && AppRequiresFullscreen)
                    continue;
                if (FAILED(m_pD3D->CheckDeviceType(pDeviceInfo.AdapterOrdinal, pDeviceInfo.DevType, 
                    adapterFormat, backBufferFormat, isWindowed)))
                {
                    continue;
                }
                // At this point, we have an adapter/device/adapterformat/backbufferformat/iswindowed
                // DeviceCombo that is supported by the system.  We still need to confirm that it's 
                // compatible with the app, and find one or more suitable depth/stencil buffer format,
                // multisample type, vertex processing type, and present interval.
                D3DDeviceCombo pDeviceCombo;

                pDeviceCombo.AdapterOrdinal = pDeviceInfo.AdapterOrdinal;
                pDeviceCombo.DevType = pDeviceInfo.DevType;
                pDeviceCombo.AdapterFormat = adapterFormat;
                pDeviceCombo.BackBufferFormat = backBufferFormat;
                pDeviceCombo.IsWindowed = isWindowed;
                if (AppUsesDepthBuffer)
                {
                    BuildDepthStencilFormatList(pDeviceCombo);
                    if (pDeviceCombo.DepthStencilFormatList.size() == 0)
                        continue;
                }
                BuildMultiSampleTypeList(pDeviceCombo);
                if (pDeviceCombo.pMultiSampleTypeList.size() == 0)
                    continue;
				
                BuildDSMSConflictList(pDeviceCombo);
                BuildVertexProcessingTypeList(pDeviceInfo, pDeviceCombo);
                if (pDeviceCombo.pVertexProcessingTypeList.empty())
                    continue;

                BuildPresentIntervalList(pDeviceInfo, pDeviceCombo);

                pDeviceInfo.DeviceComboList.push_back(pDeviceCombo);
            }
        }
    }

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: BuildDepthStencilFormatList
// Desc: Adds all depth/stencil formats that are compatible with the device 
//       and app to the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void GEDeviceEnumerator::BuildDepthStencilFormatList( D3DDeviceCombo& pDeviceCombo )
{
    const D3DFORMAT depthStencilFormatArray[] = 
    {
        D3DFMT_D16,
        D3DFMT_D15S1,
        D3DFMT_D24X8,
        D3DFMT_D24S8,
        D3DFMT_D24X4S4,
        D3DFMT_D32,
    };
    const UINT depthStencilFormatArrayCount = sizeof(depthStencilFormatArray) / 
                                              sizeof(depthStencilFormatArray[0]);

    D3DFORMAT depthStencilFmt;
    for( UINT idsf = 0; idsf < depthStencilFormatArrayCount; idsf++ )
    {
        depthStencilFmt = depthStencilFormatArray[idsf];
        if (DepthBits(depthStencilFmt) < AppMinDepthBits)
            continue;
        if (StencilBits(depthStencilFmt) < AppMinStencilBits)
            continue;
        if (SUCCEEDED(m_pD3D->CheckDeviceFormat(pDeviceCombo.AdapterOrdinal, 
            pDeviceCombo.DevType, pDeviceCombo.AdapterFormat, 
            D3DUSAGE_DEPTHSTENCIL, D3DRTYPE_SURFACE, depthStencilFmt)))
        {
            if (SUCCEEDED(m_pD3D->CheckDepthStencilMatch(pDeviceCombo.AdapterOrdinal, 
                pDeviceCombo.DevType, pDeviceCombo.AdapterFormat, 
                pDeviceCombo.BackBufferFormat, depthStencilFmt)))
            {
                pDeviceCombo.DepthStencilFormatList.push_back(depthStencilFmt);
            }
        }
    }
}




//-----------------------------------------------------------------------------
// Name: BuildMultiSampleTypeList
// Desc: Adds all multisample types that are compatible with the device and app to
//       the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void GEDeviceEnumerator::BuildMultiSampleTypeList( D3DDeviceCombo& pDeviceCombo )
{
    const D3DMULTISAMPLE_TYPE msTypeArray[] = { 
        D3DMULTISAMPLE_NONE,
        D3DMULTISAMPLE_NONMASKABLE,
        D3DMULTISAMPLE_2_SAMPLES,
        D3DMULTISAMPLE_3_SAMPLES,
        D3DMULTISAMPLE_4_SAMPLES,
        D3DMULTISAMPLE_5_SAMPLES,
        D3DMULTISAMPLE_6_SAMPLES,
        D3DMULTISAMPLE_7_SAMPLES,
        D3DMULTISAMPLE_8_SAMPLES,
        D3DMULTISAMPLE_9_SAMPLES,
        D3DMULTISAMPLE_10_SAMPLES,
        D3DMULTISAMPLE_11_SAMPLES,
        D3DMULTISAMPLE_12_SAMPLES,
        D3DMULTISAMPLE_13_SAMPLES,
        D3DMULTISAMPLE_14_SAMPLES,
        D3DMULTISAMPLE_15_SAMPLES,
        D3DMULTISAMPLE_16_SAMPLES,
    };
    const UINT msTypeArrayCount = sizeof(msTypeArray) / sizeof(msTypeArray[0]);

    D3DMULTISAMPLE_TYPE msType;
    DWORD msQuality;
    for( UINT imst = 0; imst < msTypeArrayCount; imst++ )
    {
        msType = msTypeArray[imst];
        if (SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(pDeviceCombo.AdapterOrdinal, pDeviceCombo.DevType, 
            pDeviceCombo.BackBufferFormat, pDeviceCombo.IsWindowed, msType, &msQuality)))
        {
            pDeviceCombo.pMultiSampleTypeList.push_back(msType);
            pDeviceCombo.pMultiSampleQualityList.push_back(msQuality);
        }
    }
}




//-----------------------------------------------------------------------------
// Name: BuildDSMSConflictList
// Desc: Find any conflicts between the available depth/stencil formats and
//       multisample types.
//-----------------------------------------------------------------------------
void GEDeviceEnumerator::BuildDSMSConflictList( D3DDeviceCombo& pDeviceCombo )
{
    D3DDSMSConflict DSMSConflict;

    for( UINT ids = 0; ids < pDeviceCombo.DepthStencilFormatList.size(); ids++ )
    {
        D3DFORMAT dsFmt = pDeviceCombo.DepthStencilFormatList[ids];
        for( UINT ims = 0; ims < pDeviceCombo.pMultiSampleTypeList.size(); ims++ )
        {
            D3DMULTISAMPLE_TYPE msType = pDeviceCombo.pMultiSampleTypeList[ims];
            if( FAILED( m_pD3D->CheckDeviceMultiSampleType( pDeviceCombo.AdapterOrdinal, pDeviceCombo.DevType,
                dsFmt, pDeviceCombo.IsWindowed, msType, NULL ) ) )
            {
                DSMSConflict.DSFormat = dsFmt;
                DSMSConflict.MSType = msType;
                pDeviceCombo.pDSMSConflictList.push_back(DSMSConflict);
            }
        }
    }
}




//-----------------------------------------------------------------------------
// Name: BuildVertexProcessingTypeList
// Desc: Adds all vertex processing types that are compatible with the device 
//       and app to the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void GEDeviceEnumerator::BuildVertexProcessingTypeList( D3DDeviceInfo& pDeviceInfo, 
                                                     D3DDeviceCombo& pDeviceCombo )
{
    VertexProcessingType vpt;
    if ((pDeviceInfo.Caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) != 0)
    {
        if ((pDeviceInfo.Caps.DevCaps & D3DDEVCAPS_PUREDEVICE) != 0)
        {
            if (ConfirmDeviceCallback == NULL ||
                ConfirmDeviceCallback(&pDeviceInfo.Caps, PURE_HARDWARE_VP, 
                pDeviceCombo.AdapterFormat, pDeviceCombo.BackBufferFormat))
            {
                vpt = PURE_HARDWARE_VP;
                pDeviceCombo.pVertexProcessingTypeList.push_back(vpt);
            }
        }
        if (ConfirmDeviceCallback == NULL ||
            ConfirmDeviceCallback(&pDeviceInfo.Caps, HARDWARE_VP, 
            pDeviceCombo.AdapterFormat, pDeviceCombo.BackBufferFormat))
        {
            vpt = HARDWARE_VP;
            pDeviceCombo.pVertexProcessingTypeList.push_back(vpt);
        }
        if (AppUsesMixedVP && (ConfirmDeviceCallback == NULL ||
            ConfirmDeviceCallback(&pDeviceInfo.Caps, MIXED_VP, 
            pDeviceCombo.AdapterFormat, pDeviceCombo.BackBufferFormat)))
        {
            vpt = MIXED_VP;
            pDeviceCombo.pVertexProcessingTypeList.push_back(vpt);
        }
    }
    if (ConfirmDeviceCallback == NULL ||
        ConfirmDeviceCallback(&pDeviceInfo.Caps, SOFTWARE_VP, 
        pDeviceCombo.AdapterFormat, pDeviceCombo.BackBufferFormat))
    {
        vpt = SOFTWARE_VP;
        pDeviceCombo.pVertexProcessingTypeList.push_back(vpt);
    }
}




//-----------------------------------------------------------------------------
// Name: BuildPresentIntervalList
// Desc: Adds all present intervals that are compatible with the device and app 
//       to the given D3DDeviceCombo.
//-----------------------------------------------------------------------------
void GEDeviceEnumerator::BuildPresentIntervalList( const D3DDeviceInfo& pDeviceInfo, 
                                                D3DDeviceCombo& pDeviceCombo )
{
    const UINT piArray[] = { 
        D3DPRESENT_INTERVAL_IMMEDIATE,
        D3DPRESENT_INTERVAL_DEFAULT,
        D3DPRESENT_INTERVAL_ONE,
        D3DPRESENT_INTERVAL_TWO,
        D3DPRESENT_INTERVAL_THREE,
        D3DPRESENT_INTERVAL_FOUR,
    };
    const UINT piArrayCount = sizeof(piArray) / sizeof(piArray[0]);

    UINT pi;
    for( UINT ipi = 0; ipi < piArrayCount; ipi++ )
    {
        pi = piArray[ipi];
        if( pDeviceCombo.IsWindowed )
        {
            if( pi == D3DPRESENT_INTERVAL_TWO ||
                pi == D3DPRESENT_INTERVAL_THREE ||
                pi == D3DPRESENT_INTERVAL_FOUR )
            {
                // These intervals are not supported in windowed mode.
                continue;
            }
        }
        // Note that D3DPRESENT_INTERVAL_DEFAULT is zero, so you
        // can't do a caps check for it -- it is always available.
        if( pi == D3DPRESENT_INTERVAL_DEFAULT ||
            (pDeviceInfo.Caps.PresentationIntervals & pi) )
        {
            pDeviceCombo.pPresentIntervalList.push_back(pi);
        }
    }
}
