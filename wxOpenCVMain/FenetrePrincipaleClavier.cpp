#include "FenetrePrincipale.h"
#include "imagestat.h"
//#include "outilsImage.h"

void FenetrePrincipale::TraitementClavier(wxKeyEvent &event)
{
   int key = event.GetKeyCode();

if (key==WXK_F1 && event.ShiftDown())
	{
	modeImage=0;
	MAJNouvelleImage();
	}
if (key==WXK_F2 && event.ShiftDown())
	{
		{
		modeImage=1;
		MAJNouvelleImage();
		}
	}
if (key==WXK_F3 && event.ShiftDown())
	{
		{
		modeImage=2;
		MAJNouvelleImage();
		}
	}
if (key==WXK_F4 && event.ShiftDown())
	{
		{
		modeImage=31;
		MAJNouvelleImage();
		}
	}
if (key==WXK_F5 && event.ShiftDown())
	{
		modeImage=4;
		MAJNouvelleImage();
	}

if (key=='F'&& !event.ControlDown())
	{
//	osgApp->Outils()->FusionRegion(wxCommandEvent(wxEVT_NULL));
	}
if (key=='R'&& !event.ShiftDown())
	{
	feuille->ModeRectangle(false);
	wxClientDC hdc(feuille);
	DrawWindow(hdc);
	}
if (key=='R' && event.ShiftDown() )
	{
	feuille->ModeRectangle(true);
	feuille->ModeCoupe(false);
	wxClientDC hdc(feuille);
	DrawWindow(hdc);
	}
if (feuille->ModeRectangle() && key==WXK_DELETE)
	feuille->Rectangle(feuille->IndiceRectangleSelec())->SetSize(wxSize(0,0));
if (key=='C'&& !event.ShiftDown())
	{
	feuille->ModeCoupe(false);
	wxClientDC hdc(feuille);
	DrawWindow(hdc);
	}
if (key=='C' && event.ShiftDown() )
	{
	feuille->ModeRectangle(false);
	feuille->ModeCoupe(true);
	wxClientDC hdc(feuille);
	DrawWindow(hdc);
	}
if (feuille->ModeCoupe() && key==WXK_DELETE)
	feuille->Coupe(feuille->IndiceCoupeSelec())->SetSize(wxSize(0,0));
int ind=-1;
if (key==WXK_F1)
	ind=0;
if (key==WXK_F2)
	ind=1;
if (key==WXK_F3)
	ind=2;
if (key==WXK_F4)
	ind=3;
if (key==WXK_F5)
	ind=4;
if (key==WXK_F6)
	ind=5;
if (key==WXK_F7)
	ind=6;
if (key==WXK_F8)
	ind=7;
if (key==WXK_F9)
	ind=8;
if (key==WXK_F10)
	ind=9;
if (feuille->ModeCoupe() && ind>=0)
	{
	int indPre=feuille->IndiceCoupeSelec();
	feuille->IndiceCoupeSelec(ind);
	wxRect r(*feuille->CoupeSelec());
	osgApp->ImgStat()->MAJCoupe(r);	
	wxRect	rTmp = *feuille->CoupeSelec();
	wxRect rTmp2 = feuille->RepereImageEcran(rTmp);
	RedresseRectangle(rTmp2);
	feuille->RefreshRect(rTmp2, false);
	rTmp = *feuille->Coupe(indPre);
	rTmp2 = feuille->RepereImageEcran(rTmp);
	RedresseRectangle(rTmp2);
	feuille->RefreshRect(rTmp2, false);
	//feuille->Refresh();
}
if (feuille->ModeRectangle() && ind>=0)
	{
	feuille->IndiceRectangleSelec(ind);
	osgApp->ImgStat()->Plot(true);
	feuille->Refresh(true);
	// propagate event
    }
}


void FenetrePrincipale::OnKeyDown(wxKeyEvent &event)
{
TraitementClavier(event);
event.Skip();
}


void FenetrePrincipale::OnKeyUp(wxKeyEvent &event)
{
#if wxUSE_UNICODE
    int key = event.GetUnicodeKey();
#else
    int key = event.GetKeyCode();
#endif

    // propagate event
    event.Skip();
}
