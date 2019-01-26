// RateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../HomeMon.h"
#include "RateDlg.h"
#include "afxdialogex.h"
#include "../Utils/Settings.h"


// CRateDlg dialog

IMPLEMENT_DYNAMIC(CRateDlg, CDialogEx)

CRateDlg::CRateDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_RATEDLG, pParent)
	, refresh(0)
{

}

CRateDlg::~CRateDlg()
{
}

void CRateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN1, spin);
	DDX_Text(pDX, IDC_EDIT1, refresh);
}


BEGIN_MESSAGE_MAP(CRateDlg, CDialogEx)
	ON_WM_ACTIVATE()
	ON_BN_CLICKED(IDOK, &CRateDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CRateDlg message handlers


void CRateDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	spin.SetRange32(100, 3600000);
	refresh = Settings::GetInt(".//config.ini", "window", "refresh", 60000);
	UpdateData(FALSE);
}


void CRateDlg::OnBnClickedOk()
{
	UpdateData(TRUE);

	if (refresh >= 100 && refresh <= 3600000)
	{
		Settings::Set(".//config.ini", "window", "refresh", refresh);
		CDialog::OnOK();
	}
	else
		MessageBox(L"The refresh rate must be between 100 and 3600000 ms.", L"Error", MB_ICONERROR | MB_OK);
}
