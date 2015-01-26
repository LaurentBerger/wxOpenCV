#include "FenetrePrincipale.h"
#include "courbeplplot.h"
#include "Tableur.h"
#include <vector>
using namespace std;

BEGIN_EVENT_TABLE( FenetreCoupe, wxWindow )
	EVT_KEY_DOWN(FenetreCoupe::OnKeyDown)
END_EVENT_TABLE()

static int          locate_mode=1;
static int          test_xor;
static int          fontset;
static char         *f_name;



static PLGraphicsIn gin;
static const char   *notes[] = { "Make sure you get it right!", "" };
static PLOptionTable options[] = {
    {
        "locate",               /* Turns on test of API locate function */
        NULL,
        NULL,
        &locate_mode,
        PL_OPT_BOOL,
        "-locate",
        "Turns on test of API locate function"
    },
    {
        "xor",                  /* Turns on test of xor function */
        NULL,
        NULL,
        &test_xor,
        PL_OPT_BOOL,
        "-xor",
        "Turns on test of XOR"
    },
    {
        "font",                 /* For switching between font set 1 & 2 */
        NULL,
        NULL,
        &fontset,
        PL_OPT_INT,
        "-font number",
        "Selects stroke font set (0 or 1, def:1)"
    },
    {
        "save",                 /* For saving in postscript */
        NULL,
        NULL,
        &f_name,
        PL_OPT_STRING,
        "-save filename",
        "Save plot in color postscript `file'"
    },
    {
        NULL,                   /* option */
        NULL,                   /* handler */
        NULL,                   /* client data */
        NULL,                   /* address of variable to set */
        0,                      /* mode flag */
        NULL,                   /* short syntax */
        NULL
    }                           /* long syntax */
};



/*! Constructor of our custom frame, where the Menu is created and a
 *  a wxPLplotwindow is inserted into the frame. We plot some easy functions
 *  just to show how it works. wxPLplotwindow takes care of all the setup
 *  for the use of PLplot library.
 */
FenetreCoupe::FenetreCoupe(wxFrame *w  ) : wxWindow(w,-1,wxPoint(0,0),wxSize(400,400))
{
	bgcolor=false;
osgApp=NULL;
fenMere=NULL;
  // add menu
//  SetIcon( wxIcon(graph) );

	// add the wxPLplot
	wxPoint p(0,0);
	wxSize s(400,400);
	panel = new wxPanel( this,wxID_ANY ,p,s);


	excel = new Tableur((wxFrame*)panel,11,5); 
    box = new wxGridSizer(2,1,0);
	plotwindow = new FenetreCourbe( (wxFrame*)panel,(wxFrame*)this,  -1, wxDefaultPosition, wxSize(400,400), wxWANTS_CHARS,
#if wxUSE_GRAPHICS_CONTEXT  
                                   wxPLPLOT_BACKEND_GC | wxPLPLOT_DRAW_TEXT );
#else
                                   wxPLPLOT_BACKEND_AGG | wxPLPLOT_DRAW_TEXT );
#endif
  plotwindow->Connect( wxEVT_CHAR, wxKeyEventHandler(FenetreCourbe::OnChar) );
 	box->Add( plotwindow, 1, wxALL | wxEXPAND, 10 );
  	box->Add( excel, 0, wxALL | wxEXPAND, 10 );
  panel->SetSizer( box );
    box->Fit(panel);
    box->SetSizeHints(panel);
	SetSize( 640, 500 );  // set frame size
  SetSizeHints( 220, 150 );  // set minimum frame size
  x[0]=NULL;

  wxString m_title=_T("Histogram");
  switch(plotwindow->getBackend()) {
  case wxPLPLOT_BACKEND_DC:
  	m_title += wxT(" (basic)");
  	break;
  case wxPLPLOT_BACKEND_GC:
  	m_title += wxT(" (wxGC)");
  	break;
  case wxPLPLOT_BACKEND_AGG:
  	m_title += wxT(" (AGG)");
  	break;
  default:
  	break;
  }
  //SetTitle( m_title );  
excel->Show();
wxPLplotstream* pls=plotwindow->GetStream();
    
    int x=pls->MergeOpts( options, "x01c options", notes);
	Plot(false);
}

FenetreCoupe::~FenetreCoupe() 
{
if (!fenMere)
	return;
 delete plotwindow;
 delete excel;
 delete panel;
 if (x[0])
 for (int i=0;i<3;i++)
	{
	delete []x[i];
	delete []y[i];
	delete []yFiltre[i];  /*!< courbe des histogrammes pour les trois plans */
	}
}


