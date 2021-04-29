#include "pch.h"
#include "framework.h"
#include "MFCApplication8.h"
#include "MFCApplication8Dlg.h"
#include "afxdialogex.h"
#include"stdafx.h"
#include <iphlpapi.h>
#include<WinSock2.h>
#include <tchar.h>
#include<windows.h>
#include<pcap.h>
#include <thread>


#pragma comment(lib, "Ws2_32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma warning(disable:4996)
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	CButton m_Ok;
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_Ok);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMFCApplication8Dlg 대화 상자



CMFCApplication8Dlg::CMFCApplication8Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MFCAPPLICATION8_DIALOG, pParent)
{	
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplication8Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ADD, m_Add);
	DDX_Control(pDX, IDC_DEL, m_Del);
	DDX_Control(pDX, IDC_IP, m_Ip);
	DDX_Control(pDX, IDC_NETWORK, m_Network);
	DDX_Control(pDX, IDC_NICCOMBO, m_Niccombo);
	DDX_Control(pDX, IDC_SELECT, m_Select);
	DDX_Control(pDX, IDC_NIC, m_Nic);
	DDX_Control(pDX, IDC_SCAN, m_Scan);
	DDX_Control(pDX, IDC_Host, m_Host);
	DDX_Control(pDX, IDC_STOP, m_Stop);
	DDX_Control(pDX, IDC_BUTTON2, m_Resume);
}

BEGIN_MESSAGE_MAP(CMFCApplication8Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_NICCOMBO, &CMFCApplication8Dlg::OnCbnSelchangeNiccombo)
	ON_BN_CLICKED(IDC_SELECT, &CMFCApplication8Dlg::OnBnClickedSelect)
	ON_BN_CLICKED(IDC_ADD, &CMFCApplication8Dlg::OnBnClickedAdd)
	ON_BN_CLICKED(IDC_DEL, &CMFCApplication8Dlg::OnBnClickedDel)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_NETWORK, &CMFCApplication8Dlg::OnLvnItemchangedNetwork)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_NIC, &CMFCApplication8Dlg::OnLvnItemchangedNic)
	ON_WM_CONTEXTMENU()
	ON_BN_CLICKED(IDC_SCAN, &CMFCApplication8Dlg::OnBnClickedScan)
	ON_NOTIFY(IPN_FIELDCHANGED, IDC_IP, &CMFCApplication8Dlg::OnIpnFieldchangedIp)
	ON_EN_CHANGE(IDC_Host, &CMFCApplication8Dlg::OnEnChangeHost)
	ON_BN_CLICKED(IDC_STOP, &CMFCApplication8Dlg::OnBnClickedStop)
	ON_BN_CLICKED(IDC_RESUME, &CMFCApplication8Dlg::OnBnClickedResume)
END_MESSAGE_MAP()


// CMFCApplication8Dlg 메시지 처리기

