// cssample1View.cpp : implementation of the CCssample1View class
//

#include "stdafx.h"
#include "cssample1.h"

#include "cssample1Doc.h"
#include "cssample1View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCssample1View

IMPLEMENT_DYNCREATE(CCssample1View, CView)

BEGIN_MESSAGE_MAP(CCssample1View, CView)
	//{{AFX_MSG_MAP(CCssample1View)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SIZE()
	ON_COMMAND(ID_DBL_BUF, OnDblBuf)
	ON_COMMAND(ID_NO_DBL_BUF, OnNoDblBuf)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
	ON_COMMAND(ID_POLYGONCLIPPING_DRAWWINDOW, &CCssample1View::OnPolygonclippingDrawWindow)
	ON_COMMAND(ID_POLYGONCLIPPING_DRAWPOLYGON, &CCssample1View::OnPolygonclippingDrawpolygon)
	ON_COMMAND(ID_OPTIONS_CLEAR, &CCssample1View::OnOptionsClear)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONDBLCLK()
	ON_COMMAND(ID_POLYGONCLIPPING_CLIP, &CCssample1View::OnPolygonclippingClip)
	ON_COMMAND(ID_REGOINFILLING_FILLPOLYGON, &CCssample1View::OnRegoinfillingFillpolygon)
	ON_COMMAND(ID_REFRESH_REFRESHSCREEN, &CCssample1View::OnRefreshRefreshscreen)
	ON_COMMAND(ID_WINDOWTOVIEWPORT_VIEWPORT, &CCssample1View::OnWindowtoviewportViewport)
	ON_COMMAND(ID_OPTIONS_CLEAREVERYTHING, &CCssample1View::OnOptionsCleareverything)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCssample1View construction/destruction

/*
VOID WINAPI Sleep(
  _In_  DWORD dwMilliseconds
);
*/

#include <Windows.h>
#include <iostream>
#include <sstream>

 #define DBOUT( s )            \
{                             \
   std::wostringstream os_;    \
   os_ << s;                   \
   OutputDebugStringW( os_.str().c_str() );  \
}

CCssample1View::CCssample1View()
{
	l_mouseDown=false;
	width=0; height=0;
	dblBuf=true;

	menuOption = -1;
	enable_fill = false;

	polygonStartPoints = new std::vector<CPoint>;
	polygonEndPoints = new std::vector<CPoint>;

	followWindowStartPoints = new std::vector<CPoint>;
	followWindowEndPoints = new std::vector<CPoint>;

	clippedPolygonStartPoints = new std::vector<CPoint>;
	clippedPolygonEndPoints = new std::vector<CPoint>;

	viewportPolygonStartPoints = new std::vector<CPoint>;
	viewportPolygonEndPoints = new std::vector<CPoint>;
}

CCssample1View::~CCssample1View()
{
}

BOOL CCssample1View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CCssample1View drawing

void CCssample1View::OnDraw(CDC* pDC)
{
	CCssample1Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CRect rcClient;
	GetClientRect(rcClient);
	
	width=rcClient.Width();
	height=rcClient.Height();
    
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//Clear buffer content, so that no old content remains in the buffer
	//glClear(GL_COLOR_BUFFER_BIT);
	
	
	// for double buffering
	//if(dblBuf)
		//glDrawBuffer(GL_BACK);
	//else
		glDrawBuffer(GL_FRONT_AND_BACK);

	wglMakeCurrent(m_hDC,m_hGLContext);


    //Add your own stuff here for OpenGL related operations
	if(menuOption == -1){

		glDisable(GL_LINE_STIPPLE);
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glPushMatrix();
		glBegin(GL_LINES);
			glVertex3d(start.x, height-start.y, 0);
			glVertex3d(m_LDownPos.x, height-m_LDownPos.y, 0);
		glEnd();
		glFlush();
		glPopMatrix();

       
		wglMakeCurrent(m_hDC,NULL);
		wglMakeCurrent(m_hDC, m_hGLContext);

		::SwapBuffers(m_hDC);
	}

	//Create Clipping Window
	if(menuOption == 0){
		ClearWindow();
		DrawDashedWindow();
	}

	//Create Polygon
	if(menuOption == 1){
		DrawPolygon();
	}

}

