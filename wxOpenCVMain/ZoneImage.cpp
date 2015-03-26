#include "FenetrePrincipale.h"
#include "FenetreInfo.h"
#include "wxOsgApp.h"
#include "imagestat.h"
#include "ControleCamera.h"
#include "Fenetre3D.h"
#include "FenetreSeqOpe.h"

ZoneImage::ZoneImage(wxWindow *parent,wxSize w)
    : wxScrolled<wxWindow>(parent, wxID_ANY)   {
    SetScrollRate( 1, 1 );
    SetVirtualSize( w.x, w.y );
    SetBackgroundColour( *wxWHITE );

modeRect=false;
modeCoupe=false;
facteurZoom=-1;
osgApp=NULL;
bitmapAffiche=NULL;
f3D=NULL;
//SetBackgroundStyle(wxBG_STYLE_PAINT);
for (int i=0;i<10;i++)
	{
	rectSelect[i].SetSize(wxSize(0,0));
	rectCoupe[i].SetSize(wxSize(0,0));
	}
indRect=0;
indCoupe=0;
modeComplexe=0;
Bind(wxEVT_MOTION, &ZoneImage::OnMouseMove,this);
Bind(wxEVT_PAINT,&ZoneImage::OnPaint,this);
Bind(wxEVT_DESTROY,&ZoneImage::OnClose,this);
Bind(wxEVT_ACTIVATE, &ZoneImage::OnActivate,this);
Bind(wxEVT_LEFT_UP, &ZoneImage::OnLeftButtonUp,this);
Bind(wxEVT_LEFT_DOWN, &ZoneImage::OnLeftButtonDown,this);
Bind(wxEVT_CONTEXT_MENU, &ZoneImage::OnMenuContext,this);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::Vue3D,this,Menu_3D);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::SelectPalette,this,NOIRETBLANC_,NOIRETBLANC_+9);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::ModeComplexe,this,M_MODULE_,PHASE_RD);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::MAJZoom,this,ZOOM1SUR2,ZOOM8SUR1);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::SequenceOperation,this,SEQ_OPE);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::RazSeqOp,this,STOP_SEQ);

/*Connect(ZOOM1SUR2,ZOOM8SUR1  ,wxCommandEventHandler(ZoneImage::MAJZoom));
Connect(ZOOM1SUR1,wxEVT_MENU,  wxCommandEventHandler(ZoneImage::MAJZoom));
Connect(ZOOM2SUR1,wxEVT_MENU,  wxCommandEventHandler(ZoneImage::MAJZoom));
Connect(ZOOM4SUR1,wxEVT_MENU,  wxCommandEventHandler(ZoneImage::MAJZoom));
Connect(ZOOM8SUR1,wxEVT_MENU,  wxCommandEventHandler(ZoneImage::MAJZoom));*/
}



void ZoneImage::OnSize( wxSizeEvent &w)
{

wxClientDC dc(this);
//f->DrawWindow(dc);

}

void ZoneImage::OnClose(wxWindowDestroyEvent& event)
{
delete bitmapAffiche;
if (f3D)
	{
	wxCloseEvent w;
	((Fenetre3D*)f3D)->OnClose(w);
	}

}


void ZoneImage::OnPaint(wxPaintEvent &evt)
{
    wxPaintDC dc(this);

    // this call is vital: it adjusts the dc to account for the current
    // scroll offset
    PrepareDC(dc);

	f->DrawWindow (dc);
	f->TracerContour(dc);
	f->TracerLigneHough(dc);
	f->TracerLigneProbaHough(dc);
	f->TracerCercleHough(dc);
	f->TracerBonCoin(dc);
/*        dc.SetPen( *wxRED_PEN );
    dc.SetBrush( *wxTRANSPARENT_BRUSH );
    dc.DrawRectangle( 0, 0, 200, 200 );*/
}


void ZoneImage::OnActivate(wxActivateEvent &w)
{
if (osgApp)
	{
	osgApp->IdFenetreActive(f->IdFenetre());
	if (f->Cam() && f->Cam()->IsRunning())
		{
		wxString s= "Control :" +f->GetTitle();
		osgApp->CtrlCamera()->SetTitle(s);
		osgApp->CtrlCamera()->DefCamera(f->Cam());
		}
	}
}


