// Le bloc ifdef suivant est la fa�on standard de cr�er des macros qui facilitent l'exportation 
// � partir d'une DLL. Tous les fichiers contenus dans cette DLL sont compil�s avec le symbole DLLSCILABOCV_EXPORTS
// d�fini sur la ligne de commande. Ce symbole ne doit pas �tre d�fini dans les projets
// qui utilisent cette DLL. De cette mani�re, les autres projets dont les fichiers sources comprennent ce fichier consid�rent les fonctions 
// DLLSCILAB_API comme �tant import�es � partir d'une DLL, tandis que cette DLL consid�re les symboles
// d�finis avec cette macro comme �tant export�s.
#ifdef _WINDOWS
#ifdef _USRDLL
#define DLLSCILABOCV_EXPORTS __declspec(dllexport)
#else 
#define DLLSCILABOCV_EXPORTS __declspec(dllimport)
#endif //DLLSCILABPORTAUDIO_EXPORTS
#else
#define DLLSCILABOCV_EXPORTS
#endif //_WINDOWS



// Cette classe est export�e de DllScilab.dll
#include <vector>
#include "../wxOpenCVMain/ImageInfo.h"



 class  DLLSCILABOCV_EXPORTS GateWayOCV {
    std::vector<ImageInfoCV*> tabIm; 

public :
    GateWayOCV(){tabIm.push_back(new ImageInfoCV());};
    int Lecture(int &index){return 2*index;};
};


