// cssample1View.h : interface of the CCssample1View class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CSsample1VIEW_H__C6C32F57_5FBE_4F63_841C_74E2F1DDEDF4__INCLUDED_)
#define AFX_CSsample1VIEW_H__C6C32F57_5FBE_4F63_841C_74E2F1DDEDF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>

class CCssample1View : public CView
{

struct{ GLubyte red, green, blue; } currentPixel;


protected: // create from serialization only
	CCssample1View();
	DECLARE_DYNCREATE(CCssample1View)

// Attributes
public:
	CCssample1Doc* GetDocument();

// Operations
public:
	int height;
	int width;
	bool l_mouseDown;
	CPoint current;
	CPoint start;

	int menuOption;

	CPoint windowPoints[4];
	CPoint viewport[4];
	CPoint window[4];
	CPoint OriginalWin;
	CPoint MiddleViewport;
	int left_boundary;
	int right_boundary;
	int bottom_boundary;
	int top_boundary;

	CPoint scaledPoint;

	long y_ratio;
	long x_ratio;

	long largesty;
	long largestx;

	bool enable_fill;

	CPoint start_of_polygon;

	std::vector<CPoint> *polygonStartPoints;
	std::vector<CPoint> *polygonEndPoints;
	
	std::vector<CPoint> *clippedPolygonStartPoints;
	std::vector<CPoint> *clippedPolygonEndPoints;

	std::vector<CPoint> *followWindowStartPoints;
	std::vector<CPoint> *followWindowEndPoints;

	std::vector<CPoint> *viewportPolygonStartPoints;
	std::vector<CPoint> *viewportPolygonEndPoints;

//	void Init();

	HDC m_hDC;
	HGLRC m_hGLContext;
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);
	int m_GLPixelIndex;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCssample1View)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void ClearWindow();
	//case 0: in-out		bound 0: left boundary
	//case 1: out-in		bound 1: right boundary
	//case 2: in-in			bound 2: bottom boundary
	//case 3: out-out		bound 3: top boundary
	int BoundaryLineCase(CPoint p1, CPoint p2, int bound);
	CPoint GetIntersection(CPoint p1, CPoint p2, int boundary_line);

	void DrawDashedWindow();
	void DrawPolygon();
	void ClipPolygon();
	void FillPolygon(int current_x, int current_y);
	void ChangeViewPort();
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL dblBuf;
	CPoint m_LDownPos;
	virtual ~CCssample1View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCssample1View)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblBuf();
	afx_msg void OnNoDblBuf();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPolygonclippingDrawWindow();
	afx_msg void OnPolygonclippingDrawpolygon();
	afx_msg void OnOptionsClear();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnPolygonclippingClip();
	afx_msg void OnRegoinfillingFillpolygon();
	afx_msg void OnRefreshRefreshscreen();
	afx_msg void OnWindowtoviewportViewport();
	afx_msg void OnOptionsCleareverything();
};

#ifndef _DEBUG  // debug version in cssample1View.cpp
inline CCssample1Doc* CCssample1View::GetDocument()
   { return (CCssample1Doc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CSsample1VIEW_H__C6C32F57_5FBE_4F63_841C_74E2F1DDEDF4__INCLUDED_)
