// My2DTexture.cpp: implementation of the My2DTexture class.
//
//////////////////////////////////////////////////////////////////////

#include "My2DTexture.h"
#include "MyUtil.h"
#include "DxGraphicLayer.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

My2DTexture::My2DTexture()
{
	m_lpTexture = 0;
	m_eType = TEX_TYPE_2D;
}

My2DTexture::~My2DTexture()
{

}


//-----------------------------------------------------------------------------
// Name: LoadImpl()
// Desc: ��μ�����Դ,Ϊ���ػ��ຯ��
//-----------------------------------------------------------------------------
bool My2DTexture::LoadImpl(void)
{
	HRESULT hr;
	int temp = m_cColorKey.GetAsARGB();
	if (temp == 0)
		hr = CreateTextureFromFile();
	else
		hr = CreateTextureFromFileEx();

	if (hr == S_OK)
		return true;
	else 
		return false;
}

//-----------------------------------------------------------------------------
// Name: CreateTextureFromFileEx()
// Desc: Dx��ͼ����ͼ��Ϣ��ȡ
//-----------------------------------------------------------------------------
HRESULT My2DTexture::CreateTextureFromFile()
{

	if (FAILED(D3DXCreateTextureFromFile(DxGraphicLayer::GetSingleton()->GetD3DDevice(), m_strOrigin.c_str(), &m_lpTexture)))
	{
		MessageBox(NULL, "��������ʧ��", "Texture.exe", MB_OK);
		return E_FAIL;
	}

	D3DSURFACE_DESC sdesc;
	m_lpTexture->GetLevelDesc(0, &sdesc);
	m_uHeight = sdesc.Height;				// ����߶�			
	m_uWidth = sdesc.Width;				// ������
	m_eFormat = sdesc.Format;				// ͼƬ����ɫ��ʽ
	m_dwUsage = sdesc.Usage;				// ��Դ���÷�,λ���
	m_ePool = sdesc.Pool;					// �����ʽ

	return S_OK;
}
HRESULT My2DTexture::CreateTextureFromFileEx()
{
	if (FAILED(D3DXCreateTextureFromFileEx(DxGraphicLayer::GetSingleton()->GetD3DDevice(), m_strOrigin.c_str(),
		m_uHeight, m_uWidth, m_iNumMipmaps, m_dwUsage, m_eFormat, m_ePool, m_eFilter, m_eMipFilter, m_cColorKey.GetAsARGB(),
		NULL, NULL,&m_lpTexture)))
	{
		MessageBox(NULL, "��������ʧ��", "Texture.exe", MB_OK);
		return E_FAIL;
	}

	D3DSURFACE_DESC sdesc;
	m_lpTexture->GetLevelDesc(0, &sdesc);
	m_uHeight = sdesc.Height;				// ����߶�			
	m_uWidth = sdesc.Width;				// ������
	m_eFormat = sdesc.Format;				// ͼƬ����ɫ��ʽ
	m_dwUsage = sdesc.Usage;				// ��Դ���÷�,λ���
	m_ePool = sdesc.Pool;					// �����ʽ

	return S_OK;
}
//-----------------------------------------------------------------------------
// Name: UnloadImpl()
// Desc: ���ж����Դ,Ϊ���ػ��ຯ��
//-----------------------------------------------------------------------------
bool My2DTexture::UnloadImpl(void)
{
	try
	{
		if (m_lpTexture != NULL)
			SAFE_RELEASE(m_lpTexture);
		return true;
	}
	catch (...) {
		//GELog::GetSingleton()->AppendLog("unload fails", __FILE__, __LINE__);
		return false;
	}
}

LPDIRECT3DTEXTURE9 My2DTexture::GetTexture()
{
	if (!m_lpTexture)
		LOGINFO("No such texture");
	return m_lpTexture;
}

bool My2DTexture::GetRGBAPerPix(vector<MyColor>& vRGBAPixs) const
{
	if (!m_lpTexture)
		return false;

	vRGBAPixs.clear();

	D3DLOCKED_RECT pLocked_rect;
	m_lpTexture->LockRect(0, &pLocked_rect, NULL, D3DLOCK_READONLY);
	
	char* p = (char*)pLocked_rect.pBits;

	ofstream fout("test.txt");
	//peter: hard code �� bad
	for (unsigned long row = 0; row < m_uHeight; ++row)
	{
		for (unsigned long col = 0; col < m_uWidth; ++col) {
			fout << row << "\t" << col << "\t"; 
            fout << (int)*(p++) << "\t";
			fout << (int)*(p++) << "\t";
			fout << (int)*(p++) << "\t";
			fout << (int)*(p++) << endl << endl << endl;
		}
	}
	fout.close();
	
	m_lpTexture->UnlockRect(0);
	
	return true;
}
