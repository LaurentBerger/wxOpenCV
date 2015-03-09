#include "FenetrePrincipale.h"
#include "wx/wxprec.h"

#include "imagestat.h"
#include "Tableur.h"
#include "courbeplplot.h"

BEGIN_EVENT_TABLE(ImageStatistiques, wxFrame)
    EVT_CLOSE(ImageStatistiques::OnClose)
    EVT_SCROLL(ImageStatistiques::OnSlider) 
	EVT_ACTIVATE(ImageStatistiques::OnActivate)
	EVT_PAINT(ImageStatistiques::OnPaint)
	EVT_KEY_DOWN(ImageStatistiques::OnKeyDown)
    EVT_NOTEBOOK_PAGE_CHANGED(wxID_ANY, ImageStatistiques::OnNotebook)
    EVT_NOTEBOOK_PAGE_CHANGING(wxID_ANY, ImageStatistiques::OnNotebook)
    EVT_SPINCTRL(100, ImageStatistiques::OnSpinCtrl)
    EVT_TEXT_ENTER(ID_VAL_LUM_ROUGE, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_LUM_VERT, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_LUM_BLEU, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_LUM_CANAL4, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_LUM_CANAL5, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_GAIN_ROUGE, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_GAIN_VERT, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_GAIN_BLEU, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_GAIN_CANAL4, ImageStatistiques::OnTextValider) 
    EVT_TEXT_ENTER(ID_VAL_GAIN_CANAL5, ImageStatistiques::OnTextValider) 

//    EVT_GRID_SELECT_CELL(ImageStatistiques::OnSelectCell )
//    EVT_GRID_RANGE_SELECT( ImageStatistiques::OnRangeSelected )
END_EVENT_TABLE()



using namespace std;

void ImageStatistiques::OnKeyDown(wxKeyEvent &event)
{
event.Skip();
}


ImageStatistiques::~ImageStatistiques()
{
}



ImageStatistiques::ImageStatistiques(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
SetIcon(wxIcon(_T("seec64.bmp"),wxBITMAP_TYPE_ICO ));
panneau = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxDefaultSize,
        wxTAB_TRAVERSAL  | wxNO_BORDER | wxNO_FULL_REPAINT_ON_RESIZE);

m_sizerFrame = new wxBoxSizer(wxVERTICAL);

//Centre(wxBOTH);
listeFenetreOnglet = new wxNotebook(panneau, -1,wxDefaultPosition, wxDefaultSize,wxNB_TOP);
/*Tableur *infoSequence = new Tableur((wxFrame*)listeFenetreOnglet,200,15); 
listeFenetreOnglet->AddPage(infoSequence, "Cloud Image");            */
panneau->SetSizer(m_sizerFrame);

//m_sizerFrame->Fit(this);
//m_sizerFrame->SetSizeHints(this);
m_sizerFrame->Insert(0, listeFenetreOnglet, 5, wxEXPAND | wxALL, 4);                 
m_sizerFrame->Hide(listeFenetreOnglet);                                                   
m_sizerFrame->Show(listeFenetreOnglet);                                                                           
ongletHistogramme=NULL;
ongletCoupe=NULL;
ongletFocus=NULL;
ongletRegionR=NULL;
ongletRegionV=NULL;
ongletRegionB=NULL;
ongletCurseur=NULL;
ongletCouleur=NULL;
ongletDistribRadiale=NULL; /*!< Pointeur pour accès informations sur distrib radiale */
ongletDistribAngulaire=NULL; /*!< Pointeur pour accès informations sur distrib radiale */

/*wxWindow *log = new wxWindow(listeFenetreOnglet,-1); 

wxTextCtrl *item = new wxTextCtrl( log, 2679, wxEmptyString, wxDefaultPosition, wxSize(800,500), wxTE_MULTILINE|wxHSCROLL|wxTE_RICH  );
wxLogTextCtrl *logWindow = new wxLogTextCtrl(item);
listeFenetreOnglet->AddPage((wxWindow *)log, _T("History Message"));
wxLog::SetVerbose(true);
delete wxLog::SetActiveTarget(logWindow);*/


}

