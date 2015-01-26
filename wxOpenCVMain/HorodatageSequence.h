#ifndef _HORODATAGESEQUENCE_H_
#define _HORODATAGESEQUENCE_H_

#include <string>

class HorodatageSequence {
int		nbImage;			/*< Nombre d'image dans la s�quence */ 
int		*horodatage;		/*< Horodatage en seconde des �l�ments de la s�quence */
char	**nomImage;			/*< Nom de la i �me image de la s�quence */
char	**commentaireFixe;	/*< Commentaire de la i �me image de la s�quence issu du fichier horodatage*/
char	**commentaireVar;	/*< Commentaire de la i �me image de la s�quence d�fini lors de la s�quence*/
int		indImageEnCours;
int		erreur;
std::string	nomSequence;		/*< Nom du fichier sequence */
std::string	nomFichierRapport;	/*< Nom du fichier rapport de la sequence */

public : 
HorodatageSequence(char	*);
int		NbElement(){return nbImage;};
int		IndiceImageEnCours(){return indImageEnCours;};
int		TpsElement(int i){if (i>=0 && i<nbImage) return horodatage[i];return -1;};
int		DureeSequence(){if (nbImage>0) horodatage[nbImage-1];return -1;};
char*	NomImage(){if (nbImage>0 && indImageEnCours<nbImage) return nomImage[indImageEnCours];return NULL;};
int		TpsProchaineImage();
int		DateImage();
void	Avancer(){if (nbImage>=0 && indImageEnCours<nbImage) indImageEnCours++;};
void	RAZ(){indImageEnCours=0;};
int		Erreur(){return erreur;};
char*	CommentaireFixe(){if (nbImage>=0 && indImageEnCours<nbImage) return commentaireFixe[indImageEnCours];return NULL;};
char*	CommentaireVar(){if (nbImage>=0 && indImageEnCours<nbImage) return commentaireVar[indImageEnCours];return NULL;};
std::string	NomFichierRapport(){return nomFichierRapport;};
};
#endif
