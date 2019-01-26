
// HomeMonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "../HomeMon.h"
#include "HomeMonDlg.h"
#include "afxdialogex.h"
#include "AddDlg.h"
#include "RateDlg.h"
#include "RemoveDlg.h"
#include "SerialDlg.h"
#include "../Utils/Settings.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CHomeMonDlg dialog



CHomeMonDlg::CHomeMonDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_HOMEMON_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	this->bgR = Settings::GetInt(".//config.ini", "window", "bgr", 0);
	this->bgG = Settings::GetInt(".//config.ini", "window", "bgg", 0);
	this->bgB = Settings::GetInt(".//config.ini", "window", "bgb", 0);


	this->img = new CImage();
	this->img->Create(1024, 768, 24);
	int baudrate = Settings::GetInt(".//config.ini", "serial", "baudrate", 9600);
	std::string port = Settings::GetString(".//config.ini", "serial", "port", "\\\\.\\COM1");

	this->c = new Controller(img, port, baudrate);
	this->controlRect.left = 0;
	this->controlRect.top = 0;
	this->controlRect.right = 100;
	this->controlRect.bottom = 100;
	this->drawRect = false;
	this->max = (Settings::GetInt(".//config.ini", "window", "fullscreen", 0) == 1);
	this->justCreated = true;

	this->c->Deserialize("controls.ini");
}

void CHomeMonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHomeMonDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ADDCONTROL, &CHomeMonDlg::OnAddcontrol)
	ON_COMMAND(FULLSCREEN, &CHomeMonDlg::OnFullscreen)
	ON_COMMAND(QUIT, &CHomeMonDlg::OnQuit)
	ON_COMMAND(RATE, &CHomeMonDlg::OnRate)
	ON_COMMAND(REMOVECONTROL, &CHomeMonDlg::OnRemovecontrol)
	ON_COMMAND(SAVELOG, &CHomeMonDlg::OnSavelog)
	ON_COMMAND(SCREEN, &CHomeMonDlg::OnScreen)
	ON_COMMAND(SERIAL, &CHomeMonDlg::OnSerial)
	ON_COMMAND(STOPLOG, &CHomeMonDlg::OnStoplog)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_SIZE()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
END_MESSAGE_MAP()


// CHomeMonDlg message handlers

BOOL CHomeMonDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	int t = Settings::GetInt(".//config.ini", "window", "refresh", 60000);
	SetTimer(1, t, NULL);
	this->accelerator = LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CHomeMonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();

		CPaintDC dc(this);
		img = c->GetImg();
		CBitmap bmp;
		bmp.Attach(*img);
		CClientDC cdc(this->GetDlgItem(ID_PIC));
		CDC b;
		b.CreateCompatibleDC(&cdc);
		CBitmap *old = b.SelectObject(&bmp); // OLDBITMAP = NULL
		cdc.BitBlt(0, 0, img->GetWidth(), img->GetHeight(), &b, 0, 0, SRCCOPY);
		b.SelectObject(old);
		bmp.Detach();
		ReleaseDC(&b);
		ReleaseDC(&cdc);
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CHomeMonDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHomeMonDlg::OnAddcontrol()
{
	this->addDlg = new CAddDlg(this, this->c, this);
	this->addDlg->Create(IDD_ADDDLG, this);
	this->addDlg->ShowWindow(SW_SHOW);
}


void CHomeMonDlg::OnFullscreen()
{
	if (!max)
	{
		this->GetTopLevelParent()->ModifyStyle(WS_CAPTION | WS_SYSMENU | WS_SIZEBOX, 0);
		this->GetTopLevelParent()->ShowWindow(SW_SHOWMAXIMIZED);
		max = !max;
		Settings::Set(".//config.ini", "window", "fullscreen", 1);
	}
	else
	{
		this->GetTopLevelParent()->ModifyStyle(0, WS_CAPTION | WS_SYSMENU | WS_SIZEBOX);
		this->GetTopLevelParent()->ShowWindow(SW_SHOWDEFAULT);
		max = !max;
		Settings::Set(".//config.ini", "window", "fullscreen", 0);

		CRect r, def;

		this->GetWindowRect(&def);
		r.top = Settings::GetInt(".//config.ini", "window", "top", def.top);
		r.left = Settings::GetInt(".//config.ini", "window", "left", def.left);
		r.bottom = Settings::GetInt(".//config.ini", "window", "bottom", def.bottom);
		r.right = Settings::GetInt(".//config.ini", "window", "right", def.right);

		this->GetTopLevelParent()->MoveWindow(r.left, r.top, r.Width(), r.Height(), true);
	}

	Invalidate();
}


void CHomeMonDlg::OnQuit()
{
	DestroyWindow();
}


void CHomeMonDlg::OnRate()
{
	CRateDlg r(this);
	r.DoModal();
	int t = Settings::GetInt(".//config.ini", "window", "refresh", 60000);
	SetTimer(1, t, NULL);
}


void CHomeMonDlg::OnRemovecontrol()
{
	CRemoveDlg r(this->c, this->bgR, this->bgG, this->bgB, this);
	r.DoModal();
}


void CHomeMonDlg::OnSavelog()
{
	TCHAR filt[] = _T("Log Files (*.log)|*.log|Text Documents (*.txt)|*.txt|All Files (*.*)|*.*||");
	CFileDialog f(FALSE, _T("log"), NULL, 6UL, filt);

	if (f.DoModal() == IDOK)
	{
		char tmp[1024];

		f.GetPathName();

		wcstombs(tmp, f.GetPathName(), f.GetPathName().GetLength());
		tmp[f.GetPathName().GetLength()] = '\0';
		c->StartLogging(std::string(tmp));
	}
}


