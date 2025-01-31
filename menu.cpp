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
    void DrawGrid(wxMemoryDC& dc); // 新增：绘制网格
    void UpdateCanvas();

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

    const int GRID_SIZE = 20; // 网格间距
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

    wxBitmap m_bitmap;
    wxPoint m_delta;
    bool isClicked;

private:
    DECLARE_EVENT_TABLE();

    enum Shape_Kind
    {
        Shape_None,
        Shape_Star
    }m_shapeKind;
};
#pragma endregion

#pragma region DrawFrame
//class DrawFrame : public wxFrame {
//public:
//    DrawFrame(const wxString& title);
//
//private:
//    void OnImportCAD(wxCommandEvent& event);
//    void OnSave(wxCommandEvent& event);
//    void OnExportPDF(wxCommandEvent& event);
//    void OnClearCanvas(wxCommandEvent& event);
//    void OnDrawRectangle(wxCommandEvent& event);
//    void OnDrawCircle(wxCommandEvent& event);
//    void OnDrawLine(wxCommandEvent& event);
//    void OnUndo(wxCommandEvent& event);
//    void OnAbout(wxCommandEvent& event);
//    void OnQuit(wxCommandEvent& event);
//
//    void OnPaint(wxPaintEvent& event);
//    void OnMouseDown(wxMouseEvent& event);
//    void OnMouseMove(wxMouseEvent& event);
//    void OnMouseUp(wxMouseEvent& event);
//    void DrawShapes(wxMemoryDC& dc);
//
//    wxDECLARE_EVENT_TABLE();
//
//    wxBitmap m_canvas;
//    wxPoint m_startPoint;
//    wxPoint m_endPoint;
//    bool m_drawing;
//    bool m_drawCircle;
//    bool m_drawLine;
//    bool m_drawRectangle;
//
//    struct Shape {
//        wxPoint start;
//        wxPoint end;
//        bool isCircle;
//        bool isRectangle;
//    };
//
//    std::vector<Shape> m_shapes;
//};

#pragma endregion

#pragma region SizerPanel
class SizerPanel :public wxPanel
{
public:
    SizerPanel(wxBoxSizer* sizer, wxPanel* panel, wxFrame* parent);
    void ButtonAnd(wxCommandEvent& event, wxFrame* frame);
    void ButtonOr(wxCommandEvent& event, wxFrame* frame);
    void ButtonNot(wxCommandEvent& event, wxFrame* frame);
private:
    DECLARE_EVENT_TABLE();

};
#pragma endregion


#pragma endregion


#pragma region EventTable

#pragma region MyFrameEvent

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(1001, MyFrame::OnImportCAD)
EVT_MENU(1002, MyFrame::OnSave)
EVT_MENU(1003, MyFrame::OnExportPDF)
EVT_MENU(1004, MyFrame::OnClearCanvas)
EVT_MENU(1005, MyFrame::OnDrawRectangle)
EVT_MENU(1006, MyFrame::OnDrawCircle)
EVT_MENU(1007, MyFrame::OnDrawLine)
EVT_MENU(1008, MyFrame::OnUndo)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
EVT_PAINT(MyFrame::OnPaint)
EVT_LEFT_DOWN(MyFrame::OnMouseDown)
EVT_LEFT_UP(MyFrame::OnMouseUp)
EVT_MOTION(MyFrame::OnMouseMove)

//EVT_MENU(Show_Shaped, MyFrame::OnShowShaped)
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

