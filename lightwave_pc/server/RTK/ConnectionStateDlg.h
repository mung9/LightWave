#pragma once

#include "SocketWorker.h"

#define CUSTOM_WM_UPDATE (WM_APP+1)

// ConnectionStateDlg 대화 상자

class ConnectionStateDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConnectionStateDlg)

public:
	ConnectionStateDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ConnectionStateDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CONNECTION_STATE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnClose();
	afx_msg void OnCreate();
	afx_msg BOOL OnInitDialog();

	void Update();
	void Append(Service::SocketWorker& socketWorker);
	void SetStaticTextColor(CStatic& target, COLORREF color);
	bool GetSelectedIpAndPort(CString& ipAddr, UINT& port) const;

	const CString START_SERVER = "Start Server";
	const CString STOP_SERVER = "Stop Server";

private:
	CStatic m_staticConnectionCount;
	CStatic m_staticIsListening;
	COLORREF m_colorOfServerState;

	CButton m_btnStartServer;

	std::mutex mutexState;	
public:
	afx_msg void OnBnClickedButtonToggleServer();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEnChangeEditMyPort();
	afx_msg void OnDisconnect();
	afx_msg void OnContextMenu(CWnd * pWnd, CPoint point);
	CListCtrl m_listConnections;
	CString m_ipAddr;
	UINT m_port;
};
