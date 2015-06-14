#ifndef __PARAMETREOPERATION__
#define __PARAMETREOPERATION__
#include <wx/string.h>
#include <map>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "Panoramique.h"
#include "opencv2/bgsegm.hpp"

class ImageInfoCV;



/*! \class DomaineParametre
   * \brief la classe�DomaineParametre d�finit la valeur, le domaine de d�finition, le pas de la valeur
   * utilis� pour un param�tre de fonctions. Le pas est utilis� pour modifier l'op�ration dans une fen�tre de dialogue.
   */

template <class TypeValeur>
class DomaineParametreOp {
public :
TypeValeur valeur;		/*< Valeur actuelle du param�tre*/
TypeValeur mini,maxi;	/*< Valeur extremum du param�tre */
TypeValeur pas;		/*< Pas entre deux valeurs */
bool res;			/*< Vrai si la valeur est un r�sutat*/
DomaineParametreOp(TypeValeur a, TypeValeur b, TypeValeur c, TypeValeur d) :valeur(a), mini(b), maxi(c), pas(d), res(false){};
DomaineParametreOp(TypeValeur a) :valeur(a), mini(a), maxi(a), pas(a), res(false){};
DomaineParametreOp():valeur(TypeValeur()),mini(TypeValeur()),maxi(TypeValeur()),pas(TypeValeur()),res(false){};
//DomaineParametre():valeur(cv:Point(0,0)),mini(cv:Point(0,0)),maxi(cv:Point(0,0)),pas(cv:Point(0,0)){};
};

/*! \class Parametre
   * \brief la classe�Parametre d�finit les noms et domaine des param�tres n�cessaires pour effectuer une op�ration.
   * les param�tres peuvent �tre du type int, double size ou point (type OpenCV). Un lien sur la doc de la fonction
   * est associ� aux param�tres
   * nbImageRes d�signe le nombre d'image r�sultat.
   */

class ParametreOperation {
void InitParamType();


public :
std::string	nomSequence;
std::string	nomOperation;
std::map<std::string,DomaineParametreOp<double> > doubleParam;
std::map<std::string,DomaineParametreOp<int> > intParam;
std::map<std::string,DomaineParametreOp<cv::Size> > sizeParam;
std::map<std::string, DomaineParametreOp<cv::Point> > pointParam;
std::map<std::string, ImageInfoCV* > imgParam;
std::map<std::string, cv::Ptr<cv::BackgroundSubtractor> > ecartFond;
std::map<std::string, cv::Ptr<cv::Feature2D> >detecteur;
std::vector<Panoramique *> pano;
std::vector<cv::Vec3f> cercle;
std::vector<cv::Vec4i> ligneP;
std::vector<cv::Vec2f> ligne;
std::vector<cv::Point2f> coinRef;
std::string lienHtml;
std::string refPDF;
int nbImageRes;
int nbOperande;
int idOperation;
int indEtape;
int indRes;
bool opVideo;	/*<! vrai lorsque l'op�rateur binaire prend l'image pr�c�dente de  la vid�o */
bool opAttribut;/*<! vrai lorsque l'op�rateur modifie l'une des images sans cr�er une nouvelle image */
int	 opErreur; /*<! non nul lorsqu'une erreur s'est produite */
std::string msgErreur; /*<! message d'erreur */

std::vector <ImageInfoCV *> op;	/*!< Op�rande 1 pour l'op�ration demand�e */
//ParametreOperation pOCV;	/*!< parametre de l'op�rateur Unaire */
std:: vector<int>	indOpFenetre;	/*!< Indice de la fen�tre contenant l'image de op�rande 1 */
//int indOp2Fenetre;  /*!< Indice de la fen�tre contenant l'image de op�rande 2 */
//int indOp3Fenetre;  /*!< Indice de la fen�tre contenant l'image de op�rande 2 */


// Operation cas g�n�ral nombre d'images, les images dans un tableau et les param�tres
std::vector<ImageInfoCV*>  (ImageInfoCV::*operateur)( std::vector<ImageInfoCV *> ,ParametreOperation *) ;

static std::map<std::string,std::map<std::string,int> > listeParam;


explicit ParametreOperation(std::string s);
explicit ParametreOperation(){ InitOperation(""); };
bool InitOperation(std::string s);
bool InitPtrFonction();
std::vector<ImageInfoCV*> ExecuterOperation();
void write(cv::FileStorage& fs) const;                      //Write serialization for this class
void read(const cv::FileNode& node);                          //Read serialization for this class
friend std::ostream& operator << (std::ostream &out, const ParametreOperation&);

};

#ifndef __WIN32__ // en r�lait� C++11
std::string to_string(double x);
#endif

//std::ostream& operator << (std::ostream &out, const ParametreOperation& );

#endif