void CCssample1View::ChangeViewPort(){
	GLubyte red = 0;
	GLubyte blue = 0;
	GLubyte green = 0;
	//glReadPixels(m_LDownPos.x, m_LDownPos.y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &currentPixel);
	
	//DBOUT("before: b:" << currentPixel.blue << "| g:" << currentPixel.blue << "| r:" << currentPixel.red << "\n");
	/*if(m_LDownPos.y >= (viewport[0].y-10) && m_LDownPos.y <= (viewport[0].y+10)
		&& m_LDownPos.x <= (viewport[0].x+10) && m_LDownPos.x >= (viewport[0].x-10)){
		
		viewport[0].x = m_LDownPos.x;
		viewport[0].y = m_LDownPos.y;

		viewport[1].x = viewport[1].x;
		viewport[1].y = viewport[1].y;

		viewport[2].x = viewport[1].x;
		viewport[2].y = m_LDownPos.y;

		viewport[3].x = m_LDownPos.x;
		viewport[3].y = viewport[1].y;

	
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glEnable( GL_LINE_STIPPLE );
		glLineStipple(1, 0x3F07);

		glPushMatrix();
		glBegin(GL_LINES);

			//glVertex3d(0, 0, 0);
			//glVertex3d(m_LDownPos.x, height-m_LDownPos.y, 0);
			
			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[2].x, height-window[2].y, 0);

			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[3].x, height-window[3].y, 0);

			glVertex3d(window[2].x, height-window[2].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);

			glVertex3d(window[3].x, height-window[3].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);
	
		glEnd();
			/*#######################################################################//
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glDisable( GL_LINE_STIPPLE );

		glPushMatrix();
		glBegin(GL_LINES);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[2].x, height-viewport[2].y, 0);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[3].x, height-viewport[3].y, 0);

			glVertex3d(viewport[2].x, height-viewport[2].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);

			glVertex3d(viewport[3].x, height-viewport[3].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);
			
		glEnd();
		//glFlush();
		//glPopMatrix();
		

		glColor4f(1.0, 1.0, 1.0, 0.0);
		glPointSize(9.0);
		glPushMatrix();
		glBegin(GL_POINTS);
			
			glVertex2i(window[0].x, height-window[0].y);
			glVertex2i(MiddleViewport.x, height-MiddleViewport.y);

		glEnd();
		//glFlush();
		//glPopMatrix();

		ClipPolygon();

		std::vector<CPoint>::iterator start_it=polygonStartPoints->begin();
		std::vector<CPoint>::iterator end_it=polygonEndPoints->begin();
		glColor4f(0.5, 1.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		
		glPushMatrix();
		glBegin(GL_LINES);
			while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){

				glVertex3d(start_it->x, (height-start_it->y), 0);
				glVertex3d(end_it->x, (height-end_it->y), 0);
			
				start_it++;
				end_it++;
			}

			start_it=clippedPolygonStartPoints->begin();
			end_it=clippedPolygonEndPoints->begin();

			glColor4f(1.0, 0.0, 0.0, 0.0);

			while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

			//if(largesty!=0){
				//y_ratio = (double)(height-largesty)/(height-viewport[0].y);
			//}else{
			//	y_ratio=1;
			//}

			/*if((width-largestx)!=0){
				x_ratio = (width-largestx)/viewport[0].x;
			}else{
				x_ratio=1;
			}

			y_ratio = ((double)(height-m_LDownPos.y))/(height-(viewport[0].y-(height - window[1].y)));
			x_ratio = ((double)(width-m_LDownPos.x))/(width-(viewport[0].x-(width - window[1].x)));
			//y_ratio = 1;
			//x_ratio=1;

			//viewportPolygonStartPoints->clear();
			//viewportPolygonEndPoints->clear();

			/*for(std::vector<CPoint>::iterator s_it = clippedPolygonStartPoints->begin(); s_it!= clippedPolygonStartPoints->end();s_it++){
				CPoint translatedPoint;

				translatedPoint.x = s_it->x + (width - window[1].x);
				translatedPoint.y = s_it->y + (height - window[1].y);

				scaledPoint.x = translatedPoint.x;
				scaledPoint.y = translatedPoint.y/y_ratio;

				viewportPolygonStartPoints->push_back(scaledPoint);
			}

			for(std::vector<CPoint>::iterator e_it = clippedPolygonEndPoints->begin(); e_it!= clippedPolygonEndPoints->end();e_it++){
				CPoint translatedPoint;

				translatedPoint.x = e_it->x + (width - window[1].x);
				translatedPoint.y = e_it->y + (height - window[1].y);

				scaledPoint.x = translatedPoint.x;
				scaledPoint.y = translatedPoint.y/y_ratio;

				viewportPolygonEndPoints->push_back(scaledPoint);
			}*/
			/*
			std::vector<CPoint> *newVec = new std::vector<CPoint>;
			std::vector<CPoint> *newVec2 = new std::vector<CPoint>;

			for(std::vector<CPoint>::iterator s_it = viewportPolygonStartPoints->begin(); s_it!= viewportPolygonStartPoints->end();s_it++){
				CPoint temp;

				scaledPoint.x = s_it->x/x_ratio;
				scaledPoint.y = s_it->y/y_ratio - (((m_LDownPos.y) + (height - viewport[0].y -(height - window[1].y)))*y_ratio);

				temp.x = scaledPoint.x;
				temp.y = scaledPoint.y;

				newVec->push_back(scaledPoint);
			}

			for(std::vector<CPoint>::iterator e_it = viewportPolygonEndPoints->begin(); e_it!= viewportPolygonEndPoints->end();e_it++){
				CPoint temp;
				
				scaledPoint.x = e_it->x/x_ratio;
				scaledPoint.y = e_it->y/y_ratio - (((m_LDownPos.y) + (height - viewport[0].y-(height - window[1].y)))*y_ratio);

				temp.x = scaledPoint.x;
				temp.y = scaledPoint.y;

				newVec2->push_back(scaledPoint);
			}

			viewportPolygonStartPoints->clear();
			viewportPolygonEndPoints->clear();

			for(std::vector<CPoint>::iterator s_it = newVec->begin(); s_it!= newVec->end();s_it++){
				CPoint translatedPoint;

				translatedPoint.x = s_it->x;
				translatedPoint.y = s_it->y;

				viewportPolygonStartPoints->push_back(translatedPoint);
			}

			for(std::vector<CPoint>::iterator e_it = newVec2->begin(); e_it!= newVec2->end();e_it++){
				CPoint translatedPoint;

				translatedPoint.x = e_it->x;
				translatedPoint.y = e_it->y;

				viewportPolygonEndPoints->push_back(translatedPoint);
			}


			start_it=viewportPolygonStartPoints->begin();
			end_it=viewportPolygonEndPoints->begin();
			//start_it=newVec->begin();
			//end_it=newVec2->begin();
			glColor4f(1.0, 0.0, 0.0, 0.0);

				while(start_it != viewportPolygonStartPoints->end() && end_it != viewportPolygonEndPoints->end()){
				//while(start_it != newVec->end() && end_it != newVec2->end()){

					glVertex3d(start_it->x, height-start_it->y, 0);
					glVertex3d(end_it->x, height-end_it->y, 0);
			
					start_it++;
					end_it++;
				}

		glEnd();

			//#######################################################################//
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glDisable( GL_LINE_STIPPLE );

		glPushMatrix();
		glBegin(GL_LINES);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[2].x, height-viewport[2].y, 0);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[3].x, height-viewport[3].y, 0);

			glVertex3d(viewport[2].x, height-viewport[2].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);

			glVertex3d(viewport[3].x, height-viewport[3].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);
			
		glEnd();
		//glFlush();
		//glPopMatrix();



		glFlush();
		glPopMatrix();

		wglMakeCurrent(m_hDC,NULL);
		wglMakeCurrent(m_hDC, m_hGLContext);

		::SwapBuffers(m_hDC);


	}else*/ if(m_LDownPos.y >= (window[0].y-10) && m_LDownPos.y <= (window[0].y+10)
		&& m_LDownPos.x <= (window[0].x+10) && m_LDownPos.x >= (window[0].x-10)){
		
		window[0].x = m_LDownPos.x;
		window[0].y = m_LDownPos.y;

		window[1].x = window[1].x;
		window[1].y = window[1].y;

		window[2].x = window[1].x;
		window[2].y = m_LDownPos.y;

		window[3].x = m_LDownPos.x;
		window[3].y = window[1].y;


		MiddleViewport.x = ( window[0].x + window[1].x ) / 2;
		MiddleViewport.y = ( window[0].y + window[1].y ) / 2;


		if(window[0].x < window[1].x){
			left_boundary = window[0].x;
			right_boundary = window[1].x;
		}else{
			left_boundary = window[1].x;
			right_boundary = window[0].x;
		}

		if(window[0].y < window[1].y){
			top_boundary = height-window[0].y;
			bottom_boundary = height-window[1].y;
		}else{
			top_boundary = height-window[1].y;
			bottom_boundary = height-window[0].y;
		}


		glColor4f(1.0, 1.0, 1.0, 0.0);
		glEnable( GL_LINE_STIPPLE );
		glLineStipple(1, 0x3F07);

		glPushMatrix();
		glBegin(GL_LINES);

			//glVertex3d(0, 0, 0);
			//glVertex3d(m_LDownPos.x, height-m_LDownPos.y, 0);
			
			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[2].x, height-window[2].y, 0);

			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[3].x, height-window[3].y, 0);

			glVertex3d(window[2].x, height-window[2].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);

			glVertex3d(window[3].x, height-window[3].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);
	
		glEnd();
			/*/#######################################################################//
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glDisable( GL_LINE_STIPPLE );

		glPushMatrix();
		glBegin(GL_LINES);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[2].x, height-viewport[2].y, 0);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[3].x, height-viewport[3].y, 0);

			glVertex3d(viewport[2].x, height-viewport[2].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);

			glVertex3d(viewport[3].x, height-viewport[3].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);
			
		glEnd();
		//glFlush();
		//glPopMatrix();
		*/

		glColor4f(1.0, 1.0, 1.0, 0.0);
		glPointSize(9.0);
		glPushMatrix();
		glBegin(GL_POINTS);
			
			glVertex2i(window[0].x, height-window[0].y);
			glVertex2i(MiddleViewport.x, height-MiddleViewport.y);

		glEnd();
		//glFlush();
		//glPopMatrix();

		ClipPolygon();

		std::vector<CPoint>::iterator start_it=polygonStartPoints->begin();
		std::vector<CPoint>::iterator end_it=polygonEndPoints->begin();
		glColor4f(0.5, 1.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		
		glPushMatrix();
		glBegin(GL_LINES);
			while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){

				glVertex3d(start_it->x, (height-start_it->y), 0);
				glVertex3d(end_it->x, (height-end_it->y), 0);
			
				start_it++;
				end_it++;
			}

			start_it=clippedPolygonStartPoints->begin();
			end_it=clippedPolygonEndPoints->begin();

			glColor4f(1.0, 0.0, 0.0, 0.0);

			while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

			//if(largesty!=0){
				//y_ratio = (double)(height-largesty)/(height-viewport[0].y);
			//}else{
			//	y_ratio=1;
			//}

			/*if((width-largestx)!=0){
				x_ratio = (width-largestx)/viewport[0].x;
			}else{
				x_ratio=1;
			}*/

			y_ratio = ((double)(height-m_LDownPos.y))/(height-(viewport[0].y-(height - window[1].y)));
			//x_ratio = ((double)(m_LDownPos.x))/(viewport[0].x+(width - window[1].x));
			//y_ratio = 1;
			x_ratio=1;

			//viewportPolygonStartPoints->clear();
			//viewportPolygonEndPoints->clear();

			/*for(std::vector<CPoint>::iterator s_it = clippedPolygonStartPoints->begin(); s_it!= clippedPolygonStartPoints->end();s_it++){
				CPoint translatedPoint;

				translatedPoint.x = s_it->x + (width - window[1].x);
				translatedPoint.y = s_it->y + (height - window[1].y);

				scaledPoint.x = translatedPoint.x;
				scaledPoint.y = translatedPoint.y/y_ratio;

				viewportPolygonStartPoints->push_back(scaledPoint);
			}

			for(std::vector<CPoint>::iterator e_it = clippedPolygonEndPoints->begin(); e_it!= clippedPolygonEndPoints->end();e_it++){
				CPoint translatedPoint;

				translatedPoint.x = e_it->x + (width - window[1].x);
				translatedPoint.y = e_it->y + (height - window[1].y);

				scaledPoint.x = translatedPoint.x;
				scaledPoint.y = translatedPoint.y/y_ratio;

				viewportPolygonEndPoints->push_back(scaledPoint);
			}*/

			std::vector<CPoint> *newVec = new std::vector<CPoint>;
			std::vector<CPoint> *newVec2 = new std::vector<CPoint>;

			for(std::vector<CPoint>::iterator s_it = viewportPolygonStartPoints->begin(); s_it!= viewportPolygonStartPoints->end();s_it++){
				CPoint temp;

				scaledPoint.x = s_it->x/x_ratio;
				scaledPoint.y = s_it->y/y_ratio - (((m_LDownPos.y) + (height - viewport[0].y -(height - window[1].y)))*y_ratio);

				temp.x = scaledPoint.x;
				temp.y = scaledPoint.y;

				newVec->push_back(scaledPoint);
			}

			for(std::vector<CPoint>::iterator e_it = viewportPolygonEndPoints->begin(); e_it!= viewportPolygonEndPoints->end();e_it++){
				CPoint temp;
				
				scaledPoint.x = e_it->x/x_ratio;
				scaledPoint.y = e_it->y/y_ratio - (((m_LDownPos.y) + (height - viewport[0].y-(height - window[1].y)))*y_ratio);

				temp.x = scaledPoint.x;
				temp.y = scaledPoint.y;

				newVec2->push_back(scaledPoint);
			}

			viewportPolygonStartPoints->clear();
			viewportPolygonEndPoints->clear();

			for(std::vector<CPoint>::iterator s_it = newVec->begin(); s_it!= newVec->end();s_it++){
				CPoint translatedPoint;

				translatedPoint.x = s_it->x;
				translatedPoint.y = s_it->y;

				viewportPolygonStartPoints->push_back(translatedPoint);
			}

			for(std::vector<CPoint>::iterator e_it = newVec2->begin(); e_it!= newVec2->end();e_it++){
				CPoint translatedPoint;

				translatedPoint.x = e_it->x;
				translatedPoint.y = e_it->y;

				viewportPolygonEndPoints->push_back(translatedPoint);
			}


			start_it=viewportPolygonStartPoints->begin();
			end_it=viewportPolygonEndPoints->begin();
			//start_it=newVec->begin();
			//end_it=newVec2->begin();
			glColor4f(1.0, 0.0, 0.0, 0.0);

				while(start_it != viewportPolygonStartPoints->end() && end_it != viewportPolygonEndPoints->end()){
				//while(start_it != newVec->end() && end_it != newVec2->end()){

					glVertex3d(start_it->x, height-start_it->y, 0);
					glVertex3d(end_it->x, height-end_it->y, 0);
			
					start_it++;
					end_it++;
				}

		glEnd();

			//#######################################################################//
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glDisable( GL_LINE_STIPPLE );

		glPushMatrix();
		glBegin(GL_LINES);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[2].x, height-viewport[2].y, 0);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[3].x, height-viewport[3].y, 0);

			glVertex3d(viewport[2].x, height-viewport[2].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);

			glVertex3d(viewport[3].x, height-viewport[3].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);
			
		glEnd();
		//glFlush();
		//glPopMatrix();



		glFlush();
		glPopMatrix();

		wglMakeCurrent(m_hDC,NULL);
		wglMakeCurrent(m_hDC, m_hGLContext);

		::SwapBuffers(m_hDC);


	}else if(m_LDownPos.y >= (MiddleViewport.y-10) && m_LDownPos.y <= (MiddleViewport.y+10) 
			&& m_LDownPos.x <= (MiddleViewport.x+10) && m_LDownPos.x >= (MiddleViewport.x-10)){

		window[0].x = m_LDownPos.x + (window[0].x-MiddleViewport.x);
		window[0].y = m_LDownPos.y + (window[0].y-MiddleViewport.y);

		window[1].x = m_LDownPos.x + (window[1].x-MiddleViewport.x);
		window[1].y = m_LDownPos.y + (window[1].y-MiddleViewport.y);

		window[2].x = m_LDownPos.x + (window[2].x-MiddleViewport.x);
		window[2].y = m_LDownPos.y + (window[2].y-MiddleViewport.y);

		window[3].x = m_LDownPos.x + (window[3].x-MiddleViewport.x);
		window[3].y = m_LDownPos.y + (window[3].y-MiddleViewport.y);

		MiddleViewport.x = m_LDownPos.x;
		MiddleViewport.y = m_LDownPos.y;

		if(window[0].x < window[1].x){
			left_boundary = window[0].x;
			right_boundary = window[1].x;
		}else{
			left_boundary = window[1].x;
			right_boundary = window[0].x;
		}

		if(window[0].y < window[1].y){
			top_boundary = height-window[0].y;
			bottom_boundary = height-window[1].y;
		}else{
			top_boundary = height-window[1].y;
			bottom_boundary = height-window[0].y;
		}


		glColor4f(1.0, 1.0, 1.0, 0.0);
		glEnable( GL_LINE_STIPPLE );
		glLineStipple(1, 0x3F07);

		glPushMatrix();
		glBegin(GL_LINES);

			//glVertex3d(0, 0, 0);
			//glVertex3d(m_LDownPos.x, height-m_LDownPos.y, 0);
			
			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[2].x, height-window[2].y, 0);

			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[3].x, height-window[3].y, 0);

			glVertex3d(window[2].x, height-window[2].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);

			glVertex3d(window[3].x, height-window[3].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);
	
		glEnd();
			//#######################################################################//
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glDisable( GL_LINE_STIPPLE );

		glPushMatrix();
		glBegin(GL_LINES);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[2].x, height-viewport[2].y, 0);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[3].x, height-viewport[3].y, 0);

			glVertex3d(viewport[2].x, height-viewport[2].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);

			glVertex3d(viewport[3].x, height-viewport[3].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);
			
		glEnd();
		//glFlush();
		//glPopMatrix();

		glColor4f(1.0, 1.0, 1.0, 0.0);
		glPointSize(9.0);
		glPushMatrix();
		glBegin(GL_POINTS);
			
			glVertex2i(window[0].x, height-window[0].y);
			glVertex2i(MiddleViewport.x, height-MiddleViewport.y);

		glEnd();
		//glFlush();
		//glPopMatrix();

		ClipPolygon();

		std::vector<CPoint>::iterator start_it=polygonStartPoints->begin();
		std::vector<CPoint>::iterator end_it=polygonEndPoints->begin();
		glColor4f(0.5, 1.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		
		glPushMatrix();
		glBegin(GL_LINES);
			while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

			start_it=clippedPolygonStartPoints->begin();
			end_it=clippedPolygonEndPoints->begin();

			glColor4f(1.0, 0.0, 0.0, 0.0);

			while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

			start_it=viewportPolygonStartPoints->begin();
			end_it=viewportPolygonEndPoints->begin();
			glColor4f(1.0, 0.0, 0.0, 0.0);

				while(start_it != viewportPolygonStartPoints->end() && end_it != viewportPolygonEndPoints->end()){

					glVertex3d(start_it->x, height-start_it->y, 0);
					glVertex3d(end_it->x, height-end_it->y, 0);
			
					start_it++;
					end_it++;
				}

		glEnd();
		glFlush();
		glPopMatrix();

		wglMakeCurrent(m_hDC,NULL);
		wglMakeCurrent(m_hDC, m_hGLContext);

		::SwapBuffers(m_hDC);
		
	}else{
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glEnable( GL_LINE_STIPPLE );
		glLineStipple(1, 0x3F07);

		glPushMatrix();
		glBegin(GL_LINES);

			//glVertex3d(0, 0, 0);
			//glVertex3d(m_LDownPos.x, height-m_LDownPos.y, 0);
			
			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[2].x, height-window[2].y, 0);

			glVertex3d(window[0].x, height-window[0].y, 0);
			glVertex3d(window[3].x, height-window[3].y, 0);

			glVertex3d(window[2].x, height-window[2].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);

			glVertex3d(window[3].x, height-window[3].y, 0);
			glVertex3d(window[1].x, height-window[1].y, 0);
			
		glEnd();
		//glFlush();
		//glPopMatrix();

		glColor4f(1.0, 1.0, 1.0, 0.0);
		glDisable( GL_LINE_STIPPLE );

		glPushMatrix();
		glBegin(GL_LINES);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[2].x, height-viewport[2].y, 0);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[3].x, height-viewport[3].y, 0);

			glVertex3d(viewport[2].x, height-viewport[2].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);

			glVertex3d(viewport[3].x, height-viewport[3].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);
			
		glEnd();

		glColor4f(1.0, 1.0, 1.0, 0.0);
		glPointSize(9.0);
		glPushMatrix();
		glBegin(GL_POINTS);
			
			glVertex2i(window[0].x, height-window[0].y);
			glVertex2i(MiddleViewport.x, height-MiddleViewport.y);

		glEnd();
		//glFlush();
		//glPopMatrix();


		std::vector<CPoint>::iterator start_it=polygonStartPoints->begin();
		std::vector<CPoint>::iterator end_it=polygonEndPoints->begin();
		glColor4f(0.5, 1.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		
		glPushMatrix();
		glBegin(GL_LINES);
			while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}


			start_it=clippedPolygonStartPoints->begin();
			end_it=clippedPolygonEndPoints->begin();

			glColor4f(1.0, 0.0, 0.0, 0.0);

			while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

			start_it=viewportPolygonStartPoints->begin();
			end_it=viewportPolygonEndPoints->begin();
			glColor4f(1.0, 0.0, 0.0, 0.0);

				while(start_it != viewportPolygonStartPoints->end() && end_it != viewportPolygonEndPoints->end()){

					glVertex3d(start_it->x, height-start_it->y, 0);
					glVertex3d(end_it->x, height-end_it->y, 0);
			
					start_it++;
					end_it++;
				}

		glEnd();
		glFlush();
		glPopMatrix();

		wglMakeCurrent(m_hDC,NULL);
		wglMakeCurrent(m_hDC, m_hGLContext);

		::SwapBuffers(m_hDC);

	}

}

