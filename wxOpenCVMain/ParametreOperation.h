#ifndef __PARAMETREOPERATION__
#define __PARAMETREOPERATION__
#include <wx/string.h>
#include <map>
#include <string>
#include "opencv2/opencv.hpp" 

class ImageInfoCV;

using namespace std;


/*! \class DomaineParametre
   * \brief la classe¨DomaineParametre définit la valeur, le domaine de définition, le pas de la valeur
   * utilisé pour un paramètre de fonctions. Le pas est utilisé pour modifier l'opération dans une fenêtre de dialogue.
   */

template <class TypeValeur>
class DomaineParametreOp {
public :
TypeValeur valeur;		/*< Valeur actuelle du paramètre*/
TypeValeur mini,maxi;	/*< Valeur extremum du paramètre */
TypeValeur pas;		/*< Pas entre deux valeurs */
DomaineParametreOp(TypeValeur a,TypeValeur b,TypeValeur c,TypeValeur d):valeur(a),mini(b),maxi(c),pas(d){};
DomaineParametreOp(TypeValeur a):valeur(a),mini(a),maxi(a),pas(a){};
DomaineParametreOp():valeur(TypeValeur()),mini(TypeValeur()),maxi(TypeValeur()),pas(TypeValeur()){};
//DomaineParametre():valeur(cv:Point(0,0)),mini(cv:Point(0,0)),maxi(cv:Point(0,0)),pas(cv:Point(0,0)){};
};

/*! \class Parametre
   * \brief la classe¨Parametre définit les noms et domaine des paramètres nécessaires pour effectuer une opération.
   * les paramètres peuvent être du type int, double size ou point (type OpenCV). Un lien sur la doc de la fonction 
   * est associé aux paramètres
   * nbImageRes désigne le nombre d'image résultat.  
   */

class ParametreOperation {
void InitParamType();


public :
wxString	nomSequence;
wxString	nomOperation;
std::map<std::string,DomaineParametreOp<double> > doubleParam;
std::map<std::string,DomaineParametreOp<int> > intParam;
std::map<std::string,DomaineParametreOp<cv::Size> > sizeParam;
std::map<std::string,DomaineParametreOp<cv::Point> > pointParam;
std::string lienHtml;
std::string refPDF;
int nbImageRes;	
int nbOperande;
int idOperation;
int indEtape;
int indRes;

ImageInfoCV *op1;	/*!< Opérande 1 pour l'opération demandée */
ImageInfoCV *op2;	/*!< Opérande 2 pour l'opération demandée */
ImageInfoCV *op3;	/*!< Opérande 3 pour l'opération demandée uniquement fusion plan*/
//ParametreOperation pOCV;	/*!< parametre de l'opérateur Unaire */
int	indOp1Fenetre;	/*!< Indice de la fenêtre contenant l'image de opérande 1 */
int indOp2Fenetre;  /*!< Indice de la fenêtre contenant l'image de opérande 2 */
int indOp3Fenetre;  /*!< Indice de la fenêtre contenant l'image de opérande 2 */


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

static std::map<wxString,std::map<wxString,int> > listeParam;


ParametreOperation(string s);	
ParametreOperation(){InitOperation("");};	
bool InitOperation(string s);
bool InitPtrFonction();
};

/*
string nomOperation[]={
	"PartageEaux","SeparationPlan","FusionPlan","Addition","Soustraction",
	"Multiplication","Division","Convolution","Dilatation","Erosion","Ouverture",
	"Fermeture","ChapeauHaut","ChapeauBas","GradMorph","Scharr_mod","Scharr_x",
	"Scharr_y","Laplacien","Canny","Contour","FFT","IFFT","Seuillage",
	"AdaptatifSeuillage","LissageMedian","LissageMoyenneur","LissageGaussien",
	"ComposanteConnexe","DistanceDiscrete","LigneMediane"};						
*/
#endif
