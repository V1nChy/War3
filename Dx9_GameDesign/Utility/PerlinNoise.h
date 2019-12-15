#pragma once
#include <math.h>
#include <stdlib.h>
#include <assert.h>
class CPerlinNoise2D
{
public:
	CPerlinNoise2D(void);
	~CPerlinNoise2D(void);

	struct vec2
	{
		float x,y;

		vec2(float _x, float _y):x(_x),y(_y){}; 
		vec2(){};
	};

	enum
	{
		k_tableSize = 256,
		k_tableMask = k_tableSize-1,
	};

	//柏林噪声...
	float Noise(int x, int y, float scale);
	float Noise(float x, float y, float scale);

private:

	vec2 m_vecTable[k_tableSize];			//梯度值表
	unsigned char m_lut[k_tableSize];		//伪随机索引表

	//初始化伪随机2d向量(梯度）
	void Setup();
	//获得梯度值
	const vec2& GetVec(int x, int y)const;
};
inline void CPerlinNoise2D::Setup()
{
	float step = 6.24f / k_tableSize;
	float val=0.0f;

	for (int i=0; i<k_tableSize; ++i)
	{
		m_vecTable[i].x = (float)sin(val);
		m_vecTable[i].y = (float)cos(val);
		val += step;

		m_lut[i] = rand() & k_tableMask;
	}
}

inline const CPerlinNoise2D::vec2& CPerlinNoise2D::GetVec(
	int x, 
	int y)const
{
	unsigned char a = m_lut[x&k_tableMask]; 
	unsigned char b = m_lut[y&k_tableMask]; 
	unsigned char val = m_lut[(a+b)&k_tableMask];

	assert( val<=k_tableMask && val>=0 );

	return m_vecTable[val];
}

inline float CPerlinNoise2D::Noise(
								 int x, 
								 int y, 
								 float scale)
{
	return Noise((float)x, (float)y, scale);
}
inline CPerlinNoise2D::CPerlinNoise2D()
{
	Setup();
}

inline CPerlinNoise2D::~CPerlinNoise2D()
{
}