void CCssample1View::FillPolygon(int current_x, int current_y){
	
	//ClearWindow();

	/*std::vector<CPoint>::iterator start_it=polygonStartPoints->begin();
	std::vector<CPoint>::iterator end_it=polygonEndPoints->begin();
	glColor4f(1.0, 0.0, 0.0, 0.0);
	glDisable( GL_LINE_STIPPLE );
		
	glPushMatrix();
	glBegin(GL_LINES);
		while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){

			glVertex3d(start_it->x, height-start_it->y, 0);
			glVertex3d(end_it->x, height-end_it->y, 0);
			
			start_it++;
			end_it++;
		}

	glEnd();
	*/
	
	GLubyte red = 255;

	/*for(int count = 0; count<5; count++){

	for(int y=current_y-count;y>bottom_boundary+1;y=y-5){
		enable_fill = false;
		for(int x=current_x-1; x<=right_boundary+1;x++){
			
			glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel);
			glReadPixels(x+1, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel2);
			//DBOUT("before: b:" << pixel.blue << "| g:" << pixel.blue << "| r:" << pixel.red << " || x=" << x << " y=" << y << " Fill: " << enable_fill << "\n");

			if(enable_fill == false && (pixel.red == red && pixel2.red != red)){
				enable_fill = true;

				//DBOUT("Pixel: b:" << pixel.blue << "| g:" << pixel.blue << "| r:" << pixel.red << " || x=" << x << " y=" << y << " Fill: " << enable_fill << "\n");
			}else if((pixel.red == red && pixel3.red != red)){

				enable_fill = false;
			}

			glReadPixels(x-1, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &pixel3);

			if(enable_fill){
				glColor4f(1.0, 0.0, 0.0, 0.0);
				glPushMatrix();
				glBegin(GL_POINTS);
				glBegin(GL_POINTS);
					glVertex2i(x, y);
				glEnd();
				glFlush();
				glPopMatrix();
	      
				wglMakeCurrent(m_hDC,NULL);
				wglMakeCurrent(m_hDC, m_hGLContext);

				::SwapBuffers(m_hDC);
				//DBOUT("Not Supposed to call: b:" << pixel.blue << "| g:" << pixel.blue << "| r:" << pixel.red << " || x=" << x << " y=" << y << "\n");
			}
		}
	}

	}*/

	if(enable_fill == false){
		for(int y=current_y;y>bottom_boundary;y--){
			for(int x=current_x; x<right_boundary;x++){
			
				glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &currentPixel);

				if(currentPixel.red == red){
					enable_fill = true;

					FillPolygon(x, y-1);

					y = bottom_boundary;
					enable_fill = false;
					break;
				}	
			}
		}
	}

	if(enable_fill){
		glReadPixels(current_x, current_y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &currentPixel);

		 if(currentPixel.red != red){
			 DBOUT("Pixel: x=" << current_x << " y=" << current_y << " Fill: " << enable_fill << "\n");

			glBegin(GL_POINTS);
				glVertex2i(current_x, current_y);
			glEnd();

			FillPolygon(current_x-1, current_y);
			FillPolygon(current_x+1, current_y);
			FillPolygon(current_x, current_y-1);
			FillPolygon(current_x, current_y+1);
		}
	}

	glFlush();
	glPopMatrix();
	      
	wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC, m_hGLContext);

	::SwapBuffers(m_hDC);
	
}