void ZoneImage::OnMouseMove(wxMouseEvent &event)
{

if (!osgApp)
	return;
osgApp->DefUtilisateurAbsent(0);
ImageInfoCV *imAcq=f->ImAcq();
wxPoint point = event.GetPosition();
wxRect	r(GetClientRect());
if (!r.Contains(point) ||  this->f!=osgApp->Graphique())
	return;
point=RepereEcranImage(point);
wxClientDC dc(this);
//f->TracerDIB(f->ImageAffichee(), dc,&point);


if (f->BarreEtat() && f->BarreEtat()->Curseur()  && point.x>=0 && point.x<imAcq->cols && point.y>=0 && point.y<imAcq->rows)
	{
	BarreInfo *barreEtat=f->BarreEtat();
	cv::Vec3b x;
	cv::Vec3f xx;
	cv::Vec6f xxx;
	std::complex<float> zz[3];
	int val;
	double dVal;
	switch(imAcq->type()){
	case CV_32FC1:
		dVal = imAcq->at<float>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,dVal);
		break;
	case CV_64FC1:
		dVal = imAcq->at<double>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,dVal);
		break;
	case CV_32FC3 :
		xx=imAcq->at<cv::Vec3f>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,xx[2],xx[1],xx[0]);
		break;
	case CV_32FC(6) :
		xxx=imAcq->at<cv::Vec6f>(point.y,point.x);
		zz[0]=std::complex<float>(xxx[0],xxx[1]);
		zz[1]=std::complex<float>(xxx[2],xxx[3]);
		zz[2]=std::complex<float>(xxx[4],xxx[5]);
		barreEtat->UpdateCurseur(point.x,point.y,zz[2],zz[1],zz[0]);
		break;
	case CV_64FC3 :
		xx=imAcq->at<cv::Vec3d>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,xx[2],xx[1],xx[0]);
		break;
	case CV_32SC1:
		val = imAcq->at<int>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_32SC3:
		x=imAcq->at<cv::Vec3i>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,x[2],x[1],x[0]);
		break;
	case CV_8UC1:
		val = imAcq->at<unsigned char>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_8UC3 :
		x=imAcq->at<cv::Vec3b>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,x[2],x[1],x[0]);
		break;
	case CV_16UC1 :
		val = imAcq->at<unsigned short>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_16SC1 :
		val = imAcq->at< short>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_16SC3 :
		xx=imAcq->at<cv::Vec3s>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,xx[2],xx[1],xx[0]);
		break;
	case CV_16UC(48):
		{
		barreEtat->UpdateCurseur(point.x,point.y,0/*imRegionBrute->LitPixelEntier(point.y,point.x)*/);
		if (event.m_shiftDown)
			{
			//osgApp->Outils()->RegionPrincipaleSelect(numRegion);
/*
			if (imCtr)
				{
				int i1=NULL;//imRegionBrute->LitPixelEntier(point.y,point.x);
				Contour	*tabCtr=NULL;//imCtr->tabCtr;
				wxClientDC dc(this);
				for (int i=1;i<tabCtr[i1].nbPixels;i++)
					{
					wxPoint p1(tabCtr[i1].tabPixels[i-1].col,tabCtr[i1].tabPixels[i-1].lig);
					wxPoint p2(tabCtr[i1].tabPixels[i].col,tabCtr[i1].tabPixels[i].lig);
					p1=RepereImageEcran(p1);
					p2=RepereImageEcran(p2);
					dc.DrawLine(p1,p2);
					}
				}
*/
			}
//		if (event.m_controlDown)
			//osgApp->Outils()->RegionSecondaireSelect(0/*imRegionBrute->LitPixelEntier(point.y,point.x)*/);
		}
		break;
		}
	 }
if (event.ShiftDown())
	{
	ImageInfoCV *t=NULL;
	switch(f->ModeImage()){
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:
		t = imAcq;
		break;
		}
	if (t)
		{
		osgApp->ImgStat()->AfficheValCurseur(t,point.y,point.x);
		}
	}

if (!f->ModeRectangle() && !f->ModeCoupe())
	return;
if (f->ModeRectangle())
	{
	if ( event.LeftIsDown()  ) // detect if the mouse is down and dragging a lasso
		{
		wxRect	rTmp=rectSelect[indRect];
		rectSelect[indRect].SetRight(point.x);
		rectSelect[indRect].SetBottom(point.y);
		    dc.SetBrush( *wxTRANSPARENT_BRUSH );

		// invalidate the lasso rect so that it's drawn while dragging the mouse.
			RefreshRect(RepereImageEcran(rectSelect[indRect]),false);
		Update();
			RefreshRect(RepereImageEcran(rTmp),false);
		// you may or may not need to do an update to draw now.
		Update();
		// TODO2: draw the lasso rect in your OnPaint code.  i.e. draw it if (!rectSelect[indRect].IsEmpty())
		TracerRectangle(indRect,1);
/*		wxClientDC dc(this);
		wxPoint p1(RepereImageEcran(rectSelect[indRect].GetTopLeft()));
		//ClientToScreen(&p1.x,&p1.y);
		wxBrush	tr=*wxTRANSPARENT_BRUSH;
		dc.SetBrush(tr);
		int		fZoomNume,fZoomDeno;

		CalculZoom(fZoomNume,fZoomDeno);
		wxRect rTrace(p1.x,p1.y,rectSelect[indRect].width*fZoomNume/fZoomDeno,rectSelect[indRect].height*fZoomNume/fZoomDeno);
		dc.DrawRectangle(p1.x,p1.y,rectSelect[indRect].width*fZoomNume/fZoomDeno,rectSelect[indRect].height*fZoomNume/fZoomDeno);
		dc.DrawLine(rTrace.GetBottomLeft(),rTrace.GetTopRight());
		dc.DrawLine(rTrace.GetTopLeft(),rTrace.GetBottomRight());
		DrawRectangles(dc);*/
		}
	}
