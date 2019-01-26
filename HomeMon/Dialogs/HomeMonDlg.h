
// HomeMonDlg.h : header file
//

#pragma once

#include "../Controller.h"

class CAddDlg;

// CHomeMonDlg dialog
class CHomeMonDlg : public CDialogEx
{
// Construction
public:
	CHomeMonDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HOMEMON_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnAddcontrol();
	afx_msg void OnFullscreen();
	afx_msg void OnQuit();
	afx_msg void OnRate();
	afx_msg void OnRemovecontrol();
	afx_msg void OnSavelog();
	afx_msg void OnScreen();
	afx_msg void OnSerial();
	afx_msg void OnStoplog();
	void OnOK();
	void OnCancel();
	void SetDrawRect(bool drawRect);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BOOL PreTranslateMessage(MSG *pMsg);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
private:
	Controller *c;
	uint8_t bgR;
	uint8_t bgG;
	uint8_t bgB;
	bool max;
	CImage *img;
	bool drawRect;
	RECT controlRect;
	int beginX;
	int beginY;
	CAddDlg *addDlg;
	HACCEL accelerator;
	bool justCreated;
};
