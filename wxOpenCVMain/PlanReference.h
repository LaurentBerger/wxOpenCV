#ifndef __PLANREFERENCE__
#define __PLANREFERENCE__

#include <osg/Geometry>
#include <osg/Geode>
#include <osg/ShapeDrawable>


class PlanReference: public osg::Geode {
private : 
osg::HeightField	*pl;
osg::ShapeDrawable	*surface;	
void				*osgApp;
void				*fenParent;

float	a,b,c,d,e,f;	/*<! Coefficient de la quadrique */
float	xMin,xMax,yMin,yMax;
float	z;
float	dz;
float	transparence;
public:
PlanReference(void *,void *);

~PlanReference();
void DefZRelatif(float dz){z=z+dz;};
void DefZ(float da){z=da;MAJPlan();};
void Descendre(float k){f = f -k*dz;MAJPlan();};
void Monter(float k){f = f +k*dz;MAJPlan();};
void Transparence();
void Maj();
void Maj(float an,float bn,float cn,float dn,float en,float fn);
void	DefOSGApp(void* x){osgApp=x;};

float	LireA(){return a;}
void	DefQuadricA(float x){a=x;}
float	LireB(){return b;}
void	DefQuadricB(float x){b=x;}
float	LireC(){return c;}
void	DefQuadricC(float x){c=x;}
float	LireD(){return d;}
void	DefQuadricD(float x){d=x;}
float	LireE(){return e;}
void	DefQuadricE(float x){e=x;}
float	LireF(){return f;}
void	DefQuadricF(float x){f=x;}

void MAJPlan();
};

#endif