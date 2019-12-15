#include "MyColor.h"


typedef unsigned char uint8;
typedef unsigned int  uint32;

MyColor MyColor::Black = MyColor(0.0,0.0,0.0);
MyColor MyColor::White = MyColor(1.0,1.0,1.0);
MyColor MyColor::Red = MyColor(1.0,0.0,0.0);
MyColor MyColor::Green = MyColor(0.0,1.0,0.0);
MyColor MyColor::Blue = MyColor(0.0,0.0,1.0);
MyColor MyColor::Pink = MyColor(1.0, 0.0, 1.0);
MyColor MyColor::Yellow = MyColor(1.0, 1.0, 0.0);
MyColor MyColor::NoColor = MyColor(0.0,0.0,0.0,0.0);

//---------------------------------------------------------------------
RGBA MyColor::GetAsRGBA(void) const
{
	uint8 val8;
	uint32 val32 = 0;

	// Convert to 32bit pattern
	// (RGBA = 8888)

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 = val8 << 24;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 16;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8 << 8;

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 += val8;

	return val32;
}
//---------------------------------------------------------------------
ARGB MyColor::GetAsARGB(void) const
{
	uint8 val8;
	uint32 val32 = 0;

	// Convert to 32bit pattern
	// (ARGB = 8888)

	// Alpha
	val8 = static_cast<uint8>(a * 255);
	val32 = val8 << 24;

	// Red
	val8 = static_cast<uint8>(r * 255);
	val32 += val8 << 16;

	// Green
	val8 = static_cast<uint8>(g * 255);
	val32 += val8 << 8;

	// Blue
	val8 = static_cast<uint8>(b * 255);
	val32 += val8;


	return val32;
}

//---------------------------------------------------------------------
void MyColor::SetAsRGBA(const RGBA val)
{
	uint32 val32 = val;

	// Convert from 32bit pattern
	// (RGBA = 8888)

	// Red
	r = static_cast<uint8>(val32 >> 24) / 255.0f;

	// Green
	g = static_cast<uint8>(val32 >> 16) / 255.0f;

	// Blue
	b = static_cast<uint8>(val32 >> 8) / 255.0f;

	// Alpha
	a = static_cast<uint8>(val32) / 255.0f;
}

//---------------------------------------------------------------------
void MyColor::SetAsARGB(const ARGB val)
{
	uint32 val32 = val;

	// Convert from 32bit pattern
	// (ARGB = 8888)

	// Alpha
	a = static_cast<uint8>(val32 >> 24) / 255.0f;

	// Red
	r = static_cast<uint8>(val32 >> 16) / 255.0f;

	// Green
	g = static_cast<uint8>(val32 >> 8) / 255.0f;

	// Blue
	b = static_cast<uint8>(val32) / 255.0f;
}

//---------------------------------------------------------------------
bool MyColor::operator==(const MyColor& rhs) const
{
	return (r == rhs.r &&
	    g == rhs.g &&
	    b == rhs.b &&
	    a == rhs.a);
}

//---------------------------------------------------------------------
bool MyColor::operator!=(const MyColor& rhs) const
{
	return !(*this == rhs);
}



/** Retrieves colour as RGBA.	input 0~255*/
MyColor MyColor::GetColor(int red, int green, int blue, int alpha)
{
	float s = 1.0f/255;
	MyColor color(s*red, s*green, s*blue, s*alpha);
	return color;
}


D3DCOLORVALUE MyColor::GetAsD3DCOLOR()
{
	D3DCOLORVALUE color;
	color.r = r;
	color.g = g;
	color.b = b;
	color.a = a;
	return color;
}

void MyColor::SetASD3DCOLOR(D3DCOLORVALUE color)
{
	r = color.r;
	g = color.g;
	b = color.b;
	a = color.a;	
}

