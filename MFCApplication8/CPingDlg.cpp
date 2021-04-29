// CPingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCApplication8.h"
#include "CPingDlg.h"
#include "afxdialogex.h"


// CPingDlg 대화 상자

IMPLEMENT_DYNAMIC(CPingDlg, CDialog)

CPingDlg::CPingDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_PING, pParent)
{

}

CPingDlg::~CPingDlg()
{
}

void CPingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_List);
}


BEGIN_MESSAGE_MAP(CPingDlg, CDialog)
END_MESSAGE_MAP()


// CPingDlg 메시지 처리기
