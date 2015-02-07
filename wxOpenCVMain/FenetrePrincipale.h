﻿#ifndef _WXAPPLI_H_
#define _WXAPPLI_H_

#define NB_MAX_RECTANGLE 10
#include "ImageInfo.h"
#include "CameraVirtuelle.h"
#include "HorodatageSequence.h"
#include "wx/timer.h"
#include <wx/dynlib.h>
#include <wx/fileconf.h>
#include <wx/graphics.h>

//#include <wx/dde.h>
#include <string>
#include <map>

#include "wxOsgApp.h"

class ControleCamera;
class ImageStatistiques;
class FenetreAlgo;
class OutilsImage;
class BarreInfo;
class FenetreZoom;
class DragShape;
class GlisserImage;



// Constante pour usage de la souris
#define SOURIS_STD 0
#define SELECTION_EN_COURS 1


/*! \class Operation
   * \brief classe permettant de mémoriser une opération sur une image
   *
   *  La classe Operation permet de mémoriser le nom d'une opération, les opérandes, les indices des fenêtres concernées,
   * et les paramètres de l'opération.
   */
class Operation {
public :
wxString	nomOperation; /*!< Nom de l'opération demandée */
ImageInfoCV *op1;	/*!< Opérande 1 pour l'opération demandée */
ImageInfoCV *op2;	/*!< Opérande 2 pour l'opération demandée */
// Si les valeurs précédentes sont nulles on met un indice
long indOp1;	/*!< Indice de la fenêtre contenant l'opérande 1 */
long indOp2;	/*!< Indice de la fenêtre contenant l'opérande 2 */
Parametre pOCV;	/*!< parametre de l'opérateur Unaire */
};



/*! \class ZoneImage
   * \brief classe affichant une image
   *
   *  La classe ZoneImage permet l'affichage d'une image dans une fenêtre comportant des ascenseurs.
   */
