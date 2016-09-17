#include "FenetrePrincipale.h"
#include "FenetreInfo.h"
#include "wxOsgApp.h"
#include "imagestat.h"
#include "ControleCamera.h"
#include "Fenetre3D.h"
#include "FenetreAlgo.h"
#include "FenetreSeqOpe.h"
#include "GlisserForme.h"
#include <wx/graphics.h>


ZoneImage::ZoneImage(wxWindow *parent,wxSize w)
    : wxScrolled<wxWindow>(parent, wxID_ANY)   {
    SetScrollRate( 1, 1 );
    SetVirtualSize( w.x, w.y );
    SetBackgroundColour( *wxWHITE );

m_dragMode = TEST_DRAG_NONE;
m_draggedShape = (DragShape*) NULL;
//    m_dragImage = (wxDragImage*) NULL;
m_currentlyHighlighted = (DragShape*) NULL;
m_dragImage=NULL;
SetBackgroundStyle(wxBG_STYLE_PAINT);
modeRect=false;
modeCoupe=false;
pointCtrl=false;
facteurZoom=-1;
osgApp=NULL;
bitmapAffiche=NULL;
f3D=NULL;
for (int i=0;i<NB_MAX_RECTANGLE;i++)
	{
	rectSelect[i].SetSize(wxSize(0,0));
	rectCoupe[i].SetSize(wxSize(0,0));
    rectDsMasque[i] = false;
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
Bind(wxEVT_COMMAND_MENU_SELECTED, &ZoneImage::Vue3D, this, Menu_3D);
Bind(wxEVT_COMMAND_MENU_SELECTED, &ZoneImage::SelectPalette, this, NOIRETBLANC_, NOIRETBLANC_ + 9);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::ModeComplexe,this,M_MODULE_,PHASE_RD);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::MAJZoom,this,ZOOM1SUR8,ZOOM8SUR1);
Bind(wxEVT_COMMAND_MENU_SELECTED,&ZoneImage::SequenceOperation,this,SEQ_OPE);
Bind(wxEVT_COMMAND_MENU_SELECTED, &ZoneImage::RazSeqOp, this, STOP_SEQ);
Bind(wxEVT_COMMAND_MENU_SELECTED, &ZoneImage::MenuMasque, this, RECT_DS_MASQUE, RECT_DS_MASQUE + NB_MAX_RECTANGLE);
Bind(wxEVT_COMMAND_MENU_SELECTED, &ZoneImage::PointCtrl, this,MENU_PTCTRL);


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

void ZoneImage::PointCtrl(wxCommandEvent& event)
{
    pointCtrl=!pointCtrl;
}


void ZoneImage::MenuMasque(wxCommandEvent& event)
{
    int ind = event.GetId() - RECT_DS_MASQUE;
    
    if (ind >= 0 && ind < NB_MAX_RECTANGLE)
        rectDsMasque[ind] = !rectDsMasque[ind];
    int nb = 0;
    f->ImAcq()->MajMasque();
    for (int i = 0; i < NB_MAX_RECTANGLE;i++)
        if (rectSelect[i].GetHeight()*rectSelect[i].GetWidth()>0)
        {
                f->ImAcq()->MajMasque(rectDsMasque[i], cv::Rect(rectSelect[i].GetLeftTop().x, rectSelect[i].GetLeftTop().y, rectSelect[i].GetWidth(), rectSelect[i].GetHeight()));
                if(rectDsMasque[i]) nb++;
        }

    if (nb == 0)
        f->ImAcq()->MajMasque();
    f->NouvelleImage();
    f->MAJNouvelleImage();
}
void ZoneImage::OnPaint(wxPaintEvent &evt)
{
    wxSize size = GetClientSize();
    if (size.GetHeight()*size.GetHeight()!=0)
        {
        if (mBuffer.IsOk() == false || mBuffer.GetWidth() != size.x || mBuffer.GetHeight() != size.y)
        {
            mBuffer.Create(size);
        }
        wxBufferedPaintDC  dc(this, mBuffer);
        dc.SetBackground(*wxWHITE_BRUSH);
        dc.Clear();
        PrepareDC(dc);
	    f->DrawWindow (dc);
        }
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
if (    DragMode()!=0)
    GestionCurseurSourisGlisser(event);
if (    event.LeftUp())
    GestionCurseurSourisGlisser(event);
if (!r.Contains(point) ||  this->f!=osgApp->Graphique())
	return;
point=RepereEcranImage(point);
wxClientDC dc(this);
//f->TracerDIB(f->ImageAffichee(), dc,&point);
if (f->ZoomActif() && point.x >= 0 && point.x<imAcq->cols && point.y >= 0 && point.y<imAcq->rows)
	f->FZoom()->TracerZoom(point);

if (f->BarreEtat() && f->BarreEtat()->Curseur()  && point.x>=0 && point.x<imAcq->cols && point.y>=0 && point.y<imAcq->rows)
	{
	BarreInfo *barreEtat=f->BarreEtat();
	cv::Vec3b x;
	cv::Vec3f xx;
	cv::Vec4f xxxx;
	cv::Vec6f xxx;
	std::complex<float> zz[3];
	int val;
	double dVal;
    wxCriticalSectionLocker enter(f->travailCam);
    cv::Mat m =imAcq->getMat(cv::ACCESS_READ);

	switch(imAcq->type()){
	case CV_32FC1:
		dVal = m.at<float>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,dVal);
		break;
	case CV_64FC1:
		dVal = m.at<double>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,dVal);
		break;
	case CV_32FC3:
		xx = m.at<cv::Vec3f>(point.y, point.x);;
		barreEtat->UpdateCurseur(point.x, point.y, xx[2], xx[1], xx[0]);
		break;
	case CV_32FC4:
		xxxx = m.at<cv::Vec4f>(point.y, point.x);;
		barreEtat->UpdateCurseur(point.x, point.y, xxx[2], xxx[1], xxx[0]);
		break;
	case CV_32FC(6):
		xxx=m.at<cv::Vec6f>(point.y,point.x);
		zz[0]=std::complex<float>(xxx[0],xxx[1]);
		zz[1]=std::complex<float>(xxx[2],xxx[3]);
		zz[2]=std::complex<float>(xxx[4],xxx[5]);
		barreEtat->UpdateCurseur(point.x,point.y,zz[2],zz[1],zz[0]);
		break;
	case CV_64FC3 :
		xx=m.at<cv::Vec3d>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,xx[2],xx[1],xx[0]);
		break;
	case CV_32SC1:
		val = m.at<int>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_32SC3:
		x=m.at<cv::Vec3i>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,x[2],x[1],x[0]);
		break;
	case CV_8UC1:
		val = m.at<unsigned char>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_8UC3 :
		x=m.at<cv::Vec3b>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,x[2],x[1],x[0]);
		break;
	case CV_8UC4 :
		xxxx=m.at<cv::Vec4b>(point.y,point.x);;
		barreEtat->UpdateCurseur(point.x,point.y,xxxx[2],xxxx[1],xxxx[0]);
		break;
	case CV_16UC1 :
		val = m.at<unsigned short>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_16SC1 :
		val = m.at< short>(point.y,point.x);
		barreEtat->UpdateCurseur(point.x,point.y,val);
		break;
	case CV_16SC3 :
		xx=m.at<cv::Vec3s>(point.y,point.x);;
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
		rTmp = RepereImageEcran(rTmp);
		rTmp.Inflate(8, 8);
		RefreshRect(rTmp, false);
		rectSelect[indRect].SetRight(point.x);
		rectSelect[indRect].SetBottom(point.y);
		    dc.SetBrush( *wxTRANSPARENT_BRUSH );
			RefreshRect(RepereImageEcran(rectSelect[indRect]),false);
		Update();
		TracerRectangle(indRect,1);
		}
	}
