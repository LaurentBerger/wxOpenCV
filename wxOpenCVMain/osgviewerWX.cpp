#include "wx/wxprec.h"

#ifdef __BORLANDC__
#pragma hdrstop
#endif

#ifndef WX_PRECOMP
#include "wx/wx.h"
#endif


#include <wx/image.h>
#include <wx/menu.h>


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

#include <iostream>
#include <stdlib.h>



#include <sstream>
#include <string>
#include <iostream>
#include <math.h>

#include "FenetreGraphiqueWX.h"
#include "CaptureInfo3D.h"

using namespace osgSim;
using namespace std;







BEGIN_EVENT_TABLE(FenetreGraphiqueWX, wxGLCanvas)
    EVT_SIZE            (FenetreGraphiqueWX::OnSize)
    EVT_PAINT            (FenetreGraphiqueWX::OnPaint)
    EVT_ERASE_BACKGROUND(FenetreGraphiqueWX::OnEraseBackground)
    EVT_KEY_DOWN        (FenetreGraphiqueWX::OnKeyDown)
    EVT_KEY_UP            (FenetreGraphiqueWX::OnKeyUp)
    EVT_MOUSE_EVENTS    (FenetreGraphiqueWX::OnMouse)
 	EVT_TIMER(1, FenetreGraphiqueWX::OnTimer)
 	EVT_TIMER(2, FenetreGraphiqueWX::OnTimerCopy)
 	
   EVT_JOYSTICK_EVENTS(FenetreGraphiqueWX::OnJoystickEvent)
END_EVENT_TABLE()


typedef std::vector< osg::ref_ptr<osg::Image> > ImageList;

/** create quad at specified position. */
osg::Drawable* FenetrePrincipale::CreateSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image)
{
    // set up the Geometry.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array(4);
    (*coords)[0] = corner;
    (*coords)[1] = corner+width;
    (*coords)[2] = corner+width+height;
    (*coords)[3] = corner+height;


    geom->setVertexArray(coords);

    osg::Vec3Array* norms = new osg::Vec3Array(1);
    (*norms)[0] = width^height;
    (*norms)[0].normalize();
    
    geom->setNormalArray(norms);
    geom->setNormalBinding(osg::Geometry::BIND_OVERALL);

    osg::Vec2Array* tcoords = new osg::Vec2Array(4);
    (*tcoords)[0].set(0.0f,0.0f);
    (*tcoords)[1].set(1.0f,0.0f);
    (*tcoords)[2].set(1.0f,1.0f);
    (*tcoords)[3].set(0.0f,1.0f);
    geom->setTexCoordArray(0,tcoords);
    
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,4));
    
    if (image)
    {
        osg::StateSet* stateset = new osg::StateSet;
        osg::Texture2D* texture = new osg::Texture2D;
        texture->setImage(image);
        stateset->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
        geom->setStateSet(stateset);
    }
    
    return geom;
}


osg::Drawable* FenetrePrincipale::CreerLigne(const osg::Vec3& corner,const osg::Vec3& xdir,float epaisseurTrait)
{
    // set up the Geometry.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array(2);
    (*coords)[0] = corner;
    (*coords)[1] = corner+xdir;

    geom->setVertexArray(coords);

    osg::Vec4 x_color(0.0f,1.0f,1.0f,1.0f);

    osg::Vec4Array* color = new osg::Vec4Array(2);
    (*color)[0] = x_color;
    (*color)[1] = x_color;
    
    geom->setColorArray(color);
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
    
    osg::StateSet* stateset = new osg::StateSet;
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(epaisseurTrait);
    stateset->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    geom->setStateSet(stateset);
    
    return geom;
}


