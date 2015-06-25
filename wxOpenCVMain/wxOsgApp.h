#ifndef __WXOSGAPP__
#define __WXOSGAPP__

#include "ImageInfo.h"
#include "ParametreOperation.h"
#include <wx/app.h>
#include <wx/bitmap.h>
#include <wx/timer.h>
#include <wx/dynlib.h>
#include <wx/fileconf.h>
#include <string>
#include <map>

//#include <wx/dde.h>
#include "ConnexionDDE.h"

class FenetreZoom;
class FenetrePrincipale;
class ImageStatistiques;
class OutilsImage;
class ControleCamera;
class Operation;

class EnvFenetre // Environnement fenêtre = FenetrePrincipale et zoom
{
public :
FenetrePrincipale	*fPrin;
FenetreZoom			*fZoom;
ImageStatistiques	*fStat;
void				*f3D;
EnvFenetre(FenetrePrincipale *ff,FenetreZoom *ffz,ImageStatistiques *fs){fPrin=ff;fZoom=ffz;fStat=fs;};
};


/* Define a new application type */
class wxOsgApp : public wxApp
{
protected:
wxLanguage	langue;							/*!< language choisi */
wxLocale	locale;							/*!< locale we'll be using */
wxFileConfig *configApp;					/*!< Fichier de configuration de l'application */

std::vector<ParametreOperation> listeOperation;		/*<! Liste des opérations individuelles */
std::map <int,std::vector <ParametreOperation > > tabOperation;	/*!< Tableau des opérations effectuées dans une séquence */
int 	numOpFaite;								/*!< Nombre d'opération faites ou sauvgardées dans le fichier INI */
int     numSeqOpe;								/*!< Nombre de séquence d'opérations faites ou sauvgardées dans le fichier INI */
wxPoint posFenetre;
char	utilisateurAbsent;
bool	quitter;
ServeurScilab		*serveur;
public:
wxCriticalSection m_critsectWork;

private :
#ifdef _DLL_DETECTION__
wxDynamicLibrary *dllAndor;
wxDynamicLibrary *dllplplot;
wxDynamicLibrary *dllWXplplotdrv;
wxDynamicLibrary *dllSVGplplotdrv;
#endif

char	camAndor;	/*!< égal à 1 si une caméra Andor est branchée */
char	camOpenCV;	/*!< égal à 1 si une caméra est détectée par OpenCv */


std::map <int ,EnvFenetre *>	listeFenetre;

int					nbFenetre;
int					indFenetre;

int					indPointeurSouris;		// Précise le type de cursuer
int					modeSouris;				// utilisé pour la sélection d'image

ControleCamera		*ctrlCamera;
OutilsImage			*outils;
void				*fSeqOpe;		// Fenetre permettant d'écécuter plusieurs séquences
void				*fenOpe;		//  Fenetre pour définir plusieurs opérandes

#ifdef __CESTFINII__
// surjection nombre d'images, les images dans un tableau et les paramètres
ImageInfoCV**  (ImageInfoCV::*opSurjecMultiple)(int,ImageInfoCV **,ParametreOperation *) ;
// Opération programmée ternaire : Fusion plan
ImageInfoCV*  (ImageInfoCV::*opNaireSelec)(int ,ImageInfoCV **,ParametreOperation *) ;
// Opération programmée binaire
ImageInfoCV*  (ImageInfoCV::*opBinaireSelec)(ImageInfoCV *,ImageInfoCV *,ParametreOperation *) ;
// Opération programmée unaire avec parametre
ImageInfoCV*  (ImageInfoCV::*opUnaireSelec)(ImageInfoCV *,ParametreOperation *) ;
// Surjection (plusieurs résultats   avec parametre
ImageInfoCV**  (ImageInfoCV::*opSurjecUnaire)(ImageInfoCV *,ParametreOperation *) ;
wxString	nomOperation;
int nbOperande;	/*!< Nombre d'opérande pour l'opération 1 unaire, 2 binaire, 3 fusion plan */
// Opérande sélectionnée
ImageInfoCV *op1;	/*!< Opérande 1 pour l'opération demandée */
ImageInfoCV *op2;	/*!< Opérande 2 pour l'opération demandée */
ImageInfoCV *op3;	/*!< Opérande 3 pour l'opération demandée uniquement fusion plan*/
int	indOp1Fenetre;	/*!< Indice de la fenêtre contenant l'image de opérande 1 */
int indOp2Fenetre;  /*!< Indice de la fenêtre contenant l'image de opérande 2 */
int indOp3Fenetre;  /*!< Indice de la fenêtre contenant l'image de opérande 2 */
#endif
ParametreOperation pOCV;	/*!< parametre de l'opérateur Unaire */


public :	
wxOsgApp() { langue = wxLANGUAGE_UNKNOWN; }
bool OnInit();
void Quitter(wxFrame *g);
void Ouvrir(wxCommandEvent &);	/*!< Ouverture d'un document */	
void InitFenAssociee(FenetrePrincipale *f); /*!< Ouvrir les fenetres associées à la fenetre principale */

void Video(wxCommandEvent &,int);
void Enregistrer(wxCommandEvent& event);

void OuvertureOutils();
void RetirerListe(FenetrePrincipale *);

// Méthodes liées à des opérations sur les images
void DefOperateurImage(wxString &);
void DefEtapeOperation(int x){pOCV.indEtape=x;};
void DefOperande1(ImageInfoCV* im,int i=-1);
void DefOperande2(ImageInfoCV* im,int i=-1);
void DefOperande3(ImageInfoCV* im,int i=-1);
void DefOperandeN(ImageInfoCV* im,int i=-1);
void SupOperandeN(ImageInfoCV* im,int i=-1);

void DefParametreOCV(ParametreOperation &x){pOCV=x;};
bool Operateur(){return pOCV.operateur;} /*!< Vrai si opération unaire sélectionnée */
ImageInfoCV *Op1(){return OpId(0);};
ImageInfoCV *Op2(){return OpId(1);};
ImageInfoCV *Op3(){return OpId(2);};
ImageInfoCV *OpId(int id);
int IndOpId(int id);
int IndOp1(){return IndOpId(0);};
int IndOp2(){return IndOpId(1);};
int IndOp3(){return IndOpId(2);};
int IdFenetreOp1pre();
int NbOperande(){ return pOCV.nbOperande; };
int NumSeqOpe(int *x=NULL){if (x!=NULL) numSeqOpe=*x;return numSeqOpe;};
void AnnuleOp();
std::map <int,std::vector <ParametreOperation > >  *TabSeqOperation(){return &tabOperation;}
std::vector<ImageInfoCV*> ExecuterOperation(ParametreOperation * = NULL);
    /*!
     *  \brief Fonction ExecuterOperation
     *
     *  Execute une opération définie par le pointeur opBinaireSelec ou opUnaireSelec
     *
     */
void CreerFenetreOperation();
    /*!
     *  \brief Fonction CreerFenetreOperation
     *
     *  Création d'une nouvelle fenêtre dont le résultat est le contenu de ExecuterOperation
     *
     */



wxString NomOperation(){return pOCV.nomOperation;};

void	SauverFichierConfig(wxString s,int id,int type=-1,int taille=-1);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'un paramètre dans le fichier de configuration du programme
     *  \param s : nom du paramètre à sauvegarder
	 *  \param id : indice  dépendant du paramètre
	 *  \param type : forme pour la morphologie
	 *  \param taille : taille de l'opérateur 
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, wxString chaine);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une clé dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param chaine : valeur de la cle
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, long v);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une clé dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param v : valeur de la cle
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, double v);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une clé dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param v : valeur de la cle
	 */
