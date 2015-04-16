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
	GLfloat GetHeight(byte *pHeightMap,int X,int Y);//抽样让我地图点的高度
	void SetVertexColor(byte *pHeightMap,int x,int y);//根据点的索引和高程信息设定点的颜色
public:
	bool RenderFlag;
	void loadRawFile(CString filename,int nSize,byte *pHeightMap);//读取raw文件
	void RenderRawTIN(byte pHeightMap[]);//绘制Raw生成的TIN
	vertexInfo dealColor(vertexInfo tempvertex);
	void dealVertex(BYTE pHeightMap[]);
};
