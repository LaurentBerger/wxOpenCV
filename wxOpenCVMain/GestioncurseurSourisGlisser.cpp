#include "dragicon.xpm"
#include "FenetrePrincipale.h"
#include "imagestat.h"
//#include "OutilsImage.h"
#include "GlisserForme.h"
#include "FenetreAlgo.h"

FenetrePrincipale	*ZoneImage::fenDrag=NULL;
char				ZoneImage::operationSelectionne=0;


bool FenetrePrincipale::TileBitmap(const wxRect& rect, wxDC& dc, wxBitmap& bitmap)
{
//	TracerDIB(imAffichee,dc);
    int			w = bitmap.GetWidth();
    int			h = bitmap.GetHeight();
    wxRect r=rect;
    int i, j;
    wxRect rEcran = RepereEcranImage(r);
    for (i = rEcran.x; i < rEcran.x + rEcran.width; i += w)
    {
        for (j = rEcran.y; j < rEcran.y + rEcran.height; j += h)
        {
            wxPoint p(i,j),ptEcran=RepereImageEcran(p);
            dc.DrawBitmap(bitmap, -p.x, -p.y);
            wxLogWarning(_("TileBitmap (%d,%d)"),p.x,p.y);

        }
    }
    return true;
}


DragShape* ZoneImage::AjouteForme(wxPoint ptImg,int couleur,int forme,void *fenAlgo,int indRef,wxString s)
{
	wxMemoryDC memDC;
    couleur+=11;
	wxColor c(*wxStockGDI::GetColour((wxStockGDI::Item)couleur));
    wxBitmap m_bmp = wxBitmap(8, 8);
    memDC.SelectObject(m_bmp);
    wxBrush brosse(wxColour(0, 0, 0, 255));
    brosse.SetStyle(wxBRUSHSTYLE_TRANSPARENT);
    memDC.SetBackground(brosse);
    memDC.Clear();
    memDC.SetPen(c);
    memDC.SetBrush(wxBrush(c));

	memDC.DrawCircle(4,4,8);

    DragShape* newShape = new DragShape(m_bmp);
    newShape->SetPosition(ptImg);
    newShape->SetZoneImage(this);
    newShape->SetFenAlgo(fenAlgo,indRef);
    if (s==wxEmptyString)
        newShape->SetDragMethod(SHAPE_DRAG_BITMAP);
    else
    {
        newShape->SetDragMethod(SHAPE_DRAG_TEXT);
        newShape->SetString(s);
    }
    GetDisplayList().Append(newShape);
    return newShape;
}


// On some platforms, notably Mac OS X with Core Graphics, we can't blit from
// a window, so we need to draw the background explicitly.
bool GlisserImage::UpdateBackingFromWindow(wxDC& WXUNUSED(windowDC), wxMemoryDC& destDC, const wxRect& WXUNUSED(sourceRect),
                    const wxRect& destRect) const
{
    destDC.SetClippingRegion(destRect);
    if (m_canvas->FenMere()->ImageAffichee())
		{
		wxBitmap b=*m_canvas->BitmapAffichee();
        m_canvas->FenMere()->TileBitmap(destRect, destDC, b);
		}
    m_canvas->FenMere()->DrawShapes(destDC);
    return true;
}




DragShape::DragShape(const wxBitmap& bitmap)
{
    m_bitmap = bitmap;
    m_pos.x = 0;
    m_pos.y = 0;
    m_dragMethod = SHAPE_DRAG_BITMAP;
    m_show = true;
    chaine =wxEmptyString;
}

bool DragShape::HitTest(const wxPoint& pt) const
{
    wxRect rect(GetRect());
    return rect.Contains(pt.x, pt.y);
}

bool DragShape::Draw(wxDC& dc, bool highlight)
{
    if (m_bitmap.IsOk())
    {
        wxMemoryDC memDC;
        memDC.SelectObject(m_bitmap);
        wxPoint ptImg(m_pos.x, m_pos.y),pt;
        //((ZoneImage*)zoneImage)->PrepareDC(dc);
        pt=((ZoneImage*)zoneImage)->FenMere()->RepereImageEcran(ptImg);

        dc.Blit(pt.x, pt.y, m_bitmap.GetWidth(), m_bitmap.GetHeight(),
            & memDC, 0, 0, wxCOPY, true);

        if (highlight)
        {
            dc.SetPen(*wxWHITE_PEN);
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRectangle(pt.x, pt.y, 2*m_bitmap.GetWidth(), 2*m_bitmap.GetHeight());
        }

        return true;
    }
    else
        return false;
}