void ImageStatistiques::OnActivate(wxActivateEvent& event)
{
if (!event.GetActive())
	return;
int i=listeFenetreOnglet->GetSelection();
if (i==4)
	{
	DrawPaletteActuelle();
	}
if (i==1)
	{
	if (fenMere->Cam() &&  fenMere->Cam()->IsRunning())
		{
		wxCriticalSectionLocker enter(((FenetrePrincipale*)fenMere)->travailCam);
		ongletHistogramme->Plot(true);
		}
	else if (ongletHistogramme)
		ongletHistogramme->Plot(true);
	}
}


void ImageStatistiques::OnPaint(wxPaintEvent& event)
{
int i=listeFenetreOnglet->GetSelection();
if (i==4)
	{
	DrawPaletteActuelle();
	}
if (i==1)
	{
	ongletHistogramme->Plot(true);
	}
}

void ImageStatistiques::OuvertureOngletCouleur()
{
wxPoint	position[]={
// Texte		Réglette
wxPoint(10,10),wxPoint(70,10),wxPoint(170,10),
wxPoint(270,10),wxPoint(330,10)};
wxSize	taille[]={
// Texte		Réglette
wxSize(60,20),wxSize(100,40),wxSize(100,20),wxSize(60,20),
wxSize(50,30)};
long style=wxSL_HORIZONTAL ; 

wxString	legende[]={_T("Color Min"),_T("Color Max"),_T("0")};
ongletCouleur = new wxWindow(listeFenetreOnglet,-1); 
slNivMin = new wxSlider*[fenMere->ImAcq()->channels()];
slGain = new wxSlider*[fenMere->ImAcq()->channels()];
for (int	i=0,j=0;j<fenMere->ImAcq()->channels();j++)
	{
	wxString s;
	s.Printf("%.0lf",fenMere->SeuilNivBas(j));
	new wxTextCtrl(ongletCouleur,ID_VAL_LUM_ROUGE+j,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
	position[i].y+=taille[1].y;
	i++;
	slNivMin[j]=new wxSlider(ongletCouleur,ID_ASC_LUM_ROUGE+j,fenMere->SeuilNivBas(j), -16383,16383 ,position[i], taille[i],style);
	slNivMin[j]->SetForegroundColour(wxColour(255,0,0));
	position[i].y+=taille[1].y;
	i++;
	slGain[j]=new wxSlider(ongletCouleur,ID_ASC_GAIN_ROUGE+j,fenMere->CoeffCanal(j)*100, -fenMere->CoeffCanal(j)*1000,fenMere->CoeffCanal(j)*1000 ,position[i], taille[i],style);
	position[i].y+=taille[1].y;
	i++;
	s.Printf("%.1lf",fenMere->CoeffCanal(j));
	new wxTextCtrl(ongletCouleur,ID_VAL_GAIN_ROUGE+j,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
	position[i].y+=taille[1].y;
	i++;
	i=0;
	}
//spAjustAuto =  new wxSpinCtrl(ongletCouleur,100,legende[i],position[i], taille[i],wxSP_WRAP|wxSP_ARROW_KEYS ); 
listeFenetreOnglet->AddPage(ongletCouleur, _T("Color Palette"));
ongletCouleur->Refresh();
DrawPalette();
}	

void ImageStatistiques::OnClose(wxCloseEvent& event)
{
delete []slGain;
delete []slNivMin;
delete curseur;
delete excelPalette;
if (ongletFocus)
	ongletFocus->Destroy();		/*!< Pointeur pour accès informations sur Focus variance fft */
if (ongletHistogramme)
	ongletHistogramme->Destroy();	/*!< Pointeur pour accès informations sur hsitogramme image */
if (ongletDistribRadiale)
	ongletDistribRadiale->Destroy(); /*!< Pointeur pour accès informations sur distrib radiale */
if (ongletDistribAngulaire)
	ongletDistribAngulaire->Destroy(); /*!< Pointeur pour accès informations sur distrib radiale */
if (ongletCoupe)
	ongletCoupe->Destroy();	/*!< Pointeur pour accès informations sur hsitogramme image */
if (ongletRegionR)
	ongletRegionR->Destroy();
if (ongletRegionV)
	ongletRegionV->Destroy();
if (ongletRegionB)
	ongletRegionB->Destroy();
wxFrame::OnCloseWindow(event);
}


void ImageStatistiques::OnSlider(wxScrollEvent &w)
{
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
int canal;
wxString s;
wxTextCtrl *cb;

switch(w.GetId()){
case ID_ASC_LUM_ROUGE:
case ID_ASC_LUM_VERT:
case ID_ASC_LUM_BLEU:
case ID_ASC_LUM_CANAL4:
case ID_ASC_LUM_CANAL5:
	canal = w.GetId()-ID_ASC_LUM_ROUGE;	
	fenMere->DefSeuilNivBas(w.GetInt(),canal);
	cb=(wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_LUM_ROUGE+canal,this);
	
	s.Printf("%lf",fenMere->SeuilNivBas(canal) );
	cb->SetValue(s);
	break;
case ID_ASC_GAIN_ROUGE:
case ID_ASC_GAIN_VERT:
case ID_ASC_GAIN_BLEU:
case ID_ASC_GAIN_CANAL4:
case ID_ASC_GAIN_CANAL5:
	canal = w.GetId()-ID_ASC_GAIN_ROUGE;	
	fenMere->DefCoeffCanal(w.GetInt()/100.0,canal);
	cb=(wxTextCtrl*)wxWindow::FindWindowById(ID_VAL_GAIN_ROUGE+canal,this);
	
	s.Printf("%lf",fenMere->CoeffCanal(canal) );
	cb->SetValue(s);
	break;
	} 
fenMere->NouvelleImage();
fenMere->MAJNouvelleImage();
DrawPalette();
}



void ImageStatistiques::OnSpinUp(wxScrollEvent &w)
{
}
		/*!< Gestion des boutons spins */
void ImageStatistiques::OnSpinDown(wxScrollEvent &w)
{

}

void ImageStatistiques::OnTextValider(wxCommandEvent &w)
{
wxString s=((wxTextCtrl*)w.GetEventObject())->GetValue();
long	valEntiere;
double	valFlottante;	

s.ToLong(&valEntiere);
s.ToDouble(&valFlottante);
char mode =((wxOsgApp*)osgApp)->Graphique()->ModeImage();
int canal;
switch (w.GetId()){
case ID_VAL_LUM_ROUGE:
case ID_VAL_LUM_VERT:
case ID_VAL_LUM_BLEU:
case ID_VAL_LUM_CANAL4:  
case ID_VAL_LUM_CANAL5:
	{
	canal=w.GetId()-ID_VAL_LUM_ROUGE;
	fenMere->DefSeuilNivBas(valEntiere,canal);
	slNivMin[canal]->SetValue(valEntiere);
	fenMere->NouvelleImage();
	fenMere->MAJNouvelleImage();
	DrawPalette();
	}
	break;
case ID_VAL_GAIN_ROUGE:
case ID_VAL_GAIN_VERT:
case ID_VAL_GAIN_BLEU:
case ID_VAL_GAIN_CANAL4:
case ID_VAL_GAIN_CANAL5:
	{
	canal=w.GetId()-ID_VAL_GAIN_ROUGE;
	fenMere->DefCoeffCanal(valFlottante,canal);
	slGain[canal]->SetValue(valFlottante*100);
	fenMere->NouvelleImage();
	fenMere->MAJNouvelleImage();
	DrawPalette();
	}
	break;
	}
}	


void ImageStatistiques::Plot(bool b)
{
/*if (((wxOsgApp*)osgApp)->UtilisateurAbsent())
	return;*/
if (ongletHistogramme)
	if (listeFenetreOnglet->GetCurrentPage()==ongletHistogramme && listeFenetreOnglet->GetSelection()==1)
		{
		ongletHistogramme->Plot(b);
		ongletHistogramme->Refresh(true);
		}
	else
		ongletHistogramme->Plot(false);
if (ongletCoupe && listeFenetreOnglet->GetCurrentPage()==ongletCoupe)
	ongletCoupe->Plot(b);
if (ongletDistribRadiale && listeFenetreOnglet->GetCurrentPage()==ongletDistribRadiale)
	ongletDistribRadiale->Plot(b);
if (ongletDistribAngulaire && listeFenetreOnglet->GetCurrentPage()==ongletDistribAngulaire)
	ongletDistribAngulaire->Plot(b);
}


void ImageStatistiques::OuvertureOngletStatus()
{
wxPoint	position[]={
// Texte		
wxPoint(10,10),wxPoint(80,10),
wxPoint(10,40),wxPoint(80,40),
wxPoint(10,70),wxPoint(80,70),
wxPoint(10,100),wxPoint(80,100),
wxPoint(10,130),wxPoint(80,130),
wxPoint(10,160),wxPoint(80,170)};
wxSize	taille[]={
// Texte		Réglette
wxSize(70,30),wxSize(400,30),
wxSize(70,30),wxSize(400,30),
wxSize(70,30),wxSize(400,30),
wxSize(70,30),wxSize(400,30),
wxSize(70,30),wxSize(400,30),
wxSize(70,30),wxSize(400,30)};

wxString	legende[]={	"Image",wxEmptyString,
						"Size",wxEmptyString,
						"Pixel size",wxEmptyString,
						"Minimum ",wxEmptyString,
						"Maximum ",wxEmptyString,
						"Background Image ",wxEmptyString};
wxString texte;
ongletStatus = new wxWindow(listeFenetreOnglet,-1); 

int i=0;
new wxStaticText(ongletStatus,300+i,legende[i],position[i], taille[i]);
new wxStaticText(ongletStatus,300+i+1,fenMere->GetTitle(),position[i+1], taille[i+1]);
i+=2;
new wxStaticText(ongletStatus,300+i,legende[i],position[i], taille[i]);
texte.Printf("%d rows X %d cols X %d channels",fenMere->ImAcq()->rows,fenMere->ImAcq()->cols,fenMere->ImAcq()->channels());
new wxStaticText(ongletStatus,300+i+1,texte,position[i+1], taille[i+1]);
i+=2;
new wxStaticText(ongletStatus,300+i,legende[i],position[i], taille[i]);
switch(fenMere->ImAcq()->depth())
	{
	break;
case CV_16U:
	texte="unsigned short (16 bits)";
	break;
case CV_16S:
	texte=" short (16 bits)";
	break;
case CV_8U:
	texte="Unsigned char (8 bits)";
	break;
case CV_8S:
	texte="Signed char (8 bits)";
	break;
case CV_32F:
	texte="Single precision floating point (32 bits) by channel";
	break;
case CV_64F:
	texte="Single precision floating point (32 bits) by channel";
	break;
default :
	texte ="Unknown size!";
	}

new wxStaticText(ongletStatus,300+i+1,texte,position[i+1], taille[i+1]);
i+=2;
new wxStaticText(ongletStatus,300+i,legende[i],position[i], taille[i]);
wxString v;
texte="(";
for (int i=0;i<fenMere->ImAcq()->channels()-1;i++)
	{
	v.Printf("%f,",fenMere->ImAcq()->MinIm()[i]);
	texte += v;
	}
v.Printf("%f)",fenMere->ImAcq()->MinIm()[fenMere->ImAcq()->channels()-1]);
texte += v;
new wxStaticText(ongletStatus,300+i+1,texte,position[i+1], taille[i+1]);
i+=2;
new wxStaticText(ongletStatus,300+i,legende[i],position[i], taille[i]);
texte="(";
for (int i=0;i<fenMere->ImAcq()->channels()-1;i++)
	{
	v.Printf("%f,",fenMere->ImAcq()->MaxIm()[i]);
	texte += v;
	}
v.Printf("%f)",fenMere->ImAcq()->MaxIm()[fenMere->ImAcq()->channels()-1]);
texte += v;
new wxStaticText(ongletStatus,300+i+1,texte,position[i+1], taille[i+1]);
i+=2;
new wxStaticText(ongletStatus,300+i,legende[i],position[i], taille[i]);
i++;
new wxStaticText(ongletStatus,300+i,legende[i],position[i], taille[i]);
i++;
listeFenetreOnglet->AddPage(ongletStatus, _T("Informations"));
ongletStatus->Refresh();

}	

void ImageStatistiques::OnSpinCtrl(wxSpinEvent &w)
{
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
cv::Mat	*histoImage=Histogramme();
int		deb=0,fin=histoImage->rows;
if (deb==fin)
	ongletHistogramme->Plot(true);
fin=histoImage->rows-1;
while (deb<fin && histoImage->at<float>(deb)==0 )
	deb++;
while (histoImage->at<float>(fin)==0 && fin>deb)
	fin--;
int	ratio=w.GetInt();
float	nbPixels;
float	s=0;

if (fenMere->ModeRectangle())
	{
	wxRect *r=fenMere->RectangleSelec();
	nbPixels = abs(r->GetHeight()*r->GetWidth());
	}
else
	nbPixels = fenMere->NbPixelsImage();
int i=0;

while (ratio>s && deb<fin)
	if (histoImage->at<float>(deb)<histoImage->at<float>(fin))
		{
		s +=   histoImage->at<float>(deb)/nbPixels*100;
		deb++;
		}
	else
		{
		s +=   histoImage->at<float>(fin)/nbPixels*100;
		fin--;
		}
fenMere->DefSeuilNivBas(deb);
//fenMere->DefFinNivPalette(fin);

MAJValeurPalette();

/*int minCtrl,maxCtrl;
slNivMax->SetValue(fin);
minCtrl=slNivMin->GetMin();
maxCtrl=slNivMin->GetMax( );
slNivMin->SetRange(minCtrl,fin);
slNivMin->SetValue(deb);
minCtrl=slNivMax->GetMin();
maxCtrl=slNivMax->GetMax( );
slNivMax->SetRange(deb, maxCtrl);
fenMere->MAJNouvelleImage();
DrawPalette();
DrawPalette();*/

}

void ImageStatistiques::DrawPalette()
{
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
wxWindowDC hdc(ongletCouleur);
wxColor				*pCouleur=fenMere->Palette();
if(pCouleur==NULL)
	fenMere->InitPalette(65536);
pCouleur=fenMere->Palette();
if(pCouleur==NULL)
	return;
wxPen crayon;


wxRect r(60,105,512,50);
crayon.SetColour(255,255,255);
hdc.SetPen(crayon);
wxBrush	tr=*wxWHITE_BRUSH;
hdc.SetBrush(tr);

hdc.DrawRectangle(r);
tr=*wxTRANSPARENT_BRUSH;
hdc.SetBrush(tr);

int ind=0,pas=fenMere->NbCouleurPalette()/512;
int deb=fenMere->SeuilNivBas();
int fin=(fenMere->ImAcq()->MaxIm()[0]-fenMere->SeuilNivBas(0))*fenMere->CoeffCanal(0);
for (int i=0;i<512;i++,ind+=pas)
	{
	crayon.SetColour(pCouleur[ind].Red(),pCouleur[ind].Green(),pCouleur[ind].Blue());
	hdc.SetPen(crayon);
	hdc.DrawLine(r.x+i,r.y,r.x+i,r.y+r.height-22 );
	if (i>16 && i%32==1)
		{
		wxString w;
		w.Printf(_T("%d"),i*(fin-deb)/512+deb);
		hdc.DrawLine(r.x+i,r.y,r.x+i,r.y+r.height);
		crayon.SetColour(0,0,0);
		hdc.SetPen(crayon);
		hdc.DrawText(w,r.x+i,r.y+r.height-12);
		}
	}
}


void ImageStatistiques::MAJValeurPalette()
{
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
wxString ss;
ss.Printf(_T("%lf"),fenMere->SeuilNivBas() );
wxWindowList ww=(ongletCouleur->GetChildren());
((wxTextCtrl*)ww[3])->SetValue(ss);

ss.Printf(_T("%lf"),(fenMere->ImAcq()->MaxIm()[0]-fenMere->SeuilNivBas(0))*fenMere->CoeffCanal(0));
ww=(ongletCouleur->GetChildren());
((wxTextCtrl*)ww[4])->SetValue(ss);
}


void ImageStatistiques::MAJMinMaxHisto()
{

if ( !((wxOsgApp*)osgApp)->VerifFenetre() || !ongletHistogramme)
	return;
if (!ongletHistogramme)
	return;
ongletHistogramme->MajHistogramme();
cv::Mat	*histoImage=Histogramme();
int		deb=0,fin=histoImage->rows;
if (deb==fin)
	ongletHistogramme->Plot(true);
fin=histoImage->rows-1;

while ( deb<fin && histoImage->at<float>(deb)==0)
	deb++;
while (fin>deb && histoImage->at<float>(fin)==0 )
	fin--;
wxWindowList ww=(ongletCouleur->GetChildren());

int	ratio=((wxSpinCtrl*)ww[2])->GetValue();
float	nbPixels;
float	s=0;

if (fenMere->ModeRectangle())
	{
	wxRect *r=fenMere->RectangleSelec();
	nbPixels = abs(r->GetHeight()*r->GetWidth());
	}
else
	nbPixels = fenMere->NbPixelsImage();
int i=0;

while (ratio>s && deb<fin)
	if (histoImage->at<float>(deb)<histoImage->at<float>(fin))
		{
		s +=   histoImage->at<float>(deb)/nbPixels*100;
		deb++;
		}
	else
		{
		s +=   histoImage->at<float>(fin)/nbPixels*100;
		fin--;
		}
//fenMere->DefSeuilNivBas(deb);
//fenMere->DefFinNivPalette(fin);
/*int minCtrl,maxCtrl;
slNivMax->SetValue(fin);
minCtrl=slNivMin->GetMin();
maxCtrl=slNivMin->GetMax( );
slNivMin->SetRange(minCtrl,fin);
slNivMin->SetValue(deb);
minCtrl=slNivMax->GetMin();
maxCtrl=slNivMax->GetMax( );
slNivMax->SetRange(deb, maxCtrl);
DrawPalette();*/

}

void ImageStatistiques::MAJInfo(int art,char *info)
{
if (((wxOsgApp*)osgApp)->UtilisateurAbsent())
	return;
wxWindowList&w=ongletStatus->GetChildren();

wxString ss(info, wxConvUTF8);
switch(art){
case 0:
case 1:
case 2:
case 3:
case 4:
case 6:
case 8:
case 9:
case 11:

	((wxStaticText*)w[art])->SetLabel(ss);
	break;
default: 

	((wxTextCtrl*)w[art])->ChangeValue(ss);
	break;
	}
w[art]->Refresh();
}

void ImageStatistiques::MAJInfo(int art,wxString ss)
{
if (((wxOsgApp*)osgApp)->UtilisateurAbsent())
	return;
wxWindowList&w=ongletStatus->GetChildren();

switch(art){
case 0:
case 1:
case 2:
case 3:
case 4:
case 6:
case 8:
case 9:
case 11:

	((wxStaticText*)w[art])->SetLabel(ss);
	break;
default: 

	((wxTextCtrl*)w[art])->ChangeValue(ss);
	break;
	}
w[art]->Refresh();
}


void ImageStatistiques::MAJInfo(int art,int d)
{
if (((wxOsgApp*)osgApp)->UtilisateurAbsent())
	return;
wxWindowList&w=ongletStatus->GetChildren();

wxString ss;
ss.Printf(_T("%d"),d);
switch(art){
case 0:
case 1:
case 2:
case 3:
case 4:
case 6:
case 8:
	((wxStaticText*)w[art])->SetLabel(ss);
	break;
default: 
	((wxTextCtrl*)w[art])->SetLabel(ss);
	break;
	}
w[art]->Refresh();
}

wxString ImageStatistiques::LireInfo(int art)
{
wxWindowList&w=ongletStatus->GetChildren();
wxString ss;
ss= ((wxTextCtrl*)w[art])->GetValue();
w[art]->Refresh();
return ss;
}

void ImageStatistiques::OuvertureOngletRegion() 
{
if (!fenMere || !fenMere->ImAcq())
	return;
if (fenMere->ImAcq()->channels()>=3)
	{
	ongletRegionR = new FenetreRegion((wxFrame*)listeFenetreOnglet); 
	ongletRegionR->DefOsgApp(osgApp);
	ongletRegionR->DefParent(fenMere);

	listeFenetreOnglet->AddPage(ongletRegionR, _("Red Region"));
	ongletRegionR->Refresh();
	}
if (fenMere->ImAcq()->channels()>=2)
	{
	ongletRegionV = new FenetreRegion((wxFrame*)listeFenetreOnglet); 
	ongletRegionV->DefOsgApp(osgApp);
	ongletRegionV->DefParent(fenMere);

	listeFenetreOnglet->AddPage(ongletRegionV, _("Green Region"));
	ongletRegionV->Refresh();
	}
if (fenMere->ImAcq()->channels()>=1)
	{
	ongletRegionB = new FenetreRegion((wxFrame*)listeFenetreOnglet); 
	ongletRegionB->DefOsgApp(osgApp);
	ongletRegionB->DefParent(fenMere);

	listeFenetreOnglet->AddPage(ongletRegionB, _("Blue Region "));
	ongletRegionB->Refresh();
	}
}	

void ImageStatistiques::OuvertureOngletCurseur() 
{
ongletCurseur = new wxWindow((wxFrame*)listeFenetreOnglet,-1,wxPoint(0,0),wxSize(800,400)); 

curseur = new Tableur((wxFrame*)ongletCurseur,14,14); 
curseur->SetSize(wxSize(800,400));
//listeRegion->Refresh();
curseur->SetColLabelSize(wxGRID_AUTOSIZE );
for (int i=0;i<15;i++)
	curseur->SetColSize(i, 40);

listeFenetreOnglet->AddPage(ongletCurseur, _T("Region"));
ongletCurseur->Refresh();
}	

void ImageStatistiques::AfficheValCurseur(ImageInfoCV *im, int l,int c) 
{
if (!osgApp)
	return;
for (int i=-7;i<=7;i++)
	{

	if (i+l>=0 && i+l<im->rows) 
		{
		wxString	ss;
		ss.Printf(_T("%4d"),i+l);
		curseur->DefTitreLigne(i+7, ss);
		for (int j=-7;j<=7;j++)
				if (j+c>=0 && j+c<im->cols)
					{
					wxString	ss;
					ss.Printf(_T("%4d"),j+c);
					curseur->DefTitreColonne(j+7, ss);
					}
				else
					{
					wxString	ss(_T("****"));
					curseur->DefTitreColonne(j+7, ss);
					}
		for (int j=-7;j<=7;j++)
			if (j+c>=0 && j+c<im->cols)
				{
				wxString	s;
/*				if (im->LitTypePixel()&REEL_IMAGE)
					s.Printf(_T("%7.1f"),im->LitPixelReel(c+j,l+i));
				else
					s.Printf(_T("%5d"),im->LitPixelEntier(l+i,c+j));
*/				
				curseur->DefCellule(i+7,j+7,&s);
				}
			else
				{
				wxString	s(_T("***********"));
				curseur->DefCellule(i+7,j+7,&s);
				}
		}
	else
		{
		wxString	ss(_T("***"));
		curseur->DefTitreLigne(i+7, ss);
		wxString	s(_T("***********"));
		for (int j=-7;j<=7;j++)
			curseur->DefCellule(i+7,j+7,&s);
		}
	}

}

void ImageStatistiques::OuvertureOngletDistribRadiale()
{
ongletDistribRadiale = new FenetreDistribRadiale((wxFrame*)listeFenetreOnglet); 
ongletDistribRadiale->DefOsgApp(osgApp);
ongletDistribRadiale->DefFenetrePrincipale((void*)fenMere);
ongletDistribRadiale->Plot(true);
listeFenetreOnglet->AddPage(ongletDistribRadiale, _T("I(r)"));
ongletDistribRadiale->Refresh();
}	

void ImageStatistiques::OuvertureOngletDistribAngulaire()
{
ongletDistribAngulaire = new FenetreDistribAngulaire((wxFrame*)listeFenetreOnglet); 
ongletDistribAngulaire->DefOsgApp(osgApp);
ongletDistribAngulaire->DefFenetrePrincipale((void*)fenMere);
ongletDistribAngulaire->Plot(true);
listeFenetreOnglet->AddPage(ongletDistribAngulaire, _T("I(θ)"));
ongletDistribAngulaire->Refresh();
}	


void ImageStatistiques::OuvertureOngletHistogramme()
{
ongletHistogramme = new FenetreHistogramme((wxFrame*)listeFenetreOnglet); 
ongletHistogramme->DefOsgApp(osgApp);
ongletHistogramme->DefFenetrePrincipale((void*)fenMere);
ongletHistogramme->Plot();
listeFenetreOnglet->AddPage(ongletHistogramme, _T("Histogram"));
ongletHistogramme->Refresh();
}	

void ImageStatistiques::OuvertureOngletCoupe()
{
ongletCoupe = new FenetreCoupe((wxFrame*)listeFenetreOnglet); 
ongletCoupe->DefOsgApp(osgApp);
ongletCoupe->DefFenetrePrincipale((void*)fenMere);
ongletCoupe->Plot(true);
listeFenetreOnglet->AddPage(ongletCoupe, _T("Coupe"));
ongletCoupe->Refresh();
}	


void ImageStatistiques::OuvertureOngletFocus()
{
ongletFocus = new FenetreFocus((wxFrame*)listeFenetreOnglet); 
ongletFocus->DefOsgApp(osgApp);
ongletFocus->DefFenetrePrincipale((void*)fenMere);
ongletFocus->Plot(true);
listeFenetreOnglet->AddPage(ongletFocus, _T("Focus"));
ongletFocus->Refresh();
}	

void ImageStatistiques::MAJStatRec(int ind,float *t)
{
if (((wxOsgApp*)osgApp)->UtilisateurAbsent())
	return;
if (!ongletFocus || listeFenetreOnglet->GetCurrentPage()!=ongletFocus)
	return;
for (int i=0;i<5;i++)
	ongletFocus->DefCellule(ind,i, t[i],"%.0f");
ongletFocus->DefCellule(ind,5, t[1]-t[0],"%.0f");
if (fenMere->IndiceRectangleSelec()==ind)
	ongletFocus->Plot(ind);
}

void ImageStatistiques::RAZStat()
{
ongletFocus->RAZHistoStat();
}

void ImageStatistiques::MAJCoupe(wxRect &r)
{
if (((wxOsgApp*)osgApp)->UtilisateurAbsent() || !ongletHistogramme)
	return;
if (!ongletCoupe || listeFenetreOnglet->GetCurrentPage()!=ongletCoupe)
	return;

ongletCoupe->DefRect(r);
ongletCoupe->Plot(true);
}