if (modeCoupe)
	{
	if ( event.LeftIsDown()  ) // detect if the mouse is down and dragging a lasso
		{
		wxRect	rTmp=rectCoupe[indCoupe],rTmp2=RepereImageEcran(rTmp);
		f->RedresseRectangle(rTmp2);
		rTmp2.Inflate(8,8);
		RefreshRect(rTmp2, false);
		rectCoupe[indCoupe].SetRight(point.x);
		rectCoupe[indCoupe].SetBottom(point.y);

		rTmp=rectCoupe[indCoupe];
		rTmp2=RepereImageEcran(rTmp);

		rTmp2=RepereImageEcran(rTmp);
		RefreshRect(rTmp2,false);
		Update();
		wxClientDC dc(this);
		PrepareDC(dc);
		TracerLesCoupes(dc);
		}
	}

}

void ZoneImage::OnLeftButtonUp(wxMouseEvent &event)
{
if (DragMode()!=0)
    GestionCurseurSourisGlisser(event);
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
            f->ImAcq()->MajMasque();
            int nb=0;
            for (int i = 0; i < NB_MAX_RECTANGLE;i++)
                if (rectSelect[i].GetHeight()*rectSelect[i].GetWidth()>0 && rectDsMasque[i])
                {
                        f->ImAcq()->MajMasque(rectDsMasque[i], cv::Rect(rectSelect[i].GetLeftTop().x, rectSelect[i].GetLeftTop().y, rectSelect[i].GetWidth(), rectSelect[i].GetHeight()));
                        nb++;
                }

            if (nb == 0)
                f->ImAcq()->MajMasque();
            if (nb)
            {
                f->NouvelleImage();
                f->MAJNouvelleImage();

            }
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
wxCriticalSectionLocker enter(f->travailCam);

if (f->ImAcq() && point.x>=0 && point.x<f->ImAcq()->cols && point.y>=0 && point.y<f->ImAcq()->rows && f->ImAcq()->StatComposante())
	{
	if (osgApp->ImgStat())
		{
		cv::Vec3b x;
		cv::Vec3f xx;
		cv::Vec6f xxx;
		std::complex<float> zz[3];
        cv::Mat m = f->ImAcq()->getMat(cv::ACCESS_READ);

		switch(f->ImAcq()->type())
			{
			case CV_32SC1:
				x[0] = m.at<int>(point.y,point.x);
				osgApp->ImgStat()->SelectRegion(x[0],0);
				break;
			case CV_32SC3:
				x=m.at<cv::Vec3i>(point.y,point.x);
				osgApp->ImgStat()->SelectRegion(x[0],0);
				osgApp->ImgStat()->SelectRegion(x[1],1);
				osgApp->ImgStat()->SelectRegion(x[2],2);
				break;
			}
		}
	}
wxPoint ptImg=event.GetPosition(),ptEcran=RepereEcranImage(ptImg);

DragShape* shape = FindShape(ptEcran);
if (shape)
{
    DragMode(TEST_DRAG_START);
    FormeGlisser(shape);
    wxPoint pt=event.GetPosition(),ptImg=f->RepereEcranImage(pt);
    PosDebutGlisser(ptImg);
//    Unbind(wxEVT_LEFT_UP, &ZoneImage::OnLeftButtonUp,this);
}
if (event.ShiftDown()&&f->ImAcq() && point.x>=0 && point.x<f->ImAcq()->cols && point.y>=0 && point.y<f->ImAcq()->rows )
	{
/*	ShapedFrame *shapedFrame = new ShapedFrame(f,point);
	shapedFrame->Show(true);
	PointFrame *pointFrame = new PointFrame(f,point);
	pointFrame->Show(true);*/
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
    if (event.ControlDown() && !event.ShiftDown())
    {
        if (osgApp->Op1()==NULL)
	        osgApp->DefOperande1(f->ImAcq(),f->IdFenetre());
        else if (osgApp->Op2()==NULL)
	        osgApp->DefOperande2(f->ImAcq(),f->IdFenetre());
        else 
            osgApp->DefOperandeN(f->ImAcq(),f->IdFenetre());
    }
    if (event.ControlDown() && event.ShiftDown())
    {
	    osgApp->SupOperandeN(f->ImAcq(),f->IdFenetre());
    }
}

void ZoneImage::TracerRectangle(int ind,bool croix)
{
wxClientDC dc(this);
wxPoint pTmp=rectSelect[indRect].GetTopLeft();
wxPoint p1(RepereImageEcran(pTmp));
ClientToScreen(&p1.x,&p1.y);
wxBrush	tr=*wxTRANSPARENT_BRUSH;
dc.SetBrush(tr);
			dc.SetLogicalFunction(wxXOR);
			dc.SetPen(wxPen(*wxWHITE, 3));
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
			hdc.SetLogicalFunction(wxXOR);
			hdc.SetPen(wxPen(*wxWHITE, 3));
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
    menu->Append(ZOOM1SUR8, _T("&Zoom 1/8\tCtrl-F1"));
    menu->Append(ZOOM1SUR4, _T("&Zoom 1/4\tCtrl-F1"));
    menu->Append(ZOOM1SUR2, _T("&Zoom 1/2\tCtrl-F1"));
    menu->Append(ZOOM1SUR1, _T("&Zoom x1\tCtrl-F2"));
    menu->Append(ZOOM2SUR1, _T("&Zoom x2\tCtrl-F2"));
    menu->Append(ZOOM4SUR1, _T("&Zoom x4\tCtrl-F2"));
    menu->Append(ZOOM8SUR1, _T("&Zoom x8\tCtrl-F2"));

    return menu;
}

wxMenu *ZoneImage::CreateMenuMasque(wxString *title)
{
    wxMenu *menu = new wxMenu;
    for (int i = 0; i < NB_MAX_RECTANGLE; i++)
    {
        wxString s;
        s.Printf("Rectangle %d", i);
        menu->AppendCheckItem(RECT_DS_MASQUE + i, s);
       if (rectDsMasque[i])
            menu->Check(RECT_DS_MASQUE + i, true);
        else
            menu->Check(RECT_DS_MASQUE + i, false);
    }

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
    menu.Append(Menu_Masque, _T("Mask"),CreateMenuMasque(NULL));
    bool menuParametre = false;
	if (f->ImAcq()->PtContoursPoly()->size()!=0)
		{
		menu.AppendCheckItem(Menu_Contour_Poly, _T("Draw Contour poly "));
		if (f->TracerContourPoly())
			menu.Check(Menu_Contour_Poly, true);
		menuParametre=true;
		}
	if (f->ImAcq()->PtContoursHull()->size()!=0)
		{
		menu.AppendCheckItem(Menu_Contour_Hull, _T("Draw Hull "));
		if (f->TracerEnveloppe())
			menu.Check(Menu_Contour_Hull, true);
		menuParametre=true;
		}
	if (f->ImAcq()->PtDefautConvexite()->size()!=0)
		{
		menu.AppendCheckItem(Menu_Defaut_Hull, _T("Draw Convecity defects "));
		if (f->TracerDefautEnveloppe())
			menu.Check(Menu_Defaut_Hull, true);
		menuParametre=true;
		}
	if (f->ImAcq()->PtContours()->size()!=0)
		{
		menu.AppendCheckItem(Menu_Contour, _T("Draw Contour "));
		if (f->TracerContour())
			menu.Check(Menu_Contour, true);
		menuParametre=true;
		}
	if (f->ImAcq()->HoughLigne()->size()!=0)
		{
		menu.AppendCheckItem(MENU_LIGNEHOUGH, _T("Hough (line) "));
		if (f->TracerLigneHough())
			menu.Check(MENU_LIGNEHOUGH, true);
		menuParametre=true;
		}
	if (f->ImAcq()->HoughLigneProba()->size()!=0)
		{
		menu.AppendCheckItem(MENU_LIGNEPROBAHOUGH, _T("Hough (line proba.) "));
		if (f->TracerLigneProbaHough()!=0)
			menu.Check(MENU_LIGNEPROBAHOUGH, true);
		menuParametre=true;
		}
	if (f->ImAcq()->HoughCercle()->size()!=0)
		{
		menu.AppendCheckItem(MENU_CERCLEHOUGH, _T("Hough (circle) "));
		if (f->TracerCercleHough())
			menu.Check(MENU_CERCLEHOUGH, true);
		menuParametre=true;
		}
    if ((f->ImAcq()->PointCle(IMAGEINFOCV_GFTT_GRAY_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_GFTT_GRAY_DES)->size() != 0)||
        (f->ImAcq()->BonCoin()->size()!=0 && (*f->ImAcq()->BonCoin())[0].size()!=0))
        {
        menu.AppendCheckItem(MENU_BONCOIN, _T("Good features "));
        if (f->TracerBonCoin())
            menu.Check(MENU_BONCOIN, true);
        menuParametre = true;
        }
	if (f->ImAcq()->FlotOptique())
	{
		menu.AppendCheckItem(MENU_FLOTOPTIQUE, _T("Optical Flow "));
		if (f->TracerFlotOptique())
			menu.Check(MENU_FLOTOPTIQUE, true);
		menuParametre = true;
	}
	if (f->ImAcq()->Angle()->size() != 0)
		{
		menu.AppendCheckItem(MENU_REGIONMVT, _T("Regions Motion"));
		if (f->TracerRegionMvt())
			menu.Check(MENU_REGIONMVT, true);
		menuParametre = true;
		}
    if (f->ImAcq()->PointCleMSER()->size() != 0)
        {
        menu.AppendCheckItem(MENU_POINTMSER, _T("MSER"));
        if (f->TracerPointMSER())
            menu.Check(MENU_POINTMSER, true);
        menuParametre = true;
        }
    if (f->ImAcq()->PointCle(IMAGEINFOCV_ORB_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_ORB_DES)->size() != 0)
        {
        menu.AppendCheckItem(MENU_POINTORB, _T("ORB"));
        if (f->TracerPointORB())
            menu.Check(MENU_POINTORB, true);
        menuParametre = true;
        }
    if (f->ImAcq()->PointCle(IMAGEINFOCV_SIFT_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_SIFT_DES)->size() != 0)
        {
        menu.AppendCheckItem(MENU_POINTSIFT, _T("SIFT"));
        if (f->TracerPointSIFT())
            menu.Check(MENU_POINTSIFT, true);
        menuParametre = true;
        }
    if (f->ImAcq()->PointCle(IMAGEINFOCV_SURF_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_SURF_DES)->size() != 0)
        {
        menu.AppendCheckItem(MENU_POINTSURF, _T("SURF"));
        if (f->TracerPointSURF())
            menu.Check(MENU_POINTSURF, true);
        menuParametre = true;
        }
    if (f->ImAcq()->PointCle(IMAGEINFOCV_BRISK_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_BRISK_DES)->size() != 0)
		{
		menu.AppendCheckItem(MENU_POINTBRISK, _T("BRISK"));
		if (f->TracerPointBRISK())
			menu.Check(MENU_POINTBRISK, true);
		menuParametre = true;
		}
	if (f->ImAcq()->PointCle(IMAGEINFOCV_BLOB_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_BLOB_DES)->size() != 0)
		{
		menu.AppendCheckItem(MENU_POINTBLOB, _T("BLOB"));
		if (f->TracerPointBLOB())
			menu.Check(MENU_POINTBLOB, true);
		menuParametre = true;
		}
	if (f->ImAcq()->PointCle(IMAGEINFOCV_AKAZE_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_AKAZE_DES)->size() != 0)
		{
		menu.AppendCheckItem(MENU_POINTAKAZE, _T("AKAZE"));
		if (f->TracerPointAKAZE())
			menu.Check(MENU_POINTAKAZE, true);
		menuParametre = true;
		}
	if (f->ImAcq()->PointCle(IMAGEINFOCV_KAZE_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_KAZE_DES)->size() != 0)
		{
		menu.AppendCheckItem(MENU_POINTKAZE, _T("KAZE"));
		if (f->TracerPointKAZE())
			menu.Check(MENU_POINTKAZE, true);
		menuParametre = true;
		}
	if (f->ImAcq()->PointCle(IMAGEINFOCV_AGAST_DES)!=NULL && f->ImAcq()->PointCle(IMAGEINFOCV_AGAST_DES)->size() != 0)
		{
		menu.AppendCheckItem(MENU_POINTAGAST, _T("AGAST"));
		if (f->TracerPointAGAST())
			menu.Check(MENU_POINTAGAST, true);
		menuParametre = true;
		}
	if (f->FenAlgo() != NULL && f->FenAlgo()->NbParamSouris()!=0)
		{
		menu.AppendCheckItem(MENU_PTCTRL, _T("Control Point"));
		if (pointCtrl)
			menu.Check(MENU_PTCTRL, true);
		menuParametre = true;
		}
    if (osgApp->Fenetre(f->IdFenetreOp1pre()) || menuParametre || f->ImAcq()->EtapeOp()>0)
    {
        menu.AppendCheckItem(Menu_ParAlg, _T("Algo. Parameters"));
    }
    if (f->ImAcq()->ParamPano()!=NULL)
    {
        menu.AppendCheckItem(Menu_ParPano, _T("Pano. results"));
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
	for (int i=0;i<osgApp->NbOperande();i++)
	{
		char tmp[2] = { char(i + 65), 0 };
		wxString op = wxString::FromUTF8(tmp);
		if (osgApp->OpId(i)!=NULL && osgApp->Fenetre(osgApp->IndOpId(i))!=NULL)
			{
			wxString s=osgApp->Fenetre(osgApp->IndOpId(i))->GetTitle();
			menu.AppendCheckItem(MENU_OP1+i, op+" = "+s);
			}
		else
			menu.AppendCheckItem(MENU_OP1+i, "Image as "+op);

		if (osgApp->OpId(i)==f->ImAcq())
			menu.Check(MENU_OP1+i, true);
	
	}
	menu.AppendSeparator();
	menu.AppendCheckItem(MENU_EXEC_OP, "Execute "+osgApp->NomOperation());
	menu.Append(RESET_OP,  "Operation canceled");

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


void FenetrePrincipale::MAJRectangle(wxCommandEvent& event)
{
feuille->ModeCoupe(false);
feuille->ModeRectangle(!feuille->ModeRectangle());
feuille->Refresh(false);
}

void FenetrePrincipale::MAJCoupe(wxCommandEvent& event)
{
feuille->ModeRectangle(false);
feuille->ModeCoupe(!feuille->ModeCoupe());
feuille->Refresh(false);
}


void FenetrePrincipale::TracerDescripteur(wxCommandEvent& evt)
{
switch (evt.GetId()){
case MENU_POINTORB:
	tracerORBPoint = !tracerORBPoint;
	break;
case MENU_POINTSIFT:
	tracerSIFTPoint = !tracerSIFTPoint;
	break;
case MENU_POINTSURF:
	tracerSURFPoint = !tracerSURFPoint;
	break;
case MENU_POINTKAZE:
    tracerKAZEPoint = !tracerKAZEPoint;
    break;
case MENU_POINTAKAZE:
    tracerAKAZEPoint = !tracerAKAZEPoint;
    break;
case MENU_POINTAGAST:
    tracerAGASTPoint = !tracerAGASTPoint;
    break;
case MENU_POINTBRISK:
    tracerBRISKPoint = !tracerBRISKPoint;
    break;
case MENU_POINTMSER:
    tracerMSERPoint = !tracerMSERPoint;
    break;
case MENU_POINTBLOB:
    tracerBLOBPoint = !tracerBLOBPoint;
    break;
	}
feuille->Refresh(false);

}

void FenetrePrincipale::TracerContour(wxCommandEvent& event)
{
tracerContour=!tracerContour;
feuille->Refresh(false);
}

void FenetrePrincipale::TracerEnveloppe(wxCommandEvent& event)
{
tracerEnveloppe=!tracerEnveloppe;
feuille->Refresh(false);
}

void FenetrePrincipale::TracerDefautEnveloppe(wxCommandEvent& event)
{
tracerDefautEnveloppe=!tracerDefautEnveloppe;
feuille->Refresh(false);
}

void FenetrePrincipale::TracerContourPoly(wxCommandEvent& event)
{
tracerContourPoly=!tracerContourPoly;
feuille->Refresh(false);
}

void FenetrePrincipale::TracerLigneHough(wxCommandEvent& event)
{
	tracerLigneHough = !tracerLigneHough;
    feuille->Refresh(false);
}

void FenetrePrincipale::TracerRegionMvt(wxCommandEvent& event)
{
	tracerRegionMvt = !tracerRegionMvt;
    feuille->Refresh(false);
}

void FenetrePrincipale::TracerLigneProbaHough(wxCommandEvent& event)
{
    tracerLigneProbaHough=!tracerLigneProbaHough;
    feuille->Refresh(false);
}

void FenetrePrincipale::TracerCercleHough(wxCommandEvent& event)
{
    tracerCercleHough=!tracerCercleHough;
    feuille->Refresh(false);
}

void FenetrePrincipale::TracerBonCoin(wxCommandEvent& event)
{
    tracerBonCoin=!tracerBonCoin;
    feuille->Refresh(false);
}

void FenetrePrincipale::TracerFlotOptique(wxCommandEvent& event)
{
    tracerFlotOptique=!tracerFlotOptique;
    feuille->Refresh(false);
}

void FenetrePrincipale::TracerRegionMvt(wxBufferedPaintDC &hdc)
{
if (!tracerRegionMvt || !imAcq)
	return;
if (imAcq->RegionMvt()->size()==0)
	{
	tracerRegionMvt = false;
	return;
	}

for (int i = 0; i<imAcq->RegionMvt()->size();i++)
	{
	wxPoint p_1((*(imAcq->RegionMvt()))[i].x, (*(imAcq->RegionMvt()))[i].y);
	wxPoint p_2((*(imAcq->RegionMvt()))[i].width, (*(imAcq->RegionMvt()))[i].height);
	wxPoint p1(RepereImageEcran(p_1));
	wxPoint p_3=p_1+p_2;
	wxPoint p2(RepereImageEcran(p_3));
	hdc.DrawLine(p1, p2);
}
}

void FenetrePrincipale::TracerLigneHough(wxBufferedPaintDC &hdc)
{
if (!tracerLigneHough || !imAcq)
	return;
if (!imAcq->HoughLigne())
	{
	tracerLigneHough=false;
	return;
	}

std::vector<std::vector<cv::Vec2f > > *ligne=imAcq->HoughLigne();
wxPen crayon[3]={wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
for (int k=0;k<imAcq->channels()&& k<3;k++)
	{
	crayon[k].SetWidth(3);
	hdc.SetPen(crayon[k]);
	for( size_t i = 0; i < ligne[k].size(); i++ )
		{
		float rho = (*ligne)[k][i][0], theta = (*ligne)[k][i][1];
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

void FenetrePrincipale::TracerLigneProbaHough(wxBufferedPaintDC &hdc)
{
if (!tracerLigneProbaHough || !imAcq)
	return;
if (imAcq->HoughLigneProba()->size()==0)
	{
	tracerLigneProbaHough=false;
	return;
	}
std::vector<std::vector<cv::Vec4i> > *lignep=imAcq->HoughLigneProba();
wxPen crayon[3]={wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
for (int k=0;k<imAcq->channels()&& k<3;k++)
	{
	crayon[k].SetWidth(3);
	hdc.SetPen(crayon[k]);
	for( size_t i = 0; i < (*lignep)[k].size(); i++ )
		{
		wxPoint pt1((*lignep)[k][i][0], (*lignep)[k][i][1]),pt2((*lignep)[k][i][2], (*lignep)[k][i][3]);
		wxPoint p1(RepereImageEcran(pt1));
		wxPoint p2(RepereImageEcran(pt2));
		hdc.DrawLine(p1,p2);
		}
	}
}

void FenetrePrincipale::TracerBonCoin(wxBufferedPaintDC &hdc)
{
if (!tracerBonCoin || !imAcq)
	return;
if (!imAcq->PointCle(IMAGEINFOCV_GFTT_GRAY_DES) && !(imAcq->BonCoin()!=NULL && (*imAcq->BonCoin())[0].size()!=0))
	{
	tracerBonCoin = false;
	return;
	}
if (imAcq->PointCle(IMAGEINFOCV_GFTT_GRAY_DES))
{
    for (int k = 0; k < imAcq->channels(); k++)
    {
        std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_GFTT_GRAY_DES+k);
        int fZoomNume, fZoomDeno;

        CalculZoom(fZoomNume, fZoomDeno);
        wxPen crayon[3] = { wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
        wxBrush brosse(wxColour(0, 128, 0, 128));
        hdc.SetPen(crayon[k]);
        hdc.SetBrush(brosse);
        for (int i = 0;pts && i < pts->size(); i++)
            {
            wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
            wxPoint p1(RepereImageEcran(p_1));
            wxPoint p(p1-wxPoint(2,2));
            wxSize w(5,5);
            hdc.DrawRectangle(p, w);
            }

    }
}
else
{
    for (int k = 0; k < imAcq->channels(); k++)
    {
        std::vector<cv::Point2f> *pts = &(*imAcq->BonCoin())[k];
        int fZoomNume, fZoomDeno;

        CalculZoom(fZoomNume, fZoomDeno);
        wxPen crayon[3] = { wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
        wxBrush brosse(wxColour(0, 128, 0, 128));
        hdc.SetPen(crayon[k]);
        hdc.SetBrush(brosse);
        for (int i = 0;pts && i < pts->size(); i++)
            {
            wxPoint p_1((*pts)[i].x, (*pts)[i].y);
            wxPoint p1(RepereImageEcran(p_1));
            wxPoint p(p1-wxPoint(2,2));
            wxSize w(5,5);
            hdc.DrawRectangle(p, w);
            }

    }
}
TracerAppariementPoint(hdc);
}

void FenetrePrincipale::TracerCercleHough(wxBufferedPaintDC &hdc)
{
if (!tracerCercleHough || !imAcq)
	return;
if (imAcq->HoughCercle()->size()==0)
	{
	tracerCercleHough=false;
	return;
	}
std::vector<std::vector<cv::Vec3f> > *cercle=imAcq->HoughCercle();
wxPen crayon[3]={wxPen(wxColour(255,255,0)),wxPen(wxColour(255,0,255)),wxPen(wxColour(0,255,255))};
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);
for (int k=0;k<imAcq->channels()&& k<3;k++)
	{
	crayon[k].SetWidth(2);
	hdc.SetPen(crayon[k]);
	hdc.SetBrush(*wxTRANSPARENT_BRUSH);
	for( int i = 0; i < (*cercle)[k].size(); i++ )
		{
		wxPoint p_1((*cercle)[k][i][0],(*cercle)[k][i][1]);
		wxPoint p1(RepereImageEcran(p_1));
		hdc.DrawCircle(p1,(fZoomNume*(*cercle)[k][i][2])/fZoomDeno);
		}
	}
}

void FenetrePrincipale::TracerContour(wxBufferedPaintDC &hdc)
{
if (!tracerContour || !imAcq)
	return;
if (imAcq->PtContours()->size()==0)
	{
	tracerContour=false;
	return;
	}
std::vector<std::vector<std::vector<cv::Point> > > *ptCtr=imAcq->PtContours();
std::vector<std::vector<cv::Vec4i> >  *arbre=imAcq->ArboContour();
wxPen crayon[3]={*wxBLACK_PEN,*wxBLACK_PEN,*wxBLACK_PEN};
for (int i=0;i<imAcq->channels()&& i<3;i++)
	{
	crayon[i].SetWidth(3);
	hdc.SetPen(crayon[i]);
	int nbContour=(*ptCtr)[i].size();
	for (int j=0;j<nbContour;j++)
		for (int k=1;k<(*ptCtr)[i][j].size();k++)
		{
		wxPoint p_1((*ptCtr)[i][j][k-1].x,(*ptCtr)[i][j][k-1].y),p_2((*ptCtr)[i][j][k].x,(*ptCtr)[i][j][k].y);

		wxPoint p1(RepereImageEcran(p_1));
		wxPoint p2(RepereImageEcran(p_2));
		hdc.DrawLine(p1,p2);
		}
	}


}

void FenetrePrincipale::TracerContourPoly(wxBufferedPaintDC &hdc)
{
if (!tracerContourPoly || !imAcq)
	return;
if (imAcq->PtContoursPoly()->size()==0)
	{
	tracerContourPoly=false;
	return;
	}
std::vector<std::vector<std::vector<cv::Point> > > *ptCtr=imAcq->PtContoursPoly();
wxPen crayon[3]={*wxBLACK_PEN,*wxBLACK_PEN,*wxBLACK_PEN};
for (int i=0;i<imAcq->channels()&& i<3;i++)
{
	crayon[i].SetWidth(5);
	hdc.SetPen(crayon[i]);
	int nbContour=(*ptCtr)[i].size();
    for (int j = 0; j < nbContour; j++)
    {
		for (int k=1;k<(*ptCtr)[i][j].size();k++)
		{
		    wxPoint p_1((*ptCtr)[i][j][k-1].x,(*ptCtr)[i][j][k-1].y),p_2((*ptCtr)[i][j][k].x,(*ptCtr)[i][j][k].y);

		    wxPoint p1(RepereImageEcran(p_1));
		    wxPoint p2(RepereImageEcran(p_2));
		    hdc.DrawLine(p1,p2);
		}
        if ((*ptCtr)[i][j].size()>=2)
        {
            wxPoint p_1((*ptCtr)[i][j][0].x,(*ptCtr)[i][j][0].y),p_2((*ptCtr)[i][j][(*ptCtr)[i][j].size()-1].x,(*ptCtr)[i][j][(*ptCtr)[i][j].size()-1].y);

		    wxPoint p1(RepereImageEcran(p_1));
		    wxPoint p2(RepereImageEcran(p_2));
		    hdc.DrawLine(p1,p2);

        }
    }
}


}

void FenetrePrincipale::TracerEnveloppe(wxBufferedPaintDC &hdc)
{
if (!tracerEnveloppe || !imAcq)
	return;
if (imAcq->PtContoursHull()->size()==0)
	{
	tracerEnveloppe=false;
	return;
	}
std::vector<std::vector<std::vector<int> > > *ptIdx=imAcq->PtContoursHull();
std::vector<std::vector<std::vector<cv::Point> > > *ptCtr=imAcq->PtContours();
wxPen crayon[3]={*wxBLACK_PEN,*wxBLACK_PEN,*wxBLACK_PEN};
for (int i=0;i<imAcq->channels()&& i<3;i++)
	{
	crayon[i].SetWidth(5);
	hdc.SetPen(crayon[i]);
	int nbContour=(*ptCtr)[i].size();
    for (int j = 0; j < nbContour; j++)
    {
		for (int k=1;k<(*ptIdx)[i][j].size();k++)
		{
            int ind0 = (*ptIdx)[i][j][k-1];
            int ind1 = (*ptIdx)[i][j][k];
		    wxPoint p_1((*ptCtr)[i][j][ind0].x,(*ptCtr)[i][j][ind0].y),p_2((*ptCtr)[i][j][ind1].x,(*ptCtr)[i][j][ind1].y);

		    wxPoint p1(RepereImageEcran(p_1));
		    wxPoint p2(RepereImageEcran(p_2));
		    hdc.DrawLine(p1,p2);
		}
        if ((*ptIdx)[i][j].size() >= 2)
        {
            int ind0 = (*ptIdx)[i][j][0];
            int ind1 = (*ptIdx)[i][j][(*ptIdx)[i][j].size()-1];
		    wxPoint p_1((*ptCtr)[i][j][ind0].x,(*ptCtr)[i][j][ind0].y),p_2((*ptCtr)[i][j][ind1].x,(*ptCtr)[i][j][ind1].y);

		    wxPoint p1(RepereImageEcran(p_1));
		    wxPoint p2(RepereImageEcran(p_2));
		    hdc.DrawLine(p1,p2);
        }
	}
    }


}


void FenetrePrincipale::TracerDefautEnveloppe(wxBufferedPaintDC &hdc)
{
if (!tracerDefautEnveloppe || !imAcq)
	return;
if (imAcq->PtDefautConvexite()->size()==0)
	{
	tracerDefautEnveloppe=false;
	return;
	}
std::vector<std::vector<std::vector<cv::Vec4i> > >* ptDft=imAcq->PtDefautConvexite();
std::vector<std::vector<std::vector<cv::Point> > > *ptCtr=imAcq->PtContours();
wxPen crayon[4]={*wxBLACK_PEN,*wxBLACK_PEN,*wxBLACK_PEN,*wxBLACK_PEN};
for (int i=0;i<imAcq->channels()&& i<3;i++)
{
	crayon[i].SetWidth(5);
	hdc.SetPen(crayon[i]);
    for (int j = 0; j < (*ptCtr)[i].size(); j++)
    {
        int nbDefaut=(*ptDft)[i][j].size();
	    for (int k=0;k<nbDefaut;k++)
	    {
            int ind0=(*ptDft)[i][j][k][0];
            int ind1=(*ptDft)[i][j][k][1];
            int ind2=(*ptDft)[i][j][k][2];
		    wxPoint p_1((*ptCtr)[i][j][ind0].x,(*ptCtr)[i][j][ind0].y),p_2((*ptCtr)[i][j][ind1].x,(*ptCtr)[i][j][ind1].y);
		    wxPoint p_3((*ptCtr)[i][j][ind2].x,(*ptCtr)[i][j][ind2].y);

		    wxPoint p1(RepereImageEcran(p_1));
		    wxPoint p2(RepereImageEcran(p_2));
		    wxPoint p3(RepereImageEcran(p_3));
		    hdc.DrawLine(p1,p3);
		    hdc.DrawLine(p3,p2);
	    }
        if (nbDefaut >= 2)
        {
		    wxPoint p_1((*ptCtr)[i][j][0].x,(*ptCtr)[i][j][0].y),p_2((*ptCtr)[i][j][nbDefaut-1].x,(*ptCtr)[i][j][nbDefaut-1].y);

		    wxPoint p1(RepereImageEcran(p_1));
		    wxPoint p2(RepereImageEcran(p_2));
		    hdc.DrawLine(p1,p2);
        }

    }
}


}


void FenetrePrincipale::TracerFlotOptique(wxBufferedPaintDC &hdc)
{
 if (!tracerFlotOptique || !imAcq)
	return;
if (!imAcq->FlotOptique())
	{
	tracerFlotOptique=false;
	return;
	}
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);
wxPen crayon[3]={*wxBLACK_PEN,*wxBLACK_PEN,*wxBLACK_PEN};
for (int i=0;i<imAcq->channels();i++)
	{
	int nbLigne=imAcq->FlotOptique()[i].rows;
	int nbColonne=imAcq->FlotOptique()[i].cols;
	int step=(16*fZoomDeno)/(fZoomNume);
	if (step==0)
	step=1;
	hdc.SetPen(crayon[i]);
    cv::Mat m = imAcq->FlotOptique()[i].getMat(cv::ACCESS_READ);

	for(int y = 0; y <nbLigne; y += step)
		for(int x = 0; x < nbColonne; x += step)
		{
			const cv::Point2f& fxy = m.at<cv::Point2f>(y, x);
			wxPoint p(x,y);
			wxPoint p1(RepereImageEcran(p));
			p +=wxPoint(fxy.x,fxy.y);
			wxPoint p2(RepereImageEcran(p));

			hdc.DrawLine(p1,p2);
			hdc.DrawCircle(p1, 2);
		}
	}
}

void FenetrePrincipale::TracerPointMSER(wxBufferedPaintDC &hdc)
{
if (!tracerMSERPoint || !imAcq)
	return;
if (!imAcq->PointCle())
	{
	tracerMSERPoint = false;
	return;
	}
int fZoomNume, fZoomDeno;
CalculZoom(fZoomNume, fZoomDeno);
wxGraphicsContext *gc = wxGraphicsContext::Create(hdc);

wxBrush fond(wxColor(0, 0, 0, wxALPHA_TRANSPARENT), wxBRUSHSTYLE_TRANSPARENT);
wxImage imgTrans(imAcq->cols, imAcq->rows);
imgTrans.InitAlpha();
unsigned char *alpha = imgTrans.GetAlpha();
memset(alpha, wxIMAGE_ALPHA_TRANSPARENT, imgTrans.GetWidth()*imgTrans.GetHeight());
std::vector<std::vector <cv::Point> >  *region = imAcq->PointCleMSER();
for (std::vector<std::vector <cv::Point> >::iterator itr = region->begin(); itr != region->end(); itr++)
    {
    for (std::vector<cv::Point>::iterator itp=itr->begin();itp!=itr->end();itp++)
    {

        wxPoint p_1(itp->x, itp->y);
        imgTrans.SetRGB(itp->x, itp->y, 255, 0, 0);
        imgTrans.SetAlpha(itp->x, itp->y,128);
        }
    }
wxPoint p(imAcq->cols , imAcq->rows );
wxPoint p1(RepereImageEcran(p));
wxBitmap bmp(imgTrans);
gc->DrawBitmap(bmp, 0, 0, p1.x, p1.y);
delete gc;

}

void FenetrePrincipale::TracerPointBRISK(wxBufferedPaintDC &hdc)
{
if (!tracerBRISKPoint || !imAcq)
	return;
if (!imAcq->PointCle(IMAGEINFOCV_BRISK_DES))
	{
	tracerBRISKPoint = false;
	return;
	}
std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_BRISK_DES);
int fZoomNume, fZoomDeno;


CalculZoom(fZoomNume, fZoomDeno);
wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
wxBrush brosse(wxColour(0, 0, 128, 128));
if (0 == 1)
    {
    brosse.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
hdc.SetBrush(brosse);
hdc.SetPen(crayon[0]);
for (int i = 0; i < pts->size(); i++)
    {
    wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
    wxPoint p1(RepereImageEcran(p_1));
    wxPoint p[3] = { p1 + wxPoint(-2, -2), p1 + wxPoint(2, -2), p1 + wxPoint(0, 4) };
    hdc.DrawCircle(p1.x, p1.y,4);
    }
}
//gc->StrokePath(path);
if (1==1)
    {
wxGraphicsContext *gc = wxGraphicsContext::Create(hdc);

CalculZoom(fZoomNume, fZoomDeno);
wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
wxBrush brosse(wxColour(0,0,128,128));
gc->SetBrush(brosse);
gc->SetPen(crayon[0]);
//wxGraphicsPath path=gc->CreatePath();
for (int i = 0; i < pts->size(); i++)
    {
    wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
    wxPoint p1(RepereImageEcran(p_1));
    wxPoint2DDouble p[3] = { p1 + wxPoint(-2, -2), p1 + wxPoint(2, -2), p1 + wxPoint(0, 4)};
    gc->DrawLines(3, p, wxODDEVEN_RULE);
    }
//gc->StrokePath(path);
delete gc;
    }
TracerAppariementPoint(hdc);
}

void FenetrePrincipale::TracerPointFREAK(wxBufferedPaintDC &hdc)
{
}

void FenetrePrincipale::TracerPointBLOB(wxBufferedPaintDC &hdc)
{
if (!tracerBLOBPoint || !imAcq)
	return;
if (!imAcq->PointCle(IMAGEINFOCV_BLOB_DES))
	{
	tracerBLOBPoint = false;
	return;
	}
std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_BLOB_DES);
int fZoomNume, fZoomDeno;

CalculZoom(fZoomNume, fZoomDeno);
wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
wxBrush brosse(wxColour(128, 0, 0, 128));
hdc.SetBrush(brosse);
hdc.SetPen(crayon[0]);
for (int i = 0; i < pts->size(); i++)
    {
    wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
    wxPoint p1(RepereImageEcran(p_1));
    hdc.DrawCircle(p1, 2);
    }
hdc.SetBrush(*wxTRANSPARENT_BRUSH);
crayon[0].SetWidth(3);
hdc.SetPen(crayon[0]);
for (int i = 0; i < pts->size(); i++)
    {
    wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
    wxPoint p1(RepereImageEcran(p_1));
    hdc.DrawCircle(p1, (*pts)[i].size);
    }

TracerAppariementPoint(hdc);
}

void FenetrePrincipale::TracerPointKAZE(wxBufferedPaintDC &hdc)
{
if (!tracerKAZEPoint || !imAcq)
	return;
if (!imAcq->PointCle(IMAGEINFOCV_KAZE_DES))
	{
	tracerKAZEPoint = false;
	return;
	}
std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_KAZE_DES);
int fZoomNume, fZoomDeno;

CalculZoom(fZoomNume, fZoomDeno);
wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
wxBrush brosse(wxColour(0, 128, 0, 128));
hdc.SetPen(crayon[0]);
hdc.SetBrush(brosse);
for (int i = 0; i < pts->size(); i++)
    {
    wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
    wxPoint p1(RepereImageEcran(p_1));
    wxPoint p(p1-wxPoint(2,2));
    wxSize w(5,5);
    hdc.DrawRectangle(p, w);
    }

TracerAppariementPoint(hdc);
   }

void FenetrePrincipale::TracerPointAKAZE(wxBufferedPaintDC &hdc)
{
if (!tracerAKAZEPoint || !imAcq)
	return;
if (!imAcq->PointCle(IMAGEINFOCV_AKAZE_DES))
	{
	tracerAKAZEPoint = false;
	return;
	}
std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_AKAZE_DES);
int fZoomNume, fZoomDeno;

CalculZoom(fZoomNume, fZoomDeno);
wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
wxBrush brosse(wxColour(0, 128, 0, 128));
hdc.SetPen(crayon[0]);
hdc.SetBrush(brosse);
for (int i = 0; i < pts->size(); i++)
    {
    wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
    wxPoint p1(RepereImageEcran(p_1));
    wxPoint p(p1-wxPoint(2,2));
    wxSize w(5,5);
    hdc.DrawRectangle(p, w);
    }

TracerAppariementPoint(hdc);
}

void FenetrePrincipale::TracerPointAGAST(wxBufferedPaintDC &hdc)
{
if (!tracerKAZEPoint || !imAcq)
	return;
if (!imAcq->PointCle(IMAGEINFOCV_AGAST_DES))
	{
	tracerKAZEPoint = false;
	return;
	}
std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_AGAST_DES);
int fZoomNume, fZoomDeno;

CalculZoom(fZoomNume, fZoomDeno);
wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
wxBrush brosse(wxColour(0, 128, 0, 128));
hdc.SetPen(crayon[0]);
hdc.SetBrush(brosse);
for (int i = 0; i < pts->size(); i++)
    {
    wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
    wxPoint p1(RepereImageEcran(p_1));
    wxPoint p(p1-wxPoint(2,2));
    wxSize w(5,5);
    hdc.DrawRectangle(p, w);
    }

TracerAppariementPoint(hdc);
   }

void FenetrePrincipale::TracerPointSIFT(wxBufferedPaintDC &hdc)
{
    if (!tracerSIFTPoint || !imAcq)
	    return;
    if (!imAcq->PointCle(IMAGEINFOCV_SIFT_DES))
	    {
	    tracerSIFTPoint = false;
	    return;
	    }
    std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_SIFT_DES);
    int fZoomNume, fZoomDeno;

    CalculZoom(fZoomNume, fZoomDeno);
    wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
    wxBrush brosse(wxColour(0, 128, 0, 128));
    hdc.SetPen(crayon[0]);
    hdc.SetBrush(brosse);
    for (int i = 0; i < pts->size(); i++)
        {
        wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
        wxPoint p1(RepereImageEcran(p_1));
        wxPoint p(p1-wxPoint(2,2));
        wxSize w(5,5);
        hdc.DrawRectangle(p, w);
        }

    TracerAppariementPoint(hdc);
}

void FenetrePrincipale::TracerPointSURF(wxBufferedPaintDC &hdc)
{
    if (!tracerSURFPoint || !imAcq)
	    return;
    if (!imAcq->PointCle(IMAGEINFOCV_SURF_DES))
	    {
	    tracerSURFPoint = false;
	    return;
	    }
    std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_SURF_DES);
    int fZoomNume, fZoomDeno;

    CalculZoom(fZoomNume, fZoomDeno);
    wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
    wxBrush brosse(wxColour(0, 128, 0, 128));
    hdc.SetPen(crayon[0]);
    hdc.SetBrush(brosse);
    for (int i = 0; i < pts->size(); i++)
        {
        wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
        wxPoint p1(RepereImageEcran(p_1));
        wxPoint p(p1-wxPoint(2,2));
        wxSize w(5,5);
        hdc.DrawRectangle(p, w);
        }

    TracerAppariementPoint(hdc);
}



