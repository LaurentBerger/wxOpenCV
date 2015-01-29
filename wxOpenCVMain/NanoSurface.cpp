#include "NanoSurface.h"
#include "ImageInfo.h"
#include <iostream>
#include <fstream>
#include <strstream>
#include <wx/wx.h>
#include <wx/thread.h>
using namespace std;

#include <osgDB/ReadFile>


class ProcessLectureFichier : public wxThread
{
osg::Image	**im;
char		*nomFichier;	

public:
    ProcessLectureFichier(char *nom,osg::Image	**i);

    // thread execution starts here
    virtual void *Entry();

    // called when the thread exits - whether it terminates normally or is
    // stopped with Delete() (but not when it is Kill()ed!)
    virtual void OnExit(){};

public:
    unsigned m_count;
};

ProcessLectureFichier::ProcessLectureFichier(char *nom,osg::Image	**i)
{
nomFichier = new char[strlen(nom)];
strcpy(nomFichier ,nom);
im=i;

}
void *ProcessLectureFichier::Entry()
{
*im = osgDB::readImageFile(nomFichier);
return NULL;
}



unsigned char palette[720]={245,244,242,242,240,236,239,237,233,237,234,230,234,232,226,231,229,223,229,226,220,226,223,217,
224,222,216,221,217,209,218,214,205,215,210,201,213,207,197,210,204,193,207,200,189,204,197,185,
202,195,184,200,191,177,198,188,174,196,186,170,194,183,166,192,181,162,190,178,159,188,175,155,
186,174,154,184,170,147,182,168,144,180,165,140,179,163,136,177,160,132,175,158,129,173,155,125,
172,154,124,171,150,115,171,148,110,171,146,105,171,143,100,170,141,94,170,138,89,170,136,84,
170,135,83,171,137,84,173,139,85,175,142,86,177,144,87,179,146,87,181,148,88,183,150,89,
185,152,90,186,154,93,187,156,95,188,158,97,190,160,99,191,162,102,192,163,104,193,165,106,
195,167,107,195,170,111,196,172,114,197,174,117,198,176,120,199,179,123,200,181,126,201,183,129,
202,185,130,203,187,135,204,189,139,205,192,142,206,194,145,207,196,149,208,198,152,209,200,155,
211,202,157,212,204,158,213,205,159,215,207,159,216,208,160,217,210,161,219,211,162,220,213,162,
222,214,163,223,215,166,224,217,169,225,218,171,227,219,174,228,221,176,229,222,178,230,224,181,
232,225,182,232,226,184,233,227,185,234,229,186,235,230,187,236,231,189,237,232,190,238,234,191,
239,235,192,237,233,189,235,232,188,233,232,186,232,231,185,230,230,184,228,229,182,226,228,181,
225,228,181,223,225
,178,221,224,177,219,222,176,217,220,175,215,219,173,213,217,172,211,216,171,
209,215,171,206,213,166,204,211,164,201,210,161,199,209,158,196,207,156,194,206,153,191,204,150,
189,204,150,186,203,148,183,202,147,181,201,146,178,200,145,175,200,145,173,199,144,170,198,143,
168,198,143,165,196,142,163,195,141,160,195,141,158,194,140,155,193,140,153,192,139,150,191,139,
148,191,139,151,193,144,154,195,147,157,198,150,160,200,154,163,202,157,166,204,160,169,206,163,
172,208,165,171,210,174,170,212,180,170,214,185,169,216,191,168,218,196,168,219,202,167,221,208,
167,223,210,173,226,219,179,228,224,185,230,230,191,233,235,197,235,241,203,238,246,209,240,252,
216,242,254,213,241,254,211,240,254,209,239,254,207,238,254,204,238,254,202,237,254,200,236,254,
198,236,255,196,234,255,194,233,255,193,232,255,191,231,255,189,230,255,188,228,255,186,227,255,
185,227,255,183,225,254,181,224,253,180,223,253,178,222,252,176,221,252,175,220,251,173,219,251,
172,219,251,170,217,250,169,216,249,167,215,249,166,214,248,165,213,248,163,211,247,162,210,247,
161,210,247,159,208,245,158,207,244,156,206,243,155,205,242,154,204,242,152,202,241,151,201,240,
150,201,240,148,199,238,147,198,238,146,197,237,145,196,236,144,195,235,143,194,235,142,193,234,
141,193,234,139,191,232,138,190,231,137,189,230,136,188,229,135,187,229,134,186,228,133,185,227,
132,185,227,130,183,225,129,182,225,127,182,224,126,181,224,125,180,223,123,179,222,122,178,222,
121,178,222,120,176,220,119,175,220,118,175,219,117,174,218,116,173,217,115,172,217,114,171,216,
113,171,216,98,142,171,84,121,144,70,99,117,56,78,90,42,57,63,28,35,36,14,14,9};



void NanoSurface::InitNanoSurface()
{
finNivPalette=16383;
debNivPalette=0;
pCouleur=NULL;
InitPalette(16384);
indPalette=8;
SelectPalette(indPalette);


echX=1;
echY=1;
echZ=0.1;
offsetZ0=0;
nbL=-1;
nbC=-1;
zoom=1;
tailleDuFiltre=0;
couleurActive='R';
imageFondPresente=0;
for (int i=0;i<TAILLEMAXDUFILTRE;i++)
	{
	gridMAJ[0][i]=false;
	gridMAJ[1][i]=false;
	gridMAJ[2][i]=false;
	}
}


NanoSurface::NanoSurface()
{
InitNanoSurface();
double pi=acos(-1.0);
nbL=512;
nbC=768;


grid.push_back(new osg::HeightField);
deque<osg::HeightField*>::iterator gridr;
grid[0]->allocate(nbC,nbL);
grid[0]->setXInterval(echX);
grid[0]->setYInterval(echY);

for(int r=0;r<nbL;r++)
    for(int c=0;c<nbC;c++)
    {
		double d=sqrt((r-nbL/2.0)*(r-nbL/2.0)+(c-nbC/2.0)*(c-nbC/2.0));
        grid[0]->setHeight(c,r,exp(-d*0.200)*cos(2*pi*d/10));
    }
surface[0]= new osg::ShapeDrawable(*gridr);
}

