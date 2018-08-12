#include "SequenceOperation.h"
#include <memory>


using namespace std;

bool SequenceOperation::ExecuterSequence()
{
    return false;
}

void SequenceOperation::AjouterOperation(ParametreOperation p)
{
    listeOp.push_back(p); 
    if (indRacine == -1)
    {
        indRacine = 0;
        return;
    }
}

void SequenceOperation::SauverSequence(string fileName)
{
    cv::FileStorage fs;
    fs.open(fileName.c_str(), cv::FileStorage::WRITE);
    if (fs.isOpened())
        for (auto l : listeOp)
        {
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

bool findIndOperande(shared_ptr<NoeudOperation> n)
{
    return n.get()->IndiceOperande() == 28;
}

void SequenceOperation::AjouterNoeud(shared_ptr<NoeudOperation> n)
{
    if (!n.get())
        return;
    if (n.get()->Operation())
    {
        ParametreOperation *p = n.get()->Operation();
        if (p->indRes != -1)
        {
            shared_ptr<NoeudOperation> nim;
            std::vector<shared_ptr<NoeudOperation>>::iterator it = find_if(lNoeud.begin(), lNoeud.end(), findIndOperande);
            if (it == lNoeud.end())
            {
                nim = make_shared<NoeudOperation>(p->indRes);
                n.get()->AddParent(nim);
                lNoeud.push_back(nim);
            }
            else
                n.get()->AddParent(*it);
        }
        for (int i = 0; i < p->indOpFenetre.size(); i++)
        {
            shared_ptr<NoeudOperation> nim;
            if (i < p->op.size())
            {
                nim = make_shared<NoeudOperation>(p->op[i]);
                nim.get()->AddParent(n);
                nim.get()->DefIndOperande(p->indOpFenetre[i]);
            }
            else
            {
                nim = make_shared<NoeudOperation>(p->indOpFenetre[i]);
                nim.get()->DefIndOperande(p->indOpFenetre[i]);
                nim.get()->AddParent(n);

            }
            if (nim->GetNbParent())
                n.get()->AddChildren(nim);
            lNoeud.push_back(nim);
        }
    }
}


void SequenceOperation::CreerArbre()
{
    lNoeud.clear();
    if (listeOp.size() == 0)
        return;
    shared_ptr<NoeudOperation> n = make_shared<NoeudOperation>(&listeOp[0], -1);
    AjouterNoeud(n);
    lNoeud.push_back(n);
    vector<int> listeUtil(listeOp.size(), 0);
    listeUtil[0] = 1;
    racine = n;
    for (int i = 1; i < listeOp.size(); i++)
    {
        shared_ptr<NoeudOperation> n = make_shared<NoeudOperation>(&listeOp[i], -1);
        AjouterNoeud(n);
        lNoeud.push_back(n);

    }
    listeUtil[0] = 0;
}