if (modeCoupe)
	{
	if ( event.LeftIsDown()  ) // detect if the mouse is down and dragging a lasso
		{
		wxRect	rTmp=rectCoupe[indCoupe],rTmp2=RepereImageEcran(rTmp);
		f->RedresseRectangle(rTmp2);
	//	rTmp.Inflate(2,2);
		RefreshRect(rTmp2,false);
		rectCoupe[indCoupe].SetRight(point.x);
		rectCoupe[indCoupe].SetBottom(point.y);

		rTmp=rectCoupe[indCoupe];
		rTmp2=RepereImageEcran(rTmp);
		f->RedresseRectangle(rTmp2);
		//rTmp.Inflate(2,2);

		rTmp2=RepereImageEcran(rTmp);
		RefreshRect(rTmp2,false);
		// you may or may not need to do an update to draw now.
		Update();
		// TODO2: draw the lasso rect in your OnPaint code.  i.e. draw it if (!rectSelect[indRect].IsEmpty())
		wxClientDC dc(this);
		//ClientToScreen(&p1.x,&p1.y);
		wxBrush	tr=*wxTRANSPARENT_BRUSH;
		dc.SetBrush(tr);
		wxPoint pTmp1= rectCoupe[indCoupe].GetTopLeft();
		wxPoint pTmp2=rectCoupe[indCoupe].GetBottomRight();
		dc.DrawLine(RepereImageEcran(pTmp1),RepereImageEcran(pTmp2));
		TracerLesCoupes(dc);
		}
	}

}

void ZoneImage::OnLeftButtonUp(wxMouseEvent &event)
{
f->GestionCurseurSouris(event);
if (!modeRect && !modeCoupe)
	return;
if (modeRect)
	{
	if ( abs(rectSelect[indRect].GetWidth()*rectSelect[indRect].GetWidth()))
		{
	 /*
					// TODO: commit the lasso selected image to something.
					wxMemoryDC dcMem;
					wxBitmap bitmap(rectSelect[indRect].GetWidth(), rectSelect[indRect].GetHeight());
					dcMem.SelectObject(bitmap);

					wxMemoryDC dcYourLoadedImage;

					dcMem.Blit(0, 0, rectSelect[indRect].GetWidth(), rectSelect[indRect].GetHeight(), &dcYourLoadedImage, rectSelect[indRect].GetLeft(), rectSelect[indRect].GetTop());
	*/
/*		imAcq->StatZone(rectSelect[indRect].GetTop(),rectSelect[indRect].GetBottom(),
			rectSelect[indRect].GetLeft(),rectSelect[indRect].GetRight(),stat);*/
		osgApp->ImgStat()->MAJMinMaxHisto();
		if (indRect>=0)
			{
			int ind=indRect;
			osgApp->ImgStat()->Plot(true);
			}
		}
	}
if (modeCoupe)
	if ( rectCoupe[indCoupe].width )
		osgApp->ImgStat()->MAJCoupe(rectCoupe[indCoupe]);
}

void ZoneImage::OnLeftButtonDown(wxMouseEvent &event)
{
wxPoint point = event.GetPosition();
wxRect	r(GetClientRect());
if (!r.Contains(point) ||  this->f!=osgApp->Graphique())
	return;
point=RepereEcranImage(point);

if (f->ImAcq() && point.x>=0 && point.x<f->ImAcq()->cols && point.y>=0 && point.y<f->ImAcq()->rows && f->ImAcq()->StatComposante())
	{
	if (osgApp->ImgStat())
		{
		cv::Vec3b x;
		cv::Vec3f xx;
		cv::Vec6f xxx;
		std::complex<float> zz[3];
		switch(f->ImAcq()->type())
			{
			case CV_32SC1:
				x[0] = f->ImAcq()->at<int>(point.y,point.x);
				osgApp->ImgStat()->SelectRegion(x[0],0);
				break;
			case CV_32SC3:
				x=f->ImAcq()->at<cv::Vec3i>(point.y,point.x);
				osgApp->ImgStat()->SelectRegion(x[0],0);
				osgApp->ImgStat()->SelectRegion(x[1],1);
				osgApp->ImgStat()->SelectRegion(x[2],2);
				break;
			}
		}
	}

if (event.ShiftDown()&&f->ImAcq() && point.x>=0 && point.x<f->ImAcq()->cols && point.y>=0 && point.y<f->ImAcq()->rows )
	{
	ShapedFrame *shapedFrame = new ShapedFrame(f,point);
	shapedFrame->Show(true);
	}

if (osgApp->ModeSouris()==SELECTION_EN_COURS)
	{
	GestionCurseurSouris(event);
	return;
	}
if (!ModeRectangle() && !ModeCoupe())
	return;
wxPoint point2 = ScreenToClient(event.GetPosition()); // this is in screen coords.  Use ScreenToClient if you need this in coords for your window
//point=RepereEcranImage(point);
if (ModeRectangle())
	{
	wxRect r(*RectangleSelec());
	r.Inflate(2,2);
	RefreshRect(RepereImageEcran(r),false);
	RectangleSelec()->SetLeft(point.x);
	RectangleSelec()->SetTop(point.y);
	}
if (ModeCoupe())
	{
	f->RedresseRectangle(*CoupeSelec());
	RefreshRect(RepereImageEcran(*CoupeSelec()),false);
	CoupeSelec()->SetLeft(point.x);
	CoupeSelec()->SetTop(point.y);
	}
// if you need the base class handler to get called, then call event.Skip();
}