#pragma region SizerPanelEvent
wxBEGIN_EVENT_TABLE(SizerPanel, wxPanel)
END_EVENT_TABLE()
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

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title), m_canvas(1200, 800), m_drawing(false), m_drawCircle(false), m_drawLine(false), m_drawRectangle(false) {

    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddTool(1001, "Import CAD", wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    toolbar->AddTool(1002, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE));
    toolbar->AddTool(1003, "Export to PDF", wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS));
    toolbar->AddTool(1004, "Clear Canvas", wxArtProvider::GetBitmap(wxART_DELETE));
    toolbar->AddTool(1005, "Draw Rectangle", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1006, "Draw Circle", wxArtProvider::GetBitmap(wxART_GO_DOWN));
    toolbar->AddTool(1007, "Draw Line", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1008, "Undo", wxArtProvider::GetBitmap(wxART_UNDO));
    toolbar->Realize();

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(1001, "Import CAD File\tCtrl-I", "Import CAD format files");
    menuFile->Append(1002, "Save\tCtrl-S", "Save the project");
    menuFile->Append(1003, "Export to PDF\tCtrl-E", "Export the project as a PDF");
    menuFile->Append(1004, "Clear Canvas\tCtrl-L", "Clear the drawing canvas");
    menuFile->Append(1005, "Draw Rectangle\tCtrl-R", "Draw a rectangle on the canvas");
    menuFile->Append(1006, "Draw Circle\tCtrl-C", "Draw a circle on the canvas");
    menuFile->Append(1007, "Draw Line\tCtrl-L", "Draw a line on the canvas");
    menuFile->Append(1008, "Undo\tCtrl-Z", "Undo the last drawing operation");
    menuFile->Append(wxID_EXIT, "Exit\tCtrl-Q", "Close the application");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "About\tCtrl-A", "Information about this application");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "File");
    menuBar->Append(menuHelp, "Help");
    SetMenuBar(menuBar);


#pragma region 创建侧边栏
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // 创建侧边栏
    wxPanel* sidePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 600));
    wxBoxSizer* sideSizer = new wxBoxSizer(wxVERTICAL);
    SizerPanel* sizerPanel = new SizerPanel(sideSizer, sidePanel, this);
    sidePanel->SetSizer(sideSizer);

    // 将侧边栏与主要内容部分连接
    mainSizer->Add(sidePanel, 0, wxEXPAND);

    SetSizerAndFit(mainSizer);

    SetSize(1000, 1000);
#pragma endregion
}