void CCssample1View::ClearWindow(){

	//Clear buffer content, so that no old content remains in the buffer
	glClear(GL_COLOR_BUFFER_BIT);
	
	// for double buffering
	if(dblBuf)
		glDrawBuffer(GL_BACK);
	else
		glDrawBuffer(GL_FRONT_AND_BACK);
}

void CCssample1View::DrawDashedWindow(){

	windowPoints[2].x = windowPoints[0].x;
	windowPoints[2].y = windowPoints[1].y;

	windowPoints[3].x = windowPoints[1].x;
	windowPoints[3].y = windowPoints[0].y;

	if(windowPoints[0].x < windowPoints[1].x){
		left_boundary = windowPoints[0].x;
		right_boundary = windowPoints[1].x;
	}else{
		left_boundary = windowPoints[1].x;
		right_boundary = windowPoints[0].x;
	}

	if(windowPoints[0].y < windowPoints[1].y){
		top_boundary = height-windowPoints[0].y;
		bottom_boundary = height-windowPoints[1].y;
	}else{
		top_boundary = height-windowPoints[1].y;
		bottom_boundary = height-windowPoints[0].y;
	}

	glColor4f(1.0, 1.0, 0.0, 0.0);
	glEnable( GL_LINE_STIPPLE );
	glLineStipple(1, 0x3F07);

	glPushMatrix();
	glBegin(GL_LINES);
		glVertex3d(windowPoints[0].x, height-windowPoints[0].y, 0);
		glVertex3d(windowPoints[2].x, height-windowPoints[2].y, 0);

		glVertex3d(windowPoints[0].x, height-windowPoints[0].y, 0);
		glVertex3d(windowPoints[3].x, height-windowPoints[3].y, 0);

		glVertex3d(windowPoints[2].x, height-windowPoints[2].y, 0);
		glVertex3d(windowPoints[1].x, height-windowPoints[1].y, 0);

		glVertex3d(windowPoints[3].x, height-windowPoints[3].y, 0);
		glVertex3d(windowPoints[1].x, height-windowPoints[1].y, 0);

	glEnd();
	glFlush();
	glPopMatrix();
      
	wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC, m_hGLContext);

	::SwapBuffers(m_hDC);

}