void CHomeMonDlg::OnScreen()
{
	TCHAR filt[] = _T("PNG (*.png)|*.png|All Files (*.*)|*.*||");
	CFileDialog f(FALSE, _T("png"), _T("screen.png"), 6UL, filt);

	if (f.DoModal() == IDOK)
	{
		char tmp[1024];

		f.GetPathName();

		wcstombs(tmp, f.GetPathName(), f.GetPathName().GetLength());
		tmp[f.GetPathName().GetLength()] = '\0';
		c->SaveScreenshot(std::string(tmp));
	}
}


void CHomeMonDlg::OnSerial()
{
	this->c->Stop();
	CSerialDlg s(this);
	s.DoModal();

	int baud = Settings::GetInt(".//config.ini", "serial", "baudrate", 9600);
	std::string port = Settings::GetString(".//config.ini", "serial", "port", "\\\\.\\COM1");

	this->c->Start(port, baud);
}


void CHomeMonDlg::OnStoplog()
{
	this->c->StopLogging();
}

void CHomeMonDlg::OnOK()
{
	// Do nothing.
}

void CHomeMonDlg::OnCancel()
{
	DestroyWindow();
}

void CHomeMonDlg::SetDrawRect(bool drawRect)
{
	this->drawRect = drawRect;
}


int CHomeMonDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect r, def;

	this->GetWindowRect(&def);

	r.top = Settings::GetInt(".//config.ini", "window", "top", def.top);
	r.left = Settings::GetInt(".//config.ini", "window", "left", def.left);
	r.bottom = Settings::GetInt(".//config.ini", "window", "bottom", def.bottom);
	r.right = Settings::GetInt(".//config.ini", "window", "right", def.right);

	this->MoveWindow(r.left, r.top, r.Width(), r.Height(), true);

	return 0;
}


void CHomeMonDlg::OnDestroy()
{
	RECT r;
	this->GetWindowRect(&r);

	if (!this->max)
	{
		Settings::Set(".//config.ini", "window", "top", r.top);
		Settings::Set(".//config.ini", "window", "left", r.left);
		Settings::Set(".//config.ini", "window", "bottom", r.bottom);
		Settings::Set(".//config.ini", "window", "right", r.right);
	}

	this->c->Serialize("controls.ini");
	this->c->Stop();
	this->c->StopLogging();

	CDialogEx::OnDestroy();
}


void CHomeMonDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (this->drawRect)
	{
		this->beginX = point.x;
		this->beginY = point.y;
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CHomeMonDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (drawRect)
	{
		if (point.x > beginX)
		{
			controlRect.right = point.x;
			controlRect.left = beginX;
		}
		else
		{
			controlRect.right = beginX;
			controlRect.left = point.x;
		}
		if (point.y > beginY)
		{
			controlRect.bottom = point.y;
			controlRect.top = beginY;
		}
		else
		{
			controlRect.bottom = beginY;
			controlRect.top = point.y;
		}

		addDlg->SetControlRect(controlRect);
		addDlg->ShowWindow(SW_SHOW);
		drawRect = false;
	}
	else
		controlRect.top = controlRect.bottom = controlRect.left = controlRect.right = 0;

	CDialogEx::OnLButtonUp(nFlags, point);
}


BOOL CHomeMonDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	bool ret = true;
	if (drawRect)
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_CROSS));
	else
		ret = CDialogEx::OnSetCursor(pWnd, nHitTest, message);

	return ret;
}


void CHomeMonDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	RECT r;
	this->GetClientRect(&r);
	if (this->GetDlgItem(ID_PIC) != NULL)
		this->GetDlgItem(ID_PIC)->MoveWindow(&r, true);

	if (r.right - r.left != 0 && r.bottom - r.top != 0)
	{
		this->img = new CImage();

		this->img->Create(r.right - r.left, r.bottom - r.top, 24);
		HDC h = this->img->GetDC();
		HBRUSH b = CreateSolidBrush(RGB(this->bgR, this->bgG, this->bgB));
		FillRect(h, &r, b);
		this->c->SetImg(this->img);
		this->c->Update();
		this->img->ReleaseDC();
		Invalidate();
	}
}


void CHomeMonDlg::OnTimer(UINT_PTR nIDEvent)
{
	this->c->Update();
	Invalidate();

	CDialogEx::OnTimer(nIDEvent);
}

BOOL CHomeMonDlg::PreTranslateMessage(MSG * pMsg)
{
	BOOL ret = true;

	if (this->accelerator)
		ret = TranslateAccelerator(m_hWnd, this->accelerator, pMsg);

	return ret;
}


void CHomeMonDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialogEx::OnActivate(nState, pWndOther, bMinimized);

	if (this->justCreated)
	{
		if (max)
		{
			this->GetTopLevelParent()->ModifyStyle(WS_CAPTION | WS_SYSMENU | WS_SIZEBOX, 0); /* Il primo parametro è lo stile da rimuovere, il secondo quello da aggiungere */
			this->GetTopLevelParent()->ShowWindow(SW_SHOWMAXIMIZED);
		}
		else
		{
			this->GetTopLevelParent()->ModifyStyle(0, WS_CAPTION | WS_SYSMENU | WS_SIZEBOX); /* In questo esempio specifico WS_SYSMENU (i tasti chiudi, minimizza e massimizza)5 non c'è all'avvio... */
			this->GetTopLevelParent()->ShowWindow(SW_SHOWDEFAULT);
		}

		this->justCreated = false;
	}
}
