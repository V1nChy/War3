#include ".\perlinnoise.h"
#include <Windows.h>
#include <string>
using namespace std;

float CPerlinNoise2D::Noise(
								   float x, 
								   float y,
								   float scale)
{
	vec2 pos(x*scale,y*scale);

	//找到此点的方格子
	int iX0 = int(pos.x);
	int iX1 = iX0 + 1; 
	int iY0 = int(pos.y);
	int iY1 = iY0 + 1;

	float X0 = (float)iX0;
	float X1 = (float)iX1;
	float Y0 = (float)iY0;
	float Y1 = (float)iY1;

	//获得此方格4个顶点的梯度值
	const vec2& v0 = 
		GetVec(iX0, iY0);
	const vec2& v1 = 
		GetVec(iX0, iY1);
	const vec2& v2 = 
		GetVec(iX1, iY0);
	const vec2& v3 = 
		GetVec(iX1, iY1);



	vec2 d0 (pos.x-X0, pos.y-Y0);
	vec2 d1 (pos.x-X0, pos.y-Y1);
	vec2 d2 (pos.x-X1, pos.y-Y0);
	vec2 d3 (pos.x-X1, pos.y-Y1);


	//通过内积算出方格4个顶点的权重
	float h0 = (d0.x * v0.x)+(d0.y * v0.y);
	float h1 = (d1.x * v1.x)+(d1.y * v1.y);
	float h2 = (d2.x * v2.x)+(d2.y * v2.y);
	float h3 = (d3.x * v3.x)+(d3.y * v3.y);

	float Sx = d0.x,Sy = d0.y;


	//柏林插值平滑函数f(x) = 6*x^5 - 15*x^4 + 10*x^3
	//通过柏林方程式平滑此点

	Sx = (6*powf(d0.x,5.0f))
		-(15*powf(d0.x,4.0f))
		+(10*powf(d0.x,3.0f));

	Sy = (6*powf(d0.y,5.0f))
		-(15*powf(d0.y,4.0f))
		+(10*powf(d0.y,3.0f));


	//在下2顶点的x轴上对权重点Sx进行线性插值
	float avgX0 = h0 + (Sx*(h2 - h0));
	//在上2顶点的x轴上对权重点Sx进行线性插值
	float avgX1 = h1 + (Sx*(h3 - h1));
	//在y轴上合并2个插值
	float result = avgX0 + (Sy*(avgX1 - avgX0)); 

	return result;


}
