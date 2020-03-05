// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyCanvas : public wxWindow
{
public:
    MyCanvas(wxFrame* parent);
    void OnPaint(wxPaintEvent& event);
private:
    DECLARE_EVENT_TABLE()
};

MyCanvas::MyCanvas(wxFrame* parent) : wxWindow(parent, -1)
{

}

void MyCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen(*wxBLACK);
    dc.SetBrush(*wxRED);
    dc.DrawRectangle(10, 10, 315, 315);
}

BEGIN_EVENT_TABLE(MyCanvas, wxWindow)
EVT_PAINT  (MyCanvas::OnPaint)
END_EVENT_TABLE()

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    MyCanvas* canvas;
};

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE, wxT("1"))
{

}

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
private:
    MyFrame* frame;
    MyCanvas* canvas;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    frame = new MyFrame(wxT("LED Array"), wxPoint(), wxSize(2500,1800));
    canvas = new MyCanvas(frame);
    frame->Show(true);
    return true;
}
