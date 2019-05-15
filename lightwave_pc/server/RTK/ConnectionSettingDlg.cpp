// ConnectionSettingDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ConnectionSettingDlg.h"
#include "WorkerManager.h"
#include "afxdialogex.h"


// ConnectionSettingDlg 대화 상자

IMPLEMENT_DYNAMIC(ConnectionSettingDlg, CDialogEx)

ConnectionSettingDlg::ConnectionSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CONNECTION_SETTING, pParent)
{
	CString ipAddr;
	UINT port;
	WorkerManager::GetInstance()->GetIpAddrAndPort(m_ipAddr, m_port);
}

ConnectionSettingDlg::~ConnectionSettingDlg()
{
}

UINT ConnectionSettingDlg::GetPort() const
{
	return m_port;
}

void ConnectionSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_port);
	DDX_Text(pDX, IDC_EDIT_IP, m_ipAddr);
}


BEGIN_MESSAGE_MAP(ConnectionSettingDlg, CDialogEx)
END_MESSAGE_MAP()


// ConnectionSettingDlg 메시지 처리기
