
// ThDy_DNAHybrid.MFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ThDy_DNAHybrid.MFC.h"
#include "ThDy_DNAHybrid.MFCDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CThDy_DNAHybridMFCDlg dialog



CThDy_DNAHybridMFCDlg::CThDy_DNAHybridMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CThDy_DNAHybridMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//ProjectFileName.( ES_MULTILINE);
}

void CThDy_DNAHybridMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ProjectFileName, ProjectFileName);
	DDX_Control(pDX, IDC_TAB1, programsTabs);
}

BEGIN_MESSAGE_MAP(CThDy_DNAHybridMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SaveProject, &CThDy_DNAHybridMFCDlg::OnBnClickedSaveproject)
	ON_EN_CHANGE(IDC_ProjectFileName, &CThDy_DNAHybridMFCDlg::OnEnChangeProjectfilename)
	ON_BN_CLICKED(IDC_LoadProject, &CThDy_DNAHybridMFCDlg::OnBnClickedLoadproject)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CThDy_DNAHybridMFCDlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CThDy_DNAHybridMFCDlg message handlers

BOOL CThDy_DNAHybridMFCDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	TCITEM setItem;
	setItem.mask=TCIF_TEXT; 

	setItem.pszText=L"Find Sonden";		programsTabs.InsertItem(0,&setItem);
	setItem.pszText=L"µArray";			programsTabs.InsertItem(1,&setItem);
	setItem.pszText=L"PCR Design";		programsTabs.InsertItem(2,&setItem);
	setItem.pszText=L"Multiplex PCR";	programsTabs.InsertItem(3,&setItem);
	setItem.pszText=L"Tm Calc";			programsTabs.InsertItem(4,&setItem);
	setItem.pszText=L"Setup";			programsTabs.InsertItem(5,&setItem);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CThDy_DNAHybridMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CThDy_DNAHybridMFCDlg::OnPaint()
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
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CThDy_DNAHybridMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CThDy_DNAHybridMFCDlg::OnBnClickedSaveproject()
{
	// TODO: Add your control notification handler code here
	CFileDialog dlgFile(TRUE);
CString fileName;
const int c_cMaxFiles = 100;
const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(c_cbBuffSize);
dlgFile.GetOFN().nMaxFile = c_cbBuffSize;

dlgFile.DoModal();
//fileName.ReleaseBuffer();
ProjectFileName.SetWindowTextW(dlgFile.GetOFN().lpstrFile);
//ProjectFileName.UpdateWindow();



}


void CThDy_DNAHybridMFCDlg::OnEnChangeProjectfilename()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	;
	// TODO:  Add your control notification handler code here
//	_Pr.ProjetFile( ProjectFileName.GetGetDlgItemText(0));
}


void CThDy_DNAHybridMFCDlg::OnBnClickedLoadproject()
{
	// TODO: Add your control notification handler code here
		CFileDialog dlgFile(TRUE);
CString fileName;
const int c_cMaxFiles = 100;
const int c_cbBuffSize = (c_cMaxFiles * (MAX_PATH + 1)) + 1;
dlgFile.GetOFN().lpstrFile = fileName.GetBuffer(c_cbBuffSize);
dlgFile.GetOFN().nMaxFile = c_cbBuffSize;

dlgFile.DoModal();
//fileName.ReleaseBuffer();
ProjectFileName.SetWindowTextW(dlgFile.GetOFN().lpstrFile);
//ProjectFileName.UpdateWindow();
}


void CThDy_DNAHybridMFCDlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}
