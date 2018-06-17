#include "FenetreRegion.h"
#include "imagestat.h"
#include <wx/clipbrd.h>

BEGIN_EVENT_TABLE( FenetreRegion, wxWindow )
    EVT_GRID_SELECT_CELL(FenetreRegion::OnSelectCell )
    EVT_GRID_RANGE_SELECT( FenetreRegion::OnRangeSelected )
    EVT_GRID_CELL_RIGHT_CLICK(FenetreRegion::MenuContextuel)
    EVT_GRID_LABEL_LEFT_CLICK(FenetreRegion::OterSelection)
	EVT_MENU(NEW_LEVEL,  FenetreRegion::DefinirNiveau)
	EVT_MENU(MERGE_LEVEL,  FenetreRegion::FusionNiveau)
	EVT_MENU(RESET_LEVEL,  FenetreRegion::RAZNiveau)
END_EVENT_TABLE()

FenetreRegion::FenetreRegion( wxFrame* frame,std::vector<cv::KeyPoint> *p,wxString &titre) : wxWindow(frame,-1,wxPoint(0,0),wxSize(800,400))
{
gestionControlDown=0;
osgApp=NULL;
keyPt=NULL;
fenParent=NULL;
nbRegionMax=4000;
regionSelect=NULL;
cleTri =  NULL;
valTri = NULL;
keyPt=NULL;
if (!p)
    return;
keyPt=p;
listeRegion = new Tableur((wxFrame*)this,keyPt->size(),7);
listeRegion->SetSize(wxSize(800,400));
//listeRegion->Refresh();
listeRegion->DefTitreColonne(0, _("x"));
listeRegion->DefTitreColonne(1, _("y"));
listeRegion->DefTitreColonne(2, _("diameter"));
listeRegion->DefTitreColonne(3, _("orientation"));
listeRegion->DefTitreColonne(4, _("response"));
listeRegion->DefTitreColonne(5, _("octave pyramid "));
listeRegion->DefTitreColonne(6, "class id");
listeRegion->SetColLabelSize(wxGRID_AUTOSIZE );
listeRegion->SetColSize(0, 70);
listeRegion->SetColSize(1, 70);
listeRegion->SetColSize(2, 70);
listeRegion->SetColSize(3, 70);
listeRegion->SetColSize(4, 70);
listeRegion->SetColSize(5, 70);
listeRegion->SetColSize(6, 70);
}
FenetreRegion::FenetreRegion( wxFrame* frame) : wxWindow(frame,-1,wxPoint(0,0),wxSize(800,400))
{
gestionControlDown=0;
osgApp=NULL;
fenParent=NULL;
nbRegionMax=4000;
keyPt=NULL;
regionSelect=NULL;
cleTri =  NULL;
valTri = NULL;

listeRegion = new Tableur((wxFrame*)this,0,20);
listeRegion->SetSize(wxSize(800,400));
//listeRegion->Refresh();
listeRegion->DefTitreColonne(0, _("Index"));
listeRegion->DefTitreColonne(1, _("Gray Level"));
listeRegion->DefTitreColonne(2, _("Gray Level\n(wo gradient)"));
listeRegion->DefTitreColonne(3, _("m00"));
listeRegion->DefTitreColonne(4, _("Surface\n(wo gradient) "));
listeRegion->DefTitreColonne(5, "xg");
listeRegion->DefTitreColonne(6, "yg");
listeRegion->DefTitreColonne(7, _("semi\nmajor\naxis"));
listeRegion->DefTitreColonne(8, _("semi\nminor\naxis"));
listeRegion->DefTitreColonne(9, _("theta"));
listeRegion->DefTitreColonne(10, _("mu30"));
listeRegion->DefTitreColonne(11, _("mu21"));
listeRegion->DefTitreColonne(12, _("mu12"));
listeRegion->DefTitreColonne(13, _("mu03"));
listeRegion->DefTitreColonne(14, _("hu0"));
listeRegion->DefTitreColonne(15, _("hu1"));
listeRegion->DefTitreColonne(16, _("hu2"));
listeRegion->DefTitreColonne(17, _("hu3"));
listeRegion->DefTitreColonne(18, _("hu4"));
listeRegion->DefTitreColonne(19, _("hu5"));
listeRegion->DefTitreColonne(20, _("hu6"));
listeRegion->SetColLabelSize(wxGRID_AUTOSIZE );
listeRegion->SetColSize(0, 40);
for (int i=1;i<21;i++)
    listeRegion->SetColSize(i, 50);
}



