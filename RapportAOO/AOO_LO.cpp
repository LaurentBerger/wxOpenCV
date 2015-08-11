#include "AOO_LO.h"

#include <sal/main.h>
#include <iostream>

#include <wx/xml/xml.h>
#include <wx/image.h> 

using namespace std;

void PositionImageAOO_LO::Init(void){
typeOrientationVertical= com::sun::star::text::VertOrientation::CENTER;
typeOrientationHorizontal = com::sun::star::text::HoriOrientation::CENTER;
typeOrientationHorizontalRelative = com::sun::star::text::RelOrientation::PAGE_FRAME;
typeOrientationVerticalRelative = com::sun::star::text::RelOrientation::PAGE_FRAME;

margeGauche = 1000;
margeDroite = 1000;
margeBas = 1000;
margeHaut =1000;
positionX=3000;
positionY=3000;
ancrage=com::sun::star::text::TextContentAnchorType::TextContentAnchorType_AT_PAGE;
};


AOO_LO::AOO_LO()
{
Init();
Connection();
}

void AOO_LO::Connection()
{
//
// Biblio : https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer
// Biblio : https://forum.openoffice.org/en/forum/viewtopic.php?f=20&t=53902
//
try 
	{

	xComponentContext = bootstrap();
	}
catch (Exception e)
	{ 
	int i=0; 
	} 

// gets the service manager from the office
rServiceManager =   xComponentContext->getServiceManager();
bureau=rServiceManager->createInstanceWithContext(OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.frame.Desktop") ),
                xComponentContext );
// Creates an instance of a component which supports the services specified by the factory
//Reference < XComponentLoader > componentLoader(rServiceManager->createInstanceWithContext(OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.frame.Desktop") ),
//                xComponentContext ), UNO_QUERY );
Reference < XComponentLoader > componentLoader(bureau, UNO_QUERY );

xComponentLoader  = componentLoader;
}

void AOO_LO::LancerWriter()
{
//
// Biblio : https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer
//
Sequence<PropertyValue> loadProps(2);
loadProps[0].Name = OUString::createFromAscii("Hidden");
loadProps[0].Value = Any(false);
loadProps[1].Name = OUString::createFromAscii("UpdateDocMode");
loadProps[1].Value = Any(sal_Int16(com::sun::star::document::UpdateDocMode::FULL_UPDATE));

Reference< XComponent > xComponent = xComponentLoader->loadComponentFromURL(
            createStr("private:factory/swriter"), OUString(RTL_CONSTASCII_USTRINGPARAM("_blank")),
            0, loadProps );
composantWriter =xComponent;
Reference < XTextDocument > xTD (composantWriter,UNO_QUERY);
xTextDoc = xTD;
Reference< XText > xText = xTextDoc->getText();
contenuTexte = xText;
Reference< XTextCursor> xTextCursor = xText->createTextCursor();
xCurseurTexte = xTextCursor; 
Reference<XMultiServiceFactory> oDocMSF (xTextDoc,UNO_QUERY);
aoo_loMSF=oDocMSF;


}

