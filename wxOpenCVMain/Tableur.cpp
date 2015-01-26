#include <fstream>
#include <iostream>
#include "Tableur.h"
using namespace std;

BEGIN_EVENT_TABLE(Tableur, wxGrid)
//	EVT_GRID_EDITOR_HIDDEN(Tableur::CelluleModifiee)
//	EVT_GRID_CELL_CHANGE(Tableur::CelluleModifiee)
 //   EVT_GRID_SELECT_CELL(Tableur::OnSelectCell )
 //   EVT_GRID_RANGE_SELECT( Tableur::OnRangeSelected )
END_EVENT_TABLE()


void Tableur::CelluleModifiee(wxGridEvent &w)
{

}
 
void Tableur::OnSelectCell(wxGridEvent &w)
{

}

void Tableur::OnRangeSelected(wxGridRangeSelectEvent &w)
{

}

Tableur::Tableur(wxFrame *w,int nbL,int nbC):wxGrid(w,-1,wxPoint( 0, 0 ),wxSize( 250, 200 ))
{

// Grille de nbLignes par nbColonnes
// 
if (nbL>=0 && nbC>=0)
	{
	CreateGrid( nbL+1, nbC+1 );
// We can set the sizes of individual rows and columns
// in pixels
/*	for (int i=0;i<nbL;i++)
		SetRowSize( i, 20 );
	for (int i=0;i<nbC;i++)
		SetColSize( i, 50 );*/
	nbLigne=nbL+1;
	nbColonne=nbC+1;
	}
Show();
}



void Tableur::DefCellule(int l,int c, double val,char *format)
{
if (l>=0 && l<nbLigne && c>=0 && c<nbColonne)
	{	
	wxString w;
	wxString str(format, wxConvUTF8);
	if (format)
		w.Printf(str,val);
	else
		w.Printf(_T("%lf"),val);
	if (!IsVisible(l+1, 1))
		MakeCellVisible(l+1, 1);

	SetCellValue(l, c, w);

	}
}

void Tableur::DefCellule(int l,int c, int val,char *format)
{
if (l>=0 && l<nbLigne && c>=0 && c<nbColonne)
	{	
	wxString w;
	wxString str(format, wxConvUTF8);
	if (format)
		w.Printf(str,val);
	else
		w.Printf("%d",val);
/*	if (!IsVisible(l+1, 1))
		MakeCellVisible(l+1, 1);*/

	SetCellValue(l, c, w);

	}
}


void Tableur::SelectionLigne(int l)
{
if (l>=0 && l<nbLigne )
	SelectRow(l);
}

void Tableur::CelluleVisible(int l,int c)
{
if (l>=0 && l<nbLigne && c>=0 && c<nbColonne)
	if (!IsVisible(l, 1))
		MakeCellVisible(l, 1);
}


void Tableur::DefCellule(int l,int c, char *s)
{
if (l>=0 && l<nbLigne && c>=0 && c<nbColonne && s)
	{	

	wxString str(s, wxConvUTF8);
	SetCellValue(l, c, str);

	}
}

void Tableur::DefCellule(int l,int c, wxString *s)
{
if (l>=0 && l<nbLigne && c>=0 && c<nbColonne && s)
	{	

	SetCellValue(l, c, *s);

	}
}

void Tableur::DefTitreColonne(int c, char *s)
{
if ( c>=0 && c<nbColonne)
	{
	wxString str(s, wxConvUTF8);
	SetColLabelValue(c, str);
	}
}

void Tableur::DefTitreColonne(int c, wxString &s)
{
if ( c>=0 && c<nbColonne)
	{
	SetColLabelValue(c, s);
	}
}

void Tableur::DefTitreLigne(int l, char *s)
{
if ( l>=0 && l<nbLigne)
	{
	wxString str(s, wxConvUTF8);
	SetRowLabelValue(l, str);
	}
}

void Tableur::DefTitreLigne(int l, wxString &s)
{
if ( l>=0 && l<nbLigne)
	{
	SetRowLabelValue(l, s);
	}
}

double Tableur::LitCellule(int l,int c,char	&status)
{
if (l>=0 && l<nbLigne && c>=0 && c<nbColonne)
	{
	status=1;	
	wxString w=GetCellValue(l,c);
	if (w==_T(""))
		status=0;
	double x=0;
	w.ToDouble(&x);
	return x;
	}
return 0;
}

void Tableur::AjouteLigne(int l)
{
if (l>0)
	{
	nbLigne+=l;
	AppendRows(l);
	}
}

void Tableur::EffaceLigne(int l)
{
if (l>0)
	{
	DeleteRows(nbLigne-l,l);
	nbLigne-=l;
	}
}