osg::Drawable* FenetrePrincipale::CreateAxis(const osg::Vec3& corner,const osg::Vec3& xdir,const osg::Vec3& ydir,const osg::Vec3& zdir)
{
    // set up the Geometry.
    osg::Geometry* geom = new osg::Geometry;

    osg::Vec3Array* coords = new osg::Vec3Array(6);
    (*coords)[0] = corner;
    (*coords)[1] = corner+xdir;
    (*coords)[2] = corner;
    (*coords)[3] = corner+ydir;
    (*coords)[4] = corner;
    (*coords)[5] = corner+zdir;

    geom->setVertexArray(coords);

    osg::Vec4 x_color(0.0f,1.0f,1.0f,1.0f);
    osg::Vec4 y_color(0.0f,1.0f,1.0f,1.0f);
    osg::Vec4 z_color(1.0f,0.0f,0.0f,1.0f);

    osg::Vec4Array* color = new osg::Vec4Array(6);
    (*color)[0] = x_color;
    (*color)[1] = x_color;
    (*color)[2] = y_color;
    (*color)[3] = y_color;
    (*color)[4] = z_color;
    (*color)[5] = z_color;
    
    geom->setColorArray(color);
    geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);
    
    geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,6));
    
    osg::StateSet* stateset = new osg::StateSet;
    osg::LineWidth* linewidth = new osg::LineWidth();
    linewidth->setWidth(4.0f);
    stateset->setAttributeAndModes(linewidth,osg::StateAttribute::ON);
    stateset->setMode(GL_LIGHTING,osg::StateAttribute::OFF);
    geom->setStateSet(stateset);
    
    return geom;
}