void NanoSurface::LireSequence(char *nomSequence)
{
ifstream	f(nomSequence);
int nbFichier=0;
char		tmp[256];
if (f.is_open() && nomSequence && nomSequence[strlen(nomSequence)-1]=='z')
	{


	f>>nbFichier;
	f.getline(tmp,256);
	for (int i=0;i<nbFichier;i++)
		{
		char *t=new char[300];
		tabNomFichier.push_back(t);
		f.getline(t,300);
		} // for (int i=0;i<nbFichier;i++)
	} // if (f.is_open())

char		*rep1=new char[strlen(nomSequence)+3];

strcpy(rep1,nomSequence);
int j=strlen(rep1)-1;
while (j>=0 && rep1[j]!='.')
	j--;
if (j>=0)
	{
	strcpy(rep1+j+1,"listei");	
	ifstream	ff(rep1);
	if (ff.is_open())
		{

		ff>>nbFichier>>imageFondPresente;
		ff.getline(tmp,256);
		if (imageFondPresente==2)
			{
			ff.getline(tmp,256);
			ifstream	gg(tmp);
			if (gg.is_open())
				{
				poly=new double*[3];
				for (int c=0;c<3;c++)
					{
					poly[c]=new double[6];
					for (int p=0;p<6;p++)
						gg>>poly[c][p];
					}
				}
			}
		for (int i=0;i<nbFichier;i++)
			{
			char *t=new char[300];
			ff.getline(t,300);
			tabNomImage.push_back(t);
			} // for (int i=0;i<nbFichier;i++) 
		} // if (ff.is_open())
	} // if (j>=0)
nbGrid = nbFichier;
}

NanoSurface::NanoSurface(int nbImage,ImageInfoCV **im)
/*! 
\param nomSeq nom du fichier contenant la liste des noms des fichiers de surface 
*/
{
InitNanoSurface();

nbL=-1;
nbC=-1;
indGrid=0;
nbGrid=nbImage;
if (im[0]->LitMini()>=0 && im[0]->LitMaxi()>im[0]->LitMini())
	{
	debNivPalette = im[0]->LitMini();
	finNivPalette = im[0]->LitMaxi();
	}

etat= new osg::StateSet; 
nbL=im[0]->LitNbLigne();
nbC=im[0]->LitNbColonne();

for (int i=0;i<nbGrid;i++)
	{
	LireFichierImage(im[i]);
	imValide.push_back(true);
	topoValide.push_back(false);
	}

VerificationGrille();
/** initialisation de la grille */
for (int ind=0;ind<nbGrid;ind++)
	AjouteImage(im[ind]);

deque<osg::Image*>::iterator wi=imSurface.begin();
int i=0;
while (i<nbGrid && !imValide[i] && tabNomImage.size())
	{
	i++;
	wi++;
	}
/*if (i<nbGrid && tabNomImage.size())
	imTexture = new osg::Image(*imSurface[0]);
else
	imTexture = new osg::Image;*/

for (int i=0;i<TAILLEMAXDUFILTRE;i++)
	{
	//imTexture->scaleImage(nbC/(i+1),nbL/(i+1),imTexture->r());
	gridAffichee[i]  = new osg::HeightField;
	gridAffichee[i]->allocate(nbC/(i+1),nbL/(i+1));
	gridAffichee[i]->setXInterval(echX*(i+1));
	gridAffichee[i]->setYInterval(echY*(i+1));
	if (tabNomFichier.size()==0)
		{
		gridAfficheeg[i]  = new osg::HeightField;
		gridAfficheeg[i]->allocate(nbC/(i+1),nbL/(i+1));
		gridAfficheeg[i]->setXInterval(echX*(i+1));
		gridAfficheeg[i]->setYInterval(echY*(i+1));
		gridAfficheeb[i]  = new osg::HeightField;
		gridAfficheeb[i]->allocate(nbC/(i+1),nbL/(i+1));
		gridAfficheeb[i]->setXInterval(echX*(i+1));
		gridAfficheeb[i]->setYInterval(echY*(i+1));
		}
	}

	
for (int i=0;i<nbL;i++)
	for (int j=0;j<nbC;j++)
		{
		float	z=grid[0]->getHeight(j,i);
		gridAffichee[0]->setHeight(j,i,z*echZ);
		gridMAJ[0][0]=true;	
		if (tabNomFichier.size()==0)
			{
			z=gridv[0]->getHeight(j,i);
			gridAfficheeg[0]->setHeight(j,i,z*echZ);	
			gridMAJ[1][0]=true;	
			z=gridb[0]->getHeight(j,i);
			gridAfficheeb[0]->setHeight(j,i,z*echZ);
			gridMAJ[2][0]=true;	
			}	
		}

for (int i=0;i<TAILLEMAXDUFILTRE;i++)
	surface[i]= new osg::ShapeDrawable(gridAffichee[i]);
if (tabNomFichier.size()==0)
	for (int i=0;i<TAILLEMAXDUFILTRE;i++)
		{
		surfacev[i]= new osg::ShapeDrawable(gridAfficheeg[i]);
		surfaceb[i]= new osg::ShapeDrawable(gridAfficheeb[i]);
		}
etat=getOrCreateStateSet();
tex[0]=new  osg::Texture2D;
tex[0]->setImage(imSurface[0]);
osg::Image *imS=tex[0]->getImage();
imS->copySubImage(0,0,0,imSurface[0]);
tex[0]->dirtyTextureObject();
etat->setTextureAttributeAndModes(0,tex[0]);

}

