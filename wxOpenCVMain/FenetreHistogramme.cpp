#include "FenetrePrincipale.h"
#include "courbeplplot.h"
#include "Tableur.h"
#include "imagestat.h"
#include "opencv2/opencv.hpp"
#include <vector>

using namespace cv;
using namespace std;

BEGIN_EVENT_TABLE( FenetreHistogramme, wxWindow )
	//EVT_SIZE(FenetreHistogramme::OnSize)
	EVT_KEY_DOWN(FenetreHistogramme::OnKeyDown)
	EVT_GRID_CELL_CHANGING(FenetreHistogramme::NouvelHistogramme)
END_EVENT_TABLE()

void FenetreHistogramme::OnKeyDown(wxKeyEvent &event)
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
if (!fenMere)
	return;
int id=((FenetrePrincipale *)fenMere)->IdFenetre();
((wxOpencvApp*)osgApp)->Graphique(id)->TraitementClavier(event);

event.Skip();
}

void FenetreHistogramme::OnSize(wxSizeEvent &event)
{
 //   courbe->SetSize(event.GetSize());
    panel->Layout();
}





/*! Constructor of our custom frame, where the Menu is created and a
 *  a wxPLplotwindow is inserted into the frame. We plot some easy functions
 *  just to show how it works. wxPLplotwindow takes care of all the setup
 *  for the use of PLplot library.
 */
FenetreHistogramme::FenetreHistogramme(wxFrame *w  ) : wxWindow(w,-1,wxPoint(0,0),wxDefaultSize)
{
	bgcolor=false;
osgApp=NULL;
fenMere=NULL;
  // add menu
//  SetIcon( wxIcon(graph) );

	// add the wxPLplot
	wxPoint p(0,0);
	wxSize s(1700,1700);
	x[0]=NULL;
	panel = new wxPanel( this,wxID_ANY ,wxDefaultPosition,wxSize(1000,1000)   );
	wxWindow *panel2 = new wxScrolledWindow( panel,wxID_ANY ,wxDefaultPosition,wxSize(1000,1000)   );

	excel =new Tableur((wxFrame*)panel,11,4,wxDefaultSize); 

    courbe =  new wxPLplotwindow<wxWindow>(false);
	courbe->Create(panel2,wxID_ANY,wxDefaultPosition , wxSize(400,400));
 //   courbe->SetMaxSize(s);
    if (excel)
    {
        box = new wxFlexGridSizer(2,1,1);
  //      box->SetFlexibleDirection(wxHORIZONTAL);
        box->Add( courbe,3, wxEXPAND, 0);
	    box->Add( excel, 0, wxEXPAND, 0);
         box->AddGrowableCol(0);
        box->AddGrowableRow(0);
       panel->SetSizer( box );
       box->Fit(panel);
    }
    else
    {
        /*box = new wxFlexGridSizer(1,1,0);
        box->Add( courbe, 1, wxALL | wxEXPAND, 10 );
        panel->SetSizer( box );
        box->Fit(panel);
        box->SetSizeHints(panel);
	    SetSize( 640, 500 );  // set frame size
        panel->SetSizeHints( 220, 150 );  // set minimum frame size*/

    }

        //histoImage=new long[65536];


  wxString m_title=_T("Histogram");
  //SetTitle( m_title );  
nbGraines[0]=-1;
Layout();
excel->Show();
courbe->Show();
	Plot();
}

void FenetreHistogramme::OnClose(wxCloseEvent& event) 
{
if (!fenMere)
	return;
delete courbe;
delete excel;
delete panel;
courbe = NULL;
excel = NULL;
panel = NULL;
if (x[0])
	{
	for (int i=0;i<NB_MAX_CANAUX;i++)
		{
		delete []x[i];
		delete []y[i];
		delete []yFiltre[i];  /*!< courbe des histogrammes pour les trois plans */
		}
	}
wxWindow::Close(true);
}


/*! destructeur
 */
