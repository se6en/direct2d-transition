
// direct2d-transitionDlg.h : header file
//

#pragma once

#include "direct2d-transition-draw-ctrl.h"


// Cdirect2dtransitionDlg dialog
class Cdirect2dtransitionDlg : public CDialogEx
{
// Construction
public:
	Cdirect2dtransitionDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIRECT2DTRANSITION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
   afx_msg void OnSize(UINT nType, int cx, int cy);
   afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

private:
   CDirect2DTransitionDrawCtrl          m_ctrlTransitionDraw;
   CSliderCtrl                          m_sliderProgress;
};
