#ifndef __SEQUENCEOPERATION__
#define __SEQUENCEOPERATION__
//#include <wx/string.h>
#include <map>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "ParametreOperation.h"


class NoeudOperation {
#define NOEUD_FENETRE 1
#define NOEUD_OPERATION 2
#define NOEUD_RESULTAT 4
#define NOEUD_ATTRIBUT 8
#define NOEUD_AVEC_ATTRIBUT 16

    enum
    {
        NON_INIT = 0, FENETRE_RESULTAT, FENETRE_DONNEE,
        INDICE_FENETRE_VALIDE, INDICE_FENETRE_LOGIQUE
    };
private:
    ImageInfoCV *imAcq;     // non nul si noeud = fenêtre
    ParametreOperation *pOCV;   // non nul si noeud = opération
    int indOnglet;
    int indFen;
    int typeNoeud;
    NoeudOperation *parent;     // Noeud de niveau supérieur = résultat NULL si racine
public:
    NoeudOperation(ParametreOperation *p, NoeudOperation *pUp, int n) : imAcq(NULL), pOCV(p), parent(pUp), indFen(-1), indOnglet(n), typeNoeud(0) { }
    NoeudOperation(ImageInfoCV *f, NoeudOperation *pUp) : imAcq(f), pOCV(NULL),parent(pUp), indFen(-1),  indOnglet(-1), typeNoeud(0) { }

    ParametreOperation *Operation() { return pOCV; };
    int IndiceOnglet() { return indOnglet; };
    int IndiceFenetre() { return indFen; };
    ImageInfoCV *ImAcq() { return imAcq; };
    void DefFenetre(ImageInfoCV *fz) { imAcq = fz; pOCV = NULL; };
    void DefIndFenetre(int ind) { indFen = ind; };
    void DefTypeNoeud(int s) { typeNoeud = s; };
    int TypeNoeud() { return typeNoeud; };
    NoeudOperation* GetParent() { return parent; };
};


class SequenceOperation {
    void                    *osgApp;
    bool                    majFenetre;
    std::vector<ParametreOperation> listeOp;
    int indRacine;
    std::shared_ptr<NoeudOperation> racine;
    std::vector< std::shared_ptr<NoeudOperation> > lNoeud;
public:

    SequenceOperation(bool maj) : osgApp(NULL), majFenetre(maj), indRacine(-1), racine(NULL){};
    ~SequenceOperation() {};
    void AjouterOperation(ParametreOperation p);
    void SauverSequence(std::string fileName);
    void LireSequence(std::string fileName);
    void CreerArbre();
    ParametreOperation LireOperation(int ind) { if (ind >= 0 && ind < listeOp.size()) return listeOp[ind]; else return ParametreOperation(); };
    bool DefOperation(int ind, ParametreOperation p) {
        if (ind < 0 || ind >= listeOp.size()) return false; listeOp[ind] = p; return true;
    };
    void DefOSGApp(void *w) { osgApp = w; };
    void MajFenetre(bool maj) { majFenetre = maj; };
    bool MajFenetre() { return majFenetre; };
    bool ExecuterSequence();

};
#endif
