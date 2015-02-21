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

class EnvFenetre // Environnement fen�tre = FenetrePrincipale et zoom
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

std::vector<Operation> listeOperation;		/*<! Liste des op�rations individuelles */
std::map <int,std::vector <Operation > > tabOperation;	/*!< Tableau des op�rations effectu�es dans une s�quence */
int	numOpFaite;											/*!< Nombre d'op�ration faites ou sauvgard�es dans le fichier INI */
int numSeqOpe;								/*!< Nombre de s�quence d'op�rations faites ou sauvgard�es dans le fichier INI */

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

char	camAndor;	/*!< �gal � 1 si une cam�ra Andor est branch�e */
char	camOpenCV;	/*!< �gal � 1 si une cam�ra est d�tect�e par OpenCv */


std::map <int ,EnvFenetre *>	listeFenetre;

int					nbFenetre;
int					indFenetre;

int					indPointeurSouris;		// Pr�cise le type de cursuer
int					modeSouris;				// utilis� pour la s�lection d'image

ControleCamera		*ctrlCamera;
OutilsImage			*outils;
void				*fSeqOpe;

// surjection nombre d'images, les images dans un tableau et les param�tres
ImageInfoCV**  (ImageInfoCV::*opSurjecMultiple)(int,ImageInfoCV **,Parametre *) ;
// Op�ration programm�e binaire
ImageInfoCV*  (ImageInfoCV::*opBinaireSelec)(ImageInfoCV *,ImageInfoCV *,Parametre *) ;
// Op�ration programm�e unaire avec parametre
ImageInfoCV*  (ImageInfoCV::*opUnaireSelec)(ImageInfoCV *,Parametre &) ;
// Surjection (plusieurs r�sultats   avec parametre
ImageInfoCV**  (ImageInfoCV::*opSurjecUnaire)(ImageInfoCV *,Parametre &) ;
wxString	nomOperation;
// Op�rande s�lectionn�e
ImageInfoCV *op1;	/*!< Op�rande 1 pour l'op�ration demand�e */
ImageInfoCV *op2;	/*!< Op�rande 2 pour l'op�ration demand�e */
Parametre pOCV;	/*!< parametre de l'op�rateur Unaire */
int	indOp1Fenetre;	/*!< Indice de la fen�tre contenant l'image de op�rande 1 */
int indOp2Fenetre;  /*!< Indice de la fen�tre contenant l'image de op�rande 2 */


public :	
wxOsgApp() { langue = wxLANGUAGE_UNKNOWN; }
bool OnInit();
void Quitter(wxFrame *g);
void Ouvrir(wxCommandEvent &);
void Video(wxCommandEvent &,int);
void Enregistrer(wxCommandEvent& event);

void OuvertureOutils();
void RetirerListe(FenetrePrincipale *);

// M�thodes li�es � des op�rations sur les images
void DefOperateurImage(wxString &);
void DefOperande1(ImageInfoCV* im,int i=-1){op1=im;indOp1Fenetre=i;};
void DefOperande2(ImageInfoCV* im,int i=-1){op2=im;indOp2Fenetre=i;};
void DefParametreOCV(Parametre &x){pOCV=x;};
bool OpUnaire(){return opUnaireSelec!=NULL || opSurjecUnaire!=NULL;} /*!< Vrai si op�ration unaire s�lectionn�e */
bool OpBinaire(){return opBinaireSelec!=NULL;} /*!< Vrai si op�ration binaire s�lectionn�e */
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
     *  Execute une op�ration d�finie par le pointeur opBinaireSelec ou opUnaireSelec
     *
     */
void CreerFenetreOperation();
    /*!
     *  \brief Fonction CreerFenetreOperation
     *
     *  Cr�ation d'une nouvelle fen�tre dont le r�sultat est le contenu de ExecuterOperation
     *
     */


void Erosion(wxCommandEvent &w);

wxString *NomOperation(){return &nomOperation;};

void	SauverFichierConfig(wxString s,int id,int type=-1,int taille=-1);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'un param�tre dans le fichier de configuration du programme
     *  \param s : nom du param�tre � sauvegarder
	 *  \param id : indice  d�pendant du param�tre
	 *  \param type : forme pour la morphologie
	 *  \param taille : taille de l'op�rateur 
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, wxString chaine);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une cl� dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param chaine : valeur de la cle
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, long v);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une cl� dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param v : valeur de la cle
	 */
void	SauverFichierConfig(wxString chemin,wxString cle, double v);
    /*!
     *  \brief Fonction SauverFichierConfig
     *
     *  Sauvegarde d'une cl� dans le fichier de configuration
     *  \param chemin : chemin de la cle
     *  \param cle : nom d'une cle
     *  \param v : valeur de la cle
	 */

void	LectureFichierConfig();
    /*!
     *  \brief Fonction LectureFichierConfig
     *
     *  Lecture des param�tres contenus dans le fichier de configuration
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
     *  Generation du graphe de l'op�ration dans le langage Dot
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
int					RechercheFenetre(ImageInfoCV*); /*<! recherche dans quelle fen�tre est associ�e � l'image */
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