osg::Group* FenetrePrincipale::CreerTriede()
{
	float	longueurAxeX=surface->EchX()*surface->NbColonneImage(0);
	float	longueurAxeY=surface->EchY()*surface->NbLigneImage(0);
	float	longueurAxeZ=surface->FinPalette()*surface->EchZ();
    // create the root node which will hold the model.
    osg::Group* root = new osg::Group();

    // add the drawable into a single goede to be shared...
    osg::Billboard* center = new osg::Billboard();
    center->setMode(osg::Billboard::POINT_ROT_EYE);
    center->addDrawable(
        CreateSquare(osg::Vec3(-0.5f,0.0f,-0.5f),osg::Vec3(1.0f,0.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
        osgDB::readImageFile("C:\\lib\\OpenSceneGraph-2.8.3\\OpenSceneGraph-Data-2.8.0\\Images/reflect.rgb")),
        osg::Vec3(0.0f,0.0f,0.0f));
        
    osg::Billboard* x_arrow = new osg::Billboard();
    x_arrow->setMode(osg::Billboard::AXIAL_ROT);
    x_arrow->setAxis(osg::Vec3(1.0f,0.0f,0.0f));
    x_arrow->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
    x_arrow->addDrawable(
       CreateSquare(osg::Vec3(-0.5f,0.0f,-0.5f),osg::Vec3(1.0f,0.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
       osgDB::readImageFile("C:\\lib\\OpenSceneGraph-2.8.3\\OpenSceneGraph-Data-2.8.0\\Cubemap_axis\\posx.png")),
       osg::Vec3(5.0f,0.0f,0.0f));

    osg::Billboard* y_arrow = new osg::Billboard();
    y_arrow->setMode(osg::Billboard::AXIAL_ROT);
    y_arrow->setAxis(osg::Vec3(0.0f,1.0f,0.0f));
    y_arrow->setNormal(osg::Vec3(1.0f,0.0f,0.0f));
    y_arrow->addDrawable(
        CreateSquare(osg::Vec3(0.0f,-0.5f,-0.5f),osg::Vec3(0.0f,1.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
        osgDB::readImageFile("C:\\lib\\OpenSceneGraph-2.8.3\\OpenSceneGraph-Data-2.8.0\\Cubemap_axis\\posy.png")),
        osg::Vec3(0.0f,5.0f,0.0f));

    osg::Billboard* z_arrow = new osg::Billboard();
    z_arrow->setMode(osg::Billboard::AXIAL_ROT);
    z_arrow->setAxis(osg::Vec3(0.0f,0.0f,1.0f));
    z_arrow->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
    z_arrow->addDrawable(
        CreateSquare(osg::Vec3(-0.5f,0.0f,-0.5f),osg::Vec3(1.0f,0.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
        osgDB::readImageFile("C:\\lib\\OpenSceneGraph-2.8.3\\OpenSceneGraph-Data-2.8.0\\Cubemap_axis\\posz.png")),
        osg::Vec3(0.0f,0.0f,5.0f));



    osg::Geode* axis0 = new osg::Geode();
    axis0->addDrawable(CreateAxis(osg::Vec3(0.0f,0.0f,0.0f),
    osg::Vec3(longueurAxeX,0.0f,0.0f),
    osg::Vec3(0.0f,longueurAxeY,0.0f),
    osg::Vec3(0.0f,0.0f,longueurAxeZ)));
    osg::Geode* axis1 = new osg::Geode();
    axis1->addDrawable(CreateAxis(osg::Vec3(longueurAxeX,0.0f,0.0f),
    osg::Vec3(-longueurAxeX,0.0f,0.0f),
    osg::Vec3(0.0f,longueurAxeY,0.0f),
    osg::Vec3(0.0f,0.0f,longueurAxeZ)));
    osg::Geode* axis2 = new osg::Geode();
    axis2->addDrawable(CreateAxis(osg::Vec3(longueurAxeX,longueurAxeY,0.0f),
    osg::Vec3(-longueurAxeX,0.0f,0.0f),
    osg::Vec3(0.0f,-longueurAxeY,0.0f),
    osg::Vec3(0.0f,0.0f,longueurAxeZ)));
    osg::Geode* axis3 = new osg::Geode();
    axis3->addDrawable(CreateAxis(osg::Vec3(0.0f,longueurAxeY,0.0f),
    osg::Vec3(longueurAxeX,0.0f,0.0f),
    osg::Vec3(0.0f,-longueurAxeY,0.0f),
    osg::Vec3(0.0f,0.0f,longueurAxeZ)));

    float characterSize=100;
	for (int i=1;i<=5;i++)
		{
		char	s[20];
		sprintf(s,"%.1f",i*longueurAxeX/5);
		osg::Geode* ligneGrille1 = new osg::Geode();
		osg::Vec3 posX(i*longueurAxeX/5,0.0f,0.0f);
		osg::Vec3 dX(0,-200.0f,0.0f);
		
		ligneGrille1->addDrawable(CreerLigne(posX,osg::Vec3(0.0f,longueurAxeY,0.0f),0.5));
		osgText::Text* text1 = new osgText::Text;
		text1->setFont("fonts/times.ttf");
		text1->setCharacterSize(characterSize);
		text1->setPosition(posX+dX);
		text1->setAxisAlignment(osgText::Text::YZ_PLANE);
		text1->setText(s);
		ligneGrille1->addDrawable(text1);
		root->addChild(ligneGrille1);
		
		sprintf(s,"%.1f",i*longueurAxeY/5);
		osg::Geode* ligneGrille2 = new osg::Geode();
		osg::Vec3 posY(0.0f,i*longueurAxeY/5,0.0f);
		osg::Vec3 dY(-00,-100.0f,0.0f);
		ligneGrille2->addDrawable(CreerLigne(posY,osg::Vec3(longueurAxeX,0.0f,0.0f),0.5));
		osgText::Text* text2 = new osgText::Text;
		text2->setFont("fonts/times.ttf");
		text2->setCharacterSize(characterSize);
		text2->setPosition(posY+dY);
		text2->setAxisAlignment(osgText::Text::YZ_PLANE);
		text2->setText(s);
		ligneGrille2->addDrawable(text2);

    
		root->addChild(ligneGrille2);
		}
	for (int i=1;i<=5;i++)
		{
		char	s[20];
		sprintf(s,"%.1f",i*longueurAxeZ/5/surface->EchZ());
		osg::Geode* ligneGrille1 = new osg::Geode();
		osg::Vec3 posZ(0.0,0.0f,i*longueurAxeZ/5);
		osg::Vec3 dZ(-00,-100.0f,0.0f);
		osgText::Text* text2 = new osgText::Text;
		text2->setFont("fonts/times.ttf");
		text2->setCharacterSize(characterSize);
		text2->setPosition(posZ+dZ);
		text2->setAxisAlignment(osgText::Text::YZ_PLANE);
		text2->setText(s);
		ligneGrille1->addDrawable(text2);
	
		ligneGrille1->addDrawable(CreerLigne(posZ,osg::Vec3(0.0f,longueurAxeY,0.0f),2));
		root->addChild(ligneGrille1);
		osg::Geode* ligneGrille2 = new osg::Geode();
		ligneGrille2->addDrawable(CreerLigne(osg::Vec3(0.0,0.0f,i*longueurAxeZ/5),osg::Vec3(longueurAxeX,0.0f,0.0f),0.5));
		root->addChild(ligneGrille2);
		osg::Geode* ligneGrille3 = new osg::Geode();
		ligneGrille3->addDrawable(CreerLigne(osg::Vec3(longueurAxeX,longueurAxeY,i*longueurAxeZ/5),osg::Vec3(0.0f,-longueurAxeY,0.0f),0.5));
		root->addChild(ligneGrille3);
		osg::Geode* ligneGrille4 = new osg::Geode();
		ligneGrille4->addDrawable(CreerLigne(osg::Vec3(longueurAxeX,longueurAxeY,i*longueurAxeZ/5),osg::Vec3(-longueurAxeX,0.0f,0.0f),0.5));
		root->addChild(ligneGrille4);
		}

    root->addChild(center);
    root->addChild(x_arrow);
    root->addChild(y_arrow);
    root->addChild(z_arrow);
    root->addChild(axis0);
    root->addChild(axis1);
    root->addChild(axis2);
    root->addChild(axis3);

    return root;
}







osg::Group* FenetrePrincipale::CreateScalarBar()
{

    // Create a custom color set
    std::vector<osg::Vec4> cs;
    cs.push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));   // R
    cs.push_back(osg::Vec4(0.0f,1.0f,0.0f,1.0f));   // G
    cs.push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));   // G
    cs.push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));   // B
    cs.push_back(osg::Vec4(0.0f,1.0f,1.0f,1.0f));   // R

	std::vector< osg::Vec4 > paletteTopographique;
	paletteTopographique.resize(256);
	wxColor*p=surface->Lirepalette();

	for (int i=0;i<=255;i++)
		paletteTopographique[i] = osg::Vec4(p[i*64].Red()/255.0f,p[i*64].Green()/255.0f,p[64].Blue()/255.0f,1.0f);
