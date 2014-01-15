
// BarChartDlg.cpp : implementation file
//

#include "stdafx.h"
#include "BarChart.h"
#include "BarChartDlg.h"
#include "afxdialogex.h"
#include "msxml2.h" 
#import "msxml4.dll"

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

long clorPlate[] = {RGB(51,102,255), RGB(102,51,255), RGB(204,51,255), RGB(255,51,204),RGB(255,51,102),RGB(255,102,51),RGB(255,204,51),RGB(204,255,51),RGB(102,255,51),RGB(51,255,102),RGB(51,255,204),RGB(51,204,255),RGB(0,61,245),RGB(0,46,184),RGB(245,184,0),RGB(184,138,0)};

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


// CBarChartDlg dialog



CBarChartDlg::CBarChartDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBarChartDlg::IDD, pParent)
	, step(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CBarChartDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CBarChartDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_PAINT_BTN, &CBarChartDlg::OnBnClicked)
END_MESSAGE_MAP()


// CBarChartDlg message handlers

BOOL CBarChartDlg::OnInitDialog()
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
	if (!m_toolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_RIGHT | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) || !m_toolBar.LoadToolBar(IDR_CHART_TOOLBAR))
	{
		
		return -1;
	}

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CBarChartDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CBarChartDlg::OnPaint()
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
		// Draw the icon
		//CDialogEx::OnPaint();
	}

	if (data.size() == 0)
	{
		CDialogEx::OnPaint();
		return;
	}
	this->PaintChart();
	//CDialogEx::OnPaint();
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CBarChartDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CBarChartDlg::OnBnClicked()
{
	// TODO: Add your control notification handler code here
	data.clear();
	days.clear();
	this->loadXML("E:/works/sample.xml");
	this->Invalidate(false);
}


int CBarChartDlg::loadXML(char* name)
{
	CoInitialize(NULL);

	wchar_t** xlabel;
	wchar_t** ylabel;
	
	IXMLDOMDocumentPtr doc;
	VARIANT var;
	var.pcVal = name;
	VARIANT_BOOL *b = new VARIANT_BOOL;

	if (SUCCEEDED(doc.CreateInstance("MsXML2.DOMDocument.4.0")))
	{
		_variant_t vn(name);
		doc->load(vn, b);
		if (*b == VARIANT_TRUE)
		{
			IXMLDOMNodeList *nodes = NULL;
			IXMLDOMElement *root = NULL;
			HRESULT res = doc->get_documentElement(&root);
			IXMLDOMNode *node = NULL;
			//nodes->
			if (SUCCEEDED(res))
			{
				BSTR str;
				root->get_nodeName(&str);
				MessageBox(str);
				res = root->getElementsByTagName(_bstr_t("days"), &nodes);
				if (SUCCEEDED(res))
				{
					res = nodes->get_item(0, &node);
					if (SUCCEEDED(res))
					{
						res = node->get_text(&str);
						if (SUCCEEDED(res))
						{
							size_t len = wcslen(str);
							char *buf = new char[10];
							int base = 0;
							for (int i = 0; i <= len; i++)
							{
								if ((str)[i] == _T(',') || str[i] == _T('\0'))
								{
									wchar_t* s = new wchar_t[i-base+1];
									int si = base;
									for (; si < i; si++)
									{
										s[si-base] = str[si];
									}
									s[si-base] = _T('\0');
									days.push_back(s);
									base = i + 1;
								}
							}

						}
					}
				}

				//解析sample数据
				nodes = NULL;
				IXMLDOMNamedNodeMap *attr = NULL;
				if (SUCCEEDED(root->getElementsByTagName(_bstr_t("sample"), &nodes)) && NULL != nodes)
				{
					auto len = 0L;
					if (SUCCEEDED(nodes->get_length(&len)))
					{
						//遍历sample
						for (auto i = 0; i < len; i++)
						{
							node = NULL;
							attr = NULL;
							if (SUCCEEDED(nodes->get_item(i, &node)) && NULL != node)
							{
								map<wchar_t*, double> smp;
								BSTR nodeName;

								//获取名称
								if (SUCCEEDED(node->get_attributes(&attr)) && NULL != attr)
								{
									IXMLDOMNode *attrNode = NULL;
									if (SUCCEEDED(attr->getNamedItem(_bstr_t("name"), &attrNode)) && NULL != attrNode)
									{
										if (SUCCEEDED(attrNode->get_text(&nodeName)))
										{
											//data.insert(pair<wchar_t*, map<wchar_t*, double>>(nodeName, smp));
										}
									}
								}

								//获取数据
								IXMLDOMNodeList *values = NULL;
								long size = 0;
								if (SUCCEEDED(node->get_childNodes(&values)) && NULL != values && SUCCEEDED(values->get_length(&size)))
								{
									for (int vi = 0; vi < size; vi++)
									{
										BSTR value = NULL, day = NULL;
										IXMLDOMNode *tmp = NULL, *tmp2 = NULL;
										IXMLDOMNamedNodeMap *dayMap = NULL;
										if (SUCCEEDED(values->get_item(vi, &tmp)) && NULL != tmp
											&& SUCCEEDED(tmp->get_text(&value)) && NULL != value
											&& SUCCEEDED(tmp->get_attributes(&dayMap)) && NULL != dayMap
											&& SUCCEEDED(dayMap->getNamedItem(_bstr_t("day"), &tmp2)) && NULL != tmp2
											&& SUCCEEDED(tmp2->get_text(&day)) && NULL != day)
										{
											auto it = days.begin();
											while (it != days.end())
											{
												if (0 == wcscmp(day, *it))
												{
													wchar_t *stopSign = NULL;
													smp.insert(pair<wchar_t*, double>(*it, wcstod(value, &stopSign)));
													break;
												}
												it++;
											}
										}
									}
									data.insert(pair<wchar_t*, map<wchar_t*, double>>(nodeName, smp));
								}
							}
						}// end 遍历sample
					}
				}
			}
		}
	}
	analysisData();
	return 0;
}


