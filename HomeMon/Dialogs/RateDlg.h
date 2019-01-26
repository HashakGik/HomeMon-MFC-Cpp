#pragma once
#include "afxcmn.h"


// CRateDlg dialog

class CRateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CRateDlg)

public:
	CRateDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRateDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RATEDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnBnClickedOk();
private:
	CSpinButtonCtrl spin;
	int refresh;
};
