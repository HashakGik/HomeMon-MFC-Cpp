// AddDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../HomeMon.h"
#include "AddDlg.h"
#include "afxdialogex.h"

#include "../Controls/Controls.h"
#include "../Utils/Settings.h"
#include "HomeMonDlg.h"

#include <sstream>


// CAddDlg dialog

IMPLEMENT_DYNAMIC(CAddDlg, CDialogEx)

CAddDlg::CAddDlg(CHomeMonDlg* p, Controller *c, CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ADDDLG, pParent)
	, vars(_T(""))
{
	this->parent = p;
	this->c = c;
	this->img = this->c->GetImg();
	this->fakeImg.Create(200, 200, 24);
	this->memento["x0"] = 0;
	this->memento["y0"] = 0;
	this->memento["x1"] = 100;
	this->memento["y1"] = 100;
	this->memento["cx"] = 50;
	this->memento["cy"] = 50;
	this->memento["radius"] = 50;
	this->memento["ticks"] = 5;
	this->memento["phase"] = 31400;
	this->memento["arc"] = 15700;
	this->memento["length"] = 100;
	this->memento["size"] = 10;
	this->memento["rings"] = 5;
	this->memento["dir"] = 0;
	this->memento["fill"] = 0;
	this->memento["shp"] = 0;
	this->memento["fgr"] = 0xff;
	this->memento["fgg"] = 0xff;
	this->memento["fgb"] = 0xff;
	this->memento["bgr"] = Settings::GetInt(".//config.ini", "window", "bgr");
	this->memento["bgg"] = Settings::GetInt(".//config.ini", "window", "bgg");
	this->memento["bgb"] = Settings::GetInt(".//config.ini", "window", "bgb");

	this->controls[0] = new Accelerometer(&(this->fakeImg), this->memento);
	this->controls[1] = new Bar(&(this->fakeImg), this->memento);
	this->controls[2] = new Dial(&(this->fakeImg), this->memento);
	this->controls[3] = new Plot(&(this->fakeImg), this->memento);
	this->controls[4] = new Radar(&(this->fakeImg), this->memento);

	this->justCreated = true;
}

CAddDlg::~CAddDlg()
{
	delete this->controls[0];
	delete this->controls[1];
	delete this->controls[2];
	delete this->controls[3];
	delete this->controls[4];
	this->fakeImg.Destroy();
}

void CAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBOLAB, comboLabelControl);
	DDX_Control(pDX, IDC_SPINLAB, spinLabelControl);
	DDX_Control(pDX, IDC_SLID1LAB, slider1LabelControl);
	DDX_Control(pDX, IDC_SLID2LAB, slider2LabelControl);
	DDX_Control(pDX, IDC_SLID3LAB, slider3LabelControl);
	DDX_Control(pDX, IDC_COMBO2, comboControl);
	DDX_Control(pDX, IDC_EDIT2, editControl);
	DDX_Control(pDX, IDC_SPIN1, spinControl);
	DDX_Control(pDX, IDC_SLIDER1, slider1Control);
	DDX_Control(pDX, IDC_SLIDER2, slider2Control);
	DDX_Control(pDX, IDC_SLIDER3, slider3Control);
	DDX_Control(pDX, IDC_COMBO1, typeControl);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, colorControl);
	DDX_Control(pDX, IDC_LIST3, coordControl);
	DDX_Text(pDX, IDC_EDIT1, vars);
}


BEGIN_MESSAGE_MAP(CAddDlg, CDialogEx)
	ON_WM_ACTIVATE()
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO1, &CAddDlg::OnCbnSelchangeCombo1)
	ON_BN_KILLFOCUS(IDC_MFCCOLORBUTTON1, &CAddDlg::OnBnKillfocusMfccolorbutton1)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CAddDlg::OnEnKillfocusEdit2)
	ON_EN_UPDATE(IDC_EDIT2, &CAddDlg::OnEnUpdateEdit2)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CAddDlg::OnCbnSelchangeCombo2)
	ON_BN_CLICKED(IDC_MFCCOLORBUTTON1, &CAddDlg::OnBnClickedMfccolorbutton1)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDOK, &CAddDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CAddDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &CAddDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CAddDlg message handlers


void CAddDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if (this->justCreated)
	{
		this->justCreated = false;
		this->colorControl.SetColor(RGB(0xff, 0xff, 0xff));
		this->typeControl.SetCurSel(0);
		this->controlRect.top = 0;
		this->controlRect.left = 0;
		this->controlRect.bottom = 100;
		this->controlRect.right = 100;
	}

	wchar_t tmp[256];
	coordControl.ResetContent();
	swprintf(tmp, L"x0 = %d", controlRect.left);
	coordControl.AddString(tmp);
	swprintf(tmp, L"y0 = %d", controlRect.top);
	coordControl.AddString(tmp);
	swprintf(tmp, L"x1 = %d", controlRect.right);
	coordControl.AddString(tmp);
	swprintf(tmp, L"y1 = %d", controlRect.bottom);
	coordControl.AddString(tmp);

	this->memento["x0"] = controlRect.left;
	this->memento["x1"] = controlRect.right;
	this->memento["y0"] = controlRect.top;
	this->memento["y1"] = controlRect.bottom;
	this->memento["cx"] = controlRect.left + (controlRect.right - controlRect.left) / 2;
	this->memento["cy"] = controlRect.top + (controlRect.bottom - controlRect.top) / 2;
	if (controlRect.top - controlRect.bottom < controlRect.right - controlRect.left)
	{
		this->memento["radius"] = (controlRect.bottom - controlRect.top) / 2;
		this->memento["length"] = controlRect.bottom - controlRect.top;
	}
	else
	{
		this->memento["radius"] = (controlRect.right - controlRect.left) / 2;
		this->memento["length"] = controlRect.right - controlRect.left;
	}
	COLORREF col = colorControl.GetColor();
	this->memento["fgr"] = GetRValue(col);
	this->memento["fgg"] = GetGValue(col);
	this->memento["fgb"] = GetBValue(col);

	this->OnCbnSelchangeCombo1();

	Invalidate();
}


void CAddDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}


void CAddDlg::OnPaint()
{
	CPaintDC dc(this);
	if (typeControl.GetCurSel() != -1)
	{
		CImage *sample;
		RECT r;
		this->GetDlgItem(IDC_PICADD)->GetClientRect(&r);

		sample = this->controls[typeControl.GetCurSel()]->GetSample(r);

		CBitmap bmp;
		bmp.Attach(*sample);
		CClientDC cdc(this->GetDlgItem(IDC_PICADD));
		CDC b;
		b.CreateCompatibleDC(&cdc);
		CBitmap *old = b.SelectObject(&bmp);

		cdc.BitBlt(0, 0, r.right - r.left, r.bottom - r.top, &b, 0, 0, SRCCOPY);
		b.SelectObject(old);
		bmp.Detach();
		ReleaseDC(&b);
		ReleaseDC(&cdc);
	}
}


