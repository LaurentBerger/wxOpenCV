#include <osgViewer/Viewer>
#include <osg/ShapeDrawable>
#include <osg/Vec3>
#include <osg/Vec4>
#include <osg/Quat>
#include <osg/Matrix>
#include <osg/Geometry>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osgDB/FileUtils>
#include <osgDB/ReadFile>
#include <osgGA/TrackballManipulator>
#include <osgSim/ColorRange>
#include <osgSim/SphereSegment>
#include <osgUtil/Optimizer>
#include <osgViewer/ViewerEventHandlers>

//#include "Appli.h"
#include "CaptureInfo3D.h"


void CaptureInfo3D::DeplaceCurseur(float x,float y,float z,osg::ref_ptr<osgViewer::Viewer> v )
{
DefCurseur(x,y,z,v);
}

void CaptureInfo3D::DefCurseur(float x,float y,float z,osg::ref_ptr<osgViewer::Viewer> viewer)
{
osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());
osg::Vec3 position(x,y,z);

if (geode[activeGeode]!=NULL)     
	root->removeChild(geode[activeGeode]);
geode[activeGeode] = new osg::Geode;
osg::Sphere	*sp=new osg::Sphere(position,1.5);
sd[activeGeode]=new osg::ShapeDrawable(sp);
sd[activeGeode]->setColor(osg::Vec4(0, 0, 1.0, 1.0));
geode[activeGeode]->addDrawable(sd[activeGeode]);
root->addChild(geode[activeGeode]);
}


void CaptureInfo3D::DefPositionCurseur(int i,float x,float y,float z)
{
//((wxOsgApp*)osgApp)->DefPositionCurseur(i,x,y,z);
}

void CaptureInfo3D::ActiveGeode(int n)
{
if (geode[activeGeode])
	sd[activeGeode]->setColor(osg::Vec4(0, 1.0, 1.0, 1.0));
if (n>=0 && n<12) 
	{
	activeGeode=n;   
	if (geode[activeGeode])
		sd[activeGeode]->setColor(osg::Vec4(0, 0, 1.0, 1.0));
	} // if (n>=0 && n<12)
}