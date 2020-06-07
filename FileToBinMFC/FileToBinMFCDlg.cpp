
// FileToBinMFCDlg.cpp : implementation file
//

#include "stdafx.h"
#include "FileToBinMFC.h"
#include "FileToBinMFCDlg.h"
#include "afxdialogex.h"
#include "FileOperation.h"

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


// CFileToBinMFCDlg dialog




CFileToBinMFCDlg::CFileToBinMFCDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFileToBinMFCDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CFileToBinMFCDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFileToBinMFCDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_INIT, &CFileToBinMFCDlg::OnBnClickedButtonInit)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CFileToBinMFCDlg::OnBnClickedButtonAdd)
END_MESSAGE_MAP()


// CFileToBinMFCDlg message handlers

BOOL CFileToBinMFCDlg::OnInitDialog()
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

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFileToBinMFCDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CFileToBinMFCDlg::OnPaint()
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
HCURSOR CFileToBinMFCDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CFileToBinMFCDlg::OnBnClickedButtonInit()
{ 
	fileOperationObj.removeOutputBinFile();
}


void CFileToBinMFCDlg::OnBnClickedButtonAdd()
{
	try
	{ 
		int i=0;
		TCHAR szFilter[] = _T("文本文件(*.c)|*.c|所有文件(*.*)|*.*||");
		CFileDialog fileDlg(TRUE, _T("txt"), NULL, OFN_ALLOWMULTISELECT, szFilter, this);
		//DWORD MAXFILE = 8; //2562 is the max
		//fileDlg.m_ofn.nMaxFile=MAXFILE;

		char fileName[256] = {0};
		CArray<CString, CString> array_filename;

		CString strFilePath; 
		if (IDOK == fileDlg.DoModal())
		{
			POSITION pos_file;
			pos_file = fileDlg.GetStartPosition();
          	 
			while(pos_file != NULL)
			{
				strFilePath = fileDlg.GetNextPathName(pos_file);
				array_filename.Add(strFilePath);
				SetDlgItemText(IDC_EDIT1,  strFilePath);				
				_snprintf_s(fileName, 256, "%S", strFilePath.GetBuffer());
				fileOperationObj.readTextWriteToBinMain(fileName);
			}//end while       
			
			//we write the log
			char fileName[256] = {0};
			int arrayFileNameSize=array_filename.GetSize();
			char * outputLogName=fileOperationObj.getOutputLogName();
			char * insertReturn="\n";
			char * insertLogStart="txt file array, process sequence is:\n";

			//get current time  
			CTime ctCurrentTime = CTime::GetCurrentTime();
			CString strCurrentTime = ctCurrentTime.Format("%Y-%m-%d %H:%M:%S"); 
			fileOperationObj.writeLogFile(outputLogName, insertReturn);
			_snprintf_s(fileName, 256, "%S", strCurrentTime.GetBuffer());
			fileOperationObj.writeLogFile(outputLogName, fileName);
			fileOperationObj.writeLogFile(outputLogName, insertReturn);

			//log start
			fileOperationObj.writeLogFile(outputLogName, insertLogStart);
		
			for(i=0; i<arrayFileNameSize; i++)
			{
				CString arryData= array_filename.GetAt(i);
			
				_snprintf_s(fileName, 256, "%S", arryData.GetBuffer());
				fileOperationObj.writeLogFile(outputLogName, insertReturn);
				fileOperationObj.writeLogFile(outputLogName, fileName);

			}//end for
			//end of write log
		}
	}
	catch(...)
	{
		
	}
}