/*	for (int i=0;i<256;i++)
		paletteTopographique[i] = osg::Vec4(palette1[i],palette1[i+1],palette1[i+2],1);*/
    ColorRange* cr = new ColorRange(0.0f,1.0f);
    cr->setColors(paletteTopographique);
    ScalarBar* sb = new ScalarBar(256, 11, cr, "toto", ScalarBar::VERTICAL, 0.01f, new EchelleHauteur);
    int w= sb->getNumColors();
    sb->setScalarPrinter(new EchelleHauteur);

    return (osg::Group *)sb;

}


osg::Group * FenetrePrincipale::CreateScalarBar_HUD()
{
std::vector<osg::Vec4> cs;
cs.push_back(osg::Vec4(1.0f,0.0f,0.0f,1.0f));   // R
cs.push_back(osg::Vec4(0.0f,1.0f,0.0f,1.0f));   // G
cs.push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));   // G
cs.push_back(osg::Vec4(0.0f,0.0f,1.0f,1.0f));   // B
cs.push_back(osg::Vec4(0.0f,1.0f,1.0f,1.0f));   // R

std::vector< osg::Vec4 > paletteTopographique;
wxColor*p=surface->Lirepalette();

for (int i=0;i<=255;i++)
paletteTopographique.push_back( osg::Vec4(p[i*64].Red()/255.0f,p[i*64].Green()/255.0f,p[i*64].Blue()/255.0f,1.0f));
ColorRange* cr = new ColorRange(surface->DebPalette(),surface->FinPalette(),paletteTopographique);
// cr->setColors();
osgSim::ScalarBar * geode = new ScalarBar(256,11, cr, "Scalar Height (nm)",ScalarBar::HORIZONTAL,0.1, new EchelleHauteur);
osgSim::ScalarBar::TextProperties tp;
tp._fontFile = "fonts/times.ttf";
geode->setTextProperties(tp);
osg::StateSet * stateset = geode->getOrCreateStateSet();
stateset->setMode(GL_LIGHTING, osg::StateAttribute::OFF);

