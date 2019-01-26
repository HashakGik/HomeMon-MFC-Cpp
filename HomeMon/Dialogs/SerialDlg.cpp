// SerialDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../HomeMon.h"
#include "SerialDlg.h"
#include "afxdialogex.h"

#include "../Utils/Settings.h"
#include <string>


// CSerialDlg dialog

IMPLEMENT_DYNAMIC(CSerialDlg, CDialogEx)

CSerialDlg::CSerialDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_SERIALDLG, pParent)
	, port(_T(""))
	, baud(0)
{
	this->justCreated = true;
}

CSerialDlg::~CSerialDlg()
{
}

void CSerialDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, portControl);
	DDX_Control(pDX, IDC_EDIT1, baudControl);
	DDX_CBString(pDX, IDC_COMBO1, port);
	DDX_Text(pDX, IDC_EDIT1, baud);
}


BEGIN_MESSAGE_MAP(CSerialDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &CSerialDlg::OnBnClickedOk)
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CSerialDlg message handlers


void CSerialDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	char tmp[64];
	wcstombs(tmp, port.GetBuffer(), port.GetLength());
	tmp[port.GetLength()] = '\0';

	if (this->serial.Open(std::string(tmp), 9600))
	{
		this->serial.Close();
		Settings::Set(".//config.ini", "serial", "port", port);
		Settings::Set(".//config.ini", "serial", "baudrate", baud);

		CDialog::OnOK();
	}
	else
		MessageBox(L"The selected port can't be opened.", L"Error", MB_ICONERROR | MB_OK);
}


void CSerialDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	wchar_t str[64];
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if (this->justCreated)
	{
		this->ports = this->serial.GetAvailablePorts();

		for (unsigned int i = 0; i < this->ports.size(); i++)
		{
			mbstowcs(str, this->ports[i].c_str(), 64);
			portControl.AddString(str);
		}

		std::string p = Settings::GetString(".//config.ini", "serial", "port", "\\\\.\\COM1");
		mbstowcs(str, p.c_str(), 64);

		if (this->serial.Open(p, 9600))
		{
			portControl.SetCurSel(portControl.FindStringExact(0, str));
			this->serial.Close();
		}
		else if (portControl.GetCount() > 0)
			portControl.SetCurSel(0);

		baud = Settings::GetInt(".//config.ini", "serial", "baudrate", 9600);

		UpdateData(FALSE);

		this->justCreated = false;
	}
}
