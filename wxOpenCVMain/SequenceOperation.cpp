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

void SequenceOperation::CreerArbre()
{
    lNoeud.clear();
    if (listeOp.size() == 0)
        return;
    shared_ptr<NoeudOperation> n = make_shared<NoeudOperation>(&listeOp[0], (NoeudOperation*)NULL, -1);
    lNoeud.push_back(n);
    vector<int> listeUtil(listeOp.size(), 0);
    listeUtil[0] = 1;
    racine = n;
    for (int i = 1; i < listeOp.size(); i++)
    {

    }
}
