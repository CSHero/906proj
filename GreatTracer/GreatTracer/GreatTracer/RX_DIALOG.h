#pragma once
#include "VIEWDLG.h"
#include "ScaleVariable.h"

extern GLfloat Transfer_Z;//引入鼠标指示的高度位置


// RX_DIALOG 对话框
extern CArray <RxInfo> Polygon_RxInfo;//接收机信息
extern CArray <RxInfo> Point_RxInfo;

class RX_DIALOG : public CDialog
{
	DECLARE_DYNAMIC(RX_DIALOG)

public:
	RX_DIALOG(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~RX_DIALOG();

// 对话框数据
	enum { IDD = IDD_RX_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	BOOL OnInitDialog();//对话框初始化函数
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();//确定按钮函数
};
