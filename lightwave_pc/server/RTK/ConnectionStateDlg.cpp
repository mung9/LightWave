// ConnectionStateDlg.cpp: 구현 파일
//


#include "stdafx.h"
#include "ConnectionStateDlg.h"
#include "MainFrm.h"
#include "afxdialogex.h"


// ConnectionStateDlg 대화 상자

IMPLEMENT_DYNAMIC(ConnectionStateDlg, CDialogEx)

ConnectionStateDlg::ConnectionStateDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_CONNECTION_STATE, pParent)
{
	WorkerManager::GetInstance()->GetIpAddrAndPort(m_ipAddr, m_port);

	m_colorOfServerState = RGB(255, 0, 0);
}

ConnectionStateDlg::~ConnectionStateDlg()
{

}

void ConnectionStateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_CONNECTION_COUNT, m_staticConnectionCount);
	DDX_Control(pDX, IDC_STATIC_IS_LISTENING, m_staticIsListening);
	DDX_Control(pDX, IDC_BUTTON_TOGGLE_SERVER, m_btnStartServer);
	DDX_Control(pDX, IDC_LIST_CONNECTION, m_listConnections);
	DDX_Text(pDX, IDC_EDIT_MY_IP, m_ipAddr);
	DDX_Text(pDX, IDC_EDIT_MY_PORT, m_port);
}

void ConnectionStateDlg::OnClose()
{
	CDialogEx::OnClose();
	GetParent()->SendMessageA(WM_PARENTNOTIFY, WM_CLOSE);
}

void ConnectionStateDlg::OnCreate()
{

}

BOOL ConnectionStateDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_listConnections.SetExtendedStyle(m_listConnections.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	CRect listRect;
	m_listConnections.GetWindowRect(&listRect);
	const int width = listRect.Width();

	m_listConnections.InsertColumn(1, _T("NAME"), LVCFMT_LEFT, (int)(width*0.3), 2);
	m_listConnections.InsertColumn(2, _T("IP"), LVCFMT_LEFT, (int)(width*0.3), 2);
	m_listConnections.InsertColumn(3, _T("PORT"), LVCFMT_LEFT, (int)(width*0.3), 2);

	return TRUE;
}

void ConnectionStateDlg::Update()
{
	auto pManager = WorkerManager::GetInstance();

	auto workers = pManager->GetWorkers();

	m_listConnections.DeleteAllItems();
	for (auto pWorker : workers) {
		Append(*pWorker);
	}

	if (pManager->IsListening()) {
		m_colorOfServerState = RGB(0, 0, 255);
		m_staticIsListening.SetWindowTextA("On");
		m_btnStartServer.SetWindowTextA(STOP_SERVER);
	}
	else {
		m_colorOfServerState = RGB(255, 0, 0);
		m_staticIsListening.SetWindowTextA("Off");
		m_btnStartServer.SetWindowTextA(START_SERVER);
	}

	size_t workerCount = workers.size();
	CString workerCountInStr;
	workerCountInStr.Format("%d", workerCount);
	m_staticConnectionCount.SetWindowTextA(workerCountInStr);
}

void ConnectionStateDlg::Append(Service::SocketWorker & socketWorker)
{
	CString ipAddress;
	UINT port;
	socketWorker.GetPeerName(ipAddress, port);

	CString portInStr;
	portInStr.Format("%d", port);

	CString name = socketWorker.GetWorkerName();

	const int itemIndex = m_listConnections.GetItemCount();
	int result = m_listConnections.InsertItem(itemIndex, name == "" ? "{connecting...}" : name);
	m_listConnections.SetItemText(itemIndex, 1, ipAddress);
	m_listConnections.SetItemText(itemIndex, 2, portInStr);
}

void ConnectionStateDlg::SetStaticTextColor(CStatic& target, COLORREF color)
{
	target.Invalidate();
}

bool ConnectionStateDlg::GetSelectedIpAndPort(CString& ipAddr, UINT& port) const
{
	int mark = m_listConnections.GetSelectionMark();
	if (-1 == mark) return false;

	ipAddr = m_listConnections.GetItemText(mark, 1);
	port = _ttoi(m_listConnections.GetItemText(mark, 2));

	return true;
}


BEGIN_MESSAGE_MAP(ConnectionStateDlg, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_TOGGLE_SERVER, &ConnectionStateDlg::OnBnClickedButtonToggleServer)
	ON_WM_CTLCOLOR()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_EN_CHANGE(IDC_EDIT_MY_PORT, &ConnectionStateDlg::OnEnChangeEditMyPort)
	ON_COMMAND(ID_DISCONNECT, &ConnectionStateDlg::OnDisconnect)
END_MESSAGE_MAP()


// ConnectionStateDlg 메시지 처리기


void ConnectionStateDlg::OnBnClickedButtonToggleServer()
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetMainWnd();
	if (!pMainWnd->isServerListening()) {
		pMainWnd->OnServerStart();
	}
	else {
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
/*
	CRect rc;
	GetClientRect(&rc);

	if (m_listConnections&&m_btnStartServer) {
		CRect btnRect;
		m_btnStartServer.GetClientRect(&btnRect);
		const int height = rc.Height() - btnRect.bottom + 2;
		m_listConnections.SetWindowPos(NULL, 0, btnRect.bottom+2, rc.Width(), height, SWP_SHOWWINDOW | SWP_NOZORDER);
	}*/

	//const int listBoxLeft = 17;
	//const int listBoxTop = 70;
	//const int listBoxWidth = cx-(listBoxLeft<<1);
	//const int listBoxHeight = cy-listBoxTop-10;
	//CRect listBoxRect(listBoxLeft, listBoxTop, listBoxLeft+listBoxWidth, listBoxTop+listBoxHeight);
	//if (m_listConnections.GetSafeHwnd()) {
	//	m_listConnections.MoveWindow(&listBoxRect);
	//}

	//const int btnWidth = 90;
	//const int btnHeight = 30;
	//const int btnLeft = cx - btnWidth - 20;
	//const int btnTop = 10;
	//CRect btnRect(btnLeft, btnTop, btnLeft + btnWidth, btnTop + btnHeight);
	//if (m_btnStartServer.GetSafeHwnd()) {
	//	m_btnStartServer.MoveWindow(&btnRect);
	//}
}


void ConnectionStateDlg::OnEnChangeEditMyPort()
{
	UpdateData(true);
	WorkerManager::GetInstance()->SetPort(m_port);
}

void ConnectionStateDlg::OnContextMenu(CWnd * pWnd, CPoint point)
{
	CString ipAddr;
	UINT port;
	bool result = GetSelectedIpAndPort(ipAddr, port);
	if (!result) return;

	CMenu popup;
	CMenu* pMenu;
	popup.LoadMenu(IDR_CONNECTION_MNG_POPUP);
	pMenu = popup.GetSubMenu(0);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, point.x, point.y, this);
}

void ConnectionStateDlg::OnDisconnect()
{
	CString ipAddr;
	UINT port;
	bool result = GetSelectedIpAndPort(ipAddr, port);
	if (!result) return;

	auto pWorkerManager = WorkerManager::GetInstance();
	pWorkerManager->DeleteWorker(ipAddr, port);
}