void SauverOperationFichierConfig(ParametreOperation &v);
   /*!
     *  \brief Fonction SauverOperationFichierConfig
     *
     *  Sauvegarde d'un objet ParametreOpération dans le fichier de configuration
     *  \param v : objet
	 */

void	LectureFichierConfig();
    /*!
     *  \brief Fonction LectureFichierConfig
     *
     *  Lecture des paramètres contenus dans le fichier de configuration
     */

// Gestion  du curseur souris
void	DefPointeurSouris(int modeSouris=0,int typeSouris=0);
int		PointeurSouris(int x){return indPointeurSouris;};
void	SourisQuitterFen(wxMouseEvent &event);
int		ModeSouris(){return modeSouris;};

//// Operations
void DefOperateurImage(ImageInfoCV* (ImageInfoCV::*f)(ImageInfoCV & ,ImageInfoCV &));
void GenerationGraphDot(ParametreOperation *op);
    /*!
     *  \brief Fonction GenerationGraphDot
     *
     *  Generation du graphe de l'opération dans le langage Dot
     */

    // Tile the bitmap
    bool TileBitmap(const wxRect& rect, wxDC& dc, wxBitmap& bitmap);

//// Accessors
    wxBitmap& GetBackgroundBitmap() const { return (wxBitmap&) m_background; }

    bool GetUseScreen() const { return m_useScreen; }
    void SetUseScreen(bool useScreen) { m_useScreen = useScreen; }

    void OnUseScreen(wxCommandEvent& event);

protected:
    wxBitmap    m_background;
    bool        m_useScreen;

public :	


char	UtilisateurAbsent(){if (utilisateurAbsent>=2)return 1;return 0;};
bool	Quitter(){return quitter;};
void	DefUtilisateurAbsent(char x){if (x==0)utilisateurAbsent=0;utilisateurAbsent+=x;};
void	SauverConfiguration();

int					VerifFenetre(){if (nbFenetre==0 || indFenetre==-1) return 0;else return 1;};
int					NbFenetre(){return nbFenetre;};
void				IdFenetreActive(int x);
int					RechercheFenetre(ImageInfoCV*); /*<! recherche dans quelle fenêtre est associée à l'image */
FenetrePrincipale	*Fenetre(int id){if (id>=0) return Graphique(id);return NULL;};
FenetrePrincipale	*Graphique(int =-1);
ControleCamera		*CtrlCamera(){return ctrlCamera;};
ImageStatistiques	*ImgStat(){if (nbFenetre>0 && indFenetre>=0) return listeFenetre[indFenetre]->fStat;else return NULL;};
OutilsImage			*Outils(){return outils;};
void				*FenetreSeqOpe(void *f){fSeqOpe=f;return fSeqOpe;};
void				*FenetreSeqOpe(){return fSeqOpe;};
void				*FenetreOperande(){return fenOpe;};
void				FenetreOperande(void *f){fenOpe=f;};
ServeurScilab		*Serveur(){return serveur;};

void TracerZoom(wxPoint p);

int FilterEvent(wxEvent& event);
};

#endif