FenetreHistogramme::~FenetreHistogramme() 
{
if (!fenMere)
	return;
if (!courbe)
	return;
delete courbe;
delete excel;
delete panel;
courbe = NULL;
excel = NULL;
panel = NULL;
if (x[0])
	{
		for (int i = 0; i<NB_MAX_CANAUX; i++)
		{
			delete[]x[i];
			delete[]y[i];
			delete[]yFiltre[i];  /*!< courbe des histogrammes pour les trois plans */
		}
	}
}

void FenetreHistogramme::NouvelHistogramme(wxGridEvent& event)
{
int l=event.GetRow(),c=event.GetCol();
ImageInfoCV			*imAcq=NULL;

if (!fenMere)
	return;
int id=((FenetrePrincipale *)fenMere)->IdFenetre();
CameraVirtuelle *cam=((FenetrePrincipale *)fenMere)->Cam();
//wxCriticalSectionLocker enter(((FenetrePrincipale*)fenMere)->travailCam);

if (((FenetrePrincipale *)fenMere)->ModeImage()!=1)
	imAcq =((FenetrePrincipale *)fenMere)->ImAcq();
double v;
switch (l){
case 0 :
	if (imAcq && c>=0&& c<imAcq->channels())
		event.GetString().ToLong((long*)&nbGraines[c]);
	break;
case 1 :
	if (imAcq && c>=0&& c<imAcq->channels()&& excel)
		{
		event.GetString().ToDouble(&v);
		if( v<maxHisto[c])
			minHisto[c]=v;
		else
			{
			excel->DefCellule(l,c,minHisto[c]);
			}
		}
	break;
case 2 :
	if (imAcq && c>=0&& c<imAcq->channels()&& excel)
		{
		event.GetString().ToDouble(&v);
		if( v>minHisto[c])
			maxHisto[c]=v;
		else
			excel->DefCellule(l,c,maxHisto[c]);
		}
	break;
	}
Plot(1);
}



/*!< Calcule de l'histogrammes de l'image en tenant compte du nombre de couleur.
*/

