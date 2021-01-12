//Chase Hunter
//University of Washington
//EE 497/8 2020
//The purpose of this program is to implent the attitude indicator
//LED display logic and demostrate its basic function using keyboard commands
//on both a software simulated display as well as a hardware display connected
//to the computer using a FT232H interface board controlling a hardware LED
//matrix through a I2C interface.

//Includes
#include <wx/wxprec.h>
#include <attitude_indicator.h>
#include <LED_Matrix.h>
#include <string>
using std::string;
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

//Define Visual Studio version for backwards compatablity
#define _MSC_VER 1924
//Define the size of the LED squares
#define SIZE     100
//Define size of window
//Minimum size dependent on SIZE parameter
#define WINDOW_RES_X 1000
#define WINDOW_RES_Y 1000

//Define MyCanvas class that handles the drawing of shapes within the frame
class MyCanvas : public wxPanel
{
public:
    //MyCanvas public methods
    MyCanvas(wxFrame* parent, int x_in, int y_in, const wxPoint& pos,
              const wxSize& size, LED_Matrix *panelDisplayPtr);
    void OnPaint(wxPaintEvent& event);
    void KeyDown(wxKeyEvent& event);
private:
    //Private variables
    int x, y, ledDim;
    //LED_Matrix object controls the hardware matrix display
    LED_Matrix panelDisplay;
    //Attitude indicator object controls attitude indicator logic
    attitude_indicator ai;
    //Declare event table to handle all relavent events
    DECLARE_EVENT_TABLE()
};

//Define default constructor, Initializes attitude indicator
//and hardware LED matrix
MyCanvas::MyCanvas(wxFrame* parent, int x_in, int y_in, const wxPoint& pos,
                    const wxSize& size, LED_Matrix *panelDisplayPtr) 
    : wxPanel(parent, -1, pos, size)
{
    panelDisplay = *panelDisplayPtr;
    panelDisplay.FT_Init();
    panelDisplay.calibrateMatrix();
    ledDim = 8;
    ai.setDim(ledDim);
    x = x_in;
    y = y_in;
}

//Controls the logic for a keyboard input
void MyCanvas::KeyDown(wxKeyEvent& event)
{  
    char key = event.GetUnicodeKey();

    //Pitch up   if key = W
    //Pitch down if key = S
    //Roll left  if key = Q
    //Roll right if key = E
    //Reset/calibrate display if key = X
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
    //Force the MyCanvas to redraw and the MyFrame to update
    Refresh();
    Update();
}

//Define logic during paint event and
//draws every pixel of display with color
//determined by attitude indicator logic
void MyCanvas::OnPaint(wxPaintEvent& event)
{
    //Creates paint object
    wxPaintDC dc(this);
    //Pen controls the square border color
    //Brush controls the square fill color
    dc.SetPen(*wxBLACK);
    bool flag;
    int i, j;
    //Draws every pixel the color determined by the attitude indicator object
    //Draws for both the software and hardware displays
    for (i = 0; i < ledDim; i++)
    {
        for (j = 0; j < ledDim; j++)
        {
            if (ai.getValue(i, j))
            {
                flag = true;
                dc.SetBrush(*wxGREEN);
            }
            else
            {
                flag = false;
                dc.SetBrush(*wxRED);
            }
            //Software display
            dc.DrawRectangle(SIZE*j, SIZE *i, SIZE, SIZE);
            //Hardware display
            panelDisplay.checkPixel(flag, i, j);
        }
    }
}

//Event table
BEGIN_EVENT_TABLE(MyCanvas, wxPanel)
//Paint event
EVT_PAINT  (MyCanvas::OnPaint)
//Keyboard character input
EVT_KEY_DOWN   (MyCanvas::KeyDown)
END_EVENT_TABLE()

//Define the MyFrame class that displays the frame in which the drawing occurs
class MyFrame : public wxFrame
{
public:
    //Constructor
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, LED_Matrix *frameDisplayPtr);
    //Method that occurs when frame is closed by user
    void OnClose(wxCloseEvent& event);

private:
    //LED_Matrix object controls the hardware matrix display
    LED_Matrix frameDisplay;
    //Event table
    DECLARE_EVENT_TABLE()
};

//Define default constructor, sets LED_Matrix object
MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, LED_Matrix *frameDisplayPtr)
    : wxFrame(NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE, wxT("1"))
{
    frameDisplay = *frameDisplayPtr;
}

//Controls logic for shutting down hardware LED matrix and
//destroying the displayed frame when the user closes the app
void MyFrame::OnClose(wxCloseEvent& event)
{
    frameDisplay.FT_Close();
    Destroy();
}

//Event table
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
//Close event
EVT_CLOSE (MyFrame::OnClose)
END_EVENT_TABLE()

//Define the MyApp class that handles the logic initializing and ending the frame and canvas objects
class MyApp : public wxApp
{
public:
    //Initialization method
    virtual bool OnInit();
private:
    //MyApp contains frame, canvas, and LED_Matrix objects
    //Both MyFrame and MyCanvas objects manipulate the same
    //LED_Matrix object
    MyFrame* frame;
    MyCanvas* canvas1;
    LED_Matrix display;
};

//Event occurs at program startup to initizailze MyApp object
wxIMPLEMENT_APP(MyApp);

//Initializes the program
bool MyApp::OnInit()
{
    //Create new MyFrame and MyCanvas objects
    frame = new MyFrame(wxT("LED Array"), wxPoint(), wxSize(WINDOW_RES_X, WINDOW_RES_Y), &display);
    canvas1 = new MyCanvas(frame, 10, 10, wxPoint(0,0), wxSize(WINDOW_RES_X, WINDOW_RES_Y), &display);
    //Display frame
    frame->Show(true);
    return true;
}
