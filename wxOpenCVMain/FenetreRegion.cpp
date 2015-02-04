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


FenetreRegion::FenetreRegion( wxFrame* frame) : wxWindow(frame,-1,wxPoint(0,0),wxSize(800,400))
{
gestionControlDown=0;
osgApp=NULL;
fenParent=NULL;
nbRegionMax=4000;
regionSelect=NULL;
cleTri =  NULL;
valTri = NULL;

listeRegion = new Tableur((wxFrame*)this,0,13); 
listeRegion->SetSize(wxSize(800,400));
//listeRegion->Refresh();
listeRegion->DefTitreColonne(0, _("Index"));
listeRegion->DefTitreColonne(1, _("Gray Level"));
listeRegion->DefTitreColonne(2, _("Gray Level\n(wo gradient)"));
listeRegion->DefTitreColonne(3, _("Surface"));
listeRegion->DefTitreColonne(4, _("Surface\n(wo gradient) "));
listeRegion->DefTitreColonne(5, _("xg"));
listeRegion->DefTitreColonne(6, _("yg"));
listeRegion->DefTitreColonne(7, _("semi\nmajor\naxis"));
listeRegion->DefTitreColonne(8, _("semi\nminor\naxis"));
listeRegion->DefTitreColonne(9, _("theta"));
listeRegion->DefTitreColonne(10, _("Level\ndeviation"));
listeRegion->DefTitreColonne(11, _("Level\ndeviation\n(wo gradient)"));
listeRegion->DefTitreColonne(12, _("Height\nDifference"));
listeRegion->DefTitreColonne(13, _("Level\nIndex"));
listeRegion->SetColLabelSize(wxGRID_AUTOSIZE );
listeRegion->SetColSize(0, 40);
listeRegion->SetColSize(1, 50);
listeRegion->SetColSize(2, 50);
listeRegion->SetColSize(3, 50);
listeRegion->SetColSize(4, 50);
listeRegion->SetColSize(5, 50);
listeRegion->SetColSize(6, 50);
listeRegion->SetColSize(7, 50);
listeRegion->SetColSize(8, 50);
listeRegion->SetColSize(9, 50);
listeRegion->SetColSize(10, 50);
listeRegion->SetColSize(10, 40);
listeRegion->SetColSize(10, 40);
listeRegion->SetColSize(10, 30);
}




void FenetreRegion::ListerRegion() 
{
if (!osgApp)
	return;
if (!fenParent)
	return;
if (!((FenetrePrincipale*)fenParent)->ImAcq())
	return;
cv::Mat	**s=((FenetrePrincipale*)fenParent)->ImAcq()->StatComposante();
cv::Mat	**g=((FenetrePrincipale*)fenParent)->ImAcq()->CentreGComposante();
if (!s || !g)
	return;
int nb=listeRegion->GetNumberRows()-s[0]->rows;
if (nb>0) 
	{
	listeRegion->EffaceLigne(nb);
	
	}
else if (nb<0)
	listeRegion->AjouteLigne(-nb);

for (int ii=0;ii<s[0]->rows;ii++)
	{
	
	listeRegion->DefCellule(ii,0,ii, "%5d");
		listeRegion->DefCellule(ii,3,(s[0])->at<int>(ii,cv::CC_STAT_AREA), "%7d");
		listeRegion->DefCellule(ii,5,(g[0])->at<double>(ii,0), "%6.1f");
		listeRegion->DefCellule(ii,6,(g[0])->at<double>(ii,1), "%6.1f");
	}
	
/*static wxClipboard	*pressePapier=NULL;
if (!pressePapier)
	pressePapier =new wxClipboard;
pressePapier->SetData((wxTextDataObject *)listeRegion->GetTable());*/
}


void ImageStatistiques::ListerRegion() 
{

ongletRegion->ListerRegion();	
}


void ImageStatistiques::OnSelectCell(wxGridEvent &w)
{

}

void ImageStatistiques::OnRangeSelected(wxGridRangeSelectEvent &ev)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
if (!grapheRegion)
	return;
if (grapheRegion->nbSommets==0)
	return;
if ( ev.Selecting() )
    {
    for (int i=ev.GetTopRow();i<=ev.GetBottomRow() && i<grapheRegion->nbSommets;i++)
		ongletRegion->DefRegionSelect(i,1);
	((wxOsgApp*)osgApp)->Graphique()->TracerCercle(ongletRegion->RegionSelect());
	}	
else
    {
    for (int i=0;i<grapheRegion->nbSommets;i++)
		ongletRegion->DefRegionSelect(i,0);
	((wxOsgApp*)osgApp)->Graphique()->MAJNouvelleImage();

	}	
*/
}

void FenetreRegion::OnSelectCell(wxGridEvent &w)
{

}

void FenetreRegion::OnRangeSelected(wxGridRangeSelectEvent &ev)
{
if (!osgApp)
	return;
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
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
		((wxOsgApp*)osgApp)->Graphique()->MAJNouvelleImage();
		((wxOsgApp*)osgApp)->Graphique()->TracerCercle(regionSelect);
		}	
	else
		{
		for (int i=0;i<grapheRegion->nbSommets;i++)
			regionSelect[cleTri[i]]=0;
		((wxOsgApp*)osgApp)->Graphique()->MAJNouvelleImage();

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
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
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
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
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
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
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
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
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
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
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
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
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
	((wxOsgApp*)osgApp)->Graphique()->MAJNouvelleImage();
	((wxOsgApp*)osgApp)->Graphique()->TracerCercle(regionSelect);
	gestionControlDown=0;
	if (regionSelect[cleTri[w.GetRow()]]==1)
		w.Skip();
	}
else
	w.Skip();
*/
}		

		
