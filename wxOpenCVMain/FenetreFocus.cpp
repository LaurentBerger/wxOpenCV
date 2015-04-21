#include "FenetrePrincipale.h"
#include "courbeplplot.h"
#include "Tableur.h"

BEGIN_EVENT_TABLE( FenetreFocus, wxWindow )
END_EVENT_TABLE()



/*! Constructor of our custom frame, where the Menu is created and a
 *  a wxPLplotwindow is inserted into the frame. We plot some easy functions
 *  just to show how it works. wxPLplotwindow takes care of all the setup
 *  for the use of PLplot library.
 */
FenetreFocus::FenetreFocus(wxFrame *w  ) : wxWindow(w,-1,wxPoint(0,0),wxSize(400,400))
{
bgcolor=false;
osgApp=NULL;
fenMere=NULL;
razHisto=1;
for (int i=0;i<NBELT_FILE;i++)
	for (int j=0;j<NB_MAX_RECTANGLE;j++)
		histoStat[j][i]=0;
for (int j=0;j<NB_MAX_RECTANGLE;j++)
	posFile[j]=0;

  // add menu
//  SetIcon( wxIcon(graph) );

	// add the wxPLplot
	wxPoint p(0,0);
	wxSize s(400,400);
	wxPanel* panel = new wxPanel( this,wxID_ANY ,p,s);


	excel = new Tableur((wxFrame*)panel,11,5); 
    wxSizer *box = new wxGridSizer(2,1,0);
	courbe =  new wxPLplotwindow<wxPanel>();
 	box->Add( courbe, 1, wxALL | wxEXPAND, 10 );
  	box->Add( excel, 0, wxALL | wxEXPAND, 10 );
  panel->SetSizer( box );
    box->Fit(panel);
    box->SetSizeHints(panel);
	SetSize( 640, 500 );  // set frame size
  SetSizeHints( 220, 150 );  // set minimum frame size


  wxString m_title=_T("Histogram");
  //SetTitle( m_title );  
excel->Show();
	Plot(0);
}

FenetreFocus::~FenetreFocus()
{
if (!fenMere)
	return;
delete excel;
delete courbe;


}

void	FenetreFocus::OnClose(wxCloseEvent& event)
{
}

void FenetreFocus::Plot(int indRect)
{
if (!osgApp)
	return;

wxPLplotstream* pls=courbe->GetStream();
if (!pls)
	return;
const size_t np=16384;
static PLFLT *x=new PLFLT[NBELT_FILE],*y=new PLFLT[NBELT_FILE];
PLFLT xmin=0, xmax=NBELT_FILE-1;
PLFLT ymin=1e30, ymax=-1e-30;

for( size_t i = 0; i<NBELT_FILE; i++ ) 
	{
	x[i] = i;
	if (i+posFile[indRect]>=NBELT_FILE)
		y[i]=histoStat[indRect][i+posFile[indRect]-NBELT_FILE];
	else
		y[i]=histoStat[indRect][i+posFile[indRect]];
	ymax=MAX( ymax, y[i] );
	ymin=MIN( ymin, y[i] );
	}

pls->adv( 0 );
if(bgcolor) {
	pls->scol0( 0, 255, 255, 255 );
	pls->scol0( 15, 0, 0, 0 ); 
} else {
	pls->scol0( 15, 255, 255, 255 );
	pls->scol0( 0, 0, 0, 0 ); 
}
pls->col0( 1 );
pls->env( xmin, xmax, ymin-0.001, ymax+0.001, 0, 0 );
pls->col0( 2 );
pls->lab( "t", "s(i)", "Std");

pls->col0( 3 );
//pls->wid( 2 );
pls->line( NBELT_FILE, x, y );

courbe->RenewPlot();
}


void	FenetreFocus::DefCellule(int l,int c, double val,char *format)
{
excel->DefCellule(l,c,val,format);
if (c==3 && l<NB_MAX_RECTANGLE)
	{
	if (razHisto)
		{
		for (int i=0;i<NBELT_FILE;i++)
			histoStat[l][i]=val;	
		razHisto=0;
		}
	histoStat[l][posFile[l]]=val;
	posFile[l]++;
	if (posFile[l]>=NBELT_FILE)
		posFile[l]=0;
	}
}