/*!< Lecture d'une séquence d'images listei (contenant des images) ou listez (contenant des hauteurs). Si la séquence se termine par listez alors la fonction essaye de charger
aussi une séquence listei. Dans le cas d'une séquence listei seules les images sont chargées*/
NanoSurface::NanoSurface(char *nomSeq)
/*! 
\param nomSeq nom du fichier contenant la liste des noms des fichiers de surface 
*/
{
InitNanoSurface();
/* Initialisation des noms des fichiers des hauteurs et des noms de fichiers des images */
LireSequence(nomSeq);

nbL=-1;
nbC=-1;
indGrid=0;


etat= new osg::StateSet; 

for (int ind=0;ind<nbGrid;ind++)
	{
	if (ind<tabNomFichier.size())
		topoValide.push_back(LireFichier(tabNomFichier[ind],ind));
	if (ind<tabNomImage.size())
		{
		int x=0;//LireFichierImage(tabNomImage[ind],ind);
		if (x)
			imValide.push_back(x);
		else
			{
			ImageInfoCV ima(tabNomImage[ind]);
			if (ind==0)
				{
				nbL = ima.LitNbLigne();
				nbC = ima.LitNbColonne();
				VerificationGrille();
				}
			LireFichierImage(&ima);
			imValide.push_back(1);
			AjouteImage(&ima);
			}
		}
	else
			imValide.push_back(0);
//	tex[ind]=new  osg::Texture2D(imSurface[ind]);
	} // for (int ind=0;ind<nbGrid;ind++)
/** initialisation de la grille */
//for (int ind=0;ind<nbGrid;ind++)
//	AjouteImage(imSurface[ind]);

{
int i=0;

deque<osg::Image*>::iterator wi=imSurface.begin();
while (i<nbGrid && !imValide[i] && tabNomImage.size())
	{
	i++;
	wi++;
	}
if (i<nbGrid && tabNomImage.size())
	imTexture = new osg::Image(**wi);
else
	imTexture = new osg::Image;
}
tex[0]=new  osg::Texture2D(imTexture);
for (int i=0;i<TAILLEMAXDUFILTRE;i++)
	{
	//imTexture->scaleImage(nbC/(i+1),nbL/(i+1),imTexture->r());
	gridAffichee[i]  = new osg::HeightField;
	gridAffichee[i]->allocate(nbC/(i+1),nbL/(i+1));
	gridAffichee[i]->setXInterval(echX*(i+1));
	gridAffichee[i]->setYInterval(echY*(i+1));
	if (tabNomFichier.size()==0)
		{
		gridAfficheeg[i]  = new osg::HeightField;
		gridAfficheeg[i]->allocate(nbC/(i+1),nbL/(i+1));
		gridAfficheeg[i]->setXInterval(echX*(i+1));
		gridAfficheeg[i]->setYInterval(echY*(i+1));
		gridAfficheeb[i]  = new osg::HeightField;
		gridAfficheeb[i]->allocate(nbC/(i+1),nbL/(i+1));
		gridAfficheeb[i]->setXInterval(echX*(i+1));
		gridAfficheeb[i]->setYInterval(echY*(i+1));
		}
	}

for (int i=0;i<nbL;i++)
	for (int j=0;j<nbC;j++)
		{
		float	z=grid[0]->getHeight(j,i);
		gridAffichee[0]->setHeight(j,i,z*echZ);
		gridMAJ[0][0]=true;	
		if (tabNomFichier.size()==0)
			{
			z=gridv[0]->getHeight(j,i);
			gridAfficheeg[0]->setHeight(j,i,z*echZ);	
			gridMAJ[1][0]=true;	
			z=gridb[0]->getHeight(j,i);
			gridAfficheeb[0]->setHeight(j,i,z*echZ);
			gridMAJ[2][0]=true;	
			}	
		}

for (int i=0;i<TAILLEMAXDUFILTRE;i++)
	surface[i]= new osg::ShapeDrawable(gridAffichee[i]);
if (tabNomFichier.size()==0)
	for (int i=0;i<TAILLEMAXDUFILTRE;i++)
		{
		surfacev[i]= new osg::ShapeDrawable(gridAfficheeg[i]);
		surfaceb[i]= new osg::ShapeDrawable(gridAfficheeb[i]);
		}
etat=getOrCreateStateSet();
osg::Image *im=tex[0]->getImage();
im->copySubImage(0,0,0,imSurface[0]);
tex[0]->dirtyTextureObject();
etat->setTextureAttributeAndModes(0,tex[0]);

}

/**
Lecture des Images associées aux surfaces pour utilisation comme texture
*/
int NanoSurface::LireFichierImage(char *nomFichier,int ind)
/*! 
\param nomFichier nom du  fichier image à charger
\param ind l'image sera mis à la position ind du tableau imSurface
*/
{
osg::Image *im;
ProcessLectureFichier *p = new ProcessLectureFichier(nomFichier,&im);
p->Create();
//p->Run();
p->Entry();
//while (p->IsRunning());
if (!im)
	return 0;
if (ind==0 && imageFondPresente==1)
	{
	imFond = new ImageInfoCV(im->t(),im->s(),3);
/*
	imFond->DefDataRGBVersPlan(im->data());
	imFond->DefFctImage("symétrique");
*/	
	}

imSurface.push_back(im);
return 1;
}

/** Lecture du fichier texte 
*/