void ZoneImage::GestionCurseurSouris(wxMouseEvent &event)
{
if (osgApp->Op1()==NULL)
	osgApp->DefOperande1(f->ImAcq(),f->IdFenetre());
else if (osgApp->Op2()==NULL)
	osgApp->DefOperande2(f->ImAcq(),f->IdFenetre());
}


void ZoneImage::TracerRectangle(int ind,bool croix)
{
wxClientDC dc(this);
wxPoint pTmp=rectSelect[indRect].GetTopLeft();
wxPoint p1(RepereImageEcran(pTmp));
ClientToScreen(&p1.x,&p1.y);
wxBrush	tr=*wxTRANSPARENT_BRUSH;
dc.SetBrush(tr);
int		fZoomNume,fZoomDeno;

f->CalculZoom(fZoomNume,fZoomDeno);
wxRect rTrace(p1.x,p1.y,rectSelect[indRect].width*fZoomNume/fZoomDeno,rectSelect[indRect].height*fZoomNume/fZoomDeno);
rTrace=RepereImageEcran(rectSelect[indRect]);
//dc.DrawRectangle(p1.x,p1.y,rectSelect[indRect].width*fZoomNume/fZoomDeno,rectSelect[indRect].height*fZoomNume/fZoomDeno);
dc.DrawRectangle(rTrace);
if (croix)
	{
	dc.DrawLine(rTrace.GetBottomLeft(),rTrace.GetTopRight());
	dc.DrawLine(rTrace.GetTopLeft(),rTrace.GetBottomRight());
	}
TracerLesRectangles(dc);

}

void ZoneImage::TracerLesRectangles (wxDC &hdc)
{
wxBrush wt=*wxTRANSPARENT_BRUSH;
hdc.SetBrush(wt);
for (int i=0;i<10;i++)
	if (Rectangle(i)->GetWidth()!=0 || Rectangle(i)->GetHeight()!=0)
		{
		hdc.DrawRectangle(RepereImageEcran(*Rectangle(i)));
		if (i==IndiceRectangleSelec())
			{
			int		fZoomNume,fZoomDeno;

			f->CalculZoom(fZoomNume,fZoomDeno);
			wxPoint pTmp(Rectangle(i)->GetTopLeft());
			wxPoint p1(RepereImageEcran(pTmp));
			wxRect rTrace(RepereImageEcran(*Rectangle(i)));
			hdc.DrawLine(rTrace.GetBottomLeft(),rTrace.GetTopRight());
			hdc.DrawLine(rTrace.GetTopLeft(),rTrace.GetBottomRight());
			}
		}
}

void ZoneImage::TracerLesCoupes (wxDC &hdc)
{
wxBrush wt=*wxTRANSPARENT_BRUSH;
hdc.SetBrush(wt);
for (int i=0;i<10;i++)
	if (Coupe(i)->GetWidth()!=0 || Coupe(i)->GetHeight()!=0)
		{
		wxPoint pTmp1(Coupe(i)->GetTopLeft());
		wxPoint pTmp2(Coupe(i)->GetBottomRight());
		hdc.DrawLine(RepereImageEcran(pTmp1),RepereImageEcran(pTmp2));
		}
}

wxPoint ZoneImage::RepereEcranImage(wxPoint &p)
{
wxRect	recSrc(0,0,f->ImAcq()->cols,f->ImAcq()->rows);
wxPoint pt(0,0);
long	mini=0,maxi;
int		x=GetScrollPos(wxHORIZONTAL);
int		y=GetScrollPos(wxVERTICAL);
int		fZoomNume,fZoomDeno;

f->CalculZoom(fZoomNume,fZoomDeno);

maxi=GetScrollRange(wxVERTICAL);
pt.x = (long)((p.x+x)*fZoomDeno/fZoomNume+recSrc.GetLeft());
pt.y = (long)((p.y+y)*fZoomDeno/fZoomNume+recSrc.GetTop());

return pt;
}

wxPoint ZoneImage::RepereImageEcran(wxPoint &p)
{
wxRect	recSrc(0,0,f->ImAcq()->cols,f->ImAcq()->rows);
wxPoint pt(0,0);
long	maxi;
int		x=GetScrollPos(wxHORIZONTAL);
int		y=GetScrollPos(wxVERTICAL);
int		fZoomNume,fZoomDeno;

f->CalculZoom(fZoomNume,fZoomDeno);

maxi=GetScrollRange(wxVERTICAL);
	pt.x = (long)((p.x-recSrc.GetLeft())*fZoomNume/fZoomDeno-x);
	pt.y = (long)((p.y-recSrc.GetTop())*fZoomNume/fZoomDeno-y);
wxPoint pt2=RepereEcranImage(pt);
return pt;
}

