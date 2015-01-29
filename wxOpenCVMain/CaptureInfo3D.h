#ifndef __CAPTUREINFO3D__
#define __CAPTUREINFO3D__


/**  La classe CaptureInfo3D permet la récupération des coordonnées du trackball par rapport à la topographie
La capture est autorisée après l'appui de la touche C et interdite avec la touche c.
12 marqueurs sont mémorisables. La  sélection de l'indice du marqueur se fait en utilisant 
les touches de fonction. La position du marqueur est affichée dans l'ongleur Marker
*/
class CaptureInfo3D : public osgGA::GUIEventHandler {
osg::Group*			group;
osg::ShapeDrawable*	sd[12];
osg::Geode*			geode[12];
int					activeGeode;
void*				osgApp;

bool capture;
public: 

    CaptureInfo3D(osg::Group*g) {group=g;capture=false;activeGeode=0;for (int i=0;i<12;i++)geode[i]=NULL;}        
	void AutoriseCapture(){capture=true;};    
	void InterditCapture(){capture=false;}; 
	int LireActiveGeode(){return activeGeode;};
	void ActiveGeode(int n);
	void DefPositionCurseur(int i,float x,float y,float z);
	void DeplaceCurseur(float,float,float,osg::ref_ptr<osgViewer::Viewer> );
 	void DefOSGApp(void *w){osgApp=w;};
 	void DefCurseur(float x,float y,float z,osg::ref_ptr<osgViewer::Viewer> );
   bool handle(const osgGA::GUIEventAdapter& ea,osgGA::GUIActionAdapter& aa)
    {
		if (!capture)
			return false;
        switch(ea.getEventType())
        {
            case(osgGA::GUIEventAdapter::PUSH):
            {
                osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);
                Capture(viewer,ea);
            }
            return false;

        default:
            return false;
        }
    }

    void Capture(osgViewer::Viewer* viewer, const osgGA::GUIEventAdapter& ea)
    {
        osg::Group* root = dynamic_cast<osg::Group*>(viewer->getSceneData());       
        if (!root) return;

        osgUtil::LineSegmentIntersector::Intersections intersections;
        if (viewer->computeIntersections(ea.getX(),ea.getY(),intersections))
        {
            const osgUtil::LineSegmentIntersector::Intersection& hit = *intersections.begin();

            bool handleMovingModels = false;
            const osg::NodePath& nodePath = hit.nodePath;
            for(osg::NodePath::const_iterator nitr=nodePath.begin();
                nitr!=nodePath.end();
                ++nitr)
            {
                const osg::Transform* transform = dynamic_cast<const osg::Transform*>(*nitr);
                if (transform)
                {
                    if (transform->getDataVariance()==osg::Object::DYNAMIC) handleMovingModels=true;
                }
            }
            
            osg::Vec3 position = handleMovingModels ? hit.getLocalIntersectPoint() : hit.getWorldIntersectPoint();
            float scale = 10.0f * ((float)rand() / (float)RAND_MAX);
            float intensity = 1.0f;


#if 1       
			if (geode[activeGeode]!=NULL)     
				{
				root->removeChild(geode[activeGeode]);
//				delete geode;
				}
            DefPositionCurseur(activeGeode,position.x(),position.y(),position.z());
            DefCurseur(position.x(),position.y(),position.z(),viewer);
/*            geode[activeGeode] = new osg::Geode;
            osg::Sphere	*sp=new osg::Sphere(position,0.01);
            sd[activeGeode]=new osg::ShapeDrawable(sp);
            sd[activeGeode]->setColor(osg::Vec4(0, 0, 1.0, 1.0));
            geode[activeGeode]->addDrawable(sd[activeGeode]);
            root->addChild(geode[activeGeode]);*/
#endif
 
        }
    }
    
protected:
    virtual ~CaptureInfo3D() {}
};

#endif 
