#include "FenetrePrincipale.h"

BEGIN_EVENT_TABLE(FenetreZoom, wxFrame)
    EVT_CLOSE(FenetreZoom::OnClose)
	EVT_MENU(ZOOM1SUR2,  FenetreZoom::MAJZoom)
	EVT_MENU(ZOOM1SUR1,  FenetreZoom::MAJZoom)
	EVT_MENU(ZOOM2SUR1,  FenetreZoom::MAJZoom)
	EVT_MENU(ZOOM4SUR1,  FenetreZoom::MAJZoom)
	EVT_MENU(ZOOM8SUR1,  FenetreZoom::MAJZoom)
    EVT_CONTEXT_MENU(FenetreZoom::OnMenuContext)

END_EVENT_TABLE()


FenetreZoom::FenetreZoom(FenetrePrincipale	*f)
    : wxFrame(NULL, wxID_ANY, _T("Zoom") , wxPoint(100,100),wxSize(256,256))
{
fMere = f;
facteurZoom=2;
zoomInactif=false;
}
FenetreZoom::FenetreZoom(FenetrePrincipale	*f,wxRect &r)
    : wxFrame(NULL, wxID_ANY, _T("Zoom") , r.GetLeftTop(),r.GetSize())
{
fMere = f;
facteurZoom=2;
zoomInactif=false;
}

void FenetreZoom::OnClose(wxCloseEvent& event)
{
Show(false);
zoomInactif=true;
}

void FenetreZoom::MAJZoom(wxCommandEvent& event)
{
if(event.GetId()>=ZOOM8SUR1)
	zoomInactif=true;
else
	zoomInactif=false;
facteurZoom = event.GetId()-ZOOM1SUR1;

}

void FenetreZoom::OnMenuContext(wxContextMenuEvent& event)
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

void FenetreZoom::ShowContextMenu(const wxPoint& pos)
{
wxMenu menu;

    menu.Append(ZOOM1SUR2, _T("&Zoom 1/2\tCtrl-F1"));
    menu.Append(ZOOM1SUR1, _T("&Zoom x1\tCtrl-F2"));
    menu.Append(ZOOM2SUR1, _T("&Zoom x2\tCtrl-F2"));
    menu.Append(ZOOM4SUR1, _T("&Zoom x4\tCtrl-F2"));
    menu.Append(ZOOM8SUR1, _T("&Zoom x8\tCtrl-F2"));
    menu.Append(ZOOM8SUR1, _T("&Inactif\tCtrl-F2"));


PopupMenu(&menu, pos.x, pos.y);

    // test for destroying items in popup menus
#if 0 // doesn't work in wxGTK!
    menu.Destroy(Menu_Popup_Submenu);

    PopupMenu( &menu, event.GetX(), event.GetY() );
#endif // 0
}




void FenetreZoom::TracerZoom(wxPoint p)
{
if (zoomInactif)
	{
	if (!IsShown())

		return;
	zoomInactif =false;
	}
ImageInfoCV*	imAcq=fMere->ImAcq();
wxImage *d=fMere->ImageAffichee();
wxClientDC hdc(this);

hdc.SetBrush(*wxBLACK_BRUSH);
if (facteurZoom!=0)
	{
	if (imAcq && imAcq->cols>0)
		{
		wxRect		src(0,0,imAcq->cols,imAcq->rows);
		wxRect		dst=CalculPosRectDst(src,&p),r=GetClientSize();
		hdc.DrawRectangle(r.GetRight(),r.GetTop(),r.GetRight(),r.GetBottom());
		wxRect		rSrc(src.GetLeft(),src.GetTop(),src.GetRight()-src.GetLeft()+1,src.GetBottom()-src.GetTop()+1);
		wxImage		sousImage=d->GetSubImage(rSrc);
	//	sousImage.SetMask(0);
		wxImage		imageZoom=sousImage.Scale(dst.GetRight()-dst.GetLeft()+1,dst.GetBottom()-dst.GetTop()+1);
	//	imageZoom.SetMask(0);

		wxBitmap	b(imageZoom);
		hdc.DrawBitmap(b,0,0);
		hdc.SetPen(*wxBLACK);
		hdc.DrawRectangle(dst.GetRight(),r.GetTop(),r.GetRight(),r.GetBottom());
		hdc.DrawRectangle(r.GetLeft(),dst.GetBottom(),r.GetRight(),r.GetBottom());
		}
	}
else
	{
	wxBitmap pub(*d);
	hdc.DrawBitmap(pub,-p.x,-p.y);
	}
} 

void FenetreZoom::TracerZoom(wxRect &rSrc)
{
if (zoomInactif)
	return;
ImageInfoCV*	imAcq=fMere->ImAcq();
if (rSrc.GetHeight()<=0 || rSrc.GetWidth()<=0 || imAcq->cols<=0)
	return;
if (rSrc.GetHeight()+rSrc.y>=imAcq->rows || rSrc.GetWidth()+rSrc.x>=imAcq->cols)
	return;
wxImage *d=fMere->ImageAffichee();
wxClientDC hdc(this);

hdc.SetBrush(*wxBLACK_BRUSH);

wxRect		src=rSrc;
wxRect		r=GetClientSize();

int zoomX =r.GetWidth()/rSrc.GetWidth()+1;
int zoomY =r.GetHeight()/rSrc.GetHeight()+1;

if (zoomX<zoomY)
	zoomY=zoomX;
else
	zoomX=zoomY;
if (zoomX*zoomY==0)
	return;	

wxRect		dst(0,0,zoomX*rSrc.GetWidth(),zoomY*rSrc.GetHeight());
//SetClientSize(dst);

hdc.DrawRectangle(r.GetRight(),r.GetTop(),r.GetRight(),r.GetBottom());
wxImage		sousImage=d->GetSubImage(rSrc);
//	sousImage.SetMask(0);
wxImage		imageZoom=sousImage.Scale(dst.GetRight()-dst.GetLeft()+1,dst.GetBottom()-dst.GetTop()+1);
//	imageZoom.SetMask(0);

wxBitmap	b(imageZoom);
hdc.DrawBitmap(b,0,0);
hdc.SetPen(*wxBLACK);
hdc.DrawRectangle(dst.GetRight(),r.GetTop(),r.GetRight(),r.GetBottom());
hdc.DrawRectangle(r.GetLeft(),dst.GetBottom(),r.GetRight(),r.GetBottom());

}