wxRect ZoneImage::RepereImageEcran(wxRect &r)
{
wxPoint	p1Img(r.GetLeftTop()),p2Img(r.GetBottomRight());
wxPoint	p1=RepereImageEcran(p1Img);
wxPoint	p2=RepereImageEcran(p2Img);
return wxRect(p1,p2);
}
wxRect ZoneImage::RepereEcranImage(wxRect &r)
{
wxPoint	p1(r.GetLeftTop()),p2(r.GetBottomRight());
wxPoint	p1Img=RepereEcranImage(p1);
wxPoint	p2Img=RepereEcranImage(p2);
return wxRect(p1Img,p2Img);
}


wxMenu *ZoneImage::CreateMenuPalette(wxString *title)
{
    wxMenu *menu = new wxMenu;
    menu->AppendCheckItem(NOIRETBLANC_, _("&Linear\tCtrl-F1"));
    menu->AppendCheckItem(NOIRETBLANC_+1, _("&Jet\t"));
    menu->AppendCheckItem(NOIRETBLANC_+2, _("&Rainbow\t"));
    menu->AppendCheckItem(NOIRETBLANC_+3, _("&Owner\t"));
    menu->AppendCheckItem(NOIRETBLANC_+4, _("&Reverse Owner\t"));
    menu->AppendCheckItem(NOIRETBLANC_+5, _("&Linear 8 bits\tCtrl-F2"));
    menu->AppendCheckItem(NOIRETBLANC_+6, _("&Rainbow 8bits\t"));
    menu->AppendCheckItem(NOIRETBLANC_+7, _("&Linear loop 8 bis\t"));
    menu->AppendCheckItem(NOIRETBLANC_+8, _("&Rainbow loop 8 bits\t"));
    menu->AppendCheckItem(NOIRETBLANC_+9, _("&Randomize\t"));
	menu->Check(f->IndPalette()+NOIRETBLANC_,true);

    return menu;
}

wxMenu *ZoneImage::CreateMenuComplex(wxString *title)
{
    wxMenu *menu = new wxMenu;
    menu->Append(M_MODULE_, _("modulus"));
    menu->Append(PARTIE_REELLE, _("real part"));
    menu->Append(PARTIE_IMAGINAIRE, _("imaginary part"));
    menu->Append(LOG_DB, _("10Log z"));
    menu->Append(PHASE_RD, _("phase (rd)"));

    return menu;
}

wxMenu *ZoneImage::CreateMenuZoom(wxString *title)
{
    wxMenu *menu = new wxMenu;
    menu->Append(ZOOM1SUR2, _T("&Zoom 1/2\tCtrl-F1"));
    menu->Append(ZOOM1SUR1, _T("&Zoom x1\tCtrl-F2"));
    menu->Append(ZOOM2SUR1, _T("&Zoom x2\tCtrl-F2"));
    menu->Append(ZOOM4SUR1, _T("&Zoom x4\tCtrl-F2"));
    menu->Append(ZOOM8SUR1, _T("&Zoom x8\tCtrl-F2"));

    return menu;
}

void ZoneImage::OnMenuContext(wxContextMenuEvent& event)
{
    wxPoint point = event.GetPosition();
    // If from keyboard
    if (point.x == -1 && point.y == -1) {
        wxSize size = GetSize();
        point.x = size.x / 2;
        point.y = size.y / 2;
    } else {
        point = ScreenToClient(point);
    }
    ShowContextMenu(point);
}

