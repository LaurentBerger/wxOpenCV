#include "VideoCourbe.h"

FenetreCourbe::FenetreCourbe( wxWindow* parent, wxWindowID id, const wxPoint& pos,  const wxSize& size, long style, int pl_style ) : 
          wxPLplotwindow<wxFrame>( true )
 {
    Create( parent, wxID_ANY, wxT( "wxPLplotDemo" ) );
//    frame->SetIcon( wxIcon( graph ) );
    Show();
    x.resize(6);
    y.resize(6);
    fenMere=(FenetrePrincipale*)parent;
    Plot(  );

    
 }


void FenetreCourbe::OnClose(wxCloseEvent& event)
{
fenMere->RAZFenAlgo();
wxFrame::OnCloseWindow(event);

}

void FenetreCourbe::OnChar( wxKeyEvent& event )
{
  int keycode = event.GetKeyCode();
  
  if( keycode==WXK_RETURN ||
      keycode==WXK_SPACE ||
      keycode==WXK_RIGHT ||
      keycode==WXK_ESCAPE )
    frame->Close( true );
  else
    event.Skip();
}

FenetreCourbe::~FenetreCourbe()
{
if (fenMere)
{
    fenMere->Feuille()->ClearShapes();
    fenMere->RAZCourbeVideo();
}

}






