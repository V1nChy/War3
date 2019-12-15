#include <iomanip>
#include "MyGUIImage.h"
#include "DxGraphicLayer.h"
#include "MyTextureManager.h"
#include "MyUtil.h"
MyGUIImage::MyGUIImage(MyGUIElementBase * parent, int id, const string& name, const string& src,
						bool bEnableColorKey, const MyColor& colorkey,
						int offsetX, int offsetY, int width, int height,
						const MyColor& diffuse, bool visible)
				:MyGUIElementBase(parent, id, name, offsetX, offsetY, width, height, false, visible), 
				m_pTexture(NULL)
{
	m_eType = GUI_Image;

	m_cDiffuse = diffuse;

	uLeftTop = 0;
	vLeftTop = 0;
	uRightButtom = 1;
	vRightButtom = 1;

	if (!src.empty())
	{
		m_pTexture = new My2DTexture;
		if(bEnableColorKey)
			m_pTexture->SetColorKey(colorkey);
		m_pTexture = (My2DTexture*)(MyTextureManager::GetSingleton()->CreateResource(src, m_pTexture));
	}

	// 左上, 左下, 右上, 右下
	m_vVertex[0].Init(Vector3(m_Position.x, m_Position.y+m_Size.y, 0), 1, diffuse, 0, 1);
	m_vVertex[1].Init(Vector3(m_Position.x, m_Position.y, 0), 1, diffuse, 0, 0);
	m_vVertex[2].Init(Vector3(m_Position.x+m_Size.x, m_Position.y+m_Size.y, 0), 1, diffuse, 1, 1);
	m_vVertex[3].Init(Vector3(m_Position.x+m_Size.x, m_Position.y, 0), 1, diffuse, 1, 0);
}

MyGUIImage::~MyGUIImage()
{
	FinalCleanUp();
}

void MyGUIImage::ResetImage(const string& imageFile, const MyColor& colorKey)
{
	if (m_pTexture != NULL)
	{
		if (m_pTexture->GetOrigin() != imageFile)
		{
			if (!MyTextureManager::GetSingleton()->GetResource(imageFile))
			{
				My2DTexture * texture = new My2DTexture;
				texture->SetColorKey(colorKey);

				if ((texture = dynamic_cast<My2DTexture*>(MyTextureManager::GetSingleton()->CreateResource(imageFile, texture))) != NULL)
				{
					// 正确的路径
					MyTextureManager::GetSingleton()->RemoveResource(m_pTexture->GetHandle());
					m_pTexture = texture;
				}
			}
			else
			{
				m_pTexture = (My2DTexture*)MyTextureManager::GetSingleton()->GetResource(imageFile);
			}
		}
	}
	else
	{
		m_pTexture = new My2DTexture;
		m_pTexture->SetColorKey(colorKey);
		m_pTexture = (My2DTexture*)(MyTextureManager::GetSingleton()->CreateResource(imageFile, new My2DTexture));
	}
}

HRESULT MyGUIImage::Render()
{
	if (m_bVisible)
	{
		if (m_pTexture != NULL)
		{
			DxGraphicLayer::GetSingleton()->SetFVF(My_TLVERTEX::FVF);
			DxGraphicLayer::GetSingleton()->SetTexture(0, m_pTexture);
			return DxGraphicLayer::GetSingleton()->DrawPrimitiveUp(D3DPT_TRIANGLESTRIP, 2, m_vVertex, sizeof(My_TLVERTEX));
			/*m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
			m_pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			m_pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			m_pDevice->SetFVF(My_TLVERTEX::FVF);
			m_pDevice->SetTexture(0, m_pTexture);
			m_pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, m_vVertex, sizeof(My_TLVERTEX));
			m_pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);*/
		}
	}
	
	return S_FALSE;
}

void MyGUIImage::SetPosition(int offsetFromParentX, int offsetFromParentY)
{
	int xchange = offsetFromParentX*m_eLocatType.sX - m_OffsetFromParent.x;
	int ychange = offsetFromParentY*m_eLocatType.sX - m_OffsetFromParent.y;

	MyGUIElementBase::SetPosition(offsetFromParentX, offsetFromParentY);

	m_vVertex[0].x += xchange;		m_vVertex[0].y += ychange;
	m_vVertex[1].x += xchange;		m_vVertex[1].y += ychange;
	m_vVertex[2].x += xchange;		m_vVertex[2].y += ychange;
	m_vVertex[3].x += xchange;		m_vVertex[3].y += ychange;
}

void MyGUIImage::Move(int offsetx, int offsety)
{
	Position oldPosition = m_Position;
	MyGUIElementBase::Move(offsetx, offsety);

	// 该步骤是关键
	Point2D<int> offset = m_Position - oldPosition;
	m_vVertex[0].x += offset.x;		m_vVertex[0].y += offset.y;
	m_vVertex[1].x += offset.x;		m_vVertex[1].y += offset.y;
	m_vVertex[2].x += offset.x;		m_vVertex[2].y += offset.y;
	m_vVertex[3].x += offset.x;		m_vVertex[3].y += offset.y;
}

