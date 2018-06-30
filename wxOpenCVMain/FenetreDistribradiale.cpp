#include "FenetrePrincipale.h"
#include "courbeplplot.h"
#include "Tableur.h"

BEGIN_EVENT_TABLE( FenetreDistribRadiale, wxWindow )
END_EVENT_TABLE()
BEGIN_EVENT_TABLE( FenetreDistribAngulaire, wxWindow )
END_EVENT_TABLE()



/*! Constructor of our custom frame, where the Menu is created and a
 *  a wxPLplotwindow is inserted into the frame. We plot some easy functions
 *  just to show how it works. wxPLplotwindow takes care of all the setup
 *  for the use of PLplot library.
 */
FenetreDistribRadiale::FenetreDistribRadiale(wxFrame *w  ) : wxWindow(w,-1,wxPoint(0,0),wxSize(400,400))
{
	bgcolor=false;
osgApp=NULL;
fenMere=NULL;
dr=NULL;

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


  wxString m_title=_T("I(r)");
  //SetTitle( m_title );  
excel->Show();
	Plot(true);
}

void FenetreDistribRadiale::Plot(bool b)
{
if (!osgApp)
	return;
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
if (!b)
	return;
if (!fenMere)
	return;
int id=((FenetrePrincipale *)fenMere)->IdFenetre();

ImageInfoCV			*imAcq;
if (((FenetrePrincipale *)fenMere)->ModeImage()!=1)
	imAcq =((FenetrePrincipale *)fenMere)->ImAcq();
else
	return;
long nbLig=imAcq->rows,nbCol=imAcq->cols;
nbVal=imAcq->rows+imAcq->cols;
const size_t np=16384;
nbPlan=imAcq->LitNbPlan();
if (dr==NULL)
	{
	dr= new double *[5*nbPlan];
	for (int i=0;i<5*nbPlan;i++)
		dr[i]=new double[nbVal];
	x=new PLFLT[np];
	y=new PLFLT[np];
	}
long ligDeb=0,colDeb=0;
for (int i=0;i<5*nbPlan;i++)
	for (int j=0;j<nbVal;j++)
		dr[i][j]=0;

if (((wxOpencvApp*)osgApp)->Graphique()->ModeRectangle())
	{
	wxRect	*r=((wxOpencvApp*)osgApp)->Graphique()->RectangleSelec();
	colDeb=r->GetLeft();
	ligDeb=r->GetTop();
	nbLig=r->GetHeight();
	nbCol=r->GetWidth();
	}
//imAcq->DistributionRadiale(colDeb+nbCol/2,ligDeb+nbLig/2,dr,nbVal);

wxPLplotstream* pls=courbe->GetStream();
if (!pls)
	return;

PLFLT xmin=0, xmax=std::min(nbLig,nbCol);
PLFLT ymin=1e30, ymax=1e-30;

for( size_t i = 0; i<nbVal && i<std::min(nbLig,nbCol); i++ ) 
	{
	x[i] = i;
	y[i]=dr[0][i];
	ymin=MIN( ymin, dr[0][i] );
	ymax=MAX( ymax, dr[0][i] );
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
pls->env( xmin, xmax, ymin*.99, ymax*1.01, 0, 0 );
pls->col0( 2 );
pls->lab( "x", "y", "Coupe");

pls->col0( 3 );
//pls->wid( 2 );
pls->line( std::min(nbLig,nbCol), x, y );

courbe->RenewPlot();
}

FenetreDistribRadiale::~FenetreDistribRadiale()
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
if (!fenMere)
	return;
if (!dr)
	return;

for (int i=0;i<5*nbPlan;i++)
	delete [](dr[i]);
delete []dr;
delete []x;
delete []y;
}

void	FenetreDistribRadiale::DefCellule(int l,int c, double val,char *format)
{
}


/*! Constructor of our custom frame, where the Menu is created and a
 *  a wxPLplotwindow is inserted into the frame. We plot some easy functions
 *  just to show how it works. wxPLplotwindow takes care of all the setup
 *  for the use of PLplot library.
 */
FenetreDistribAngulaire::FenetreDistribAngulaire(wxFrame *w  ) : wxWindow(w,-1,wxPoint(0,0),wxSize(400,400))
{
	bgcolor=false;
osgApp=NULL;
fenMere=NULL;
dr=NULL;
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


  wxString m_title="I(r)";
  //SetTitle( m_title );  
excel->Show();
	Plot(true);
}


FenetreDistribAngulaire::~FenetreDistribAngulaire()
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
if (!fenMere)
	return;
if (!dr)
	return;
int id=((FenetrePrincipale *)fenMere)->IdFenetre();
ImageInfoCV			*imAcq;
if (((wxOpencvApp*)osgApp)->Graphique(id)->ModeImage()!=1)
	imAcq =((wxOpencvApp*)osgApp)->Graphique(id)->ImAcq();
else
	return;
for (int i=0;i<5*imAcq->LitNbPlan();i++)
	delete []dr[i];
delete []dr;
delete []x;
delete []y;
}

void FenetreDistribAngulaire::Plot(bool b)
{
if (!osgApp)
	return;
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
if (!b)
	return;
if (!fenMere)
	return;
int id=((FenetrePrincipale *)fenMere)->IdFenetre();

ImageInfoCV			*imAcq;
if (((FenetrePrincipale *)fenMere)->ModeImage()!=1)
	imAcq =((FenetrePrincipale *)fenMere)->ImAcq();
else
	return;
long nbLig=imAcq->rows,nbCol=imAcq->cols;
int nbVal=360/10;
const size_t np=16384;
if (dr==NULL)
	{
	dr= new double *[5*imAcq->LitNbPlan()];
	for (int i=0;i<5*imAcq->LitNbPlan();i++)
		dr[i]=new double[nbVal];
	x=new PLFLT[np];
	y=new PLFLT[np];
	}
long ligDeb=0,colDeb=0;
	for (int i=0;i<5*imAcq->LitNbPlan();i++)
		for (int j=0;j<nbVal;j++)
			dr[i][j]=0;

if (((wxOpencvApp*)osgApp)->Graphique()->ModeRectangle())
	{
	wxRect	*r=((wxOpencvApp*)osgApp)->Graphique()->RectangleSelec();
	colDeb=r->GetLeft();
	ligDeb=r->GetTop();
	nbLig=r->GetHeight();
	nbCol=r->GetWidth();
	}
//imAcq->DistributionAngulaire(colDeb+nbCol/2,ligDeb+nbLig/2,dr,nbVal,std::min(nbLig,nbCol));

wxPLplotstream* pls=courbe->GetStream();
if (!pls)
	return;
PLFLT xmin=0, xmax=10*nbVal;
PLFLT ymin=1e30, ymax=1e-30;

for( size_t i = 0; i<nbVal && i<std::min(nbLig,nbCol); i++ ) 
	{
	x[i] = i*10;
	y[i]=dr[0][i];
	ymin=MIN( ymin, dr[0][i] );
	ymax=MAX( ymax, dr[0][i] );
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
pls->env( xmin, xmax, ymin*.99, ymax*1.01, 0, 0 );
pls->col0( 2 );
pls->lab( "x", "y", "Coupe");

pls->col0( 3 );
//pls->wid( 2 );
pls->line( std::min(nbLig,nbCol), x, y );

courbe->RenewPlot();
}


void	FenetreDistribAngulaire::DefCellule(int l,int c, double val,char *format)
{
}