int NanoSurface::LireFichier(char *nomFichier,int ind)
/*! 
\param nomFichier nom du  fichier des altitudes
\param ind les altitudes  seront mis à la position ind du tableau grid
*/
{
static	float **hauteur=NULL;
#ifndef FICHIERFLOAT
string	wF(nomFichier);
if (wF.find(".tif"))
	{
	ImageInfo imZ(nomFichier);
	int nbLigne=-1,nbColonne;
	if (nbLigne==-1)
		{
		nbColonne=imZ.LitNbColonne();
		nbLigne= imZ.LitNbLigne();
		nbLigne =nbLigne;
		nbColonne =nbColonne;
		nbL=nbLigne;
		nbC=nbColonne;
		}
	if (hauteur ==NULL)
		{
		hauteur=new float*[nbLigne+1];
		for (int i=0;i<nbLigne+1;i++)
			hauteur[i]=new float[nbColonne+1];
		}
	float	*dataF;
	unsigned short *datas;
	for (int i=0;i<=nbLigne;i++)
		for (int j=0;j<=nbColonne;j++)
			if (imZ.LitTypePixel()==ENTIER_IMAGE)
				hauteur[i][j]=imZ.LitPixelEntier(i,j)/16384.;	
			else
				hauteur[i][j]=imZ.LitPixelReel(i,j)/16384.;	
	
	}
else
	{
	wF=wF+".bin";		
	fstream f(wF.c_str(),ios_base::in | ios_base::binary );
	if (!f.is_open())
		return 0;
	int nbLigne=-1,nbColonne;
	if (nbLigne==-1)
		{
		nbColonne=735;
		nbLigne=573 ;
		nbLigne =nbLigne;
		nbColonne =nbColonne;
		nbL=nbLigne;
		nbC=nbColonne;
		}
	if (hauteur ==NULL)
		{
		hauteur=new float*[nbLigne+1];
		for (int i=0;i<nbLigne+1;i++)
			hauteur[i]=new float[nbColonne+1];
		}
	int	i=0,j=0;
	for (i=0;i<=nbLigne;i++)
		for (j=0;j<=nbColonne;j++)
			hauteur[i][j]=0;	
	for (i=0;i<nbLigne;i++)
		f.read((char*)hauteur[i],sizeof(float)*(nbColonne+1));
	}
#else
/* Ouverture du fichier et lecture de l'ensemble du fichier en mémoire */
fstream		f(nomFichier,ios_base::in | ios_base::binary );
f.seekg( 0, ios_base::end);
int	tailleFichier=(int)f.tellg()+1;
f.seekg( 0, ios_base::beg);
unsigned char	*c=new unsigned char[tailleFichier];
f.read((char *)c,tailleFichier);
c[tailleFichier-1]=0;
/* Détermination du nombre de lignes et colonnes du fichier en exploitant
la dernière ligne du fichier */
unsigned char *ptr=c+tailleFichier-8;
while (*ptr!=13 && ptr>c)
	ptr--;
istrstream gg((char *)ptr);
int nbLigne=-1,nbColonne;
if (nbLigne==-1)
	{
	gg>>nbColonne>>nbLigne ;
	nbLigne =nbLigne;
	nbColonne =nbColonne;
	nbL=nbLigne;
	nbC=nbColonne;
	}
if (hauteur ==NULL)
	{
	hauteur=new float*[nbLigne+1];
	for (int i=0;i<nbLigne+1;i++)
		hauteur[i]=new float[nbColonne+1];
	}
/** initialisation de la grille */
int	i=0,j=0;
for (i=0;i<=nbL;i++)
	for (j=0;j<=nbC;j++)
		hauteur[i][j]=0;	
i=0;
j=0;
int		col,lig;
while (i<tailleFichier)
	{
	char		tmp[1024];
	float		z;
	
	j=i;
	while (c[j]!=13 && j<tailleFichier)
		j++;
	c[j]=0;
	while (c[j]<=31 && j<tailleFichier)
		j++;
	strcpy(tmp,(char*)c+i);
	i=j;
	istrstream g(tmp);
	
	g>>col>>lig>>z;
//	col = col/2;
//	lig = lig/2;
	hauteur[nbLigne-lig][col]=z;
	}
string	wF(nomFichier);
wF=wF+".bin";		
fstream ggc(wF.c_str(),ios_base::out | ios_base::binary );
for (i=0;i<nbL;i++)
	ggc.write((const char*)hauteur[i],sizeof(float)*(nbColonne+1));
delete c;
#endif
/** initialisation de la grille */
osg::HeightField *g = new osg::HeightField;
g->allocate((nbC),(nbL));
g->setXInterval(echX);
g->setYInterval(echY);
for (int i=0;i<nbL;i++)
	for (int j=0;j<nbC;j++)
		{
		float z=(hauteur[zoom*i][zoom*j]+hauteur[zoom*i+1][zoom*j]+hauteur[zoom*i][zoom*j+1]+hauteur[zoom*i+1][zoom*j+1])/4;
		z=(hauteur[zoom*i][zoom*j]);
		g->setHeight(j,i,z*echZ);	
		}
grid.push_back(g);
return 1;
}

void NanoSurface::VerificationGrille()
{
if (tabNomFichier.size())
	return;
static	float **hauteur=NULL;
if (nbL==-1)
	{
	nbC=imSurface[0]->s();
	nbL=imSurface[0]->t() ;
	}
if (hauteur ==NULL)
	{
	hauteur=new float*[nbL];
	for (int i=0;i<nbL;i++)
		hauteur[i]=new float[nbC];
	int	i=0,j=0;
	for (i=0;i<nbL;i++)
		for (j=0;j<nbC;j++)
			hauteur[i][j]=0;	
	}


return ;
}

