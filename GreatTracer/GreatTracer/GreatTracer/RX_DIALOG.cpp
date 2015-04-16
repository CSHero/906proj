// RX_DIALOG.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GreatTracer.h"
#include "RX_DIALOG.h"

CVIEWDLG *cview2;

// RX_DIALOG �Ի���

IMPLEMENT_DYNAMIC(RX_DIALOG, CDialog)

RX_DIALOG::RX_DIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(RX_DIALOG::IDD, pParent)
{

}

RX_DIALOG::~RX_DIALOG()
{
}

void RX_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RX_DIALOG, CDialog)
	ON_BN_CLICKED(IDOK, &RX_DIALOG::OnBnClickedOk)
END_MESSAGE_MAP()


// RX_DIALOG ��Ϣ�������

void RX_DIALOG::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	CString temp;
	cview2->AddEle();
	int a =Point_RxInfo.GetCount();
	for(int i=0;i<a;i++){
		GetDlgItemText(IDC_EDIT_RX_PZ_DES,temp);
		Point_RxInfo[i].Description=temp;

		if(Point_RxInfo[i].Pos.z<0){
			GetDlgItemText(IDC_EDIT__RX_PZ_Z,temp);
			Point_RxInfo[i].Pos.z=(GLfloat)atof(temp);
			cview2->Deal_Rx_Vertex_Cube(Point_RxInfo[i]);
		}
	}
	OnOK();
	free(cview2);
}

BOOL RX_DIALOG::OnInitDialog(){
	SetDlgItemText(IDC_EDIT_RX_PZ_DES,"Untitled Rx");
	CString temp;
	temp.Format("%.2f",Transfer_Z);
	SetDlgItemText(IDC_EDIT__RX_PZ_Z,temp);

	return TRUE;
}