class ZoneImage : public  wxScrolled<wxWindow>
{
FenetrePrincipale	*f;				/*!< Cadre où est affichée la fenêtre avec ascenseur */
wxOsgApp			*osgApp;		/*!< lien sur l'application */
void				*f3D;			/*!< lien sur la fenetre 3D représentant l'image */
int					facteurZoom;	/*!< zoom=2^facteurZoom*/
wxRect				rectSelect[NB_MAX_RECTANGLE]; /*!< zone rectangulaire sélectionnée*/
wxRect				rectCoupe[NB_MAX_RECTANGLE]; /*!< diagonale d'un rectangle associée à une coupe*/
bool				modeRect;	/*!< Sélection des rectangles actives */
bool				modeCoupe; /*!< Sélection des coupes actives */
int					indRect; /*!< Indice du rectangle actif */
int					indCoupe; /*!< Indice de la coupe active */
wxBitmap			*bitmapAffiche;/*!< bitmap de l'image */
char				modeComplexe;/*!< Type d'affichage pour une image complexe */

public :
ZoneImage(wxWindow *parent,wxSize w); 
    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe ZoneImage
     *
     *  \param parent : fenêtre mére
		\param size : taille de la fenêtre
	*/
void DefFenetrePrincipale(FenetrePrincipale *w){f=w;};
    /*!
     *  \brief DefFenetrePrincipale
     *
     *  Définition de la fenêtre parent
	 * \param w : fenêtre parent
	 */

void DefOSGApp(wxOsgApp *w){osgApp=w;};
    /*!
     *  \brief DefOSGApp
     *
     *  Définition de l'application
	 * \param w : application
	 */
void F3D(void *f){f3D=f;};
    /*!
     *  \brief F3D
     *
     *  Fenetre 3D (OSG) assoicée à l'image
	 * \param w : application
	 */
void OnSize( wxSizeEvent &w);
    /*!
     *  \brief OnSize
     *
     *  Gestion du dimensionnement de la fenêtre par la souris
	 * \param w : évènement souris
	 */
void TracerRectangle(int ind,bool croix);
    /*!
     *  \brief TracerRectangle
     *
     *  Tracer du rectangle rectSelect[ind] avec une croix si croix est vrai
	 * \param ind : indice du rectangle dans le tableau  rectSelect
	 * \param croix : si vrai les diagonales du rectangles sont tracées.
	 */
void TracerLesRectangles (wxDC &hdc);
   /*!
     *  \brief TracerLesRectangles
     *
     *  Tracer de tous les rectangles du tableau rectSelect (dans le système de coordonnées image)sur l'écran
	 * \param hdc : contexte graphique
	 */

void TracerLesCoupes (wxDC &hdc);
   /*!
     *  \brief TracerLesCoupes
     *
     *  Tracer des coupes définies par les diagonales des rectangles du tableau rectCope (dans le système de coordonnées image)sur l'écran
	 * \param hdc : contexte graphique
	 */

// Changement de coordonnées
wxPoint RepereEcranImage(wxPoint &p);
   /*!
     *  \brief RepereEcranImage
     *
     * Changement de système de coordonnées d'un point : Ecran vers Image
	 * \param p : point dans le système de coordonnées écran
	 * \return point dans les système de coordonnées image
	 */
wxPoint RepereImageEcran(wxPoint &p);
   /*!
     *  \brief RepereImageEcran
     *
     * Changement de système de coordonnées d'un point : Image vers Ecran
	 * \param p : point dans le système de coordonnées écran
	 * \return point dans les système de coordonnées image
	 */
wxRect RepereImageEcran(wxRect &r);
   /*!
     *  \brief RepereImageEcran
     *
     * Changement de système de coordonnées d'un rectange : Image vers Ecran
	 * \param p : point dans le système de coordonnées écran
	 * \return point dans les système de coordonnées image
	 */
wxRect RepereEcranImage(wxRect &r);
   /*!
     *  \brief RepereImageEcran
     *
     * Changement de système de coordonnées d'un rectangle : Ecran vers Image
	 * \param p : point dans le système de coordonnées écran
	 * \return point dans les système de coordonnées image
	 */
void ModeRectangle(bool t){modeRect=t;};
   /*!
     *  \brief ModeRectangle
     *
     * Activation du mode rectangle. Dans ce mode il est possible de séletionner un rectangle avec la souris.
	 * L'histogramme est calculé à partir des points images inclus dans le rectangle.
	 * \param t : vrai pour activer le mode rectangle
	 */
bool ModeRectangle(){return modeRect;};
   /*!
     *  \brief ModeRectangle
     *
	 * Indique si le mode rectangle est actif.
	 * \return vrai si le mode rectangle est actif
	 */
wxRect *RectangleSelec(){return &rectSelect[indRect];};
   /*!
     *  \brief RectangleSelec
     *
	 * La fonction retourne comme résultat le rectangle sélectionné.
	 * \return pointeur sur le rectangle sélectionné.
	 */
wxRect *Rectangle(int i){if (i>=0 && i<NB_MAX_RECTANGLE) return &rectSelect[i];return NULL;};
   /*!
     *  \brief Rectangle
	 * La fonction retourne le i ème rectangle du tableau rectSelect.
	 * \return pointeur sur le i ème rectangle du tableau rectSelect.
	 */
int IndiceRectangleSelec(){return indRect;};
   /*!
     *  \brief IndiceRectangleSelec
	 * La fonction retourne l'indice du rectangle  sélectionné dans le tableau rectSelect.
	 * \return indice du rectangle sélectionné dans le tableau rectSelect.
	 */
void IndiceRectangleSelec(int i){indRect=i;};
   /*!
     *  \brief IndiceRectangleSelec
	 * La fonction fixe le nouveau rectangle sélectionné dans le tableau rectSelect.
	 * \param i : le i ème rectangle du tableau rectSelect sera le rectangle sélectionné.
	 */
void ModeCoupe(bool t){modeCoupe=t;};
   /*!
     *  \brief ModeCoupe
     *
     *  Activation du mode coupe. Dans ce mode il est possible de séletionner une ligne avec la souris.
	 * Une coupe de l'image peut alors être afficgée dans la zone Stat.
	 * \param t : vrai pour activer le mode coupe
	 */
char ModeCoupe(){return modeCoupe;};
  /*!
     *  \brief ModeRectangle
     *
	 * Indique si le mode coupe est actif.
	 * \return vrai si le mode coupe est actif
	 */
wxRect *CoupeSelec(){return &rectCoupe[indCoupe];};
   /*!
     *  \brief CoupeSelec
     *
	 * La fonction retourne comme résultat le rectangle sélectionné associé à la coupe.
	 * \return pointeur sur le rectangle sélectionné.
	 */
wxRect *Coupe(int i){if (i>=0 && i<NB_MAX_RECTANGLE) return &rectCoupe[i];return NULL;};
   /*!
     *  \brief Coupe
	 * La fonction fixe la nouvelle coupe sélectionnée dans le tableau rectCoupe.
	 * \param i : le i ème rectangle du tableau coupeSelect sera la nouvelle coupe sélectionnée.
	 */
int IndiceCoupeSelec(){return indCoupe;};
   /*!
     * \brief IndiceCoupeSelec
	 * La fonction retourne l'indice de la coupe  sélectionnée dans le tableau rectCoupe.
	 * \return indice de la coupe sélectionnée dans le tableau rectCoupe
	 */
void IndiceCoupeSelec(int i){indCoupe=i;};
  /*!
     *  \brief IndiceCoupeSelec
	 * La fonction fixe la nouvelle coupe sélectionnée dans le tableau rectCoupe.
	 * \param i : le i ème rectangle du tableau rectCoupe sera la coupe sélectionnés.
	 */
char ModeComplexe(){return modeComplexe;};
  /*!
     *  \brief ModeComplexe
	 * La fonction retourne la manière dont les nombres complexes seront affichés.
	 */

void BitmapAffichee(wxBitmap *v){bitmapAffiche=v;};
  /*!
     *  \brief BitmapAffichee
     *
	 * Définit le bitmap qui ser affiché dans la fenêtre.
	 * \param v  : nouveau bitmap à afficher
	 */
wxBitmap *BitmapAffichee(){return bitmapAffiche;};
  /*!
     *  \brief BitmapAffichee
     *
	 * Définit le bitmap qui ser affiché dans la fenêtre.
	 * \return bitmap affiché
	 */
int FacteurZoom(){return facteurZoom;};
  /*!
     *  \brief FacteurZoom
     *
	 * La valeur du facteur de zoom de l'image est retournée par cette fonction. Ilest égal à 2^facteurZoom.
	 * \return log2 du zoom
	 */
void FacteurZoom(int f){facteurZoom=f;};
  /*!
     *  \brief FacteurZoom
     *
	 * Fixe la valeur du facteur de zoom de l'image.
	 * \param f : log2 du zoom
	 */

private:
    void OnPaint(wxPaintEvent& WXUNUSED(event));
	void OnClose(wxWindowDestroyEvent& event);
	void OnActivate(wxActivateEvent &w);
	void OnLeftButtonUp(wxMouseEvent &event);
	void OnLeftButtonDown(wxMouseEvent &event);
	void OnMouseMove(wxMouseEvent &event);
	void GestionCurseurSouris(wxMouseEvent &event);
	void OnMenuContext(wxContextMenuEvent& event);
	void ShowContextMenu(const wxPoint& pos);
	void Vue3D(wxCommandEvent& event);
	void SelectPalette(wxCommandEvent& event);
	void ModeComplexe(wxCommandEvent& event);
	void MAJZoom(wxCommandEvent& event);

wxMenu *CreateMenuComplex(wxString *title);
wxMenu *CreateMenuPalette(wxString *title);
wxMenu *CreateMenuZoom(wxString *title);
};


