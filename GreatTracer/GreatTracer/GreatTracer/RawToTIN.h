#pragma once
#include "stdafx.h"
#include "VIEWDLG.h"
#include "windows.h"
#include <vector>
#include <cstring>
#include "ScaleVariable.h"
using namespace std;

struct vertexInfo{
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};
struct vertexColor{
	GLfloat red;
	GLfloat green;
	GLfloat blue;
};
struct vertexCoor{
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

extern GLfloat MapCenterX;
extern GLfloat MapCenterY;
extern GLfloat MapCenterZ;

class CRawToTIN
{
public:
	CRawToTIN(void);
	~CRawToTIN(void);
private:
	GLfloat GetHeight(byte *pHeightMap,int X,int Y);//�������ҵ�ͼ��ĸ߶�
	void SetVertexColor(byte *pHeightMap,int x,int y);//���ݵ�������͸߳���Ϣ�趨�����ɫ
public:
	bool RenderFlag;
	void loadRawFile(CString filename,int nSize,byte *pHeightMap);//��ȡraw�ļ�
	void RenderRawTIN(byte pHeightMap[]);//����Raw���ɵ�TIN
	vertexInfo dealColor(vertexInfo tempvertex);
	void dealVertex(BYTE pHeightMap[]);
};