void FenetreHistogramme::MajHistogramme()
{
ImageInfoCV			*imAcq;
if (!fenMere)
	return;
int id=((FenetrePrincipale *)fenMere)->IdFenetre();
CameraVirtuelle *cam=((FenetrePrincipale *)fenMere)->Cam();
if (((FenetrePrincipale *)fenMere)->ModeImage()!=1)
	imAcq =((FenetrePrincipale *)fenMere)->ImAcq();
if (imAcq==NULL)
	return;
if (nbGraines[0]==-1)
	{

	if ((*(imAcq->MinIm())).size()==0)
		imAcq->ExtremumLoc();
	switch(imAcq->depth())
	{
	case CV_32S:
		for (int i=0;i< (*(imAcq->MinIm())).size() && i<NB_MAX_CANAUX;i++)
			{
			minHisto[i]=(*(imAcq->MinIm()))[i];
			maxHisto[i]=(*(imAcq->MaxIm()))[i]; 
			nbGraines[i]=maxHisto[i]-minHisto[i]+1;
			}

		break;
	case CV_32F:
		for (int i=0;i<(*(imAcq->MinIm())).size() && i<NB_MAX_CANAUX;i++)
			{
			nbGraines[i]=256;
			minHisto[i]=(*(imAcq->MinIm()))[i];
			maxHisto[i]=(*(imAcq->MaxIm()))[i]; 
			}

		break;
	case CV_16U:
		for (int i=0;i<(*(imAcq->MinIm())).size() && i<NB_MAX_CANAUX;i++)
			{
			nbGraines[i]=65536;
			minHisto[i]=(*(imAcq->MinIm()))[i];
			maxHisto[i]=(*(imAcq->MaxIm()))[i]; 
			}
		break;
	case CV_16S:
		for (int i=0;i<(*(imAcq->MinIm())).size() && i<NB_MAX_CANAUX;i++)
			{
			nbGraines[i]=65536;
			minHisto[i]=(*(imAcq->MinIm()))[i];
			maxHisto[i]=(*(imAcq->MaxIm()))[i]; 
			}
		break;
	case CV_8U:
		for (int i=0;i<(*(imAcq->MinIm())).size() && i<NB_MAX_CANAUX;i++)
			{
			nbGraines[i]=256;
			minHisto[i]=0;
			maxHisto[i]=255; 
			}
		break;
	case CV_8S:
		for (int i=0;i<(*(imAcq->MinIm())).size() && i<NB_MAX_CANAUX;i++)
			{
			nbGraines[i]=256;
			minHisto[i]=-128;
			maxHisto[i]=127; 
			}
		break;
		}
	}

long nbLig=imAcq->rows,nbCol=imAcq->cols;
long ligDeb=0,colDeb=0;

if (((wxOpencvApp*)osgApp)->Graphique(id)->ModeRectangle())
	{
	wxRect	*r=((wxOpencvApp*)osgApp)->Graphique(id)->RectangleSelec();
	if (r->GetWidth()<0)
		{
		colDeb=r->GetRight();
		nbCol=-r->GetWidth();
		}
	else
		{
		colDeb=r->GetLeft();
		nbCol=r->GetWidth();
		}
	if (r->GetHeight()<0)
		{
		ligDeb=r->GetBottom();
		nbLig=-r->GetHeight();
		}
	else	
		{
		ligDeb=r->GetTop();
		nbLig=r->GetHeight();
		}
	if (colDeb+nbCol>=imAcq->cols)
		nbCol = imAcq->cols-1-colDeb;
	if (ligDeb+nbLig>=imAcq->rows)
		nbLig = imAcq->rows-1-ligDeb;
		
	}
cv::Rect const zoneTraitee(colDeb, ligDeb, nbCol, nbLig);
if (ligDeb<0  || colDeb<0 || nbLig<0 || nbCol<0 || imAcq->size().height * imAcq->size().width==0)
	{
	return;
	}
UMat masque;
if (imAcq!=NULL && imAcq->size().height * imAcq->size().width >0)
    masque = UMat::zeros(imAcq->size(), CV_8U);  // type of mask is CV_8U
if (zoneTraitee.area()>0)
    masque(zoneTraitee) = 255;

int *nbCanaux={0}; 
int hbins = 30, sbins = 32;
float etendu[2];
if (imAcq->MinIm()==NULL)
	imAcq->ExtremumLoc();
etendu[0]=minHisto[0];
etendu[1]=maxHisto[0];
// saturation varies from 0 (black-gray-white) to
// 255 (pure spectrum color)
const float* listeEtendu[] = { etendu }; 
vector<Mat> planCouleur;

try
	{
	switch(imAcq->depth())
	{
	case CV_8U:
		split( *imAcq, planCouleur );
		for (int i=0;i<imAcq->channels()&&i<NB_MAX_CANAUX;i++)
			{
			etendu[0]=minHisto[i];
			etendu[1]=maxHisto[i]+1;
			calcHist(&planCouleur[i],1,nbCanaux,masque, histoImage[i], 1, &nbGraines[i], listeEtendu,
						 true, 
						 false);
			}
		break;
	case CV_8S:
		split( *imAcq, planCouleur );
		for (int i=0;i<imAcq->channels()&&i<NB_MAX_CANAUX;i++)
			{
			etendu[0]=minHisto[i];
			etendu[1]=maxHisto[i]+1;
			calcHist(&planCouleur[i],1,nbCanaux,masque, histoImage[i], 1, &nbGraines[i], listeEtendu,
						 true, 
						 false);
			}
		break;
	case CV_16S:
		split( *imAcq, planCouleur );
		for (int i=0;i<imAcq->channels()&&i<NB_MAX_CANAUX;i++)
			{
			Mat imSrc;
			etendu[0]=minHisto[i];
			etendu[1]=maxHisto[i]+1;
			planCouleur[i].convertTo(imSrc, CV_32FC1);

			calcHist(&imSrc,1,nbCanaux,masque, histoImage[i], 1, &nbGraines[i], listeEtendu,
						 true, 
						 false);
			}
		break;
	case CV_16U:
		split( *imAcq, planCouleur );
		for (int i=0;i<imAcq->channels()&&i<NB_MAX_CANAUX;i++)
			{
			etendu[0]=minHisto[i];
			etendu[1]=maxHisto[i]+1;
			Mat imF;
			planCouleur[i].convertTo(imF, CV_32F);

			calcHist(&imF,1,nbCanaux,masque, histoImage[i], 1, &nbGraines[i], listeEtendu,
						 true, 
						 false);
			}
		break;
	case CV_32F:
		split( *imAcq, planCouleur );
		for (int i=0;i<imAcq->channels()&&i<NB_MAX_CANAUX;i++)
			{
			etendu[0]=minHisto[i];
			etendu[1]=maxHisto[i]+1;
			calcHist(&planCouleur[i],1,nbCanaux,masque, histoImage[i], 1, &nbGraines[i], listeEtendu,
						 true, 
						 false);
			}
		break;
	case CV_32S:
		split( *imAcq, planCouleur );
		for (int i=0;i<imAcq->channels()&&i<NB_MAX_CANAUX;i++)
			{
			etendu[0]=minHisto[i];
			etendu[1]=maxHisto[i]+1;
			Mat imF;
			planCouleur[i].convertTo(imF, CV_32F);

			calcHist(&imF,1,nbCanaux,masque, histoImage[i], 1, &nbGraines[i], listeEtendu,
						 true, 
						 false);
			}
		break;
	}
	}
catch(cv::Exception& e)
	{
	wxString s(e.msg);

	wxMessageBox(_("An error occured in calculation histogram  :")+s);
	}
}