void CCssample1View::DrawPolygon(){

	std::vector<CPoint>::iterator start_it=polygonStartPoints->begin();
	std::vector<CPoint>::iterator end_it=polygonEndPoints->begin();

	glDisable( GL_LINE_STIPPLE );
	glColor4f(0.5, 1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){
			glVertex3d(start_it->x, height-start_it->y, 0);
			glVertex3d(end_it->x, height-end_it->y, 0);

			start_it++;
			end_it++;
		}
		glVertex3d(polygonStartPoints->back().x, height-polygonStartPoints->back().y, 0);
		glVertex3d(m_LDownPos.x, height-m_LDownPos.y, 0);

	glEnd();
	glFlush();
	glPopMatrix();

    wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC, m_hGLContext);

	::SwapBuffers(m_hDC);

}

void CCssample1View::ClipPolygon(){
	
	/*Draw Boundries
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glEnable( GL_LINE_STIPPLE );
	glLineStipple(2, 0xF);

	glPushMatrix();
	glBegin(GL_LINES);
		glVertex3d(left_boundary, 0, 0);
		glVertex3d(left_boundary, height, 0);

		glVertex3d(right_boundary, 0, 0);
		glVertex3d(right_boundary, height, 0);

		glVertex3d(0, bottom_boundary, 0);
		glVertex3d(width, bottom_boundary, 0);
		
		glVertex3d(0, top_boundary, 0);
		glVertex3d(width, top_boundary, 0);
	glEnd();
	glFlush();
	glPopMatrix();
	
	::SwapBuffers(m_hDC);
	*/

	//clip left boundary
	int boundary_case = -1;
	int boundLine = 0;
	CPoint intersect_point;
	CPoint last_window_point;

	std::vector<CPoint> *tempStartPoints = new std::vector<CPoint>;
	std::vector<CPoint> *tempEndPoints = new std::vector<CPoint>;

		clippedPolygonStartPoints->clear();
		clippedPolygonEndPoints->clear();

		followWindowStartPoints->clear();
		followWindowEndPoints->clear();

		for(std::vector<CPoint>::iterator s_it = polygonStartPoints->begin(); s_it!= polygonStartPoints->end();s_it++){
			clippedPolygonStartPoints->push_back(*s_it);
		}

		for(std::vector<CPoint>::iterator e_it = polygonEndPoints->begin(); e_it!= polygonEndPoints->end();e_it++){
			clippedPolygonEndPoints->push_back(*e_it);
		}

	std::vector<CPoint>::iterator start_it;
	std::vector<CPoint>::iterator end_it;
	

	for(boundLine; boundLine < 4; boundLine++){

		start_it=clippedPolygonStartPoints->begin();
		end_it=clippedPolygonEndPoints->begin();

		while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){
			boundary_case = BoundaryLineCase(*start_it, *end_it, boundLine);

			if(boundary_case == 0){ //in-out
				intersect_point = GetIntersection(*start_it, *end_it, boundLine);
				tempStartPoints->push_back(*start_it);
				tempEndPoints->push_back(intersect_point);

				followWindowStartPoints->push_back(intersect_point);
				
			}
		
			if(boundary_case == 1){ //out-in
				intersect_point = GetIntersection(*start_it, *end_it, boundLine);
				tempStartPoints->push_back(intersect_point);
				tempEndPoints->push_back(*end_it);

				if(!(followWindowStartPoints->empty())){

					followWindowEndPoints->push_back(intersect_point);
				}else{
					last_window_point = intersect_point;
				}
			}
		
			if(boundary_case == 2){ //in-in
				tempStartPoints->push_back(*start_it);
				tempEndPoints->push_back(*end_it);
			}
		
			if(boundary_case == 3){ //out-out
				//Do Not Add Element
				//Do Nothing
			}
			
			start_it++;
			end_it++;
		}

		followWindowEndPoints->push_back(last_window_point);

		clippedPolygonStartPoints->clear();
		clippedPolygonEndPoints->clear();

		for(std::vector<CPoint>::iterator s_it = tempStartPoints->begin(); s_it!= tempStartPoints->end();s_it++){
			clippedPolygonStartPoints->push_back(*s_it);
		}

		for(std::vector<CPoint>::iterator e_it = tempEndPoints->begin(); e_it!= tempEndPoints->end();e_it++){
			clippedPolygonEndPoints->push_back(*e_it);
		}


		for(std::vector<CPoint>::iterator s_it = followWindowStartPoints->begin(); s_it!= followWindowStartPoints->end();s_it++){
			clippedPolygonStartPoints->push_back(*s_it);
		}

		for(std::vector<CPoint>::iterator e_it = followWindowEndPoints->begin(); e_it!= followWindowEndPoints->end();e_it++){
			clippedPolygonEndPoints->push_back(*e_it);
		}


		tempStartPoints->clear();
		tempEndPoints->clear();

		followWindowStartPoints->clear();
		followWindowEndPoints->clear();
	}	


		viewportPolygonStartPoints->clear();
		viewportPolygonEndPoints->clear();

		largesty = height;
		largestx = 0;

		for(std::vector<CPoint>::iterator s_it = clippedPolygonStartPoints->begin(); s_it!= clippedPolygonStartPoints->end();s_it++){
			CPoint translatedPoint;

			translatedPoint.x = s_it->x + (width - window[1].x);
			translatedPoint.y = s_it->y + (height - window[1].y);

			/*scaledPoint.x = translatedPoint.x;
			scaledPoint.y = translatedPoint.y/ratio;*/

			if(largesty > translatedPoint.y)
				largesty = translatedPoint.y;

			if(largestx < translatedPoint.x)
				largestx = translatedPoint.x;

			viewportPolygonStartPoints->push_back(translatedPoint);
		}

		for(std::vector<CPoint>::iterator e_it = clippedPolygonEndPoints->begin(); e_it!= clippedPolygonEndPoints->end();e_it++){
			CPoint translatedPoint;

			translatedPoint.x = e_it->x + (width - window[1].x);
			translatedPoint.y = e_it->y + (height - window[1].y);

			viewportPolygonEndPoints->push_back(translatedPoint);
		}

	/*//////////////////////////////
	ClearWindow();
	start_it=clippedPolygonStartPoints->begin();
	end_it=clippedPolygonEndPoints->begin();
	glColor4f(1.0, 0.0, 0.0, 0.0);
	glDisable( GL_LINE_STIPPLE );
		
	glPushMatrix();
	glBegin(GL_LINES);
		while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){
			glVertex3d(start_it->x, height-start_it->y, 0);
			glVertex3d(end_it->x, height-end_it->y, 0);
			
			start_it++;
			end_it++;
		}

	glEnd();
	glFlush();
	glPopMatrix();
	::SwapBuffers(m_hDC);
	//glClear(GL_COLOR_BUFFER_BIT);
	//////////////////////////*/

}

