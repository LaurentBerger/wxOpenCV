#include "FenetrePrincipale.h"
#include "ControleCamera.h"
#include "imagestat.h"
#include <wx/xml/xml.h>
//#include "OutilsImage.h"

#define AJOUTERCHAMP(TypeNoeud1,NomNoeud1,NomAttr,valAttr,TypeNoeud2,Contenu2,pere)\
	n=new wxXmlNode(TypeNoeud1 ,NomNoeud1);\
	n->AddAttribute(NomAttr,valAttr);\
	nt=new wxXmlNode(TypeNoeud2,"");\
	nt->SetContent(Contenu2);\
	n->AddChild(nt);\
	pere->AddChild(n);

/**
Creer un rapport sur image  
*/
void FenetrePrincipale::CreerRapport(wxCommandEvent& evt)
{
if (imAcq==NULL)
	{
	wxMessageBox(_T("Error CreerRapport"), _T("Document is empty"), wxOK );
	return;
	}
// Entete Docbook
wxXmlDocument xml;
wxXmlNode	*racine=new wxXmlNode(wxXML_ELEMENT_NODE,"book");
racine->AddAttribute("version","5.0");
racine->AddAttribute("xml:id","wxOpenCV");
racine->AddAttribute("xml:lang","fr");
racine->AddAttribute("xmlns","http://docbook.org/ns/docbook");
racine->AddAttribute("xmlns:xi","http://www.w3.org/1999/xlink");
racine->AddAttribute("xmlns:ns5","http://www.w3.org/1998/Math/MathML");
racine->AddAttribute("xmlns:ns4","http://www.w3.org/1999/xhtml");
racine->AddAttribute("xmlns:ns3","http://www.w3.org/2000/svg");
racine->AddAttribute("xmlns:ns","http://docbook.org/ns/docbook");
wxString nom=repertoireDuDocument+"/"+nomDuDocument;
nom.Replace("\\","/");
wxString texte="Rapport :"+nomDuDocument+"\r";
xml.SetRoot(racine);
// Titre du livre
wxXmlNode *pageDeTitre=new wxXmlNode(wxXML_ELEMENT_NODE,"info");
wxXmlNode	*n=new wxXmlNode(wxXML_ELEMENT_NODE,"title");
wxXmlNode	*nt=new wxXmlNode(wxXML_TEXT_NODE,texte);
nt->SetContent(texte);
n->AddChild(nt);
pageDeTitre->AddChild(n);
// Photo de couverture
wxXmlNode	*figure=new wxXmlNode(wxXML_ELEMENT_NODE,"figure");
wxXmlNode	*titre=new wxXmlNode(wxXML_ELEMENT_NODE,"title");
figure->AddAttribute("xml:id","Original");
nt=new wxXmlNode(wxXML_TEXT_NODE,"");
nt->SetContent(_("Original"));
titre->AddChild(nt);
figure->AddChild(titre);
wxXmlNode	*media=new wxXmlNode(wxXML_ELEMENT_NODE,"mediaobject");
wxXmlNode	*image=new wxXmlNode(wxXML_ELEMENT_NODE,"imageobject");
nom=repertoireDuDocument+"/"+nomDuDocument;
nom.Replace("\\","/");
image->AddAttribute("fileref",nom);
image->AddAttribute("width","100%");
media->AddChild(image);
figure->AddChild(media);
pageDeTitre->AddChild(figure);
racine->AddChild(pageDeTitre);

// Chapitre description
wxXmlNode	*chapI=new wxXmlNode(wxXML_ELEMENT_NODE,"section");
n=new wxXmlNode(wxXML_ELEMENT_NODE,"title");
nt=new wxXmlNode(wxXML_TEXT_NODE," ");
nt->SetContent("Image");
n->AddChild(nt);
chapI->AddChild(n);
n=new wxXmlNode(wxXML_ELEMENT_NODE,"xsl:param");
nt=new wxXmlNode(wxXML_TEXT_NODE,texte);
n->AddAttribute("name","header.column.widths");
nt->SetContent("1 2 1");
n->AddChild(nt);
chapI->AddChild(n);

AJOUTERCHAMP(wxXML_ELEMENT_NODE,"xsl:template","name","header.content",wxXML_TEXT_NODE,"Rapport wxOpenCV",chapI);
AJOUTERCHAMP(wxXML_ELEMENT_NODE,"xsl:template","name","footer.content",wxXML_TEXT_NODE,nomDuDocument,chapI);


AJOUTERCHAMP(wxXML_ELEMENT_NODE,"para","id","text1",wxXML_TEXT_NODE,texte,chapI);


//docLO.InsererImage((char *)(static_cast<const char*>(nom)),10*imAcq->cols,10*imAcq->rows);

texte="Image :"+nomDuDocument;
AJOUTERCHAMP(wxXML_ELEMENT_NODE,"para","id","text2",wxXML_TEXT_NODE,texte,chapI);
racine->AddChild(chapI);
wxXmlNode	*chapII=new wxXmlNode(wxXML_ELEMENT_NODE,"section");
n=new wxXmlNode(wxXML_ELEMENT_NODE,"title");
nt=new wxXmlNode(wxXML_TEXT_NODE," ");
nt->SetContent("Statistics");
n->AddChild(nt);
chapII->AddChild(n);

wxWindow *w=imgStatIm->OngletStatus();

texte = ((wxStaticText*)w->FindWindow(301))->GetLabel();
texte = "Image :"+texte+"\r";
AJOUTERCHAMP(wxXML_ELEMENT_NODE,"para","id","text3",wxXML_TEXT_NODE,texte,chapII);
texte = ((wxStaticText*)w->FindWindow(303))->GetLabel();
texte = "Dimension et nombre de canaux :"+texte+"\r";
AJOUTERCHAMP(wxXML_ELEMENT_NODE,"para","id","text4",wxXML_TEXT_NODE,texte,chapII);
texte = ((wxStaticText*)w->FindWindow(305))->GetLabel();
texte = "taille du pixel : "+texte+"\r";
AJOUTERCHAMP(wxXML_ELEMENT_NODE,"para","id","text5",wxXML_TEXT_NODE,texte,chapII);
texte = ((wxStaticText*)w->FindWindow(307))->GetLabel();
texte = "Intensité minimale :"+texte+"\r";
AJOUTERCHAMP(wxXML_ELEMENT_NODE,"para","id","text6",wxXML_TEXT_NODE,texte,chapII);
texte = ((wxStaticText*)w->FindWindow(309))->GetLabel();
texte = "Intensité maximale :"+texte+"\r";
AJOUTERCHAMP(wxXML_ELEMENT_NODE,"para","id","text7",wxXML_TEXT_NODE,texte,chapII);


Tableur *t=imgStatIm->TableurHistogramme();
if (!t)
	return;
wxXmlNode *table=new wxXmlNode(wxXML_ELEMENT_NODE,"table");
wxXmlNode *tgroup=new wxXmlNode(wxXML_ELEMENT_NODE,"tgroup");
texte.Printf("%d",t->GetCols()+1);
tgroup->AddAttribute("cols",texte);
tgroup->AddAttribute("align","right");
wxXmlNode *titreCol=new wxXmlNode(wxXML_ELEMENT_NODE,"thead");
wxXmlNode *ligne=new wxXmlNode(wxXML_ELEMENT_NODE,"row");
wxXmlNode *entree=new wxXmlNode(wxXML_ELEMENT_NODE,"entry");
ligne->AddChild(entree);
for (int c=0;c<t->GetCols();c++)
	{
	wxXmlNode *entree=new wxXmlNode(wxXML_ELEMENT_NODE,"entry");
	nt=new wxXmlNode(wxXML_TEXT_NODE,"");
	nt->SetContent(t->GetColLabelValue(c));
	entree->AddChild(nt);
	ligne->AddChild(entree);
	}
titreCol->AddChild(ligne);
tgroup->AddChild(titreCol);
wxXmlNode *corps=new wxXmlNode(wxXML_ELEMENT_NODE,"tbody");

wxString s;
wxString cellule;
char col[3];
int lig=1;
col[0]='A';col[1]=0;;col[2]=0;

for (int l=1;l<=t->GetRows();l++)
	{
	ligne=new wxXmlNode(wxXML_ELEMENT_NODE,"row");
	
	for (int c=0;c<t->GetCols();c++)
		{
		wxXmlNode *entree=new wxXmlNode(wxXML_ELEMENT_NODE,"entry");
		if (c==0)
			s = t->GetRowLabelValue(l-1);
		else	
			s=t->GetCellValue(l-1,c-1);
		nt=new wxXmlNode(wxXML_TEXT_NODE,"");
		nt->SetContent(s);
		entree->AddChild(nt);
		ligne->AddChild(entree);
		}
	corps->AddChild(ligne);
	}
tgroup->AddChild(corps);
table->AddChild(tgroup);
chapII->AddChild(table);
racine->AddChild(chapII);
nom=repertoireDuDocument+"/"+nomDuDocument+".svg";
nom.Replace("\\","/");
figure=new wxXmlNode(wxXML_ELEMENT_NODE,"figure");
titre=new wxXmlNode(wxXML_ELEMENT_NODE,"title");
figure->AddAttribute("xml:id","Histogram");
nt=new wxXmlNode(wxXML_TEXT_NODE,"");
nt->SetContent(_("Histogram"));
titre->AddChild(nt);
figure->AddChild(titre);
media=new wxXmlNode(wxXML_ELEMENT_NODE,"mediaobject");
image=new wxXmlNode(wxXML_ELEMENT_NODE,"imageobject");
image->AddAttribute("fileref",nom);
image->AddAttribute("width","100%");
media->AddChild(image);
figure->AddChild(media);

nom=repertoireDuDocument+"\\"+nomDuDocument+".svg";
imgStatIm->SauverHistogrammeSVG(nom);	

figure=new wxXmlNode(wxXML_ELEMENT_NODE,"figure");
titre=new wxXmlNode(wxXML_ELEMENT_NODE,"title");
figure->AddAttribute("xml:id","Histogram");
nt=new wxXmlNode(wxXML_TEXT_NODE,"");
nt->SetContent(_("Histogram"));
titre->AddChild(nt);
figure->AddChild(titre);
media=new wxXmlNode(wxXML_ELEMENT_NODE,"mediaobject");
image=new wxXmlNode(wxXML_ELEMENT_NODE,"imageobject");
nom.Replace("\\","/");
image->AddAttribute("fileref",nom);
image->AddAttribute("width","100%");
media->AddChild(image);
figure->AddChild(media);
chapII->AddChild(figure);
racine->AddChild(chapII);

wxDateTime horo(wxDateTime::Now  (  ) );

wxString nomRapport;

nomRapport=horo.FormatISOCombined('-');
nomRapport.Replace("-","");
nomRapport.Replace(":","");
nomRapport="rapport"+nomRapport+".xml";
imgStatIm->SauverHistogrammeSVG(nomRapport);	
xml.Save(nomRapport);
wxExecute("wxRapportAOO "+nomRapport);

#ifdef __LIBREOFFICEINSTALL__

int argc=1;
char *argv=" ";
sal_detail_initialize( argc, &argv);

if (imAcq==NULL)
	{
	wxMessageBox(_T("Error CreerRapport"), _T("Document is empty"), wxOK );
	return;
	}
wxString nom=repertoireDuDocument+"\\"+nomDuDocument;
  AOO_LO	docLO;
  StyleAOO_LO s("Times",36, 
 		com::sun::star::awt::FontWeight::BOLD,true,
		com::sun::star::style::ParagraphAdjust::ParagraphAdjust_CENTER,0,
		com::sun::star::style::BreakType::BreakType_PAGE_AFTER);
wxString texte="Rapport :"+nomDuDocument+"\r";
  docLO.LancerWriter();
	docLO.ChangerStylePage("First Page");
	docLO.CreerStyle("MonStyle",s);
    docLO.ChangerStyle("MonStyle");
 	docLO.InsererImage((char *)(static_cast<const char*>(nom)),10*imAcq->cols,10*imAcq->rows);
	docLO.AjouterTexte((char *)(static_cast<const char*>(texte)));
 	docLO.AjouterSignet("tdm");
	texte="Image :"+nomDuDocument;
	docLO.InsererEntete((char *)(static_cast<const char*>(texte)));
	docLO.InsererPiedDePage("Rapport wxOpenCV");

    docLO.ChangerStyle("Heading 1");
    docLO.AjouterTexte( "Image \n\r");
    docLO.ChangerStyle("Heading 2");
    docLO.AjouterTexte("Size\r");
	docLO.ChangerStyle("Standard");

	wxWindow *w=imgStatIm->OngletStatus();
	texte = ((wxStaticText*)w->GetWindowChild(301))->GetLabel();
	texte = "Image :"+texte+"\r";
    docLO.AjouterTexte((char *)(static_cast<const char*>(texte)));
	texte = ((wxStaticText*)w->GetWindowChild(303))->GetLabel();
	texte = "Dimension et nombre de canal :"+texte+"\r";
    docLO.AjouterTexte((char *)(static_cast<const char*>(texte)));
	texte = ((wxStaticText*)w->GetWindowChild(305))->GetLabel();
	texte = "taille du pixel : "+texte+"\r";
    docLO.AjouterTexte((char *)(static_cast<const char*>(texte)));
	texte = ((wxStaticText*)w->GetWindowChild(307))->GetLabel();
	texte = "Intensité minimale :"+texte+"\r";
    docLO.AjouterTexte((char *)(static_cast<const char*>(texte)));
	texte = ((wxStaticText*)w->GetWindowChild(309))->GetLabel();
	texte = "Intensité maximale :"+texte+"\r";
    docLO.AjouterTexte((char *)(static_cast<const char*>(texte)));


	docLO.SautDePage();

    docLO.ChangerStyle("Heading 2");
    docLO.AjouterTexte("Statistic\r");
	docLO.InsererTableau(imgStatIm->TableurHistogramme());

	nom=repertoireDuDocument+"\\"+nomDuDocument+".svg";
	imgStatIm->SauverHistogrammeSVG(nom);	
	docLO.pImage.ancrage=com::sun::star::text::TextContentAnchorType::TextContentAnchorType_AS_CHARACTER;
	docLO.InsererImage((char *)(static_cast<const char*>(nom)),8000,8000);
	docLO.AtteindreSignet("tdm");
	docLO.TableDesMatieres();
	docLO.NumeroterPage();
	docLO.SautDePage();
	docLO.MAJDocument();
	docLO.ExportPDF("file:///c:/tmp/testPDF.pdf");
	//docLO.ListerProprietesFiltre();
	docLO.SauverDocument("file:///c:/tmp/test.doc","MS Word 97");
	docLO.SauverDocument("file:///c:/tmp/test.odt","");
	docLO.InfoDocument();
#endif

}