void MyFrame::OnImportCAD(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open CAD File", "", "",
        "CAD files (*.dwg;*.dxf)|*.dwg;*.dxf",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = openFileDialog.GetPath();
        wxMessageBox("Imported CAD file: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnSave(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save File", "", "",
        "Project files (*.proj)|*.proj",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveFileDialog.GetPath();
        wxMessageBox("Saved project to: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnExportPDF(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Export to PDF", "", "",
        "PDF files (*.pdf)|*.pdf",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveFileDialog.GetPath();
        wxMessageBox("Exported to PDF: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnClearCanvas(wxCommandEvent& event) {
    m_shapes.clear();
    wxMemoryDC dc(m_canvas);
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();

    Refresh();
    wxMessageBox("Canvas cleared!", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnDrawRectangle(wxCommandEvent& event) {
    m_drawRectangle = true;
    m_drawCircle = false;
    m_drawLine = false;
    wxMessageBox("Click and drag to draw a rectangle on the canvas.", "Draw Rectangle", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnDrawCircle(wxCommandEvent& event) {
    m_drawCircle = true;
    m_drawLine = false;
    m_drawRectangle = false;
    wxMessageBox("Click and drag to draw a circle on the canvas.", "Draw Circle", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnDrawLine(wxCommandEvent& event) {
    m_drawLine = true;
    m_drawCircle = false;
    m_drawRectangle = false;
    wxMessageBox("Click to set start and end points for the line.", "Draw Line", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnUndo(wxCommandEvent& event) {
    if (!m_shapes.empty()) {
        m_shapes.pop_back();
        UpdateCanvas();
    }
    else {
        wxMessageBox("No shapes to undo!", "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.DrawBitmap(m_canvas, 0, 0, true);
}

void MyFrame::OnMouseDown(wxMouseEvent& event) {
    if (event.LeftIsDown()) {
        m_startPoint = event.GetPosition();
        m_drawing = true;
    }
}

void MyFrame::OnMouseMove(wxMouseEvent& event) {
    if (m_drawing) {
        m_endPoint = event.GetPosition();
        UpdateCanvas(); // Update the canvas with the current shape
    }
}

void MyFrame::OnMouseUp(wxMouseEvent& event) {
    if (m_drawing) {
        m_drawing = false;

        // 对齐到网格
        m_startPoint.x = (m_startPoint.x / GRID_SIZE) * GRID_SIZE;
        m_startPoint.y = (m_startPoint.y / GRID_SIZE) * GRID_SIZE;
        m_endPoint.x = (m_endPoint.x / GRID_SIZE) * GRID_SIZE;
        m_endPoint.y = (m_endPoint.y / GRID_SIZE) * GRID_SIZE;

        Shape shape = { m_startPoint, m_endPoint, m_drawCircle, m_drawRectangle };

        if (m_drawLine) {
            shape.isCircle = false;
            shape.isRectangle = false;
        }

        m_shapes.push_back(shape);
        UpdateCanvas(); // Finalize the drawing
    }
}

void MyFrame::UpdateCanvas() {
    wxMemoryDC dc(m_canvas);
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();

    // 绘制网格
    DrawGrid(dc);

    // Draw previous shapes
    DrawShapes(dc);

    // Draw current temporary shape
    if (m_drawing) {
        dc.SetBrush(wxBrush(wxColor(0, 0, 255, 128))); // semi-transparent color
        dc.SetPen(wxPen(wxColor(0, 0, 255), 2));

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
    }

    Refresh();
}

void MyFrame::DrawShapes(wxMemoryDC& dc) {
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

void MyFrame::DrawGrid(wxMemoryDC& dc) {
    dc.SetPen(wxPen(wxColor(200, 200, 200), 1)); // 浅灰色网格线

    for (int x = 0; x < m_canvas.GetWidth(); x += GRID_SIZE) {
        dc.DrawLine(x, 0, x, m_canvas.GetHeight());
    }

    for (int y = 0; y < m_canvas.GetHeight(); y += GRID_SIZE) {
        dc.DrawLine(0, y, m_canvas.GetWidth(), y);
    }
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is an industrial design application for CAD management.", "About", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnQuit(wxCommandEvent& event) {
    Close(true);
}


//void MyFrame::OnShowShaped(wxCommandEvent& event)
//{
//    ShapedFrame* frame = new ShapedFrame(this);
//    frame->Show();
//}
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

#pragma region SizerFrame有关方法实现
SizerPanel::SizerPanel(wxBoxSizer* Sizer, wxPanel* Panel, wxFrame* parent)
    :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
    wxFrame* parentFrame = parent;
    // 在侧边栏中添加按钮
    wxButton* button1 = new wxButton(Panel, wxID_ANY, "AND");
    button1->Bind(wxEVT_BUTTON, [this, parentFrame](wxCommandEvent& event) {
        ButtonAnd(event, parentFrame); });
    Sizer->Add(button1, 0, wxEXPAND | wxALL, 5);

    wxButton* button2 = new wxButton(Panel, wxID_ANY, "Or");
    button2->Bind(wxEVT_BUTTON, [this, parentFrame](wxCommandEvent& event) {
        ButtonOr(event, parentFrame); });
    Sizer->Add(button2, 0, wxEXPAND | wxALL, 5);

    wxButton* button3 = new wxButton(Panel, wxID_ANY, "Not");
    button3->Bind(wxEVT_BUTTON, [this, parentFrame](wxCommandEvent& event) {
        ButtonNot(event, parentFrame); });
    Sizer->Add(button3, 0, wxEXPAND | wxALL, 5);
}

void SizerPanel::ButtonAnd(wxCommandEvent& event, wxFrame* parentFrame)
{
    /*ShapedFrame* frame = new ShapedFrame(parentFrame);
    frame->m_bitmap = wxBitmap("and.png", wxBITMAP_TYPE_PNG);
    frame->SetShape(wxRegion(frame->m_bitmap, *wxWHITE));
    frame->Show();*/
}

void SizerPanel::ButtonOr(wxCommandEvent& event, wxFrame* parentFrame)
{
    /*ShapedFrame* frame = new ShapedFrame(parentFrame);
    frame->m_bitmap = wxBitmap("or.png", wxBITMAP_TYPE_PNG);
    frame->SetShape(wxRegion(frame->m_bitmap, *wxWHITE));
    frame->Show();*/
}

void SizerPanel::ButtonNot(wxCommandEvent& event, wxFrame* parentFrame)
{
    /*ShapedFrame* frame = new ShapedFrame(parentFrame);
    frame->m_bitmap = wxBitmap("not.png", wxBITMAP_TYPE_PNG);
    frame->SetShape(wxRegion(frame->m_bitmap, *wxWHITE));
    frame->Show();*/
}
#pragma endregion

#pragma endregion#include <wx/wx.h>
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
    void OnImportCAD(wxCommandEvent& event);
    void OnSave(wxCommandEvent& event);
    void OnExportPDF(wxCommandEvent& event);
    void OnClearCanvas(wxCommandEvent& event);
    void OnDrawNot(wxCommandEvent& event);
    void OnDrawAnd(wxCommandEvent& event);
    void OnDrawOr(wxCommandEvent& event);
    void OnDrawLine(wxCommandEvent& event);
    void OnUndo(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);

    void OnPaint(wxPaintEvent& event);
    void OnMouseDown(wxMouseEvent& event);
    void OnMouseMove(wxMouseEvent& event);
    void OnMouseUp(wxMouseEvent& event);
    void DrawShapes(wxMemoryDC& dc);
    void DrawGrid(wxMemoryDC& dc); // 新增：绘制网格
    void UpdateCanvas();

    wxDECLARE_EVENT_TABLE();

    wxBitmap m_canvas;
    wxPoint m_startPoint;
    wxPoint m_endPoint;
    bool m_drawing;
    bool m_drawNot;
    bool m_drawLine;
    bool m_drawAnd;
    bool m_drawOr;

    struct Shape {
        wxPoint start;
        wxPoint end;
        bool isNot;
        bool isAnd;
        bool isOr;
    };

    std::vector<Shape> m_shapes;

    const int GRID_SIZE = 20; // 网格间距
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

    wxBitmap m_bitmap;
    wxPoint m_delta;
    bool isClicked;

private:
    DECLARE_EVENT_TABLE();

    enum Shape_Kind
    {
        Shape_None,
        Shape_Star
    }m_shapeKind;
};
#pragma endregion

#pragma region DrawFrame
//class DrawFrame : public wxFrame {
//public:
//    DrawFrame(const wxString& title);
//
//private:
//    void OnImportCAD(wxCommandEvent& event);
//    void OnSave(wxCommandEvent& event);
//    void OnExportPDF(wxCommandEvent& event);
//    void OnClearCanvas(wxCommandEvent& event);
//    void OnDrawRectangle(wxCommandEvent& event);
//    void OnDrawCircle(wxCommandEvent& event);
//    void OnDrawLine(wxCommandEvent& event);
//    void OnUndo(wxCommandEvent& event);
//    void OnAbout(wxCommandEvent& event);
//    void OnQuit(wxCommandEvent& event);
//
//    void OnPaint(wxPaintEvent& event);
//    void OnMouseDown(wxMouseEvent& event);
//    void OnMouseMove(wxMouseEvent& event);
//    void OnMouseUp(wxMouseEvent& event);
//    void DrawShapes(wxMemoryDC& dc);
//
//    wxDECLARE_EVENT_TABLE();
//
//    wxBitmap m_canvas;
//    wxPoint m_startPoint;
//    wxPoint m_endPoint;
//    bool m_drawing;
//    bool m_drawNot;
//    bool m_drawLine;
//    bool m_drawAnd;
//
//    struct Shape {
//        wxPoint start;
//        wxPoint end;
//        bool isNot;
//        bool isAnd;
//    };
//
//    std::vector<Shape> m_shapes;
//};

#pragma endregion

#pragma region SizerPanel
class SizerPanel :public wxPanel
{
public:
    SizerPanel(wxBoxSizer* sizer, wxPanel* panel, wxFrame* parent);
    void ButtonAnd(wxCommandEvent& event, wxFrame* frame);
    void ButtonOr(wxCommandEvent& event, wxFrame* frame);
    void ButtonNot(wxCommandEvent& event, wxFrame* frame);
private:
    DECLARE_EVENT_TABLE();

};
#pragma endregion


#pragma endregion


#pragma region EventTable

#pragma region MyFrameEvent

wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(1001, MyFrame::OnImportCAD)
EVT_MENU(1002, MyFrame::OnSave)
EVT_MENU(1003, MyFrame::OnExportPDF)
EVT_MENU(1004, MyFrame::OnClearCanvas)
EVT_MENU(1005, MyFrame::OnDrawAnd)
EVT_MENU(1006, MyFrame::OnDrawNot)
EVT_MENU(1007, MyFrame::OnDrawLine)
EVT_MENU(1008, MyFrame::OnDrawOr)
EVT_MENU(1009, MyFrame::OnUndo)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
EVT_PAINT(MyFrame::OnPaint)
EVT_LEFT_DOWN(MyFrame::OnMouseDown)
EVT_LEFT_UP(MyFrame::OnMouseUp)
EVT_MOTION(MyFrame::OnMouseMove)

//EVT_MENU(Show_Shaped, MyFrame::OnShowShaped)
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

#pragma region SizerPanelEvent
wxBEGIN_EVENT_TABLE(SizerPanel, wxPanel)
END_EVENT_TABLE()
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

MyFrame::MyFrame(const wxString& title)
    : wxFrame(NULL, wxID_ANY, title), m_canvas(1200, 800), m_drawing(false), m_drawNot(false), m_drawLine(false), m_drawAnd(false) {

    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddTool(1001, "Import CAD", wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    toolbar->AddTool(1002, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE));
    toolbar->AddTool(1003, "Export to PDF", wxArtProvider::GetBitmap(wxART_FILE_SAVE_AS));
    toolbar->AddTool(1004, "Clear Canvas", wxArtProvider::GetBitmap(wxART_DELETE));
    toolbar->AddTool(1005, "Draw Rectangle", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1006, "Draw Circle", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1007, "Draw Line", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1008, "Draw Or", wxArtProvider::GetBitmap(wxART_TIP));
    toolbar->AddTool(1009, "Undo", wxArtProvider::GetBitmap(wxART_UNDO));
    toolbar->Realize();

    wxMenu* menuFile = new wxMenu;
    menuFile->Append(1001, "Import CAD File\tCtrl-I", "Import CAD format files");
    menuFile->Append(1002, "Save\tCtrl-S", "Save the project");
    menuFile->Append(1003, "Export to PDF\tCtrl-E", "Export the project as a PDF");
    menuFile->Append(1004, "Clear Canvas\tCtrl-L", "Clear the drawing canvas");
    menuFile->Append(1005, "Draw Rectangle\tCtrl-R", "Draw a rectangle on the canvas");
    menuFile->Append(1006, "Draw Circle\tCtrl-C", "Draw a circle on the canvas");
    menuFile->Append(1007, "Draw Line\tCtrl-L", "Draw a line on the canvas");
    menuFile->Append(1008, "Undo\tCtrl-Z", "Undo the last drawing operation");
    menuFile->Append(wxID_EXIT, "Exit\tCtrl-Q", "Close the application");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "About\tCtrl-A", "Information about this application");

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "File");
    menuBar->Append(menuHelp, "Help");
    SetMenuBar(menuBar);


#pragma region 创建侧边栏
    wxBoxSizer* mainSizer = new wxBoxSizer(wxHORIZONTAL);

    // 创建侧边栏
    wxPanel* sidePanel = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(100, 600));
    wxBoxSizer* sideSizer = new wxBoxSizer(wxVERTICAL);
    SizerPanel* sizerPanel = new SizerPanel(sideSizer, sidePanel, this);
    sidePanel->SetSizer(sideSizer);

    // 将侧边栏与主要内容部分连接
    mainSizer->Add(sidePanel, 0, wxEXPAND);

    SetSizerAndFit(mainSizer);

    SetSize(1000, 1000);
#pragma endregion
}

void MyFrame::OnImportCAD(wxCommandEvent& event) {
    wxFileDialog openFileDialog(this, "Open CAD File", "", "",
        "CAD files (*.dwg;*.dxf)|*.dwg;*.dxf",
        wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = openFileDialog.GetPath();
        wxMessageBox("Imported CAD file: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnSave(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Save File", "", "",
        "Project files (*.proj)|*.proj",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveFileDialog.GetPath();
        wxMessageBox("Saved project to: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnExportPDF(wxCommandEvent& event) {
    wxFileDialog saveFileDialog(this, "Export to PDF", "", "",
        "PDF files (*.pdf)|*.pdf",
        wxFD_SAVE | wxFD_OVERWRITE_PROMPT);

    if (saveFileDialog.ShowModal() == wxID_OK) {
        wxString filePath = saveFileDialog.GetPath();
        wxMessageBox("Exported to PDF: " + filePath, "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnClearCanvas(wxCommandEvent& event) {
    m_shapes.clear();
    wxMemoryDC dc(m_canvas);
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();

    Refresh();
    wxMessageBox("Canvas cleared!", "Info", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnDrawAnd(wxCommandEvent& event) {
    m_drawAnd = true;
    m_drawNot = false;
    m_drawLine = false;
    m_drawOr = false;
    wxMessageBox("Click and drag to draw a And on the canvas.", "Draw And", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnDrawNot(wxCommandEvent& event) {
    m_drawNot = true;
    m_drawLine = false;
    m_drawAnd = false;
    m_drawOr = false;
    wxMessageBox("Click and drag to draw a Not on the canvas.", "Draw Not", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnDrawLine(wxCommandEvent& event) {
    m_drawLine = true;
    m_drawNot = false;
    m_drawAnd = false;
    m_drawOr = false;
    wxMessageBox("Click to set start and end points for the line.", "Draw Line", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnDrawOr(wxCommandEvent& event) {
    m_drawOr = true;
    m_drawLine = false;
    m_drawNot = false;
    m_drawAnd = false;
    wxMessageBox("Click and drag to draw a Or on the canvas.", "Draw Or", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnUndo(wxCommandEvent& event) {
    if (!m_shapes.empty()) {
        m_shapes.pop_back();
        UpdateCanvas();
    }
    else {
        wxMessageBox("No shapes to undo!", "Info", wxOK | wxICON_INFORMATION);
    }
}

void MyFrame::OnPaint(wxPaintEvent& event) {
    wxPaintDC dc(this);
    dc.DrawBitmap(m_canvas, 0, 0, true);
}

void MyFrame::OnMouseDown(wxMouseEvent& event) {
    if (event.LeftIsDown()) {
        m_startPoint = event.GetPosition();
        m_drawing = true;
    }
}

void MyFrame::OnMouseMove(wxMouseEvent& event) {
    if (m_drawing) {
        m_endPoint = event.GetPosition();
        UpdateCanvas(); // Update the canvas with the current shape
    }
}

void MyFrame::OnMouseUp(wxMouseEvent& event) {
    if (m_drawing) {
        m_drawing = false;

        // 对齐到网格
        m_startPoint.x = (m_startPoint.x / GRID_SIZE) * GRID_SIZE;
        m_startPoint.y = (m_startPoint.y / GRID_SIZE) * GRID_SIZE;
        m_endPoint.x = (m_endPoint.x / GRID_SIZE) * GRID_SIZE;
        m_endPoint.y = (m_endPoint.y / GRID_SIZE) * GRID_SIZE;

        Shape shape = { m_startPoint, m_endPoint, m_drawNot, m_drawAnd,m_drawOr };

        if (m_drawLine) {
            shape.isNot = false;
            shape.isAnd = false;
            shape.isOr = false;
        }

        m_shapes.push_back(shape);
        UpdateCanvas(); // Finalize the drawing
    }
}

void MyFrame::UpdateCanvas() {
    wxMemoryDC dc(m_canvas);
    dc.SetBackground(wxBrush(GetBackgroundColour()));
    dc.Clear();

    // 绘制网格
    DrawGrid(dc);

    // Draw previous shapes
    DrawShapes(dc);

    // Draw current temporary shape
    if (m_drawing) {
        wxPen pen(*wxBLUE);
        dc.SetPen(pen);

        // 创建一个透明的画刷
        wxBrush brush(*wxTRANSPARENT_BRUSH);
        dc.SetBrush(brush);

        if (m_drawNot) {
            wxPoint points[] = { wxPoint(m_startPoint.x,m_startPoint.y + 2 * GRID_SIZE),wxPoint(m_startPoint.x,m_startPoint.y + 4 * GRID_SIZE),wxPoint(m_startPoint.x + 2 * GRID_SIZE,m_startPoint.y + 3 * GRID_SIZE) };
            dc.DrawPolygon(3, points);
            dc.DrawCircle(wxPoint(points[2].x + GRID_SIZE / 2, points[2].y), GRID_SIZE / 2);
        }
        else if (m_drawLine) {
            dc.DrawLine(m_startPoint.x, m_startPoint.y + 2 * GRID_SIZE, m_endPoint.x, m_endPoint.y + 2 * GRID_SIZE);
        }
        else if (m_drawAnd) {
            dc.DrawRectangle(m_startPoint.x, m_startPoint.y + 2 * GRID_SIZE, 2 * GRID_SIZE, 4 * GRID_SIZE);
            dc.DrawEllipticArc(m_startPoint.x, m_startPoint.y + 2 * GRID_SIZE, 4 * GRID_SIZE, 4 * GRID_SIZE, -90, 90);

        }
        else if (m_drawOr)
        {
            dc.DrawEllipticArc(m_startPoint.x - 4 * GRID_SIZE, m_startPoint.y - 2 * GRID_SIZE, 8 * GRID_SIZE, 8 * GRID_SIZE, -90, -30);
            dc.DrawEllipticArc(m_startPoint.x - 4 * GRID_SIZE, m_startPoint.y + 2 * GRID_SIZE, 8 * GRID_SIZE, 8 * GRID_SIZE, 30, 90);
            dc.DrawEllipticArc(m_startPoint.x - 4 * GRID_SIZE, m_startPoint.y + 1.5 * GRID_SIZE, 5 * GRID_SIZE, 5 * GRID_SIZE, -53, 53);
        }
    }

    Refresh();
}

void MyFrame::DrawShapes(wxMemoryDC& dc) {
    wxPen pen(*wxBLUE, 2);
    dc.SetPen(pen);

    // 创建一个透明的画刷
    wxBrush brush(*wxTRANSPARENT_BRUSH);
    dc.SetBrush(brush);

    for (const auto& shape : m_shapes) {
        if (shape.isNot) {
            wxPoint points[] = { wxPoint(shape.start.x,shape.start.y + 2 * GRID_SIZE),wxPoint(shape.start.x,shape.start.y + 4 * GRID_SIZE),wxPoint(shape.start.x + 2 * GRID_SIZE,shape.start.y + 3 * GRID_SIZE) };
            dc.DrawPolygon(3, points);
            dc.DrawCircle(wxPoint(points[2].x + GRID_SIZE / 2, points[2].y), GRID_SIZE / 2);
        }
        else if (shape.isAnd) {
            dc.DrawRectangle(shape.start.x, shape.start.y + 2 * GRID_SIZE, 2 * GRID_SIZE, 4 * GRID_SIZE);
            dc.DrawEllipticArc(shape.start.x, shape.start.y + 2 * GRID_SIZE, 4 * GRID_SIZE, 4 * GRID_SIZE, -90, 90);
        }
        else if (shape.isOr)
        {
            dc.DrawEllipticArc(shape.start.x - 4 * GRID_SIZE, shape.start.y - 2 * GRID_SIZE, 8 * GRID_SIZE, 8 * GRID_SIZE, -90, -30);
            dc.DrawEllipticArc(shape.start.x - 4 * GRID_SIZE, shape.start.y + 2 * GRID_SIZE, 8 * GRID_SIZE, 8 * GRID_SIZE, 30, 90);
            dc.DrawEllipticArc(shape.start.x - 4 * GRID_SIZE, shape.start.y + 1.5 * GRID_SIZE, 5 * GRID_SIZE, 5 * GRID_SIZE, -53, 53);
        }
        else
        {
            dc.DrawLine(shape.start.x, shape.start.y + 2 * GRID_SIZE, shape.end.x, shape.end.y + 2 * GRID_SIZE);
        }
    }
}

void MyFrame::DrawGrid(wxMemoryDC& dc) {
    dc.SetPen(wxPen(wxColor(200, 200, 200), 1)); // 浅灰色网格线

    for (int x = 0; x < m_canvas.GetWidth(); x += GRID_SIZE) {
        dc.DrawLine(x, 0, x, m_canvas.GetHeight());
    }

    for (int y = 0; y < m_canvas.GetHeight(); y += GRID_SIZE) {
        dc.DrawLine(0, y, m_canvas.GetWidth(), y);
    }
}

void MyFrame::OnAbout(wxCommandEvent& event) {
    wxMessageBox("This is an industrial design application for CAD management.", "About", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnQuit(wxCommandEvent& event) {
    Close(true);
}


//void MyFrame::OnShowShaped(wxCommandEvent& event)
//{
//    ShapedFrame* frame = new ShapedFrame(this);
//    frame->Show();
//}
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

#pragma region SizerFrame有关方法实现
SizerPanel::SizerPanel(wxBoxSizer* Sizer, wxPanel* Panel, wxFrame* parent)
    :wxPanel(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize)
{
    wxFrame* parentFrame = parent;
    // 在侧边栏中添加按钮
    wxButton* button1 = new wxButton(Panel, wxID_ANY, "AND");
    button1->Bind(wxEVT_BUTTON, [this, parentFrame](wxCommandEvent& event) {
        ButtonAnd(event, parentFrame); });
    Sizer->Add(button1, 0, wxEXPAND | wxALL, 5);

    wxButton* button2 = new wxButton(Panel, wxID_ANY, "Or");
    button2->Bind(wxEVT_BUTTON, [this, parentFrame](wxCommandEvent& event) {
        ButtonOr(event, parentFrame); });
    Sizer->Add(button2, 0, wxEXPAND | wxALL, 5);

    wxButton* button3 = new wxButton(Panel, wxID_ANY, "Not");
    button3->Bind(wxEVT_BUTTON, [this, parentFrame](wxCommandEvent& event) {
        ButtonNot(event, parentFrame); });
    Sizer->Add(button3, 0, wxEXPAND | wxALL, 5);
}

void SizerPanel::ButtonAnd(wxCommandEvent& event, wxFrame* parentFrame)
{
    /*ShapedFrame* frame = new ShapedFrame(parentFrame);
    frame->m_bitmap = wxBitmap("and.png", wxBITMAP_TYPE_PNG);
    frame->SetShape(wxRegion(frame->m_bitmap, *wxWHITE));
    frame->Show();*/
}

void SizerPanel::ButtonOr(wxCommandEvent& event, wxFrame* parentFrame)
{
    /*ShapedFrame* frame = new ShapedFrame(parentFrame);
    frame->m_bitmap = wxBitmap("or.png", wxBITMAP_TYPE_PNG);
    frame->SetShape(wxRegion(frame->m_bitmap, *wxWHITE));
    frame->Show();*/
}

void SizerPanel::ButtonNot(wxCommandEvent& event, wxFrame* parentFrame)
{
    /*ShapedFrame* frame = new ShapedFrame(parentFrame);
    frame->m_bitmap = wxBitmap("not.png", wxBITMAP_TYPE_PNG);
    frame->SetShape(wxRegion(frame->m_bitmap, *wxWHITE));
    frame->Show();*/
}
#pragma endregion

#pragma endregion