void FenetreRegion::ListerPointInteret(std::vector<cv::KeyPoint> *p)
{
if (!osgApp)
	return;
if (!fenParent)
	return;
if (!((FenetrePrincipale*)fenParent)->ImAcq())
	return;
if (p!=NULL)
    keyPt= p;
for (int ii=0;ii<keyPt->size();ii++)
	{

	listeRegion->DefCellule(ii,0,(*keyPt)[ii].pt.x, "%5.1f");
	listeRegion->DefCellule(ii,1,(*keyPt)[ii].pt.y, "%5.1f");
	listeRegion->DefCellule(ii,2,(*keyPt)[ii].size, "%5.1f");
	listeRegion->DefCellule(ii,3,(*keyPt)[ii].angle, "%5.1f");
	listeRegion->DefCellule(ii,4,(*keyPt)[ii].response, "%5.1f");
	listeRegion->DefCellule(ii,5,(*keyPt)[ii].octave, "%d");
	listeRegion->DefCellule(ii,6,(*keyPt)[ii].class_id, "%d");
	}

}

void FenetreRegion::ListerContour(std::vector<std::vector<cv::Point> >*p)
{
if (!osgApp)
	return;
if (!fenParent)
	return;
if (!((FenetrePrincipale*)fenParent)->ImAcq())
	return;
if (p!=NULL)
    contour= p;
for (int ii=0;ii<contour->size();ii++)
	{

	listeRegion->DefCellule(ii,0,static_cast<int>((*contour)[ii].size()), "%d");
	}

}

    int sign(double x){ if (x<0)return -1; else return 1; };



void FenetreRegion::ListerRegion(int indPlan)
{
if (!osgApp)
	return;
if (!fenParent)
	return;
if (!((FenetrePrincipale*)fenParent)->ImAcq())
	return;
std::vector<cv::Mat>	*s=((FenetrePrincipale*)fenParent)->ImAcq()->StatComposante();
std::vector<cv::Mat>	*g=((FenetrePrincipale*)fenParent)->ImAcq()->CentreGComposante();
std::vector<std::vector<cv::Moments> > *m=((FenetrePrincipale*)fenParent)->ImAcq()->MomentComposante();
std::vector<std::vector<std::vector<double> > > *hu=((FenetrePrincipale*)fenParent)->ImAcq()->HuMoment();
if (s->size()==0 || g->size()==0)
	return;
int nb=listeRegion->GetNumberRows()-(*s)[indPlan].rows;
if (nb>0)
	{
	listeRegion->EffaceLigne(nb);

	}
else if (nb<0)
	listeRegion->AjouteLigne(-nb);

for (int ii=0;ii<(*s)[indPlan].rows;ii++)
	{

	listeRegion->DefCellule(ii,0,ii, "%5d");
	listeRegion->DefCellule(ii,3,(*s)[indPlan].at<int>(ii,cv::CC_STAT_AREA), "%7d");
	listeRegion->DefCellule(ii,5,(*g)[indPlan].at<double>(ii,0), "%6.1f");
	listeRegion->DefCellule(ii,6,(*g)[indPlan].at<double>(ii,1), "%6.1f");
	if((*m)[indPlan].size()>ii && (*m)[indPlan][ii].m00>0)
		{
		listeRegion->DefCellule(ii,7,2*sqrt((*m)[indPlan][ii].mu20/(*m)[indPlan][ii].m00), "%6.1f");
		listeRegion->DefCellule(ii,8,2*sqrt((*m)[indPlan][ii].mu02/(*m)[indPlan][ii].m00), "%6.1f");
        double theta;
        if ((*m)[indPlan][ii].mu20 - (*m)[indPlan][ii].mu02 != 0)
        {
            theta=1/2.0*atan(2*(*m)[indPlan][ii].mu11/((*m)[indPlan][ii].mu20-(*m)[indPlan][ii].mu02));
            int s = (1 - sign((*m)[indPlan][ii].mu20 - (*m)[indPlan][ii].mu02))*sign((*m)[indPlan][ii].mu11);
			theta  += s*acos(-1.0)/4;
        }
        else if ((*m)[indPlan][ii].mu11>0)
            theta =acos(-1.0)/4;
        else
            theta =-acos(-1.0)/4;
		if (theta<0)
			theta += 2*acos(-1.0);
		listeRegion->DefCellule(ii,10,(*m)[indPlan][ii].mu30, "%6.1f");
		listeRegion->DefCellule(ii,11,(*m)[indPlan][ii].mu21, "%6.1f");
		listeRegion->DefCellule(ii,12,(*m)[indPlan][ii].mu12, "%6.1f");
		listeRegion->DefCellule(ii,13,(*m)[indPlan][ii].mu03, "%6.1f");
		listeRegion->DefCellule(ii,9,theta*180/acos(-1.0), "%6.2f");
		}
    if (hu->size() > indPlan )
    {
        listeRegion->DefCellule(ii,14,(*hu)[indPlan][ii][0], "%6f");
        listeRegion->DefCellule(ii,15,(*hu)[indPlan][ii][1], "%6f");
        listeRegion->DefCellule(ii,16,(*hu)[indPlan][ii][2], "%6f");
        listeRegion->DefCellule(ii,17,(*hu)[indPlan][ii][3], "%6f");
        listeRegion->DefCellule(ii,18,(*hu)[indPlan][ii][4], "%6f");
        listeRegion->DefCellule(ii,19,(*hu)[indPlan][ii][5], "%6f");
        listeRegion->DefCellule(ii,20,(*hu)[indPlan][ii][6], "%6f");

    }

}


//listeRegion->DefTitreColonne(1, _("Gray Level"));
//listeRegion->DefTitreColonne(2, _("Gray Level\n(wo gradient)"));
/*//listeRegion->DefTitreColonne(4, _("Surface\n(wo gradient) "));
listeRegion->DefTitreColonne(9, _("theta"));
listeRegion->DefTitreColonne(14, _("hu0"));
listeRegion->DefTitreColonne(15, _("hu1"));
listeRegion->DefTitreColonne(16, _("hu2"));
listeRegion->DefTitreColonne(17, _("hu3"));
listeRegion->DefTitreColonne(18, _("hu4"));
listeRegion->DefTitreColonne(19, _("hu5"));
listeRegion->DefTitreColonne(20, _("hu6"));




}

/*static wxClipboard	*pressePapier=NULL;
if (!pressePapier)
	pressePapier =new wxClipboard;
pressePapier->SetData((wxTextDataObject *)listeRegion->GetTable());*/
}


