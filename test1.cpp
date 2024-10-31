#include <wx/wx.h>
#include <wx/window.h>
#include "wx/wxprec.h"
#include <wx/animate.h>

enum
{
	Show_Shaped = 100//用于绑定显示图片事件
};

#pragma region 类声明
/// <summary>
/// 应用类
/// </summary>
class MyApp :public wxApp
{
public:
	virtual bool OnInit();
};

/// <summary>
/// 窗口类
/// </summary>
class MyFrame :public wxFrame
{
public:
	MyFrame(const wxString& title);
	void OnQuit(wxCommandEvent& event);
	void OnAbout(wxCommandEvent& event);
	//void OnSize(wxSizeEvent& event);
	void OnShowShaped(wxCommandEvent& event);
	void OnButtonOk(wxCommandEvent& event);

private:
	//声明事件表
	DECLARE_EVENT_TABLE();
};

/// <summary>
/// 图形有关窗口
/// </summary>
class ShapedFrame :public wxFrame
{
public:
	ShapedFrame(wxFrame *parent);
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

DECLARE_APP(MyApp);
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
	MyFrame* myFrame = new MyFrame(wxT("hello"));
	wxInitAllImageHandlers();
	//动态绑定事件
	//myFrame->Connect(wxID_ABOUT, wxEVT_COMMAND_MENU_SELECTED,wxCommandEventHandler(MyFrame::OnAbout));
	myFrame->Show(true);
	return true;
}

#pragma region 事件表
/// <summary>
/// MyFrame类的事件表
/// </summary>
BEGIN_EVENT_TABLE(MyFrame, wxFrame)
EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
EVT_MENU(wxID_EXIT, MyFrame::OnQuit)
EVT_MENU(Show_Shaped,MyFrame::OnShowShaped)
EVT_SIZE(MyFrame::OnSize)
EVT_BUTTON(wxID_OK, MyFrame::OnButtonOk)
END_EVENT_TABLE()

/// <summary>
///ShapedFrame事件表
/// </summary>
BEGIN_EVENT_TABLE(ShapedFrame, wxFrame)
EVT_MOTION(ShapedFrame::OnMouseMove)
EVT_PAINT(ShapedFrame::OnPaint)
EVT_LEFT_DOWN(ShapedFrame::OnLeftDown)
EVT_LEFT_UP(ShapedFrame::OnLeftUp)
EVT_RIGHT_UP(ShapedFrame::OnExit)
END_EVENT_TABLE()
#pragma endregion

#pragma region MyFrame
MyFrame::MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title)
{
	SetIcon(wxIcon("modrian.xpm"));

	//设置菜单
	wxMenu* showMenu = new wxMenu;
	wxMenu* fileMenu = new wxMenu;
	wxMenu* helpMenu = new wxMenu;

	helpMenu->Append(wxID_ABOUT, wxT("&About...\tQ"), wxT("Show About Dialogue"));
	fileMenu->Append(wxID_EXIT, wxT("&Exit...\tAlt-X"), wxT("Quit this programme"));
	showMenu->Append(Show_Shaped, wxT("&Show the shape\tS"));

	//菜单条
	wxMenuBar* bar = new wxMenuBar;

	bar->Append(helpMenu, wxT("File"));
	bar->Append(fileMenu, wxT("Help"));
	bar->Append(showMenu, wxT("Show"));

	SetMenuBar(bar);

	//状态条
	CreateStatusBar(2);
	int widths[2] = { 200,-1 };
	SetStatusWidths(2, widths);
	SetStatusText(wxT("Welcome to wxWidgets"), 0);

	//按钮
	wxButton* newButton = new wxButton(this, wxID_OK, wxT("Click Me"), wxPoint(10, 10), wxSize(150, 30));
	newButton->Bind(wxEVT_BUTTON, &MyFrame::OnButtonOk, this);

	//创建窗口
	wxWindow* newWindow = new wxWindow(this, wxID_ANY, wxPoint(100, 100), wxSize(200, 200));

	
}

void MyFrame::OnQuit(wxCommandEvent& event)
{
	Close();
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
	wxString str;
	//显示的字符
	str.Printf(wxT("Hello and welcome to %s"), wxVERSION_STRING);
	wxMessageBox(str, wxT("About UESTC"), wxOK | wxICON_INFORMATION, this);
}

void MyFrame::OnShowShaped(wxCommandEvent& event)
{
	ShapedFrame* frame = new ShapedFrame(this);
	frame->Show();
}

//void MyFrame::OnSize(wxSizeEvent& event)
//{
//	int currentHeight, currentWidth;
//	this->GetSize(&currentWidth, &currentHeight);
//
//	int newWidth = currentWidth * 2;
//	int newHeight = currentHeight * 2;
//	wxSize* newSize = new wxSize(newWidth, newHeight);
//
//	this->SetMaxSize(*newSize);
//}

void MyFrame::OnButtonOk(wxCommandEvent& event)
{
	wxMessageBox("Button Clicked", "Button Event", wxOK | wxICON_INFORMATION, this);
}
#pragma endregion

#pragma region ShapedFrame
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