int CCssample1View::BoundaryLineCase(CPoint p1, CPoint p2, int bound){
	
	//case 0: in-out		bound 0: left boundary
	//case 1: out-in		bound 1: right boundary
	//case 2: in-in			bound 2: bottom boundary
	//case 3: out-out		bound 3: top boundary

	int bound_case = -1;

	if(bound == 0){ //left boundary
		if(p1.x <= left_boundary){ //out
			if(p2.x > left_boundary){ //in
				bound_case = 1;
			}else if(p2.x <= left_boundary){ //out
				bound_case = 3;
			}
		}else{ // in
			if(p2.x > left_boundary){ //in
				bound_case = 2;
			}else if(p2.x <= left_boundary){ //out
				bound_case = 0;
			}
		}

	}


	if(bound == 1){ //right boundary
		if(p1.x <= right_boundary){ //in
			if(p2.x > right_boundary){ //out
				bound_case = 0;
			}else if(p2.x <= right_boundary){ //in
				bound_case = 2;
			}
		}else{ // out
			if(p2.x > right_boundary){ //out
				bound_case = 3;
			}else if(p2.x <= right_boundary){ //in
				bound_case = 1;
			}
		}

	}


	if(bound == 2){  //bottom boundary
		if((height-p1.y) >= bottom_boundary){ //in
			if((height-p2.y) < bottom_boundary){ //out
				bound_case = 0;
			}else if((height-p2.y) >= bottom_boundary){ //in
				bound_case = 2;
			}
		}else{ // out
			if((height-p2.y) < bottom_boundary){ //out
				bound_case = 3;
			}else if((height-p2.y) >= bottom_boundary){ //in
				bound_case = 1;
			}
		}

	}


	if(bound == 3){  //top boundary

		if((height-p1.y) >= top_boundary){ //out
			if((height-p2.y) < top_boundary){ //in
				bound_case = 1;
			}else if((height-p2.y) >= top_boundary){ //out
				bound_case = 3;
			}
		}else{ // in
			if((height-p2.y) < top_boundary){ //in
				bound_case = 2;
			}else if((height-p2.y) >= top_boundary){ //out
				bound_case = 0;
			}
		}

	}

	return bound_case;

}

CPoint CCssample1View::GetIntersection(CPoint p1, CPoint p2, int boundary_line){
	CPoint intersect_point;
	float m;
	float b;

	int boundary;

	int y_intersect;
	int x_intersect;

	if(boundary_line == 0 || boundary_line == 1){
		if(boundary_line == 0)
			boundary = left_boundary;
		else
			boundary = right_boundary;

		m = ((float)(p2.y - p1.y))/(p2.x-p1.x);
		b = p1.y - (m * p1.x);

		y_intersect = (int)(m*boundary) + (int)b;

		intersect_point.x = boundary;
		intersect_point.y = y_intersect;
	
	}else{
		if(boundary_line == 2){
			boundary = height - bottom_boundary;
		}else{
			boundary = height - top_boundary;
		}

		if((p2.x - p1.x) == 0){
			intersect_point.x = p1.x;
			intersect_point.y = boundary;
		}else{

			m = ((float)(p2.y - p1.y))/(p2.x-p1.x);
			b = p1.y - (m * p1.x);

			x_intersect = (int)((boundary-b)/m);
			
			intersect_point.x = (int)((boundary-b)/m);
			intersect_point.y = boundary;
			
		}
	}

	return intersect_point;
}

/////////////////////////////////////////////////////////////////////////////
// CCssample1View printing

BOOL CCssample1View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CCssample1View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CCssample1View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CCssample1View diagnostics

#ifdef _DEBUG
void CCssample1View::AssertValid() const
{
	CView::AssertValid();
}

