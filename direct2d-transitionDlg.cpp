
// direct2d-transitionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "direct2d-transition.h"
#include "direct2d-transitionDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Cdirect2dtransitionDlg dialog



Cdirect2dtransitionDlg::Cdirect2dtransitionDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIRECT2DTRANSITION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cdirect2dtransitionDlg::DoDataExchange(CDataExchange* pDX)
{
   DDX_Control(pDX, IDC_STATIC_DRAW_CTRL, m_ctrlTransitionDraw);
   DDX_Control(pDX, IDC_SLIDER_PROGRESS, m_sliderProgress);

	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cdirect2dtransitionDlg, CDialogEx)
   ON_WM_SIZE()
	ON_WM_PAINT()
   ON_WM_HSCROLL()
	ON_WM_QUERYDRAGICON()
END_MESSAGE_MAP()


// Cdirect2dtransitionDlg message handlers

BOOL Cdirect2dtransitionDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

   m_sliderProgress.SetRange(0, 100);
   m_sliderProgress.SetPos(0);

   MoveWindow(CRect(0, 0, 840, 570));
   CenterWindow();

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Cdirect2dtransitionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Cdirect2dtransitionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Cdirect2dtransitionDlg::OnSize(UINT nType, int cx, int cy)
{
   if (m_ctrlTransitionDraw.GetSafeHwnd() == nullptr || m_sliderProgress.GetSafeHwnd() == nullptr)
   {
      return;
   }

   CRect rcClient;
   GetClientRect(rcClient);

   int nLeft = rcClient.left + (rcClient.Width() - 800) / 2;
   int nTop = rcClient.top + 20;
   m_ctrlTransitionDraw.MoveWindow(nLeft, nTop, 800, 450);

   nTop = nTop + 450 + 20;
   m_sliderProgress.MoveWindow(nLeft, nTop, 800, 40);
}

void Cdirect2dtransitionDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar * pScrollBar)
{
   if (m_sliderProgress.GetSafeHwnd() == nullptr)
   {
      return;
   }

   int nCurPos = m_sliderProgress.GetPos();

   m_ctrlTransitionDraw.SetProgress((float)nCurPos / 100.f);

   CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}