stateset->setMode(GL_DEPTH_TEST,osg::StateAttribute::OFF);
stateset->setRenderBinDetails(11, "RenderBin");

osg::MatrixTransform * modelview = new osg::MatrixTransform;
modelview->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
osg::Matrixd matrix(osg::Matrixd::scale(1000,1000,1000) * osg::Matrixd::translate(120,10,0)); // I've played with these values a lot and it seems to work, but I have no idea why
modelview->setMatrix(matrix);
modelview->addChild(geode);

osg::Projection * projection = new osg::Projection;
projection->setMatrix(osg::Matrix::ortho2D(0,1280,0,1024)); // or whatever the OSG window res is
projection->addChild(modelview);


return projection; //make sure you delete the return sb line
}







/* My frame constructor */
FenetrePrincipale::FenetrePrincipale(wxFrame *frame, const wxString& title, const wxPoint& pos,
    const wxSize& size, long style)
    : wxFrame(frame, wxID_ANY, title, pos, size, style)
{
// Installation des menus 
wxMenu *menuFile = new wxMenu;
wxMenu *menuEdition = new wxMenu;
// Menu Fichier
menuFile->Append(OUVRIR_FICHIER, _T("&Ouvrir\tCtrl-O"), _T("Ouvrir"));
menuFile->Append(QUITTER_, _T("&Quitter\tCtrl-Q"), _T("Quitter"));
wxMenuBar *menuBar = new wxMenuBar();
// Ajout des menus à la barre des menus 
menuBar->Append(menuFile, _T("&Fichier"));
((wxFrame*)this)->SetBackgroundColour(*wxBLACK);
// Association barre des menus avec la trame
SetMenuBar(menuBar);
SetViewer(NULL);

listeObjets=NULL;
systemAxe=NULL;
legendePalette=NULL;
imageSurface=NULL;



SetIcon(wxIcon("seec64.bmp",wxBITMAP_TYPE_ICO ));
fenImage=NULL;
modeVideo=0;
tabSurface = new NanoSurface*[NBSURFACEMAX];
indSurface = 0;
surface=NULL;
for (int i=0;i<NBSURFACEMAX;i++)
	tabSurface[i]=NULL;
}


