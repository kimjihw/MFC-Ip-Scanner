
// MFCApplication8Dlg.h: 헤더 파일
//

#pragma once

#include <winsock2.h>
#include <iphlpapi.h>
#include <stdio.h>
#include <stdlib.h>
#pragma comment(lib, "IPHLPAPI.lib")

// CMFCApplication8Dlg 대화 상자
class CMFCApplication8Dlg : public CDialogEx
{
// 생성입니다.
public:
	CMFCApplication8Dlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION8_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	CButton m_Add;
	CButton m_Del;
	CIPAddressCtrl m_Ip;
	CListCtrl m_Network;
	CComboBox m_Niccombo;
	CButton m_Select;
	afx_msg void OnCbnSelchangeNiccombo();

	afx_msg void OnBnClickedSelect();
	afx_msg void OnBnClickedAdd();
	afx_msg void OnBnClickedDel();
	afx_msg void OnLvnItemchangedNetwork(NMHDR* pNMHDR, LRESULT* pResult);

	/* variables used to print DHCP time info */
	struct tm newtime;
	char buffer[32];
	errno_t error;

	afx_msg void OnLbnSelchangeList1();
	afx_msg void OnLbnSelchangeList();
	CListCtrl m_Nic;
	afx_msg void OnLvnItemchangedNic(NMHDR* pNMHDR, LRESULT* pResult);

	void EnableControls(BOOL bEnable);


	CString CMFCApplication8Dlg::ExecCmd();
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	int incIP(IPAddr*);

	int getMacString(BYTE* mac, char* mac_str);
	struct in_addr DestIpStruct;

	IPAddr DestIp = 0;
	IPAddr SrcIp = 0;
	IPAddr NetwIp = 0;
	IPAddr MaskIp = 0;
	int i = 0;
	int inum;
	char ip_str[15], maskip_str[15], mac_str[18];

	ULONG MacAddr[2]; // MAC addr = 6 bytes
	ULONG PhysAddrLen;
	ULONG hosts_count = 0;


	IP_ADAPTER_INFO* pAdapterInfo;
	ULONG ulOutBufLen = sizeof(IP_ADAPTER_INFO);
	ULONG dwRetVal = 0; // Errors


	CButton m_Scan;
	afx_msg void OnBnClickedScan();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	CWinThread* m_pThread;
	CStatic m_staticDisp;

	static UINT ThreadFuction(LPVOID lpvoid);

	CString ExecCmd(LPCSTR pCmdArg);


	afx_msg void OnIpnFieldchangedIp(NMHDR* pNMHDR, LRESULT* pResult);
	// CListCtrl m_Ping;
	// 	CListBox m_ping;// CListCtrl m_Ping;
	// 	CListBox m_ping;// CListCtrl m_Ping;
// 	CListBox m_ping;// CListCtrl m_Ping;
// 	CListBox m_ping;
	CEdit m_Host;
	afx_msg void OnEnChangeHost();
	CButton m_Save;
	CEdit m_Edit;
	afx_msg void OnEnChangeEdit1();

	CButton m_Stop;
	afx_msg void OnBnClickedStop();
	CButton m_Resume;
	afx_msg void OnBnClickedResume();
};