/*! \class FenetrePrincipale
   * \brief classe affichant une image
   *
   *  La classe FenetrePrincipale contient une zone pour afficher l'image et une barre d'état pour afficher des informations sur l'image.
   */
class FenetrePrincipale : public wxFrame
{

ZoneImage			*feuille;			/*!< Fenêtre où est affchée l'image */
int					idFenetre;			/*!< Identifiant unique pour de la fenêtre dans la liste des fenetres */
char				fenetreSauvee;		/*!< Passe à 1 lorsque la fenêtre est sauvée */
char				socketPerdu;		/*!< Est-ce encore utile?*/
wxString			repertoireDuDocument; /*!< Répertoire d'origine de l'image sur le disque */
wxString			nomDuDocument; /*!< Nom de l'image */
wxString			nomImageBiais;/*!< Nom de l'image du biais associé à cette image*/
wxString			nomImageTache;/*!< Nom de l'image des tâches sur l'optique du capteur*/
wxString			nomImageQuadrique;/*!< Nom de l'image du fond (une fois les tâches enlevées */

Operation			origineImage;
//int					idFenetreOp1Pre; /*!< Pour image issue d'une opération : identifiant de operande1*/	
//int					idFenetreOp2Pre; /*!< Pour image issue d'une opération : identifiant de operande2*/
//wxString			nomOperationPre;	 /*!< Pour image issue d'une opération : nom de l'opération */
//Parametre			pOCVPre;		 /*!< Pour image issue d'une opération : parametre de l'opération précédente*/

char				modeCamera;			/*!< 1 affichage de l'image capturée */
char				imageTraitee;		/*!< 1 si traitement de l'image captureée est terminé  */	
char				interdireAffichage;
char				correctionTache;	/*!< 1 active correction tache */
char				correctionFonction; /*!< 1 active correction fonction fond */
char				correctionBiais;	/*!< 1 active correction du biais du CCD */
char				modeImage;			/*!< 0 image, 1 module gradient, 2 binarisation, 3 region */
char				modeFiltre;			/*!< 0 image standard, image filtrée avec passe bas le + fort */
char				modeMoyenne;		/*!< Moyenne temporelle active lors de l'acquisition */
char				typeAcqImage;		/*!< 0 Standard, 1 Acquisition image noire, 2 Image des taches, 3 Image fonction fond */
int					nbImageBiais;
int					dImageBiais;
int					nbImageTache;
int					dImageTache;
int					nbImageFonction;
int					dImageFonction;
int					indFiltreMoyenne;	/*!< Indice des coefficients du filtre Butterworth*/ 
int					nbMarcheFit;
double				**poly;				/*!< Coefficient de la quadrique pour la correction du fond */ 


ImageInfoCV			*imAcq;				/*!< Dernière image calculée incluant les corrections */

double				alphad,alpham;		/*!< Paramétres pour le filtre de deriche */
double				seuilModuleHaut;	/*!< Seuil du module pour limiter les régions */
double				seuilModuleBas;		/*!< Seuil du module pour limiter les régions */
int					seuilSurface;		/*!< Surface minimale pour qu'une zone soit retenue pour l'ajustement */
double				diffHauteur;		/*!< Différence de hauteur pour fusion des régions */

wxImage				*imAffichee;		/*!< wxImage affichée résultant de l'image opencv et de la palette*/

wxColor				*pCouleur;
wxColor				*pLineaire;
wxColor				*pAleatoire;
wxColor				*pJet;
wxColor				*pRainbow;
wxColor				*pPerso;
wxColor				*pPersoInv;
wxColor				*paletteChargee[10];
bool				planActif[10];		/*!< plan couleur actif pour l'affichage */

char				fondDejaDefini;		/*!< Vrai Une image de fond existe déjà */
char				modeTransparence;	/*!< Mode transparence 0 une seule image affichée, 1 mode actif */
int					cTransparence[4];	/*!< Coefficient de transparence pour les images image en cours, module seuil région */
unsigned char		*tabRGB;			/*!< Image couleur affichée */
unsigned char		*tabRGBTransparence;// Image couleur pour la gestion de la transparence
static	int			palArcEnCiel[1000];	/*!< Données contenant la palette arc en ciel */
double				*seuilNivBas;		/*!< les valeurs d'image avec une valeur inférieur seront de couleur 0*/
double				*coeffCanal;		/*!< coefficent multiplicatif pour caluler le niveau du pixel dans la palette nivPale
coeffCanal[i]*max(I(x,y,i)- seuilNivBas(i),0) */
//int					debNivPalette[6];	// Début du niveau associés à la couleur 0 
//int					finNivPalette[6];	// Fin du niveau associés à la dernière couleur de la palette 
int					indPalette;			/*!< Indice de palette sélectionnée */
int					nbCouleurPalette;	/*!< Nombre de couleur dans la palette */
wxColour			*p;
wxOsgApp			*osgApp;
FenetreZoom			*fenZoom;
ImageStatistiques	*imgStatIm;
FenetreAlgo			*fenAlgo;
HorodatageSequence  *dateSeq;
wxTimer				*horlogeSeq;
BarreInfo			*barreEtat;
CameraVirtuelle		*cam;
public :

wxCriticalSection	travailCam; /*!< Utilisé pour bloquer la mémoire image ou communiquer avec le thread */
protected:
bool				zoomActif;
bool				statActif;
wxTimer				*detectionUtilisateur;

static FenetrePrincipale *fenDrag;

private : // Gestion du curseur*
wxList          m_displayList; // A list of DragShapes
int             m_dragMode;
DragShape*      m_draggedShape;
DragShape*      m_currentlyHighlighted; // The shape that's being highlighted
wxPoint         m_dragStartPos;
GlisserImage*   m_dragImage;

static char		operationSelectionne; // le curseur est un texte

public:
    /*!
     *  \brief Constructeur
     *
     *  Constructeur de la classe FenetrePrincipale
     *
     *  \param frame : fenêtre mére
	 *  \param title : Nom du document
	 *  \param pos : position de la fenêtre par rapport à la fenêtre mère
	 *  \param size : Taille initiale de la fenêtre
	 *  \param style : style par défaut de la fenêtre
     */
FenetrePrincipale(wxFrame *frame, const wxString& title, const wxPoint& pos, 
    const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
    /*!
     *  \brief Destructeur
     *  Destructeur de la classe FenetrePrincipale
     */
~FenetrePrincipale();

int IdFenetre(){return idFenetre;};
    /*!
     *  \brief IdFenetre
     *  \return identifiant unique de l'objet instancié
     *
     */
int IdFenetreOp1pre(){return origineImage.indOp1;};
    /*!
     *  \brief fonction IdFenetreOp1pre
     *  identificateur de la fenetre contenant l'image operateur1.
     */
ImageInfoCV *ImageOp1pre();
    /*!
     *  \brief fonction ImageOp1pre
     *  Image actuelle =im1 (OP) im2. Retourne im1 ou null si image non valide.
     */
ImageInfoCV *ImageOp2pre();
    /*!
     *  \brief fonction ImageOp2pre
     *  Image actuelle =im1 (OP) im2. Retourne im2 ou null si image non valide.
     */
Operation *OrigineImage(){return &origineImage;};
    /*!
     *  \brief fonction OrigineImage
     *  caractéristiques de l'opération.
     */
void RAZFenAlgo(){fenAlgo=NULL;};
    /*!
     *  \brief fonction RAZFenAlgo
     *  Appelée lors de la fermeture de la fenêtre algorithme.
     */

//void AdjustHV(int orientation);
void OnSize( wxSizeEvent &);
void OnActivate(wxActivateEvent &);
void OnKeyDown(wxKeyEvent &event);
void TraitementClavier(wxKeyEvent &event);
void OnEraseBackground(wxEraseEvent& event);

void OnKeyUp(wxKeyEvent &event);
void OnIdle(wxIdleEvent& event);
void OnClose(wxCloseEvent& event);
void OnOuvrirHorodatage(wxCommandEvent& event);

void OnThreadUpdateQueue(EvtPointSuivis &w);


void OnOuvrir(wxString &);
void OuvrirVideo();
void InitImageFenetre();
void OuvrirBiais(wxCommandEvent& event);
void OuvrirBiais(char	*nomFichier);
void OuvrirTache(wxCommandEvent& event);
void OuvrirTache(char	*nomFichier);
void EnregistrerBiais();
void EnregistrerTache();
void DefImageQuadrique();
void Enregistrer(wxCommandEvent& event);
void EnregistrerSous(wxCommandEvent& event);
void CreerRapport(wxCommandEvent& event);
    /*!
     *  \brief CreerRapport
     *  Création d'un rapport sur l'image
     */
void ModeCamera(wxCommandEvent& event);
    /*!
     *  \brief ModeCamera
     *  bascule pour activer ou désactiver la caméra
     */
void DoPauseThread();
    /*!
     *  \brief DoPauseThread
     *  Activation de la caméra
     */
void OnCloseThread(wxCloseEvent&);
    /*!
     *  \brief OnCloseThread
     *  Arrêt de la caméra
     */

// Gestion du curseur de la souris pour les opérations sur les images
void DefPointeurSouris(int type);
void GestionCurseurSouris(wxMouseEvent &event);
void GestionCurseurSourisGlisser(wxMouseEvent &event);
void ArreterDragging(FenetrePrincipale* =NULL);
DragShape* FindShape(const wxPoint& pt) const; 
void DrawShapes(wxDC& dc);
void EraseShape(DragShape* shape, wxDC& dc);
void ClearShapes();
wxList& GetDisplayList() { return m_displayList; };
bool TileBitmap(const wxRect& rect, wxDC& dc, wxBitmap& bitmap);
void	OperationEncours(char b){operationSelectionne=b;};
void	SourisQuitterFen(wxMouseEvent &event);


// Gestion de l'image par rapport à la fenêtre
void AssosierImage(ImageInfoCV *);	/*< Associer une image à une fenêtre*/
void DefHistorique(int ind1=-1,int ind2=-1 ,wxString nomF=wxEmptyString,Parametre *pOCV=NULL); /*< Associer origine à une image */
    /*!
     *  \brief Fonction DefHistorique
     *
     *  L'image est issue d'un calcul
     *
     *  \param ind1 : premier argument de l'opérande
	 *  \param ind2 : deuxième argument de l'opérande -1 sinon
	 *  \param nomF : nom de l'opération
	 *  \param pOCV : parametre de l'opération NULL s'il n'existe pas
     */
// Accès aux membres privés
Parametre *ParamOCV(){return &origineImage.pOCV;}
    /*!
     *  \brief Fonction ParamOCV
     *
     *  Lecture des parametres utilisés pour calculer l'image
     *
     */

CameraVirtuelle* Cam(){return cam;};
BarreInfo *BarreEtat(){return barreEtat;};
void RepertoireDoc(wxString &s){repertoireDuDocument=s;};
void NomDoc(wxString &s){nomDuDocument=s;};
char ModeCamera(){return modeCamera;};
char ImageTraitee(){return imageTraitee;};
char ModeImage(){return modeImage;};
wxImage* ImageAffichee(){return imAffichee;};
int NbMarcheFit(){return nbMarcheFit;};
wxColor *Palette(){return pCouleur;};
char ModeRectangle(){return feuille->ModeRectangle();};
char ModeCoupe(){return feuille->ModeCoupe();};
wxRect *RectangleSelec(){return feuille->RectangleSelec();};
wxRect *Rectangle(int i){if (i>=0 && i<NB_MAX_RECTANGLE) return feuille->Rectangle(i);return NULL;};
int IndiceRectangleSelec(){return feuille->IndiceRectangleSelec();};
int FacteurZoom(){return feuille->FacteurZoom();};
ImageInfoCV	*	ImAcq(void){return imAcq;};

void DefinitionFondMicro();
void DefinitionFondQuadrique(wxCommandEvent& event);
void OnPaint(wxPaintEvent& event);
void OnMenuContext(wxContextMenuEvent& event);
void ShowContextMenu(const wxPoint& pos);
void OnTimer(wxTimerEvent& event);
void DetectionUtilisateur(wxTimerEvent& event);
char CorrectionBiais(){return correctionBiais;};
char CorrectionFond(){return correctionTache;}; 
void ActiveCorrectionFonction(){correctionFonction=1;fondDejaDefini=1;};
void ActiveCorrectionTache(){correctionTache=1;fondDejaDefini=1;};
void ActiveCorrectionBiais(){correctionBiais=1;};
void DesactiveCorrectionBiais(){correctionBiais=0;};
void DesactiveCorrectionFonction(){correctionFonction=0;};
void DesactiveCorrectionTache(){correctionTache=0;};
void ActiveModeMoyenne(){modeMoyenne=1;};
void DesactiveModeMoyenne(){modeMoyenne=0;};
void ActiveTransparence(){modeTransparence=1;};
void DesactiveTransparence(){modeTransparence=0;};
int	 LireCoefTransparence(int i){if (i>=0 &&i<=3)	return cTransparence[i];return 0;};
void DefCoefTransparence(int i,int val){if (i>=0 &&i<=3)	cTransparence[i]=val;return ;};
void DefIndFiltreMoyenne(int x){indFiltreMoyenne=x;};
bool BasculePlan(int i){if (i>=0&&i<10) {planActif[i]= !planActif[i];return planActif[i];}return false;}
bool PlanActif(int i){if (i>=0&&i<10) return planActif[i];return false;};
wxMenu *CreateMenuPalette(wxString *title);
wxMenu *CreateMenuZoom(wxString *title);

double DericheAlphaM(){return alpham;};
void DericheAlphaM(double x){alpham=x;};
double DericheAlphaD(){return alphad;};
void DericheAlphaD(double x){alphad=x;};
double SeuilModuleHaut(){return seuilModuleHaut;};
double SeuilModuleBas(){return seuilModuleBas;};
void SeuilModuleHaut(double x){seuilModuleHaut=x;};
void SeuilModuleBas(double x){seuilModuleBas=x;};
void DiffHauteur(double x){diffHauteur=x;};
float DiffHauteur(){return diffHauteur;};
int SeuilSurface(){return seuilSurface;};
void SeuilSurface(int x){seuilSurface=x;};
void DefNbMarcheFit(int x);


// menu
/* MENU FICHIER */
void Quitter(wxCommandEvent& event);
/* MENU IMAGE */
void ImageCamera(wxCommandEvent& event);
/* MENU ACQUISITION */
void DebutAcquisition(wxCommandEvent& event);
void FinAcquisition(wxCommandEvent& event);
/* MENU CONTEXTUEL SOURIS STD*/
void MAJZoom(wxCommandEvent& event);
void MAJRectangle(wxCommandEvent& event){feuille->ModeCoupe(false);feuille->ModeRectangle(!feuille->ModeRectangle());};
void MAJCoupe(wxCommandEvent& event){feuille->ModeRectangle(false);feuille->ModeCoupe(!feuille->ModeCoupe());};
void MAJFiltreMax(wxCommandEvent& event){modeFiltre= !modeFiltre;};
void SelectPalette(wxCommandEvent& event);
/* MENU CONTEXTUEL Operation*/
void PrepOperation(wxCommandEvent& event);
   /*!
     *  \brief Fonction ParamAlgo
     *
     *  L'image est issue d'un calcul. Les valeurs des paramètres peuvent être modifiés
     *	dans la fenêtre.
     *  \param event : premier argument de l'opérande
     */
void ParamAlgo(wxCommandEvent& event);
/* Palette et Image */
void DefSeuilNivBas(double x,int plan=-1);
void DefCoeffCanal(double x,int plan=-1);
double SeuilNivBas(int plan=-1){if (plan==-1) return seuilNivBas[0];return seuilNivBas[plan];};
double CoeffCanal(int plan=-1){if (plan==-1) return coeffCanal[0];return coeffCanal[plan];};
int  NbCouleurPalette(){return nbCouleurPalette;};
void InitPalette(int nbCouleur);
void TracerDIB(wxImage *d,wxDC &hdc,wxPoint *curseur=NULL);
void DrawWindow (wxDC &hdc);
void DrawRectangles (wxDC &hdc);
void TracerRectangle(int ind,int croix);

void DrawLignes (wxDC &hdc);
void TracerCercle(char *rSelect);
void DIB(ImageInfoCV *im);
void DIBImage(ImageInfoCV *im,int	indPlanTransparent=0);
void RAZTransparence();

void DefOSGApp(wxOsgApp *w){osgApp=w;if (feuille) feuille->DefOSGApp(w); };
void F3D(void *f){if (feuille) feuille->F3D(f);};
void *OSGApp(){return osgApp; };
void  DefZoom(FenetreZoom *f){fenZoom=f;};
void  DefImgStat(ImageStatistiques *f){imgStatIm=f;};
ImageStatistiques  *ImgStat(){return imgStatIm;};

void BasculeZoom();
void BasculeStat();

void DefId(int n ){idFenetre=n;};
void NouvelleImage();
void MAJNouvelleImage();
int	NbPixelsImage(){return imAcq->rows*imAcq->cols;};
ControleCamera		*CtrlCamera(void);
void RedresseRectangle(wxRect &r);
void CalculZoom(int &,int &);
wxRect CalculPosRectDst(wxRect &src,wxPoint *p);
wxPoint RepereEcranImage(wxPoint &p);
wxPoint RepereImageEcran(wxPoint &p);
wxRect RepereImageEcran(wxRect &r);
wxRect RepereEcranImage(wxRect &r);
void EnregistrerImage();
void InitImageBiais(){nbImageBiais=0;};
void InitImageTache(){nbImageTache=0;};
void InitImageFonction(){nbImageFonction=0;};
double ***LirePolyCorrection(){return &poly;};
void IncrementImageBiais(int x){dImageBiais=x;};
void IncrementImageTache(int x){dImageTache=x;};
void IncrementImageFond(int x){dImageFonction=x;};
void DefTypeAcqImage(char x){typeAcqImage=x;};
void InterdireAffichage(){interdireAffichage=true;};
void AutoriserAffichage(){interdireAffichage=false;};
char UtilisateurAbsent();
void RecupDerniereConfig();
void SauveDerniereConfig();
std::string  CommentaireConfig();
void InitIHM();

char SequenceEnCours(){if (horlogeSeq) return horlogeSeq->IsRunning();return 0;};
int	TempsRestantHorlogeSeq(){if (horlogeSeq) return horlogeSeq->GetInterval();return -1;};

// Curseur
void DefCurseur(int r=1,int xc=15,int yc=15);



wxColour CouleurAleatoire(int i){if (pAleatoire) return pAleatoire[i];return wxColour(0,0,0);};

char TypeAcqImage(void){return typeAcqImage;};
private:


    DECLARE_EVENT_TABLE()
};

/*! \class FenetreZoom
   * \brief classe affichant ua zone de l'image agrandie.
   *
   */

class FenetreZoom : public wxFrame
{
int		facteurZoom;
char	zoomInactif;
FenetrePrincipale	*fMere;
public:
FenetreZoom(FenetrePrincipale	*);
FenetreZoom(FenetrePrincipale	*,wxRect &);

void OnClose(wxCloseEvent& event);
void MAJZoom(wxCommandEvent& event);
void OnMenuContext(wxContextMenuEvent& event);
void ShowContextMenu(const wxPoint& pos);

void TracerZoom(wxPoint p);
void TracerZoom(wxRect &rSrc);
void CalculZoom(int &nume,int &deno);
wxRect CalculPosRectDst(wxRect &src,wxPoint *p);
wxPoint RepereEcranImage(wxPoint &p);
wxPoint RepereImageEcran(wxPoint &p);
wxRect RepereImageEcran(wxRect &r);
wxRect RepereEcranImage(wxRect &r);
    DECLARE_EVENT_TABLE()
};


/*! \class BarreInfo
   * \brief classe permettant d'accéder à des informations sur l'image
   *
   *  La classe BarreInfo permet d'afficher la valeur du pixel où est le curseur, d'afficher un zoom sur l'image,
   *  activer des statistiques sur l'image, de remplacer l'image par l'entrée d'une caméra.
   */
class BarreInfo : public wxStatusBar
{
char	curseur;
public:
    BarreInfo(wxWindow *parent);
    virtual ~BarreInfo();

void UpdateClock();
void UpdateCurseur(int x,int y,int val);
void UpdateCurseur(int x,int y,double val);
void UpdateCurseur(int x,int y,int r,int g,int b);
void UpdateCurseur(int x,int y,std::complex<float> r,std::complex<float> g,std::complex<float> b);

