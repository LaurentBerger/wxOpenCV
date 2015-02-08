#ifndef _EVENEMENTCAMERA_H_
#define _EVENEMENTCAMERA_H_

#include "ControleCamera.h"
#include "FenetrePrincipale.h"


class ProcessGestionCamera : public wxThread
{
ControleCamera		*ctrlCam;
CameraVirtuelle		*cam;
int					tacheAFaire; /*<! Tache à faire, 1 Accumulation d'image CV_8UC3, 2 Accumulation d'image CV_32FC3 */
void				*fenParent;

public:
    ProcessGestionCamera(ControleCamera *,CameraVirtuelle *);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit(){};

public:
    unsigned m_count;
cv::VideoCapture	*cap;			// Gestionnaire de capture d'OpenCV ouvert par CameraOpenCV
};

#endif

