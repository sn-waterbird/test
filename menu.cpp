#include <wx/wx.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/filedlg.h>  // For wxFileDialog
#include <wx/msgdlg.h>   // For wxMessageBox

// MyApp class
class MyApp : public wxApp {
public:
    virtual bool OnInit();
};

// MyFrame class
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
    void OnClose();
    wxDECLARE_EVENT_TABLE();
};

// Event table
wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(1001, MyFrame::OnImportImage)
EVT_MENU(1002, MyFrame::OnImportJPEG)
EVT_MENU(1003, MyFrame::OnImportPNG)
EVT_MENU(1004, MyFrame::OnImportGIF)
EVT_MENU(1005, MyFrame::OnSave)
EVT_MENU(1006, MyFrame::OnUndo)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
wxEND_EVENT_TABLE()

// Application implementation
wxIMPLEMENT_APP(MyApp);

//tittle命名
bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame("genshin");
    frame->Show(true);
    return true;
}

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

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    menuBar->Append(menuEXIT, "&Exit");
    SetMenuBar(menuBar);
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


