
// BarChartDlg.h : header file
//

#pragma once
#include "afxext.h"

#include <vector>
#include <map>
#include <sstream>
#include <cmath>

#define mod(a, b) (a - ((int)(a / b))*b)

using namespace std;

// CBarChartDlg dialog
class CBarChartDlg : public CDialogEx
{
// Construction
public:
	CBarChartDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_BARCHART_DIALOG };

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
private:
	CToolBar m_toolBar;
public:
	afx_msg void OnBnClicked();
private:
	int loadXML(char* name);
protected:
	void PaintChart();
	void analysisData(void);
private:
	map<wchar_t*, map<wchar_t*, double>> data;
	vector<wchar_t*> days;
	double step;
	double maxd;
	double mind;
};
