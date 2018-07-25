#include "FenetreSeqOpe.h"
#include "imagestat.h"
#include "GrapheOperation.h"
#include <wx/hyperlink.h>

using namespace std;

#define IDSAUVER 500
#define IND_OPE 100
#define LISTE_OP_SEQ 101
#define IND_HYPER 10 // hyperlien
#define IND_STATIC 200 // champ static
#define IND_SPIN 300 // bouton spin
#define IND_COMBO 500 // combobox pour choix
#define IND_SELECMULTIPLE IND_HYPER-1 // Selection de fichier
#define IND_FENETREUNIQUE IND_HYPER-2 // Selection de fichier
#define IND_FICHIERS IND_HYPER-3 // Selection de fichier
#define IND_CONFIG IND_HYPER-4 // Selection de fichier




FenetreSequenceOperation::FenetreSequenceOperation(FenetrePrincipale *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size,wxOpencvApp *osg, long style)
     : wxFrame(frame, wxID_ANY, title, pos, size, wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN)
{
ImageInfoCV xx;
fenMere=frame;
tailleMax=wxSize(0,0);
panneau = new wxPanel( this,wxID_ANY ,wxPoint(0,0),wxSize(400,400));
//wxPanel	*panneauCtrl = new wxPanel( panel,  -1, wxDefaultPosition, wxSize(400,400));
this->osgApp =osg;
seqActif=osg->TabSeqOperation();

new wxStaticText( panneau, -1, _("Sequence"),wxPoint(10,20),wxSize(60,20) );
new wxButton( panneau,wxID_OK,_("Execute all"),wxPoint(160,20),wxSize(70,20));
new wxButton(panneau, IND_SELECMULTIPLE, _("Select file to process"), wxPoint(230, 20), wxSize(150, 20));
wxSpinCtrl *spw=new wxSpinCtrl(panneau,IND_OPE,_("Sequence"),wxPoint(80,20),wxSize(60,20));
new wxCheckBox(panneau, IND_FENETREUNIQUE, _("No new window"), wxPoint(260, 40), wxSize(120, 20));
wxButton *b=new wxButton(panneau, IND_FICHIERS , _("Load"), wxPoint(10, 40), wxSize(60, 20));
wxCheckBox *c=new wxCheckBox(panneau, IND_CONFIG, _("Use File Config"), wxPoint(80, 40), wxSize(160, 20));
c->SetValue(true);
b->Disable();
spw->SetRange(0,seqActif->size()-1);
spw->SetValue(0);
nbEtape=100;
int n=(*seqActif).begin()->first;
if (nbEtape<(*seqActif)[n].size())
	nbEtape=(*seqActif)[n].size();
nomEtape=new wxString[nbEtape];
int i=0;

for (std::vector <ParametreOperation >::iterator it = (*seqActif)[n].begin() ; it != (*seqActif)[n].end(); ++it,++i)
    nomEtape[i]=(*it).nomOperation;
choixOp=new wxListBox( panneau,LISTE_OP_SEQ,wxPoint(80,60),wxSize(150,-1),(*seqActif)[n].size(),nomEtape);
choixOp->SetSelection(0);
InsererCtrlEtape(&((*seqActif)[n][0]));
Bind(wxEVT_SPINCTRL, &FenetreSequenceOperation::OnSpinEntier,this);
Bind(wxEVT_SPINCTRLDOUBLE, &FenetreSequenceOperation::OnSpinReel,this);
Bind(wxEVT_COMMAND_LISTBOX_SELECTED, &FenetreSequenceOperation::OnOpeSelec,this);
Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FenetreSequenceOperation::Executer,this,wxID_OK);
Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FenetreSequenceOperation::SelectionFichier, this, IND_SELECMULTIPLE);
Bind(wxEVT_COMMAND_BUTTON_CLICKED, &FenetreSequenceOperation::OuvrirSequence, this, IND_FICHIERS);
Bind(wxEVT_COMMAND_COMBOBOX_SELECTED, &FenetreSequenceOperation::ComboBox, this);
Bind(wxEVT_COMMAND_CHECKBOX_CLICKED, &FenetreSequenceOperation::BasculeConfig, this, IND_CONFIG);
    }



