#include "FenetrePrincipale.h"
#include "GlisserForme.h"
// DragShape



DragShape::DragShape(const wxBitmap& bitmap)
{
    m_bitmap = bitmap;
    m_pos.x = 0;
    m_pos.y = 0;
    m_dragMethod = SHAPE_DRAG_BITMAP;
    m_show = true;
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

        dc.Blit(m_pos.x, m_pos.y, m_bitmap.GetWidth(), m_bitmap.GetHeight(),
            & memDC, 0, 0, wxCOPY, true);

        if (highlight)
        {
            dc.SetPen(*wxWHITE_PEN);
            dc.SetBrush(*wxTRANSPARENT_BRUSH);
            dc.DrawRectangle(m_pos.x, m_pos.y, 2*m_bitmap.GetWidth(), 2*m_bitmap.GetHeight());
        }

        return true;
    }
    else
        return false;
}


