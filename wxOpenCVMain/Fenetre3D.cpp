#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgGA/TrackballManipulator>
#include <osgSim/ColorRange>
#include <osgSim/SphereSegment>
#include <osgUtil/Optimizer>
#include <osgViewer/ViewerEventHandlers>
#include <osg/Billboard>
#include <osg/LineWidth>

#include "Fenetre3D.h"
#include "BarreEchelle.h"
#include "FenetrePrincipale.h"




void Fenetre3D::ActiveGeode(int i)
{
cInfo3D->ActiveGeode(i);
}

void Fenetre3D::DeplaceCurseur(int i,int j)
{
//parametrage->DefPositionCurseur(parametrage->IndCurseurActif(),x,y,z);
//graphique->DeplaceCurseur(x,y,z);
//MAJCurseur();
int ind=cInfo3D->LireActiveGeode();
cInfo3D->DeplaceCurseur(0,0,0,viewer);
};

void Fenetre3D::MAJGraphiqueNanoSurface(int d)
{
fgOSGWX->MAJ(d);
};

Fenetre3D::Fenetre3D(wxFrame *frame, const wxString& title, const wxPoint& pos, 
    const wxSize& size, long style ): wxFrame(NULL, wxID_ANY, title, pos, size, style)
{
int width = 600;
int height = 600;
GetClientSize(&width, &height);
width = 600;
height = 600;



return ;
}



void Fenetre3D::SetViewer(osgViewer::Viewer *v)
{
    viewer = v;
}

void Fenetre3D::OnIdle(wxIdleEvent &event)
{
if (!viewer->isRealized())
    return;

    {
    viewer->frame();
    }

event.RequestMore();
}

void Fenetre3D::OnClose(wxCloseEvent& event)
{
((FenetrePrincipale*)fenParent)->F3D(NULL);
Unbind(wxEVT_IDLE,&Fenetre3D::OnIdle,this);
viewer->removeEventHandler(cInfo3D);
listeObjets->removeChild(imageSurface);
listeObjets->removeChild(legendePalette);
listeObjets->removeChild(surfaceReference);
listeObjets->removeChild(systemAxe);
//delete surfaceReference;
//delete surface;
//delete viewer;

    

wxFrame::OnCloseWindow(event);
}


void Fenetre3D::InstallGraphiquePhase1(FenetreGraphiqueWX *osg,GraphicsOSGWX *gOSG)
{

    int width, height;
    GetClientSize(&width, &height);


fgOSGWX = osg;
fgOSGWX->DefOSGApp((void*)osgApp);



// construct the viewer.
double fovy,aspectRatio,zNear,zFar;
viewer= new osgViewer::Viewer;
viewer->getCamera()->setGraphicsContext(gOSG);
viewer->getCamera()->setViewport(0,0,width,height);
viewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,zNear,zFar);
viewer->getCamera()->setProjectionMatrixAsPerspective(fovy,1,zNear,zFar);

// add the stats handler
viewer->addEventHandler(new osgViewer::StatsHandler);
viewer->setThreadingModel(osgViewer::Viewer::ThreadPerContext);


wxGLContext *glContexte =new wxGLContext(fgOSGWX);
//fgOSGWX->SetCurrent(*glContexte);

ImageInfoCV *tabImage[1];
tabImage[0]=((FenetrePrincipale*)fenParent)->ImAcq();
surface = new NanoSurface(1,tabImage,fenParent);
	surface->addDrawable(surface->LitShapeDrawable().get());
	imageSurface = (osg::Group *)surface;
	fgOSGWX->DefSurface(surface);
	surface->Maj(0);



return ;
}


void Fenetre3D::InstallGraphique(char *nomFichier)
{

}


void Fenetre3D::MAJAxe()
{
bool x;

x=listeObjets->getChildValue(systemAxe);
listeObjets->removeChild(systemAxe);
systemAxe=CreerTriede();
listeObjets->addChild(systemAxe,x);
}

void Fenetre3D::MAJEchelleCouleur(float deb,float fin)
{
osg::Group *group=(osg::Group *)viewer->getSceneData();
	

osg::MatrixTransform* echelle=(osg::MatrixTransform*)group->getChild(1);
osgSim::ScalarBar *g= (osgSim::ScalarBar *)(((osg::MatrixTransform*)(echelle->getChild(0)))->getChild(0));


wxColor*p=surface->Lirepalette();

std::vector< osg::Vec4 > paletteTopographique;
for (int i=0;i<=255;i++)
	paletteTopographique.push_back( osg::Vec4(p[i*64].Red()/255.0f,p[i*64].Green()/255.0f,p[i*64].Blue()/255.0f,1.0f));
osgSim::ColorRange* cr = new osgSim::ColorRange(deb,fin,paletteTopographique);


g->setScalarsToColors(cr);

	g->update();


} 

void Fenetre3D::InstallGraphiquePhase2(osg::Group *group)
{
	
legendePalette=CreateScalarBar_HUD();
systemAxe=CreerTriede();

// load the nodes from the commandline arguments.
surfaceReference= new PlanReference(osgApp,this);
if (!surfaceReference)
	return ;
fgOSGWX->DefSurfaceReference(surfaceReference);
//group->addChild(echelle);
//group->addChild(surfaceReference);
//group->addChild(pyr);

if (listeObjets==NULL)
	listeObjets = new osg::Switch;
listeObjets->addChild(imageSurface,true);
listeObjets->addChild(legendePalette,false);
listeObjets->addChild(surfaceReference,false);
listeObjets->addChild(systemAxe,false);
viewer->setSceneData(listeObjets);

    
//viewer->setSceneData(group);
viewer->setCameraManipulator(new osgGA::TrackballManipulator);
//cInfo3D = new CaptureInfo3D(group);
cInfo3D = new CaptureInfo3D(listeObjets);
cInfo3D->DefOSGApp(osgApp);
viewer->addEventHandler(cInfo3D);
Bind(wxEVT_IDLE,&Fenetre3D::OnIdle,this);
Bind(wxEVT_CLOSE_WINDOW,&Fenetre3D::OnClose,this);
}

void Fenetre3D::MAJNoeud(int key)
{
bool x;
switch (key){
case '1':
	x=listeObjets->getChildValue(imageSurface);
	listeObjets->setChildValue(imageSurface,!x);
	break;
case '2':
	x=listeObjets->getChildValue(legendePalette);
	listeObjets->setChildValue(legendePalette,!x);
	break;
case '3':
	x=listeObjets->getChildValue(surfaceReference);
	listeObjets->setChildValue(surfaceReference,!x);
	break;
case '4':
	x=listeObjets->getChildValue(systemAxe);
	listeObjets->setChildValue(systemAxe,!x);
	break;
	}
}