void ImageStatistiques::ListerRegion()
{


if (ongletRegionB && fenMere->ImAcq()->channels()>=1)
	{
	ongletRegionB->ListerRegion(0);
	}
if (ongletRegionV && fenMere->ImAcq()->channels()>=2)
	{
	ongletRegionV->ListerRegion(1);
	}
if (ongletRegionR && fenMere->ImAcq()->channels()>=3)
	{
	ongletRegionR->ListerRegion(2);
	}
}


void ImageStatistiques::OnSelectCell(wxGridEvent &w)
{

}

void ImageStatistiques::OnRangeSelected(wxGridRangeSelectEvent &ev)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
if ( ev.Selecting() )
    {
    for (int i=ev.GetTopRow();i<=ev.GetBottomRow() && i<grapheRegion->nbSommets;i++)
		ongletRegion->DefRegionSelect(i,1);
	((wxOpencvApp*)osgApp)->Graphique()->TracerCercle(ongletRegion->RegionSelect());
	}
else
    {
    for (int i=0;i<grapheRegion->nbSommets;i++)
		ongletRegion->DefRegionSelect(i,0);
	((wxOpencvApp*)osgApp)->Graphique()->MAJNouvelleImage();

	}
*/
}


void ImageStatistiques::SelectRegion(int w,int plan)
{
switch(plan){
case 0:
	if (ongletRegionB)
		ongletRegionB->SelectRegion(w);
	break;
case 1:
	if (ongletRegionV)
		ongletRegionV->SelectRegion(w);
	break;
case 2:
	if (ongletRegionR)
		ongletRegionR->SelectRegion(w);
	break;
	}
}

void FenetreRegion::OnSelectCell(wxGridEvent &w)
{

}
void FenetreRegion::SelectRegion(int w)
{
listeRegion->SelectRow(w);

}

