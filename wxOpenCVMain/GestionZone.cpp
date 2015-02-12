#include "FenetrePrincipale.h"
#include "imagestat.h"
//#include "OutilsImage.h"
#include "GlisserForme.h"

FenetrePrincipale	*FenetrePrincipale::fenDrag=NULL;
char				FenetrePrincipale::operationSelectionne=0;

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

DragShape* FenetrePrincipale::FindShape(const wxPoint& pt) const
{
    wxList::compatibility_iterator node = m_displayList.GetFirst();
    while (node)
    {
        DragShape* shape = (DragShape*) node->GetData();
		shape->SetDragMethod(SHAPE_DRAG_TEXT);
        if (shape->HitTest(pt) || 1==1)
            return shape;
        node = node->GetNext();
    }
    return (DragShape*) NULL;
}

void FenetrePrincipale::DrawShapes(wxDC& dc)
{
    return;
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

void FenetrePrincipale::EraseShape(DragShape* shape, wxDC& dc)
{
    wxSize sz = GetClientSize();
    wxRect rect(0, 0, sz.x, sz.y);

    wxRect rect2(shape->GetRect());
    dc.SetClippingRegion(rect2.x, rect2.y, rect2.width, rect2.height);

    TileBitmap(rect, dc, osgApp->GetBackgroundBitmap());

    dc.DestroyClippingRegion();
}

void FenetrePrincipale::ClearShapes()
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

void FenetrePrincipale::GestionCurseurSouris(wxMouseEvent &event)
{
}



void FenetrePrincipale::GestionCurseurSourisGlisser(wxMouseEvent &event)
{
#ifdef __MSVC__
	if (fenDrag && operationSelectionne!=0 && fenDrag !=this )
		{
		fenDrag->ArreterDragging();
		operationSelectionne=1;
		}
    if (operationSelectionne==1 )
    {
 		fenDrag=this;
        DragShape* shape = FindShape(event.GetPosition());
        if (shape)
        {
            // We tentatively start dragging, but wait for
            // mouse movement before dragging properly.

            m_dragMode = TEST_DRAG_START;
            m_dragStartPos = event.GetPosition();
            m_draggedShape = shape;
			m_draggedShape->SetPosition(event.GetPosition());
			operationSelectionne=2;
        }
    }
    else if (operationSelectionne==2 && event.LeftDown() && event.ShiftDown()  && m_dragMode != TEST_DRAG_NONE)
    {
        // Finish dragging
		ArreterDragging();
    }
    else if (operationSelectionne==2 /*&& event.Dragging()*/ && m_dragMode != TEST_DRAG_NONE)
    {
        if (m_dragMode == TEST_DRAG_START)
        {
            // We will start dragging if we've moved beyond a couple of pixels

            int tolerance = 2;
            int dx = abs(event.GetPosition().x - m_dragStartPos.x);
            int dy = abs(event.GetPosition().y - m_dragStartPos.y);
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
                    m_dragImage = new GlisserImage(this, wxString("  Erode this image"), wxCursor(wxCURSOR_HAND));
                    break;
                }
                case SHAPE_DRAG_ICON:
                {
                    m_dragImage = new GlisserImage(this, wxICON(dragicon), wxCursor(wxCURSOR_HAND));
                    m_dragImage->DefOSGApp(osgApp);
					break;
                }
            }

            bool fullScreen = true;//osgApp->GetUseScreen();

            // The offset between the top-left of the shape image and the current shape position
            wxPoint beginDragHotSpot = m_dragStartPos - m_draggedShape->GetPosition();

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

 /*           if (onShape && (onShape != m_currentlyHighlighted) && onShape->IsShown())
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
            }*/

            // Move and show the image again
            m_dragImage->Move(event.GetPosition());

            if (mustUnhighlightOld || mustHighlightNew)
                 m_dragImage->Show();
        }
    }
#endif
}

void FenetrePrincipale::OnEraseBackground(wxEraseEvent& event)
{
    if (imAffichee)
    {
        wxSize sz = GetClientSize();
        wxRect rect(0, 0, sz.x, sz.y);

		wxBitmap	b(*imAffichee);

        if (event.GetDC())
        {
            TileBitmap(rect, *(event.GetDC()), b);
        }
        else
        {
            wxClientDC dc(this);
            TileBitmap(rect, dc, b);
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
int		fZoomNume,fZoomDeno;

CalculZoom(fZoomNume,fZoomDeno);
wxRect rTrace(p1.x,p1.y,feuille->RectangleSelec()->width*fZoomNume/fZoomDeno,feuille->RectangleSelec()->height*fZoomNume/fZoomDeno);
dc.DrawRectangle(p1.x,p1.y,feuille->RectangleSelec()->width*fZoomNume/fZoomDeno,feuille->RectangleSelec()->height*fZoomNume/fZoomDeno);
if (croix)
	{
	dc.DrawLine(rTrace.GetBottomLeft(),rTrace.GetTopRight());
	dc.DrawLine(rTrace.GetTopLeft(),rTrace.GetBottomRight());
	}
DrawRectangles(dc);

}

void FenetrePrincipale::ArreterDragging(FenetrePrincipale *f)
{
if (f==NULL)
	{
	    m_dragMode = TEST_DRAG_NONE;

        if (!m_draggedShape || !m_dragImage)
            return;

/*        m_draggedShape->SetPosition(m_draggedShape->GetPosition()
                                    + event.GetPosition() - m_dragStartPos);*/

        m_dragImage->Hide();
        m_dragImage->EndDrag();
        wxDELETE(m_dragImage);

        m_draggedShape->SetShow(true);

        m_currentlyHighlighted = (DragShape*) NULL;

        m_draggedShape = (DragShape*) NULL;

        Refresh(true);
		operationSelectionne=0;
	}
else
	f->ArreterDragging(NULL);
}
