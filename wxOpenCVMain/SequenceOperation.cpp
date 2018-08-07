#include "ParametreOperation.h"

#include "ImageInfo.h"
#include <map>


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