bool FenetreSequenceOperation::IndiceSequence(int &ind)
{
    wxSpinCtrl *ws=(wxSpinCtrl *)wxWindow::FindWindowById(IND_OPE,panneau);

    if (ws==NULL)
    {
        ind=-1;
        return false;
    }
    ind = ws->GetValue();
    return true;
 }
    /*!< Indice de la séquence choisie par l'utilisateur*/
bool FenetreSequenceOperation::IndiceOperation(int &ind)
{
   ind=choixOp->GetSelection();

   return true;
}


void FenetreSequenceOperation::InsererCtrlEtape(ParametreOperation *op)
{
ParametreOperation *pOCV=op;
int ligne=150;
int	indStatic=IND_STATIC;
int indSpin=IND_SPIN;
int indHyper=IND_HYPER;
int indCombo=IND_COMBO;

wxStaticText		*wst;
wxHyperlinkCtrl		*whc;
wxSpinCtrlDouble	*wsd;

lienCombo.clear();
if((whc = (wxHyperlinkCtrl*)wxWindow::FindWindowById(indHyper, panneau)) != NULL)
	whc->SetURL(pOCV->lienHtml);
else
	new wxHyperlinkCtrl (panneau,indHyper," OpenCV Documentation",pOCV->lienHtml,wxPoint(10,ligne-30),wxSize(150,20));
indHyper++;
if ((whc=(wxHyperlinkCtrl*)wxWindow::FindWindowById(indHyper,panneau))!=NULL)
	whc->SetURL(pOCV->lienHtml);
else
	new wxHyperlinkCtrl(panneau,indHyper,"PDF Documentation",pOCV->refPDF,wxPoint(160,ligne-30),wxSize(200,20));
indHyper++;
std::map<std::string,DomaineParametreOp<cv::Size> >::iterator its;
if (tailleMax.x<410)
	tailleMax.x= 410;
if (tailleMax.y<ligne)
	tailleMax.y= ligne;
for (its=pOCV->sizeParam.begin();its!=pOCV->sizeParam.end();its++)
	{
	wxString nombre;
	nombre.Printf("%d",its->second.valeur.width);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indSpin-100,panneau))!=NULL)
		{
		wst->SetLabel(its->first + " x");
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau, indSpin-100, its->first + " x", p, s);
	wst->Show(true);
	p += wxPoint(s.GetX(),0);
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	else
		wsd->Move(p);
	wsd->SetIncrement((double)its->second.pas.width);
	wsd->SetRange(its->second.mini.width, its->second.maxi.width);
	wsd->SetValue(its->second.valeur.width);
	wsd->Show(true);
	indStatic++;
	indSpin++;
	p += wxPoint(s.GetX(),0);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indSpin-100,panneau))!=NULL)
		{
		wst->SetLabel(its->first + " y");
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau, indSpin-100, its->first + " y", p, s);
	wst->Show(true);
	p += wxPoint(s.GetX(),0);
	nombre.Printf("%d",its->second.valeur.height);
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	else
		wsd->Move(p);

	wsd->SetName(its->first);
	wsd->SetRange(its->second.mini.height, its->second.maxi.height);
	wsd->SetIncrement((double)its->second.pas.height);
	wsd->SetValue(its->second.valeur.height);
	wsd->Show(true);
	indStatic++;
	indSpin++;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	ligne+=20;
	}
std::map<std::string,DomaineParametreOp<int> >::iterator iti;
for (iti=pOCV->intParam.begin();iti!=pOCV->intParam.end();iti++)
	{
    if (ParametreOperation::listeParam.find(iti->first) == ParametreOperation::listeParam.end())
        {
	    wxString nombre;
	    nombre.Printf("%d",iti->second.valeur);
	    wxPoint p(10,ligne);
	    wxSize	s(100,20);
	    if ((wst=(wxStaticText*)panneau->FindWindowById(indSpin-100,panneau))!=NULL)
		    {
		    wst->SetLabel(iti->first);
		    wst->Move(p);
		    }
	    else
		    wst = new wxStaticText(panneau,indSpin-100,iti->first,p, s);
	    wst->Show(true);
	    p += wxPoint(s.GetX(),0);


        if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		    wsd =new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	    else
		    wsd->Move(p);
	    wsd->SetRange(iti->second.mini,iti->second.maxi);
	    wsd->SetName(iti->first);
	    wsd->SetValue(iti->second.valeur);
	    wsd->SetIncrement(iti->second.pas);
	    wsd->Show(true);
	    indSpin++;
        /*wxSpinCtrlDouble *sp = new wxSpinCtrlDouble(page, indOriCtrl + 2 * nbParam + 1, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
        sp->SetRange(iti->second.mini, iti->second.maxi);
        sp->SetIncrement(iti->second.pas);
        if (iti->second.res)
            sp->Disable();*/
	    if (tailleMax.x<p.x+s.x)
		    tailleMax.x= p.x+s.x;
	    if (tailleMax.y<p.y+s.y)
		    tailleMax.y= p.y+s.y;
	    ligne+=20;
        }
	}
