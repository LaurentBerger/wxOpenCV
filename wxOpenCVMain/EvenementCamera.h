#ifndef _EVENEMENTCAMERA_H_
#define _EVENEMENTCAMERA_H_

#include "ControleCamera.h"
#include "FenetrePrincipale.h"


class ProcessGestionCamera : public wxThread
{
ControleCamera		*ctrlCam;
CameraVirtuelle		*cam;

public:
    ProcessGestionCamera(ControleCamera *,CameraVirtuelle *);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit(){};

public:
    unsigned m_count;
wxMutex				accesBitmap;	// Utilis� pour bloquer la m�moire image
FenetrePrincipale	*parent;		// Pointeur sur la fen�tre principale
wxBitmap			m_bitmap;		// bitmap inutilis�
wxImage				*image;			// Image contenant la vid�o
cv::VideoCapture	*cap;			// Gestionnaire de capture d'OpenCV
};

#endif

