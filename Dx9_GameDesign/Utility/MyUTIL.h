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


//ͷ�ļ�
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

//������,�ṩһЩ���õ�С����
namespace MyUTIL{

	/** �жϵ��Ƿ���һ��2D�����ε��ڲ�
	@param
	px ���x����
	@param
	py ���y����
	@param
	ax �����ε�һ�����x����
	@param
	ay �����ε�һ�����y����
	@param
	bx �����εڶ������x����
	@param
	by �����εڶ������y����
	@param
	cx �����ε��������x����
	@param
	cy �����ε��������y����
	@returns
	��������������ڲ�,����<b>true</b>
	@par
	����㲻���������ڲ�,����<b>false</b>
	*/
	bool PointInTri2D(float px, float pz, float ax, float az, float bx, float bz, float cx, float cz);

	// ��;�����ײ���
	bool DotInRect(int dotX, int dotY, int rectX, int rectY, 
					int rectWidth, int rectHeight);

	bool DotInRect(float dotX, float dotY, float rectX, float rectY, 
		float rectWidth, float rectHeight);


	// ����������ײ���
	bool RectCollision(int rectAX,int rectAY,int rectAWidth,int rectAHeight,
					int rectBX,int rectBY,int rectBWidth,int rectBHeight);

	//�ַ�ת������
	float StrToFloat(const string& str);
	//�ַ�ת����
	int StrToInt(const string& str);
	//�ַ�ת����
	bool StrToBool(const string& str);

	//����ת�ַ�
	string IntToStr(int i);
	//����ת�ַ�
	string FloatToStr(float f, int bitAfterDot);

	string GetNextStrNotCmt(ifstream& fin);

	string GetThisLineRest(ifstream& fin);

	/** �õ����к����ַ���
	 *	@param [const string&] stopStr : �ַ������ܰ���stopStr�е��ַ�������Щ�ַ���ʾ�ַ�������	
	 *	@note: ��֧��ע��
	 */
	string GetNextStrStopBy(ifstream& fin, const string& stopStr);

	/* ���ַ�������������ص��ǽ��ַ����е�"\n"���"\\n"��� */
	void WriteStrWithLineSymbol(ofstream& fout, string str);
	
	void ReadWholeFile(string filename, string& content);

	string ToLowerCase(const string& str);
	string ToUpperCase(const string& str);

	/** ����һ��Ŀ¼�������ļ���������Ŀ¼��
	* @param [string] sRootDir: ������Ŀ¼
	* @param [vector<string>&] vFileName: ���ص��ļ��б�
	* @param [const string&] sPostfix: �ļ��ĺ�׺��������Ϊ��(��ʾ�������ļ�����)
	* @param [const string&] sPrefix: Ŀ¼��ǰ׺�����ú�����ʱ���ø�ֵ
	* @return [vector<string>&] : �����������ڸ�Ŀ¼�µ������ļ���
	*/
	vector<string>& FindFilesInDir(string sRootDir, vector<string>& vFileName, const string& sPostfix = "", const string& sPrefix = "");

	/** �Ƿ���2��ָ��ڤ */
	bool Is2Power(int n);

}

#endif