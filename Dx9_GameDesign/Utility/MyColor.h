#ifndef AFX_GECOLOR_H__39A2C0F1_6A32_45A2_A53B_CA5C86936FE7__INCLUDED_
#define AFX_GECOLOR_H__39A2C0F1_6A32_45A2_A53B_CA5C86936FE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <assert.h>
#include <d3d9.h>
#include <d3d9types.h>

typedef unsigned int RGBA;
typedef unsigned int ARGB;


/** ÑÕÉ«Àà */
class MyColor
{
public:
	static MyColor Black;
	static MyColor White;
	static MyColor Red;
	static MyColor Blue;
	static MyColor Green;
	static MyColor Pink;
	static MyColor Yellow;
	static MyColor NoColor;	///< 0x00000000	

	union {
		struct {
			float r, g, b, a;
		};
		float val[4];
	};

	MyColor(float red = 1.0f,
		float green = 1.0f,
		float blue = 1.0f,
		float alpha = 1.0f) : r(red), g(green), b(blue), a(alpha)
	{ }
	virtual ~MyColor() {}

	bool operator==(const MyColor& rhs) const;
	bool operator!=(const MyColor& rhs) const;

	/** Retrieves colour as RGBA.	input 0~255*/
	static MyColor GetColor(int red, int green, int blue, int alpha);

	/** Retrieves colour as RGBA.	*/
	RGBA GetAsRGBA(void) const;

	/** Retrieves colour as ARGB.	*/
	ARGB GetAsARGB(void) const;

	/** Sets colour as RGBA.	    */
	void SetAsRGBA(const RGBA val);

	/** Sets colour as ARGB.	    */
	void SetAsARGB(const ARGB val);

	D3DCOLORVALUE GetAsD3DCOLOR();

	void SetASD3DCOLOR(D3DCOLORVALUE color);


	inline void operator = (const MyColor& color)
	{
		r = color.r;
		g = color.g;
		b = color.b;
		a = color.a;
	}

	// arithmetic operations
	inline MyColor operator + (const MyColor& rkVector) const
	{
		MyColor kSum;

		kSum.r = r + rkVector.r;
		kSum.g = g + rkVector.g;
		kSum.b = b + rkVector.b;
		kSum.a = a + rkVector.a;

		return kSum;
	}

	inline MyColor operator - (const MyColor& rkVector) const
	{
		MyColor kDiff;

		kDiff.r = r - rkVector.r;
		kDiff.g = g - rkVector.g;
		kDiff.b = b - rkVector.b;
		kDiff.a = a - rkVector.a;

		return kDiff;
	}

	inline MyColor operator * (const float fScalar) const
	{
		MyColor kProd;

		kProd.r = fScalar*r;
		kProd.g = fScalar*g;
		kProd.b = fScalar*b;
		kProd.a = fScalar*a;

		return kProd;
	}

	inline MyColor operator * (const MyColor& rhs) const
	{
		MyColor kProd;

		kProd.r = rhs.r * r;
		kProd.g = rhs.g * g;
		kProd.b = rhs.b * b;
		kProd.a = rhs.a * a;

		return kProd;
	}

	inline MyColor operator / (const MyColor& rhs) const
	{
		MyColor kProd;

		kProd.r = rhs.r / r;
		kProd.g = rhs.g / g;
		kProd.b = rhs.b / b;
		kProd.a = rhs.a / a;

		return kProd;
	}

	inline MyColor operator / (const float fScalar) const
	{
		assert(fScalar != 0.0);

		MyColor kDiv;

		float fInv = 1.0f / fScalar;
		kDiv.r = r * fInv;
		kDiv.g = g * fInv;
		kDiv.b = b * fInv;
		kDiv.a = a * fInv;

		return kDiv;
	}

	inline friend MyColor operator * (const float fScalar, const MyColor& rkVector)
	{
		MyColor kProd;

		kProd.r = fScalar * rkVector.r;
		kProd.g = fScalar * rkVector.g;
		kProd.b = fScalar * rkVector.b;
		kProd.a = fScalar * rkVector.a;

		return kProd;
	}

	// arithmetic updates
	inline MyColor& operator += (const MyColor& rkVector)
	{
		r += rkVector.r;
		g += rkVector.g;
		b += rkVector.b;
		a += rkVector.a;

		return *this;
	}

	inline MyColor& operator -= (const MyColor& rkVector)
	{
		r -= rkVector.r;
		g -= rkVector.g;
		b -= rkVector.b;
		a -= rkVector.a;

		return *this;
	}

	inline MyColor& operator *= (const float fScalar)
	{
		r *= fScalar;
		g *= fScalar;
		b *= fScalar;
		a *= fScalar;
		return *this;
	}

	inline MyColor& operator /= (const float fScalar)
	{
		assert(fScalar != 0.0);

		float fInv = 1.0f / fScalar;

		r *= fInv;
		g *= fInv;
		b *= fInv;
		a *= fInv;

		return *this;
	}

};

#endif // !defined(AFX_MyColor_H__39A2C0F1_6A32_45A2_A53B_CA5C86936FE7__INCLUDED_)
