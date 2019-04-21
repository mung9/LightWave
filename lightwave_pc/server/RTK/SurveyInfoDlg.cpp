// SurveyInfoDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "SurveyInfoDlg.h"
#include "afxdialogex.h"


// SurveyInfoDlg 대화 상자

IMPLEMENT_DYNAMIC(SurveyInfoDlg, CDialogEx)

SurveyInfoDlg::SurveyInfoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SURVEY_INFO, pParent)
	, m_memo(_T(""))
{

}

SurveyInfoDlg::SurveyInfoDlg(const SurveyTask::Survey & survey, CWnd * pParent /*=nullptr*/)
	:SurveyInfoDlg(pParent)
{
	updateSurvey(survey);
}

SurveyInfoDlg::~SurveyInfoDlg()
{
}

void SurveyInfoDlg::setSurvey(const SurveyTask::Survey & survey)
{
	updateSurvey(survey);
}

void SurveyInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_MEMO, m_memo);
}

void SurveyInfoDlg::updateSurvey(SurveyTask::Survey survey)
{
	m_memo = survey.getMemo();
}


BEGIN_MESSAGE_MAP(SurveyInfoDlg, CDialogEx)
END_MESSAGE_MAP()


// SurveyInfoDlg 메시지 처리기
