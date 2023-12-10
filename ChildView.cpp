
// ChildView.cpp: CChildView 클래스의 구현
//
#include "pch.h"
#include "framework.h"
#include "MenuAndControls.h"
#include "ChildView.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <string>
#include <sstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString GetSystemTimeAndDate() {
	return CTime::GetCurrentTime().Format("%Y-%m-%d %H:%M:%S");
}

// CChildView

CChildView::CChildView()
{
}

CChildView::~CChildView()
{
}


BEGIN_MESSAGE_MAP(CChildView, CWnd)
	ON_WM_PAINT()
	ON_WM_CONTEXTMENU()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_RBUTTONDBLCLK()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CChildView 메시지 처리기

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(nullptr, IDC_ARROW), reinterpret_cast<HBRUSH>(COLOR_WINDOW+1), nullptr);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

	// Rought double buffering 
	CRect rect;
	GetClientRect(&rect);

	CDC memDC;
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.SelectObject(&bitmap);
	memDC.Rectangle(0, 0, rect.Width(), rect.Height());

	{
		// 여기에 메시지 처리기 코드를 추가합니다.
		
		// 현재 시간 표시
		memDC.TextOutW(10, 10, m_current_time);

		// 마우스 위치 표시
		std::string pos =
			"(" + std::to_string(m_mouse_pos.x) + ", "
			+ std::to_string(m_mouse_pos.y) + ")";
		memDC.TextOut(10, 30, CString(pos.c_str()));

		// 마우스 이벤트 표시
		memDC.TextOut(10, 50, _T("Event: ") + m_mouse_event);
	}

	dc.BitBlt(0, 0, rect.Width(), rect.Height(), &memDC,
						0, 0, SRCCOPY);

	
	// 그리기 메시지에 대해서는 CWnd::OnPaint()를 호출하지 마십시오.
}


void CChildView::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMenu* pMenu = AfxGetMainWnd()->GetMenu()->GetSubMenu(2);
	if(pMenu != NULL)
	pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, point.x, point.y,
		AfxGetMainWnd());
}


int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	SetTimer(0, /* ms */ 1000, nullptr);

	m_current_time = GetSystemTimeAndDate();

	return 0;
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) {
  // 마우스가 윈도우를 벗어나도 위치 받도록 설정
	CWnd::SetCapture();

	// 마우스 위치 변수 업데이트
	m_mouse_event = "LButtonDown";

	// OnPaint() 호출
	CWnd::Invalidate();

	// 부모 함수 호출
	CWnd::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point) {
	// 윈도우 영역을 벗어난 마우스 위치를 받지 않도록 되돌림
	ReleaseCapture();

	// 마우스 위치 변수 업데이트
	m_mouse_event = "LButtonUp";

	// OnPaint() 호출
	CWnd::Invalidate();

	// 부모 함수 호출
	CWnd::OnLButtonUp(nFlags, point);
}


void CChildView::OnLButtonDblClk(UINT nFlags, CPoint point) {
	m_mouse_event = "LButtonDblClk";
	CWnd::Invalidate();
	CWnd::OnLButtonDblClk(nFlags, point);
}


void CChildView::OnRButtonDown(UINT nFlags, CPoint point) {
	CWnd::SetCapture();
	m_mouse_event = "RButtonDown";
	CWnd::Invalidate();
	CWnd::OnRButtonDown(nFlags, point);
}


void CChildView::OnRButtonUp(UINT nFlags, CPoint point) {
	m_mouse_event = "RButtonUp";
	ReleaseCapture();
	CWnd::Invalidate();
	CWnd::OnRButtonUp(nFlags, point);
}


void CChildView::OnRButtonDblClk(UINT nFlags, CPoint point) {
	m_mouse_event = "RButtonDblClk";
	CWnd::Invalidate();
	CWnd::OnRButtonDblClk(nFlags, point);
}


void CChildView::OnMButtonDown(UINT nFlags, CPoint point) {
	CWnd::SetCapture();
	m_mouse_event = "MButtonDown";
	CWnd::Invalidate();
	CWnd::OnMButtonDown(nFlags, point);
}


void CChildView::OnMButtonUp(UINT nFlags, CPoint point) {
	ReleaseCapture();
	m_mouse_event = "MButtonUp";
	CWnd::Invalidate();
	CWnd::OnMButtonUp(nFlags, point);
}

void CChildView::OnMButtonDblClk(UINT nFlags, CPoint point) {
	m_mouse_event = "MButtonDblClk";
	CWnd::Invalidate();
	CWnd::OnMButtonDblClk(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) {
	m_mouse_pos = point;
	CWnd::Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}


BOOL CChildView::OnEraseBkgnd(CDC* pDC) {
	return CWnd::OnEraseBkgnd(pDC);
	// return true;
}


void CChildView::OnTimer(UINT_PTR nIDEvent) {
	if (nIDEvent == 0) {
		m_current_time = GetSystemTimeAndDate();
		CWnd::Invalidate();
	}

	CWnd::OnTimer(nIDEvent);
}