BOOL CMFCApplication8Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	/*CString strHost;
	CString s;

	int nEntrys = AfxGetApp()->GetProfileIntA("MRU", "Entires", 0);
	for (int n = 0; n < nEntrys; n++) {
		s.Format("Host%d", n);
		strHost = AfxGetApp()->GetProfileStringA("MRU", s, "");
	}
	*/

	CString str;
	pAdapterInfo = (IP_ADAPTER_INFO*)malloc(sizeof(IP_ADAPTER_INFO));

	if (pAdapterInfo == NULL) {
		return -1;
	}

	if (GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == ERROR_BUFFER_OVERFLOW) {

		free(pAdapterInfo);
		pAdapterInfo = (IP_ADAPTER_INFO*)malloc(ulOutBufLen);
	}

	if (dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen) == NO_ERROR) {
		IP_ADAPTER_INFO* d = pAdapterInfo;

		for (d; d->Next;) {
			str = d->Description;
			m_Niccombo.AddString(str);
			d = d->Next;
		}
	}
	CRect rt;
	m_Network.GetWindowRect(&rt);
	m_Network.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT );
	m_Network.InsertColumn(1, TEXT("NO."), LVCFMT_CENTER, rt.Width() * 0.1);
	m_Network.InsertColumn(2, TEXT("IP"), LVCFMT_CENTER, rt.Width() * 0.4);
	m_Network.InsertColumn(3, TEXT("MAC"), LVCFMT_CENTER, rt.Width() * 0.4);
	m_Network.InsertColumn(4, TEXT("PING"), LVCFMT_CENTER, rt.Width() * 0.1);

	m_Nic.GetWindowRect(&rt);
	m_Nic.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_Nic.InsertColumn(0, TEXT("항목"), LVCFMT_CENTER, rt.Width() * 0.3);
	m_Nic.InsertColumn(1, TEXT("내용"), LVCFMT_CENTER, rt.Width() * 0.7);
	m_Nic.InsertItem(0, _T("Adapter Name"));
	m_Nic.InsertItem(1, _T("Adapter Desc"));
	m_Nic.InsertItem(2, _T("Type"));
	m_Nic.InsertItem(3, _T("IP Address"));
	m_Nic.InsertItem(4, _T("Mask"));
	m_Nic.InsertItem(5, _T("Gateway"));
	m_Nic.InsertItem(6, _T("DHCP Enable"));

	//for (int i = 0; i < m_Network.GetHeaderCtrl()->GetItemCount(); ++i)
	//	m_Network.SetColumnWidth(i, LVSCW_AUTOSIZE_USEHEADER);


	m_pThread = NULL;
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMFCApplication8Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMFCApplication8Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMFCApplication8Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFCApplication8Dlg::OnCbnSelchangeNiccombo()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CMFCApplication8Dlg::OnBnClickedSelect()
{
	IP_ADAPTER_INFO* tmpPtrAdInf = pAdapterInfo;


	int index;
	CString n;
	int num = 0;
	index = m_Niccombo.GetCurSel();
	if ((dwRetVal = GetAdaptersInfo(pAdapterInfo, &ulOutBufLen)) == NO_ERROR) {
		CString str;
		str = tmpPtrAdInf->AdapterName;
		switch (index) {
		case 0:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			
			break;
		case 1:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			break;
		case 2:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			break;
		case 3:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			break;


		case 4:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			break;
		case 5:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			break;
		case 6:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			break;
		case 7:
			for (int j = 0; j < index; j++) {
				tmpPtrAdInf = tmpPtrAdInf->Next;
			}
			m_Nic.SetItem(0, 1, LVIF_TEXT, tmpPtrAdInf->AdapterName, 0, 0, 0, NULL);
			m_Nic.SetItem(1, 1, LVIF_TEXT, tmpPtrAdInf->Description, 0, 0, 0, NULL);
			switch (tmpPtrAdInf->Type) {
			case MIB_IF_TYPE_OTHER:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Other", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_ETHERNET:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Ethernet", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_TOKENRING:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Token Ring", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_FDDI:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "FDDI", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_PPP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "PPP", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_LOOPBACK:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Loopback", 0, 0, 0, NULL);
				break;
			case MIB_IF_TYPE_SLIP:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Slip", 0, 0, 0, NULL);
				break;
			default:
				m_Nic.SetItem(2, 1, LVIF_TEXT, "Unknow type", 0, 0, 0, NULL);
				break;
			}
			m_Nic.SetItem(3, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpAddress.String, 0, 0, 0, NULL);
			m_Nic.SetItem(4, 1, LVIF_TEXT, tmpPtrAdInf->IpAddressList.IpMask.String, 0, 0, 0, NULL);
			m_Nic.SetItem(5, 1, LVIF_TEXT, tmpPtrAdInf->GatewayList.IpAddress.String, 0, 0, 0, NULL);
			if (tmpPtrAdInf->DhcpEnabled) {
				m_Nic.SetItem(6, 1, LVIF_TEXT, "Yes", 0, 0, 0, NULL);
			}
			else
				m_Nic.SetItem(6, 1, LVIF_TEXT, "No", 0, 0, 0, NULL);
			break;
		}
		
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication8Dlg::OnBnClickedAdd()
{
	BYTE ipFirst, ipSecond, ipThird, ipForth;
	CString strIpAddr;
	CString strResult;
	char ping[] = "ping ";
	CString address;
	int num = 0;
	if (m_Ip.GetAddress(ipFirst, ipSecond, ipThird, ipForth)) {
		strIpAddr.Format(_T("%d.%d.%d.%d"), ipFirst, ipSecond, ipThird, ipForth);
		address = strcat(ping, strIpAddr);
		strResult = ExecCmd(address);
		AfxMessageBox(strResult);
		num++;
	}

	else {
		CString strName;
		GetDlgItemText(IDC_Host, strName);
		address = strcat(ping, strName);
		strResult = ExecCmd(address);
		AfxMessageBox(strResult);
	}
	m_Host.Clear();
	m_Ip.ClearAddress();

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication8Dlg::OnBnClickedDel()
{
	int count = m_Network.GetItemCount();

	for (int i = count; i >= 0; i--) {
		if (m_Network.GetItemState(i, LVIS_SELECTED) != 0) {
			m_Network.DeleteItem(i);
			
		}
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}

int getMacString(BYTE* mac, char* mac_str)
{
	UCHAR* mac_byte = (UCHAR*)mac;

	sprintf_s(mac_str, 18, "%02x-%02x-%02x-%02x-%02x-%02x", mac_byte[0], mac_byte[1], mac_byte[2], mac_byte[3],
		mac_byte[4], mac_byte[5]);

	return 0;
}

int incIP(IPAddr* ip)
{
	IPAddr tmp_ip = ntohl(*ip);
	tmp_ip += 1;
	*ip = htonl(tmp_ip);

	return 0;
}

void CMFCApplication8Dlg::OnLvnItemchangedNetwork(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 0;
}


void CMFCApplication8Dlg::OnLvnItemchangedNic(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CMFCApplication8Dlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CMenu menu;

	menu.CreatePopupMenu();

	menu.AppendMenuA(MF_STRING, 2001, _T("ADD"));
	menu.AppendMenuA(MF_STRING, 2001, _T("DELETE"));

	menu.TrackPopupMenu(TPM_LEFTALIGN,
		point.x,
		point.y,
		AfxGetMainWnd());
}

int CMFCApplication8Dlg::incIP(IPAddr* ip)
{
	IPAddr tmp_ip = ntohl(*ip);
	tmp_ip += 1;
	*ip = htonl(tmp_ip);

	return 0;
}

int CMFCApplication8Dlg::getMacString(BYTE* mac, char* mac_str)
{
	UCHAR* mac_byte = (UCHAR*)mac;

	sprintf_s(mac_str, 18, "%02x-%02x-%02x-%02x-%02x-%02x", mac_byte[0], mac_byte[1], mac_byte[2], mac_byte[3], mac_byte[4], mac_byte[5]);

	return 0;
}


void CMFCApplication8Dlg::OnBnClickedScan()
{
	IP_ADAPTER_INFO* tmpPtrAdInf = pAdapterInfo;

	CString num;
	int n = 0;
	int index;

	index = m_Niccombo.GetCurSel();

	memset(ip_str, 0, 15);
	memset(maskip_str, 0, 15);
	switch (index) {
	case 0:
		
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	case 1:
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	case 2:
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	case 3:
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	case 4:
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	case 5:
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	case 6:
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	case 7:
		for (int i = 0; i < index; i++)
			tmpPtrAdInf = tmpPtrAdInf->Next;

		strcpy(ip_str, tmpPtrAdInf->IpAddressList.IpAddress.String);
		strcpy(maskip_str, tmpPtrAdInf->IpAddressList.IpMask.String);
		getMacString(tmpPtrAdInf->Address, mac_str);

		DestIp = inet_addr(ip_str) & inet_addr(maskip_str);

		hosts_count = ntohl(~MaskIp);
		if (NULL == m_pThread) {
			m_pThread = ::AfxBeginThread(ThreadFuction, this);

			if (NULL == m_pThread) {
				::AfxMessageBox(_T("AfxBeginThread Fail"));
			}
		}
		break;
	}
}
	
	




BOOL CMFCApplication8Dlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN) {

		if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_SPACE)
			return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}

UINT CMFCApplication8Dlg::ThreadFuction(LPVOID lpvoid)
{
	CMFCApplication8Dlg* pDlg = (CMFCApplication8Dlg*)lpvoid;
	CString num;
	int n = 0;
	for (ULONG j = 0; j <= pDlg->hosts_count - 2; j++) {
		pDlg->incIP(&pDlg->DestIp);
		pDlg->PhysAddrLen = 6;
		num.Format(_T("%d"), n);
		if (SendARP(pDlg->DestIp, 0, pDlg->MacAddr, &pDlg->PhysAddrLen) != NO_ERROR) {
			pDlg->DestIpStruct.S_un.S_addr = pDlg->DestIp;
			pDlg->m_Network.InsertItem(j, num);
			pDlg->m_Network.SetItemText(n, 1, inet_ntoa(pDlg->DestIpStruct));
			pDlg->getMacString((BYTE*)pDlg->MacAddr, pDlg->mac_str);
			pDlg->m_Network.SetItemText(n, 2, pDlg->mac_str);;
			pDlg->m_Network.SetItemText(n, 3, "X");
		}

		else if (SendARP(pDlg->DestIp, 0, pDlg->MacAddr, &pDlg->PhysAddrLen) == NO_ERROR) {
			pDlg->DestIpStruct.S_un.S_addr = pDlg->DestIp;			
			pDlg->m_Network.InsertItem(j, num);
			pDlg->m_Network.SetItemText(n, 1, inet_ntoa(pDlg->DestIpStruct));
			pDlg->getMacString((BYTE*)pDlg->MacAddr, pDlg->mac_str);
			pDlg->m_Network.SetItemText(n, 2, pDlg->mac_str);;
			pDlg->m_Network.SetItemText(n, 3, "O");			
		}
		n++;
	}
	
	return 0;
}

CString CMFCApplication8Dlg::ExecCmd(LPCSTR pCmdArg)
{
	CString strResult;

	HANDLE hChildStdoutRd; //read
	HANDLE hChildStdoutWr; //write

	BOOL fSuccess;

	//pipe를 설정하기 위해 생성
	SECURITY_ATTRIBUTES saAttr = { sizeof(SECURITY_ATTRIBUTES) };
	saAttr.bInheritHandle = TRUE; // 자식 프로세스가 부모의 커널 오브젝트 핸들을 상속받게 하기 위함
	//saAttr.lpSecurityDescriptor = NULL; // 개체에 호출 프로세스의 액세스 토큰과 연결된 기본 보안 설명자가 할당

	if ((!CreatePipe(&hChildStdoutRd, &hChildStdoutWr, &saAttr, 0)))
		return strResult;

	STARTUPINFO si{ sizeof(STARTUPINFO) };

	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
	si.hStdOutput = hChildStdoutWr;
	si.hStdError = hChildStdoutWr;

	si.wShowWindow = SW_HIDE;

	PROCESS_INFORMATION pi = { 0 };

	fSuccess = CreateProcess(NULL, (LPSTR)pCmdArg, NULL, NULL, TRUE, CREATE_NEW_CONSOLE, NULL, NULL, &si, &pi);
	
	if (!fSuccess)
		return strResult;

	WaitForSingleObject(pi.hProcess, 2000);
	TerminateProcess(pi.hProcess, 0);

	if (!CloseHandle(hChildStdoutWr))
		return strResult;

	for (;;)
	{
		DWORD dwRead;
		CHAR chBuf[4096];

		bool done = !ReadFile(hChildStdoutRd, chBuf, 4096, &dwRead, NULL) || dwRead == 0;

		if (done)
			break;
		strResult += CString(chBuf, dwRead);
	}

	CloseHandle(hChildStdoutRd);
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	return strResult;
}

void CMFCApplication8Dlg::OnIpnFieldchangedIp(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMIPADDRESS pIPAddr = reinterpret_cast<LPNMIPADDRESS>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}



void CMFCApplication8Dlg::OnEnChangeHost()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// 이 알림 메시지를 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



void CMFCApplication8Dlg::OnBnClickedStop()
{
	if (NULL != m_pThread) {
		::SuspendThread(m_pThread->m_hThread);
		AfxMessageBox(_T("Stop Thread"));
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CMFCApplication8Dlg::OnBnClickedResume()
{
	if (NULL != m_pThread) {
		::ResumeThread(m_pThread->m_hThread);
		AfxMessageBox(_T("Restart Thread"));
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
