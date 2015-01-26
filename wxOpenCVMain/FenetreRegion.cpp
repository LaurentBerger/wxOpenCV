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
nbRegionMax=4000;
regionSelect=new char[nbRegionMax];
cleTri =  new long[nbRegionMax];
valTri = new float[nbRegionMax];

listeRegion = new Tableur((wxFrame*)this,0,13); 
listeRegion->SetSize(wxSize(800,400));
//listeRegion->Refresh();
listeRegion->DefTitreColonne(0, "Index");
listeRegion->DefTitreColonne(1, "Gray Level");
listeRegion->DefTitreColonne(2, "Gray Level\n(wo gradient)");
listeRegion->DefTitreColonne(3, "Surface");
listeRegion->DefTitreColonne(4, "Surface\n(wo gradient) ");
listeRegion->DefTitreColonne(5, "xg");
listeRegion->DefTitreColonne(6, "yg");
listeRegion->DefTitreColonne(7, "semi\nmajor\naxis");
listeRegion->DefTitreColonne(8, "semi\nminor\naxis");
listeRegion->DefTitreColonne(9, "theta");
listeRegion->DefTitreColonne(10, "Level\ndeviation");
listeRegion->DefTitreColonne(11, "Level\ndeviation\n(wo gradient)");
listeRegion->DefTitreColonne(12, "Height\nDifference");
listeRegion->DefTitreColonne(13, "Level\nIndex");
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
/*
GrapheImageInfo*	grapheRegion=NULL;//((wxOsgApp*)osgApp)->Graphique()->GrapheImage();
nbZoneId=0;
if (listeRegion->GetNumberRows()!=grapheRegion->nbSommets)
	{
	if (grapheRegion->nbSommets>nbRegionMax)
		{
		delete regionSelect;
		delete cleTri;
		delete valTri;
		nbRegionMax = grapheRegion->nbSommets;
		regionSelect =  new char[nbRegionMax];
		cleTri =  new long[nbRegionMax];
		valTri = new float[nbRegionMax];
		}
    }
for (int i=0;i<grapheRegion->nbSommets;i++)
	{
	regionSelect[i]=0;
	cleTri[i]=i;
	valTri[i]=(*grapheRegion)[i].moyImage[5];
	}
grapheRegion->trier(valTri,0,grapheRegion->nbSommets-1,cleTri);
int nb=listeRegion->GetNumberRows()-grapheRegion->nbSommets;
if (nb>0) 
	{
	listeRegion->EffaceLigne(nb);
	
	}
else if (nb<0)
	listeRegion->AjouteLigne(-nb);
float	*par=NULL;
if(((wxOsgApp*)osgApp)->Graphique()->ModeRectangle())
	{
	wxRect r(*((wxOsgApp*)osgApp)->Graphique()->RectangleSelec());
//	grapheRegion->InitGraphe(((wxOsgApp*)osgApp)->Graphique()->ImRegion(),r.GetLeft(),r.GetTop(),r.GetWidth(),r.GetHeight());
	}
for (int ii=0;ii<grapheRegion->nbSommets;ii++)
	{
	int i=cleTri[ii];
	listeRegion->DefCellule(ii,0,i, "%5d");
		{
		if(((wxOsgApp*)osgApp)->Graphique()->ModeRectangle() && (*grapheRegion)[i].moyImage[4]>0)
			{
			wxString	s;
			s.Printf(_T("%7.1f(%7.1f)"),(*grapheRegion)[i].moyImage[0],(*grapheRegion)[i].moyImage[4]);
			
			listeRegion->DefCellule(ii,1,&s);
			s.Printf(_T("%7.1f"),(*grapheRegion)[i].moyImage[5]);
			
			listeRegion->DefCellule(ii,2,&s);
			
			s.Printf(_T("%7.0f(%7.0f)"),(*grapheRegion)[i].surface,(*grapheRegion)[i].moyImage[1]);
			listeRegion->DefCellule(ii,3,&s);
			s.Printf(_T("%7.0f"),(*grapheRegion)[i].nbPixels[5]);
			listeRegion->DefCellule(ii,4,&s);
			
			s.Printf(_T("%5.1f(%5.1f)"),(*grapheRegion)[i].xg,(*grapheRegion)[i].moyImage[2]);
			listeRegion->DefCellule(ii,5, &s);
			s.Printf(_T("%5.1f(%5.1f)"),(*grapheRegion)[i].yg,(*grapheRegion)[i].moyImage[3]);
			listeRegion->DefCellule(ii,6,&s);
			}
		else
			{
			listeRegion->DefCellule(ii,1,(*grapheRegion)[i].moyImage[0], "%7.1f");
			listeRegion->DefCellule(ii,2,(*grapheRegion)[i].moyImage[5], "%7.1f");
			listeRegion->DefCellule(ii,3,(*grapheRegion)[i].surface, "%7.0f");
			listeRegion->DefCellule(ii,4,(*grapheRegion)[i].nbPixels[5], "%7.0f");
			listeRegion->DefCellule(ii,5,(*grapheRegion)[i].xg, "%6.1f");
			listeRegion->DefCellule(ii,6,(*grapheRegion)[i].yg, "%6.1f");
			}
		listeRegion->DefCellule(ii,7,sqrt(2*(*grapheRegion)[i].v1), "%6.1f");
		listeRegion->DefCellule(ii,8,sqrt(2*(*grapheRegion)[i].v2), "%6.1f");
		listeRegion->DefCellule(ii,9,(*grapheRegion)[i].theta, "%6.1f");
		listeRegion->DefCellule(ii,10,sqrt((*grapheRegion)[i].varImage[0]), "%6.1f");
		listeRegion->DefCellule(ii,11,sqrt((*grapheRegion)[i].varImage[5]), "%6.1f");
		if (ii>0)
			{
			double x=(*grapheRegion)[i].moyImage[5]-(*grapheRegion)[cleTri[ii-1]].moyImage[5];
			listeRegion->DefCellule(ii,12, x,"%4.1f");
			}
		listeRegion->DefCellule(ii,13, 0,"%3d");
		}
	}
*/	
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

		