void FenetreCoupe::Plot(bool b)
{
if (!osgApp)
	return;
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
if (!b)
	return;
ImageInfoCV			*imAcq=((wxOsgApp*)osgApp)->Graphique()->ImAcq();
long nbLig=imAcq->LitNbLigne(),nbCol=imAcq->LitNbColonne();
static long*h=new long[nbLig+nbCol];
long ligDeb=0,colDeb=0;

cv::Point pt1((float)rCoupe.GetLeft(),(float)rCoupe.GetTop());
cv::Point pt2((float)rCoupe.GetRight(),(float)rCoupe.GetBottom());
if (((wxOsgApp*)osgApp)->Graphique()->ModeRectangle())
	{
	wxRect	*r=((wxOsgApp*)osgApp)->Graphique()->RectangleSelec();
	colDeb=r->GetLeft();
	ligDeb=r->GetTop();
	nbLig=r->GetHeight();
	nbCol=r->GetWidth();
	}

int nbVal=0;


//imAcq->CoupeLineaire((float)rCoupe.GetLeft(),(float)rCoupe.GetTop(),(float)rCoupe.GetRight(),(float)rCoupe.GetBottom(),h);
// grabs pixels along the line (pt1, pt2)
// from 8-bit 3-channel image to the buffer
cv::LineIterator it(*imAcq, pt1, pt2, 8);
cv::LineIterator it2 = it;
vector<cv::Vec3b> buf(it.count);
const size_t np=16384;

if (x[0]==NULL)
	for (int i=0;i<3;i++)	
		{
		x[i]=new PLFLT[np];
		y[i]=new PLFLT[np];
		yFiltre[i]=new PLFLT[np];
		}
switch(imAcq->type())
{
case CV_8UC1:
case CV_16UC1:
	for(int i = 0; i < it.count; i++, ++it)
		{
		x[0][i] =  norm(it.pos()-it2.pos());
		y[0][i]=((const cv::Vec3b)*it)[0];
		nbVal++;
		}
	break;
case CV_8UC3:
	for(int i = 0; i < it.count; i++, ++it)
		{
		x[0][i] = norm(it.pos()-it2.pos());
		y[0][i]=((const cv::Vec3b)*it)[0];
		y[1][i]=((const cv::Vec3b)*it)[1];
		y[2][i]=((const cv::Vec3b)*it)[2];
		nbVal++;
		}
	break;
}

wxPLplotstream* pls=plotwindow->GetStream();
    

double moyenneH=0,varianceH=0,cumulH=0;
PLFLT ymin=1e30, ymax=-1e30;

int nbPlan=1;
if (imAcq->type()==CV_8UC3)
	nbPlan=3;

for (int j=0;j<nbPlan;j++)
	{
	double moyenneH=0,varianceH=0,cumulH=0;
	for( size_t i = 0; i<nbVal; i++ ) 
		{
		cumulH ++;
		moyenneH += y[j][i];
		ymin=MIN( ymin, y[j][i] );
		ymax=MAX( ymax, y[j][i] );
		}
	if (cumulH>0)
		moyenneH = moyenneH/cumulH;
	for( size_t i = 0; i<nbVal; i++ ) 
		varianceH += (y[j][i]-moyenneH)*(y[j][i]-moyenneH);
	if (cumulH>0)
		varianceH = varianceH/cumulH;
	for( size_t i = 0; i<nbVal; i++ ) 
		{
		int nb=0;
		yFiltre[j][i]=0;
		for (int jj=-2;jj<=2;jj++)
			if (i+jj>=0 && i+jj<nbVal)
			{
			yFiltre[j][i]+=y[j][i+jj];
			nb += 1;
			}
		yFiltre[j][i]/=nb;
		}

	excel->DefCellule(0,0,"mean");
	excel->DefCellule(1,0,"Std");
	excel->DefCellule(2,0,"Max");
	excel->DefCellule(3,0,"Min");
	excel->DefCellule(0,j+1,moyenneH,"%f");
	excel->DefCellule(1,j+1,sqrt(varianceH),"%f");
	excel->DefCellule(2,j+1,ymax,"%f");
	excel->DefCellule(3,j+1,ymin,"%f");
	}
pls->adv( 0 );
pls->scol0 (	3, 0,0,255);
pls->scol0 (	4, 0,0,128);
pls->scol0 (	5, 0,255,0);
pls->scol0 (	6, 0,128,0);
pls->scol0 (	7, 255,0,0);
pls->scol0 (	8, 128,0,0);
if(bgcolor) {
	pls->scol0( 0, 255, 255, 255 );
	pls->scol0( 15, 0, 0, 0 ); 
} else {
	pls->scol0( 15, 255, 255, 255 );
	pls->scol0( 0, 0, 0, 0 ); 
}
pls->col0( 1 );
pls->env( x[0][0], x[0][nbVal-1], ymin*.99, ymax*1.01, 0, 0 );
pls->col0( 2 );
pls->lab( "x", "y", "Coupe");

for (int j=0;j<nbPlan;j++)
	{
	pls->col0( 3+2*j );
	pls->line( nbVal, x[0], y[j] );
	pls->col0( 4+2*j );
	pls->line( nbVal, x[0], yFiltre[j] );
	}
plotwindow->RenewPlot();
if (((wxOsgApp*)osgApp)->Graphique()->ModeCoupe())
	CoordonneeGraphique();
}


void	FenetreCoupe::DefCellule(int l,int c, double val,char *format)
{
}

void FenetreCoupe::CoordonneeGraphique()
{
wxPLplotstream* pls=plotwindow->GetStream();


 //while(1)
	{           
            int x=pls->GetCursor( &gin ) ;
           if (x)
             {
				 pls->gra();

				pls->text();
				if ( gin.keysym < 0xFF && isprint( gin.keysym ) )
					{
	/*                cout << "wx = " << gin.wX << ", wy = " << gin.wY <<
					", dx = " << gin.dX << ",  dy = " << gin.dY <<
					",  c = '" << gin.keysym << "'" << endl;*/
					}
				else
					{
	/*                cout << "wx = " << gin.wX << ", wy = " << gin.wY <<
					", dx = " << gin.dX << ",  dy = " << gin.dY <<
					",  c = '" << gin.keysym << "'" << endl;*/
					}
				//	printf("wx = %f,  wy = %f, dx = %f,  dy = %f,  c = 0x%02x\n",
				//                              gin.wX, gin.wY, gin.dX, gin.dY, gin.keysym);

				pls->gra();
				}
	}
}


void FenetreCoupe::OnKeyDown(wxKeyEvent &event)
{
((wxOsgApp*)osgApp)->Graphique()->TraitementClavier(event);

event.Skip();
}
