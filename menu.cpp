#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/filedlg.h>  // For wxFileDialog
#include <wx/msgdlg.h>   // For wxMessageBox
#include <wx/dcclient.h>
#include <cmath>
#include <vector>

// MyApp class
enum
{
    Show_Shaped = 100,//用于绑定显示图片事件
    Show_Draw = 101//绑定画图功能
};

class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

#pragma region Frame

// MyFrame class
#pragma region MyFrame
class MyFrame : public wxFrame {
public:
    MyFrame(const wxString& title);

private:
    void OnImportImage(wxCommandEvent& event);
    void OnImportJPEG(wxCommandEvent& event);
    void OnImportPNG(wxCommandEvent& event);
    void OnImportGIF(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnShowShaped(wxCommandEvent& event);
    void OnShowDraw(wxCommandEvent& event);
    void OnButtonOk(wxCommandEvent& event);
    wxDECLARE_EVENT_TABLE();
};
#pragma endregion

#pragma region ShapedFrame
class ShapedFrame :public wxFrame
{
public:
    ShapedFrame(wxFrame* parent);
    void OnMouseMove(wxMouseEvent& event);
    void OnLeftUp(wxMouseEvent& event);
    void OnLeftDown(wxMouseEvent& event);
    void OnPaint(wxPaintEvent& event);
    void SetWindowShape();
    void OnExit(wxMouseEvent& event);

private:
    DECLARE_EVENT_TABLE();
    wxBitmap m_bitmap;
    wxPoint m_delta;

    enum Shape_Kind
    {
        Shape_None,
        Shape_Star
    }m_shapeKind;
};
#pragma endregion

#pragma region DrawFrame
class DrawFrame : public wxFrame {
public:
    DrawFrame(const wxString& title);

private:
    void OnImportCAD(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnExportPDF(wxCommandEvent& event);
    void OnClearCanvas(wxCommandEvent& event);
    void OnDrawRectangle(wxCommandEvent& event);
    void OnDrawCircle(wxCommandEvent& event);
    void OnDrawLine(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);

    void OnPaint(wxPaintEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void DrawShapes(wxMemoryDC& dc);

    wxDECLARE_EVENT_TABLE();

    wxBitmap m_canvas;
    wxPoint m_startPoint;
    wxPoint m_endPoint;
    bool m_drawing;
    bool m_drawCircle;
    bool m_drawLine;
    bool m_drawRectangle;

    struct Shape {
        wxPoint start;
        wxPoint end;
        bool isCircle;
        bool isRectangle;
    };

    std::vector<Shape> m_shapes;
};

#pragma endregion

#pragma endregion


#pragma region EventTable

#pragma region MyFrameEvent
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(1001, MyFrame::OnImportImage)
EVT_MENU(1002, MyFrame::OnImportJPEG)
EVT_MENU(1003, MyFrame::OnImportPNG)
EVT_MENU(1004, MyFrame::OnImportGIF)
EVT_MENU(1005, MyFrame::OnSave)
EVT_MENU(1006, MyFrame::OnUndo)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
EVT_MENU(Show_Shaped, MyFrame::OnShowShaped)
EVT_MENU(Show_Draw, MyFrame::OnShowDraw)
EVT_BUTTON(wxID_OK, MyFrame::OnButtonOk)
wxEND_EVENT_TABLE()
#pragma endregion

#pragma region ShapedFrameEvent
BEGIN_EVENT_TABLE(ShapedFrame, wxFrame)
EVT_MOTION(ShapedFrame::OnMouseMove)
EVT_PAINT(ShapedFrame::OnPaint)
EVT_LEFT_DOWN(ShapedFrame::OnLeftDown)
EVT_LEFT_UP(ShapedFrame::OnLeftUp)
EVT_RIGHT_UP(ShapedFrame::OnExit)
END_EVENT_TABLE()
#pragma endregion

#pragma region DrawFrameEvent
wxBEGIN_EVENT_TABLE(DrawFrame, wxFrame)
EVT_MENU(1001, DrawFrame::OnImportCAD)
EVT_MENU(1002, DrawFrame::OnSave)
EVT_MENU(1003, DrawFrame::OnExportPDF)
EVT_MENU(1004, DrawFrame::OnClearCanvas)
EVT_MENU(1005, DrawFrame::OnDrawRectangle)
EVT_MENU(1006, DrawFrame::OnDrawCircle)
EVT_MENU(1008, DrawFrame::OnDrawLine)
EVT_MENU(1007, DrawFrame::OnUndo)
EVT_MENU(wxID_ABOUT, DrawFrame::OnAbout)
EVT_MENU(wxID_EXIT, DrawFrame::OnQuit)
EVT_PAINT(DrawFrame::OnPaint)
EVT_LEFT_DOWN(DrawFrame::OnMouseDown)
EVT_LEFT_UP(DrawFrame::OnMouseUp)
EVT_MOTION(DrawFrame::OnMouseMove)
wxEND_EVENT_TABLE()
#pragma endregion


#pragma endregion

// Application implementation
wxIMPLEMENT_APP(MyApp);

//tittle命名
bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("genshin");
    wxInitAllImageHandlers();
    frame->Show(true);
    return true;
}


#pragma region 不同Frame对应方法实现

#pragma region MyFrame有关方法实现

// MyFrame constructor
MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title) {
#pragma region 创建toolbar
    // Create a toolbar
    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddTool(1001, "Import Image", wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    toolbar->AddTool(1005, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE));
    toolbar->AddTool(1006, "Undo", wxArtProvider::GetBitmap(wxART_UNDO));
#pragma endregion

#pragma region 创建menu与menubar

    // Create Import submenu
    wxMenu* menuImport = new wxMenu;
    menuImport->Append(1001, "&Import Image\tCtrl-I", "Import an image file");

    // Create submenu for image formats
    wxMenu* menuImageFormats = new wxMenu;
    menuImageFormats->Append(1002, "&JPEG", "Import JPEG format");
    menuImageFormats->Append(1003, "&PNG", "Import PNG format");
    menuImageFormats->Append(1004, "&GIF", "Import GIF format");

    // Add the image formats submenu to the Import Image option
    menuImport->AppendSubMenu(menuImageFormats, "&Image Formats");

    // Add the Import submenu to the File menu
    wxMenu* menuFile = new wxMenu;
    menuFile->AppendSubMenu(menuImport, "&Import");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "&About\tCtrl-A", "About this program");

