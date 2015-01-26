#ifndef __GLISSERFORME__
#define __GLISSERFORME__

#include "wx/image.h"
#include "wx/dcmemory.h"

// Under Windows, change this to 1
// to use wxGenericDragImage

#define wxUSE_GENERIC_DRAGIMAGE 1

#if wxUSE_GENERIC_DRAGIMAGE
#include "wx/generic/dragimgg.h"
#define wxDragImage wxGenericDragImage
#else
#include "wx/dragimag.h"
#endif

// Ways to drag a shape

#define SHAPE_DRAG_BITMAP       1
#define SHAPE_DRAG_TEXT         2
#define SHAPE_DRAG_ICON         3

// Dragging modes
#define TEST_DRAG_NONE     0
#define TEST_DRAG_START    1
#define TEST_DRAG_DRAGGING 2


class FenetrePrincipale;
class wxOsgApp;
// Shape

class DragShape: public wxObject
{
public:
    DragShape(const wxBitmap& bitmap);
    ~DragShape(){};

//// Operations

    bool HitTest(const wxPoint& pt) const;
    bool Draw(wxDC& dc, bool highlight = false);

//// Accessors

    wxPoint GetPosition() const { return m_pos; }
    void SetPosition(const wxPoint& pos) { m_pos = pos; }

    wxRect GetRect() const { return wxRect(m_pos.x, m_pos.y, m_bitmap.GetWidth(), m_bitmap.GetHeight()); }

    wxBitmap& GetBitmap() const { return (wxBitmap&) m_bitmap; }
    void SetBitmap(const wxBitmap& bitmap) { m_bitmap = bitmap; }

    int GetDragMethod() const { return m_dragMethod; }
    void SetDragMethod(int method) { m_dragMethod = method; }

    bool IsShown() const { return m_show; }
    void SetShow(bool show) { m_show = show; }

protected:
    wxPoint     m_pos;
    wxBitmap    m_bitmap;
    int         m_dragMethod;
    bool        m_show;
};


class GlisserImage: public wxDragImage
{
public:
    GlisserImage(FenetrePrincipale* canvas): m_canvas(canvas) {}

    GlisserImage(FenetrePrincipale* canvas, const wxBitmap& image, const wxCursor& cursor = wxNullCursor):
        wxDragImage(image, cursor), m_canvas(canvas)
    {
    }

    GlisserImage(FenetrePrincipale* canvas, const wxIcon& image, const wxCursor& cursor = wxNullCursor):
        wxDragImage(image, cursor), m_canvas(canvas)
    {
    }

    GlisserImage(FenetrePrincipale* canvas, const wxString& str, const wxCursor& cursor = wxNullCursor):
        wxDragImage(str, cursor), m_canvas(canvas)
    {
    }

    // On some platforms, notably Mac OS X with Core Graphics, we can't blit from
    // a window, so we need to draw the background explicitly.
    virtual bool UpdateBackingFromWindow(wxDC& windowDC, wxMemoryDC& destDC, const wxRect& sourceRect,
                    const wxRect& destRect) const;
void DefOSGApp(wxOsgApp *w){osgApp=w;};

protected:
    FenetrePrincipale*   m_canvas;
	wxOsgApp			*osgApp;
};



#endif
