// RemoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../HomeMon.h"
#include "RemoveDlg.h"
#include "afxdialogex.h"


// CRemoveDlg dialog

IMPLEMENT_DYNAMIC(CRemoveDlg, CDialogEx)

CRemoveDlg::CRemoveDlg(Controller *c, uint8_t bgR, uint8_t bgG, uint8_t bgB, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_REMOVEDLG, pParent)
{
	this->c = c;
	this->bgR = bgR;
	this->bgG = bgG;
	this->bgB = bgB;
	this->sample = nullptr;
	this->toBeRemoved = nullptr;
	this->justCreated = true;
}

CRemoveDlg::~CRemoveDlg()
{
}

void CRemoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, control);
	DDX_Control(pDX, IDC_LIST2, associatedControl);
}


BEGIN_MESSAGE_MAP(CRemoveDlg, CDialogEx)
	ON_WM_ACTIVATE()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CRemoveDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDOK, &CRemoveDlg::OnBnClickedOk)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CRemoveDlg message handlers


void CRemoveDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (this->justCreated)
	{
		wchar_t tmp[256];
		std::vector<std::string> s = this->c->GetControlNames();

		for (std::string str : s)
		{
			mbstowcs(tmp, str.c_str(), str.length());
			tmp[str.length()] = '\0';
			control.AddString(tmp);
		}

		if (control.GetCount() > 0)
		{
			control.SetCurSel(0);
			OnCbnSelchangeCombo1();
		}

		this->justCreated = false;
	}
}


void CRemoveDlg::OnCbnSelchangeCombo1()
{
	if (this->c->GetControls().size() > 0)
	{
		int i = control.GetCurSel();
		std::vector<std::string> v = this->c->GetVariables(this->c->GetControls()[i]);
		wchar_t tmp[256];

		associatedControl.ResetContent();

		for (std::string str : v)
		{
			mbstowcs(tmp, str.c_str(), str.length());
			tmp[str.length()] = '\0';
			associatedControl.AddString(tmp);
		}

		RECT r;
		this->GetDlgItem(IDC_PREVIEWREMOVE)->GetClientRect(&r);
		this->toBeRemoved = c->GetControls()[i];
		if (this->toBeRemoved != nullptr)
			this->sample = this->toBeRemoved->GetSample(r);
		Invalidate();
	}

	UpdateData(FALSE);
}


void CRemoveDlg::OnBnClickedOk()
{
	if (control.GetCurSel() != -1 && MessageBox(L"The control will be permanently deleted. Are you sure?", L"Warning", MB_ICONWARNING | MB_YESNO) == IDYES)
	{
		this->c->Detach(toBeRemoved);
		this->c->Serialize("controls.ini");
		CDialog::OnOK();
	}
}


void CRemoveDlg::OnPaint()
{
	CPaintDC dc(this);

	if (this->sample != nullptr)
	{
		CBitmap bmp;
		bmp.Attach(*sample);
		CClientDC cdc(this->GetDlgItem(IDC_PREVIEWREMOVE));
		CDC b;
		b.CreateCompatibleDC(&cdc);
		CBitmap *old = b.SelectObject(&bmp); // OLDBITMAP = NULL
		RECT r;
		this->GetDlgItem(IDC_PREVIEWREMOVE)->GetClientRect(&r);
		cdc.StretchBlt(0, 0, r.right - r.left, r.bottom - r.top, &b, 0, 0, sample->GetWidth(), sample->GetHeight(), SRCCOPY);
		b.SelectObject(old);
		bmp.Detach();
		ReleaseDC(&b);
		ReleaseDC(&cdc);
	}
}
