#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "afxcolorbutton.h"

#include "../Controller.h"

// CAddDlg dialog

class CHomeMonDlg;

class CAddDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CAddDlg)

public:
	CAddDlg(CHomeMonDlg *p, Controller *c, CWnd* pParent = NULL);   // standard constructor
	virtual ~CAddDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ADDDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNcDestroy();
	afx_msg void OnPaint();
	void OnInputChange();
	void SetControlRect(RECT controlRect);
	afx_msg void OnCbnSelchangeCombo1();
	afx_msg void OnBnKillfocusMfccolorbutton1();
	afx_msg void OnEnKillfocusEdit2();
	afx_msg void OnEnUpdateEdit2();
	afx_msg void OnCbnSelchangeCombo2();
	afx_msg void OnBnClickedMfccolorbutton1();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton1();
private:
	Control *controls[5];
	CImage fakeImg;
	CImage *img;
	Controller *c;
	std::map<std::string, int> memento;
	//std::vector<std::string> variables;
	bool drawRect;
	RECT controlRect;
	bool justCreated;
	CHomeMonDlg *parent;
	CStatic comboLabelControl;
	CStatic spinLabelControl;
	CStatic slider1LabelControl;
	CStatic slider2LabelControl;
	CStatic slider3LabelControl;
	CComboBox comboControl;
	CEdit editControl;
	CSpinButtonCtrl spinControl;
	CSliderCtrl slider1Control;
	CSliderCtrl slider2Control;
	CSliderCtrl slider3Control;
	CComboBox typeControl;
	CMFCColorButton colorControl;
	CListBox coordControl;
	CString vars;
	CSpinButtonCtrl minControl;
	CSpinButtonCtrl maxControl;
public:
	afx_msg void OnEnKillfocusEdit3();
	afx_msg void OnEnUpdateEdit3();
	afx_msg void OnEnKillfocusEdit4();
	afx_msg void OnEnUpdateEdit4();
};