    // Add the quit submenu to the EXIT menu
    wxMenu* menuEXIT = new wxMenu;
    menuEXIT->Append(wxID_EXIT, "E&xit\tCtrl-Q", "Exit the application");

    //添加显示图片功能
    wxMenu* showMenu = new wxMenu;
    showMenu->Append(Show_Shaped, wxT("&Show the shape\tS"));

    wxMenu* drawMenu = new wxMenu;
    drawMenu->Append(Show_Draw, wxT("Show the paintings"));


    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuEXIT, "&Exit");
    menuBar->Append(showMenu, wxT("Show"));
    menuBar->Append(drawMenu, "Draw");
    SetMenuBar(menuBar);
#pragma endregion

    //创建按钮
    wxPanel* panel = new wxPanel(this, wxID_ANY);
    wxButton* newButton = new wxButton(panel, wxID_OK, wxT("Click Me"), wxPoint(10, 10), wxSize(150, 30));
    newButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonOk, this);
}

// Event handler functions
void MyFrame::OnImportImage(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open Image file", "", "",
        "Image files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = openFileDialog.GetPath();
        wxMessageBox("Imported image: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnImportJPEG(wxCommandEvent& event) {
    wxMessageBox("Importing a JPEG file...", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnImportPNG(wxCommandEvent& event) {
    wxMessageBox("Importing a PNG file...", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnImportGIF(wxCommandEvent& event) {
    wxMessageBox("Importing a GIF file...", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnSave(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save File", "", "",
        "Image files (*.jpg;*.jpeg;*.png;*.gif)|*.jpg;*.jpeg;*.png;*.gif",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveFileDialog.GetPath();
        wxMessageBox("Saved file to: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnUndo(wxCommandEvent& event) {
    wxMessageBox("Undoing the last action...", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is a wxWidgets application with multiple toolbars.", "About", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnQuit(wxCommandEvent& event) {
    Close(true);
}

void MyFrame::OnShowShaped(wxCommandEvent& event)
{
    ShapedFrame* frame = new ShapedFrame(this);
    frame->Show();
}

void MyFrame::OnButtonOk(wxCommandEvent& event)
{
    wxMessageBox("Button Clicked", "Button Event", wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnShowDraw(wxCommandEvent& event)
{
    DrawFrame* frame = new DrawFrame("Industrial Design Tool");
    frame->Show();
}
#pragma endregion

#pragma region ShapedFrame有关方法实现
ShapedFrame::ShapedFrame(wxFrame* parent) :wxFrame(NULL, wxID_ANY, wxEmptyString, wxDefaultPosition, wxSize(250, 300), wxFRAME_SHAPED | wxSIMPLE_BORDER | wxFRAME_NO_TASKBAR | wxSTAY_ON_TOP)
{
    /*wxAnimationCtrl* an = new wxAnimationCtrl(this, wxID_ANY, wxAnimation(wxT("ddear.gif"), wxANIMATION_TYPE_ANY), wxDefaultPosition, wxDefaultSize, 2097152L, wxAnimationCtrlNameStr);
    an->Play();*/
    m_shapeKind = Shape_Star;
    m_bitmap = wxBitmap("star.png", wxBITMAP_TYPE_PNG);
    SetSize(wxSize(m_bitmap.GetWidth(), m_bitmap.GetHeight()));
    SetToolTip("Right Click To Close");
    SetWindowShape();
}

void ShapedFrame::SetWindowShape()
{
    switch (m_shapeKind)
    {
    case Shape_None:
        SetShape(wxRegion());
        break;

    case Shape_Star:
        SetShape(wxRegion(m_bitmap, *wxWHITE));
    }

}

void ShapedFrame::OnLeftDown(wxMouseEvent& evt)
{
    CaptureMouse();
    wxPoint pos = ClientToScreen(evt.GetPosition());
    wxPoint origin = GetPosition();
    int dx = pos.x - origin.x;
    int dy = pos.y - origin.y;
    m_delta = wxPoint(dx, dy);
}

void ShapedFrame::OnLeftUp(wxMouseEvent& WXUNUSED(evt))
{
    if (HasCapture())
    {
        ReleaseMouse();
    }
}

void ShapedFrame::OnMouseMove(wxMouseEvent& event)
{
    wxPoint pt = event.GetPosition();
    if (event.Dragging() && event.LeftIsDown())
    {
        wxPoint pos = ClientToScreen(pt);
        Move(wxPoint(pos.x - m_delta.x, pos.y - m_delta.y));
    }
}

void ShapedFrame::OnExit(wxMouseEvent& event)
{
    Close();
}

void ShapedFrame::OnPaint(wxPaintEvent& event)
{
    wxPaintDC dc(this);
    dc.DrawBitmap(m_bitmap, 0, 0, true);
}
#pragma endregion

#pragma region DrawFrame有关方法实现
DrawFrame::DrawFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title), m_canvas(800, 600), m_drawing(false), m_drawCircle(false), m_drawLine(false), m_drawRectangle(false) {
    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddTool(1001, "Import CAD", wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    toolbar->AddTool(1002, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE));
    toolbar->AddTool(1003, "Export to PDF", wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS));
    toolbar->AddTool(1004, "Clear Canvas", wxArtProvider::GetBitmap(wxART_DELETE));
    toolbar->AddTool(1005, "Draw Rectangle", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1006, "Draw Circle", wxArtProvider::GetBitmap(wxART_GO_DOWN));
    toolbar->AddTool(1008, "Draw Line", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1007, "Undo", wxArtProvider::GetBitmap(wxART_UNDO));
    toolbar->Realize();

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(1001, "Import CAD File\tCtrl-C", "Import CAD format files");
    menuFile->Append(1002, "Save\tCtrl-S", "Save the project");
    menuFile->Append(1003, "Export to PDF\tCtrl-E", "Export the project as a PDF");
    menuFile->Append(1004, "Clear Canvas\tCtrl-L", "Clear the drawing canvas");
    menuFile->Append(1005, "Draw Rectangle\tCtrl-R", "Draw a rectangle on the canvas");
    menuFile->Append(1006, "Draw Circle\tCtrl-C", "Draw a circle on the canvas");
    menuFile->Append(1008, "Draw Line\tCtrl-L", "Draw a line on the canvas");
    menuFile->Append(1007, "Undo\tCtrl-Z", "Undo the last drawing operation");
    menuFile->Append(wxID_EXIT, "Exit\tCtrl-Q", "Close the application");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "About\tCtrl-A", "Information about this application");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "File");
    menuBar->Append(menuHelp, "Help");
    SetMenuBar(menuBar);

}

void DrawFrame::OnImportCAD(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open CAD File", "", "",
        "CAD files (*.dwg;*.dxf)|*.dwg;*.dxf",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = openFileDialog.GetPath();
        wxMessageBox("Imported CAD file: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawFrame::OnSave(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save File", "", "",
        "Project files (*.proj)|*.proj",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveFileDialog.GetPath();
        wxMessageBox("Saved project to: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawFrame::OnExportPDF(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Export to PDF", "", "",
        "PDF files (*.pdf)|*.pdf",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveFileDialog.GetPath();
        wxMessageBox("Exported to PDF: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawFrame::OnClearCanvas(wxCommandEvent& event) {
    m_shapes.clear();
    wxMemoryDC dc(m_canvas);
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();

    Refresh();
    wxMessageBox("Canvas cleared!", "Info", wxOK | wxICON_INFORMATION);
}

void DrawFrame::OnDrawRectangle(wxCommandEvent& event) {
    m_drawRectangle = true;
    m_drawCircle = false;
    m_drawLine = false;
    wxMessageBox("Click and drag to draw a rectangle on the canvas.", "Draw Rectangle", wxOK | wxICON_INFORMATION);
}

void DrawFrame::OnDrawCircle(wxCommandEvent& event) {
    m_drawCircle = true;
    m_drawLine = false;
    m_drawRectangle = false;
    wxMessageBox("Click and drag to draw a circle on the canvas.", "Draw Circle", wxOK | wxICON_INFORMATION);
}

void DrawFrame::OnDrawLine(wxCommandEvent& event) {
    m_drawLine = true;
    m_drawCircle = false;
    m_drawRectangle = false;
    wxMessageBox("Click to set start and end points for the line.", "Draw Line", wxOK | wxICON_INFORMATION);
}

void DrawFrame::OnUndo(wxCommandEvent& event) {
    if (!m_shapes.empty()) {
        m_shapes.pop_back();
        wxMemoryDC dc(m_canvas);
        dc.SetBackground(wxBrush(GetBackgroundColour()));
        dc.Clear();
        DrawShapes(dc);
        Refresh();
    }
    else {
        wxMessageBox("No shapes to undo!", "Info", wxOK | wxICON_INFORMATION);
    }
}

void DrawFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.DrawBitmap(m_canvas, 0, 0, true);
}

void DrawFrame::OnMouseDown(wxMouseEvent& event) {
    if (event.LeftIsDown()) {
        m_startPoint = event.GetPosition();
        m_drawing = true;
    }
}

void DrawFrame::OnMouseMove(wxMouseEvent& event) {
    if (m_drawing) {
        m_endPoint = event.GetPosition();
        wxMemoryDC dc(m_canvas);
        dc.SetBrush(wxBrush(wxColor(0, 0, 255)));
        dc.SetPen(wxPen(wxColor(0, 0, 255), 2));

        dc.SetBackground(wxBrush(GetBackgroundColour()));
        dc.Clear();

        if (m_drawCircle) {
            int radius = static_cast<int>(sqrt(pow(m_endPoint.x - m_startPoint.x, 2) + pow(m_endPoint.y - m_startPoint.y, 2)));
            dc.DrawCircle(m_startPoint.x, m_startPoint.y, radius);
        }
        else if (m_drawLine) {
            dc.DrawLine(m_startPoint.x, m_startPoint.y, m_endPoint.x, m_endPoint.y);
        }
        else if (m_drawRectangle) {
            dc.DrawRectangle(m_startPoint.x, m_startPoint.y,
                m_endPoint.x - m_startPoint.x,
                m_endPoint.y - m_startPoint.y);
        }

        DrawShapes(dc);
        Refresh();
    }
}

void DrawFrame::OnMouseUp(wxMouseEvent& event) {
    if (m_drawing) {
        m_drawing = false;

        Shape shape = { m_startPoint, m_endPoint, m_drawCircle, m_drawRectangle };

        if (m_drawLine) {
            shape.isCircle = false;
            shape.isRectangle = false;
        }

        m_shapes.push_back(shape);
    }
}

void DrawFrame::DrawShapes(wxMemoryDC& dc) {
    dc.SetBrush(wxBrush(wxColor(0, 0, 255)));
    dc.SetPen(wxPen(wxColor(0, 0, 255), 2));

    for (const auto& shape : m_shapes) {
        if (shape.isCircle) {
            int radius = static_cast<int>(sqrt(pow(shape.end.x - shape.start.x, 2) + pow(shape.end.y - shape.start.y, 2)));
            dc.DrawCircle(shape.start.x, shape.start.y, radius);
        }
        else if (shape.isRectangle) {
            dc.DrawRectangle(shape.start.x, shape.start.y,
                shape.end.x - shape.start.x,
                shape.end.y - shape.start.y);
        }
        else {
            dc.DrawLine(shape.start.x, shape.start.y, shape.end.x, shape.end.y);
        }
    }
}

void DrawFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is an industrial design application for CAD management.", "About", wxOK | wxICON_INFORMATION);
}

void DrawFrame::OnQuit(wxCommandEvent& event) {
    Close(true);
}
#pragma endregion

#pragma endregion
