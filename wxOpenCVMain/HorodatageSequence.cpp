#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <wx/filename.h>

#include "HorodatageSequence.h"
using namespace std;


/** http://www.codeguru.com/forum/showthread.php?t=231054*/
template <class T>
bool from_string(T& t, 
                 const std::string& s, 
                 std::ios_base& (*f)(std::ios_base&))
{
  std::istringstream iss(s);
  return !(iss >> f >> t).fail();
}



HorodatageSequence::HorodatageSequence(char	*nomFichier)
{
indImageEnCours=0;
nbImage=0;
horodatage=NULL;
erreur=0;

nomSequence = nomFichier;
ifstream	f(nomFichier);


if (!f.is_open())
	return;
char	tmp[8192];
int		nbLigne=0;
int		maxCol3=0,maxCol4=0;
while (!f.eof() && erreur==0)
	{
	f.getline(tmp,8192);
	if (f.rdstate() && strlen(tmp)>0)
		{
		erreur =1;
		}
	else if (strlen(tmp)>0)
		{
		string	s(tmp);
		
		int w=s.find(",");
		
		if (w!=s.npos)
			{
			string reste(s.substr(w+1,s.length()-w-1));
			nbLigne++;
			w=reste.find(",");
			if (w!=reste.npos)
				{
				string reste2(reste.substr(w+1,reste.length()-w-1));
				w=reste2.find(",");
				if (w!=reste.npos)
					{
					reste=reste2.substr(w+1,reste2.length()-w-1);
					w=reste.find(",");
					
					if (w==reste.npos)
						{
						if (w>maxCol3)
							w=maxCol3;
						reste2 = reste.substr(w+1,reste2.length()-w-1);
						if (maxCol4<reste2.length())
							maxCol4=reste2.length();
						}
					else
						erreur=1;
					}
				
				}
			else
				erreur++;
			}
		else
			erreur++;
		}
	}
if (erreur)
	{
	erreur = nbLigne;
	return;
	}
nbImage=nbLigne;
nbLigne =0;
horodatage = new int[nbImage];
nomImage = new char*[nbImage];
commentaireFixe = new char*[nbImage];
commentaireVar = new char*[nbImage];
f.clear();
int pos=f.tellg();
f.seekg(0, ios_base::beg );
pos=f.tellg();
while (!f.eof())
	{
	f.getline(tmp,8192);
	if (strlen(tmp)>0)
		{
		string	s(tmp);
		
		int w=s.find(",");
		string nom(s.substr(1,w-2));
		string reste1(s.substr(w+1,s.length()-w-1)),reste2,reste3;
		string date,com1,com2;
		w=reste1.find(",");
		if (w!=reste1.npos)
			{
			date=reste1.substr(0,w);
			reste2 =reste1.substr(w+1,reste1.length()-w-1);
			w=reste2.find(",");
			if (w!=reste2.npos)
				{
				if (w>1)
					{
					com1=reste2.substr(0,w);
					if (w>2)
						com2=reste2.substr(w+1,reste2.length()-w-1);
					}
				}
			else
				com1=reste2;	
			nomImage[nbLigne]=new char[nom.length()+5];
			strcpy(nomImage[nbLigne],nom.c_str());
			if (nom.find(".tif")==s.npos)
				strcat(nomImage[nbLigne],".tif");
			horodatage[nbLigne]=0;
			string sHeure="",sMinute="",sSeconde="";
			w=date.find(":");
			if (w!=date.npos)
				{
				sHeure = date.substr(0,w);
				date = date.substr(w+1,date.length()-w-1);
				w=date.find(":");
				if (w!=date.npos)
					{
					sMinute = date.substr(0,w);
					sSeconde = date.substr(w+1,date.length()-w-1);
					}
				
				}
			int heure,minute,seconde;
			from_string<int>(heure, sHeure,std::dec);		
			from_string<int>(minute, sMinute,std::dec);		
			from_string<int>(seconde, sSeconde,std::dec);		
			horodatage[nbLigne]=(heure*60+minute)*60+seconde;
			if (nbLigne>0 && horodatage[nbLigne]<=horodatage[nbLigne-1])
				{
				erreur = nbLigne+1;
				return;
				}
			if (com1.length()>0)
				{
				commentaireFixe[nbLigne]=new char[com1.length()];
				strcpy(commentaireFixe[nbLigne],com1.c_str());
				}
			else
				commentaireFixe[nbLigne]=0;
			if (com2.length()>0)
				{
				commentaireVar[nbLigne]=new char[com2.length()];
				strcpy(commentaireVar[nbLigne],com2.c_str());
				}
			else
				commentaireVar[nbLigne]=0;
			nbLigne++;
			}
		}
	}
indImageEnCours=0;

wxFileName	nomImage(NomImage());
wxString	nomVolume,nomRep,nomFic,extension;
wxString	nom;


nomImage.SplitPath(NomImage(),&nomVolume,&nomRep,&nomFic,&extension);
nom=nomVolume+nomImage.GetVolumeSeparator()+nomRep+nomImage.GetPathSeparator();
nomImage.SplitPath(nomSequence,&nomVolume,&nomRep,&nomFic,&extension);
nom = nom + nomFic+_T(".txt");
nomFichierRapport = nom.c_str();
}

int		HorodatageSequence::TpsProchaineImage()
{
if (nbImage>0 && indImageEnCours<nbImage)
	if ( indImageEnCours>0) 
		return horodatage[indImageEnCours]-horodatage[indImageEnCours-1]; 
	else
		return horodatage[indImageEnCours]; 
return -1;
}

int		HorodatageSequence::DateImage()
{
if (nbImage>0 && indImageEnCours<nbImage)
	return horodatage[indImageEnCours]; 
return -1;
}

