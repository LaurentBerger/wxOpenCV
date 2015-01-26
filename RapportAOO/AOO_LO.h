#ifndef __AOO_LO_CLASS__
#define __AOO_LO_CLASS__
#include <stdio.h>
#include <wchar.h>

#include <cppuhelper/bootstrap.hxx>

#include <osl/file.hxx>
#include <osl/process.h>
#include <rtl/process.h>

#include <com/sun/star/beans/XPropertySet.hpp>
#include <com/sun/star/bridge/XUnoUrlResolver.hpp>
#include <com/sun/star/frame/XComponentLoader.hpp>
#include <com/sun/star/frame/XDispatchHelper.hpp>
#include <com/sun/star/lang/XMultiComponentFactory.hpp>
#include <com/sun/star/registry/XSimpleRegistry.hpp>
#include <com/sun/star/drawing/XDrawPagesSupplier.hpp>
#include <com/sun/star/container/XNamed.hpp>
#include <com/sun/star/lang/XMultiServiceFactory.hpp>
#include <com/sun/star/frame/XStorable.hpp>
#include <com/sun/star/drawing/XShape.hpp>
#include <com/sun/star/drawing/XShapes.hpp>
#include <com/sun/star/graphic/XGraphicProvider.hpp>
	
#include <com/sun/star/document/UpdateDocMode.hpp>
#include <com/sun/star/text/XTextDocument.hpp>
#include <com/sun/star/text/XText.hpp>
#include <com/sun/star/text/XTextField.hpp>
#include <com/sun/star/text/XTextFrame.hpp>
#include <com/sun/star/text/XTextTable.hpp>
#include <com/sun/star/text/XTextGraphicObjectsSupplier.hpp>

#include <com/sun/star/text/PageNumberType.hpp>
#include <com/sun/star/style/NumberingType.hpp>
#include <com/sun/star/util/Date.hpp>
#include <com/sun/star/util/Time.hpp>
#include <com/sun/star/util/NumberFormat.hpp>
#include <com/sun/star/util/XRefreshable.hpp>

#include <com/sun/star/text/XBookmarksSupplier.hpp>
#include <com/sun/star/text/XBookmarkInsertTool.hpp>
#include <com/sun/star/text/XFootnote.hpp>			// Pied de page
#include <com/sun/star/text/XFootnotesSupplier.hpp>			// Pied de page
#include <com/sun/star/text/XDocumentIndex.hpp>	// Pour la table des matières
#include <com/sun/star/text/XDocumentIndexMark.hpp>	// Pour la table des matières
//#include <com/sun/star/text/ContentIndex.hpp>	// Pour la table des matières
#include <com/sun/star/awt/XStyleSettingsSupplier.hpp>
#include <com/sun/star/table/XTableRows.hpp>		// Pour les tableaux
#include <com/sun/star/container/XIndexAccess.hpp>	// Pour les tableaux
#include <com/sun/star/container/XNameContainer.hpp>// Pour les tableaux

#include <string.h>

// Pour les styles
#include <com/sun/star/style/PageStyleLayout.hpp>
#include <com/sun/star/style/XStyleFamiliesSupplier.hpp>
#include <com/sun/star/style/XStyleSupplier.hpp>
#include <com/sun/star/awt/FontFamily.hpp>
#include <com/sun/star/awt/FontWeight.hpp>
#include <com/sun/star/style/BreakType.hpp>
#include <com/sun/star/style/ParagraphAdjust.hpp>
//#include <com/sun/star/style/XPageStyle.hpp>

#include <com\sun\star\io\xinputstream.hpp>
#include <com/sun/star/text/ControlCharacter.hpp>
#include <com/sun/star/text/TextContentAnchorType.hpp>
// Orientation de l'élément par rapport page/paragraphe/caractère
#include <com/sun/star/text/RelOrientation.hpp>
#include <com/sun/star/text/HoriOrientation.hpp>
#include <com/sun/star/text/VertOrientation.hpp>

#include <com/sun/star/document/XDocumentPropertiesSupplier.hpp>
#include <com/sun/star/document/XDocumentProperties.hpp>


using namespace rtl;
using namespace com::sun::star::uno;
using namespace com::sun::star::lang;
using namespace com::sun::star::beans;
using namespace com::sun::star::bridge;
using namespace com::sun::star::frame;
using namespace com::sun::star::registry;
using namespace com::sun::star::drawing;
using namespace cppu;
using namespace com::sun::star::awt;
using namespace com::sun::star::graphic;
using namespace com::sun::star::document;
using namespace com::sun::star::style;

using namespace com::sun::star::text;
using namespace com::sun::star::table;
using namespace com::sun::star::container;
using namespace com::sun::star::io;
using namespace com::sun::star::awt;