void CBarChartDlg::PaintChart()
{
	CPaintDC dc(this);
	int margin = 10;
	int yLabelWt = 20;
	int xLabelHt = 10;
	int legendHt = 20;

	CRect rect(10, 10, 100, 100);
	GetClientRect(&rect);
	
	CPen pen(PS_SOLID, 2, RGB(0, 0, 0));
	CPen *old = dc.SelectObject(&pen);

	int w = rect.Width();
	int h = rect.Height();
	int cw = w - margin * 2 - yLabelWt;
	int ch = h - margin * 2 - xLabelHt - legendHt;
	int legendW = 30;
	int legendLineW = 10;

	int x = rect.Width() - 20;
	int y = rect.Height() - 20;

	//画两个坐标轴
	dc.MoveTo(margin + yLabelWt, margin);
	dc.LineTo(margin + yLabelWt, h - margin - legendHt - xLabelHt);
	dc.LineTo(w - margin, h - margin - legendHt - xLabelHt);

	//画x轴标示
    for (int i = 0; i < days.size(); i++)
    {
		CString str(days[i]);
		rect.left = margin + yLabelWt+(i + 1)
			* (w - margin * 2 - yLabelWt) / (days.size() + 1);
		rect.right = rect.left + 20;
		rect.top = h - margin-legendHt - xLabelHt;
		rect.bottom = h - margin - legendHt;
		dc.DrawText(str, &rect, DT_SINGLELINE|DT_VCENTER);
    }

    int stepNum = (int) ((maxd - mind) / step);
	CString str;

	//画y轴坐标
    for (int i = 0; i <= stepNum; i++)
    {
		rect.top = (h - margin
            - legendHt - xLabelHt)
            - i * ch / (stepNum) + 10;
		rect.bottom = (h - margin
            - legendHt - xLabelHt)
            - i * ch / (stepNum);
		rect.left = margin;
		rect.right = rect.left + 40;
		str.Format(_T("%2f"), mind + i * step);
        dc.DrawText(str, &rect, DT_SINGLELINE|DT_VCENTER);

		if (i == 0)
		{
			continue;
		}

		CPen pen(PS_DASH, 1, RGB(105, 105, 105));
		CPen *old = dc.SelectObject(&pen);
		dc.MoveTo(margin + yLabelWt, (h - margin
            - legendHt - xLabelHt)
            - i * ch / (stepNum));
        dc.LineTo(w - margin, (h - margin - legendHt - xLabelHt) - i * ch / (stepNum));
		dc.SelectObject(old);
    }
    int index = 0;

	auto it = data.begin();
	while (it != data.end())
    {
        //Color clr = g2.getColor();
		//画图例
		str.Format(_T("%s"), it->first);
		rect.bottom = h - margin;
		rect.top = rect.bottom - legendHt;
		rect.left = margin + yLabelWt + legendW * index + legendLineW;
		rect.right = rect.left + legendW;
        dc.DrawText(str, &rect, DT_SINGLELINE|DT_VCENTER);
        //g2.setColor(colorMap[index % colorMap.length]);

		CPen pen(PS_SOLID, 3, clorPlate[index%16]);
		CPen *old = dc.SelectObject(&pen);
		dc.MoveTo(margin + yLabelWt + legendW * index, h - margin - legendHt / 2);
        dc.LineTo(margin + yLabelWt + legendW * index + legendLineW, h - margin - legendHt / 2);

		if (FALSE)
        {
            int tmp = days.size() * (data.size() + 1) + 1;
            for (int i = 0; i < days.size(); i++)
            {
                //g2.setBackground(g2.getColor());
				dc.Rectangle(margin + yLabelWt + cw/tmp*(1 + data.size() * i + i + index),
                    (int)((h - margin - legendHt - xLabelHt) - (it->second[days[i]] - mind)/step * ch / (stepNum)),
					margin + yLabelWt + cw/tmp*(1 + data.size() * i + i + index) + cw/tmp, (int)((h - margin
                        - legendHt - xLabelHt) - (it->second[days[i]] - mind)/step * ch / (stepNum) + (it->second[days[i]] - mind)/step*ch/(stepNum)));
                if (0 == index)
                {
					str.Format(_T("%s"), days[i]);
					rect.bottom = h - margin-legendHt;
					rect.top = rect.bottom - xLabelHt;
					rect.left = margin + yLabelWt + cw/tmp*(1 + data.size() * i + data.size()/2 + i + index);
					rect.right = rect.left + 30;
                    dc.DrawText(str, &rect, DT_SINGLELINE|DT_VCENTER);
                }
            }
        } else
        {
            //Stroke old = g2.getStroke();
            //g2.setStroke(new BasicStroke(2));
            int y1 = 0, y2 = 0;
            for (int i = 0; i < days.size(); i++)
            {
                if (i == 0)
                {
					y1 = (int) (h - margin - legendHt - xLabelHt - (it->second[days[i]] - mind)/step*ch/(stepNum));
                    continue;
                }
				y2 = (int) (h - margin - legendHt - xLabelHt - (it->second[days[i]] - mind)/step*ch/(stepNum));
				dc.MoveTo(margin + yLabelWt+(i) * (w - margin * 2 - yLabelWt) / (days.size() + 1), y1);
                dc.LineTo(margin + yLabelWt+(i + 1) * (w - margin * 2 - yLabelWt) / (days.size() + 1), y2);
                y1=y2;
            }
            //g2.setStroke(old);
        }

		dc.SelectObject(old);

        index++;
		it++;
    }
	
	dc.SelectObject(old);
}


