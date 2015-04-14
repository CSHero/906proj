// TX_PROPER_DIALOG.cpp : 实现文件
//

#include "stdafx.h"
#include "GreatTracer.h"
#include "TX_PROPER_DIALOG.h"


CVIEWDLG *cview3;


IMPLEMENT_DYNAMIC(TX_PROPER_DIALOG, CDialog)

TX_PROPER_DIALOG::TX_PROPER_DIALOG(CWnd* pParent /*=NULL*/)
	: CDialog(TX_PROPER_DIALOG::IDD, pParent)
{

}

TX_PROPER_DIALOG::~TX_PROPER_DIALOG()
{
}

void TX_PROPER_DIALOG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ANTE, m_Ante_Combo);
	//DDX_Control(pDX, IDC_WAVEFORM, m_Wave_Combo);
}


BEGIN_MESSAGE_MAP(TX_PROPER_DIALOG, CDialog)
	//ON_BN_CLICKED(IDOK, &TX_PROPER_DIALOG::OnBnClickedOk)
	ON_BN_CLICKED(IDOK, &TX_PROPER_DIALOG::OnBnClickedOk)
END_MESSAGE_MAP()

BOOL TX_PROPER_DIALOG::OnInitDialog(){
	SetDlgItemText(IDC_EDIT_DESCRIPTION,"Untitled Tx");
	SetDlgItemText(IDC_EDIT_TX_HEIGHT,"0.0");
	SetDlgItemText(IDC_EDIT_CARRIERFRE,"900");
	SetDlgItemText(IDC_EDIT_INPUTPOWER,"100");
	SetDlgItemText(IDC_EDIT_ANTE,"0");
	SetDlgItemText(IDC_EDIT_WAVEFORM,"1");
	return TRUE;
}

// TX_PROPER_DIALOG 消息处理程序


void TX_PROPER_DIALOG::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码

	cview3->AddEle();
	int a =Point_TxInfo.GetCount();
	CString temp;
	int n=Point_TxInfo.GetCount();
	for(int i=0;i<Point_TxInfo.GetCount();i++){

		GetDlgItemText(IDC_EDIT_DESCRIPTION,temp);
		Point_TxInfo[i].Description=temp;

		if(Point_TxInfo[i].Pos.z<0){
			GetDlgItemText(IDC_EDIT_TX_HEIGHT,temp);
			Point_TxInfo[i].Pos.z=(GLfloat)atof(temp);
			cview3->Deal_Tx_Vertex_Cube(Point_TxInfo[i]);
		}
		if(Point_TxInfo[i].CarrierFrequency<0){
			GetDlgItemText(IDC_EDIT_CARRIERFRE,temp);
			Point_TxInfo[i].CarrierFrequency=(GLfloat)atof(temp);
		}
		if(Point_TxInfo[i].InputPower<0){
			GetDlgItemText(IDC_EDIT_INPUTPOWER,temp);
			Point_TxInfo[i].InputPower=(GLfloat)atof(temp);
		}
		if(Point_TxInfo[i].AntennaType<0){
			GetDlgItemText(IDC_EDIT_ANTE,temp);
			Point_TxInfo[i].AntennaType=atoi(temp);
		}
		if(Point_TxInfo[i].WaveForm<0){
			GetDlgItemText(IDC_EDIT_WAVEFORM,temp);
			Point_TxInfo[i].WaveForm=atoi(temp);
		}
	}
	OnOK();
	free(cview3);
}

int TX_PROPER_DIALOG::TestPara(int b){
	int a=1;
	return b+a;
}