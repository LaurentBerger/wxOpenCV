#include "FenetrePrincipale.h"
#include "ControleCamera.h"
#include "imagestat.h"



/*!< La fonction nouvelle Image est appelée pour mettre à jour les données de l'image imAcq. 
imAcq est calculée selon la valeur des parmètres modeMoyenne, correctionBiais, correctionTache et correctionFond.<br>
si modeMoyenne est à 1 alors un filtre récursif passe bas d'ordre 1 est utilisé pour calculer imAcq en utilisant imAcq à t-1.
Les corrections sont faites dans l'ordre correction du biais (1), correction des taches (2) et correctionFonction (3).

*/

void FenetrePrincipale::NouvelleImage()
{
if (ModeCamera())
	{
	imageTraitee=false;
	}
#ifdef __TOTO
unsigned long taille=CtrlCamera()->NbLigne()*CtrlCamera()->NbColonne();
unsigned short* data= (unsigned short*)(imAcqBrut1->data);
if (ModeCamera() && CtrlCamera())
	CtrlCamera()->Image(data,taille);
int nbPixels=imAcqBrut1->LitNbLigne()*imAcqBrut1->LitNbColonne();
if (modeMoyenne)
	{
	unsigned short *valAcq=(unsigned short *)imAcq->LitPlan(0);
	unsigned short *valAcq2=(unsigned short *)imAcq2->LitPlan(0);
	unsigned short *val=(unsigned short *)imAcqBrutFil->LitPlan(0);
	unsigned short *val2=(unsigned short *)imAcqBrutFilMax->LitPlan(0);
	unsigned short *valB1=(unsigned short *)imAcqBrut1->LitPlan(0);
	unsigned short *valB2=(unsigned short *)imAcqBrut2->LitPlan(0);
	for (int i=0;i<nbPixels;i++,val++,valB1++,valB2++,valAcq++,valAcq2++,val2++)
		{
		*val = bbButter[indFiltreMoyenne]*(*valB1 + *valB2)-aaButter[indFiltreMoyenne]* *val;
		*val2 = bbButter[1]*(*valB1 + *valB2)-aaButter[1]* *val2;
		*valAcq=*val;
		*valAcq2=*val2;
		}
	}
else
	{
	unsigned short *valAcq=(unsigned short *)imAcq->LitPlan(0);
	unsigned short *valAcq2=(unsigned short *)imAcq2->LitPlan(0);
	unsigned short *valB1=(unsigned short *)imAcqBrut1->LitPlan(0);
	for (int i=0;i<nbPixels;i++,valB1++,valAcq++,valAcq2++)
		{
		*valAcq=*valB1;
		*valAcq2=*valB1;
		}
	}
if (correctionBiais )
	{
	unsigned short *biais=(unsigned short *)nivBiais->LitPlan(0);
	unsigned short *val=(unsigned short *)imAcq->LitPlan(0);
	unsigned short *val2=(unsigned short *)imAcq2->LitPlan(0);
	for (int i=0;i<nbPixels;i++,biais++,val++,val2++)
		if (*val>*biais)
			{
			*val = *val- *biais;
			*val2 = *val2- *biais;
			}
		else 
			{
			*val=0;
			*val2=0;
			}
	}
if (correctionTache)
	{
	float	*tache=(float*)imTache->LitPlan(0);
	unsigned short *val=(unsigned short *)imAcq->LitPlan(0);
	unsigned short *val2=(unsigned short *)imAcq2->LitPlan(0);
	for (int i=0;i<nbPixels;i++,val++,val2++,tache++)
		{
		*val = *val * *tache;
		*val2 = *val2 * *tache;
		}
	}
if (correctionFonction)
	{
	float *valF=(float *)imQuadrique->LitPlan(0);
	unsigned short *val=(unsigned short *)imAcq->LitPlan(0);
	unsigned short *val2=(unsigned short *)imAcq2->LitPlan(0);
	switch(imAcq->LitTypeModeleFctSEEC()){
	case 0:
	case 1:
	case 2:
		for (int i=0;i<nbPixels;i++,valF++,val++,val2++)
				{
				*val = *val * *valF;
				*val2 = *val2 * *valF;
				}
		break;
	case 3:
		for (int i=0;i<nbPixels;i++,valF++,val++,val2++)
								{
				*val = (*val-poly[0][6])  * *valF;
				*val2 = (*val2-poly[0][6])  * *valF;
				}
		break;
	case 4:
		for (int i=0;i<nbPixels;i++,valF++,val++,val2++)
				{
				*val = (*val-poly[0][10])  * *valF;
				*val2 = (*val2-poly[0][10])  * *valF;
				}
		break;
		}
	}
	
void *tmp=(void*)imAcqBrut2;
imAcqBrut2 =  imAcqBrut1;
imAcqBrut1 = (ImageInfoCV*)tmp;
switch(typeAcqImage){
case 0:
	break;
case 1: // Acquisition image noire
	{
	int nbPixels=nivBiais->LitNbLigne()*nivBiais->LitNbColonne();
	unsigned short	*data=(unsigned short*)nivBiais->LitPlan(0);
	unsigned short *val=(unsigned short *)imAcqBrut1->LitPlan(0);
	for (int i=0;i<nbPixels;i++,data++,val++)
		*data = (*data*nbImageBiais+ *val)/(nbImageBiais+1);
	nbImageBiais += dImageBiais;
	}
	break;
case 2: // 2 Image des taches
	{
	int nbPixels=imTache->LitNbLigne()*imTache->LitNbColonne();
	float m=imAcq->Moyenne();
	unsigned short *data=(unsigned short *)imRefTache->LitPlan(0);
	unsigned short *val=(unsigned short *)imAcq->LitPlan(0);
	for (int i=0;i<nbPixels;i++,data++,val++)
		if (*val>0)
			*data = (*data*nbImageTache+ *val)/(nbImageTache+1);
	nbImageTache += dImageTache;
	}
	break;
case 3: // 3 Image fonction fond
	break;
	}
#endif
delete feuille->BitmapAffichee();
feuille->BitmapAffichee(NULL);
//wxRect r=feuille->GetClientRect();
//feuille->RefreshRect (r, false);
//feuille->Update();

}


void FenetrePrincipale::MAJNouvelleImage()
{
if (interdireAffichage)
	return;
if (statActif)
	imgStatIm->Plot(modeCamera);

switch(modeImage){
case 0:
//	if (modeFiltre==0)
		{
		if (cam && cam->IsRunning())
			{
			wxCriticalSectionLocker enter(travailCam);
			DIB(imAcq);
			}
		else
			DIB(imAcq);
		}
//	else
//		DIB(imAcq2);
	break;
case 1:
case 2:
case 3:
case 4:
	break;
	}
if (cam && cam->IsRunning())
	{
    feuille->Refresh();
	}
else
	{
	wxRect r=feuille->GetClientRect();
	feuille->RefreshRect (r, false);
	feuille->Update();
	}
if (feuille->ModeRectangle())
	{
	for (int i=0;i<10;i++)
		if (!feuille->Rectangle(i)->IsEmpty())
			{
			float stat[5];	
/*
			imAcq->StatZone(rectSelect[i].GetTop(),rectSelect[i].GetBottom(),
				rectSelect[i].GetLeft(),rectSelect[i].GetRight(),stat);
*/
			osgApp->ImgStat()->MAJStatRec(i,stat);
			if (i==feuille->IndiceRectangleSelec())
				TracerRectangle(i,1);
			else
				TracerRectangle(i,0);
			}
	}	
}
