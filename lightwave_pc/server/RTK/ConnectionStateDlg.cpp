// ConnectionStateDlg.cpp: 구현 파일
//


#include "stdafx.h"
#include "SocketWorker.h"
#include "ConnectionStateDlg.h"
#include "MainFrm.h"
#include "afxdialogex.h"


// ConnectionStateDlg 대화 상자

IMPLEMENT_DYNAMIC(ConnectionStateDlg, CDialogEx)

ConnectionStateDlg::ConnectionStateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CONNECTION_STATE, pParent)
{
	m_colorOfServerState = RGB(255, 0, 0);
}

ConnectionStateDlg::~ConnectionStateDlg()
{

}

void ConnectionStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CONNECTION, m_listBoxConnection);
	DDX_Control(pDX, IDC_STATIC_CONNECTION_COUNT, m_staticConnectionCount);
	DDX_Control(pDX, IDC_STATIC_IS_LISTENING, m_staticIsListening);
	DDX_Control(pDX, IDC_BUTTON_TOGGLE_SERVER, m_btnStartServer);
}

void ConnectionStateDlg::onClose()
{
	CDialogEx::OnClose();
	GetParent()->SendMessageA(WM_PARENTNOTIFY, WM_CLOSE);
}

void ConnectionStateDlg::onCreate()
{
	
}

void ConnectionStateDlg::update(bool isListening, const std::vector<std::shared_ptr<SocketWorker>> workers)
{
	std::unique_lock<std::mutex> lock(mutexState);
	m_listBoxConnection.ResetContent();
	for (auto pWorker : workers) {
		CString ipAddress;
		UINT port;
		pWorker->GetPeerName(ipAddress, port);
		m_listBoxConnection.AddString(ipAddress);
	}

	if (isListening) {
		setStaticTextColor(m_staticIsListening, RGB(0, 0, 0xff));
		m_staticIsListening.SetWindowTextA("On");
	}
	else {
		setStaticTextColor(m_staticIsListening, RGB(0xff, 0, 0));
		m_staticIsListening.SetWindowTextA("Off");
	}

	size_t workerCount = workers.size();
	CString workerCountInStr;
	workerCountInStr.Format("%d", workerCount);
	m_staticConnectionCount.SetWindowTextA(workerCountInStr);
}

void ConnectionStateDlg::setStaticTextColor(CStatic& target, COLORREF color)
{
	target.Invalidate();
}


BEGIN_MESSAGE_MAP(ConnectionStateDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_TOGGLE_SERVER, &ConnectionStateDlg::OnBnClickedButtonToggleServer)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ConnectionStateDlg 메시지 처리기


void ConnectionStateDlg::OnBnClickedButtonToggleServer()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	if (!pMainWnd->isServerListening()) {
		m_btnStartServer.SetWindowTextA(STOP_SERVER);
		m_colorOfServerState = RGB(0, 0, 255);
		pMainWnd->OnServerStart();
	}
	else {
		m_btnStartServer.SetWindowTextA(START_SERVER);
		m_colorOfServerState = RGB(255, 0, 0);
		pMainWnd->OnServerStop();
	}	
	m_staticIsListening.Invalidate();
}


HBRUSH ConnectionStateDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_IS_LISTENING) {
		pDC->SetTextColor(m_colorOfServerState);
	}
	
	return hbr;
}


void ConnectionStateDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	const int listBoxLeft = 17;
	const int listBoxTop = 70;
	const int listBoxWidth = cx-(listBoxLeft<<1);
	const int listBoxHeight = cy-listBoxTop-10;
	CRect listBoxRect(listBoxLeft, listBoxTop, listBoxLeft+listBoxWidth, listBoxTop+listBoxHeight);
	if (m_listBoxConnection.GetSafeHwnd()) {
		m_listBoxConnection.MoveWindow(&listBoxRect);
	}

	const int btnWidth = 90;
	const int btnHeight = 30;
	const int btnLeft = cx - btnWidth - 20;
	const int btnTop = 10;
	CRect btnRect(btnLeft, btnTop, btnLeft + btnWidth, btnTop + btnHeight);
	if (m_btnStartServer.GetSafeHwnd()) {
		m_btnStartServer.MoveWindow(&btnRect);
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}