void DragShape::SetPosition(const wxPoint& pos,bool pasDeMaj)
{
    m_pos = pos;
    if (fenAlgo != NULL && pasDeMaj)
    {
        ((FenetreAlgo*)fenAlgo)->PositionSouris(indRef,m_pos);
    }
}




void FenetrePrincipale::RedresseRectangle(wxRect &r)
{
if (r.GetTop()>r.GetBottom())
	{
	int x=r.GetTop();
	r.SetTop(r.GetBottom());
	r.SetBottom(x);
	}
if (r.GetRight()<r.GetLeft())
	{
	int x=r.GetLeft();
	r.SetLeft(r.GetRight());
	r.SetRight(x);
	}
}

DragShape* ZoneImage::FindShape(const wxPoint& pt) const
{
    wxList::compatibility_iterator node = m_displayList.GetFirst();
    while (node)
    {
        DragShape* shape = (DragShape*) node->GetData();
        if (shape->HitTest(pt) )
        {
            return shape;
        }
        node = node->GetNext();
    }
    return (DragShape*) NULL;
}

void ZoneImage::DrawShapes(wxDC &dc)
{
	wxList::compatibility_iterator node = m_displayList.GetFirst();
    while (node)
    {
        DragShape* shape = (DragShape*) node->GetData();
        if (shape->IsShown() && m_draggedShape != shape)
        {
            shape->Draw(dc, (m_currentlyHighlighted == shape));
        }
        node = node->GetNext();
    }
}

void ZoneImage::EraseShape(DragShape* shape, wxDC& dc)
{
    wxSize sz = GetClientSize();
    wxRect rect(0, 0, sz.x, sz.y);

    wxRect rect2(shape->GetRect());
    dc.SetClippingRegion(rect2.x, rect2.y, rect2.width, rect2.height);

    f->TileBitmap(rect, dc, osgApp->GetBackgroundBitmap());

    dc.DestroyClippingRegion();
}

void ZoneImage::ClearShapes()
{
    wxList::compatibility_iterator node = m_displayList.GetFirst();
    while (node)
    {
        DragShape* shape = (DragShape*) node->GetData();
        delete shape;
        node = node->GetNext();
    }
    m_displayList.Clear();
}

void wxOsgApp::SourisQuitterFen(wxMouseEvent &event)
{
int x=0;

}

void FenetrePrincipale::SourisQuitterFen(wxMouseEvent &event)
{
int x=0;
}




