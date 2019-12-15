#ifndef _MY_UTIL_H_
#define _MY_UTIL_H_
#pragma once

#pragma warning(disable:4786)

#define MAX(a,b)            (((a) > (b)) ? (a) : (b))
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))

#define SAFE_DELETE_VECTOR(p)	{ for(int i=0;i<p.size();i++) { if(p[i]) delete p[i]; } p.resize(0); }
#define SAFE_DELETE(p)			{ if(p) { delete (p);     (p)=NULL; } }
#define SAFE_DELETE_ARRAY(p)	{ if(p) { delete[] (p);   (p)=NULL; } }
#define SAFE_RELEASE(p)			{ if(p) { (p)->Release(); (p)=NULL; } }
#define SAFE_RELEASE_VECTOR(p)	{ for(int i=0;i<p.size();++i) SAFE_RELEASE(p[i]); p.clear(); }

#define LOGINFO(str) {MessageBox(0, str, 0, 0);}


//头文件
#include <WINDOWS.H>
#include <WINDOWSX.H>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <map>
#include <cstddef>
#include <algorithm>
#include <stdio.h>
using namespace std;

#include "MySceneManager.h"
#include "MyFontManager.h"
#include "MyGUIManager.h"
#include "MyMath.h" 
#include "Matrix.h"
#include "MyTime.h"
#include "Vector.h"
#include "MyVertexformat.h"

//工具类,提供一些有用的小函数
namespace MyUTIL{

	/** 判断点是否在一个2D三角形的内部
	@param
	px 点的x坐标
	@param
	py 点的y坐标
	@param
	ax 三角形第一个点的x坐标
	@param
	ay 三角形第一个点的y坐标
	@param
	bx 三角形第二个点的x坐标
	@param
	by 三角形第二个点的y坐标
	@param
	cx 三角形第三个点的x坐标
	@param
	cy 三角形第三个点的y坐标
	@returns
	如果点在三角形内部,返回<b>true</b>
	@par
	如果点不在三角形内部,返回<b>false</b>
	*/
	bool PointInTri2D(float px, float pz, float ax, float az, float bx, float bz, float cx, float cz);

	// 点和矩形碰撞检测
	bool DotInRect(int dotX, int dotY, int rectX, int rectY, 
					int rectWidth, int rectHeight);

	bool DotInRect(float dotX, float dotY, float rectX, float rectY, 
		float rectWidth, float rectHeight);


	// 两个矩形碰撞检测
	bool RectCollision(int rectAX,int rectAY,int rectAWidth,int rectAHeight,
					int rectBX,int rectBY,int rectBWidth,int rectBHeight);

	//字符转浮点数
	float StrToFloat(const string& str);
	//字符转整型
	int StrToInt(const string& str);
	//字符转布尔
	bool StrToBool(const string& str);

	//整型转字符
	string IntToStr(int i);
	//浮点转字符
	string FloatToStr(float f, int bitAfterDot);

	string GetNextStrNotCmt(ifstream& fin);

	string GetThisLineRest(ifstream& fin);

	/** 得到流中合理字符串
	 *	@param [const string&] stopStr : 字符串不能包含stopStr中的字符，由这些字符表示字符串结束	
	 *	@note: 不支持注释
	 */
	string GetNextStrStopBy(ifstream& fin, const string& stopStr);

	/* 将字符串单行输出，特点是将字符串中的"\n"变成"\\n"输出 */
	void WriteStrWithLineSymbol(ofstream& fout, string str);
	
	void ReadWholeFile(string filename, string& content);

	string ToLowerCase(const string& str);
	string ToUpperCase(const string& str);

	/** 搜索一个目录下所有文件（包括子目录）
	* @param [string] sRootDir: 搜索根目录
	* @param [vector<string>&] vFileName: 返回的文件列表
	* @param [const string&] sPostfix: 文件的后缀名，可以为空(表示不关心文件类型)
	* @param [const string&] sPrefix: 目录的前缀，调用函数的时候不用赋值
	* @return [vector<string>&] : 符合条件的在该目录下的所有文件名
	*/
	vector<string>& FindFilesInDir(string sRootDir, vector<string>& vFileName, const string& sPostfix = "", const string& sPrefix = "");

	/** 是否是2的指数冥 */
	bool Is2Power(int n);

}

#endif