void FenetreRegion::OnRangeSelected(wxGridRangeSelectEvent &ev)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
if (!gestionControlDown)
	{
	if ( ev.Selecting() )
		{
		for (int i=ev.GetTopRow();i<=ev.GetBottomRow() && i<grapheRegion->nbSommets;i++)
					regionSelect[cleTri[i]]=1;
		((wxOpencvApp*)osgApp)->Graphique()->MAJNouvelleImage();
		((wxOpencvApp*)osgApp)->Graphique()->TracerCercle(regionSelect);
		}
	else
		{
		for (int i=0;i<grapheRegion->nbSommets;i++)
			regionSelect[cleTri[i]]=0;
		((wxOpencvApp*)osgApp)->Graphique()->MAJNouvelleImage();

		}
	ev.Skip();
	}
*/
}


void FenetreRegion::ShowContextMenu(const wxPoint& pos)
{
wxMenu menu;

menu.Append(NEW_LEVEL, _T("New Level"));
menu.AppendCheckItem(MERGE_LEVEL, _T("Merge Level"));
menu.AppendCheckItem(RESET_LEVEL, _T("Reset Level"));


PopupMenu(&menu, pos.x, pos.y);

}

void FenetreRegion::MenuContextuel(wxGridEvent &w)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;

wxPoint point = w.GetPosition();

if (point.x == -1 && point.y == -1) {
    wxSize size = GetSize();
    point.x = size.x / 2;
    point.y = size.y / 2;
} else {
    //point = ScreenToClient(point);
}
ShowContextMenu(point);
*/


/*nbZoneId++;
for (int i=0;i<grapheRegion->nbSommets;i++)
	if(regionSelect[cleTri[i]])
		listeRegion->DefCellule(i,9,nbZoneId, "%5d");*/
}

void FenetreRegion::FusionNiveau(wxCommandEvent &w)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
int	minNiveau=grapheRegion->nbSommets;
char status;
for (int i=0;i<grapheRegion->nbSommets;i++)
	{
	if (regionSelect[cleTri[i]]==1)
		{
		int n=(int)listeRegion->LitCellule(i,10,status);
		if (n<minNiveau)
			minNiveau = n;
		}
	}
for (int i=0;i<grapheRegion->nbSommets;i++)
	{
	if (regionSelect[cleTri[i]]==1)
		listeRegion->DefCellule(i,10,minNiveau,"%4d");
	}
*/
}

void FenetreRegion::RAZNiveau(wxCommandEvent &w)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
for (int i=0;i<grapheRegion->nbSommets;i++)
	{
	if (regionSelect[cleTri[i]]==1)
		listeRegion->DefCellule(i,13,0,"%4d");
	}
*/
}

void FenetreRegion::DefinirNiveau(wxCommandEvent &w)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
int	minNiveau=0;
char status;
for (int i=0;i<grapheRegion->nbSommets;i++)
	{
	int n=(int)listeRegion->LitCellule(i,13,status);
	if (n>minNiveau)
		minNiveau = n;
	}
minNiveau = minNiveau+1;
for (int i=0;i<grapheRegion->nbSommets;i++)
	{
	if (regionSelect[cleTri[i]]==1)
		listeRegion->DefCellule(i,13,minNiveau,"%4d");
	}
*/
}


void FenetreRegion::LireNiveau(int *classe)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
char	status;
for (int i=0;i<grapheRegion->nbSommets;i++)
	{
	int n=(int)listeRegion->LitCellule(i,13,status);
	if (status)
		classe[cleTri[i]]=n;
	else
		classe[cleTri[i]]=0;
	}
*/
}


void FenetreRegion::OterSelection(wxGridEvent &w)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOpencvApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
if (w.ControlDown())
	{
	gestionControlDown=1;

	regionSelect[cleTri[w.GetRow()]]=1-regionSelect[cleTri[w.GetRow()]];
	if (regionSelect[cleTri[w.GetRow()]]==0)
		{
		listeRegion->ClearSelection();

		for (int i=0;i<grapheRegion->nbSommets;i++)
			if (regionSelect[cleTri[i]])
				listeRegion->SelectRow(i,true);
		Refresh();
		}
	((wxOpencvApp*)osgApp)->Graphique()->MAJNouvelleImage();
	((wxOpencvApp*)osgApp)->Graphique()->TracerCercle(regionSelect);
	gestionControlDown=0;
	if (regionSelect[cleTri[w.GetRow()]]==1)
		w.Skip();
	}
else
	w.Skip();
*/
}