void FenetreHistogramme::Plot(char fenetreActive)
{
if (!osgApp )
	return;
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
if (!fenMere)
	return;
int id=((FenetrePrincipale *)fenMere)->IdFenetre();
Layout();
ImageInfoCV			*imAcq;
CameraVirtuelle *cam=((FenetrePrincipale *)fenMere)->Cam();
if (((FenetrePrincipale *)fenMere)->ModeImage()!=1)
	imAcq =((FenetrePrincipale *)fenMere)->ImAcq();
int nbPlan=imAcq->channels();
if (histoImage[0].cols==0 || fenetreActive)
	{
	MajHistogramme();

	}
//imAcq->HistogrammeLocal(ligDeb,colDeb,&nbLig,&nbCol,histoImage);
if (!fenetreActive)
	{
	return;
	} 
wxPLplotstream* pls=courbe->GetStream();
if (!pls)
	return;
int iMin=0,iMax=16383;
if (excel)
{
    wxArrayInt selection=excel->GetSelectedRows();

    if (selection.size()!=0)
	    {
	    iMin=selection.Item(0);
	    iMax=selection.Item(selection.size()-1);
	    }
}
const size_t np=65536;
if (x[0]==NULL)
	for (int i=0;i<NB_MAX_CANAUX;i++)	
		{
		x[i]=new PLFLT[np];
		y[i]=new PLFLT[np];
		yFiltre[i]=new PLFLT[np];
		}
PLFLT xmin=iMin, xmax=iMax;
PLFLT ymin=1e30, ymax=-1e30;

wxString titre[]={_("Blue"),_("Green"),_("Red"),"canal 4","Canal 5"};
if (excel)
{
    excel->DefTitreLigne(0,"bins");
    excel->DefTitreLigne(1,"minimum");
    excel->DefTitreLigne(2,"maximum");
    excel->DefTitreLigne(3,"mean");
    excel->DefTitreLigne(4,"Std");
    excel->DefTitreLigne(5,"Mode");
    excel->DefTitreLigne(6,"Max");
    excel->DefTitreLigne(7,"Min");
    excel->DefTitreLigne(8,"Kurtosis");
    excel->DefTitreLigne(9,"SeuilKurtosis");
    excel->DefTitreLigne(10,"Asymetrie");
    excel->DefTitreLigne(11,"Seuil");
}
for (int j=0;j<nbPlan && j<NB_MAX_CANAUX && histoImage[j].cols;j++)
	{
	xmin=minHisto[j];
	xmax=maxHisto[j];
	iMin=xmin;
	iMax=xmax;
	if (excel)
        excel->DefTitreColonne(j,titre[j]);
	double moyenneH=0,varianceH=0,cumulH=0;
	int mode=iMin;
	for( int i = 0; i<nbGraines[j] ; i++ ) 
		{
		cumulH += histoImage[j].at<float>(i);
		x[j][i] = xmin+(xmax-xmin)/nbGraines[j]*i;
		moyenneH += x[j][i]*histoImage[j].at<float>(i)*(xmax-xmin)/nbGraines[j];
		y[j][i]=histoImage[j].at<float>(i);
		ymin=MIN( ymin, histoImage[j].at<float>(i) );
		ymax=MAX( ymax, histoImage[j].at<float>(i) );
		if (ymax==histoImage[j].at<float>(i))
			mode=i;
		}
	if (cumulH>0)
		moyenneH = moyenneH/cumulH;
	for( int i = 0; i<nbGraines[j]; i++ ) 
		{
		double p2;
		p2=(x[j][i]-moyenneH)*(x[j][i]-moyenneH);
		varianceH += x[j][i]*histoImage[j].at<float>(i);
		}
	
	if (cumulH>0)
		varianceH = varianceH/cumulH;
	double kurto=0,skew=0;
	int seuilKurto=0,seuilSkew=0;
	for( int i = 0; i<nbGraines[j]; i++ ) 
		{
		double p;
		p=(x[j][i]-moyenneH);
		kurto += p*p*p*p*histoImage[j].at<float>(i)/(cumulH*varianceH*varianceH);
		skew += p*p*p*histoImage[j].at<float>(i)/(cumulH*pow(varianceH,1.5));
		if (kurto<3.25)
			seuilKurto=i;
		if (skew<0.63)
			seuilSkew=i;
		}

	for( int i = 0; i<nbGraines[j]; i++ ) 
		{
		int nb=0;
		yFiltre[j][i]=0;
		for (int jj=-10;jj<=10;jj++)
			if (i+jj>=0 && i+jj<nbGraines[j])
			{
			yFiltre[j][i]+=y[j][i+jj];
			nb += 1;
			}
		yFiltre[j][i]/=nb;
		}
    if (excel)
    {
	    excel->DefCellule(0,j,nbGraines[j],"%d");
	    excel->DefCellule(1,j,minHisto[j]);
	    excel->DefCellule(2,j,maxHisto[j]);
	    excel->DefCellule(3,j,moyenneH,"%f");
	    excel->DefCellule(4,j,sqrt(varianceH),"%f");
	    excel->DefCellule(5,j,mode,"%d");
	    excel->DefCellule(6,j,ymax,"%f");
	    excel->DefCellule(7,j,ymin,"%f");
	    excel->DefCellule(8,j,kurto,"%f");
	    excel->DefCellule(9,j,seuilKurto,"%d");
	    excel->DefCellule(10,j,skew,"%f");
	    excel->DefCellule(11,j,seuilSkew,"%d");
	    for (int jj=3;jj<=11;jj++)
		    excel->SetReadOnly(jj,j);
    }
	}
 
pls->adv( 0 );
pls->scol0 (	3, 0,0,255);
pls->scol0 (	4, 0,0,128);
pls->scol0 (	5, 0,255,0);
pls->scol0 (	6, 0,128,0);
pls->scol0 (	7, 255,0,0);
pls->scol0 (	8, 128,0,0);

if(bgcolor) 
	{
	pls->scol0( 0, 255, 255, 255 );
	pls->scol0( 15, 0, 0, 0 ); 
	} 
else 
	{
	pls->scol0( 15, 255, 255, 255 );
	pls->scol0( 2, 0, 0, 0 ); 
	}
pls->col0( 1 );
pls->env( xmin, xmax, ymin*.99, ymax*1.01, 0, 0 );
pls->col0( 0 );
pls->lab( "x", "y", "Histogram");
for (int j=0;j<nbPlan && j<NB_MAX_CANAUX;j++)
	if(((FenetrePrincipale *)fenMere)->PlanActif(j))
		{
		pls->col0( 3+2*j);
		pls->width( 2.0 );
		pls->line( nbGraines[j], x[j], y[j] );
		pls->col0( 4+2*j);
		pls->width( 3.0 );
		pls->line( nbGraines[j], x[j], yFiltre[j] );
		}
pls->RenewPlot();
Refresh();
//plotwindow->SavePlot(wxString("pdf"),wxString("tt.pdf"));
//plotwindow->SavePlot(wxString("wxpng"),wxString("tt.png"));

}