void ZoneImage::ShowContextMenu(const wxPoint& pos)
{
wxMenu menu;

if (osgApp->ModeSouris()==SOURIS_STD)
	{
	ImageInfoCV *imAcq=f->ImAcq();

	menu.Append(Menu_3D, _T("&3D"));
	menu.Append(Menu_Popup_Palette, _T("&Palette"), CreateMenuPalette(NULL));
	menu.Append(Menu_Popup_Zoom, _T("&Zoom"), CreateMenuZoom(NULL));
	menu.AppendCheckItem(Menu_Rectangle, _T("Stat Rectangle"));
	menu.AppendCheckItem(Menu_Coupe, _T("Section"));
	bool menuParametre=false;
	if (f->ImAcq()->PtContours())
		{
		menu.AppendCheckItem(Menu_Contour, _T("Draw Contour "));
		if (f->TracerContour())
			menu.Check(Menu_Contour, true);
		menuParametre=true;
		}
	if (f->ImAcq()->HoughLigne())
		{
		menu.AppendCheckItem(MENU_LIGNEHOUGH, _T("Hough (line) "));
		if (f->TracerLigneHough())
			menu.Check(MENU_LIGNEHOUGH, true);
		menuParametre=true;
		}
	if (f->ImAcq()->HoughLigneProba())
		{
		menu.AppendCheckItem(MENU_LIGNEPROBAHOUGH, _T("Hough (line proba.) "));
		if (f->TracerLigneProbaHough())
			menu.Check(MENU_LIGNEPROBAHOUGH, true);
		menuParametre=true;
		}
	if (f->ImAcq()->HoughCercle())
		{
		menu.AppendCheckItem(MENU_CERCLEHOUGH, _T("Hough (circle) "));
		if (f->TracerCercleHough())
			menu.Check(MENU_CERCLEHOUGH, true);
		menuParametre=true;
		}
	if (f->ImAcq()->BonCoin())
		{
		menu.AppendCheckItem(MENU_BONCOIN, _T("Good features "));
		if (f->TracerBonCoin())
			menu.Check(MENU_BONCOIN, true);
		menuParametre=true;
		}
	if (osgApp->Fenetre(f->IdFenetreOp1pre())|| 		menuParametre)
		{
		menu.AppendCheckItem(Menu_ParAlg, _T("Algo. Parameters"));
		}
	menu.AppendCheckItem(SEQ_OPE, _T("Sequenceoperation"));
	if(f->Cam() && f->Cam()->IsRunning() && f->SeqOp()->size()!=0)
		{
		menu.Append(STOP_SEQ, _T("&Stop Sequence"));
		}
	if(imAcq->depth()==CV_32F && imAcq->channels()%2==0)
		{
		menu.AppendSeparator();
		menu.Append(Menu_Popup_Palette, _T("&Complex"), CreateMenuComplex(NULL));
		}
	menu.AppendSeparator();
	menu.Append(ENREGISTRER_FICHIER, _T("Save"));
	menu.Append(ENREGISTRERSOUS_FICHIER, _T("Save As"));
	menu.Append(CREER_RAPPORT, _T("Create a report"));
	menu.Append(QUITTER_, _T("Close"));
	if (ModeRectangle())
		menu.Check(Menu_Rectangle, true);
	if (ModeCoupe())
		menu.Check(Menu_Coupe, true);
/*	if (f->mmodeFiltre)
		menu.Check(Menu_FilMax, true);*/
	}
else
	{
	if (osgApp->Op1()!=NULL)
		{
		wxString s=osgApp->Fenetre(osgApp->IndOp1())->GetTitle();
		menu.AppendCheckItem(MENU_OP1, "A = "+s);
		}
	else
		menu.AppendCheckItem(MENU_OP1, "Image as A");
	if (osgApp->OpBinaire() || osgApp->OpNaire())
		{
		if (osgApp->Op2()!=NULL)
			{
			wxString s=osgApp->Fenetre(osgApp->IndOp2())->GetTitle();
			menu.AppendCheckItem(MENU_OP2, "B ="+s);
			}
		else
			menu.AppendCheckItem(MENU_OP2, "Image as B");
		}
	if (osgApp->OpNaire())
		{
		if (osgApp->Op3()!=NULL)
			{
			wxString s=osgApp->Fenetre(osgApp->IndOp3())->GetTitle();
			menu.AppendCheckItem(MENU_OP3, "C ="+s);
			}
		else
			menu.AppendCheckItem(MENU_OP3, "Image as C");
		}
	menu.AppendSeparator();
	menu.AppendCheckItem(MENU_EXEC_OP, "Execute "+osgApp->NomOperation());
	menu.Append(RESET_OP,  "Operation canceled");
	if (osgApp->Op1()==f->ImAcq())
		menu.Check(MENU_OP1, true);
	if ((osgApp->OpNaire() || osgApp->OpBinaire()) && osgApp->Op2()==f->ImAcq())
		menu.Check(MENU_OP2, true);
	if (osgApp->OpNaire() && osgApp->Op3()==f->ImAcq())
		menu.Check(MENU_OP3, true);

	}


PopupMenu(&menu, pos.x, pos.y);

    // test for destroying items in popup menus
#if 0 // doesn't work in wxGTK!
    menu.Destroy(Menu_Popup_Submenu);

    PopupMenu( &menu, event.GetX(), event.GetY() );
#endif // 0
}

void ZoneImage::Vue3D(wxCommandEvent& event)
{
wxSize sfen(530,570);
if (f3D)
	{
	wxMessageBox(_("3D view already opened"));
	return;
	}
Fenetre3D *g=new Fenetre3D(NULL,_("3D"),wxPoint(0,0), sfen);
f3D=g;
g->DeffParent(f);

int *attributes = new int[7];
attributes[0] = int(WX_GL_DOUBLEBUFFER);
attributes[1] = WX_GL_RGBA;
attributes[2] = WX_GL_DEPTH_SIZE;
attributes[3] = 8;
attributes[4] = WX_GL_STENCIL_SIZE;
attributes[5] = 8;
attributes[6] = 0;


FenetreGraphiqueWX *fgOSGWX = new FenetreGraphiqueWX(g, wxID_ANY, wxDefaultPosition,
                                           sfen, wxSUNKEN_BORDER, _("osgviewerWX"), attributes);
fgOSGWX->DefOSGApp(((FenetrePrincipale*)f)->OSGApp());
fgOSGWX->DeffParent((void*)g);
g->DeffParent(f);

GraphicsOSGWX* gw = new GraphicsOSGWX(fgOSGWX);

fgOSGWX->SetGraphicsWindow(gw);

g->InstallGraphiquePhase1(fgOSGWX,gw);
// construct the viewer.
	g->InstallGraphiquePhase2(NULL);
/*fgOSGWX->Show(true);
    wxGLContext *glContexte =new wxGLContext(fgOSGWX);
    fgOSGWX->SetCurrent(*glContexte);
	GLubyte *verGl=(GLubyte*)glGetString(GL_VERSION);
	HGLRC WINAPI wwww=wglGetCurrentContext();*/
//fgOSGWX->Show(true);


g->Show(true);
}


