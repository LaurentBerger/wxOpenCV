#ifndef __FENETREINFO__
#include <wx/wx.h>
#include <opencv2/core.hpp>

// Define a new frame type: this is going to the frame showing the
// effect of wxFRAME_SHAPED
class ShapedFrame : public wxFrame
{
public:
    // ctor(s)
    ShapedFrame(wxFrame *parent,wxPoint &);
    void SetWindowShape();

    // event handlers (these functions should _not_ be virtual)
    void OnDoubleClick(wxMouseEvent& evt);
    void OnLeftDown(wxMouseEvent& evt);
    void OnLeftUp(wxMouseEvent& evt);
    void OnMouseMove(wxMouseEvent& evt);
    void OnExit(wxMouseEvent& evt);
    void OnPaint(wxPaintEvent& evt);

private:
    enum ShapeKind
    {
        Shape_None,
        Shape_Star,
#if wxUSE_GRAPHICS_CONTEXT
        Shape_Circle,
#endif // wxUSE_GRAPHICS_CONTEXT
        Shape_Max
    } m_shapeKind;

    wxBitmap m_bmp;
    wxPoint  m_delta;

    template<typename T_>
    void DrawSelected(cv::Mat im, wxPoint & pSrc, wxMemoryDC & memDC);

    // any class wishing to process wxWidgets events must use this macro
};

// Define a new frame type: this is going to the frame showing the
// effect of wxFRAME_SHAPED
class PointFrame : public wxFrame
{
public:
    // ctor(s)
    PointFrame(wxFrame *parent,wxPoint &);
    void SetWindowShape();

    // event handlers (these functions should _not_ be virtual)
    void OnDoubleClick(wxMouseEvent& evt);
    void OnLeftDown(wxMouseEvent& evt);
    void OnLeftUp(wxMouseEvent& evt);
    void OnMouseMove(wxMouseEvent& evt);
    void OnExit(wxMouseEvent& evt);
    void OnPaint(wxPaintEvent& evt);

private:

    wxBitmap m_bmp;
    wxPoint  m_delta;

    // any class wishing to process wxWidgets events must use this macro
};


#endif

