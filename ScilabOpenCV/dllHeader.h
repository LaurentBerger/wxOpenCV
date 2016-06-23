// Le bloc ifdef suivant est la façon standard de créer des macros qui facilitent l'exportation 
// à partir d'une DLL. Tous les fichiers contenus dans cette DLL sont compilés avec le symbole DLLSCILABOCV_EXPORTS
// défini sur la ligne de commande. Ce symbole ne doit pas être défini dans les projets
// qui utilisent cette DLL. De cette manière, les autres projets dont les fichiers sources comprennent ce fichier considèrent les fonctions 
// DLLSCILAB_API comme étant importées à partir d'une DLL, tandis que cette DLL considère les symboles
// définis avec cette macro comme étant exportés.
#ifdef _WINDOWS
#ifdef _USRDLL
#define DLLSCILABOCV_EXPORTS __declspec(dllexport)
#else 
#define DLLSCILABOCV_EXPORTS __declspec(dllimport)
#endif //DLLSCILABPORTAUDIO_EXPORTS
#else
#define DLLSCILABOCV_EXPORTS
#endif //_WINDOWS



// Cette classe est exportée de DllScilab.dll
#include <vector>
#include "../wxOpenCVMain/ImageInfo.h"



 class  DLLSCILABOCV_EXPORTS GateWayOCV {
    std::vector<ImageInfoCV*> tabIm; 

public :
    GateWayOCV(){tabIm.push_back(new ImageInfoCV());};
    int Lecture(int &index){return 2*index;};
};


