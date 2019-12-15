// GEDeviceSetting.h: interface for the GEDeviceSetting class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GEDEVICESETTING_H__E3159A1D_701D_4F70_A622_97DC01417209__INCLUDED_)
#define AFX_GEDEVICESETTING_H__E3159A1D_701D_4F70_A622_97DC01417209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GEDeviceEnumerator.h"

class GEDeviceSetting  
{
public:
    bool IsWindowed;

    D3DAdapterInfo* pWindowed_AdapterInfo;
    D3DDeviceInfo* pWindowed_DeviceInfo;
    D3DDeviceCombo* pWindowed_DeviceCombo;

    D3DDISPLAYMODE Windowed_DisplayMode; // not changable by the user
    D3DFORMAT Windowed_DepthStencilBufferFormat;
    D3DMULTISAMPLE_TYPE Windowed_MultisampleType;
    DWORD Windowed_MultisampleQuality;
    VertexProcessingType Windowed_VertexProcessingType;
    UINT Windowed_PresentInterval;
    int Windowed_Width;
    int Windowed_Height;

    D3DAdapterInfo* pFullscreen_AdapterInfo;
    D3DDeviceInfo* pFullscreen_DeviceInfo;
    D3DDeviceCombo* pFullscreen_DeviceCombo;

    D3DDISPLAYMODE Fullscreen_DisplayMode; // changable by the user
    D3DFORMAT Fullscreen_DepthStencilBufferFormat;
    D3DMULTISAMPLE_TYPE Fullscreen_MultisampleType;
    DWORD Fullscreen_MultisampleQuality;
    VertexProcessingType Fullscreen_VertexProcessingType;
    UINT Fullscreen_PresentInterval;

    D3DAdapterInfo* PAdapterInfo() const { return IsWindowed ? pWindowed_AdapterInfo : pFullscreen_AdapterInfo; }
    D3DDeviceInfo* PDeviceInfo() const { return IsWindowed ? pWindowed_DeviceInfo : pFullscreen_DeviceInfo; }
    D3DDeviceCombo* PDeviceCombo() const { return IsWindowed ? pWindowed_DeviceCombo : pFullscreen_DeviceCombo; }

    int AdapterOrdinal() const { return PDeviceCombo()->AdapterOrdinal; }
    D3DDEVTYPE DevType() const { return PDeviceCombo()->DevType; }
    D3DFORMAT BackBufferFormat() const { return PDeviceCombo()->BackBufferFormat; }

    D3DDISPLAYMODE DisplayMode() const { return IsWindowed ? Windowed_DisplayMode : Fullscreen_DisplayMode; }
    void SetDisplayMode(D3DDISPLAYMODE value) { if (IsWindowed) Windowed_DisplayMode = value; else Fullscreen_DisplayMode = value; }

    D3DFORMAT DepthStencilBufferFormat() const { return IsWindowed ? Windowed_DepthStencilBufferFormat : Fullscreen_DepthStencilBufferFormat; }
    void SetDepthStencilBufferFormat(D3DFORMAT value) { if (IsWindowed) Windowed_DepthStencilBufferFormat = value; else Fullscreen_DepthStencilBufferFormat = value; }

    D3DMULTISAMPLE_TYPE MultisampleType() const { return IsWindowed ? Windowed_MultisampleType : Fullscreen_MultisampleType; }
    void SetMultisampleType(D3DMULTISAMPLE_TYPE value) { if (IsWindowed) Windowed_MultisampleType = value; else Fullscreen_MultisampleType = value; }

    DWORD MultisampleQuality() const { return IsWindowed ? Windowed_MultisampleQuality : Fullscreen_MultisampleQuality; }
    void SetMultisampleQuality(DWORD value) { if (IsWindowed) Windowed_MultisampleQuality = value; else Fullscreen_MultisampleQuality = value; }

    VertexProcessingType GetVertexProcessingType() const { return IsWindowed ? Windowed_VertexProcessingType : Fullscreen_VertexProcessingType; }
    void SetVertexProcessingType(VertexProcessingType value) { if (IsWindowed) Windowed_VertexProcessingType = value; else Fullscreen_VertexProcessingType = value; }

    UINT PresentInterval() const { return IsWindowed ? Windowed_PresentInterval : Fullscreen_PresentInterval; }
    void SetPresentInterval(UINT value) { if (IsWindowed) Windowed_PresentInterval = value; else Fullscreen_PresentInterval = value; }
};



/*
//-----------------------------------------------------------------------------
// Name: class CD3DSettingsDialog
// Desc: Dialog box to allow the user to change the D3D settings
//-----------------------------------------------------------------------------
class GED3DSettingsDialog
{
private:
    HWND m_hDlg;
    GEDeviceEnumerator* m_pEnumeration;
    GEDeviceSetting m_d3dSettings;

private:
    // ComboBox helper functions
    void ComboBoxAdd( int id, void* pData, TCHAR* pstrDesc );
    void ComboBoxSelect( int id, void* pData );
    void* ComboBoxSelected( int id );
    bool ComboBoxSomethingSelected( int id );
    UINT ComboBoxCount( int id );
    void ComboBoxSelectIndex( int id, int index );
    void ComboBoxClear( int id );
    bool ComboBoxContainsText( int id, TCHAR* pstrText );

    void AdapterChanged( void );
    void DeviceChanged( void );
    void WindowedFullscreenChanged( void );
    void AdapterFormatChanged( void );
    void ResolutionChanged( void );
    void RefreshRateChanged( void );
    void BackBufferFormatChanged( void );
    void DepthStencilBufferFormatChanged( void );
    void MultisampleTypeChanged( void );
    void MultisampleQualityChanged( void );
    void VertexProcessingChanged( void );
    void PresentIntervalChanged( void );

public:
    GED3DSettingsDialog( GEDeviceEnumerator* pEnumeration, GEDeviceSetting* pSettings);
    INT_PTR ShowDialog( HWND hwndParent );
    INT_PTR DialogProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
    void GetFinalSettings( GEDeviceSetting* pSettings ) { *pSettings = m_d3dSettings; }
};


*/


#endif // !defined(AFX_GEDEVICESETTING_H__E3159A1D_701D_4F70_A622_97DC01417209__INCLUDED_)
