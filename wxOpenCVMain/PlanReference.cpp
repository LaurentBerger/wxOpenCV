#include "PlanReference.h"
#include "Appli.h"
#include <osg/Texture2D>

PlanReference::PlanReference(void *x)
{
DefOSGApp(x);
((wxOsgApp*)osgApp)->DefPlanRefence(this);
dz=0.1;
transparence=0.5;
a=0;
b=0;
c=0;
d=0;
e=0;
f=1;
float	 echx=((wxOsgApp*)osgApp)->nanoSurf->EchX();
float	 echy=((wxOsgApp*)osgApp)->nanoSurf->EchY();


pl = new osg::HeightField(); 
pl->allocate(100,100);

MAJPlan();
surface = new osg::ShapeDrawable(pl);
surface->setColor(osg::Vec4(1.0f,0.0f,0.0f,transparence));

osg::StateSet *etat=getOrCreateStateSet();

osg::Texture2D* texture = new osg::Texture2D;
texture->setDataVariance(osg::Object::DYNAMIC); // protect from being optimized away as static state.
texture->setBorderColor(osg::Vec4(1.0f,1.0f,1.0f,0.9f)); // only used when wrap is set to CLAMP_TO_BORDER

etat->setTextureAttributeAndModes(0,texture,osg::StateAttribute::ON);
etat->setMode(GL_BLEND,osg::StateAttribute::ON);
etat->setRenderingHint(osg::StateSet::TRANSPARENT_BIN);

addDrawable(surface);

}

void PlanReference::Maj()
{
surface->setUseDisplayList(false);	
surface->dirtyBound(); 
surface->setUseDisplayList(true);	
}

void PlanReference::Maj(float an,float bn,float cn,float dn,float en,float fn)
{
a=an;b=bn;c=cn;d=dn;e=en;f=fn;
MAJPlan();
surface->setUseDisplayList(false);	
surface->dirtyBound(); 
surface->setUseDisplayList(true);	
}

void PlanReference::MAJPlan()
{
float		echx=((wxOsgApp*)osgApp)->nanoSurf->EchX();
float		echy=((wxOsgApp*)osgApp)->nanoSurf->EchY();
float		echz=((wxOsgApp*)osgApp)->nanoSurf->EchZ();
int			nbL=((wxOsgApp*)osgApp)->nanoSurf->NbLigneImage(0);
int			nbC=((wxOsgApp*)osgApp)->nanoSurf->NbColonneImage(0);



pl->setXInterval(echx*nbC/100);
pl->setYInterval(echy*nbL/100);
for (int i=0;i<100;i++)
	for (int j=0;j<100;j++)
		{
		float x=float(j*nbC/100.-nbC/2)/nbC,y=float(-i*nbL/100.+nbL/2)/nbL;
		z=a*x*x+b*y*y+c*x*y+d*x+e*y+f;
		pl->setHeight(j,i,z*echz);	
		}
}

void PlanReference::Transparence()
{
transparence+=0.1;
if(transparence>1)
	transparence =0;
surface->setColor(osg::Vec4(1.0f,0.0f,0.0f,transparence));

};