void ZoneImage::ModeComplexe(wxCommandEvent& event)
{
modeComplexe=event.GetId()-M_MODULE_;

f->MAJNouvelleImage();
delete bitmapAffiche;
bitmapAffiche =  NULL;
wxRect r=GetClientRect();
RefreshRect (r, false);
Update();
return;
}



void ZoneImage::SelectPalette(wxCommandEvent& event)
{

f->SelectPalette(event);

f->MAJNouvelleImage();
delete bitmapAffiche;
bitmapAffiche =  NULL;
wxRect r=GetClientRect();
RefreshRect (r, false);
Update();
osgApp->ImgStat()->DrawPaletteActuelle();
osgApp->ImgStat()->DrawPalette();
return;
}

void ZoneImage::MAJZoom(wxCommandEvent& event)
{
f->MAJZoom(event);
delete bitmapAffiche;
bitmapAffiche =  NULL;
wxRect r=GetClientRect();
RefreshRect (r, false);
Update();
}

void ZoneImage::SequenceOperation(wxCommandEvent& event)
{
if (osgApp->FenetreSeqOpe()==NULL)
	{
	if (osgApp->TabSeqOperation()->size()==0)
		wxMessageBox(_("No sequence available"));
	else
		{
		wxString s(_("Sequence on ")+f->GetTitle());
		FenetreSequenceOperation *fs=new FenetreSequenceOperation(f,s,wxPoint(530,0), wxSize(430,570),this->osgApp);
		osgApp->FenetreSeqOpe(fs);
		}
	}
if (osgApp->FenetreSeqOpe())
	((FenetreSequenceOperation *)osgApp->FenetreSeqOpe())->Show(true);
}


void ZoneImage::RazSeqOp(wxCommandEvent& event)
{
if (f) f->RazSeqOp();
}

void FenetrePrincipale::TracerContour(wxCommandEvent& event)
{
tracerContour=!tracerContour;
if( tracerContour)
	{
	wxClientDC hdc(feuille);
	feuille->DoPrepareDC(hdc);
	TracerContour(hdc);
	}
else
	feuille->Refresh(true);
}

void FenetrePrincipale::TracerLigneHough(wxCommandEvent& event)
{
tracerLigneHough=!tracerLigneHough;
if( tracerLigneHough)
	{
	wxClientDC hdc(feuille);
	feuille->DoPrepareDC(hdc);
	TracerLigneHough(hdc);
	}
else
	feuille->Refresh(true);
}

void FenetrePrincipale::TracerLigneProbaHough(wxCommandEvent& event)
{
tracerLigneProbaHough=!tracerLigneProbaHough;
if( tracerLigneProbaHough)
	{
	wxClientDC hdc(feuille);
	feuille->DoPrepareDC(hdc);
	TracerLigneProbaHough(hdc);
	}
else
	feuille->Refresh(true);
}

void FenetrePrincipale::TracerCercleHough(wxCommandEvent& event)
{
tracerCercleHough=!tracerCercleHough;
if( tracerCercleHough)
	{
	wxClientDC hdc(feuille);
	feuille->DoPrepareDC(hdc);
	TracerCercleHough(hdc);
	}
else
	feuille->Refresh(true);
}

void FenetrePrincipale::TracerBonCoin(wxCommandEvent& event)
{
tracerBonCoin=!tracerBonCoin;
if( tracerBonCoin)
	{
	wxClientDC hdc(feuille);
	feuille->DoPrepareDC(hdc);
	TracerBonCoin(hdc);
	}
else
	feuille->Refresh(true);
}