void CAddDlg::OnCbnSelchangeCombo1()
{
	switch (typeControl.GetCurSel())
	{
	case 0:
		comboLabelControl.ShowWindow(SW_SHOW);
		comboControl.ShowWindow(SW_SHOW);
		spinLabelControl.ShowWindow(SW_HIDE);
		spinControl.ShowWindow(SW_HIDE);
		editControl.ShowWindow(SW_HIDE);
		slider1LabelControl.ShowWindow(SW_SHOW);
		slider1Control.ShowWindow(SW_SHOW);
		slider2LabelControl.ShowWindow(SW_HIDE);
		slider2Control.ShowWindow(SW_HIDE);
		slider3LabelControl.ShowWindow(SW_HIDE);
		slider3Control.ShowWindow(SW_HIDE);

		slider1LabelControl.SetWindowTextW(L"Rings");
		comboLabelControl.SetWindowTextW(L"Shape");
		comboControl.ResetContent();
		comboControl.AddString(L"Square");
		comboControl.AddString(L"Circle");
		comboControl.SetCurSel(this->memento["shp"]);

		slider1Control.SetRange(0, 50);
		slider1Control.SetPos(this->memento["rings"]);
		break;
	case 1:
		comboLabelControl.ShowWindow(SW_SHOW);
		comboControl.ShowWindow(SW_SHOW);
		spinLabelControl.ShowWindow(SW_HIDE);
		spinControl.ShowWindow(SW_HIDE);
		editControl.ShowWindow(SW_HIDE);
		slider1LabelControl.ShowWindow(SW_HIDE);
		slider1Control.ShowWindow(SW_HIDE);
		slider2LabelControl.ShowWindow(SW_HIDE);
		slider2Control.ShowWindow(SW_HIDE);
		slider3LabelControl.ShowWindow(SW_HIDE);
		slider3Control.ShowWindow(SW_HIDE);

		comboLabelControl.SetWindowTextW(L"Direction");
		comboControl.ResetContent();
		comboControl.AddString(L"Horizontal");
		comboControl.AddString(L"Vertical");
		comboControl.SetCurSel(this->memento["dir"]);
		break;
	case 2:
		comboLabelControl.ShowWindow(SW_SHOW);
		comboControl.ShowWindow(SW_SHOW);
		spinLabelControl.ShowWindow(SW_HIDE);
		spinControl.ShowWindow(SW_HIDE);
		editControl.ShowWindow(SW_HIDE);
		slider1LabelControl.ShowWindow(SW_SHOW);
		slider1Control.ShowWindow(SW_SHOW);
		slider2LabelControl.ShowWindow(SW_SHOW);
		slider2Control.ShowWindow(SW_SHOW);
		slider3LabelControl.ShowWindow(SW_SHOW);
		slider3Control.ShowWindow(SW_SHOW);

		comboLabelControl.SetWindowTextW(L"Direction");
		slider1LabelControl.SetWindowTextW(L"Ticks");
		slider2LabelControl.SetWindowTextW(L"Arc");
		slider3LabelControl.SetWindowTextW(L"Phase");
		comboControl.ResetContent();
		comboControl.AddString(L"Clockwise");
		comboControl.AddString(L"Counterclockwise");
		comboControl.SetCurSel(this->memento["dir"]);

		slider1Control.SetRange(0, 50);
		slider2Control.SetRange(0, 62800);
		slider3Control.SetRange(0, 62800);
		slider1Control.SetPos(this->memento["ticks"]);
		slider2Control.SetPos(this->memento["arc"]);
		slider3Control.SetPos(this->memento["phase"]);
		break;
	case 3:
		comboLabelControl.ShowWindow(SW_HIDE);
		comboControl.ShowWindow(SW_HIDE);
		spinLabelControl.ShowWindow(SW_HIDE);
		spinControl.ShowWindow(SW_HIDE);
		editControl.ShowWindow(SW_HIDE);
		slider1LabelControl.ShowWindow(SW_HIDE);
		slider1Control.ShowWindow(SW_HIDE);
		slider2LabelControl.ShowWindow(SW_HIDE);
		slider2Control.ShowWindow(SW_HIDE);
		slider3LabelControl.ShowWindow(SW_HIDE);
		slider3Control.ShowWindow(SW_HIDE);
		break;
	case 4:
		comboLabelControl.ShowWindow(SW_SHOW);
		comboControl.ShowWindow(SW_SHOW);
		spinLabelControl.ShowWindow(SW_SHOW);
		spinControl.ShowWindow(SW_SHOW);
		editControl.ShowWindow(SW_SHOW);
		slider1LabelControl.ShowWindow(SW_SHOW);
		slider1Control.ShowWindow(SW_SHOW);
		slider2LabelControl.ShowWindow(SW_HIDE);
		slider2Control.ShowWindow(SW_HIDE);
		slider3LabelControl.ShowWindow(SW_HIDE);
		slider3Control.ShowWindow(SW_HIDE);

		comboLabelControl.SetWindowTextW(L"Fill");
		slider1LabelControl.SetWindowTextW(L"Rings");
		spinLabelControl.SetWindowTextW(L"Variables");
		comboControl.ResetContent();
		comboControl.AddString(L"Yes");
		comboControl.AddString(L"No");
		comboControl.SetCurSel(this->memento["fill"]);

		slider1Control.SetRange(0, 50);
		spinControl.SetRange32(1, 50);
		slider1Control.SetPos(this->memento["rings"]);
		spinControl.SetPos32(this->memento["size"]);
		break;
	}

	this->OnInputChange();
}

