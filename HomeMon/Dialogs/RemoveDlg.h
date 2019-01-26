#pragma once
#include "../Controller.h"
#include "afxwin.h"

// CRemoveDlg dialog

class CRemoveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRemoveDlg)

public:
	CRemoveDlg(Controller *c, uint8_t bgR, uint8_t bgG, uint8_t bgB, CWnd* pParent = NULL);   // standard constructor
	virtual ~CRemoveDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_REMOVEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnClickedOk();
	afx_msg void OnPaint();

private:
	Controller *c;
	uint8_t bgR;
	uint8_t bgG;
	uint8_t bgB;
	CImage *sample;
	Control *toBeRemoved;
	bool justCreated;
	CComboBox control;
	CListBox associatedControl;
};
