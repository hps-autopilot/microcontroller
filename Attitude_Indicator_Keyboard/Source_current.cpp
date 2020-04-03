// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <attitude_indicator.h>
#include <string>
using std::string;
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

class MyCanvas : public wxPanel
{
public:
    MyCanvas(wxFrame* parent, int x_in, int y_in, const wxPoint& pos,
              const wxSize& size);
    void OnPaint(wxPaintEvent& event);
    void KeyDown(wxKeyEvent& event);
private:
    
    int x, y, ledDim;
    attitude_indicator ai;
    DECLARE_EVENT_TABLE()
};

MyCanvas::MyCanvas(wxFrame* parent, int x_in, int y_in, const wxPoint& pos,
                    const wxSize& size) 
    : wxPanel(parent, -1, pos, size)
{
    ledDim = 8;
    ai.setDim(ledDim);
    x = x_in;
    y = y_in;
}

void MyCanvas::KeyDown(wxKeyEvent& event)
{  
    char key = event.GetUnicodeKey();

    switch (key)
    {
    case 'W': 
        ai.pitchUp();
        break;
    case 'S':
        ai.pitchDown();
        break;
    case 'Q':
        ai.rollLeft();
        break;
    case 'E':
        ai.rollRight();
        break;
    case 'X':
        ai.calibrate();
        break;
    }
    
    Refresh();
    Update();
}


void MyCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen(*wxBLACK);
    int i, j;
    int size = 200;
    for (i = 0; i < ledDim; i++)
    {
        for (j = 0; j < ledDim; j++)
        {
            if (ai.getValue(i, j))
            {
                dc.SetBrush(*wxGREEN);
            }
            else
            {
                dc.SetBrush(*wxRED);
            }
            dc.DrawRectangle(size*j, size*i, size, size);
        }
    }
}


BEGIN_EVENT_TABLE(MyCanvas, wxPanel)
EVT_PAINT  (MyCanvas::OnPaint)
EVT_KEY_DOWN   (MyCanvas::KeyDown)
//EVT_KEY_UP (MyCanvas::KeyUp)
END_EVENT_TABLE()

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    MyCanvas* canvas1;
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
    MyCanvas* canvas1;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    frame = new MyFrame(wxT("LED Array"), wxPoint(), wxSize(2500,1800));
    canvas1 = new MyCanvas(frame, 10, 10, wxPoint(0,0), wxSize(2500, 1800));
    frame->Show(true);
    return true;
}
