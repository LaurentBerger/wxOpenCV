#include "imagestat.h"


void ImageStatistiques::OuvertureOngletPalette() 
{
ongletPalette = new wxWindow(listeFenetreOnglet,-1); 

	wxPoint p(0,0);
	wxSize s(400,400);
	wxPanel* panel = new wxPanel( ongletPalette,wxID_ANY ,p,s);

wxWindow *fenPalette = new wxWindow(panel,-1,wxPoint(0,0),wxSize(200,512)); 

excelPalette = new Tableur((wxFrame*)panel,6,1); 
excelPalette->DefTitreColonne(0, "Intensity");
excelPalette->SetColSize(0, 80);

excelPalette->DefTitreColonne(1, "Z (nm)");
excelPalette->SetColSize(1, 50);
wxSizer *box = new wxGridSizer(2,1,0);

 	box->Add( fenPalette, 1, wxALL | wxEXPAND, 0 );
  	box->Add( excelPalette, 0, wxALL | wxEXPAND, 0 );
  panel->SetSizer( box );
    box->Fit(panel);
    box->SetSizeHints(panel);

listeFenetreOnglet->AddPage(ongletPalette, _T("Palette"));
DrawPaletteActuelle();
ongletPalette->Refresh();
}	


void ImageStatistiques::OnNotebook(wxNotebookEvent &evt)
{
int i=evt.GetSelection();
switch(i){
case 0:
	break;
case 1:
	if (ongletHistogramme)
		ongletHistogramme->Plot(true);
	break;
case 2:
	if (ongletHistogramme)
		ongletHistogramme->SetFocus();
	break;
case 3:
	if (ongletCoupe)
		ongletCoupe->SetFocus();
	break;
case 4:
	if (ongletDistribRadiale)
		ongletDistribRadiale->SetFocus();
	break;
case 5:
	if (ongletDistribAngulaire)
		ongletDistribAngulaire->SetFocus();
	break;
case 6:
	if (ongletFocus)
		ongletFocus->SetFocus();
	break;
case 7 :
	DrawPaletteActuelle();
	break;
case 8:
	break;
	}
}

void ImageStatistiques::DrawPaletteActuelle()
{
if ( !((wxOsgApp*)osgApp)->VerifFenetre())
	return;
wxWindowDC hdc(ongletPalette);
wxColor				*pCouleur=((wxOsgApp*)osgApp)->Graphique()->Palette();


wxRect r(0,0,200,600);

wxBrush	tr=*wxWHITE_BRUSH;
hdc.SetBrush(tr);

hdc.DrawRectangle(r);
tr=*wxTRANSPARENT_BRUSH;
hdc.SetBrush(tr);

wxPen crayon;
int ind=0,pas=((wxOsgApp*)osgApp)->Graphique()->NbCouleurPalette()/512;
int deb=((wxOsgApp*)osgApp)->Graphique()->SeuilNivBas();
ImageInfoCV* im=((wxOsgApp*)osgApp)->Graphique()->ImAcq();
int fin=((*(im->MaxIm()))[0]-((wxOsgApp*)osgApp)->Graphique()->SeuilNivBas(0))*fenMere->CoeffCanal(0);

for (int i=0;i<512;i++,ind+=pas)
	{
	crayon.SetColour(pCouleur[ind].Red(),pCouleur[ind].Green(),pCouleur[ind].Blue());
	hdc.SetPen(crayon);
	hdc.DrawLine(0,512-i,100,512-i);
	if (i>16 && i%32==1)
		{
		wxString w;
		w.Printf(_T("%d"),i*(fin-deb)/512+deb);
		hdc.DrawLine(100,512-i,150,512-i);
		crayon.SetColour(0,0,0);
		hdc.SetPen(crayon);
		hdc.DrawText(w,110,512-i);
		}
	}
}