void NanoSurface::AjouteImage(osg::Image *im)
{
osg::HeightField *gr = new osg::HeightField;
osg::HeightField *gv = new osg::HeightField;
osg::HeightField *gb = new osg::HeightField;
gr->allocate((nbC+1),(nbL+1));
gr->setXInterval(echX);
gr->setYInterval(echY);
grid.push_back(gr);

gv = new osg::HeightField;
gv->allocate((nbC+1),(nbL+1));
gv->setXInterval(echX);
gv->setYInterval(echY);
gridv.push_back(gv);
gb = new osg::HeightField;
gb->allocate((nbC+1),(nbL+1));
gb->setXInterval(echX);
gb->setYInterval(echY);
gridb.push_back(gb);
unsigned char *data = im->data();

static ImageInfoCV	*imOrig;
static int			preImage=0;
int nbBitPixel=im->getPixelSizeInBits();
int nbPlanCouleur=im->r();
switch(nbBitPixel){
case 16:
	imOrig = new ImageInfoCV(im->t(),im->s(),CV_16UC1); 
//	memcpy(imOrig->LitPlan(0),data,im->t()*im->s()*2);
	break;
case 24:
default :
	imOrig = new ImageInfoCV(im->t(),im->s(),CV_16UC3); 
	imOrig->DefDataRGBVersPlan(data);
	break;
	}
/*
if (preImage  && imageFondPresente==1)
	imOrig->CorrectionFond(imFond,10,10);
else if (imageFondPresente==2)
	imOrig->CorrectionFonctionFond(imOrig,poly,0,0);
*/
switch(imOrig->type()){
case CV_8U :
	{
	unsigned char *datar=(unsigned char *)imOrig->LitPlan(0);
	unsigned char *datav=(unsigned char *)imOrig->LitPlan(1);
	unsigned char *datab=(unsigned char *)imOrig->LitPlan(2);
	if (datav==NULL)
		datav=datar;
	if (datab==NULL)
		datab=datar;
	for (int i=0;i<im->t();i++)
		for (int j=0;j<im->s();j++)
			if (preImage && imageFondPresente==1)
				{
				gr->setHeight(j,i,(255-(*datar++))/255.0);	
				gv->setHeight(j,i,(255-(*datav++))/255.0);	
				gb->setHeight(j,i,(255-(*datab++))/255.0);	
				}
			else
				{
				gr->setHeight(j,i,((*datar++))/255.0);	
				gv->setHeight(j,i,((*datav++))/255.0);	
				gb->setHeight(j,i,((*datab++))/255.0);	
				}
	}
	break;
case T_SHORT_IMAGE :
	{
	unsigned short *datar=(unsigned short *)imOrig->LitPlan(0);
	unsigned short *datav=(unsigned short *)imOrig->LitPlan(1);
	unsigned short *datab=(unsigned short *)imOrig->LitPlan(2);
	if (datav==NULL)
		datav=datar;
	if (datab==NULL)
		datab=datar;
	float maxNiv=16384.0;
	for (int i=0;i<im->t();i++)
		for (int j=0;j<im->s();j++)
			if (preImage && imageFondPresente==1)
				{
				gr->setHeight(j,i,(maxNiv-(*datar++)));	
				gv->setHeight(j,i,(maxNiv-(*datav++))/maxNiv);	
				gb->setHeight(j,i,(maxNiv-(*datab++))/maxNiv);	
				}
			else
				{
				gr->setHeight(j,im->t()-i-1,((*datar++)));	
				gv->setHeight(j,im->t()-i-1,((*datav++)));	
				gb->setHeight(j,im->t()-i-1,((*datab++)));	
				}
	}
	break;
	}
preImage=1;
}

void NanoSurface::AjouteImage(ImageInfoCV *im)
{
osg::HeightField *gr = new osg::HeightField;
osg::HeightField *gv = new osg::HeightField;
osg::HeightField *gb = new osg::HeightField;
gr->allocate((nbC+1),(nbL+1));
gr->setXInterval(echX);
gr->setYInterval(echY);
grid.push_back(gr);

gv = new osg::HeightField;
gv->allocate((nbC+1),(nbL+1));
gv->setXInterval(echX);
gv->setYInterval(echY);
gridv.push_back(gv);
gb = new osg::HeightField;
gb->allocate((nbC+1),(nbL+1));
gb->setXInterval(echX);
gb->setYInterval(echY);
gridb.push_back(gb);
switch(im->LitTaillePixel()){
case T_OCTET_IMAGE :
	{
	unsigned char *datar=(unsigned char *)im->LitPlan(0);
	unsigned char *datav=(unsigned char *)im->LitPlan(1);
	unsigned char *datab=(unsigned char *)im->LitPlan(2);
	if (datav==NULL)
		datav=datar;
	if (datab==NULL)
		datab=datar;
	for (int i=0;i<im->LitNbLigne();i++)
		for (int j=0;j<im->LitNbColonne();j++)
				{
				gr->setHeight(j,i,((*datar++))/255.0);	
				gv->setHeight(j,i,((*datav++))/255.0);	
				gb->setHeight(j,i,((*datab++))/255.0);	
				}
	}
	break;
case T_SHORT_IMAGE :
	{
	unsigned short *datar=(unsigned short *)im->LitPlan(0);
	unsigned short *datav=(unsigned short *)im->LitPlan(1);
	unsigned short *datab=(unsigned short *)im->LitPlan(2);
	if (datav==NULL)
		datav=datar;
	if (datab==NULL)
		datab=datar;
	float maxNiv=16384.0;
	for (int i=0;i<im->LitNbLigne();i++)
		for (int j=0;j<im->LitNbColonne();j++)
				{
				gr->setHeight(j,im->LitNbLigne()-i-1,((*datar++)));	
				gv->setHeight(j,im->LitNbLigne()-i-1,((*datav++)));	
				gb->setHeight(j,im->LitNbLigne()-i-1,((*datab++)));	
				}
	}
	break;
	}
}

void NanoSurface::InitPalette(int nbCouleur)
{
double pi=acos(-1.0);
long i;
nbCouleurPalette=nbCouleur;
if (!pCouleur)
	{
	pCouleur = new wxColour[nbCouleur];
	pJet = new wxColour[16384];
	pRainbow = new wxColour[16384];
	pPerso = new wxColour[16384];
	pPersoInv = new wxColour[16384];
	float red,green,blue;
	{
	ifstream ff("jetcolor16384.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pJet[i].Set(red*255,green*255,blue*255);
			}
		ff.close();
		}
	}
	
	{
	ifstream ff("rainbow16384.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pRainbow[i].Set(red*255,green*255,blue*255);
			}
		ff.close();
		}
	}
	{
	ifstream ff("palperso.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pPerso[i].Set(red,green,blue);
			}
		ff.close();
		}
	}
	{
	ifstream ff("palpersoinv.txt");
	if (ff.is_open())
		{
		for (int i=0;i<16384;i++)
			{
			ff>>red>>green>>blue;
			pPersoInv[i].Set(red,green,blue);
			}
		ff.close();
		}
	}
	
	pAleatoire= new wxColour[nbCouleur];
	}