void FenetrePrincipale::TracerLigneHough(wxDC &hdc)
{
if (!tracerLigneHough || !imAcq)
	return;
if (!imAcq->HoughLigne())
	{
	tracerLigneHough=false;
	return;
	}

std::vector<cv::Vec2f> *ligne=imAcq->HoughLigne();
wxPen crayon[3]={wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
for (int k=0;k<imAcq->channels()&& k<3;k++)
	{
	crayon[k].SetWidth(3);
	hdc.SetPen(crayon[k]);
	for( size_t i = 0; i < ligne[k].size(); i++ )
		{
		float rho = ligne[k][i][0], theta = ligne[k][i][1];
		wxPoint pt1, pt2;
		double a = cos(theta), b = sin(theta);
		double x0 = a*rho, y0 = b*rho;
		pt1.x = cvRound(x0 + 1000*(-b));
		pt1.y = cvRound(y0 + 1000*(a));
		pt2.x = cvRound(x0 - 1000*(-b));
		pt2.y = cvRound(y0 - 1000*(a));
		wxPoint p1(RepereImageEcran(pt1));
		wxPoint p2(RepereImageEcran(pt2));
		hdc.DrawLine(p1,p2);
		}
	}
}

void FenetrePrincipale::TracerLigneProbaHough(wxDC &hdc)
{
if (!tracerLigneProbaHough || !imAcq)
	return;
if (!imAcq->HoughLigneProba())
	{
	tracerLigneProbaHough=false;
	return;
	}
std::vector<cv::Vec4i> *lignep=imAcq->HoughLigneProba();
wxPen crayon[3]={wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
for (int k=0;k<imAcq->channels()&& k<3;k++)
	{
	crayon[k].SetWidth(3);
	hdc.SetPen(crayon[k]);
	for( size_t i = 0; i < lignep[k].size(); i++ )
		{
		wxPoint pt1(lignep[k][i][0], lignep[k][i][1]),pt2(lignep[k][i][2], lignep[k][i][3]);
		wxPoint p1(RepereImageEcran(pt1));
		wxPoint p2(RepereImageEcran(pt2));
		hdc.DrawLine(p1,p2);
		}
	}
}

void FenetrePrincipale::TracerBonCoin(wxDC &hdc)
{
if (!tracerBonCoin || !imAcq)
	return;
if (!imAcq->BonCoin())
	{
	tracerBonCoin=false;
	return;
	}
std::vector<cv::Point2f> *boncoin=imAcq->BonCoin();
wxPen crayon[3]={wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
for (int k=0;k<imAcq->channels()&& k<3;k++)
	{
	crayon[k].SetWidth(2);
	hdc.SetPen(crayon[k]);
	hdc.SetBrush(*wxTRANSPARENT_BRUSH);
	for( int i = 0; i < boncoin[k].size(); i++ )
		{ 
		wxPoint p_1(boncoin[k][i].x,boncoin[k][i].y);
		wxPoint p1(RepereImageEcran(p_1));
		hdc.DrawCircle(p1,5);
		}
	}
if (imAcq->CoinRef())
	{
	std::vector<cv::Point2f> *boncoin=imAcq->CoinRef();
	for (int k=0;k<imAcq->channels()&& k<3;k++)
		{
		crayon[k].SetWidth(2);
		hdc.SetPen(crayon[(k+1)%3]);
		hdc.SetBrush(*wxTRANSPARENT_BRUSH);
		for( int i = 0; i < boncoin[k].size(); i++ )
			if (boncoin[k][i].x>=0 && boncoin[k][i].y>=0)
				{ 
				wxPoint p_1(boncoin[k][i].x,boncoin[k][i].y);
				wxPoint p1(RepereImageEcran(p_1));
				hdc.DrawRoundedRectangle(p1-wxSize(5,5),wxSize(10,10),2);
				}
		}
	}
}

void FenetrePrincipale::TracerCercleHough(wxDC &hdc)
{
if (!tracerCercleHough || !imAcq)
	return;
if (!imAcq->HoughCercle())
	{
	tracerCercleHough=false;
	return;
	}
std::vector<cv::Vec3f> *cercle=imAcq->HoughCercle();
wxPen crayon[3]={wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);
for (int k=0;k<imAcq->channels()&& k<3;k++)
	{
	crayon[k].SetWidth(2);
	hdc.SetPen(crayon[k]);
	hdc.SetBrush(*wxTRANSPARENT_BRUSH);
	for( int i = 0; i < cercle[k].size(); i++ )
		{ 
		wxPoint p_1(cercle[k][i][0],cercle[k][i][1]);
		wxPoint p1(RepereImageEcran(p_1));
		hdc.DrawCircle(p1,(fZoomNume*cercle[k][i][2])/fZoomDeno);
		}
	}
}

void FenetrePrincipale::TracerContour(wxDC &hdc)
{
if (!tracerContour || !imAcq)
	return;
if (!imAcq->PtContours())
	{
	tracerContour=false;
	return;
	}
std::vector<std::vector<cv::Point> > *ptCtr=imAcq->PtContours();
std::vector<cv::Vec4i> *arbre=imAcq->ArboContour();
wxPen crayon[3]={*wxBLACK_PEN,*wxBLACK_PEN,*wxBLACK_PEN};
for (int i=0;i<imAcq->channels()&& i<3;i++)
	{
	crayon[i].SetWidth(3);
	hdc.SetPen(crayon[i]);
	int nbContour=ptCtr[i].size();
	for (int j=0;j<nbContour;j++)
		for (int k=1;k<ptCtr[i][j].size();k++)
		{
		wxPoint p_1(ptCtr[i][j][k-1].x,ptCtr[i][j][k-1].y),p_2(ptCtr[i][j][k].x,ptCtr[i][j][k].y);

		wxPoint p1(RepereImageEcran(p_1));
		wxPoint p2(RepereImageEcran(p_2));
		hdc.DrawLine(p1,p2);
		}
	}


}
