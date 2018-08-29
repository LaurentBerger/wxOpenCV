#ifndef __SEQUENCEOPERATION__
#define __SEQUENCEOPERATION__
//#include <wx/string.h>
#include <map>
#include <string>
#include <iostream>
#include "opencv2/opencv.hpp"
#include "ParametreOperation.h"


class NoeudOperation {
private:
    ImageInfoCV *imAcq;     // non nul si noeud = fenêtre
    ParametreOperation pOCV;   // non nul si noeud = opération
    int indOnglet;
    int indFen;
    int indOp;
    int typeNoeud;
    std::vector<std::shared_ptr<NoeudOperation>> children;
    std::vector<std::shared_ptr<NoeudOperation>> parent;
public:
    NoeudOperation(ParametreOperation *p, int n) : imAcq(NULL), pOCV(*p), indFen(-1), indOnglet(n), typeNoeud(0) { };
    NoeudOperation(ImageInfoCV *f) : imAcq(f), pOCV(ParametreOperation()), indFen(-1), indOnglet(-1), typeNoeud(0) { };
    NoeudOperation(int ind) : imAcq(NULL), pOCV(ParametreOperation()), indFen(-1), indOnglet(-1), typeNoeud(0), indOp(ind) { };

    ParametreOperation *Operation() { return &pOCV; };
    int IndiceOnglet() { return indOnglet; };
    int IndiceFenetre() { return indFen; };
    int IndiceOperande() { return indOp; };
    int TypeNoeud() { return typeNoeud; };
    ImageInfoCV *ImAcq() { return imAcq; };
    void DefImage(ImageInfoCV *fz) { imAcq = fz; pOCV = ParametreOperation(); };
    void DefIndFenetre(int ind) { indFen = ind; };
    void DefIndOperande(int ind) { indOp = ind; };
    void DefTypeNoeud(int s) { typeNoeud = s; };
    void DefIndiceOnglet(int s) { indOnglet = s; };
    void AddChildren(std::shared_ptr<NoeudOperation> n) { children.push_back(n); };
    void AddParent(std::shared_ptr<NoeudOperation> n) { parent.push_back(n); };
    int GetNbParent() { return parent.size(); };
    std::shared_ptr<NoeudOperation> GetParent(int i) { if (i < parent.size()) return parent[i]; return NULL; };

    enum Type_Noeud
    {
        NOEUD_FENETRE = 1, NOEUD_OPERATION = 2,
        NOEUD_RESULTAT = 4, NOEUD_ATTRIBUT = 8,
        NOEUD_AVEC_ATTRIBUT = 16
    };

    enum
    {
        NON_INIT = 0, FENETRE_RESULTAT, FENETRE_DONNEE,
        INDICE_FENETRE_VALIDE, INDICE_FENETRE_LOGIQUE
    };

};


class SequenceOperation {
    void                    *osgApp;
    bool                    majFenetre;
    std::vector<ParametreOperation> listeOp;
    int indRacine;
    std::shared_ptr<NoeudOperation> racine;
    std::vector< std::shared_ptr<NoeudOperation> > lNoeud;
    int nbOnglet;
public:

    SequenceOperation(bool maj) : osgApp(NULL), majFenetre(maj), indRacine(-1), racine(NULL),nbOnglet(0){};
    ~SequenceOperation() {};
    bool AjouterOperation(ParametreOperation p);
    void SauverSequence(std::string fileName);
    void LireSequence(std::string fileName);
    bool AjouterNoeud(std::shared_ptr<NoeudOperation> n);
    void CreerArbre();
    ParametreOperation LireOperation(int ind) { if (ind >= 0 && ind < listeOp.size()) return listeOp[ind]; else return ParametreOperation(); };
    bool DefOperation(int ind, ParametreOperation p) {
        if (ind < 0 || ind >= listeOp.size()) return false; listeOp[ind] = p; return true;
    };
    void DefOSGApp(void *w) { osgApp = w; };
    void MajFenetre(bool maj) { majFenetre = maj; };
    bool MajFenetre() { return majFenetre; };
    int Etape(ParametreOperation &);
    bool ExecuterSequence();

};
#endif

