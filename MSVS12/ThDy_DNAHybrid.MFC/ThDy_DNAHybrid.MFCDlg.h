
// ThDy_DNAHybrid.MFCDlg.h : header file
//

#pragma once
#include "ThDy_programs\init_thdy_prog_param.h"      // aqui esta ya : #include "..\ThDySec\common_basics.h" 
#include "afxwin.h"
#include "afxcmn.h"


// CThDy_DNAHybridMFCDlg dialog
class CThDy_DNAHybridMFCDlg : public CDialogEx
{
// Construction
public:
	CThDy_DNAHybridMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_THDY_DNAHYBRIDMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	ThDyProject		_Pr;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSaveproject();
	afx_msg void OnEnChangeProjectfilename();
	CEdit ProjectFileName;
	afx_msg void OnBnClickedLoadproject();
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl programsTabs;
};
