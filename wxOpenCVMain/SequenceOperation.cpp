#include "SequenceOperation.h"
#include <memory>


using namespace std;

bool SequenceOperation::ExecuterSequence()
{
    return false;
}

bool SequenceOperation::AjouterOperation(ParametreOperation p)
{
    listeOp.push_back(p); 
    shared_ptr<NoeudOperation> n = make_shared<NoeudOperation>(&listeOp.back(), -1);
    bool b = AjouterNoeud(n);
    if (!b)
        listeOp.pop_back();
    if (indRacine == -1)
    {
        indRacine = 0;
        return b;
    }
    return b;
}

void SequenceOperation::SauverSequence(string fileName)
{
    cv::FileStorage fs;
    fs.open(fileName.c_str(), cv::FileStorage::WRITE);
    if (fs.isOpened())
        for (auto l : listeOp)
        {
            l.indEtape = Etape(l);
            l.write(fs);
        }
    fs.release();
}

void SequenceOperation::LireSequence(string fileName)
{
    cv::FileStorage fs;
    fs.open(fileName.c_str(), cv::FileStorage::READ);
    if (fs.isOpened())
        for (auto l : listeOp)
        {
            l.write(fs);
        }
    fs.release();
}

struct FindIndRes {
    explicit FindIndRes(int i): n(i) { }
    inline bool operator()(const shared_ptr<NoeudOperation> & m) const 
    { if (m.get()->Operation()) 
        return m.get()->Operation()->indRes == n;
      return false;
    }
private:
    int n;
}; 

struct FindIndOpe {
    explicit FindIndOpe(int i) : n(i) { }
    inline bool operator()(const shared_ptr<NoeudOperation> & m) const
    {
        if (!m.get()->Operation())
            return m.get()->IndiceOperande() == n;
        return false;
    }
private:
    int n;
};
struct FindOpe {
    explicit FindOpe(ImageInfoCV *i) : n(i) { }
    inline bool operator()(const shared_ptr<NoeudOperation> & m) const
    {
        if (!m.get()->Operation())
            return m.get()->ImAcq() == n;
        return false;
    }
private:
    ImageInfoCV *n;
};

struct FindOperation {
    explicit FindOperation(ParametreOperation p) : n(p) { }
    inline bool operator()(const shared_ptr<NoeudOperation> & m) const
    {
        if (m.get()->Operation())
        {
            ParametreOperation  *p=m.get()->Operation();
            if (p && p->indRes == n.indRes)
            {
                if (p->indOpFenetre.size() != n.indOpFenetre.size())
                    return false;
                for (int i = 0; i < p->indOpFenetre.size(); i++)
                    if (p->indOpFenetre[i] != n.indOpFenetre[i])
                        return false;
                return true;
            }
            else
                return false;

        }
        return false;
    }
private:
    ParametreOperation n;
};