for (i=0;i<nbCouleur;i++)
		pCouleur[i].Set(127*(1+cos(2*pi*i/nbCouleur)),127*(1+cos(2*pi*i/nbCouleur+2*pi/3)),127*(1+cos(2*pi*i/nbCouleur+4*pi/3)));
pCouleur[nbCouleur-1]=*wxWHITE;
pCouleur[0]=*wxBLACK;
for (i=0;i<nbCouleur;i++)
		pAleatoire[i].Set(rand()&0xFF,rand()&0xFF,rand()&0xFF);

}



void NanoSurface::LireFichierImage(Image *im)
{
float kPalette;
if (indPalette==0 || indPalette>=6)
	kPalette=16384./(finNivPalette-debNivPalette+1);
else
	kPalette=256./(finNivPalette-debNivPalette+1);

if (im->LitTaillePixel()&T_OCTET_IMAGE)
	{
	unsigned char *datar = (unsigned char*)im->LitPlan(0);
	unsigned char *datav = (unsigned char*)im->LitPlan(1);
	unsigned char *datab = (unsigned char*)im->LitPlan(2);

	tabNomImage.push_back("xxxx");
	osg::Image* iFond = new osg::Image;
	iFond->setImage( im->LitNbColonne(),im->LitNbLigne(), 1,
				3, GL_RGB, GL_UNSIGNED_BYTE,
				new unsigned char[im->LitNbColonne()* im->LitNbLigne()* 3],
				osg::Image::USE_NEW_DELETE);
	unsigned char *data ;

	//iFond->setImage(
	for (int i=0;i<im->LitNbLigne();i++)
		{
		data = iFond->data()+3*(im->LitNbLigne()-i-1)*im->LitNbColonne();
		for (int j=0;j<im->LitNbColonne();j++)
			{
			if (im->LitNbPlan()==3)
				{
				*data++=*datav++;
				*data++=*datab++;
				}
			else	
				{
				*data++=*datar;
				*data++=*datar;
				}
			*data++=*datar++;
			} // for (int j=0;j<im->LitNbColonne();j++)
		}
	imSurface.push_back(iFond);
	}
else if (im->LitTaillePixel()==T_SHORT_IMAGE)
	{
	unsigned short *datar = (unsigned short*)im->LitPlan(0);
	unsigned short *datav = (unsigned short*)im->LitPlan(1);
	unsigned short *datab = (unsigned short*)im->LitPlan(2);

	tabNomImage.push_back("xxxx");
	osg::Image* iFond = new osg::Image;
	iFond->setImage( im->LitNbColonne(),im->LitNbLigne(), 1,
				3, GL_RGB, GL_UNSIGNED_BYTE,
				new unsigned char[im->LitNbColonne()* im->LitNbLigne()* 3],
				osg::Image::USE_NEW_DELETE);
	unsigned char *data ;

	//iFond->setImage(
	for (int i=0;i<im->LitNbLigne();i++)
		{
		data = iFond->data()+3*(im->LitNbLigne()-i-1)*im->LitNbColonne();
		for (int j=0;j<im->LitNbColonne();j++)
			if (datav)
				{
				*data++=(*datar++)/256;
				*data++=(*datav++)/256;
				*data++=(*datab++)/256;
				} // for (int j=0;j<im->LitNbColonne();j++)
			else
				{
				unsigned short val = (*datar-debNivPalette); 
				if (*datar<debNivPalette)
					val=0;
				if (kPalette!=1)
					val = val *kPalette;
				if (val>=nbCouleurPalette)
					val=nbCouleurPalette-1;
				*data++=pCouleur[val].Red();
				*data++=pCouleur[val].Green();
				*data++=pCouleur[val].Blue();
				} // for (int j=0;j<im->LitNbColonne();j++)
		}
	imSurface.push_back(iFond);
	}
else if (im->LitTaillePixel()==T_LONG_IMAGE && im->LitTypePixel()==REEL_IMAGE)
	{
	float	max[3]={-1e8,-1e8,-1e8};
	float	min[3]={1e8,1e8,1e8};
	for (int k=0;k<im->LitNbPlan();k++)
		{
		float *data = (float*)im->LitPlan(k);
		for (int i=0;i<im->LitNbLigne();i++)
			for (int j=0;j<im->LitNbColonne();j++,data++)
				if (*data>max[k])
					max[k]=*data;
				else if (*data<min[k])
					min[k] = *data;
		}
	float *datar = (float*)im->LitPlan(0);
	float *datav = (float*)im->LitPlan(1);
	float *datab = (float*)im->LitPlan(2);

	tabNomImage.push_back("xxxx");
	osg::Image* iFond = new osg::Image;
	iFond->setImage( im->LitNbColonne(),im->LitNbLigne(), 1,
				3, GL_RGB, GL_UNSIGNED_BYTE,
				new unsigned char[im->LitNbColonne()* im->LitNbLigne()* 3],
				osg::Image::USE_NEW_DELETE);
	unsigned char *data ;

	//iFond->setImage(
	for (int i=0;i<im->LitNbLigne();i++)
		{
		data = iFond->data()+3*(im->LitNbLigne()-i-1)*im->LitNbColonne();
		for (int j=0;j<im->LitNbColonne();j++)
			{
			if (im->LitNbPlan()==1)
				{
				*data++=(*datar-min[0])*256/(max[0]-min[0]);
				*data++=(*datar-min[0])*256/(max[0]-min[0]);
				*data++=(*datar++-min[0])*256/(max[0]-min[0]);
				}
			else
				{
				*data++=(*datar++ -min[0])*256/(max[0]-min[0]);
				*data++=(*datav++ -min[1])*256/(max[1]-min[1]);
				*data++=(*datab++ -min[2])*256/(max[2]-min[2]);
				}
			} // for (int j=0;j<im->LitNbColonne();j++)
		}
	imSurface.push_back(iFond);
	}
}