void CCssample1View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CCssample1Doc* CCssample1View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CCssample1Doc)));
	return (CCssample1Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCssample1View message handlers

BOOL CCssample1View::SetWindowPixelFormat(HDC hDC)
{
	PIXELFORMATDESCRIPTOR pixelDesc;

	pixelDesc.nSize		= sizeof(PIXELFORMATDESCRIPTOR);
	pixelDesc.nVersion	= 1;
	/***************
	pixelDesc.dwFlags=PFD_DRAW_TO_WINDOW | 
					PFD_DRAW_TO_BITMAP | 
					PFD_SUPPORT_OPENGL | 
					PFD_SUPPORT_GDI | 
					PFD_STEREO_DONTCARE;
	*********************/
	//Stuff with double buffering
    pixelDesc.dwFlags =    PFD_DRAW_TO_WINDOW | 
                PFD_SUPPORT_OPENGL | 
                PFD_DOUBLEBUFFER | //set as a double buffer mode. (otherwise, single buffer mode)
                PFD_STEREO_DONTCARE;  
	
	pixelDesc.iPixelType	= PFD_TYPE_RGBA;
	pixelDesc.cColorBits	= 32;
	pixelDesc.cRedBits		= 8;
	pixelDesc.cRedShift		= 16;
	pixelDesc.cGreenBits	= 8;
	pixelDesc.cGreenShift	= 8;
	pixelDesc.cBlueBits		= 8;
	pixelDesc.cBlueShift	= 0;
	pixelDesc.cAlphaBits	= 0;
	pixelDesc.cAlphaShift	= 0;
	pixelDesc.cAccumBits	= 64;	
	pixelDesc.cAccumRedBits	= 16;
	pixelDesc.cAccumGreenBits	= 16;
	pixelDesc.cAccumBlueBits	= 16;
	pixelDesc.cAccumAlphaBits	= 0;
	pixelDesc.cDepthBits		= 32;
	pixelDesc.cStencilBits		= 8;
	pixelDesc.cAuxBuffers		= 0;
	pixelDesc.iLayerType		= PFD_MAIN_PLANE;
	pixelDesc.bReserved			= 0;
	pixelDesc.dwLayerMask		= 0;
	pixelDesc.dwVisibleMask		= 0;
	pixelDesc.dwDamageMask		= 0;

	m_GLPixelIndex = ChoosePixelFormat( hDC, &pixelDesc);
	if (m_GLPixelIndex==0) // Let's choose a default index.
	{
		m_GLPixelIndex = 1;	
		if (DescribePixelFormat(hDC, m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc)==0)
		{
			return FALSE;
		}
	}

	if (SetPixelFormat( hDC, m_GLPixelIndex, &pixelDesc)==FALSE)
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CCssample1View::CreateViewGLContext(HDC hDC)
{
	m_hGLContext = wglCreateContext(m_hDC);
    if (m_hGLContext == NULL)
    {
        return FALSE;
    }

    if (wglMakeCurrent(m_hDC, m_hGLContext)==FALSE)
    {
        return FALSE;
    }
    return TRUE;
}

int CCssample1View::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	HWND hWnd = GetSafeHwnd();
    m_hDC = ::GetDC(hWnd);

    if (SetWindowPixelFormat(m_hDC)==FALSE)
        return 0;
    if (CreateViewGLContext(m_hDC)==FALSE)
        return 0;
	return 0;
}

void CCssample1View::OnDestroy() 
{
	CView::OnDestroy();
	
	if(wglGetCurrentContext()!=NULL) 
    {
        // make the rendering context not current
        wglMakeCurrent(NULL, NULL) ;
    }
    
    if (m_hGLContext!=NULL)
    {
        wglDeleteContext(m_hGLContext);
        m_hGLContext = NULL;
    }

    // Now the associated DC can be released.
    CView::OnDestroy(); 	
}

void CCssample1View::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	polygonEndPoints->push_back(m_LDownPos);
	polygonStartPoints->push_back(m_LDownPos);

	CView::OnRButtonDown(nFlags, point);
}

void CCssample1View::OnLButtonDown(UINT nFlags, CPoint point) 
{
	l_mouseDown = TRUE;
	m_LDownPos = point;

	if(menuOption == -1){
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if(menuOption == 0){
		windowPoints[0] = point;
		glClear(GL_COLOR_BUFFER_BIT);
	}

	if(menuOption == 1){
		DrawDashedWindow();
		if(polygonStartPoints->empty()){
			start = m_LDownPos;
			polygonStartPoints->push_back(start);
		}else{
			polygonStartPoints->clear();
			polygonEndPoints->clear();

			start = m_LDownPos;
			polygonStartPoints->push_back(start);
		}
		glClear(GL_COLOR_BUFFER_BIT);

	}

	if(menuOption == 2){
		ClipPolygon();

		ClearWindow();
		std::vector<CPoint>::iterator start_it=clippedPolygonStartPoints->begin();
		std::vector<CPoint>::iterator end_it=clippedPolygonEndPoints->begin();
		glColor4f(1.0, 0.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		
		glPushMatrix();
		glBegin(GL_LINES);
			while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){
				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

		glEnd();
		glFlush();
		glPopMatrix();
		::SwapBuffers(m_hDC);
	}

	if(menuOption == 10){
		ClearWindow();

		std::vector<CPoint>::iterator start_it=clippedPolygonStartPoints->begin();
		std::vector<CPoint>::iterator end_it=clippedPolygonEndPoints->begin();
		glColor4f(1.0, 0.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		glPointSize(1.0);
		
		glPushMatrix();
		glBegin(GL_LINES);
			while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

		glEnd();

		FillPolygon(left_boundary, top_boundary);

		/*
		glFlush();
		glPopMatrix();
	      
		wglMakeCurrent(m_hDC,NULL);
		wglMakeCurrent(m_hDC, m_hGLContext);

		::SwapBuffers(m_hDC);
		*/
	}

	//CView::OnLButtonDown(nFlags, point);
}

void CCssample1View::OnLButtonUp(UINT nFlags, CPoint point) 
{
	l_mouseDown=false;
	//one line has drawn, clear everything
	CView::OnLButtonUp(nFlags, point);
}

void CCssample1View::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (l_mouseDown){
		if(menuOption == -1)
		{
			start=m_LDownPos;
			m_LDownPos = point;
			InvalidateRect(NULL, FALSE);
		}
		
		if(menuOption == 0){

			m_LDownPos = point;
			windowPoints[1] = point;

			InvalidateRect(NULL, FALSE);
		}
		
		if(menuOption == 1){
			glClear(GL_COLOR_BUFFER_BIT);
			DrawDashedWindow();
			m_LDownPos = point;

			InvalidateRect(NULL, FALSE);
		}

		if(menuOption == 20){
			glClear(GL_COLOR_BUFFER_BIT);
			ChangeViewPort();
			m_LDownPos = point;

			InvalidateRect(NULL, FALSE);
		}
	}

	//DBOUT("Mouse: x=" << point.x << " y=" << point.y << "\n");
	
	CView::OnMouseMove(nFlags, point);
}

void CCssample1View::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, width, 0.0, height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// for double buffering
	glDrawBuffer(GL_BACK);
	
}

void CCssample1View::OnDblBuf() 
{
	dblBuf=true;	
}

void CCssample1View::OnNoDblBuf() 
{
	dblBuf=false;	
}


void CCssample1View::OnPolygonclippingDrawWindow()
{
	menuOption = 0;
}

void CCssample1View::OnPolygonclippingDrawpolygon()
{
	menuOption = 1;
}

void CCssample1View::OnPolygonclippingClip()
{
	menuOption = 2;
}

void CCssample1View::OnOptionsClear()
{
	windowPoints[0] = 0;
	windowPoints[1] = 0;
	windowPoints[2] = 0;
	windowPoints[3] = 0;

	polygonStartPoints->clear();
	polygonEndPoints->clear();

	menuOption = -1;
	ClearWindow();
}


void CCssample1View::OnRButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	std::vector<CPoint>::iterator start_it=polygonStartPoints->begin();
	std::vector<CPoint>::iterator end_it=polygonEndPoints->begin();

	glDisable( GL_LINE_STIPPLE );
	glColor4f(0.5, 1.0, 0.0, 0.0);
	glBegin(GL_LINES);
		/*while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){
			glVertex3d(start_it->x, height-start_it->y, 0);
			glVertex3d(end_it->x, height-end_it->y, 0);

			start_it++;
			end_it++;
		}*/

		polygonEndPoints->push_back(polygonStartPoints->front());

		glVertex3d(polygonStartPoints->back().x, height-polygonStartPoints->back().y, 0);
		glVertex3d(polygonEndPoints->back().x, height-polygonEndPoints->back().y, 0);

	glEnd();
	glFlush();
	glPopMatrix();

    wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC, m_hGLContext);

	::SwapBuffers(m_hDC);


	/*
	start_it=polygonStartPoints->begin();
	end_it=polygonEndPoints->begin();

	glDisable( GL_LINE_STIPPLE );
	glColor4f(1.0, 1.0, 1.0, 0.0);
	glBegin(GL_LINES);
		while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){
			glVertex3d(start_it->x, height-start_it->y, 0);
			glVertex3d(end_it->x, height-end_it->y, 0);

			start_it++;
			end_it++;
		}

	glEnd();
	glFlush();
	glPopMatrix();

    wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC, m_hGLContext);

	::SwapBuffers(m_hDC);
	*/

	CView::OnRButtonDblClk(nFlags, point);
}


