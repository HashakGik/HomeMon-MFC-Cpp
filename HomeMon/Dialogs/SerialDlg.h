#pragma once
#include "afxwin.h"
#include "../Utils/Serial.h"

#include <vector>
#include <string>

// CSerialDlg dialog

class CSerialDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSerialDlg)

public:
	CSerialDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSerialDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
private:
	Serial serial;
	std::vector<std::string> ports;
	bool justCreated;
	CComboBox portControl;
	CEdit baudControl;
	CString port;
	int baud;
};
