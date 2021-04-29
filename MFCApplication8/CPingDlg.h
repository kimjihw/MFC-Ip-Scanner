#pragma once


// CPingDlg 대화 상자

class CPingDlg : public CDialog
{
	DECLARE_DYNAMIC(CPingDlg)

public:
	CPingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CPingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PING };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CListBox m_List;
};
