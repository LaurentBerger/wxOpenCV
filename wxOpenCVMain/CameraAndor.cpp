#include "ImageInfo.h"
#include "CameraAndor.h"
#include "EvenementCamera.h"
#include <fstream>

using namespace std;

#ifdef __LIBANDOR__
CameraAndor::CameraAndor(void)
{
wxString		repTravail=wxGetCwd();

wxString nomOrdinateur(wxGetHostName());



chaineErreur="";
modeAcquisition=5;
modeLecture=4;
char	tmp[1024];
emCCDGainMode=-1;
strcpy(tmp,repTravail.mb_str( wxConvUTF8));
erreurFct=Initialize(tmp);  // Initialize driver in current directory
testDriver=1;
if (erreurFct)
	{
	AjouteMsgErreur("Initialization errors:\n");
    AjouteMsgErreur("Initialize Error\n");
    }
// Get camera capabilities
caps.ulSize = sizeof(AndorCapabilities);
erreurFct=GetCapabilities(&caps);
if(erreurFct!=DRV_SUCCESS)
	{
	testDriver=0;
	AjouteMsgErreur("Get Andor Capabilities information Error\n");
	}

// Get Head Model
erreurFct=GetHeadModel(nomCamera);
if(erreurFct!=DRV_SUCCESS)
  AjouteMsgErreur("Get Head Model information Error\n");

// Get detector information
erreurFct=GetDetector(&nbColonnePhys,&nbLignePhys);
if(erreurFct!=DRV_SUCCESS)
  AjouteMsgErreur("Get Detector information Error\n");

// Set acquisition mode to required setting specified in xxxxWndw.c
erreurFct=SetAcquisitionMode(modeAcquisition);
if(erreurFct!=DRV_SUCCESS)
  AjouteMsgErreur("Set Acquisition Mode Error\n");

// Set read mode to required setting specified in xxxxWndw.c
erreurFct=SetReadMode(modeLecture);
if(erreurFct!=DRV_SUCCESS)
  AjouteMsgErreur("Set Read Mode Error\n");

// Set Vertical speed to max
int STemp = 0;
int VSnumber = 0;
GetNumberVSSpeeds(&nbVitesseDecalageVerticale);
if (nbVitesseDecalageVerticale)
	vitesseDecalageVerticale = new float[nbVitesseDecalageVerticale];
for(int iSpeed=0; iSpeed<nbVitesseDecalageVerticale; iSpeed++)
	{
	GetVSSpeed(iSpeed, vitesseDecalageVerticale+iSpeed);
	if(vitesseDecalageVerticale[iSpeed] > STemp)
		{
		STemp = vitesseDecalageVerticale[iSpeed];
		VSnumber = iSpeed;
		}
	}
erreurFct=SetVSSpeed(VSnumber);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("Set Vertical Speed Error\n");

// Set Horizontal Speed to max
STemp = 0;
int HSnumber = 0;
int nbVitHMax=0;
int ADnumber = 0;
erreurFct = GetNumberADChannels(&nbCannauxAD);
if (erreurFct != DRV_SUCCESS)
  AjouteMsgErreur("Get number AD Channel Error\n");
else 
	{
	vitesseDecalageHorizontale = new float*[nbCannauxAD];
	for (int iAD = 0; iAD < nbCannauxAD; iAD++) 
		{
		GetNumberHSSpeeds(iAD, 0, &nbVitesseDecalageHorizontale);
		vitesseDecalageHorizontale[iAD] = new float[nbVitesseDecalageHorizontale+1];
		vitesseDecalageHorizontale[iAD][0] = nbVitesseDecalageHorizontale;
		
		for (int iSpeed = 0; iSpeed < nbVitesseDecalageHorizontale; iSpeed++) 
			{
			GetHSSpeed(iAD, 0, iSpeed, &vitesseDecalageHorizontale[iAD][iSpeed+1]);
			if(vitesseDecalageHorizontale[iAD][iSpeed+1] > STemp)
				{
				STemp = vitesseDecalageHorizontale[iAD][iSpeed+1];
				HSnumber = iSpeed;
				ADnumber = iAD;
				}
			}
		}
	}

erreurFct=SetADChannel(ADnumber);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("Set AD Channel Error\n");
erreurFct=SetHSSpeed(0,HSnumber);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("Set Horizontal Speed Error\n");
  // Wait for 2 seconds to allow MCD to calibrate fully before allowing an
  // acquisition to begin
if (testDriver)
	wxMilliSleep(2000);
//PrepareAcquisition();
pisteCentrale=1;				// piste ensemble de lignes
nbLignePiste=1;				// nombre de ligne composant une piste
nbPiste=nbLignePhys;
indLignePremierpixel=1;
nbLigneInterPiste=1;
nbColonneParPixel=1;
nbLigneParPixel=1;
colonneDebut=1;
ligneDebut=1;
colonneFin=nbColonnePhys;
ligneFin=nbLignePhys;
tempsExposition=0.1;
nbScanParAccumulation=1;
periodeAccumulation=1;	// en seconde
nbAccumulationparCinetique=1;
periodeCinetique=1;			// en seconde
frameTransferMode=0;
modeDeclenchement=0;			// Mode de déclenchement de l'acquisition p45 SDK Andor

controleObturateur=0;			// Controle de l'obturateur	p52 SDK Andor

controleTemperature=1;		// Controle de la température de refroidissement du CCD
erreurFct=CoolerON();
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("Cooler Error\n");
erreurFct=GetTemperature(&temperature);				// Temperature en °C
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur(" Error\n");
erreurFct=SetExposureTime(tempsExposition);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetExposureTime Error\n");
erreurFct=SetImage(nbColonneParPixel,nbLigneParPixel,colonneDebut,colonneFin,ligneDebut,ligneFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetImage Error\n");
erreurFct=StartAcquisition();
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("StartAcquisition Error\n");
if (GetFunctionEMCCDGain())
	{
	SetEMGainMode(2);
	emCCDGainMode=2;
	if(erreurFct!=DRV_SUCCESS)
		AjouteMsgErreur("SetEMGainMode Error\n");
	}
HANDLE hEvent=CreateEvent(NULL,FALSE,FALSE,_T("ANDOREVENT"));
if ( !hEvent ) return ;
SetDriverEvent(hEvent);
}

CameraAndor::~CameraAndor(void)
{
AbortAcquisition();
chaineErreur="";
erreurFct=CoolerOFF();        // Switch off cooler (if used)
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("Error switching cooler off");

erreurFct=ShutDown();
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("Error shutting down");
}

void CameraAndor::DefTempsExposition(double x)
{
chaineErreur="";
drapeauErreur=0;
AbortAcquisition();
tempsExposition = (float)x;
erreurFct=SetExposureTime(tempsExposition);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetExposureTime Error\n");
StartAcquisition();
}

void CameraAndor::DefModeGain(int i)
{
chaineErreur="";
drapeauErreur=0;
AbortAcquisition();
erreurFct=SetEMGainMode(i);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetEMGainMode Error\n");
else
	emCCDGainMode=i;
StartAcquisition();
}

void CameraAndor::DefEMCCDGain(int v)
{
chaineErreur="";
drapeauErreur=0;
AbortAcquisition();
erreurFct=SetEMCCDGain(v);
if (erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetEMGain Error\n");
StartAcquisition();
}

void CameraAndor::LireIntervalGain(int *x,int *y)
{
drapeauErreur=0;
if (GetEMGainRange(x,y)!=DRV_SUCCESS)
	AjouteMsgErreur("GetEMGainRange Error\n");
}

void CameraAndor::DefNombreAccumulation(int x)
{
drapeauErreur=0;
chaineErreur="";
nbScanParAccumulation = x;

AbortAcquisition();
erreurFct=SetNumberAccumulations(nbScanParAccumulation);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetNumberAccumulations Error\n");
StartAcquisition();
}

void CameraAndor::DefFrequenceAccumulation(double x)
{
chaineErreur="";
if (x==0)
	return;
periodeAccumulation=1/x;

AbortAcquisition();
erreurFct=SetAccumulationCycleTime(periodeAccumulation);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetAccumulationCycleTime Error\n");
StartAcquisition();
}

void CameraAndor::DefNombreCinetique(int x)
{
chaineErreur="";
nbAccumulationparCinetique = x;
erreurFct=SetNumberKinetics(nbAccumulationparCinetique);
AbortAcquisition();
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetNumberKinetics Error\n");
//PrepareAcquisition();
//StartAcquisition();
}

void CameraAndor::DefFrequenceCinetique(double x)
{
chaineErreur="";
if (x==0)
	return;
periodeCinetique=1/x;

AbortAcquisition();
erreurFct=SetKineticCycleTime(periodeCinetique);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetExposureTime Error\n");
StartAcquisition();
}

void CameraAndor::DefTailleImage(int article)
{
chaineErreur="";
switch(article){
case 0:
	colonneDebut=1;
	ligneDebut=1;
	colonneFin=1004;
	ligneFin=1002;
	break;
case 1:
	colonneDebut=1;
	ligneDebut=1;
	colonneFin=512;
	ligneFin=512;
	break;
case 2:
	colonneDebut=1;
	ligneDebut=1;
	colonneFin=256;
	ligneFin=256;
	SetReadMode(1);
	break;
case 3:
	colonneDebut=1;
	ligneDebut=1;
	colonneFin=128;
	ligneFin=128;
	SetReadMode(1);
	break;
	}

AbortAcquisition();
erreurFct=SetImage(nbColonneParPixel,nbLigneParPixel,colonneDebut,colonneFin,ligneDebut,ligneFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetImage Error\n");
StartAcquisition();
}

void CameraAndor::DefBinning(int article)
{
chaineErreur="";
switch(article){
case 0:
	nbColonneParPixel=1;	
	nbLigneParPixel=1;
	break;
case 1:
	nbColonneParPixel=2;	
	nbLigneParPixel=2;
	break;
case 2:
	nbColonneParPixel=4;	
	nbLigneParPixel=4;
	break;
case 3:
	nbColonneParPixel=8;	
	nbLigneParPixel=8;
	break;
case 4:
	nbColonneParPixel=16;	
	nbLigneParPixel=16;
	break;
	}
AbortAcquisition();
erreurFct=SetImage(nbColonneParPixel,nbLigneParPixel,colonneDebut,colonneFin,ligneDebut,ligneFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetImage Error\n");
StartAcquisition();
}

void CameraAndor::DefModeAcquisition(int article)
{
chaineErreur="";
switch(article){
case 0:
	modeAcquisition=1;	
	break;
case 1:
	modeAcquisition=2;	
	break;
case 2:
	modeAcquisition=3;	
	break;
case 3:
	modeAcquisition=4;	
	break;
	}

AbortAcquisition();
erreurFct=SetAcquisitionMode(modeAcquisition);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SetAcquisitionMode Error\n");
erreurFct=StartAcquisition();
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("SeStartAcquisition DefModeAcquisition Error\n");
}
void CameraAndor::DefModeDeclenchement(int article)
{
chaineErreur="";
}
void CameraAndor::DefZoneLecture(int article)
{
chaineErreur="";
}

char CameraAndor::ImagePrete()
{
if (TestDriver()==0)
	return 1;
chaineErreur="";
int status;
erreurFct=GetStatus(&status);
if(erreurFct!=DRV_SUCCESS)
	{
	AjouteMsgErreur("GetStatus Error\n");
	return 0;
	}
if (status==DRV_ACQUIRING)
	return 1;
return 0;
}

char CameraAndor::Image(unsigned short *data,unsigned long &taille)
{

chaineErreur="";
if (TestDriver()==0)
	{
	ifstream g("imagepardefaut.txt");
	char	nomFichier[1024];
	static ImageInfoCV *imTest= NULL;
	if (!imTest)
		if (g.is_open())
			{
			g.getline(nomFichier,1024);
			imTest = new ImageInfoCV(nomFichier);
			}
		else
			imTest = new ImageInfoCV("C:\\Images\\seec images\\20091102_Treated\\EPC_P89_START_j7h0m29_END_j7h16m17_T45\\EgPC_P89_Day9A45_09.tif");
	unsigned short *dataTest=(unsigned short *)imTest->data;
	for (int i=0;i<(ligneFin-ligneDebut+1);i++)
		for (int j=0;j<colonneFin-colonneDebut+1;j++,data++,dataTest++)
			{
			*data =*dataTest;
			}
	return 1;
	}
long indIni,indFin;
erreurFct = GetNumberNewImages(&indIni,&indFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("GetNumberNewImages Error\n");
TailleImage(taille);
erreurFct = GetMostRecentImage16(data,taille);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("GetImages Error\n");
taille=0;return 1;
}


char CameraAndor::TailleImage(unsigned long &taille)
{
if (TestDriver()==0)
	{
	taille = 1004*1002;
	ligneFin =1001;
	ligneDebut=0;
	colonneFin=1003;
	colonneDebut=0;
	return 1;
	}
chaineErreur="";
taille=(ligneFin-ligneDebut+1)*(colonneFin-colonneDebut+1)/nbColonneParPixel/nbLigneParPixel;
return 1;
};

void CameraAndor::DefCoinGauche(int x)
{
colonneDebut=x;
AbortAcquisition();
erreurFct = SetImage(nbColonneParPixel,nbColonneParPixel,colonneDebut,colonneFin,ligneDebut,ligneFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("DefCoinGauche SetImages Error\n");
StartAcquisition();
return;
}

void CameraAndor::DefCoinDroit(int x)
{
colonneFin=x;
AbortAcquisition();
erreurFct = SetImage(nbColonneParPixel,nbColonneParPixel,colonneDebut,colonneFin,ligneDebut,ligneFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("DefCoinGauche SetImages Error\n");
StartAcquisition();
return;
}

void CameraAndor::DefCoinHaut(int x)
{
ligneDebut = x;
AbortAcquisition();
erreurFct = SetImage(nbColonneParPixel,nbColonneParPixel,colonneDebut,colonneFin,ligneDebut,ligneFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("DefCoinGauche SetImages Error\n");
StartAcquisition();
return;
}

void CameraAndor::DefCoinBas(int x)
{
ligneFin=x;
AbortAcquisition();
erreurFct = SetImage(nbColonneParPixel,nbColonneParPixel,colonneDebut,colonneFin,ligneDebut,ligneFin);
if(erreurFct!=DRV_SUCCESS)
	AjouteMsgErreur("DefCoinGauche SetImages Error\n");
StartAcquisition();
return;
}

char CameraAndor::EMGain8Bit()
{
if (!testDriver)
	return 0;
return caps.ulEMGainCapability &0x1;
}

char CameraAndor::EMGain12Bit()
{
if (!testDriver)
	return 0;
return caps.ulEMGainCapability &0x2;
}

char CameraAndor::EMGainLinear12Bit()
{
if (!testDriver)
	return 0;
return caps.ulEMGainCapability &0x4;
}

char CameraAndor::EMGainReal12Bit()
{
if (!testDriver)
	return 0;
return caps.ulEMGainCapability &0x8;
}

	
char CameraAndor::GetFunctionEMCCDGain()
{
if (!testDriver)
	return 0;
return caps.ulGetFunctions &0x20;
}

#endif
