#ifndef __NANOSURFACE__
#define __NANOSURFACE__


/** \file  NanoSurface.h
*/

#include <wx/colour.h>
#include <osg/ShapeDrawable>
#include <osg/Geode>
#include <osg/Texture2D>

#include <deque>

#include "ImageInfo.h"

/**  NanoSurface : classe permettant de tracer en 3D les donn&eacute;es issues d'un système mesurant 
z=f(t,x,y) et \f$\stackrel{\rightarrow}{u}=\left[\right]\f$. 
La surface affichée est obtenue à partir d'une matrice d'altitudes et d'une image.  
dont les dimensions, le nombre de lignes et de colonnes, sont identiques. Lors de l'affichage 
l'image est plaquée sur la surface. La mise à jour de la surface affichée se fait soit en temps réel, soit hors ligne 
selon le paramétrage choisie par l'utilisateur.
*/

#define TAILLEMAXDUFILTRE 11

class NanoSurface: public osg::Geode
{
osg::ref_ptr<osg::ShapeDrawable>	surface[TAILLEMAXDUFILTRE];		
osg::ref_ptr<osg::ShapeDrawable>	surfacev[TAILLEMAXDUFILTRE];		
osg::ref_ptr<osg::ShapeDrawable>	surfaceb[TAILLEMAXDUFILTRE];		
	/*!< Surface à tracer */
std::deque<osg::HeightField*>	grid;		/*!< Matrices des altitudes dépendant du temps ou composante rouge*/
std::deque<osg::HeightField*>	gridv;		/*!< Matrices des altitudes pour composante verte */
std::deque<osg::HeightField*>	gridb;		/*!< Matrices des altitudes pour composante bleue */
osg::HeightField		*gridAffichee[TAILLEMAXDUFILTRE];
	/*!< Matrices des altitude affichées. Taille des matrices prévues pour zoom */
osg::HeightField		*gridAfficheeg[TAILLEMAXDUFILTRE];
	/*!< Matrices des altitude affichées. Taille des matrices prévues pour zoom */
osg::HeightField		*gridAfficheeb[TAILLEMAXDUFILTRE];
	/*!< Matrices des altitude affichées. Taille des matrices prévues pour zoom */
bool					gridMAJ[3][TAILLEMAXDUFILTRE];
std::deque<osg::Image*> imSurface;	/*!< Images à plaquer sur la surface */
osg::Image 				*imTexture;		/*!< Image associée à la texture */
osg::Texture2D			*tex[TAILLEMAXDUFILTRE];
	/*!< Textures construites à partir des images */
osg::StateSet		*etat;			/*!< A FINIR*/

std::deque<int>		imValide;		/*!< Vrai : fichier image en mémoire */
std::deque<int>		topoValide;		/*!< Vrai : fichier topographie en mémoire */

std::deque<char*>	tabNomFichier;/*!< Nom du fichier zi=fi(x,y) pour l'image ind */
std::deque<char*>	tabNomImage;	/*!< Nom du fichier bitmap (R,G,B)=Ii(x,y) pour l'image ind */
char				couleurActive;	/*!< Composante couleur sélectionnée (R,G,B)*/

int	nbL;				/*!< Nombre de lignes des surfaces et images */
int nbC;				/*!< Nombre de colonnes des surfaces et images */
int	nbGrid;				/*!< Nombre de surfaces dans la séquence temporelle */
int	indGrid;			/*!< Indice (variable t dans z=f(t,x,y)) de la surface actuellement affichée */
int	tailleDuFiltre;		/*!< Valeur de taille du filtre pour le lissage des données */
int	imageFondPresente;	/*!< Valeur 1 la première image est le fond pour la correction, 2 un polynome */
ImageInfoCV	*imFond;	/*!< Image du fond lorqu'elle est présente dans la série */
float	echX;			/*!< Echelle selon X */
float	echY;			/*!< Echelle selon Y */
float	echZ;			/*!< Echelle selon Z */
float	offsetZ0;		/*!< offset pour définir le niveau 0 utilisé pour la palette de couleur*/
int		zoom;			/*!< zoom variable utilisée pour le lissage */

void	*osgApp;
double	**poly;			/*!< Quadrique à utiliser pour corriger les images */


int					debNivPalette;		// Début du niveau associés à la couleur 0 
int					finNivPalette;		// Fin du niveau associés à la dernière couleur de la palette 
int					indPalette;
int					nbCouleurPalette;	// Nombre de couleur dans la palette

wxColor				*pCouleur;
wxColor				*pAleatoire;
wxColor				*pJet;
wxColor				*pRainbow;
wxColor				*pPerso;
wxColor				*pPersoInv;


void InitNanoSurface(); /*!< Initialisation des paramètres */


public :
NanoSurface();			/*!< Constructeur par défaut utilisé pour les tests*/
NanoSurface(char *nomSequence );
NanoSurface::NanoSurface(int ,ImageInfoCV **);
void InitPalette(int nbCouleur);
void SelectPalette(int ind);
int	LireNbCouleur(){return nbCouleurPalette;};
wxColor *Lirepalette(){return pCouleur;};

void LireSequence(char *nomSequence);	/*!< Lire une séquence à partir d'un nom de fichier */

						/*!< Constructeur initialisant la séquence d'affichage */
osg::ref_ptr<osg::ShapeDrawable> LitShapeDrawable();
int LireFichier(char *nomFichier,int ind);
						/*!< Lecture d'une matrice de surface */
int LireFichierImage(char *nomFichier,int ind);
						/*!< Lecture d'une image */
void Maj(int d);		/*!< Mise à jour de la surface affichée lors d'un changement  de variable t */
osg::Texture2D *LitTexture(){return tex[tailleDuFiltre];};

void AjouteImage(osg::Image *);
void AjouteImage(ImageInfoCV *im);
void AjouteImage(ImageInfoCV **);
int	ImageExiste(int ind);
int	TopographieExiste(int ind);
int NbLigneImage(int ind);
int NbColonneImage(int ind);
void DefDebPalette(int x){debNivPalette=x;};
void DefFinPalette(int x){finNivPalette=x;};
int DebPalette(){return debNivPalette;};
int FinPalette(){return finNivPalette;};
int	IndGrid(){return indGrid;};
char	*LireNomFichier(int ind);
char	*LireNomFichierImage(int ind);
void	VerificationGrille();
int		LireNombreFichier(){return nbGrid;};
double	LireHauteur(int,int,int =-1);
float	EchX(){return echX;};
float	EchY(){return echY;};
float	EchZ(){return echZ;};
float	OffsetZ0(){return offsetZ0;};
int		LireTailleFiltre(){return tailleDuFiltre;};
void	DefOSGApp(void* x){osgApp=x;};
void	DefEchX(float x){echX=x;};
void	DefEchY(float x){echY=x;};
void	DefEchZ(float x){echZ=x;};
void	DefOffsetZ0(float x){offsetZ0=x;};
void	DefTailleFiltre(int i);
void	SelectionComposante(char c);


};

#endif
