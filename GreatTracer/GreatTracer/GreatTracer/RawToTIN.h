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
	int GetHeight(byte *pHeightMap,int X,int Y);//�������ҵ�ͼ��ĸ߶�
	void SetVertexColor(byte *pHeightMap,int x,int y);//���ݵ�������͸߳���Ϣ�趨�����ɫ
public:
	void loadRawFile(CString filename,int nSize,byte *pHeightMap);//��ȡraw�ļ�
	void RenderRawTIN(byte pHeightMap[]);//����Raw���ɵ�TIN
};
