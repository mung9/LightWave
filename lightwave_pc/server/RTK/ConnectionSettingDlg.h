#pragma once


// ConnectionSettingDlg 대화 상자

class ConnectionSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(ConnectionSettingDlg)

public:
	ConnectionSettingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~ConnectionSettingDlg();

	UINT GetPort() const;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CONNECTION_SETTING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	UINT m_port;
};