void MyGUIImage::SetSize(int width, int height)
{
	MyGUIElementBase::SetSize(width, height);

	m_vVertex[0].x = m_Position.x;			m_vVertex[0].y = m_Position.y + m_Size.y;
	m_vVertex[1].x = m_Position.x;			m_vVertex[1].y = m_Position.y;
	m_vVertex[2].x = m_Position.x + m_Size.x;	m_vVertex[2].y = m_Position.y + m_Size.y;
	m_vVertex[3].x = m_Position.x + m_Size.x;	m_vVertex[3].y = m_Position.y;
	
}
void MyGUIImage::SetSize()
{
	float width = m_pTexture->GetWidth();
	float height = m_pTexture->GetHeight();
	MyGUIElementBase::SetSize(width, height);

	m_vVertex[0].x = m_Position.x;			m_vVertex[0].y = m_Position.y + height;
	m_vVertex[1].x = m_Position.x;			m_vVertex[1].y = m_Position.y;
	m_vVertex[2].x = m_Position.x + width;	m_vVertex[2].y = m_Position.y + height;
	m_vVertex[3].x = m_Position.x + width;	m_vVertex[3].y = m_Position.y;

}

void MyGUIImage::RotateInRadian(float fRadian)
{
	/*
		旋转矩阵：
			cos		-sin
			sin		cos
	*/	
	float fcosR = Angle::CosTable(fRadian);
	float fsinR = Angle::SinTable(fRadian);
	D3DXVECTOR2 pivot( (m_vVertex[0].x + m_vVertex[3].x) * 0.5f, (m_vVertex[0].y + m_vVertex[3].y) * 0.5f );
	
	D3DXVECTOR2 temp;
	temp.x = (m_vVertex[0].x - pivot.x) * fcosR + (m_vVertex[0].y - pivot.y) * fsinR + pivot.x;	
	temp.y = -(m_vVertex[0].x - pivot.x) * fsinR + (m_vVertex[0].y - pivot.y) * fcosR + pivot.y;		
	m_vVertex[0].x = temp.x;	m_vVertex[0].y = temp.y;

	temp.x = (m_vVertex[1].x - pivot.x) * fcosR + (m_vVertex[1].y - pivot.y) * fsinR + pivot.x;	
	temp.y = -(m_vVertex[1].x - pivot.x) * fsinR + (m_vVertex[1].y - pivot.y) * fcosR + pivot.y;		
	m_vVertex[1].x = temp.x;	m_vVertex[1].y = temp.y;

	temp.x = (m_vVertex[2].x - pivot.x) * fcosR + (m_vVertex[2].y - pivot.y) * fsinR + pivot.x;	
	temp.y = -(m_vVertex[2].x - pivot.x) * fsinR + (m_vVertex[2].y - pivot.y) * fcosR + pivot.y;	
	m_vVertex[2].x = temp.x;	m_vVertex[2].y = temp.y;

	temp.x = (m_vVertex[3].x - pivot.x) * fcosR + (m_vVertex[3].y - pivot.y) * fsinR + pivot.x;	
	temp.y = -(m_vVertex[3].x - pivot.x) * fsinR + (m_vVertex[3].y - pivot.y) * fcosR + pivot.y;		
	m_vVertex[3].x = temp.x;	m_vVertex[3].y = temp.y;
}

void MyGUIImage::RotateInDegree(float fDegree)
{
	return RotateInRadian(DEG_TO_RAD(fDegree));
}

void MyGUIImage::SetDiffuse(const MyColor& color)
{
	m_cDiffuse = color;
	m_vVertex[0].diffuse = color.GetAsARGB();
	m_vVertex[1].diffuse = color.GetAsARGB();
	m_vVertex[2].diffuse = color.GetAsARGB();
	m_vVertex[3].diffuse = color.GetAsARGB();
}

void MyGUIImage::SetAlpha(float fAlpha)
{
	if (fAlpha > 1)
		fAlpha = 1;
	else if (fAlpha == 0 )
		fAlpha = 0;

	m_cDiffuse.a = fAlpha;
	m_vVertex[0].diffuse = m_cDiffuse.GetAsARGB();
	m_vVertex[1].diffuse = m_cDiffuse.GetAsARGB();
	m_vVertex[2].diffuse = m_cDiffuse.GetAsARGB();
	m_vVertex[3].diffuse = m_cDiffuse.GetAsARGB();
}

void MyGUIImage::FinalCleanUp()
{
	
}

void MyGUIImage::SetUVMap(float uLeftUp, float vLeftUp, float uRightDown, float vRightDown)
{
	this->uLeftTop = uLeftUp;
	this->vLeftTop = vLeftUp;
	this->uRightButtom = uRightDown;
	this->vRightButtom = vRightDown;

	m_vVertex[0].tu = uLeftUp;
	m_vVertex[0].tv = vRightDown;
	m_vVertex[1].tu = uLeftUp;
	m_vVertex[1].tv = vLeftUp;
	m_vVertex[2].tu = uRightDown;
	m_vVertex[2].tv = vRightDown;
	m_vVertex[3].tu = uRightDown;
	m_vVertex[3].tv = vLeftUp;

}

void MyGUIImage::SetUVMap(float uLeftUp, float vLeftUp, float uRightUp, float vRightUp,
			  float uLeftDown, float vLeftDown, float uRightDown, float vRightDown)
{
	this->uLeftTop = uLeftUp;
	this->vLeftTop = vLeftUp;
	this->uRightButtom = uRightDown;
	this->vRightButtom = vRightDown;

	m_vVertex[0].tu = uLeftDown;
	m_vVertex[0].tv = vLeftDown;
	m_vVertex[1].tu = uLeftUp;
	m_vVertex[1].tv = vLeftUp;
	m_vVertex[2].tu = uRightDown;
	m_vVertex[2].tv = vRightDown;
	m_vVertex[3].tu = uRightUp;
	m_vVertex[3].tv = vRightUp;
}
