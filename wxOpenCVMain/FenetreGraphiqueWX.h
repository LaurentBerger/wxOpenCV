#ifndef _FENETREGRAPHIQUEWX_H_
#define _FENETREGRAPHIQUEWX_H_

#include "wx/defs.h"
#include "wx/app.h"
#include "wx/cursor.h"
#include <wx/glcanvas.h>
#include <osgViewer/Viewer>
#include <string>
#include "wx/sound.h"
#include "wx/joystick.h"
#include "NanoSurface.h"
#include "PlanReference.h"
#include "wx/timer.h"


/**  La classe FenetreGraphiqueWX permet l'affichage 3D d'une surface en utilisant OpenGL.
Cette classe hérite de wxGLCanvas et de osgViewer::GraphicsWindow. La scène affichée est 
composé d'une surface z=f(t,x,y) (voir classe nanosurface), d'un plan de référence, 
et d'une échelle. La variable t peut être modifié en utilisant les touches + et -.
Le plan de référence a une hauteur z qui peut être modifié en utilisant 
les touches clavier w et x. La transparence de ce plan est de même modifiable en utilisant la touche t
La position d'observation de la scène est ajustable en utilisant la souris (ou un joystick avec des bugs).
*/
class FenetreGraphiqueWX : public wxGLCanvas
{
protected :

wxSound			m_fire;
NanoSurface		*surface; 
PlanReference	*surfaceReference;
void			*osgApp;
void			*fenParent;

wxGLContext		*glContexte;

wxTimer				*horlogeSeq;
wxTimer				*horlogeCopy;
int					basculeCopyAvance;

    osg::ref_ptr<osgViewer::GraphicsWindow> _graphics_window;

public:
    FenetreGraphiqueWX(wxWindow *parent, wxWindowID id = wxID_ANY,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize, long style = 0,
        const wxString& name = wxT("TestGLCanvas"),
        int *attributes = 0);

    ~FenetreGraphiqueWX();
    
    void SetGraphicsWindow(osgViewer::GraphicsWindow *gw)   { _graphics_window = gw; }
    void init();
	void DefOSGApp(void *w){osgApp=w;};
	void DeffParent(void *w){fenParent=w;};

    void OnPaint(wxPaintEvent& event);				/*!< Tracer de la fenetre graphique */
    void OnSize(wxSizeEvent& event);				/*!< Gestion du redimensionement de la fenetre */
    void OnEraseBackground(wxEraseEvent& event);
	void OnMouseEnter(wxMouseEvent &event);			/*!< Focus pour la fenetre */
    void OnChar(wxKeyEvent &event);				/*!< Gestion du clavier */
    void OnKeyDown(wxKeyEvent &event);				/*!< Gestion du clavier */
    void OnKeyUp(wxKeyEvent &event);				/*!< Gestion du clavier */
    void OnMouse(wxMouseEvent &event);				/*!< Gestion déplacement souris */
	void OnJoystickEvent(wxJoystickEvent& event);	/*!< Gestion du joystickTimer pour vidéo */
	void OnMouseWheel(wxMouseEvent &event);			/*!< Gestion de la molette */
	void OnTimer(wxTimerEvent& event);		/*!< Gestion du joystickTimer pour vidéo */
	void OnTimerCopy(wxTimerEvent& event);		/*!< Gestion du joystickTimer pour vidéo */
	void MAJ(int d);								/*!< Mise à jour du graphique suite à un changement */
    void UseCursor(bool value);

    //
    // GraphicsWindow interface
    //
    void useCursor(bool cursorOn);

	
    // 
    void DefSurfaceReference(PlanReference *s){surfaceReference =s;};
    void DefSurface(NanoSurface *s){surface =s;};

private:
    wxCursor _oldCursor;

};


class GraphicsOSGWX : public osgViewer::GraphicsWindow
{
public:
    GraphicsOSGWX(FenetreGraphiqueWX *canvas);
    ~GraphicsOSGWX();

    void init();

    //
    // GraphicsWindow interface
    //
    void grabFocus();
    void grabFocusIfPointerInWindow();
    void useCursor(bool cursorOn);

    bool makeCurrentImplementation();
    void swapBuffersImplementation();

    // not implemented yet...just use dummy implementation to get working.
    virtual bool valid() const { return true; }
    virtual bool realizeImplementation() { return true; }
    virtual bool isRealizedImplementation() const  { return _canvas->IsShownOnScreen(); }
    virtual void closeImplementation() {}
    virtual bool releaseContextImplementation() { return true; }

private:
    // XXX need to set _canvas to NULL when the canvas is deleted by
    // its parent. for this, need to add event handler in OSGCanvas
    FenetreGraphiqueWX*  _canvas;
};




#endif

