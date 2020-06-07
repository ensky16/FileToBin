
// FileToBinMFCDlg.h : header file
//

#pragma once

#include "FileOperation.h"

// CFileToBinMFCDlg dialog
class CFileToBinMFCDlg : public CDialogEx
{
// Construction
public:
	CFileToBinMFCDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_FILETOBINMFC_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonInit();
	afx_msg void OnBnClickedButtonAdd();
	
	FileOperation fileOperationObj;
};
