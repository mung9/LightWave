// SurveyView.cpp: 구현 파일
//

#include "stdafx.h"
#include "SurveyView.h"
#include "Survey.h"
#include "afxdialogex.h"
#include "VeCad/cadapi.h"

using namespace ProgramManager;
using DataType::ShapeType::CDS_Rect;
using DataType::ShapeType::CDS_Point;

namespace MapEx {
	// SurveyView 대화 상자

	IMPLEMENT_DYNAMIC(SurveyView, CDialogEx)

	SurveyView::SurveyView(CCadManager * manager, CWnd * pParent)
	{
		m_pManager = manager;
	}

	SurveyView::~SurveyView()
	{
	}

	void SurveyView::setManager(ProgramManager::CCadManager * pManager)
	{
		m_pManager = pManager;
	}

	void SurveyView::setMapRect(CDS_Rect rect)
	{
		m_mapRect = rect;
	}

	int SurveyView::addSurvey(Survey survey)
	{
		m_surveys.push_back(survey);
		return m_surveys.size();
	}

	void SurveyView::DoDataExchange(CDataExchange* pDX)
	{
		CDialogEx::DoDataExchange(pDX);
	}

	BOOL SurveyView::OnInitDialog()
	{
		BOOL res = CDialogEx::OnInitDialog();
		if (!res) return FALSE;

		SetWindowLong(m_hWnd, GWL_EXSTYLE,
			GetWindowLong(m_hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);

		SetLayeredWindowAttributes(::GetSysColor(COLOR_3DFACE), (255 * 20) / 100, LWA_COLORKEY);
		//SetLayeredWindowAttributes(0, (255 * 50) / 100, LWA_ALPHA);

		return TRUE;
	}


	BEGIN_MESSAGE_MAP(SurveyView, CDialogEx)
		ON_WM_PAINT()
	END_MESSAGE_MAP()


	// SurveyView 메시지 처리기


	void SurveyView::OnPaint()
	{
		CPaintDC dc(this);

		CRect winRect;
		GetWindowRect(&winRect);

		CPen pen;
		pen.CreatePen(PS_SOLID, 5, RGB(255, 255, 0));
		SelectObject(dc, pen);
		
		for (Survey s : m_surveys) {
			long x, y;
			CadApi::CadCoordDispToWin(m_pManager->GetVDwg(), s.GetY(), s.GetX(), &x, &y);

			Ellipse(dc, x-3, y-3, x + 3, y + 3);
		}
		ReleaseDC(&dc);

	}

	void SurveyView::updatePos()
	{
		HWND hCadWnd = m_pManager->GetHwnd();
		CRect cadRect;
		::GetWindowRect(hCadWnd, &cadRect);

		SetWindowPos(NULL, cadRect.left, cadRect.top, cadRect.Width(), cadRect.Height(), NULL);
		Invalidate(TRUE);
	}
}


