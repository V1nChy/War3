#ifndef __GUIPICBOX
#define __GUIPICBOX

#include "MyGUIElementBase.h"
#include "MyVertexformat.h"
#include "My2DTexture.h"

/** ͼƬ�� */
class MyGUIImage: public MyGUIElementBase
{
private:
	void FinalCleanUp();
protected:
	My2DTexture* m_pTexture;
	MyColor m_cDiffuse;
	My_TLVERTEX m_vVertex[4];
	float uLeftTop;
	float vLeftTop;
	float uRightButtom;
	float vRightButtom;

public:
	MyGUIImage(MyGUIElementBase * parent, int id, const string& name, const string& src = "Texture/white.png",
		bool bEnableColorKey = false, const MyColor& colorkey = MyColor::NoColor,
		int offsetX = 0, int offsetY = 0, int width = 100, int height = 100,
		const MyColor& diffuse = MyColor::White, bool visible = true);

	/** ����Texture��Դ */
	void ResetImage(const string& imageFile, const MyColor& colorKey=MyColor::NoColor);

	void SetUVMap(float uLeftUp, float vLeftUp, float uRightDown, float vRightDown);
	void SetUVMap(float uLeftUp, float vLeftUp, float uRightUp, float vRightUp, 
		float uLeftDown, float vLeftDown, float uRightDown, float vRightDown);
	
	virtual ~MyGUIImage();

	inline const MyColor& GetDiffuse() const
	{	return m_cDiffuse;	}
	

	void SetDiffuse(const MyColor& color);
	void SetAlpha(float fAlpha);

	virtual void SetPosition(int offsetformparentx, int offsetformparenty);
	virtual void SetSize(int width, int height);
	virtual void SetSize();
	virtual void Move(int offsetx, int offsety);

	/** ˳ʱ����ת����Ϊ������ʱ����ת����Ϊ�� */
	virtual void RotateInRadian(float fRadian);
	virtual void RotateInDegree(float fDegree);

	virtual HRESULT Render();
};


#endif
