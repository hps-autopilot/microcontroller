// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/timer.h>
#include <attitude_indicator.h>
#include <LED_Matrix.h>
#include <data_reader.h>
#include <string>
using std::string;
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#define _MSC_VER 1924



#define SIZE     100
int TIME = 80; //Time in milliseconds between data updates

class MyCanvas : public wxPanel
{
public:
    MyCanvas(wxFrame* parent, int x_in, int y_in, const wxPoint& pos,
              const wxSize& size, LED_Matrix *panelDisplayPtr);
    void OnPaint(wxPaintEvent& event);
    void KeyDown(wxKeyEvent& event);
    void OnTimer(wxTimerEvent& event);
private:
    wxTimer timer;
    int x, y, ledDim;
    float roll, pitch, yaw;
    data_reader sensorData;
    LED_Matrix panelDisplay;
    attitude_indicator ai;
    DECLARE_EVENT_TABLE()
};

MyCanvas::MyCanvas(wxFrame* parent, int x_in, int y_in, const wxPoint& pos,
                    const wxSize& size, LED_Matrix *panelDisplayPtr) 
    : wxPanel(parent, -1, pos, size), timer(this, -1)
{
    panelDisplay = *panelDisplayPtr;
    panelDisplay.FT_Init();
    panelDisplay.calibrateMatrix();
    string fileLocation = "C:/Users/tilt7/AppData/Local/Packages/Microsoft.BluetoothLEExplorer_j38ftamvfgtc8/LocalState/test.txt";
    sensorData.setFile(fileLocation);
    ledDim = 8;
    ai.setDim(ledDim);
    x = x_in;
    y = y_in;
    timer.Start(TIME);
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

void MyCanvas::OnTimer(wxTimerEvent& event)
    
{
    float degreesRoll, degreesPitch;
    double pitchSensitivity = 6;
    double rollSensitivity  = 6;
    sensorData.read(&roll, &pitch, &yaw);
    if (!roll == 0 || !pitch == 0)
    {
        degreesRoll = roll * TIME/1000;
        degreesPitch = pitch * TIME/1000;
        
        if (degreesRoll < -1*rollSensitivity)
        {
            ai.rollLeft();
        }
        else if (degreesRoll > rollSensitivity)
        {
            ai.rollRight();
        }
        if (degreesPitch > pitchSensitivity)
        {
            ai.pitchDown();
        }
        else if (degreesPitch < -1*pitchSensitivity)
        {
            ai.pitchUp();
        }
        
    }
    Refresh();
    Update();
}

void MyCanvas::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.SetPen(*wxBLACK);
    bool flag;
    int i, j;
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
            dc.DrawRectangle(SIZE*j, SIZE *i, SIZE, SIZE);
            panelDisplay.checkPixel(flag, i, j);
        }
    }
}


BEGIN_EVENT_TABLE(MyCanvas, wxPanel)
EVT_PAINT  (MyCanvas::OnPaint)
EVT_KEY_DOWN   (MyCanvas::KeyDown)
EVT_TIMER(-1, MyCanvas::OnTimer)
END_EVENT_TABLE()

class MyFrame : public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, LED_Matrix *frameDisplayPtr);
    void OnClose(wxCloseEvent& event);

private:
    LED_Matrix frameDisplay;
    DECLARE_EVENT_TABLE()
};

MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, LED_Matrix *frameDisplayPtr)
    : wxFrame(NULL, -1, title, pos, size, wxDEFAULT_FRAME_STYLE, wxT("1"))
{
    frameDisplay = *frameDisplayPtr;
}

void MyFrame::OnClose(wxCloseEvent& event)
{
    frameDisplay.FT_Close();
    Destroy();
}

BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_CLOSE (MyFrame::OnClose)
END_EVENT_TABLE()

class MyApp : public wxApp
{
public:
    virtual bool OnInit();
private:
    MyFrame* frame;
    MyCanvas* canvas1;
    LED_Matrix display;
};

wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
    frame = new MyFrame(wxT("LED Array"), wxPoint(), wxSize(2500,1800), &display);
    canvas1 = new MyCanvas(frame, 10, 10, wxPoint(0,0), wxSize(2500, 1800), &display);
    frame->Show(true);
    return true;
}