/**
Ouvrir l'image calibre et la ranger dans la classe NanoRaptor 
*/
void FenetrePrincipale::OnOuvrir(wxCommandEvent& event)
{
wxFileDialog ouverture(this, "Ouvrir Séquence", "", "", "*.listez;*.listei;*.bmp");
if (ouverture.ShowModal()!=wxID_OK)
	return;
string s(ouverture.GetFilename());
s = ouverture.GetDirectory()+"\\"+s;
if (s.find(".listez")!=string::npos || s.find(".listei")!=string::npos)
	{
	//InstallGraphique((char*)s.c_str());
	
	InstallGraphiquePhase1();
	surface=new NanoSurface((char*)s.c_str());

	surface->DefOSGApp(osgApp);
	osgApp->nanoSurf=surface;
	fgOSGWX->DefSurface(surface);
	surface->addDrawable(surface->LitShapeDrawable().get());
	imageSurface = (osg::Group *)surface;
	osgDB::writeNodeFile(*surface,std::string("test.osg"));
	InstallGraphiquePhase2(NULL);
	
	
	osgApp->OuvertureOngletFichier(surface); 
	osgApp->OuvertureOngletCurseur(surface); 
	osgApp->OuvertureOngletEchelle(surface); 
	osgApp->OuvertureOngletCourbe(surface); 
	osgApp->OuvertureOngletControlFiltre(surface); 
	osgApp->OuvertureOngletControlVue(surface); 

	osgApp->OuvertureOngletPositionCamera(surface);
	osgApp->OuvertureOngletControlQuadric(surface);
	}
else 
	{
	if(!fenImage)
		fenImage= new FenetreImage(NULL,wxT("Image"),wxDefaultPosition, wxDefaultSize);
	fenImage->DefOSGApp(osgApp);
	fenImage->DefFenParametrage(osgApp->parametrage );
	osgApp->DefFenetreImage(fenImage);
	fenImage->OuvertureOngletImage(NULL,(char*)s.c_str());
	fenImage->Show(true);
	osgApp->OuvertureOngletParametreImage(fenImage->ImNano());
	osgApp->OuvertureOngletInfoImage(fenImage->ImNano());
	osgApp->OuvertureOngletInfoRegion(fenImage->ImNano());
	osgApp->OuvertureOngletInfoQuadrique(fenImage->ImNano());

	}
}





void FenetrePrincipale::InstallGraphiquePhase1()
{

int width = 600;
int height = 600;
GetClientSize(&width, &height);
width = 600;
height = 600;

int *attributes = new int[7];
attributes[0] = int(WX_GL_DOUBLEBUFFER);
attributes[1] = WX_GL_RGBA;
attributes[2] = WX_GL_DEPTH_SIZE;
attributes[3] = 8;
attributes[4] = WX_GL_STENCIL_SIZE;
attributes[5] = 8;
attributes[6] = 0;


fgOSGWX = new FenetreGraphiqueWX(this, wxID_ANY, wxDefaultPosition,
                                            wxSize(width, height), wxSUNKEN_BORDER, wxT("osgviewerWX"), attributes);
fgOSGWX->DefOSGApp((void*)osgApp);

GraphicsOSGWX* gw = new GraphicsOSGWX(fgOSGWX);

fgOSGWX->SetGraphicsWindow(gw);


// construct the viewer.
double fovy,aspectRatio,zNear,zFar;
viewer= new osgViewer::Viewer;
viewer->getCamera()->getProjectionMatrixAsPerspective(fovy,aspectRatio,zNear,zFar);
viewer->getCamera()->setProjectionMatrixAsPerspective(fovy,1,zNear,zFar);

viewer->getCamera()->setGraphicsContext(gw);
viewer->getCamera()->setViewport(0,0,width,height);
// add the stats handler
viewer->addEventHandler(new osgViewer::StatsHandler);
viewer->setThreadingModel(osgViewer::Viewer::SingleThreaded);


		{
		wxGLContext *glContexte =new wxGLContext(fgOSGWX);
		fgOSGWX->SetCurrent(*glContexte);
		}

/*fgOSGWX->Show(true);
    wxGLContext *glContexte =new wxGLContext(fgOSGWX);
    fgOSGWX->SetCurrent(*glContexte);
	GLubyte *verGl=(GLubyte*)glGetString(GL_VERSION);
	HGLRC WINAPI wwww=wglGetCurrentContext();*/


return ;
}


void FenetrePrincipale::InstallGraphique(char *nomFichier)
{
InstallGraphiquePhase1();

surface=new NanoSurface(nomFichier);
surface->DefOSGApp(osgApp);
osgApp->nanoSurf=surface;
fgOSGWX->DefSurface(surface);
surface->addDrawable(surface->LitShapeDrawable().get());
osg::Node* model = surface;
osg::Group* group = new osg::Group;
if (listeObjets==NULL)
	listeObjets = new osg::Switch;
//group->addChild(model);
//InstallGraphiquePhase2(group);
listeObjets->addChild(surface,false);
InstallGraphiquePhase2(listeObjets);

}