void NanoSurface::Maj(int d)
{

double pi=acos(-1.0);
static float value,k=1;
value= clock()/double(CLOCKS_PER_SEC) ;//sin(fs->getSimulationTime());
//etat->setTextureMode(indGrid,GL_TEXTURE_2D,osg::StateAttribute::OFF);

if (d!=0)
	for (int i=0;i<TAILLEMAXDUFILTRE;i++)
		{
		gridMAJ[0][i]=false;
		gridMAJ[1][i]=false;
		gridMAJ[2][i]=false;
		}
indGrid +=d;
if (indGrid>=nbGrid)
	indGrid=0;
if (indGrid<0)
	indGrid=nbGrid-1;
osg::HeightField *gridSrc,*gridDst;

int canal=0;
switch(couleurActive){
case 'R':
	gridSrc=grid[indGrid];
	gridDst=gridAffichee[tailleDuFiltre];
	canal=0;
	break;
case 'G':
	gridSrc=gridv[indGrid];
	gridDst=gridAfficheeg[tailleDuFiltre];
	canal=1;
	break;
case 'B':
	gridSrc=gridb[indGrid];
	gridDst=gridAfficheeb[tailleDuFiltre];
	canal=2;
	break;
	}
gridMAJ[canal][tailleDuFiltre]=true;
float	facteurCorrectif=float(nbC+1)/gridDst->getNumColumns();
gridDst->setXInterval(echX*facteurCorrectif);
facteurCorrectif=float(nbL+1)/gridDst->getNumRows();
gridDst->setYInterval(echY*facteurCorrectif);
int pas=tailleDuFiltre+1;
if (pas==0)
	pas=1;

for (int i=tailleDuFiltre,y=0;i<nbL;i+=pas,y++)
	for (int j=tailleDuFiltre,x=0;j<nbC;j+= pas,x++)
		if (tailleDuFiltre==0)
			gridDst->setHeight(j,i,echZ*gridSrc->getHeight(j,i));
		else
			{
			float	h=0;
			int		nbElt=0;
			for (int l=i-tailleDuFiltre;l<=i+tailleDuFiltre;l++)
				if (l>=0 && l<nbL)
					for (int k=j-tailleDuFiltre;k<=j+tailleDuFiltre;k++)
						if (k>=0 && k<nbC)
							{
							nbElt++;
							h+=gridSrc->getHeight(k,l);
							}
			if (x<=gridDst->getNumColumns() &&
				y<=gridDst->getNumRows())
				if (nbElt>0)
					gridDst->setHeight(x,y,echZ*h/nbElt);				
				else	
					gridDst->setHeight(x,y,0);				
			}	
			

osg::Image *im=tex[0]->getImage();
if (im->getPixelSizeInBits()==24)
	{
	float kPalette;
	if (indPalette==0 || indPalette>=6)
		kPalette=16384./(finNivPalette-debNivPalette+1);
	else
		kPalette=256./(finNivPalette-debNivPalette+1);
	unsigned char *data=im->data();
	for (int i=tailleDuFiltre,y=0;i<nbL;i+=pas,y++)
		for (int j=tailleDuFiltre,x=0;j<nbC;j+= pas,x++)
			{
			double z=gridSrc->getHeight(j,i);
			for (int l=i-tailleDuFiltre;l<=i+tailleDuFiltre;l++)
				if (l>=0 && l<nbL)
					for (int k=j-tailleDuFiltre;k<=j+tailleDuFiltre;k++)
						if (k>=0 && k<nbC)
							{
							int	posMem=(l*im->s()+k)*3;
							unsigned short val = (z-debNivPalette); 
							if (z<debNivPalette)
								val=0;
							if (kPalette!=1)
								val = val *kPalette;
							if (val>=nbCouleurPalette)
								val=nbCouleurPalette-1;
							data[posMem] = pCouleur[val].Red();
							data[posMem+1] = pCouleur[val].Green();
							data[posMem+2] = pCouleur[val].Blue();
							}
			}
	}
else if (im->getPixelSizeInBits()==16)
	{
	unsigned short *data=(unsigned short *)imSurface[indGrid]->data();
	for (int i=tailleDuFiltre,y=0;i<nbL;i+=pas,y++)
		for (int j=tailleDuFiltre,x=0;j<nbC;j+= pas,x++)
			{
			double z=gridDst->getHeight(x,y)*echZ;
			int indH=239-(int)((z-offsetZ0)/echZ*240);
			if (indH<0)
				indH=0;
			if (indH>239)
				indH=239;
			for (int l=i-tailleDuFiltre;l<=i+tailleDuFiltre;l++)
				if (l>=0 && l<nbL)
					for (int k=j-tailleDuFiltre;k<=j+tailleDuFiltre;k++)
						if (k>=0 && k<nbC)
							{
							int	posMem=(l*imSurface[indGrid]->s()+k);
							data[posMem] = data[posMem]<<3;
							}
			}
	}

//tex[0]->setImage(imSurface[indGrid]);

//im->copySubImage(0,0,0,imSurface[indGrid]);
//im->scaleImage(0,0,0,imSurface[indGrid]);
tex[0]->dirtyTextureObject();
etat->setTextureAttributeAndModes(0,tex[0]);

}


char	*NanoSurface::LireNomFichier(int n)
{
if (n<0 || n>=nbGrid)
	return NULL;
if (tabNomFichier.size())
	{
	return tabNomFichier[n];
	}
return NULL;
}