void CBarChartDlg::analysisData(void)
{
	bool find = false;
	double max = 0;
	double min = 0;

	if (data.size() == 0)
	{
		return;
	}

	auto it = data.begin();
	while (it != data.end())
	{
		auto smp = it->second;
		auto it2 = smp.begin();
		while(it2 != smp.end())
		{
			if (!find)
			{
				max = it2->second;
				min = it2->second;
				find = true;
				continue;
			}

			max = max > it2->second ? max : it2->second;
			min = min < it2->second ? min : it2->second;
			it2++;
		}
		it++;
	}

	

        double delta = max - min;
        if (delta == 0)
        {
            delta = max;
        }
        if (delta == 0)
        {
            delta = 1;
        }

        string d;
		stringstream ss;
		ss<<delta;
		ss>>d;

        int magnitude = 0;

        if (d[0] == '0')
        {
            for (int i = 2; i < d.length(); i++)
            {
                if (d[i] != '.' && d[i] != '0')
                {
                    magnitude = 1 - i;
                    break;
                }
            }
        } else
        {
			magnitude = d.length();
			for (int i = 0; i < magnitude; i++)
			{
				if (d[i] == '.')
				{
					magnitude = i;
					break;
				}
			}
        }
        double tmp = magnitude > 0 ? pow(10, magnitude - 1) : pow(10,
                magnitude);
        double tail = mod(delta, tmp);
        int head = (int) (delta - tail) + 2;
        step = 1;
        if (head > 10)
        {
            step = 2;
        } else if (head < 2)
        {
            step = 0.2;
        } else if (head < 5)
        {
            step = 0.5;
        }

        mind = min - mod(min, step);
        if (mind > min)
        {
            mind -= step;
        }

        maxd = max - mod(max, step) + step;
        if (maxd < max)
        {
            maxd += step;
        }
}
