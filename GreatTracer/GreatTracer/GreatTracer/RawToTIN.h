#pragma once
#include "stdafx.h"
#include "VIEWDLG.h"
#include "windows.h"
#include <cstring>
using namespace std;
class CRawToTIN
{
public:
	CRawToTIN(void);
	~CRawToTIN(void);
private:
	int GetHeight(byte *pHeightMap,int X,int Y);//抽样让我地图点的高度
	void SetVertexColor(byte *pHeightMap,int x,int y);//根据点的索引和高程信息设定点的颜色
public:
	void loadRawFile(CString filename,int nSize,byte *pHeightMap);//读取raw文件
	void RenderRawTIN(byte pHeightMap[]);//绘制Raw生成的TIN
};
