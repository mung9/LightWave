// SurveyImageViewDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SurveyImageViewDlg.h"
#include "afxdialogex.h"


// SurveyImageViewDlg 대화 상자

IMPLEMENT_DYNAMIC(SurveyImageViewDlg, CDialogEx)


SurveyImageViewDlg::SurveyImageViewDlg(CWnd * pParent)
	: CDialogEx(IDD_DLG_SURVEY_IMAGE, pParent)
{
}

SurveyImageViewDlg::SurveyImageViewDlg(CString imagePath, CWnd * pParent)
	: CDialogEx(IDD_DLG_SURVEY_IMAGE, pParent)
{
	m_imagePath = imagePath;
}

SurveyImageViewDlg::~SurveyImageViewDlg()
{
}

BOOL SurveyImageViewDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	if (m_image) {
		const int width = m_image.GetWidth();
		const int height = m_image.GetHeight();
		rect = { 0, 0, width, height };
	}
	
	bool result = m_imageContainer.Create(
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | SS_BITMAP | SS_CENTERIMAGE | SS_NOTIFY,
		rect,
		this, 0
	);
	ASSERT(result);

	m_imageContainer.ShowWindow(SW_SHOW);
	//m_image.Load(m_imagePath);
	//SetWindowPos(NULL, 0, 0, m_image.GetWidth(), m_image.GetHeight(), SWP_NOMOVE);
	
	return TRUE;
}

void SurveyImageViewDlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;

	CImage image;
	image.Load(m_imagePath);
	SetWindowPos(NULL, 0, 0, image.GetWidth(), image.GetHeight(), SWP_NOMOVE);

	CBitmap* pBitmap;
	pBitmap = CBitmap::FromHandle(image);
	HBITMAP result = m_imageContainer.SetBitmap(image);

	// Now it would be supposed to fit the control:
	CDC *pDC = m_imageContainer.GetDC();
	CDC m_DC;

	m_DC.CreateCompatibleDC(pDC);
	m_DC.SelectObject(pBitmap);
	pDC->BitBlt(0, 0, image.GetWidth(), image.GetHeight(), &m_DC, 0, 0, SRCCOPY);
	pBitmap->DeleteObject();
	m_DC.DeleteDC();
	//pDC->StretchBlt(0, 0, 50,50, &m_DC, 0, 0, m_image.GetWidth(), m_image.GetHeight(), SRCCOPY);

	/*ReleaseDC(pDC);
	ReleaseDC(&m_DC);*/
}

void SurveyImageViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(SurveyImageViewDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// SurveyImageViewDlg 메시지 처리기