void ZoneImage::GestionCurseurSourisGlisser(wxMouseEvent &event)
{
//#ifdef __MSVC__
	if (fenDrag  && fenDrag !=f )
		{
		ArreterDragging(event);
		}
    if (event.LeftUp() && m_dragMode != TEST_DRAG_NONE)
    {
        // Finish dragging
		ArreterDragging(event);
    }
    else if (event.Dragging() && m_dragMode != TEST_DRAG_NONE)
    {
        if (m_dragMode == TEST_DRAG_START)
        {
            // We will start dragging if we've moved beyond a couple of pixels

            int tolerance = 2;
            wxPoint ptImg = m_dragStartPos,pt=f->RepereImageEcran(ptImg);

            int dx = abs(event.GetPosition().x - pt.x);
            int dy = abs(event.GetPosition().y - pt.y);
            if (dx <= tolerance && dy <= tolerance)
                return;

            // Start the drag.
            m_dragMode = TEST_DRAG_DRAGGING;

            if (m_dragImage)
                delete m_dragImage;

            // Erase the dragged shape from the canvas
            m_draggedShape->SetShow(false);

            // redraw immediately
            Refresh(true);
            Update();

            switch (m_draggedShape->GetDragMethod())
            {
                case SHAPE_DRAG_BITMAP:
                {
                    m_dragImage = new GlisserImage(this, m_draggedShape->GetBitmap(), wxCursor(wxCURSOR_HAND));
                    break;
                }
                case SHAPE_DRAG_TEXT:
                {
                    m_dragImage = new GlisserImage(this, m_draggedShape->GetString(), wxCursor(wxCURSOR_HAND));
                    break;
                }
                case SHAPE_DRAG_ICON:
                {
                    m_dragImage = new GlisserImage(this, wxICON(dragicon), wxCursor(wxCURSOR_HAND));
                    m_dragImage->DefOSGApp(osgApp);
					break;
                }
            }

            bool fullScreen = false;//osgApp->GetUseScreen();

            // The offset between the top-left of the shape image and the current shape position
            ptImg=m_draggedShape->GetPosition();
            wxPoint ptEcran = f->RepereImageEcran(ptImg);
            wxPoint beginDragHotSpot = pt - ptEcran;

            // Now we do this inside the implementation: always assume
            // coordinates relative to the capture window (client coordinates)

            //if (fullScreen)
            //    beginDragHotSpot -= ClientToScreen(wxPoint(0, 0));

            if (!m_dragImage->BeginDrag(beginDragHotSpot, this, fullScreen))
            {
                wxDELETE(m_dragImage);
                m_dragMode = TEST_DRAG_NONE;

            } else
            {
                m_dragImage->Move(event.GetPosition());
                m_dragImage->Show();
            }
        }
        else if (m_dragMode == TEST_DRAG_DRAGGING )
        {
            // We're currently dragging. See if we're over another shape.
            DragShape* onShape = FindShape(event.GetPosition());

            bool mustUnhighlightOld = false;
            bool mustHighlightNew = false;

            if (m_currentlyHighlighted)
            {
                if ((onShape == (DragShape*) NULL) || (m_currentlyHighlighted != onShape))
                    mustUnhighlightOld = true;
            }

            if (onShape && (onShape != m_currentlyHighlighted) && onShape->IsShown())
                mustHighlightNew = true;

            if (mustUnhighlightOld || mustHighlightNew)
                m_dragImage->Hide();

            // Now with the drag image switched off, we can change the window contents.
            if (mustUnhighlightOld)
                m_currentlyHighlighted = (DragShape*) NULL;

            if (mustHighlightNew)
                m_currentlyHighlighted = onShape;

            if (mustUnhighlightOld || mustHighlightNew)
            {
                Refresh(mustUnhighlightOld);
                Update();
            }

            // Move and show the image again
            m_dragImage->Move(event.GetPosition());

            if (mustUnhighlightOld || mustHighlightNew)
                 m_dragImage->Show();
        }
        else
               ArreterDragging(event);
    }
    else
           ArreterDragging(event);
//#endif
}

void ZoneImage::OnEraseBackground(wxEraseEvent& event)
{
    if (f->ImageAffichee())
    {
        wxSize sz = GetClientSize();
        wxRect rect(0, 0, sz.x, sz.y);

		wxBitmap	b(*f->ImageAffichee());
            wxPaintDC dc(this);
        if (event.GetDC())
        {
            f->TileBitmap(rect, dc, b);
        }
        else
        {
            f->TileBitmap(rect, dc, b);
        }
    }
    else
        event.Skip(); // The official way of doing it
}


void FenetrePrincipale::TracerRectangle(int ind,int croix)
{
wxClientDC dc(this);
wxPoint pTmp=feuille->RectangleSelec()->GetTopLeft();
wxPoint p1(RepereImageEcran(pTmp));
//ClientToScreen(&p1.x,&p1.y);
wxBrush	tr=*wxTRANSPARENT_BRUSH;
dc.SetBrush(tr);
dc.SetLogicalFunction(wxXOR);
dc.SetPen(wxPen(*wxWHITE, 3));
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);
wxRect rTrace(p1.x,p1.y,feuille->RectangleSelec()->width*fZoomNume/fZoomDeno,feuille->RectangleSelec()->height*fZoomNume/fZoomDeno);
dc.DrawRectangle(p1.x,p1.y,feuille->RectangleSelec()->width*fZoomNume/fZoomDeno,feuille->RectangleSelec()->height*fZoomNume/fZoomDeno);
if (croix)
	{
	dc.DrawLine(rTrace.GetBottomLeft(),rTrace.GetTopRight());
	dc.DrawLine(rTrace.GetTopLeft(),rTrace.GetBottomRight());
	}
feuille->Refresh(false);
}

void ZoneImage::ArreterDragging(wxMouseEvent &event)
{
	m_dragMode = TEST_DRAG_NONE;

    if (!m_draggedShape || !m_dragImage)
        return;
    wxPoint pt= event.GetPosition() ;
    wxPoint img = m_draggedShape->GetPosition()+f->RepereEcranImage(pt)- m_dragStartPos;
    m_draggedShape->SetPosition(img);

    m_dragImage->Hide();
    m_dragImage->EndDrag();
    wxDELETE(m_dragImage);

    m_draggedShape->SetShow(true);

    m_currentlyHighlighted = (DragShape*) NULL;

    m_draggedShape = (DragShape*) NULL;

    Refresh(true);
}