bool SequenceOperation::AjouterNoeud(shared_ptr<NoeudOperation> n)
{
    if (!n.get())
        return false;
    if (n.get()->Operation())
    {
        ParametreOperation *p = n.get()->Operation();
        if (p->indRes != -1)
        {
            shared_ptr<NoeudOperation> nim;
            std::vector<shared_ptr<NoeudOperation>>::iterator it = find_if(lNoeud.begin(), lNoeud.end(), FindIndOpe(p->indRes));
            if (it == lNoeud.end())
            {
                n.get()->DefIndiceOnglet(nbOnglet++);
                lNoeud.push_back(n);
                nim = make_shared<NoeudOperation>(p->indRes);
                n.get()->AddParent(nim);
                nim.get()->AddChildren(n);
                lNoeud.push_back(nim);
                for (int i = 0; i < p->indOpFenetre.size(); i++)
                {
                    shared_ptr<NoeudOperation> nim;
                    std::vector<shared_ptr<NoeudOperation>>::iterator it;
                    if (i < p->op.size() && p->op[i])
                    {
                        it = find_if(lNoeud.begin(), lNoeud.end(), FindOpe(p->op[i]));
                        if (it == lNoeud.end())
                        {
                            nim = make_shared<NoeudOperation>(p->op[i]);
                            nim.get()->AddParent(n);
                            nim.get()->DefIndOperande(p->indOpFenetre[i]);
                        }
                        else
                        {
                            (*it)->AddParent(n);
                            n.get()->AddChildren(*it);
                        }
                    }
                    else
                    {
                        it = find_if(lNoeud.begin(), lNoeud.end(), FindIndOpe(p->indOpFenetre[i]));
                        if (it == lNoeud.end())
                        {
                            nim = make_shared<NoeudOperation>(p->indOpFenetre[i]);
                            nim.get()->DefIndOperande(p->indOpFenetre[i]);
                            nim.get()->AddParent(n);
                        }
                        else
                        {
                            (*it)->AddParent(n);
                            n.get()->AddChildren(*it);
                        }
                    }
                    if (nim && nim->GetNbParent())
                        n.get()->AddChildren(nim);
                    if (nim && nim.get())
                        lNoeud.push_back(nim);
                }
            }
            else
                return false;
        }
        else  // Attribut
        {
            std::vector<shared_ptr<NoeudOperation>>::iterator it = find_if(lNoeud.begin(), lNoeud.end(), FindOperation(*p));
            if (it != lNoeud.end())
                return false;
            n.get()->DefTypeNoeud(NoeudOperation::Type_Noeud::NOEUD_ATTRIBUT);
            n.get()->DefIndiceOnglet(nbOnglet++);
            lNoeud.push_back(n);
            for (int i = 0; i < p->indOpFenetre.size(); i++)
            {
                shared_ptr<NoeudOperation> nim;
                std::vector<shared_ptr<NoeudOperation>>::iterator it;
                if (i < p->op.size() && p->op[i])
                {
                    it = find_if(lNoeud.begin(), lNoeud.end(), FindOpe(p->op[i]));
                    if (it == lNoeud.end())
                    {
                        nim = make_shared<NoeudOperation>(p->op[i]);
                        nim.get()->AddParent(n);
                        nim.get()->DefIndOperande(p->indOpFenetre[i]);
                    }
                    else
                    {
                        (*it)->AddParent(n);
                        n.get()->AddChildren(*it);
                    }
                }
                else
                {
                    it = find_if(lNoeud.begin(), lNoeud.end(), FindIndOpe(p->indOpFenetre[i]));
                    if (it == lNoeud.end())
                    {
                        nim = make_shared<NoeudOperation>(p->indOpFenetre[i]);
                        nim.get()->DefIndOperande(p->indOpFenetre[i]);
                        nim.get()->AddParent(n);
                    }
                    else
                    {
                        (*it)->AddParent(n);
                        n.get()->AddChildren(*it);
                    }
                }
                if (nim)
                {
                    n.get()->AddParent(nim);
                    nim.get()->AddChildren(n);
                }
                if (nim && nim.get())
                    lNoeud.push_back(nim);
            }
        }
    }
    return true;
}


void SequenceOperation::CreerArbre()
{
    lNoeud.clear();
    if (listeOp.size() == 0)
        return;
    shared_ptr<NoeudOperation> n = make_shared<NoeudOperation>(&listeOp[0], -1);
    AjouterNoeud(n);
    vector<int> listeUtil(listeOp.size(), 0);
    listeUtil[0] = 1;
    racine = n;
    for (int i = 1; i < listeOp.size(); i++)
    {
        shared_ptr<NoeudOperation> n = make_shared<NoeudOperation>(&listeOp[i], -1);
        AjouterNoeud(n);
    }
    listeUtil[0] = 0;
}

int SequenceOperation::Etape(ParametreOperation &p)
{
    std::vector<shared_ptr<NoeudOperation>>::iterator it;
    it = find_if(lNoeud.begin(), lNoeud.end(), FindOperation(p));
    if (it==lNoeud.end())
        return -1;
    return (*it)->IndiceOnglet();
}