void AOO_LO::NumeroterPage()
{
//
// Biblio http://api.libreoffice.org/examples/java/Text/TextDocuments.java
//
Reference<XTextField> xNumPage(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextField.PageNumber" ))), UNO_QUERY); 
Reference<XTextField> xNumTotPage(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextField.PageCount" ))), UNO_QUERY); 
Reference<XTextField> xDatePage(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextField.DateTime" ))), UNO_QUERY); 
Reference<XTextField> xHeurePage(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextField.DateTime" ))), UNO_QUERY); 
Reference<XPropertySet> propNum(xNumPage,UNO_QUERY);
Reference<XPropertySet> propDate(xDatePage,UNO_QUERY);
Reference<XPropertySet> propHeure(xHeurePage,UNO_QUERY);
Reference<XPropertySet> propTotNum(xNumTotPage,UNO_QUERY);
ListerProprietes(propDate);
propNum->setPropertyValue("SubType",::Any(sal_Int16(com::sun::star::text::PageNumberType::PageNumberType_CURRENT)));
propNum->setPropertyValue("NumberingType",::Any((com::sun::star::style::NumberingType::ARABIC)));
propTotNum->setPropertyValue("NumberingType",::Any((com::sun::star::style::NumberingType::ARABIC)));

propDate->setPropertyValue("IsDate",::Any(sal_Bool(true)));
propHeure->setPropertyValue("IsDate",::Any(sal_Bool(false)));
Reference <XInterface> xIntStyle (aoo_loMSF->createInstance(createStr("com.sun.star.style.PageStyle")),UNO_QUERY);

Reference< XStyleFamiliesSupplier > xSupplier(xTextDoc,UNO_QUERY);
Reference<XNameAccess> xNameAcess=xSupplier->getStyleFamilies();
Reference<XNameContainer> xPageStyle(xNameAcess->getByName("PageStyles"),UNO_QUERY);
Reference<XStyle> xst(xPageStyle->getByName("Standard"),UNO_QUERY);
Reference<XPropertySet> propSt(xst,UNO_QUERY);
Reference<XText> headerText(propSt->getPropertyValue("FooterText"),UNO_QUERY);

Reference<XTextCursor> xx=headerText->createTextCursor();
xx->gotoStart(false);
headerText->insertString(xx,OUString ("\t", (sal_Int32)strlen("\t"),RTL_TEXTENCODING_ISO_8859_1 ),true);
xx->gotoStart(false);
headerText->insertTextContent(xx,xHeurePage,true);
xx->gotoStart(false);
headerText->insertString(xx,OUString (" ", (sal_Int32)strlen(" "),RTL_TEXTENCODING_ISO_8859_1 ),true);
xx->gotoStart(false);
headerText->insertTextContent(xx,xDatePage,true);
xx->gotoEnd(false);

headerText->insertString(xx,OUString ("\t", (sal_Int32)strlen("\t"),RTL_TEXTENCODING_ISO_8859_1 ),true);
xx->gotoEnd(false);

headerText->insertTextContent(xx,xNumPage,true);
xx->gotoEnd(false);
headerText->insertString(xx,OUString ("/", (sal_Int32)strlen("/"),RTL_TEXTENCODING_ISO_8859_1 ),true);
xx->gotoEnd(false);
headerText->insertTextContent(xx,xNumTotPage,true);
}




void AOO_LO::AjouterSignet(char *signet)
{
//
// Biblio http://api.libreoffice.org/examples/java/Text/BookmarkInsertion.java
//

Reference <XNamed> xBook(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.Bookmark" ))), UNO_QUERY );
Reference< XBookmarksSupplier> xBookmarksSupplier (xTextDoc,UNO_QUERY);
Reference< XNamed> xNameAccess(xBook,UNO_QUERY);
xNameAccess->setName(createStr(signet));
Reference< XTextContent >xTextContent (xBook,UNO_QUERY);
Reference<XTextRange> xtr(xCurseurTexte,UNO_QUERY);
contenuTexte->insertTextContent(xtr, xTextContent, true);
}

void AOO_LO::AtteindreSignet(char *signet)
{
//
// Biblio https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer#Acc.C3.A9der_.C3.A0_un_signet
//
Reference< XBookmarksSupplier> xBookmarksSupplier (xTextDoc,UNO_QUERY);
Reference< XNameAccess > xNameAccess = xBookmarksSupplier->getBookmarks();

Any bookmark = xNameAccess->getByName(OUString::createFromAscii(signet));
 
Reference< XTextContent >xTextContent (bookmark,UNO_QUERY);
Reference< XTextRange > xTextRange = xTextContent->getAnchor();
//Reference< XText > xText = xTextRange2->getText() ; 
xCurseurTexte = xTextRange->getText()->createTextCursorByRange( xTextRange)  ; 


}


void AOO_LO::AjouterTexte(char *texte,int pos)
{
//
// Biblio : https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer
//
if (pos>=0)
	xCurseurTexte->goRight(pos,0); // added to move the cursor
contenuTexte->insertString(xCurseurTexte,OUString (texte, (sal_Int32)strlen(texte),RTL_TEXTENCODING_ISO_8859_1 ),false);      // save file

}

void AOO_LO::TableDesMatieres()
{



//
// Biblio https://forum.openoffice.org/en/forum/viewtopic.php?f=44&t=31839
// http://api.libreoffice.org/examples/DevelopersGuide/Text/TextDocuments.java
Reference<XPropertySet> xProp(aoo_loMSF->createInstance(OUString(RTL_CONSTASCII_USTRINGPARAM(("com.sun.star.text.ContentIndex")))),UNO_QUERY);
//Reference<XDocumentIndex> xTdm(aoo_loMSF->createInstance(OUString(RTL_CONSTASCII_USTRINGPARAM(("com.sun.star.text.ContentIndex")))),UNO_QUERY);
//Reference<XPropertySet> xProp( xTdm,UNO_QUERY);
char *texte="Table des matières";
xProp->setPropertyValue("Title",Any(OUString (texte, (sal_Int32)strlen(texte),RTL_TEXTENCODING_ISO_8859_1 )));
xProp->setPropertyValue("Level",Any(sal_Int16(5)));
//xProp->setPropertyValue("ParaStyleLevel1",Any(createStr("Heading 1")));
//xProp->setPropertyValue("ParaStyleLevel2",Any(createStr("Heading 2")));
//xProp->setPropertyValue("ParaStyleLevel3",Any(createStr("Heading 3")));
//xProp->setPropertyValue("CreateFromLevelParagraphStyles",Any(sal_Bool(false)));
//xProp->setPropertyValue("CreateFromChapter",Any(sal_Bool(true)));
xProp->setPropertyValue("CreateFromMarks",Any(sal_Bool(false)));
xProp->setPropertyValue("CreateFromOutline",Any(sal_Bool(true)));
xProp->setPropertyValue("LevelFormat",Any(sal_Bool(true)));
xProp->setPropertyValue("IsProtected",Any(sal_Bool(false)));

Reference<XTextContent> xTdmContenu(xProp,UNO_QUERY);
contenuTexte->insertTextContent(xCurseurTexte,xTdmContenu,false);
Reference<XDocumentIndex> xTdm(xProp,UNO_QUERY);
//xTdm->update();
xPropTableDesMatieres=xProp;
ListerProprietesTDM(xTdm);

}

void AOO_LO::MAJDocument()
{
//
// Biblio http://scenari-platform.org/svn/dev-core/projects/ifcam/Jav_Transform/src/com/scenari/s/co/transform/oo/impl/TransformerUpdate.java
// Biblio http://markmail.org/message/kjzos3fmjmcvxbmc
//
Reference<com::sun::star::util::XRefreshable> refreshable(composantWriter,UNO_QUERY); 
refreshable->refresh(); 

Reference<XDocumentIndex> xTdm(xPropTableDesMatieres,UNO_QUERY);
xTdm->update();
_sleep(1000);

Reference< XDispatchHelper > rDispatchHelper(rServiceManager->createInstanceWithContext(OUString( RTL_CONSTASCII_USTRINGPARAM("com.sun.star.frame.DispatchHelper") ),xComponentContext ),UNO_QUERY);
Reference< XFrame > rFrame(bureau,UNO_QUERY);
Reference< XDispatchProvider > rDispatchProvider(rFrame,UNO_QUERY);
Sequence<PropertyValue> args(0);
	rDispatchHelper->executeDispatch(rDispatchProvider,
			OUString::createFromAscii(".uno:UpdateAll"),
			OUString::createFromAscii(""),0,args);
_sleep(1000);
	rDispatchHelper->executeDispatch(rDispatchProvider,
			OUString::createFromAscii(".uno:UpdateAllIndexes"),
			OUString::createFromAscii(""),0,args);
_sleep(1000);
	rDispatchHelper->executeDispatch(rDispatchProvider,
			OUString::createFromAscii(".uno:UpdateFields"),
			OUString::createFromAscii(""),0,args);
}


void AOO_LO::InsererNoteDePage(char *texte)
{
//
// Biblio http://api.libreoffice.org/examples/DevelopersGuide/Text/TextDocuments.java
//
Reference <XFootnote> xNote (aoo_loMSF->createInstance(OUString(RTL_CONSTASCII_USTRINGPARAM(("com.sun.star.text.Footnote")))),UNO_QUERY);
xNote->setLabel(OUString ("texte annoté", (sal_Int32)strlen("texte annoté"),RTL_TEXTENCODING_ISO_8859_1 ));
Reference<XTextContent> xt(xNote,UNO_QUERY);
contenuTexte->insertTextContent(xCurseurTexte,xt,false);
Reference<XFootnotesSupplier> xFootnoteSupplier(xTextDoc,UNO_QUERY );
Reference<XIndexAccess> xFootnotes(xFootnoteSupplier->getFootnotes(),UNO_QUERY );
Reference<XFootnote> xn(xFootnotes->getByIndex(0),UNO_QUERY );
Reference<XSimpleText>lanote(xn,UNO_QUERY);
Reference<XTextRange> xtr(lanote->createTextCursor(),UNO_QUERY);
lanote->insertString(xtr,OUString (texte, (sal_Int32)strlen(texte),RTL_TEXTENCODING_ISO_8859_1 ),false);

}



void AOO_LO::ListerStyle()
{
// Bilio
// http://api.libreoffice.org/examples/java/Text/StyleInitialization.java
//
Reference< XStyleFamiliesSupplier > xSupplier(xTextDoc,UNO_QUERY);
Reference<XNameAccess> xNameAcess=xSupplier->getStyleFamilies();
//Reference<XNameContainer> xParaStyleCollection(xNameAcess->getByName("ParagraphStyles"),UNO_QUERY);
Reference<XNameContainer> xParaStyleCollection(xNameAcess->getByName("PageStyles"),UNO_QUERY);
Sequence<OUString>s=xParaStyleCollection->getElementNames();
for (int i=0;i<s.getLength();i++)
	{
	Any p=xParaStyleCollection->getByName(s[i]);
	OUString pp;
	Reference<XStyle> xStyle(xParaStyleCollection->getByName(s[i]),UNO_QUERY);

	
	OString str=OUStringToOString(xStyle->getName() , RTL_TEXTENCODING_ASCII_US );
	
	printf("%d-> %s\n",i,str.pData->buffer);


	}

}

void AOO_LO::ListerProprietes(Reference< XPropertySet> x)
{
// Biblio
// https://wiki.openoffice.org/w/index.php?title=Documentation/DevGuide/ProUNO/Properties
//
Reference<XPropertySetInfo> s(x->getPropertySetInfo());
Sequence< Property > p(s->getProperties());
Property a;
for (int i=0;i<p.getLength();i++)
	{
	a = (p.getArray())[i];
	OString str=OUStringToOString( a.Name, RTL_TEXTENCODING_ASCII_US );
	printf("%d-> %s ",i,str.pData->buffer);
	str=OUStringToOString( a.Type.getTypeName(), RTL_TEXTENCODING_ASCII_US ); 
	
	printf("(type:%s)\n",str.pData->buffer);
	}
}



void AOO_LO::ListerProprietesCurseur()
{

// Biblio OUString
// https://wiki.openoffice.org/wiki/FR/Documentation/LanguageCpp#Conversion_OUString_en_OString
// Biblio Sequence
// https://wiki.openoffice.org/wiki/FR/Documentation/LanguageCpp#Les_Sequences_.28suite.29

Reference< XPropertySet> xCursorProps( xCurseurTexte,UNO_QUERY);
printf("Propriété Curseur\n");
ListerProprietes(xCursorProps);

}

void AOO_LO::ListerProprietesDoc()
{

// Biblio OUString
// https://wiki.openoffice.org/wiki/FR/Documentation/LanguageCpp#Conversion_OUString_en_OString
// Biblio Sequence
// https://wiki.openoffice.org/wiki/FR/Documentation/LanguageCpp#Les_Sequences_.28suite.29

Reference< XPropertySet> xCursorProps( contenuTexte,UNO_QUERY);
//		Reference < XTextDocument > xTextDocument (xComponent,UNO_QUERY);
printf("Propriété XTEXT\n");
ListerProprietes(xCursorProps);
}

void AOO_LO::ListerProprietesFiltre()
{

Reference<XStorable> rStore(composantWriter, UNO_QUERY);

Reference< XPropertySet> xCursorProps( rStore,UNO_QUERY);
printf("Propriété Filtre\n");
ListerProprietes(xCursorProps);
}



void AOO_LO::ListerProprietesTDM(Reference<XDocumentIndex> xTdm)
{

// Biblio OUString
// https://wiki.openoffice.org/wiki/FR/Documentation/LanguageCpp#Conversion_OUString_en_OString
// Biblio Sequence
// https://wiki.openoffice.org/wiki/FR/Documentation/LanguageCpp#Les_Sequences_.28suite.29

Reference< XPropertySet> xCursorProps( xTdm,UNO_QUERY);
//		Reference < XTextDocument > xTextDocument (xComponent,UNO_QUERY);
printf("Propriété TDM\n");
ListerProprietes(xCursorProps);
}

void AOO_LO::InsererEntete(char *texte )
{
// Bilio
// https://joinup.ec.europa.eu/svn/ooo-xadessig-it/branches/oooconf2009-demo/oxsit-sig_opt_uno/test/it/plio/ext/oxsit/test/ooo/TestWriterReportCreation.java
//
Reference <XInterface> xIntStyle (aoo_loMSF->createInstance(createStr("com.sun.star.style.PageStyle")),UNO_QUERY);

Reference< XStyleFamiliesSupplier > xSupplier(xTextDoc,UNO_QUERY);
Reference<XNameAccess> xNameAcess=xSupplier->getStyleFamilies();
Reference<XNameContainer> xPageStyle(xNameAcess->getByName("PageStyles"),UNO_QUERY);
Reference<XStyle> xst(xPageStyle->getByName("Standard"),UNO_QUERY);
Reference<XPropertySet> prop(xst,UNO_QUERY);
printf("StylePage\n");
ListerProprietes(prop);
prop->setPropertyValue("HeaderIsOn",::Any(sal_Bool(true)));
Reference<XText> headerText(prop->getPropertyValue("HeaderText"),UNO_QUERY);


headerText->setString(OUString (texte, (sal_Int32)strlen(texte),RTL_TEXTENCODING_ISO_8859_1 ));
//xParaStyleCollection->insertByName( createStr(nomStyle), makeAny(prop) );
}

void AOO_LO::InsererPiedDePage(char *texte )
{
// Bilio
// https://joinup.ec.europa.eu/svn/ooo-xadessig-it/branches/oooconf2009-demo/oxsit-sig_opt_uno/test/it/plio/ext/oxsit/test/ooo/TestWriterReportCreation.java
//
Reference <XInterface> xIntStyle (aoo_loMSF->createInstance(createStr("com.sun.star.style.PageStyle")),UNO_QUERY);

Reference< XStyleFamiliesSupplier > xSupplier(xTextDoc,UNO_QUERY);
Reference<XNameAccess> xNameAcess=xSupplier->getStyleFamilies();
Reference<XNameContainer> xPageStyle(xNameAcess->getByName("PageStyles"),UNO_QUERY);
Reference<XStyle> xst(xPageStyle->getByName("Standard"),UNO_QUERY);
Reference<XPropertySet> prop(xst,UNO_QUERY);
prop->setPropertyValue("FooterIsOn",::Any(sal_Bool(true)));
Reference<XText> headerText(prop->getPropertyValue("FooterText"),UNO_QUERY);
//Reference <XTextField> numPage(aoo_loMSF->createInstance(createStr("com.sun.star.text.TextField.PageNumber")),UNO_QUERY);
//headerNumPage->insertTextContent();
headerText->setString(OUString (texte, (sal_Int32)strlen(texte),RTL_TEXTENCODING_ISO_8859_1 ));
//xParaStyleCollection->insertByName( createStr(nomStyle), makeAny(prop) );
}

void AOO_LO::CreerStyle(char *nomStyle,StyleAOO_LO &s)
{
// Bilio
// http://api.libreoffice.org/examples/java/Text/StyleCreation.java
//
Reference <XInterface> xIntStyle (aoo_loMSF->createInstance(createStr("com.sun.star.style.ParagraphStyle")),UNO_QUERY);

Reference< XStyleFamiliesSupplier > xSupplier(xTextDoc,UNO_QUERY);
Reference<XNameAccess> xNameAcess=xSupplier->getStyleFamilies();
Reference<XNameContainer> xParaStyleCollection(xNameAcess->getByName("ParagraphStyles"),UNO_QUERY);
Reference<XPropertySet> prop(xIntStyle,UNO_QUERY);
prop->setPropertyValue("CharFontName",::Any(createStr(s.police.c_str())));
prop->setPropertyValue("CharHeight",::Any((s.tailleCar)));
prop->setPropertyValue("CharWeight",::Any(s.poidsCar));
prop->setPropertyValue("CharAutoKerning",::Any(sal_Bool( s.crenage)));
prop->setPropertyValue("ParaAdjust",::Any(sal_Int32 (s.alignement)));
prop->setPropertyValue("ParaFirstLineIndent",::Any(sal_Int32(s.retPremLigne)));
prop->setPropertyValue("BreakType",::Any(sal_Int32(s.breakType)));


xParaStyleCollection->insertByName( createStr(nomStyle), makeAny(prop) );
}

void AOO_LO::ChangerStylePage(char *styleName) 
{
//
// Biblio https://forum.openoffice.org/en/forum/viewtopic.php?f=44&t=68055&p=303360&hilit=style#p303360
// https://www.openoffice.org/api/docs/common/ref/com/sun/star/style/ParagraphStyle.html#PageDescName
//
Reference< XPropertySet> xCursorProps( xCurseurTexte,UNO_QUERY);
rtl::OUString aPageStyle;

xCursorProps->setPropertyValue("PageDescName", ::Any(createStr( styleName)));
xCursorProps->getPropertyValue("PageStyleName")>>= aPageStyle;


}


void AOO_LO::ChangerStyle(char *styleName) 
{
//
// Biblio https://forum.openoffice.org/en/forum/viewtopic.php?f=44&t=68055&p=303360&hilit=style#p303360
//
Reference< XPropertySet> xCursorProps( xCurseurTexte,UNO_QUERY);
xCursorProps->setPropertyValue("ParaStyleName", ::Any(createStr( styleName)));

}

void AOO_LO::SautDePage() 
{
//
// Biblio https://forum.openoffice.org/en/forum/viewtopic.php?f=44&t=68055&p=303360&hilit=style#p303360
//
Reference< XPropertySet> xCursorProps( xCurseurTexte,UNO_QUERY);
xCursorProps->setPropertyValue("BreakType", ::Any(com::sun::star::style::BreakType::BreakType_PAGE_BEFORE));
}

void AOO_LO::ExportPDF(char *nom)
{
//
// Biblio : https://forum.openoffice.org/en/forum/viewtopic.php?f=44&t=71217
//
Reference<XStorable> rStore(composantWriter, UNO_QUERY);

Sequence<PropertyValue> storeProps(2);
storeProps[0].Name = createStr("CompressionMode");
storeProps[0].Value = Any(createStr("1"));
storeProps[1].Name = createStr("PageRange");
storeProps[1].Value = Any(createStr("1-"));

Sequence<PropertyValue> filterOpts(2);


filterOpts[0].Name = "FilterName";
filterOpts[0].Value = Any(createStr("writer_pdf_Export")); 
filterOpts[1].Name = "Overwrite";
filterOpts[1].Value = Any(sal_Bool(true)); 

/*filterOpts[1].Name = "Overwrite";
filterOpts[1].Value = Any(sal_Bool(true));
filterOpts[2].Name = "FilterData";
filterOpts[2].Value = Any(storeProps[0]);*/
rStore->storeToURL(createStr(nom), filterOpts);

}


void AOO_LO::SauverDocument(char *nom,char *filtre)
{
//
// Biblio : https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer
//
Reference<XStorable> rStore(composantWriter, UNO_QUERY);
Sequence<PropertyValue> storeProps(1);
storeProps[0].Name = createStr("FilterName");
storeProps[0].Value = Any(createStr(filtre));
//rStore->storeAsURL(createStr(nom), Sequence < ::com::sun::star::beans::PropertyValue >());
rStore->storeToURL(createStr(nom), storeProps);

}

void AOO_LO::InsererCadre (char *texte, int largeur,int hauteur) 
{ 
//
// Biblio : https://wiki.openoffice.org/wiki/Documentation/DevGuide/Text/Text_Frames
//
try { 
	Reference <XTextFrame> xFrame (aoo_loMSF->createInstance(createStr("com.sun.star.text.TextFrame")),UNO_QUERY);
	Reference < XShape> xShape (xFrame,UNO_QUERY); 

	::com::sun::star::awt::Point *Pos = new ( ::com::sun::star::awt::Point );
	::com::sun::star::awt::Size *Size = new ( ::com::sun::star::awt::Size );
    Size->Width = largeur;
    Size->Height = hauteur;
	xShape->setSize(*Size);

	Reference<XPropertySet> xFrameProps (xFrame,UNO_QUERY);

	xCurseurTexte->gotoEnd( false ); // Insert a new paragraph
	contenuTexte->insertControlCharacter ( xCurseurTexte,com::sun::star::text::ControlCharacter::PARAGRAPH_BREAK, false ); // Then insert the new frame 
	contenuTexte->insertTextContent(xCurseurTexte, xFrame, false);
	// Access the XText interface of the text contained within the frame
	Reference<XText> xFrameText = xFrame->getText(); // Create a TextCursor over the
	Reference<XTextCursor> xFrameCursor = xFrameText->createTextCursor(); // Insert some text into the frame
	xFrameText->insertString( xFrameCursor, OUString (texte, (sal_Int32)strlen(texte),RTL_TEXTENCODING_ISO_8859_1 ), false ); 
	contenuTexte->insertControlCharacter ( xCurseurTexte,com::sun::star::text::ControlCharacter::PARAGRAPH_BREAK, false ); 
	} 
catch (Exception e)
	{ 
	int i=0; 
	} 
}

void AOO_LO::InsererImage2(char *nomFichier)
{
//
// Biblio : https://www.mail-archive.com/prog@fr.openoffice.org/msg03538.html
//
char aFileName[200]; // nom de l'image avec chemin complet
char aStr[1024];
OUString sGraphicUrl, anOUString; 
    
// Création d'un objet graphique
Reference <XPropertySet> xGraphic ( aoo_loMSF->createInstance(OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextGraphicObject" ))), UNO_QUERY );
strcpy(aFileName, nomFichier);
sprintf (aStr, "file://%s", aFileName); // chemin + nom de l'image à insérer
sGraphicUrl = OUString (aStr, (sal_Int32)strlen(aStr),RTL_TEXTENCODING_ISO_8859_1 );
 
// Créer une image dans le document pour ne plus avoir de lien avec le fichier
Reference <XNameContainer> xBitmap(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.drawing.BitmapTable" ))), UNO_QUERY );
  
sprintf (aStr, "ID017"); // création d'une bitmap dans le document avec un nom au choix
  
xBitmap->insertByName(createStr(aStr), makeAny(sGraphicUrl)); 
// remplissage des pixels de la bitmap avec ceux de l'image à intégrer et intégration dans le  doc
try {
// récupération de l'URL  de l'image intégrée dans le document (URL créée automatiquement par OO)
	xBitmap->getByName(createStr(aStr)) >>= anOUString; // EXCEPTION DECLENCHEE
    
	xGraphic->setPropertyValue(createStr("GraphicURL"), 
	makeAny(anOUString)); 	
	}
catch (Exception e)
	{ 
	int i=0; 
	}   
Reference <XTextRange>   xTextRange = contenuTexte->getEnd();
Reference <XTextContent> contenuGraphique (xGraphic,UNO_QUERY);
contenuTexte->insertTextContent(xTextRange, contenuGraphique,false);
}

void AOO_LO::InsererImage(char *nomFichier,int largeur,int hauteur)
{
//
// Biblio : http://stackoverflow.com/questions/6236515/how-to-insert-an-image-in-to-an-openoffice-writer-document-with-java?rq=1
//
#ifdef __TT
char aStr[1024];
OUString sGraphicUrl, anOUString; 
    
sprintf (aStr, "file:///%s", nomFichier); // chemin + nom de l'image à insérer
sGraphicUrl = OUString (aStr, (sal_Int32)strlen(aStr),RTL_TEXTENCODING_ISO_8859_1 );

Reference <XTextContent> xImage(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextGraphicObject" ))), UNO_QUERY ); 
Reference <XNameContainer> xBitmap(aoo_loMSF->createInstance(OUString( 
RTL_CONSTASCII_USTRINGPARAM("com.sun.star.drawing.BitmapTable" ))), UNO_QUERY );

Reference<XPropertySet> xProps(xImage,UNO_QUERY);
try{
xBitmap->insertByName("ID007", makeAny(sGraphicUrl));
}
catch (Exception e)
	{ 
	int i=0; 
	}   
Any p;
Any s=xBitmap->getByName(createStr("ID007"));
xProps->setPropertyValue("GraphicURL", makeAny(s));
xProps->setPropertyValue("AdjustContrast", Any((sal_Int16 )50));
/*
    TextContentAnchorType_AT_PARAGRAPH = 0,
    TextContentAnchorType_AS_CHARACTER = 1,
    TextContentAnchorType_AT_PAGE = 2,
    TextContentAnchorType_AT_FRAME = 3,
    TextContentAnchorType_AT_CHARACTER = 4,
    TextContentAnchorType_MAKE_FIXED_SIZE = SAL_MAX_ENUM
*/
Any prop;
prop <<= pImage.ancrage;
xProps->setPropertyValue("AnchorType", prop);
prop <<= pImage.typeOrientationVertical;
xProps->setPropertyValue( "VertOrient",prop);

prop <<= pImage.typeOrientationHorizontal;
xProps->setPropertyValue( "HoriOrient",prop);
prop <<= pImage.typeOrientationHorizontalRelative;
xProps->setPropertyValue( "HoriOrientRelation",prop);
prop <<= pImage.typeOrientationVerticalRelative;
xProps->setPropertyValue( "VertOrientRelation",prop);
xProps->setPropertyValue( "LeftMargin",Any((sal_Int16 )(pImage.margeGauche)));
xProps->setPropertyValue( "RightMargin",Any((sal_Int16 )(pImage.margeDroite)));
xProps->setPropertyValue( "BottomMargin",Any((sal_Int16 )(pImage.margeBas)));
xProps->setPropertyValue( "TopMargin",Any((sal_Int16 )(pImage.margeHaut)));
if (pImage.ancrage==com::sun::star::text::TextContentAnchorType::TextContentAnchorType_AT_PAGE)
	{
	xProps->setPropertyValue( "HoriOrientPosition",Any((sal_Int32 )pImage.positionX));
	xProps->setPropertyValue( "VertOrientPosition", Any((sal_Int32 ) pImage.positionY ) );
	}
xProps->setPropertyValue( "Description",makeAny(createStr(nomFichier)));
                // Setting the vertical position

                // Setting the width
xProps->setPropertyValue( "Width", Any((sal_Int32 ) largeur ) );

                // Setting the height
xProps->setPropertyValue( "Height", Any((sal_Int32 ) hauteur ) );

Reference <XTextRange>   xTextRange = contenuTexte->getEnd();
contenuTexte->insertTextContent(xTextRange, xImage,false);
xBitmap->removeByName("ID007");
#endif
}


void AOO_LO::InsererImageLien(char *nomFichier)
{
//
// Biblio :LibreOfficeSdk\examples\java\Text  _GraphicsInserter.java
//
char aFileName[200]; // nom de l'image avec chemin complet
char aStr[1024];
OUString sGraphicUrl, anOUString; 
    
// Création d'un objet graphique
Reference<XPropertySet> xGraphic ( aoo_loMSF->createInstance(OUString(RTL_CONSTASCII_USTRINGPARAM("com.sun.star.text.TextGraphicObject" ))), UNO_QUERY );
// Propriétés définies dans  https://wiki.openoffice.org/wiki/Documentation/DevGuide/Text/Shape_Objects_in_Text
strcpy(aFileName, nomFichier);
sprintf (aStr, "file:///%s", aFileName); // chemin + nom de l'image à insérer
//sGraphicUrl = OUString (aStr );
xGraphic->setPropertyValue("GraphicURL", makeAny(createStr(aStr)));
xGraphic->setPropertyValue("AdjustContrast", Any((sal_Int16 )50));
Any prop;
prop <<= com::sun::star::text::TextContentAnchorType::TextContentAnchorType_AT_PAGE;
xGraphic->setPropertyValue("AnchorType", prop);
xGraphic->setPropertyValue( "VertOrient",Any((sal_Int16 )0));
xGraphic->setPropertyValue( "HoriOrient",Any((sal_Int16 )0));
xGraphic->setPropertyValue( "HoriOrientPosition",Any((sal_Int32 )5500));
xGraphic->setPropertyValue( "Description",makeAny(createStr("Une image de koala")));

                // Setting the vertical position
xGraphic->setPropertyValue( "VertOrientPosition", Any((sal_Int32 ) 4200 ) );

                // Setting the width
xGraphic->setPropertyValue( "Width", Any((sal_Int32 ) 4400 ) );

                // Setting the height
xGraphic->setPropertyValue( "Height", Any((sal_Int32 ) 4000 ) );
Reference <XTextRange>   xTextRange = contenuTexte->getEnd();
Reference <XTextContent> contenuGraphique (xGraphic,UNO_QUERY);
contenuTexte->insertTextContent(xTextRange, contenuGraphique,false);
}


void AOO_LO::InsererTableau(void *t)
{
/*
Reference <XTextTable>  xt=InsererTableau(t->GetRows()+1,t->GetCols()+1);
wxString s;
wxString cellule;
char col[3];
int lig=1;
col[0]='A';col[1]=0;;col[2]=0;
for (int l=0;l<=t->GetRows();l++)
	for (int c=0;c<=t->GetCols();c++)
		{
		if (c<=26)
			{
			col[0]=c+'A';
			col[1]=0;
			}
		else
			{
			col[0]=c/26+'A';
			col[1]=c%26+'A';
			col[2]=0;
			}
		if (l==0)
			s = t->GetColLabelValue(c);
		else if (c==0)
			s = t->GetRowLabelValue(l);
		else	
			
			s=t->GetCellValue(l-1,c-1);
		cellule.Printf("%s%d",col,l+1);
		DefValeurCellule(xt,(char *)(static_cast<const char*>(cellule)),(char *)(static_cast<const char*>(s)));
		}
*/
}


Reference <XTextTable>  AOO_LO::InsererTableau(int nbLig,int nbCol)
{
//
// Biblio : https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer Tableaux dans un document Writer
//
Reference <XTextTable> xTable (aoo_loMSF->createInstance(createStr("com.sun.star.text.TextTable")),UNO_QUERY);
if (!xTable.is()) 
	{
	printf("Erreur creation XTextTable interface !\n");
	return NULL;
	}
 
xTable->initialize(nbLig, nbCol);
Reference <XTextRange>   xTextRange = contenuTexte->getEnd();
Reference <XTextContent> contenuTableau (xTable,UNO_QUERY);
 
contenuTexte->insertTextContent(xTextRange, contenuTableau,(unsigned char) 0);
return xTable;
}

void AOO_LO::ChangerCouleurTableau(Reference <XTextTable> xTable,int rgb)
{
//
// Biblio : https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer Tableaux dans un document Writer
//
Reference<XPropertySet> xTableProps (xTable,UNO_QUERY);
Any prop;
prop <<= (sal_Bool)false;
xTableProps->setPropertyValue(OUString::createFromAscii("BackTransparent"),prop);
prop <<= (long)rgb; 
xTableProps->setPropertyValue(OUString::createFromAscii("BackColor"),prop);
}


void AOO_LO::ChangerCouleurLigne(Reference <XTextTable> xTable,int indLig,int rgb)
{
//
// Biblio : https://wiki.openoffice.org/wiki/FR/Documentation/OpenOffice_Writer Tableaux dans un document Writer
//
Any prop;
 
Reference<XTableRows> xTableRows = xTable->getRows();
Reference<XIndexAccess> theRows (xTableRows,UNO_QUERY);
Reference<XPropertySet> xRowProps (theRows->getByIndex((short)indLig),UNO_QUERY);
prop <<= (sal_Bool)false;
xRowProps->setPropertyValue(OUString::createFromAscii("BackTransparent"),prop);
prop <<= (long)rgb;
xRowProps->setPropertyValue(OUString::createFromAscii("BackColor"),prop);
}

void AOO_LO::ChangerCouleurTexteCellule(Reference <XTextTable> tableau,char *ref,int rgb)
{
Reference<XCell> xCell = tableau->getCellByName(createStr(ref));
Reference<XText>   xText = Reference<XText>(xCell,UNO_QUERY);
Reference <XTextCursor>  xTextCursor = xText->createTextCursor();

Reference<XPropertySet> oCPS(xTextCursor,UNO_QUERY);
Any prop;

prop <<= (long)rgb; // color red  
oCPS->setPropertyValue(createStr("CharColor"),prop);
}

void AOO_LO::ChangerCouleurFondCellule(Reference <XTextTable> tableau,char *ref,int rgb)
{
Reference<XCell> xCell = tableau->getCellByName(createStr(ref));
Reference<XText>   xText = Reference<XText>(xCell,UNO_QUERY);
Reference <XTextCursor>  xTextCursor = xText->createTextCursor();

Reference<XPropertySet> oCPS(xTextCursor,UNO_QUERY);
Any prop;

prop <<= (long)rgb; // color red  
// BUG oCPS->setPropertyValue(createStr("BackGroundColor"),prop);
}


void AOO_LO::DefFormuleCellule(Reference <XTextTable> tableau,char *ref,char *formule)
{
Reference<XCell> xCell = tableau->getCellByName(createStr(ref));
Reference<XText>   xText = Reference<XText>(xCell,UNO_QUERY);
xCell->setFormula(createStr(formule));
}

void AOO_LO::DefValeurCellule(Reference <XTextTable> tableau,char *ref,char *texte)
{
Reference<XCell> xCell = tableau->getCellByName(createStr(ref));
Reference<XText>   xText = Reference<XText>(xCell,UNO_QUERY);
Reference <XTextCursor>  xTextCursor = xText->createTextCursor();
xTextCursor->setString(createStr(texte));   
}

void AOO_LO::DefValeurCellule(Reference <XTextTable> tableau,char *ref,int v)
{
Reference<XCell> xCell = tableau->getCellByName(createStr(ref));
Reference<XText>   xText = Reference<XText>(xCell,UNO_QUERY);
xCell->setValue(v);
}

void AOO_LO::InfoDocument()
{
           
Reference< XDocumentPropertiesSupplier > xDocumentPropertiesSupplier(aoo_loMSF, UNO_QUERY);
Reference< XDocumentProperties > xDocumentProperties = xDocumentPropertiesSupplier->getDocumentProperties();
           
Reference< XPropertyContainer > xPropertyContainer = xDocumentProperties->getUserDefinedProperties();
Reference< XPropertySet > xPropertySet(xPropertyContainer, UNO_QUERY);
 ListerProprietes(xPropertySet);      
Reference< XPropertySetInfo > xPropertySetInfo = xPropertySet->getPropertySetInfo();
           
if (xPropertySetInfo->hasPropertyByName(OUString::createFromAscii("Info 1")))
{
    Any newValue, sName;
    newValue <<= OUString::createFromAscii("Value has been set by API");
    xPropertySet->setPropertyValue (OUString::createFromAscii("Info 1"), newValue);
}
}


void AOO_LO::InsererPageDeTitre(wxXmlNode *f)
{
StyleAOO_LO s("Times",36, 
 		com::sun::star::awt::FontWeight::BOLD,true,
		com::sun::star::style::ParagraphAdjust::ParagraphAdjust_CENTER,0,
		com::sun::star::style::BreakType::BreakType_PAGE_AFTER);
wxXmlNode *n=f->GetChildren();
wxString	titre;
while (n)
	{
	if (n->GetName().Cmp("title")==0)
		{
		titre=n->GetChildren()->GetContent();
		ChangerStylePage("First Page");
		CreerStyle("MonStyle",s);
		ChangerStyle("MonStyle");
		}
	if (n->GetName().Cmp("figure")==0)
		{
		wxXmlNode *n1=n->GetChildren();
		while (n1)
			{
			if (n1->GetName().Cmp("mediaobject")==0)
	 			{
				wxXmlNode *n2=n1->GetChildren();
				if (n2->GetName().Cmp("imageobject")==0)
					{
					wxString nom;
					if (n2->GetAttribute("fileref",&nom))
						{
						wxImage im(nom);
						InsererImage((char *)(static_cast<const char*>(nom)),im.GetWidth()*10,im.GetHeight()*10);
						}
					}
				}
			n1=n1->GetNext();
			}
		}
	n =  n->GetNext();
	}
AjouterTexte((char *)(static_cast<const char*>(titre)));

}

void AOO_LO::InsererChapitre(wxXmlNode *f)
{
wxXmlNode *n=f->GetChildren();
int niveau=n->GetDepth();
wxString	titre;
while (n)
	{
	if (n->GetName().Cmp("title")==0)
		{
		titre=n->GetChildren()->GetContent()+"\n\r";
		switch(niveau){
		case 1:ChangerStyle("Heading 1");break;
		case 2:ChangerStyle("Heading 2");break;
		case 3:ChangerStyle("Heading 3");break;
			}
		AjouterTexte( (char *)(static_cast<const char*>(titre)));
		ChangerStyle("Standard");
		}
	if (n->GetName().Cmp("xsl:template")==0 )
		{
		wxString nom;
		if (n->GetAttribute("name",&nom))
			{
			if (nom.Cmp("header.content")==0&& entete.Length()==0)
				{
				entete=n->GetChildren()->GetContent();
				InsererEntete((char *)(static_cast<const char*>(entete)));
				InsererPiedDePage("Rapport wxOpenCV");

				}
			if (nom.Cmp("footer.content")==0&& pied.Length()==0)
				{
				pied=n->GetChildren()->GetContent();
				InsererPiedDePage((char *)(static_cast<const char*>(pied)));

				}
			}
			
		}
	if (n->GetName().Cmp("para")==0)
		{
		titre=n->GetChildren()->GetContent()+"\n\r";
		AjouterTexte( (char *)(static_cast<const char*>(titre)));
		}
	if (n->GetName().Cmp("figure")==0)
		{
		wxXmlNode *n1=n->GetChildren();
		while (n1)
			{
			if (n1->GetName().Cmp("mediaobject")==0)
	 			{
				wxXmlNode *n2=n1->GetChildren();
				if (n2->GetName().Cmp("imageobject")==0)
					{
					wxString nom;
					if (n2->GetAttribute("fileref",&nom))
						{
						if (nom.Find(".svg")==wxNOT_FOUND)
							{
							wxImage im(nom);
							InsererImage((char *)(static_cast<const char*>(nom)),im.GetWidth()*10,im.GetHeight()*10);
							}
						else
							{
							pImage.ancrage=com::sun::star::text::TextContentAnchorType::TextContentAnchorType_AS_CHARACTER;
							InsererImage((char *)(static_cast<const char*>(nom)),8000,8000);

							}
						}
					}
				}
			n1=n1->GetNext();
			}
		}
	n =  n->GetNext();
	}
AjouterTexte((char *)(static_cast<const char*>(titre)));
}


    //============================================================================
int main(int argc, char **argv)
{
if (argc!=2)
	return -1;
wxXmlDocument docBook;
wxInitAllImageHandlers();
 	 
if (!docBook.Load(argv[1]))
	return -1;
int argcc=1;
char *argvv=" ";
sal_detail_initialize( argcc, &argvv);

AOO_LO	docLO;
docLO.LancerWriter();

wxXmlNode *racine=docBook.GetRoot();
wxXmlNode *f=racine->GetChildren();
do
	{
	wxString s=f->GetName();
	if (s.Cmp("info")==0)
		{
		docLO.InsererPageDeTitre(f);
 		docLO.AjouterSignet("tdm");
		}
	if (s.Cmp("section")==0)
		{
		docLO.InsererChapitre(f);
		}
	f=f->GetNext();
	}
while (f);
	 
docLO.AtteindreSignet("tdm");
docLO.TableDesMatieres();
docLO.NumeroterPage();
docLO.SautDePage();
docLO.MAJDocument();
wxString nomAoo("file:///f:/tmp/");
nomAoo=nomAoo+argv[1];
nomAoo.Replace(".xml",".pdf");
docLO.ExportPDF((char *)(static_cast<const char*>(nomAoo)));
//docLO.ListerProprietesFiltre();
nomAoo.Replace(".pdf",".doc");
docLO.SauverDocument((char *)(static_cast<const char*>(nomAoo)),"MS Word 97");
nomAoo.Replace(".doc",".odt");
docLO.SauverDocument((char *)(static_cast<const char*>(nomAoo)),"");
docLO.InfoDocument();

   return 0;
 }



