#include "FenetrePrincipale.h"
//#include "outilsImage.h"
#include "imagestat.h"
#include "Tableur.h"
#include <fstream>
#include <wx/busyinfo.h>


using namespace std;


#define SEUILLAGEHYSTERSIS

/*!< 1 Commentaire fonction Image Camera */
void FenetrePrincipale::ImageCamera(wxCommandEvent& event)
{
modeImage = 0;
//((wxOpencvApp*)osgApp)->Outils()->MAJValeurPalette();
MAJNouvelleImage();

}


#ifdef __ESTCEUTILE__
void OutilsImage::OuvertureOngletQuadriqueMulti()
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;


wxPoint	position[]={
// Texte		Réglette
wxPoint(10,10),wxPoint(130,10),
wxPoint(10,50),wxPoint(130,50),
wxPoint(10,90),wxPoint(130,90),wxPoint(200,90),
wxPoint(10,130),wxPoint(130,130),
wxPoint(250,10),wxPoint(290,10),wxPoint(340,10),wxPoint(440,10),
wxPoint(250,30),wxPoint(290,30),wxPoint(340,30),wxPoint(440,30),
wxPoint(250,50),wxPoint(290,50),wxPoint(340,50),wxPoint(440,50),
wxPoint(430,110),wxPoint(490,30),wxPoint(490,50),
wxPoint(290,90),wxPoint(200,130)};
wxSize	taille[]={
// Texte		Réglette
wxSize(120,20),wxSize(70,20),
wxSize(120,20),wxSize(70,20),
wxSize(120,40),wxSize(60,20),wxSize(60,20),
wxSize(120,20),wxSize(70,20),
wxSize(40,20),wxSize(50,20),wxSize(100,20),wxSize(50,20),
wxSize(40,20),wxSize(50,20),wxSize(100,20),wxSize(50,20),
wxSize(40,20),wxSize(50,20),wxSize(100,20),wxSize(50,20),
wxSize(50,20),wxSize(50,20),wxSize(50,20),
wxSize(200,20),wxSize(60,20)};
long style=wxSL_HORIZONTAL|wxSL_AUTOTICKS|wxSL_LABELS ; 

