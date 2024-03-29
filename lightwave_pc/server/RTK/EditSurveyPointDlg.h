#pragma once

#include "Survey.h"
// EditSurveyPointDlg 대화 상자

class EditSurveyPointDlg : public CDialogEx
{
	DECLARE_DYNAMIC(EditSurveyPointDlg)

public:
	EditSurveyPointDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~EditSurveyPointDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_EDITSURVEYPOINT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_ptName;
	CString m_ptX;
	CString m_ptY;
	CString m_surveyedTime;
	std::vector<SurveyTask::Survey> m_surveys;
	CListCtrl m_listSurveys;

	SurveyTask::Survey* m_pSelectedSurvey = nullptr;

	afx_msg BOOL OnInitDialog();
	afx_msg void OnLvnItemActivateListSurveypointlist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedBtnApply();
	CString m_ptId;
};
