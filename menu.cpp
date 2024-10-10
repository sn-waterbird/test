#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/filedlg.h>  // For wxFileDialog
#include <wx/msgdlg.h>   // For wxMessageBox

// MyApp class
enum
{
    Show_Shaped = 100//用于绑定显示图片事件
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

    // Create a toolbar
    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddTool(1001, "Import Image", wxArtProvider::GetBitmap(wxART_FILE_OPEN));
    toolbar->AddTool(1005, "Save", wxArtProvider::GetBitmap(wxART_FILE_SAVE));
    toolbar->AddTool(1006, "Undo", wxArtProvider::GetBitmap(wxART_UNDO));
    toolbar->Realize();

    // Create a menu
    wxMenu* menuFile = new wxMenu;
    wxMenu* menuEXIT = new wxMenu;
    wxMenu* showMenu = new wxMenu;


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
    menuFile->AppendSubMenu(menuImport, "&Import");

    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT, "&About\tCtrl-A", "About this program");

    // Add the quit submenu to the EXIT menu
    menuEXIT->Append(wxID_EXIT, "E&xit\tCtrl-Q", "Exit the application");

    //添加显示图片功能
    showMenu->Append(Show_Shaped, wxT("&Show the shape\tS"));

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuEXIT, "&Exit");
    menuBar->Append(showMenu, wxT("Show"));
    SetMenuBar(menuBar);

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

#pragma endregion