wxString	legende[]={
_T("Mean parameter α"),wxEmptyString,
_T("derivative parameter β"),wxEmptyString,
_T("Threshold high and low"),wxEmptyString,wxEmptyString,
_T("Minimum Surface"),wxEmptyString),
_T("Index"),_T("Surface"),_T("E& ecart"),_T("C"),
_T("9999"),_T("1000000"),_T("16000"),_T("16000"),
_T("5"),_T("6"),_T("7"),_T("8"),
_T("Merge"),_T("     "),_T("     "),_T("     ")};
ongletQuadriqueMulti = new wxWindow(listeFenetreOnglet,-1); 
int i=0;
new wxStaticText(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
i++;
wxString	s;
s.Printf(_T("%lf"),((wxOpencvApp*)osgApp)->Graphique()->DericheAlphaM());
new wxTextCtrl(ongletQuadriqueMulti,400+i,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
new wxStaticText(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
i++;
s.Printf(_T("%lf"),((wxOpencvApp*)osgApp)->Graphique()->DericheAlphaD());
new wxTextCtrl(ongletQuadriqueMulti,400+i,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
new wxStaticText(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
i++;
s.Printf(_T("%6.1lf"),(((wxOpencvApp*)osgApp)->Graphique()->SeuilModuleHaut()));
new wxTextCtrl(ongletQuadriqueMulti,400+i,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
s.Printf(_T("%6.1lf"),(((wxOpencvApp*)osgApp)->Graphique()->SeuilModuleBas()));
new wxTextCtrl(ongletQuadriqueMulti,400+i,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
new wxStaticText(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
i++;
s.Printf(_T("%d"),((wxOpencvApp*)osgApp)->Graphique()->SeuilSurface());
new wxTextCtrl(ongletQuadriqueMulti,400+i,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;
for (;i<21;i++)
	new wxStaticText(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
new wxButton(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
i++;
new wxStaticText(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
i++;
new wxStaticText(ongletQuadriqueMulti,400+i,legende[i],position[i], taille[i]);
i++;
wxArrayString modelFct;
modelFct.Add(_T("Atténuation quadratique mono"));
modelFct.Add(_T("Atténuation quadratique multi"));
modelFct.Add(_T("Atténuation cubique multi"));
modelFct.Add(_T("Atténuation Quadratique multi + bruit"));
modelFct.Add(_T("Atténuation Cubique multi + bruit"));
modelFct.Add(_T("Additif quadratique mono"));
modelFct.Add(_T("Additif quadratique multi"));
modelFct.Add(_T("Additif cubique multi"));
wxChoice *t=new wxChoice(ongletQuadriqueMulti,400+i,position[i],taille[i], modelFct);
//t->SetSelection(((wxOpencvApp*)osgApp)->Graphique()->ImAcq()->LitTypeModeleFctSEEC());
i++;
s.Printf(_T("%5.1f"),((wxOpencvApp*)osgApp)->Graphique()->DiffHauteur());
new wxTextCtrl(ongletQuadriqueMulti,400+i,s,position[i], taille[i],wxTE_PROCESS_ENTER|wxTE_PROCESS_TAB);
i++;

listeFenetreOnglet->AddPage(ongletQuadriqueMulti, _T("Quadric Multi-level"));
ongletQuadriqueMulti->Refresh();
}

int OutilsImage::TypeModele()
{
wxWindowList ww=(ongletQuadriqueMulti->GetChildren());
return ((wxChoice*)ww[24])->GetCurrentSelection();
}


void OutilsImage::OnTextValider(wxCommandEvent &w)
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
wxString s=((wxTextCtrl*)w.GetEventObject())->GetValue();
long	valEntiere;
double	valFlottante;	

s.ToLong(&valEntiere);
s.ToDouble(&valFlottante);
char mode =((wxOpencvApp*)osgApp)->Graphique()->ModeImage();
switch (w.GetId()){
case 409:
	{
	((wxOpencvApp*)osgApp)->Graphique()->DefDebNivPalette(valEntiere);
	int minCtrl,maxCtrl;
	slNivMin->SetValue(valEntiere);
	maxCtrl=slNivMax->GetMax( );
	slNivMax->SetRange(valEntiere, maxCtrl);
	((wxOpencvApp*)osgApp)->Graphique()->MAJNouvelleImage();
	((wxOpencvApp*)osgApp)->ImgStat()->DrawPalette();
	}
	break;
case 410:
	{
	((wxOpencvApp*)osgApp)->Graphique()->DefFinNivPalette(valEntiere);
	int minCtrl,maxCtrl;
	slNivMax->SetValue(valEntiere);
	minCtrl=slNivMin->GetMin();
	slNivMin->SetRange(minCtrl,valEntiere);
	((wxOpencvApp*)osgApp)->Graphique()->MAJNouvelleImage();
	((wxOpencvApp*)osgApp)->ImgStat()->DrawPalette();
	}
	break;
	}
switch(mode){
case 3 :
	break;
case 2 :	
	break;
	}
}
	
void OutilsImage::RegionPrincipaleSelect(int ind)
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;
}

void OutilsImage::RegionSecondaireSelect(int ind)
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;


}

void OutilsImage::FusionRegion(wxCommandEvent &w)
{
if ( !((wxOpencvApp*)osgApp)->VerifFenetre())
	return;

}

	
#endif

void FenetrePrincipale::DefNbMarcheFit(int x)
{
/*
if (nbMarcheFit!=x && poly)
	{
	for (int i=0;i<imAcq->LitNbPlan();i++)
		delete poly[i];
	delete poly;
	poly=NULL;
	}
if (poly==NULL)
	{
	poly =new double*[imAcq->LitNbPlan()];
	for (int i=0;i<imAcq->LitNbPlan();i++)
		{
		poly[i] = new  double[11+x];
		for (int j=0;j<11+x;j++)
			poly[i][j]=0;
		poly[i][5]=1;
		}
	}
nbMarcheFit = x;
*/	
}
