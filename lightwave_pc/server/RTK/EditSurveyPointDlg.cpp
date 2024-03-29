// EditSurveyPointDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "EditSurveyPointDlg.h"
#include "TaskManager.h"
#include "time.h"
#include "afxdialogex.h"




// EditSurveyPointDlg 대화 상자

IMPLEMENT_DYNAMIC(EditSurveyPointDlg, CDialogEx)

EditSurveyPointDlg::EditSurveyPointDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_EDITSURVEYPOINT, pParent)
	, m_ptName(_T(""))
	, m_ptX(_T(""))
	, m_ptY(_T(""))
	, m_surveyedTime(_T(""))
	, m_ptId(_T(""))
{

}

EditSurveyPointDlg::~EditSurveyPointDlg()
{
}

void EditSurveyPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_POINTNAME, m_ptName);
	DDX_Text(pDX, IDC_EDIT_POINTX, m_ptX);
	DDX_Text(pDX, IDC_EDIT_POINTY, m_ptY);
	DDX_Text(pDX, IDC_EDIT_SURVEYTIME, m_surveyedTime);
	DDX_Text(pDX, IDC_EDIT_POINTID, m_ptId);
	DDX_Control(pDX, IDC_LIST_SURVEYPOINTLIST, m_listSurveys);
}

BOOL EditSurveyPointDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listSurveys.SetExtendedStyle(m_listSurveys.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CRect listRect;
	m_listSurveys.GetWindowRect(&listRect);
	const int width = listRect.Width();

	m_listSurveys.InsertColumn(1, _T("작업번호(id)"), LVCFMT_LEFT, (int)(width*0.25), 2);
	m_listSurveys.InsertColumn(2, _T("X"), LVCFMT_LEFT, (int)(width*0.25), 2);
	m_listSurveys.InsertColumn(3, _T("Y"), LVCFMT_LEFT, (int)(width*0.25), 2);
	m_listSurveys.InsertColumn(4, _T("SURVEYED"), LVCFMT_LEFT, (int)(width*0.25), 2);

	auto pTaskManager = ProgramManager::TaskManager::GetInstance();
	auto pLoadedTask = pTaskManager->GetLoadedTask();
	if (!pLoadedTask) return FALSE;

	m_surveys = pLoadedTask->GetSurveys();
	const int surveysCount = m_surveys.size();
	for (int i = 0; i < surveysCount; ++i) {
		char idInStr[10];
		itoa(m_surveys[i].GetId(), idInStr, 10);
		const int itemIndex = m_listSurveys.GetItemCount();
		int result = m_listSurveys.InsertItem(itemIndex, idInStr);

		CString xInStr, yInStr;
		xInStr.Format("%f", m_surveys[i].GetX());
		yInStr.Format("%f", m_surveys[i].GetY());
		m_listSurveys.SetItemText(itemIndex, 1, xInStr);
		m_listSurveys.SetItemText(itemIndex, 2, yInStr);
		m_listSurveys.SetItemText(itemIndex, 3, m_surveys[i].HasBeenSurveyed() ? "true" : "false");
	}

	return TRUE;
}


BEGIN_MESSAGE_MAP(EditSurveyPointDlg, CDialogEx)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_SURVEYPOINTLIST, &EditSurveyPointDlg::OnLvnItemActivateListSurveypointlist)
	ON_BN_CLICKED(IDC_BTN_APPLY, &EditSurveyPointDlg::OnBnClickedBtnApply)
END_MESSAGE_MAP()


// EditSurveyPointDlg 메시지 처리기


void EditSurveyPointDlg::OnLvnItemActivateListSurveypointlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	int index = pNMIA->iItem;
	m_pSelectedSurvey = &m_surveys[index];

	m_ptId.Format("%d", m_pSelectedSurvey->GetId());
	m_ptName = m_pSelectedSurvey->GetName();
	m_ptX.Format("%f", m_pSelectedSurvey->GetX());
	m_ptY.Format("%f", m_pSelectedSurvey->GetY());
	if (m_pSelectedSurvey->HasBeenSurveyed()) {
		m_surveyedTime = TimeUtil::ConvertTime2StrFormatted(m_pSelectedSurvey->GetUpdatedTime());
	}
	else {
		m_surveyedTime = _T("측량정보 없음.");
	}

	UpdateData(FALSE);

	*pResult = 0;
}

void EditSurveyPointDlg::OnBnClickedBtnApply()
{
	UpdateData();

	SurveyTask::Survey survey = m_pSelectedSurvey ? *m_pSelectedSurvey : SurveyTask::Survey();
	survey.SetName(m_ptName);
	survey.SetPoint(atof(m_ptX), atof(m_ptY));

	auto pTaskManager = ProgramManager::TaskManager::GetInstance();
	if (m_pSelectedSurvey) {
		*m_pSelectedSurvey = survey;
		bool patchResult = pTaskManager->GetLoadedTask()->PatchSurvey(m_pSelectedSurvey->GetId(), survey);
	}
	else {
		pTaskManager->RegisterSurvey(survey);
	}
}