void CCssample1View::OnRegoinfillingFillpolygon()
{
	menuOption = 10;
}


void CCssample1View::OnRefreshRefreshscreen()
{
	// TODO: Add your command handler code here
	::SwapBuffers(m_hDC);
}


void CCssample1View::OnWindowtoviewportViewport()
{
	// TODO: Add your command handler code here
	window[0].x = windowPoints[0].x;
	window[0].y = windowPoints[0].y;

	window[1].x = windowPoints[1].x;
	window[1].y = windowPoints[1].y;

	window[2].x = windowPoints[2].x;
	window[2].y = windowPoints[2].y;

	window[3].x = windowPoints[3].x;
	window[3].y = windowPoints[3].y;

	MiddleViewport.x = ( window[0].x + window[1].x ) / 2;
	MiddleViewport.y = ( window[0].y + window[1].y ) / 2;

	OriginalWin = window[1];

	viewport[1].x = window[1].x + (width - window[1].x);
	viewport[1].y = window[1].y + (height - window[1].y);

	viewport[0].x = window[0].x + (width - window[1].x);
	viewport[0].y = window[0].y + (height - window[1].y);

	viewport[2].x = window[2].x + (width - window[1].x);
	viewport[2].y = window[2].y + (height - window[1].y);

	viewport[3].x = window[3].x + (width - window[1].x);
	viewport[3].y = window[3].y + (height - window[1].y);

		viewportPolygonStartPoints->clear();
		viewportPolygonEndPoints->clear();

		for(std::vector<CPoint>::iterator s_it = clippedPolygonStartPoints->begin(); s_it!= clippedPolygonStartPoints->end();s_it++){
			CPoint translatedPoint;

			translatedPoint.x = s_it->x + (width - window[1].x);
			translatedPoint.y = s_it->y + (height - window[1].y);

			viewportPolygonStartPoints->push_back(translatedPoint);
		}

		for(std::vector<CPoint>::iterator e_it = clippedPolygonEndPoints->begin(); e_it!= clippedPolygonEndPoints->end();e_it++){
			CPoint translatedPoint;

			translatedPoint.x = e_it->x + (width - window[1].x);
			translatedPoint.y = e_it->y + (height - window[1].y);

			viewportPolygonEndPoints->push_back(translatedPoint);
		}

		std::vector<CPoint>::iterator start_it=viewportPolygonStartPoints->begin();
		std::vector<CPoint>::iterator end_it=viewportPolygonEndPoints->begin();
		glColor4f(1.0, 0.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		glPointSize(1.0);
		
		glPushMatrix();
		glBegin(GL_LINES);
			while(start_it != viewportPolygonStartPoints->end() && end_it != viewportPolygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

		glEnd();
		glFlush();
		glPopMatrix();

	glColor4f(1.0, 1.0, 1.0, 0.0);
	glEnable( GL_LINE_STIPPLE );
	glLineStipple(1, 0x3F07);

	glPushMatrix();
	glBegin(GL_LINES);
		glVertex3d(window[0].x, height-window[0].y, 0);
		glVertex3d(window[2].x, height-window[2].y, 0);

		glVertex3d(window[0].x, height-window[0].y, 0);
		glVertex3d(window[3].x, height-window[3].y, 0);

		glVertex3d(window[2].x, height-window[2].y, 0);
		glVertex3d(window[1].x, height-window[1].y, 0);

		glVertex3d(window[3].x, height-window[3].y, 0);
		glVertex3d(window[1].x, height-window[1].y, 0);

	glEnd();
	
		glColor4f(1.0, 1.0, 1.0, 0.0);
		glDisable( GL_LINE_STIPPLE );

		glPushMatrix();
		glBegin(GL_LINES);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[2].x, height-viewport[2].y, 0);

			glVertex3d(viewport[0].x, height-viewport[0].y, 0);
			glVertex3d(viewport[3].x, height-viewport[3].y, 0);

			glVertex3d(viewport[2].x, height-viewport[2].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);

			glVertex3d(viewport[3].x, height-viewport[3].y, 0);
			glVertex3d(viewport[1].x, height-viewport[1].y, 0);
			
		glEnd();

		glColor4f(1.0, 1.0, 1.0, 0.0);
		glPointSize(9.0);
		glPushMatrix();
		glBegin(GL_POINTS);
			
			glVertex2i(window[0].x, height-window[0].y);
			glVertex2i(MiddleViewport.x, height-MiddleViewport.y);
			glVertex2i(viewport[0].x, height-viewport[0].y);

		glEnd();

		glFlush();
		glPopMatrix();

		start_it=polygonStartPoints->begin();
		end_it=polygonEndPoints->begin();

		glColor4f(0.5, 1.0, 0.0, 0.0);
		glDisable( GL_LINE_STIPPLE );
		
		glPushMatrix();
		glBegin(GL_LINES);

			while(start_it != polygonStartPoints->end() && end_it != polygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

			start_it=clippedPolygonStartPoints->begin();
			end_it=clippedPolygonEndPoints->begin();


			glColor4f(1.0, 0.0, 0.0, 0.0);

			while(start_it != clippedPolygonStartPoints->end() && end_it != clippedPolygonEndPoints->end()){

				glVertex3d(start_it->x, height-start_it->y, 0);
				glVertex3d(end_it->x, height-end_it->y, 0);
			
				start_it++;
				end_it++;
			}

		glEnd();
		glFlush();
		glPopMatrix();
      
	wglMakeCurrent(m_hDC,NULL);
	wglMakeCurrent(m_hDC, m_hGLContext);

	::SwapBuffers(m_hDC);

	menuOption = 20;
}


void CCssample1View::OnOptionsCleareverything()
{
	// TODO: Add your command handler code here
	polygonStartPoints->clear();
	polygonEndPoints->clear();

	followWindowStartPoints->clear();
	followWindowEndPoints->clear();

	clippedPolygonStartPoints->clear();
	clippedPolygonEndPoints->clear();

	//Clear buffer content, so that no old content remains in the buffer
	glClear(GL_COLOR_BUFFER_BIT);
	
	// for double buffering
	if(dblBuf)
		glDrawBuffer(GL_BACK);
	else
		glDrawBuffer(GL_FRONT_AND_BACK);
}