void CAddDlg::OnInputChange()
{
	if (!this->justCreated)
	{
		switch (typeControl.GetCurSel())
		{
		case 0:
			this->memento["shp"] = comboControl.GetCurSel();
			this->memento["rings"] = slider1Control.GetPos();
			break;
		case 1:
			this->memento["dir"] = comboControl.GetCurSel();
			break;
		case 2:
			this->memento["dir"] = comboControl.GetCurSel();
			this->memento["ticks"] = slider1Control.GetPos();
			this->memento["arc"] = slider2Control.GetPos();
			this->memento["phase"] = slider3Control.GetPos();
			break;
		case 3:
			break;
		case 4:
			this->memento["fill"] = comboControl.GetCurSel();
			this->memento["size"] = spinControl.GetPos32();
			this->memento["rings"] = slider1Control.GetPos();
			break;
		}
		COLORREF col = colorControl.GetColor();
		this->memento["fgr"] = GetRValue(col);
		this->memento["fgg"] = GetGValue(col);
		this->memento["fgb"] = GetBValue(col);

		this->controls[typeControl.GetCurSel()]->SetState(this->memento);
		Invalidate();
	}
}

void CAddDlg::SetControlRect(RECT controlRect)
{
	this->controlRect = controlRect;
}


void CAddDlg::OnBnKillfocusMfccolorbutton1()
{
	this->OnInputChange();
}


void CAddDlg::OnEnKillfocusEdit2()
{
	this->OnInputChange();
}


void CAddDlg::OnEnUpdateEdit2()
{
	this->OnInputChange();
}

void CAddDlg::OnCbnSelchangeCombo2()
{
	this->OnInputChange();
}


void CAddDlg::OnBnClickedMfccolorbutton1()
{
	this->OnInputChange();
}


void CAddDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// Apparently sliders fire a WM_HSCROLL message...
	this->OnInputChange();

	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CAddDlg::OnBnClickedOk()
{
	char tmp[4096];
	UpdateData(TRUE);
	wcstombs(tmp, vars, vars.GetLength());
	tmp[vars.GetLength()] = '\0';
	std::string s(tmp), str;
	std::stringstream ss(s);
	std::vector<std::string> v;
	Control *contr;


	if (s.length() > 0 && this->controlRect.right - this->controlRect.left > 0 && this->controlRect.bottom - this->controlRect.top > 0)
	{
		do
		{
			std::getline(ss, str, ';');
			v.push_back(str);
		} while (!ss.fail());
		v.pop_back();

		switch (typeControl.GetCurSel())
		{
		case 0:
			contr = new Accelerometer(this->img, this->memento);
			break;
		case 1:
			contr = new Bar(this->img, this->memento);
			break;
		case 2:
			contr = new Dial(this->img, this->memento);
			break;
		case 3:
			contr = new Plot(this->img, this->memento);
			break;
		case 4:
			contr = new Radar(this->img, this->memento);
			break;
		}
		this->c->Attach(contr, v);
		CDialog::OnOK();
		DestroyWindow();
	}
	else if (this->controlRect.right - this->controlRect.left <= 0 || this->controlRect.bottom - this->controlRect.top <= 0)
		MessageBox(L"Invalid rectangle.", L"Error", MB_OK | MB_ICONERROR);
	else
		MessageBox(L"Invalid attached variables.", L"Error", MB_OK | MB_ICONERROR);
}


void CAddDlg::OnBnClickedCancel()
{
	CDialog::OnCancel();

	DestroyWindow();
}


void CAddDlg::OnBnClickedButton1()
{
	this->ShowWindow(SW_HIDE);
	this->GetParent()->ShowWindow(SW_SHOW);
	this->parent->SetDrawRect(true);
}