char	*NanoSurface::LireNomFichierImage(int n)
{
if (n<0 || n>=nbGrid)
	return NULL;
if (tabNomImage.size())
	return tabNomImage[n];
return NULL;
}

double		NanoSurface::LireHauteur(int c,int l,int ind)
{
if (ind==-1)
	ind=indGrid;
if (ind<0 || ind>=nbGrid)
	ind=indGrid;

return grid[ind]->getHeight(c,l);
}


void	NanoSurface::DefTailleFiltre(int i)
{
if (i<0 || i>=TAILLEMAXDUFILTRE || i==tailleDuFiltre)
	return; 
//osg::Group* root = dynamic_cast<osg::Group*>(((wxOsgApp*)osgApp)->LireViewer()->getSceneData());
switch(couleurActive){
case 'R':
	removeDrawable(surface[tailleDuFiltre].get());
	break;
case 'G':
	removeDrawable(surfacev[tailleDuFiltre].get());
	break;
case 'B':
	removeDrawable(surfaceb[tailleDuFiltre].get());
	break;
	}

tailleDuFiltre=i;
switch(couleurActive){
case 'R':
	addDrawable(surface[tailleDuFiltre].get());
	break;
case 'G':
	addDrawable(surfacev[tailleDuFiltre].get());
	break;
case 'B':
	addDrawable(surfaceb[tailleDuFiltre].get());
	break;
	}

}

void	NanoSurface::SelectionComposante(char c)
{
if (c!='R' && c!='G' && c!='B')
	return; 
if (c==couleurActive)
	return;
if (c=='G' && gridv.size()==0)
	return;
if (c=='B' && gridb.size()==0)
	return;
	
switch(couleurActive){
case 'R':
	removeDrawable(surface[tailleDuFiltre].get());
	break;
case 'G':
	removeDrawable(surfacev[tailleDuFiltre].get());
	break;
case 'B':
	removeDrawable(surfaceb[tailleDuFiltre].get());
	break;
	} // switch(couleurActive)
int canal=0;
couleurActive=c;
switch(c){
case 'R':
	canal=0;
	break;
case 'G':
	canal=1;
	break;
case 'B':
	canal=2;
	break;
	}
if (!gridMAJ[canal][tailleDuFiltre] )
	{
	Maj(0);
	((wxOsgApp*)osgApp)->MAJGraphiqueNanoSurface(0);
	}
switch(c){
case 'R':
	addDrawable(surface[tailleDuFiltre].get());
	canal=0;
	break;
case 'G':
	addDrawable(surfacev[tailleDuFiltre].get());
	canal=1;
	break;
case 'B':
	addDrawable(surfaceb[tailleDuFiltre].get());
	canal=2;
	break;
	}
}

osg::ref_ptr<osg::ShapeDrawable> NanoSurface::LitShapeDrawable()
{
switch(couleurActive){
case 'R':
	return surface[tailleDuFiltre];
	break;
case 'G':
	return surfacev[tailleDuFiltre];
	break;
case 'B':
	return surfaceb[tailleDuFiltre];
	break;
	}
return surface[tailleDuFiltre];
}


int NanoSurface::NbLigneImage(int ind)
{
if (imValide[ind]) 
	{
	return imSurface[ind]->t();
	}
return -1;
}

int NanoSurface::NbColonneImage(int ind)
{
if (imValide[ind]) 
	{
	return imSurface[ind]->s();
	}
return -1;
}

int	NanoSurface::ImageExiste(int ind)
{
if (ind<0 || ind>=imValide.size())
	return 0;

return imValide[ind];
}

int	NanoSurface::TopographieExiste(int ind)
{
if (ind<0 || ind>=topoValide.size())
	return 0;
return topoValide[ind];
}





void NanoSurface::AjouteImage(ImageInfo **im)
/*! 
\param nomSeq nom du fichier contenant la liste des noms des fichiers de surface 
*/
{
//InitNanoSurface();
SelectPalette(8);
nbL=-1;
nbC=-1;

nbGrid++;
etat= new osg::StateSet; 
nbL=im[0]->LitNbLigne();
nbC=im[0]->LitNbColonne();

	LireFichierImage(im[0]);
	imValide.push_back(true);
	topoValide.push_back(false);
/** initialisation de la grille */
AjouteImage(im[0]);

//VerificationGrille();
deque<osg::Image*>::iterator wi=imSurface.begin();
int ib=0;
while (ib<nbGrid && !imValide[ib] && tabNomImage.size())
	{
	ib++;
	wi++;
	}

for (int i=0;i<nbL;i++)
	for (int j=0;j<nbC;j++)
		{
		float	z=grid[0]->getHeight(j,i);
		gridAffichee[0]->setHeight(j,i,z*echZ);
		gridMAJ[0][0]=true;	
		if (tabNomFichier.size()==0)
			{
			z=gridv[0]->getHeight(j,i);
			gridAfficheeg[0]->setHeight(j,i,z*echZ);	
			gridMAJ[1][0]=true;	
			z=gridb[0]->getHeight(j,i);
			gridAfficheeb[0]->setHeight(j,i,z*echZ);
			gridMAJ[2][0]=true;	
			}	
		}

//im->copySubImage(0,0,0,imSurface[indGrid]);
//im->scaleImage(0,0,0,imSurface[indGrid]);
osg::Image *imS=tex[0]->getImage();
imS->copySubImage(0,0,0,imSurface[ib]);
tex[0]->dirtyTextureObject();
etat->setTextureAttributeAndModes(0,tex[0]);
}

void NanoSurface::SelectPalette(int ind)
{
wxColour	*p=NULL;
indPalette=ind;
switch(indPalette){
case 6:
	p=pJet;
	break;
case 7:
	p=pRainbow;
	break;
case 8:
	p=pPerso;
	break;
case 9:
	p=pPersoInv;
	break;
	}
for (int i=0;p  && i<16384;i++)
		pCouleur[i].Set(p[i].Red(),p[i].Green(),p[i].Blue());
return;
}
