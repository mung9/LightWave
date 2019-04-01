#pragma once

#include "CadManager.h"
#include "Shape\\DS_Rect.h"

class Survey;

// SurveyView 대화 상자
namespace MapEx {
	class SurveyView : public CDialogEx
	{
		DECLARE_DYNAMIC(SurveyView)

	public:
		SurveyView(ProgramManager::CCadManager* pManager=NULL, CWnd* pParent = nullptr); // 표준 생성자
		virtual ~SurveyView();

		void setManager(ProgramManager::CCadManager* pManager);
		void setMapRect(DataType::ShapeType::CDS_Rect rect);
		int addSurvey(Survey survey);

		// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
		enum { IDD = IDD_SURVEY_VIEW_LAYER };
#endif

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

		virtual BOOL OnInitDialog();

		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnPaint();

	protected:
		DataType::ShapeType::CDS_Rect m_mapRect;

	public:
		void updatePos();

	private:
		std::vector<Survey> m_surveys;
		ProgramManager::CCadManager* m_pManager = NULL;
	};

}

