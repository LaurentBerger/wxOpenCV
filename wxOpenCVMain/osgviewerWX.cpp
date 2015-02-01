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
#include "Fenetre3D.h"
#include "BarreEchelle.h"

using namespace osgSim;
using namespace std;









typedef std::vector< osg::ref_ptr<osg::Image> > ImageList;

/** create quad at specified position. */
osg::Drawable* Fenetre3D::CreateSquare(const osg::Vec3& corner,const osg::Vec3& width,const osg::Vec3& height, osg::Image* image)
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


osg::Drawable* Fenetre3D::CreerLigne(const osg::Vec3& corner,const osg::Vec3& xdir,float epaisseurTrait)
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


osg::Drawable* Fenetre3D::CreateAxis(const osg::Vec3& corner,const osg::Vec3& xdir,const osg::Vec3& ydir,const osg::Vec3& zdir)
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


osg::Group* Fenetre3D::CreerTriede()
{
	float	longueurAxeX=surface->EchX()*surface->NbColonneImage(0);
	float	longueurAxeY=surface->EchY()*surface->NbLigneImage(0);
	float	longueurAxeZ=surface->FinPalette()*surface->EchZ();
    // create the root node which will hold the model.
    osg::Group* root = new osg::Group();

    // add the drawable into a single goede to be shared...
    osg::Billboard* center = new osg::Billboard();
    center->setMode(osg::Billboard::POINT_ROT_EYE);
	osg::Image *im=osgDB::readImageFile("F:\\Lib\\OpenSceneGraph-Data-3.0.0\\Images\\reflect.rgb");
    center->addDrawable(
        CreateSquare(osg::Vec3(-0.5f,0.0f,-0.5f),osg::Vec3(1.0f,0.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
        im),
        osg::Vec3(0.0f,0.0f,0.0f));
        
    osg::Billboard* x_arrow = new osg::Billboard();
    x_arrow->setMode(osg::Billboard::AXIAL_ROT);
    x_arrow->setAxis(osg::Vec3(1.0f,0.0f,0.0f));
    x_arrow->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
    x_arrow->addDrawable(
       CreateSquare(osg::Vec3(-0.5f,0.0f,-0.5f),osg::Vec3(1.0f,0.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
       osgDB::readImageFile("F:/Lib/OpenSceneGraph-Data-3.0.0/Cubemap_axis/posx.png")),
       osg::Vec3(5.0f,0.0f,0.0f));

    osg::Billboard* y_arrow = new osg::Billboard();
    y_arrow->setMode(osg::Billboard::AXIAL_ROT);
    y_arrow->setAxis(osg::Vec3(0.0f,1.0f,0.0f));
    y_arrow->setNormal(osg::Vec3(1.0f,0.0f,0.0f));
    y_arrow->addDrawable(
        CreateSquare(osg::Vec3(0.0f,-0.5f,-0.5f),osg::Vec3(0.0f,1.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
        osgDB::readImageFile("F:/Lib/OpenSceneGraph-Data-3.0.0/Cubemap_axis/posy.png")),
        osg::Vec3(0.0f,5.0f,0.0f));

    osg::Billboard* z_arrow = new osg::Billboard();
    z_arrow->setMode(osg::Billboard::AXIAL_ROT);
    z_arrow->setAxis(osg::Vec3(0.0f,0.0f,1.0f));
    z_arrow->setNormal(osg::Vec3(0.0f,-1.0f,0.0f));
    z_arrow->addDrawable(
        CreateSquare(osg::Vec3(-0.5f,0.0f,-0.5f),osg::Vec3(1.0f,0.0f,0.0f),osg::Vec3(0.0f,0.0f,1.0f),
        osgDB::readImageFile("F:/Lib/OpenSceneGraph-Data-3.0.0/Cubemap_axis/posz.png")),
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







osg::Group* Fenetre3D::CreateScalarBar()
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


osg::Group * Fenetre3D::CreateScalarBar_HUD()
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
//ColorRange* cr = new ColorRange(0.0,255.0,paletteTopographique);
//ColorRange* cr = new ColorRange(0.0,255.0);
//cr->setColors();
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


