void FenetrePrincipale::TracerPointORB(wxBufferedPaintDC &hdc)
{
if (!tracerORBPoint || !imAcq)
	return;
if (!imAcq->PointCle())
	{
	tracerORBPoint = false;
	return;
	}
std::vector<cv::KeyPoint> *pts = imAcq->PointCle(IMAGEINFOCV_ORB_DES);
int fZoomNume, fZoomDeno;

CalculZoom(fZoomNume, fZoomDeno);
wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
wxBrush brosse(wxColour(128, 0, 0, 128));
hdc.SetBrush(brosse);
hdc.SetPen(crayon[0]);
for (int i = 0; i < pts->size(); i++)
	{
	wxPoint p_1((*pts)[i].pt.x, (*pts)[i].pt.y);
	wxPoint p1(RepereImageEcran(p_1));
	hdc.DrawCircle(p1, 2);
	}
try
{
    TracerAppariementPoint(hdc);
}
catch (cv::Exception& e)
		{
		wxString s(e.msg);
		}
}

void FenetrePrincipale::TracerAppariementPoint(wxBufferedPaintDC &hdc)
{
if ( !imAcq)
	return;
if (!imAcq->PointCle() ||(imAcq->Appariement()==NULL || imAcq->Appariement()->size()==0))
	{
	return;
	}

std::vector<cv::KeyPoint> *pts1 = imAcq->PointCle(-1);
std::vector<cv::DMatch> *m = imAcq->Appariement();
FenetrePrincipale *f;
ParametreOperation *pOCV;

if (m->size()==0)
    return;
if (imAcq->ListeOpAttribut()->find("matchdescriptormatcher") == imAcq->ListeOpAttribut()->end())
{
    if (imAcq->ListePointCleApp()==NULL)
        return;
    std::map<int, std::vector<cv::KeyPoint> >*lApp=imAcq->ListePointCleApp();
    std::map<int, std::vector<cv::KeyPoint> >::iterator it=imAcq->ListePointCleApp()->begin();

    for (; it != lApp->end();it++)
    {
        double max_dist = 0; double min_dist = 100;
        m = imAcq->Appariement(it->first);
        wxLogWarning("Match %d ",(int) m->size());
          //-- Quick calculation of max and min distances between keypoints
        if (m!=NULL && imAcq->PointCle(it->first) != NULL && imAcq->PointCleApp(it->first) != NULL)
        {
            for( int i = 0; i < m->size(); i++ )
            { 
                double dist = (*m)[i].distance;
                if( dist < min_dist ) min_dist = dist;
                if( dist > max_dist ) max_dist = dist;
            }
            pts1 = imAcq->PointCle(it->first);
            std::vector<cv::KeyPoint> *pts2 = imAcq->PointCleApp(it->first);
            if (pts2->size()==0)
	            return;
            int fZoomNume, fZoomDeno;
            //wxPoint pos = ClientToScreen(pt);
            CalculZoom(fZoomNume, fZoomDeno);
            wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
            crayon[0].SetWidth(3);
            hdc.SetPen(crayon[0]);
            for (int i = 0; i < m->size(); i++)
                if ((*m)[i].distance<std::max(2*min_dist,(min_dist+max_dist)/3))
                    {
	                wxPoint p_1((*pts1)[(*m)[i].queryIdx].pt.x, (*pts1)[(*m)[i].queryIdx].pt.y);
	                wxPoint p1(RepereImageEcran(p_1));
	                wxPoint p_2((*pts2)[(*m)[i].trainIdx].pt.x, (*pts2)[(*m)[i].trainIdx].pt.y);
	                wxPoint p2(RepereImageEcran(p_2));

	                hdc.DrawLine(p1-wxPoint(5,0), p2+wxPoint(5,0));
	                hdc.DrawLine(p1-wxPoint(0,5), p2+wxPoint(0,5));
	                }
        }

    }
}
else
{
    pOCV = &imAcq->ListeOpAttribut()->find("matchdescriptormatcher")->second;
    if (pOCV->indOpFenetre.size()>=2)
        f = osgApp->Fenetre(pOCV->indOpFenetre[1]);
    else
        f= NULL; 
    if (!f) return;
    std::vector<cv::KeyPoint> *pts2 = f->imAcq->PointCle();
    if (pts2->size()==0)
	    return;
    int fZoomNume, fZoomDeno;
    //wxPoint pos = ClientToScreen(pt);
    CalculZoom(fZoomNume, fZoomDeno);
    wxPen crayon[3] = { *wxBLACK_PEN, *wxBLACK_PEN, *wxBLACK_PEN };
    wxScreenDC ecran;
    ecran.StartDrawingOnTop();
    for (int i = 0; i < m->size(); i++)
	    {
	    wxPoint p_1((*pts1)[(*m)[i].queryIdx].pt.x, (*pts1)[(*m)[i].queryIdx].pt.y);
	    wxPoint p1(RepereImageEcran(p_1));
	    wxPoint p_2((*pts2)[(*m)[i].trainIdx].pt.x, (*pts2)[(*m)[i].trainIdx].pt.y);
	    wxPoint p2(f->RepereImageEcran(p_2));

	    p1 = ClientToScreen(p1);
	    p2 = f->ClientToScreen(p2);
	    ecran.DrawLine(p1, p2);
	    }
    ecran.EndDrawingOnTop();
}


}