#include <string>
#include <wx/xml/xml.h>

//using namespace std;

    // handy cause OUStrings are very often used in the API
    #define createStr(x)            (OUString::createFromAscii(x)) 

class PositionImageAOO_LO {
public :
short margeGauche;
short margeDroite;
short margeBas;
short margeHaut;
short typeOrientationVertical;
short typeOrientationHorizontal;
short typeOrientationHorizontalRelative;
short typeOrientationVerticalRelative;
short ancrage;
int	positionX;
int positionY;

PositionImageAOO_LO(){Init();};
void Init(void);
};



class StyleAOO_LO {
public :
std::string	police;
float	tailleCar;		// Hauteur de la police
float	poidsCar;		// Gras, italique : com.sun.star.awt.FontWeight.BOLD...
int		alignement;		// Ajustement du paragraphe : com.sun.star.style.ParagraphAdjust.CENTER_value
bool    crenage;		// Espacement entre caractères
int		retPremLigne;	// Retrait première ligne
int		breakType;		// com.sun.star.style.BreakType.PAGE_AFTER 
StyleAOO_LO(char *s,float t ,float p ,int a,bool c,int r1,int b)
{
police=s;
tailleCar=t;
poidsCar=p;
alignement=a;
crenage=c;
retPremLigne=r1;
breakType=b;
};
StyleAOO_LO()
{
police = "Times";tailleCar=12;
poidsCar=com::sun::star::awt::FontWeight::BOLD;
crenage=true;
alignement=com::sun::star::style::ParagraphAdjust::ParagraphAdjust_CENTER;
retPremLigne=0;
breakType=com::sun::star::style::BreakType::BreakType_PAGE_AFTER;
};

};


class AOO_LO {
private :
void Init(){
xComponentContext=NULL;rServiceManager=NULL;xComponentLoader=NULL;
composantWriter=NULL;xTextDoc=NULL;xCurseurTexte=NULL;};
public :
AOO_LO(void);
~AOO_LO(void){     composantWriter->dispose();};
void Connection();
void LancerWriter();
// Texte
void AjouterTexte(char * ,int pos=-1);
void AjouterSignet(char *);
void NumeroterPage();
void AtteindreSignet(char *);
void ChangerStyle(char *styleName);
void ChangerStylePage(char *styleName);
void CreerStyle(char *styleName,StyleAOO_LO &s);
void InsererNoteDePage(char *);
void InsererEntete(char *);
void InsererPiedDePage(char * );
void InsererCadre (char *texte, int largeur,int hauteur);
void SautDePage() ;

void SauverDocument(char *nom,char *filtre);
void ExportPDF(char *nom);
void InfoDocument();

// Image
void InsererImage(char *nomFichier,int largeur,int hauteur);
void InsererImage2(char *);
void InsererImageLien(char *);
// Lister proprietes
void ListerProprietes(Reference< XPropertySet> x);
void ListerProprietesCurseur();
void ListerProprietesTDM(Reference<XDocumentIndex> xTdm);
void ListerStyle();
void ListerProprietesDoc();
void ListerProprietesFiltre();

// Manipulationde tableau
Reference <XTextTable> InsererTableau(int nbLig,int nbCol);
void InsererTableau(void *t);
void ChangerCouleurLigne(Reference <XTextTable> xTable,int indLig,int rgb);
void ChangerCouleurTableau(Reference <XTextTable> xTable,int rgb);
void ChangerCouleurTexteCellule(Reference <XTextTable> ,char *ref,int rgb);
void ChangerCouleurFondCellule(Reference <XTextTable> ,char *ref,int rgb);

void DefFormuleCellule(Reference <XTextTable> ,char *ref,char *formule);
void DefValeurCellule(Reference <XTextTable> ,char *ref,char *valeur);
void DefValeurCellule(Reference <XTextTable> ,char *ref,int);

// Table des matières
void TableDesMatieres();
void MAJDocument();

void InsererPageDeTitre(wxXmlNode *);
void InsererChapitre(wxXmlNode *);

PositionImageAOO_LO pImage;

private :
wxString	entete;
wxString	pied;
Reference< XComponentContext >		xComponentContext;
Reference< XMultiComponentFactory > rServiceManager;
Reference< XInterface  > bureau;
Reference < XComponentLoader >		xComponentLoader;
Reference<XMultiServiceFactory>		aoo_loMSF;

// Pour writer
Reference< XComponent >composantWriter;
Reference < XTextDocument > xTextDoc;
Reference< XText > contenuTexte;
Reference< XTextCursor> xCurseurTexte;
Reference<XPropertySet> xPropTableDesMatieres;

};
#endif