    // event handlers
    void OnTimer(wxTimerEvent& WXUNUSED(event)) { UpdateClock(); }
    void OnSize(wxSizeEvent& event);
    void BasculeCurseur(wxCommandEvent& event);
	void MAJButton();
    void OnButton(wxCommandEvent& event);
	char Curseur(){return curseur;};
	void OnKeyDown(wxKeyEvent &event);
	void ActiveVideo(){m_videoBmp->Show(true);};
private:
	FenetrePrincipale	*fen;
    // toggle the state of the status bar controls
    void DoToggle();
    wxBitmap	videoRouge,videoVert;

    wxBitmap CreateBitmapForButton(bool on = false);

    enum
    {
        Champ_Text,
		Champ_Rouge,
		Champ_Vert,
		Champ_Bleu,
		Champ_BitmapZoom,
        Champ_BitmapStat,
        Champ_Checkbox,
        Champ_BitmapVideo,
        Champ_Clock
    };

    wxTimer m_timer;
    wxCheckBox *m_checkbox;
    wxBitmapButton *m_videoBmp;
    wxButton *m_statBmp;
    wxButton *m_zoomBmp;
    wxButton *m_planRouge;
    wxButton *m_planVert;
    wxButton *m_planBleu;

    DECLARE_EVENT_TABLE()
};

// IDs for the controls and the menu commands
enum
{
    // menu items
    OUVRIR_FICHIER =1,
    ENREGISTRER_FICHIER,
    ENREGISTRERSOUS_FICHIER,
	CREER_RAPPORT,
    OUVRIR_FICHIER_HORODATAGE,
    QUITTER_ ,
    REEL_ACQ,
    LANCER_ACQ,
    ARRET_ACQ,
    IMAGE_CAMERA,
	EDGES_MENU,
	SEGMENTATION_HISTOGRAM,
	SEGMENTATION_IMAGE,
	SEGMENTATION_EDGES,
	QUADRIC_FIT,

