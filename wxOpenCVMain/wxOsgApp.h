#ifndef __WXOSGAPP__
#define __WXOSGAPP__

#include "ImageInfo.h"
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

std::vector<Operation> listeOperation;		/*<! Liste des opérations individuelles */
std::map <int,std::vector <Operation > > tabOperation;	/*!< Tableau des opérations effectuées dans une séquence */
int	numOpFaite;											/*!< Nombre d'opération faites ou sauvgardées dans le fichier INI */
int numSeqOpe;								/*!< Nombre de séquence d'opérations faites ou sauvgardées dans le fichier INI */

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
void				*fSeqOpe;

// surjection nombre d'images, les images dans un tableau et les paramètres
ImageInfoCV**  (ImageInfoCV::*opSurjecMultiple)(int,ImageInfoCV **,Parametre *) ;
// Opération programmée binaire
ImageInfoCV*  (ImageInfoCV::*opBinaireSelec)(ImageInfoCV *,ImageInfoCV *,Parametre *) ;
// Opération programmée unaire avec parametre
ImageInfoCV*  (ImageInfoCV::*opUnaireSelec)(ImageInfoCV *,Parametre &) ;
// Surjection (plusieurs résultats   avec parametre
ImageInfoCV**  (ImageInfoCV::*opSurjecUnaire)(ImageInfoCV *,Parametre &) ;
wxString	nomOperation;
// Opérande sélectionnée
ImageInfoCV *op1;	/*!< Opérande 1 pour l'opération demandée */
ImageInfoCV *op2;	/*!< Opérande 2 pour l'opération demandée */
Parametre pOCV;	/*!< parametre de l'opérateur Unaire */
int	indOp1Fenetre;	/*!< Indice de la fenêtre contenant l'image de opérande 1 */
int indOp2Fenetre;  /*!< Indice de la fenêtre contenant l'image de opérande 2 */


public :	
wxOsgApp() { langue = wxLANGUAGE_UNKNOWN; }
bool OnInit();
void Quitter(wxFrame *g);
void Ouvrir(wxCommandEvent &);
void Video(wxCommandEvent &,int);
void Enregistrer(wxCommandEvent& event);

void OuvertureOutils();
void RetirerListe(FenetrePrincipale *);

// Méthodes liées à des opérations sur les images
void DefOperateurImage(wxString &);
void DefOperande1(ImageInfoCV* im,int i=-1){op1=im;indOp1Fenetre=i;};
void DefOperande2(ImageInfoCV* im,int i=-1){op2=im;indOp2Fenetre=i;};
void DefParametreOCV(Parametre &x){pOCV=x;};
bool OpUnaire(){return opUnaireSelec!=NULL || opSurjecUnaire!=NULL;} /*!< Vrai si opération unaire sélectionnée */
bool OpBinaire(){return opBinaireSelec!=NULL;} /*!< Vrai si opération binaire sélectionnée */
ImageInfoCV *Op1(){return op1;};
ImageInfoCV *Op2(){return op2;};
int IndOp1(){return indOp1Fenetre;};
int IndOp2(){return indOp2Fenetre;};
void AnnuleOp();
std::map <int,std::vector <Operation > >  *TabSeqOperation(){return &tabOperation;}
ImageInfoCV **ExecuterOperation(Parametre * = NULL);
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


void Erosion(wxCommandEvent &w);

wxString *NomOperation(){return &nomOperation;};

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
void GenerationGraphDot(Operation *op);
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
void				IdFenetreActive(int x);
int					RechercheFenetre(ImageInfoCV*); /*<! recherche dans quelle fenêtre est associée à l'image */
FenetrePrincipale	*Fenetre(int id){if (id>=0) return Graphique(id);return NULL;};
FenetrePrincipale	*Graphique(int =-1);
ControleCamera		*CtrlCamera(){return ctrlCamera;};
ImageStatistiques	*ImgStat(){if (nbFenetre>0 && indFenetre>=0) return listeFenetre[indFenetre]->fStat;else return NULL;};
OutilsImage			*Outils(){return outils;};
void				*FenetreSeqOpe(void *f){fSeqOpe=f;return fSeqOpe;};
void				*FenetreSeqOpe(){return fSeqOpe;};
ServeurScilab		*Serveur(){return serveur;};

void TracerZoom(wxPoint p);

int FilterEvent(wxEvent& event);
};

#endif
