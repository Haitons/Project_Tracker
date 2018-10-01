
// Project_TrackerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Project_Tracker.h"
#include "Project_TrackerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include <opencv2/opencv.hpp>  

using namespace cv;
using namespace std;

// Global variables  
Rect box;
bool drawing_box = false;
bool gotBB = false;

// bounding box mouse callback  
void mouseHandler(int event, int x, int y, int flags, void *param){



	switch (event){
	case CV_EVENT_MOUSEMOVE:
		if (drawing_box){
			box.width = x - box.x;
			box.height = y - box.y;
		}
		break;


	case CV_EVENT_LBUTTONDOWN:
		drawing_box = true;
		box = Rect(x, y, 0, 0);
		break;
	case CV_EVENT_LBUTTONUP:
		drawing_box = false;
		if (box.width < 0){
			box.x += box.width;
			box.width *= -1;
		}
		if (box.height < 0){
			box.y += box.height;
			box.height *= -1;
		}
		gotBB = true;
		break;
	}
}


// tracker: get search patches around the last tracking box,  
// and find the most similar one  
void tracking(Mat frame, Mat &model, Rect &trackBox)
{
	Mat gray;
	cvtColor(frame, gray, CV_RGB2GRAY);

	Rect searchWindow;
	searchWindow.width = trackBox.width * 3;
	searchWindow.height = trackBox.height * 3;
	searchWindow.x = trackBox.x + trackBox.width * 0.5 - searchWindow.width * 0.5;
	searchWindow.y = trackBox.y + trackBox.height * 0.5 - searchWindow.height * 0.5;
	searchWindow &= Rect(0, 0, frame.cols, frame.rows);

	Mat similarity;
	matchTemplate(gray(searchWindow), model, similarity, CV_TM_CCOEFF_NORMED);

	double mag_r;
	Point point;
	minMaxLoc(similarity, 0, &mag_r, 0, &point);
	trackBox.x = point.x + searchWindow.x;
	trackBox.y = point.y + searchWindow.y;
	model = gray(trackBox);
}
// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CProject_TrackerDlg �Ի���



CProject_TrackerDlg::CProject_TrackerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProject_TrackerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CProject_TrackerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CProject_TrackerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CProject_TrackerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CProject_TrackerDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT1, &CProject_TrackerDlg::OnEnChangeEdit1)
	ON_LBN_SELCHANGE(IDC_LIST1, &CProject_TrackerDlg::OnLbnSelchangeList1)
END_MESSAGE_MAP()


// CProject_TrackerDlg ��Ϣ�������

BOOL CProject_TrackerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��
s
	// TODO:  �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CProject_TrackerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CProject_TrackerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC  dc(this);
		CRect  rect;
		GetClientRect(&rect);
		CDC  dcMem;
		dcMem.CreateCompatibleDC(&dc);
		CBitmap  bmpBackground;
		bmpBackground.LoadBitmap(IDB_BITMAP1);
		//IDB_BITMAP�����Լ���ͼ��Ӧ��ID
		BITMAP  bitmap;
		bmpBackground.GetBitmap(&bitmap);
		CBitmap  *pbmpOld = dcMem.SelectObject(&bmpBackground); dc.StretchBlt(0, 0, rect.Width(), rect.Height(), &dcMem, 0, 0,
		bitmap.bmWidth, bitmap.bmHeight, SRCCOPY);

	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CProject_TrackerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CProject_TrackerDlg::OnBnClickedButton1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	VideoCapture capture;

	capture.open(0);

	bool fromfile = true;
	//Init camera  
	if (!capture.isOpened())
	{
		cout << "capture device failed to open!" << endl;
		return;
	}
	//Register mouse callback to draw the bounding box  
	cvNamedWindow("Tracker", CV_WINDOW_NORMAL);
	cvSetMouseCallback("Tracker", mouseHandler, NULL);

	Mat frame, model;
	capture >> frame;
	while (!gotBB)
	{
		if (!fromfile)

			capture >> frame;

		imshow("Tracker", frame);
		if (cvWaitKey(20) == 'q')
			return;
	}
	//Remove callback  
	cvSetMouseCallback("Tracker", mouseHandler, 0);

	Mat gray;
	cvtColor(frame, gray, CV_RGB2GRAY);
	model = gray(box);

	int frameCount = 0;

	while (1)
	{
		capture >> frame;
		if (frame.empty())
			return;
		double t = (double)cvGetTickCount();
		frameCount++;

		// tracking  
		tracking(frame, model, box);

		// show  
		stringstream buf;
		buf << frameCount;
		string num = buf.str();
		putText(frame, num, Point(20, 20), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 3);
		rectangle(frame, box, Scalar(0, 0, 255), 3);
		imshow("Tracker", frame);


		t = (double)cvGetTickCount() - t;
		//cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;

		if (cvWaitKey(1) == 27)

			break;

	}
	cvDestroyWindow("Tracker");
	 return;
	
}


void CProject_TrackerDlg::OnBnClickedButton2()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	VideoCapture capture;

	capture.open("i.mov");

	bool fromfile = true;
	//Init camera  
	if (!capture.isOpened())
	{
		cout << "capture device failed to open!" << endl;
		return;
	}
	//Register mouse callback to draw the bounding box  
	cvNamedWindow("Tracker", CV_WINDOW_NORMAL);
	cvSetMouseCallback("Tracker", mouseHandler, NULL);

	Mat frame, model;
	capture >> frame;
	while (!gotBB)
	{
		if (!fromfile)

			capture >> frame;

		imshow("Tracker", frame);
		if (cvWaitKey(20) == 'q')
			return;
	}
	//Remove callback  
	cvSetMouseCallback("Tracker", mouseHandler, 0);

	Mat gray;
	cvtColor(frame, gray, CV_RGB2GRAY);
	model = gray(box);

	int frameCount = 0;

	while (1)
	{
		capture >> frame;
		if (frame.empty())
			return;
		double t = (double)cvGetTickCount();
		frameCount++;

		// tracking  
		tracking(frame, model, box);

		// show  
		stringstream buf;
		buf << frameCount;
		string num = buf.str();
		putText(frame, num, Point(20, 20), FONT_HERSHEY_SIMPLEX, 1, Scalar(0, 0, 255), 3);
		rectangle(frame, box, Scalar(0, 0, 255), 3);
		imshow("Tracker", frame);


		t = (double)cvGetTickCount() - t;
		//cout << "cost time: " << t / ((double)cvGetTickFrequency()*1000.) << endl;

		if (cvWaitKey(1) == 27)

			break;

	}
	cvDestroyWindow("Tracker");
	return;
}


void CProject_TrackerDlg::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CProject_TrackerDlg::OnLbnSelchangeList1()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}
