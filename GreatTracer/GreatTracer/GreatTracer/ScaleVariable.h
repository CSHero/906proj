#pragma once

#include "gl/gl.h"
#include "stdafx.h"

typedef struct{//三维向量
	GLdouble x,y,z;
}Vec3T;

typedef struct{//鼠标在OpenGL坐标系中的二维坐标
	GLfloat x;
	GLfloat y;
}CurPos2D;

typedef struct{//鼠标在OpenGL坐标系中的三维坐标
	GLfloat x;
	GLfloat y;
	GLfloat z;
}CurPos3D;

typedef struct{
	CurPos3D P1;
	CurPos3D P2;
}line;

typedef struct{
	CurPos3D P1;
	CurPos3D P2;
	CurPos3D P3;
	GLfloat normal[3];//法向量
}Triangle;

typedef struct{
	GLfloat red,green,blue;
}ColorRamp;

typedef struct{
	CurPos3D Pos1st;
	CurPos3D Pos2nd;
	CurPos3D Pos3rd;
	CurPos3D Pos4th;
	ColorRamp Color;
}RenderCell;

typedef struct{//发射机数据
	CString Description;
	CurPos3D Pos;
	GLfloat InputPower;
	GLfloat CarrierFrequency;
	int AntennaType;
	int WaveForm;
}TxInfo;

typedef struct{//接收机数据
	CString Description;
	CurPos3D Pos;
	GLfloat Distance;
	GLfloat ReceivedPower;
	int ModulType;
	GLfloat Delay;
	GLfloat Phase;
}RxInfo;

typedef struct {//天线数据
	GLfloat StartRad;
	GLfloat EndRad;
	GLfloat PowerGain;//功率增益
	//添加建模相关的参数

}AntennaInfo;

typedef struct{//波形数据
	int ModulType;//调制方式

}ModualInfo;

class ScaleVariable
{
public:
	ScaleVariable(void);
	~ScaleVariable(void);
};
