#ifndef __BARREECHELLE__
#define __BARREECHELLE__

#include <osgSim/ScalarBar>
#include <osgSim/ScalarsToColors>
#include <iostream>

using osgSim::ScalarBar;

// Create a custom scalar printer
struct EchelleHauteur: public ScalarBar::ScalarPrinter
{
    std::string printScalar(float scalar)
    {
        std::cout<<"In MyScalarPrinter::printScalar"<<std::endl;
        if(scalar==0.0f) return ScalarBar::ScalarPrinter::printScalar(scalar)+" Bottom";
        else if(scalar==0.5f) return ScalarBar::ScalarPrinter::printScalar(scalar)+" Middle";
        else if(scalar==1.0f) return ScalarBar::ScalarPrinter::printScalar(scalar)+" Top";
        else return ScalarBar::ScalarPrinter::printScalar(scalar);
    }
};

#endif