std::map<std::string,DomaineParametreOp<double> >::iterator itd;
for (itd=pOCV->doubleParam.begin();itd!=pOCV->doubleParam.end();itd++)
	{
	wxString nombre;
	nombre.Printf("%f",itd->second.valeur);
	wxPoint p(10,ligne);
	wxSize	s(100,20);
	if ((wst=(wxStaticText*)panneau->FindWindowById(indSpin-100,panneau))!=NULL)
		{
		wst->SetLabel(itd->first);
		wst->Move(p);
		}
	else
		wst = new wxStaticText(panneau,indSpin-100,itd->first,p, s);
	wst->Show(true);
	p += wxPoint(s.GetX(),0);
	if ((wsd=(wxSpinCtrlDouble*)panneau->FindWindowById(indSpin,panneau))==NULL)
		wsd=new wxSpinCtrlDouble(panneau,indSpin,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	else
		wsd->Move(p);
	wsd->SetRange(itd->second.mini,itd->second.maxi);
	wsd->SetName(itd->first);
	wsd->SetIncrement(itd->second.pas);
	wsd->SetValue(itd->second.valeur);
	wsd->Show(true);
	indSpin++;
	if (tailleMax.x<p.x+s.x)
		tailleMax.x= p.x+s.x;
	if (tailleMax.y<p.y+s.y)
		tailleMax.y= p.y+s.y;
	ligne+=20;
	}
std::map<std::string, DomaineParametreOp<cv::Point> >::iterator itp;
for (itp = pOCV->pointParam.begin(); itp != pOCV->pointParam.end(); itp++)
{
	wxString nombre;
	nombre.Printf("%d", itp->second.valeur.x);
	wxPoint p(10, ligne);
	wxSize	s(100, 20);
	if ((wst = (wxStaticText*)panneau->FindWindowById(indSpin-100, panneau)) != NULL)
	{
		wst->SetLabel(itp->first + " x");
		wst->Move(p);
	}
	else
		wst = new wxStaticText(panneau, indSpin-100, itp->first + " x", p, s);
	wst->Show(true);
	p += wxPoint(s.GetX(), 0);
	//	wxSpinCtrl *spw=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	if ((wsd = (wxSpinCtrlDouble*)panneau->FindWindowById(indSpin, panneau)) == NULL)
		wsd = new wxSpinCtrlDouble(panneau, indSpin, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
	else
		wsd->Move(p);
	wsd->SetRange(0, 256);
	wsd->SetIncrement((double)itp->second.pas.x);
	wsd->SetRange(itp->second.mini.y, itp->second.maxi.y);
	wsd->SetValue(itp->second.valeur.x);
	wsd->Show(true);
	indSpin++;
	p += wxPoint(s.GetX(), 0);
	if ((wst = (wxStaticText*)panneau->FindWindowById(indSpin-100, panneau)) != NULL)
	{
		wst->SetLabel(itp->first + " y");
		wst->Move(p);
	}
	else
		wst = new wxStaticText(panneau, indSpin-100, itp->first + " y", p, s);
	wst->Show(true);
	p += wxPoint(s.GetX(), 0);
	nombre.Printf("%d", itp->second.valeur.y);
	//	wxSpinCtrl *sph=new wxSpinCtrl(page,indOriCtrl+2*nbParam+1,nombre,p,s,wxSP_WRAP|wxSP_ARROW_KEYS );
	//	sph->SetRange(0,256);
	if ((wsd = (wxSpinCtrlDouble*)panneau->FindWindowById(indSpin, panneau)) == NULL)
		wsd = new wxSpinCtrlDouble(panneau, indSpin, nombre, p, s, wxSP_WRAP | wxSP_ARROW_KEYS);
	else
		wsd->Move(p);

	wsd->SetName(itp->first);
	wsd->SetRange(itp->second.mini.y, itp->second.maxi.y);
	wsd->SetIncrement((double)itp->second.pas.y);
	wsd->SetValue(itp->second.valeur.y);
	wsd->Show(true);
	indSpin++;
	if (tailleMax.x<p.x + s.x)
		tailleMax.x = p.x + s.x;
	if (tailleMax.y<p.y + s.y)
		tailleMax.y = p.y + s.y;
	ligne += 20;
}

for (iti=pOCV->intParam.begin();iti!=pOCV->intParam.end();iti++)
	{
    if (ParametreOperation::listeParam.find(iti->first) != ParametreOperation::listeParam.end())
        {
	    wxString nombre;
	    nombre.Printf("%d",iti->second.valeur);
	    wxPoint p(10,ligne);
	    wxSize	s(100,20);
	    if ((wst=(wxStaticText*)panneau->FindWindowById(indCombo-100,panneau))!=NULL)
		    {
		    wst->SetLabel(iti->first);
		    wst->Move(p);
		    }
	    else
		    wst = new wxStaticText(panneau,indCombo-100,iti->first,p, s);
	    wst->Show(true);
	    p += wxPoint(s.GetX(),0);


        int nbChaine = pOCV->listeParam[iti->first].size(),valDefaut=0;
        wxString *choix = new wxString[nbChaine], choixDefaut;
        int i = 0;
        for (std::map <string, int >::iterator iter = pOCV->listeParam[iti->first].begin(); iter != pOCV->listeParam[iti->first].end(); ++iter, ++i)
            {
            choix[i] = iter->first;
            if (iter->second == iti->second.valeur)
            {
                choixDefaut = iter->first;
                valDefaut=i;
            }
            }
        wxComboBox *cb;
        if ((cb = (wxComboBox*)panneau->FindWindowById(indCombo, panneau)) == NULL)
        {
            cb = new wxComboBox(panneau, indCombo, choixDefaut, p, wxSize(250, -1), nbChaine, choix,0,wxDefaultValidator,iti->first);
            lienCombo.insert(make_pair(indCombo, iti->first));
        }
        else
            {
            cb->Clear();
            cb->Insert(nbChaine, choix, 0);
            cb->SetSelection(valDefaut);
            cb->Move(p);
            cb->Show(true);
            lienCombo.insert(make_pair(indCombo, iti->first));
        }
        if (iti->second.res)
            cb->Disable();
        indCombo++;
 	    if (tailleMax.x<p.x + s.x)
		    tailleMax.x = p.x + s.x;
	    if (tailleMax.y<p.y + s.y)
		    tailleMax.y = p.y + s.y;
	    ligne += 20;
       }
    }

int i = indSpin;
while (panneau->FindWindowById(i, panneau))
    {
	panneau->FindWindowById(i-100,panneau)->Show(false);
    panneau->FindWindowById(i, panneau)->Show(false);
    i++;
    }
i = indCombo;
while (panneau->FindWindowById(i, panneau))
    {
 	panneau->FindWindowById(i-100,panneau)->Show(false);
    panneau->FindWindowById(i, panneau)->Show(false);
    i++;
    }
SetClientSize(tailleMax + wxSize(10, 10));
panneau->Update();
}


FenetreSequenceOperation::~FenetreSequenceOperation()
{
delete []nomEtape;
nomEtape=NULL;
if (osgApp)
	((wxOpencvApp*)osgApp)->FenetreSeqOpe(NULL);

}

void FenetreSequenceOperation::OnActivate(wxActivateEvent& event)
{
}

void FenetreSequenceOperation::OnPaint(wxPaintEvent& event)
{
}

void FenetreSequenceOperation::OnSpinEntier(wxSpinEvent &w)
{
wxOpencvApp *app=(wxOpencvApp *)osgApp;
if (!osgApp)
	return;
int opSelec=w.GetValue();
std::map <int, std::vector <ParametreOperation > >::iterator it = (*seqActif).begin();
for (int i=0;i<opSelec&& it!=(*seqActif).end();i++,it++);
choixOp->Clear();
if (nbEtape<it->second.size())
	{
	nbEtape=(*seqActif)[opSelec].size();
	delete nomEtape;
	nomEtape=new wxString[nbEtape];
	}
int i=0;
for (std::vector <ParametreOperation >::iterator it2 = it->second.begin() ; it2 != it->second.end(); ++it2,++i)
    nomEtape[i]=(*it2).nomOperation;
choixOp->Insert((*it).second.size(),nomEtape,0);
choixOp->SetSelection(0);
InsererCtrlEtape(&(it->second)[0]);

}

void FenetreSequenceOperation::ComboBox(wxCommandEvent &w)
{
    wxOpencvApp *app = (wxOpencvApp *)osgApp;
    if (!osgApp)
        return;
    int indSeq;
    int opSelec;
    if (!IndiceOperation(opSelec))
        return;
    if (!IndiceSequence(indSeq))
        return;
    string nom;


    wxComboBox *cb = ((wxComboBox*)w.GetEventObject());
    ParametreOperation p = (*seqActif)[indSeq][opSelec];
    wxStaticText *st = (wxStaticText*)wxWindow::FindWindowById(w.GetId() - 100, this);
    if (!st)
        throw("wxStaticText undefined");
    if (lienCombo.find(w.GetId()) != lienCombo.end())
        nom = lienCombo[w.GetId()].c_str();
    if (p.intParam.find(nom) != p.intParam.end())
        {

        if (ParametreOperation::listeParam.find(nom) != ParametreOperation::listeParam.end())
            {
            int nb = ((wxComboBox*)(w.GetEventObject()))->GetCurrentSelection();
            int i = 0;
            std::map <string, int  >::iterator iter = p.listeParam[nom].begin();
            for (; iter != p.listeParam[nom].end() && i != nb; ++iter, ++i);
            if (i == nb)
            {
                (*seqActif)[indSeq][opSelec].intParam[nom].valeur = iter->second;
            }
            }
        else
            {
            if (p.intParam[nom].valeur == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
                return;
            (*seqActif)[indSeq][opSelec].intParam[nom].valeur = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();

            }
        }
    if (OperandePresent(&((*seqActif)[indSeq])))
        ExecuterSequence(&((*seqActif)[indSeq]));

}


void FenetreSequenceOperation::OnSpinReel(wxSpinDoubleEvent &w)
{
wxOpencvApp *app=(wxOpencvApp *)osgApp;
if (!osgApp)
	return;
int indSeq;
if (!IndiceSequence(indSeq))
    return;
int indOpe;
if (!IndiceOperation(indOpe))
    return;
string nom;



nom=((wxWindow*)w.GetEventObject())->GetName().c_str();
ParametreOperation p=(*seqActif)[indSeq][indOpe];
wxStaticText *st = (wxStaticText*)wxWindow::FindWindowById(w.GetId() - 100, this);
if (!st)
	throw("wxStaticText undefined");
nom = st->GetLabel().c_str();
if (p.doubleParam.find(nom) != p.doubleParam.end())
{
	if (p.doubleParam[nom].valeur == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	(*seqActif)[indSeq][indOpe].doubleParam[nom].valeur = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
}
if (p.intParam.find(nom) !=p.intParam.end())
{
if (p.intParam[nom].valeur == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
		return;
	(*seqActif)[indSeq][indOpe].intParam[nom].valeur = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
}
if (p.sizeParam.find(nom.substr(0, nom.length() - 2)) != p.sizeParam.end())
{
	if ((w.GetId()) % 4 == 0)
	{
		if (p.sizeParam[nom.substr(0, nom.length() - 2)].valeur.width == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		(*seqActif)[indSeq][indOpe].sizeParam[nom.substr(0, nom.length() - 2)].valeur.width = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
	else
	{
		if (p.sizeParam[nom.substr(0, nom.length() - 2)].valeur.height == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		(*seqActif)[indSeq][indOpe].sizeParam[nom.substr(0, nom.length() - 2)].valeur.height = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
}
if (p.pointParam.find(nom.substr(0, nom.length() - 2)) != p.pointParam.end())
{
	if (nom.substr(nom.length() - 1, 1) == 'x')
	{
		if (p.pointParam[nom.substr(0, nom.length() - 2)].valeur.x == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		(*seqActif)[indSeq][indOpe].pointParam[nom.substr(0, nom.length() - 2)].valeur.x = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
	else
	{
		if (p.pointParam[nom.substr(0, nom.length() - 2)].valeur.y == ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue())
			return;
		(*seqActif)[indSeq][indOpe].pointParam[nom.substr(0, nom.length() - 2)].valeur.y = ((wxSpinCtrlDouble*)(w.GetEventObject()))->GetValue();
	}
}





if (OperandePresent(&((*seqActif)[indSeq])))
	ExecuterSequence(&((*seqActif)[indSeq]));
}


void FenetreSequenceOperation::OnOpeSelec(wxCommandEvent& event)
{
int indSeq;
if (!IndiceSequence(indSeq))
    return;
if (!osgApp)
	return;
wxOpencvApp *app=(wxOpencvApp *)osgApp;
int indOpe=event.GetInt();
if (indOpe<0 || indOpe>=(*seqActif)[indSeq].size())
	return;
InsererCtrlEtape(&((*seqActif)[indSeq][indOpe]));


}

void FenetreSequenceOperation::SelectionFichier(wxCommandEvent& event)
{
if (!osgApp)
	return;
wxOpencvApp *app=(wxOpencvApp *)osgApp;

wxFileDialog ouverture(NULL, "Ouvrir ", wxEmptyString, wxEmptyString, "*.tif;*.jpg;*.bmp;*.png;*.yml;*.gz",wxFD_FILE_MUST_EXIST |wxFD_MULTIPLE );
if (ouverture.ShowModal()!=wxID_OK)
	return;
ouverture.GetPaths( fichierSelectionnes);
int opSelec;
if (!IndiceOperation(opSelec))
    return;
std::map <int,std::vector <ParametreOperation > >::iterator itRef=(*seqActif).begin();
for (int i=0;i<opSelec && itRef!=(*seqActif).end();i++,itRef++);

if (itRef!=(*seqActif).end())
	for (int i=0;i<fichierSelectionnes.size();i++)
		{
		vector<ImageInfoCV*> r;
		std::map <int,std::vector <ParametreOperation > >::iterator it=itRef;
		wxCharBuffer ww=fichierSelectionnes[i].mb_str ();
		char *nomFichier=ww.data() ;
		ImageInfoCV *imIni= new ImageInfoCV(nomFichier);
		ImageInfoCV **im=NULL;
		ImageInfoCV *imTmp=NULL;
		for (std::vector <ParametreOperation > ::iterator it=itRef->second.begin();it!=itRef->second.end();it++)
			{
			ParametreOperation pOCV=*it;
			if (im)
				{
				pOCV.op[0]=im[0];
				if (imTmp)
					delete imTmp;
				imTmp=im[0];
				}
			else
				pOCV.op[0]=imIni;
			pOCV.indOpFenetre[0]=-1;

			r=pOCV.ExecuterOperation();
			}
		delete imIni;
		delete imTmp;
		wxString nom(nomFichier);
		nom=itRef->second.begin()->nomSequence+nom+".yml";
		ww=nom.mb_str ();
		nomFichier=ww.data() ;
		cv::FileStorage fs(nomFichier, cv::FileStorage::WRITE);
        cv::Mat m = r[0]->getMat(cv::ACCESS_READ);
		fs<<"Image"<<m;
		fs.release();

		delete im[0];
		}


}



void  FenetreSequenceOperation::OnSpinPlus(wxSpinEvent& w)
{

}

void  FenetreSequenceOperation::OnSpinMoins(wxSpinEvent& w)
{
}

void FenetreSequenceOperation::OnTextValider(wxCommandEvent &w)
{
}
void FenetreSequenceOperation::OnKeyDown(wxKeyEvent &)
{
}

void FenetreSequenceOperation::OnClose(wxCloseEvent& event)
{
delete []nomEtape;
nomEtape=NULL;
((wxOpencvApp *)osgApp)->FenetreSeqOpe((void*)0xFFFFFFFF);
wxFrame::OnCloseWindow(event);

}

void FenetreSequenceOperation::Executer(wxCommandEvent& event)
{
int opSelec;
if (!IndiceSequence(opSelec))
    return;
if (!osgApp)
	return;
wxOpencvApp *app=(wxOpencvApp *)osgApp;
std::map <int,std::vector <ParametreOperation > >::iterator it=(*seqActif).begin();
for (int i=0;i<opSelec&& it!=(*seqActif).end();i++,it++);
ExecuterSequence(&(it->second));

}

void FenetreSequenceOperation::ExecuterSequence(std::vector <ParametreOperation> *sq)
{
if (!osgApp)
	return;
OperandePresent(sq);
wxOpencvApp	*app=(wxOpencvApp *)osgApp;
ImageInfoCV **im=NULL;
ImageInfoCV *imTmp=NULL;
int i=0;
if (fenMere->Cam() && fenMere->Cam()->IsRunning())
	{
	wxCriticalSectionLocker enter(((FenetrePrincipale*)fenMere)->paramCam);

	fenMere->DefSeqCamera(sq);
	return;
	}

vector<ImageInfoCV*> r;
for (std::vector <ParametreOperation > ::iterator it=sq->begin();it!=sq->end();it++)
	{
	ParametreOperation pOCV=*it;
	wxString  nomOperation(it->nomOperation);
	app->DefOperateurImage(nomOperation);
	int indFen1=fenMere->IdFenetre();
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return ;
		}
	if (r.size()==0)
		{
		pOCV.op[0]=fenMere->ImAcq();
		pOCV.indOpFenetre[0]=indFen1;
		}
	else
		{
		if (imTmp)
			delete imTmp;
		imTmp=r[0];
		pOCV.op[0]=r[0];
        pOCV.indOpFenetre[0] = -1;
		}

	int indFen2=-1;
    if (it->indOpFenetre.size()>=2)
        indFen2=it->indOpFenetre[1];
	if (indFen2>=0)
		{

		pOCV.op[1]=app->Graphique(indFen2)->ImAcq();
		}
//	if (pOCV.intParam.find(
    if (it+1 == sq->end())
        r=app->ExecuterOperation(&pOCV,true);
    else
        r = app->ExecuterOperation(&pOCV, true);

	}
if (r.size()!=0)
	{
	int opSelec;
    if (!IndiceOperation(opSelec))
        opSelec=9999;
	wxCheckBox *wb=(wxCheckBox *)wxWindow::FindWindowById(IND_HYPER-2,panneau);
		
	FenetrePrincipale *f;
	if (wb->GetValue() && idFenetre.find(opSelec)!=idFenetre.end() && app->Graphique(idFenetre[opSelec].second) )
		{
		f=idFenetre[opSelec].first;
		f->AssosierImage(r[0]);

		}
	else
		{
		f = new FenetrePrincipale(NULL, "wxOpenCV",
		wxPoint(0,0), wxSize(530,570),wxCLOSE_BOX|wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxRESIZE_BORDER | wxSYSTEM_MENU | wxCAPTION | wxCLIP_CHILDREN);
		wxString s;
		s.Printf("%d : %s( %d) of image %d ",app->NbFenetre(),_("Sequence"),opSelec,fenMere->IdFenetre());
		f->SetTitle(s);
		f->DefOSGApp(app);
		idFenetre[opSelec] = make_pair (f, f->IdFenetre());
		f->AssosierImage(r[0]);
		app->InitFenAssociee(f);
		f->InitIHM();
		}


	f->NouvelleImage();
	f->MAJNouvelleImage();
	if (f->ImgStat())
		f->ImgStat()->Plot(true);
	f->DefHistorique();
	}
}



bool FenetreSequenceOperation::OperandePresent(std::vector <ParametreOperation> *sq)
{
if (!osgApp)
	return false;
wxOpencvApp	*app=(wxOpencvApp *)osgApp;
ImageInfoCV **im=NULL;
ImageInfoCV *imTmp=NULL;
int i=0;
bool videoActive=false;
if (fenMere->Cam() && fenMere->Cam()->IsRunning())
	videoActive=true;
for (std::vector <ParametreOperation > ::iterator it=sq->begin();it!=sq->end();it++)
	{
	ParametreOperation pOCV=*it;
	wxString  nomOperation(it->nomOperation);
	int indFen1=fenMere->IdFenetre();
	if (indFen1<0 )
		{
		wxMessageBox(_("Previous image is closed?"),_("Problem"), wxOK );
		return false;
		}
	if (!fenMere->ImAcq())
		{
		wxMessageBox(_("Empty image?"),_("Problem"), wxOK );
		return false;
		}
	long indFen2=-1;
    if(it->indOpFenetre.size()>=2)
        indFen2=it->indOpFenetre[1];
	bool annuler=false;
	while (it->operateur && it->nbOperande>=2 && !app->Graphique(indFen2) && !videoActive)
		{
			wxTextEntryDialog  adr( NULL,_("Empty image. Give window id of image"),"0");
			if (adr.ShowModal())
				adr.GetValue().ToCLong(&indFen2);
			else
				annuler=true;
            it->indOpFenetre[1] = indFen2;
		}
	if (annuler)
		return false;
	if (it->op.size()>=2)
		if (it->operateur && indFen2!=-1 && !videoActive && app->Fenetre(indFen2))
			it->op[1]=app->Graphique(indFen2)->ImAcq();
		else
			it->op[1] =NULL;
	}
return true;
}


void  FenetreSequenceOperation::OuvrirSequence(wxCommandEvent& event)
{
if (!osgApp)
return;
wxOpencvApp *app = (wxOpencvApp *)osgApp;

wxFileDialog ouverture(NULL, _("Open Sequence"), wxEmptyString, wxEmptyString, "*.xml", wxFD_FILE_MUST_EXIST );
if (ouverture.ShowModal() != wxID_OK)
    return;
#ifdef __WINDOWS__
wxString nom(ouverture.GetDirectory() + "\\" + ouverture.GetFilename());
#else
wxString nom(ouverture.GetDirectory() + "/" + ouverture.GetFilename());
#endif

cv::FileStorage fs((string)nom.c_str(), cv::FileStorage::READ);
if (!fs.isOpened())
{
    wxMessageBox(_("File cannot be opened"),_("Error"));
    return;
}
int indEtape=0;
seqFichier.clear();
string s("Operation");
do
{
    s="Operation";
    s += to_string(indEtape);
    ParametreOperation p;
    if (!fs[s].empty())
    {
        p.read(fs[s]);
        p.InitPtrFonction();
        seqFichier[0].push_back(p);
    }
    indEtape++;
}
while (!fs[s].empty());
seqActif = &seqFichier;
InsererCtrlEtape(&(seqFichier[0][0]));
wxSpinCtrl *spw = (wxSpinCtrl*) wxWindow::FindWindowById(IND_OPE, panneau);
wxSpinEvent evt;
evt.SetEventObject(spw);
spw->SetRange(0, 0);
spw->SetValue(0);
OnSpinEntier(evt);
GrapheOperation *fenOperation = new GrapheOperation(seqFichier[0], (wxOpencvApp*)osgApp, nom.c_str(), 50, 50, 450, 600);
fenOperation->Show(true);


}

void  FenetreSequenceOperation::BasculeConfig(wxCommandEvent& event)
{
wxCheckBox *c = (wxCheckBox *)event.GetEventObject();
wxButton *b = (wxButton *)wxWindow::FindWindowById(IND_FICHIERS, panneau);
if (!c->GetValue())
{
   b->Enable();
   OuvrirSequence(event);
}
else
{
    b->Disable();
    wxOpencvApp *app = (wxOpencvApp *)osgApp;

    seqActif = app->TabSeqOperation();
    InsererCtrlEtape(&((*seqActif)[0][0]));
    wxSpinCtrl *spw = (wxSpinCtrl*)wxWindow::FindWindowById(IND_OPE, panneau);
    wxWindow::FindWindowById(IND_OPE, panneau);
    wxSpinEvent evt;
    evt.SetEventObject(spw);
    spw->SetRange(0, 0);
    spw->SetValue(0);
    OnSpinEntier(evt);
    }
}

