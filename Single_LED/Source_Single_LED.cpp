//Chase Hunter
//University of Washington
//EE 497/8 2020
//The purpose of this program is to demonstrate the basics implementing keyboard controls
//to manipulate the drawing.

//Includes
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//Define Visual Studio version for backwards compatablity
#define _MSC_VER 1924

//Define MyCanvas class that handles the drawing of shapes within the frame
class MyCanvas : public wxWindow
{
public:
    //MyCanvas public methods
    MyCanvas(wxFrame* parent);
    void OnPaint(wxPaintEvent& event);
    void LedInput(wxKeyEvent& event);
private:
    //Private variables
    bool color;
    //Declare event table to handle all relavent events
    DECLARE_EVENT_TABLE()
};

//Define default constructor, Initializes color to red
MyCanvas::MyCanvas(wxFrame* parent) : wxWindow(parent, -1)
{
    color = false;
}

//Controls the logic for a keyboard input
void MyCanvas::LedInput(wxKeyEvent& event)
{  
    //If entered key is w, change color to green
    //Square remains green while w is held down
    if (event.GetUnicodeKey() == 'w')
    {
        color = true;
    }
    else
    {
        color = false;
    }
    //Force the MyCanvas to redraw and the MyFrame to update
    Refresh();
    Update();
}

//Define logic during paint event and
//draws square with color based on keyboard input
void MyCanvas::OnPaint(wxPaintEvent& event)
{
    //Declares drawing object
    wxPaintDC dc(this);
    //Pen handles the border color
    dc.SetPen(*wxBLACK);
    //Brush handles the fill color of the square
    //Set brush to green if color is true else red
    if (color)
    {
        dc.SetBrush(*wxGREEN);
    }
    else
    {
        dc.SetBrush(*wxRED);
    }
    //Draw square with defined size and location
    dc.DrawRectangle(10, 10, 315, 315);
}

//Event table
BEGIN_EVENT_TABLE(MyCanvas, wxWindow)
//Paint event
EVT_PAINT  (MyCanvas::OnPaint)
//Keyboard character input
EVT_CHAR(MyCanvas::LedInput)
//Keyboard key released
EVT_KEY_UP(MyCanvas::LedInput)
END_EVENT_TABLE()

//Define the MyFrame class that displays the frame in which the drawing occurs
class MyFrame : public wxFrame
{
public:
    //Constructor
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
private:
    //MyFrame contains a MyCanvas variable pointer so the MyCanvas object can
    //draw shapes within the frame
    MyCanvas* canvas;
};

//Define default constructor. Empty because MyFrame uses predefined wxWidgets constructor
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE, wxT("1"))
{

}

//Define the MyApp class that handles the logic initializing and ending the frame and canvas objects
class MyApp : public wxApp
{
public:
    //Initialization method
    virtual bool OnInit();
private:
    //MyApp contains frame and canvas objects
    MyFrame* frame;
    MyCanvas* canvas;
};

//Event occurs at program startup to initizailze MyApp object
wxIMPLEMENT_APP(MyApp);

//Initializes the program
bool MyApp::OnInit()
{
    //Create new MyFrame and MyCanvas objects
    frame = new MyFrame(wxT("Single LED"), wxPoint(), wxSize(2500,1800));
    canvas = new MyCanvas(frame);
    //Display frame
    frame->Show(true);
    return true;
}