void FenetreZoom::CalculZoom(int &nume,int &deno)
{
deno=1;
switch(facteurZoom){
case -1 :
	nume=1;
	deno=2;
	break;
case 0 :
	nume=1;
	break;
case 1 :
	nume=2;
	break;
case 2 :
	nume=4;
	break;
case 3 :
	nume=8;
	break;
default :
	deno=1;
	nume=1;
	}
}	


wxRect FenetreZoom::CalculPosRectDst(wxRect &src,wxPoint *p)
{
wxRect	recSrc(0,0,fMere->ImAcq()->cols,fMere->ImAcq()->rows);
wxRect	rMax;
long	origX=0,origY=0;
int		fZoomNume,fZoomDeno;
wxPoint	refPos(0,0);

if (p)
	refPos= *p;


CalculZoom(fZoomNume,fZoomDeno);

rMax=this->GetClientRect();
wxPoint	p1(0,0);
wxPoint	p2(	(rMax.GetRight()+fZoomNume-1)/fZoomDeno*fZoomDeno,
			(rMax.GetBottom()+fZoomNume-1)/fZoomDeno*fZoomDeno);

wxPoint	p1Img=RepereEcranImage(p1);
wxPoint	p2Img=RepereEcranImage(p2);
wxPoint	transLat(refPos.x-p2Img.x/2,refPos.y-p2Img.y/2);
p2Img.x += fZoomDeno-1;
p2Img.y += fZoomDeno-1;
if (p)
	{
	p1Img += transLat;
	p2Img += transLat;
	}

if (!recSrc.Contains(p1Img))
	{
	if (p1Img.x<0)
		{
		p2Img.x+= -p1Img.x; 
		p1Img.x =0;
		}
	if (p1Img.y<0)
		{
		p2Img.y+= -p1Img.y; 
		p1Img.y =0;
		}
	}
if (!recSrc.Contains(p2Img))
	{
	if (p2Img.x>=recSrc.GetRight())
		{
		p1Img.x += recSrc.GetRight()-p2Img.x-1;
		p2Img.x = recSrc.GetRight()-1;
		}
	if (p2Img.y>=recSrc.GetBottom())
		{
		p1Img.y += recSrc.GetBottom()-p2Img.y-1;
		p2Img.y = recSrc.GetBottom()-1;
		}
		
	}
	p2 = RepereImageEcran(p2Img);
	p1 = RepereImageEcran(p1Img);
wxRect dst(p1,p2);
src=wxRect(p1Img,p2Img);
	return dst;
long largeur = p2Img.x - p1Img.x;
long hauteur = p2Img.y - p1Img.y;
origX = p->x-largeur/2;
long right = p->x+largeur/2;
origY = p->y-hauteur/2;
long bottom = p->y+hauteur/2;
if (recSrc.GetRight()<right)
	{
	right= recSrc.GetRight();
	origX = right-largeur;
	}
if (recSrc.GetBottom()<bottom)
	{
	bottom= recSrc.GetBottom();
	origY = bottom-hauteur;
	}
if (recSrc.GetLeft()>origX)
	{
	origX= recSrc.GetLeft();
	right = origX+largeur;
	}
if (recSrc.GetTop()>origY)
	{
	origY= recSrc.GetTop();
	bottom = origY +hauteur;
	}
src=wxRect(origX,origY,right,bottom);
return dst;
}

wxPoint FenetreZoom::RepereEcranImage(wxPoint &p)
{
wxRect	recSrc(0,0,fMere->ImAcq()->cols,fMere->ImAcq()->rows);
wxPoint pt(0,0);
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);

pt.x = (long)((p.x)*fZoomDeno/fZoomNume);
pt.y = (long)((p.y)*fZoomDeno/fZoomNume);

return pt;
}

wxPoint FenetreZoom::RepereImageEcran(wxPoint &p)
{
wxRect	recSrc(0,0,fMere->ImAcq()->cols,fMere->ImAcq()->rows);
wxPoint pt(0,0);
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);

	pt.x = (long)((p.x-recSrc.GetLeft())*fZoomNume/fZoomDeno);
	pt.y = (long)((p.y-recSrc.GetTop())*fZoomNume/fZoomDeno);

return pt;
}

wxRect FenetreZoom::RepereImageEcran(wxRect &r)
{
wxPoint	p1Img(r.GetLeftTop()),p2Img(r.GetBottomRight());
wxPoint	p1=RepereImageEcran(p1Img);
wxPoint	p2=RepereImageEcran(p2Img);
return wxRect(p1,p2);
}
wxRect FenetreZoom::RepereEcranImage(wxRect &r)
{
wxPoint	p1(r.GetLeftTop()),p2(r.GetBottomRight());
wxPoint	p1Img=RepereEcranImage(p1);
wxPoint	p2Img=RepereEcranImage(p2);
return wxRect(p1Img,p2Img);
}