void FenetrePrincipale::MAJAxe()
{
bool x;

x=listeObjets->getChildValue(systemAxe);
listeObjets->removeChild(systemAxe);
systemAxe=CreerTriede();
listeObjets->addChild(systemAxe,x);
}

void FenetrePrincipale::MAJEchelleCouleur(float deb,float fin)
{
osg::Group *group=(osg::Group *)viewer->getSceneData();
	

osg::MatrixTransform* echelle=(osg::MatrixTransform*)group->getChild(1);
osgSim::ScalarBar *g= (osgSim::ScalarBar *)(((osg::MatrixTransform*)(echelle->getChild(0)))->getChild(0));


wxColor*p=surface->Lirepalette();
s
std::vector< osg::Vec4 > paletteTopographique;
for (int i=0;i<=255;i++)
	paletteTopographique.push_back( osg::Vec4(p[i*64].Red()/255.0f,p[i*64].Green()/255.0f,p[i*64].Blue()/255.0f,1.0f));
ColorRange* cr = new ColorRange(deb,fin,paletteTopographique);


g->setScalarsToColors(cr);

	g->update();


} 

void FenetrePrincipale::InstallGraphiquePhase2(osg::Group *group)
{
	
legendePalette=CreateScalarBar_HUD();
systemAxe=CreerTriede();

// load the nodes from the commandline arguments.
surfaceReference= new PlanReference(osgApp);
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
}

void FenetrePrincipale::MAJNoeud(int key)
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

void FenetrePrincipale::AvanceVideo()
{
if (surface && modeVideo)
	{
	//surface->LitShapeDrawable()->setUseDisplayList(false);
	surface->Maj(1);
	osgApp->MAJFichier();
	osgApp->MAJEchelle();
	osgApp->MAJCurseur();
	//surface->LitShapeDrawable()->dirtyBound(); 
	}
}


void FenetrePrincipale::OnIdle(wxIdleEvent &event)
{
if (viewer.get())
    {


    viewer->frame();
    }
event.RequestMore();
}

void FenetrePrincipale::DeplaceCurseur(float x,float y,float z)
{
int ind=cInfo3D->LireActiveGeode();
cInfo3D->DeplaceCurseur(x,y,z,viewer);
};


void FenetrePrincipale::AjouteNanoSurface(int nb,ImageInfo **tab)
{
static int nbScene=0;
if (!surface)
	{
	InstallGraphiquePhase1();
	surface=new NanoSurface(nb,tab);

	surface->DefOSGApp(osgApp);
	osgApp->nanoSurf=surface;
	fgOSGWX->DefSurface(surface);
	surface->addDrawable(surface->LitShapeDrawable().get());
	imageSurface = (osg::Group *)surface;
	osgDB::writeNodeFile(*surface,std::string("test.osg"));
	InstallGraphiquePhase2(NULL);
	osgApp->OuvertureOngletFichier(surface); 
	osgApp->OuvertureOngletCurseur(surface); 
	osgApp->OuvertureOngletEchelle(surface); 
	osgApp->OuvertureOngletCourbe(surface); 
	osgApp->OuvertureOngletControlFiltre(surface); 
	osgApp->OuvertureOngletControlVue(surface); 

	osgApp->OuvertureOngletPositionCamera(surface);
	osgApp->OuvertureOngletControlQuadric(surface);
	fenImage->DefQuadrique('R');
	surface->Maj(0);
	}
else
	{
	surface->AjouteImage(tab);

/*	surface->addDrawable(surface->LitShapeDrawable().get());
	osg::Node* model = surface;
	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(model);
	InstallGraphiquePhase2(group);
	scene[nbScene]=group;
	nbScene++;
	if (nbScene==10)
		nbScene=0;*/
MAJEchelleCouleur(surface->DebPalette(),surface->FinPalette());

	}	
}




