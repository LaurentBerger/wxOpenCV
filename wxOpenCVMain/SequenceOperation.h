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
    int indOp;
    int typeNoeud;
    std::vector<std::shared_ptr<NoeudOperation>> children;
    std::vector<std::shared_ptr<NoeudOperation>> parent;
public:
    NoeudOperation(ParametreOperation *p, int n) : imAcq(NULL), pOCV(p), indFen(-1), indOnglet(n), typeNoeud(0) { };
    NoeudOperation(ImageInfoCV *f) : imAcq(f), pOCV(NULL), indFen(-1), indOnglet(-1), typeNoeud(0) { };
    NoeudOperation(int ind) : imAcq(NULL), pOCV(NULL), indFen(-1), indOnglet(-1), typeNoeud(0), indOp(ind) { };

    ParametreOperation *Operation() { return pOCV; };
    int IndiceOnglet() { return indOnglet; };
    int IndiceFenetre() { return indFen; };
    int IndiceOperande() { return indOp; };
    ImageInfoCV *ImAcq() { return imAcq; };
    void DefImage(ImageInfoCV *fz) { imAcq = fz; pOCV = NULL; };
    void DefIndFenetre(int ind) { indFen = ind; };
    void DefIndOperande(int ind) { indOp = ind; };
    void DefTypeNoeud(int s) { typeNoeud = s; };
    int TypeNoeud() { return typeNoeud; };
    void AddChildren(std::shared_ptr<NoeudOperation> n) { children.push_back(n); };
    void AddParent(std::shared_ptr<NoeudOperation> n) { parent.push_back(n); };
    int GetNbParent() { return parent.size(); };
    std::shared_ptr<NoeudOperation> GetParent(int i) { if (i < parent.size()) return parent[i]; return NULL; };
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
    void AjouterNoeud(std::shared_ptr<NoeudOperation> n);
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