    // it is important for the id corresponding to the "About" command to have
    // this standard value as otherwise it won't be handled properly under Mac
    // (where it is special and put into the "Apple" menu)
	SOCKET_ID,
    Menu_Help_About = wxID_ABOUT,

    Menu_Popup_ToBeDeleted = 2000,
    Menu_Popup_ToBeGreyed,
    Menu_3D ,
    Menu_Rectangle,
    Menu_Coupe,
    Menu_FilMax,
	Menu_ParAlg,
    Menu_Popup_Palette,
    Menu_Popup_Zoom,
    ARCENCIEL_ = 2684,
    NOIRETBLANC_,
    M_MODULE_, 
    PARTIE_REELLE,
    PARTIE_IMAGINAIRE, 
    LOG_DB, 
    PHASE_RD,

    DEFINITION_FOND_MICRO,
    DEFINITION_FOND_QUADRIQUE,
    ZOOM1SUR2=3000,
    ZOOM1SUR1,
    ZOOM2SUR1,
    ZOOM4SUR1,
    ZOOM8SUR1,
    NEW_LEVEL,
    MERGE_LEVEL,
    RESET_LEVEL,
	MENU_OP1,
	MENU_OP2,
	MENU_EXEC_OP,
	RESET_OP,
	LBnxn,		// ListBox pour filtre nxn
	ROUETAILLE_MORPH,		// SpinCtrl pour taille de l'opérateur morphologie
	LBTY_MORPH,     // ListBox pour forme de l'opérateur
	ID_OP_MORPH,	// Fenetre d'affichage de l'opérateur de morphoogie
	LBIND_MORPH, 
	LBBord,	// Liste box pour la frontiere
	LBPrecision,// Liste box pour la précision
// Id pour les controles de la fenêtre ImageStatistique
	ID_VAL_LUM_ROUGE,
	ID_VAL_LUM_VERT,
	ID_VAL_LUM_BLEU,
	ID_VAL_LUM_CANAL4,
	ID_VAL_LUM_CANAL5,
	ID_VAL_GAIN_ROUGE,
	ID_VAL_GAIN_VERT,
	ID_VAL_GAIN_BLEU,
	ID_VAL_GAIN_CANAL4,
	ID_VAL_GAIN_CANAL5,
	ID_ASC_LUM_ROUGE,
	ID_ASC_LUM_VERT,
	ID_ASC_LUM_BLEU,
	ID_ASC_LUM_CANAL4,
	ID_ASC_LUM_CANAL5,
	ID_ASC_GAIN_ROUGE,
	ID_ASC_GAIN_VERT,
	ID_ASC_GAIN_BLEU,
	ID_ASC_GAIN_CANAL4,
	ID_ASC_GAIN_CANAL5,
	ID_FENETRE_ALGO=4000,


	ID_INUTILSE_DERNIER
};



#endif 
