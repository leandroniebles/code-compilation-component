#include <iostream>
#include <wx/glcanvas.h>
#include <wx/dcclient.h>
#include "Events.h"
#include "Draw.h"
#include "Global.h"
#include "Textures.h"
#include "Canvas.h"
static int gl_attrib[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0};
using namespace std;

Canvas *canvas=NULL;

BEGIN_EVENT_TABLE(Canvas, wxGLCanvas)
	EVT_SIZE(Canvas::OnSize)
	EVT_PAINT(Canvas::OnPaint)
	EVT_LEFT_DOWN(Canvas::OnMouseLeftDown)
	EVT_LEFT_UP(Canvas::OnMouseLeftUp)
	EVT_RIGHT_DOWN(Canvas::OnMouseRightDown)
	EVT_RIGHT_UP(Canvas::OnMouseRightUp)
	EVT_MIDDLE_DOWN(Canvas::OnMouseMiddleDown)
	EVT_MIDDLE_UP(Canvas::OnMouseMiddleUp)
	EVT_LEFT_DCLICK(Canvas::OnMouseDClick)
	EVT_MOTION(Canvas::OnMouseMove)
	EVT_ERASE_BACKGROUND(Canvas::OnEraseBackground)
	EVT_TIMER(wxID_ANY, Canvas::OnRedrawTime)
	EVT_MOUSEWHEEL(Canvas::OnMouseWheel)
	EVT_IDLE(Canvas::OnIdle)
	EVT_CHAR(Canvas::OnChar)
	EVT_KEY_DOWN(Canvas::OnKeyDown)
	EVT_KEY_UP(Canvas::OnKeyUp)
END_EVENT_TABLE()
	
Canvas::Canvas(wxWindow *parent) : wxGLCanvas(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize,wxFULL_REPAINT_ON_RESIZE,"canvas",gl_attrib) {
	canvas=this;
	redraw_timer = new wxTimer(GetEventHandler(),wxID_ANY);
	mouse_buttons=modifiers=0;
}

Canvas::~Canvas() {
	
}

void Canvas::OnPaint(wxPaintEvent& event) {
	if (!IsShown()) return;
	wxGLCanvas::SetCurrent();
	wxPaintDC(this);
	static wxCursor *cursores=NULL;
	static CURSORES old_cursor=Z_CURSOR_COUNT;
	if (!cursores) {
		if(Entity::shape_colors)
			glClearColor(color_back_alt[0],color_back_alt[1],color_back_alt[2],1.f);
		else
			glClearColor(color_back[0],color_back[1],color_back[2],1.f);
		glDisable(GL_DEPTH);
		int win_w,win_h;
		GetClientSize(&win_w, &win_h);
		reshape_cb(win_w,win_h);
		Texture::LoadTextures();
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
		cursores=new wxCursor[Z_CURSOR_COUNT];
		cursores[Z_CURSOR_INHERIT]=wxCursor(wxCURSOR_ARROW);
		cursores[Z_CURSOR_CROSSHAIR]=wxCursor(wxCURSOR_CROSS);
		cursores[Z_CURSOR_HAND]=wxCursor(wxCURSOR_HAND);
		cursores[Z_CURSOR_TEXT]=wxCursor(wxCURSOR_IBEAM);
		cursores[Z_CURSOR_DESTROY]=wxCursor(wxCURSOR_NO_ENTRY);
		cursores[Z_CURSOR_NONE]=wxCursor(wxCURSOR_SIZING);
		cursores[Z_CURSOR_MOVE]=wxCursor(wxCURSOR_SIZING);
	}
	glLineStipple(zoom,0x0707);
	
	display_cb();
	if (old_cursor!=mouse_cursor) wxSetCursor(cursores[mouse_cursor]);
	glFlush();
	SwapBuffers();
}

void Canvas::OnSize(wxSizeEvent& event) {
	wxGLCanvas::OnSize(event);
	int win_w,win_h;
	GetClientSize(&win_w, &win_h);
	reshape_cb(win_w,win_h);
	Refresh();
}

void Canvas::OnRedrawTime(wxTimerEvent &evt) {
	Refresh();
}

void Canvas::OnEraseBackground(wxEraseEvent& event) {
	
}

void Canvas::OnMouseMove(wxMouseEvent& event) {
	if (mouse_buttons)
		motion_cb(event.GetX(),event.GetY());
	else
		passive_motion_cb(event.GetX(),event.GetY());
}

void Canvas::OnMouseMiddleDown(wxMouseEvent& event) {
	mouse_buttons|=MOUSE_MIDDLE;
	mouse_cb(ZMB_MIDDLE,ZMB_DOWN,event.GetX(),event.GetY());
}

void Canvas::OnMouseMiddleUp(wxMouseEvent& event) {
	mouse_cb(ZMB_MIDDLE,ZMB_UP,event.GetX(),event.GetY());
	mouse_buttons&=~MOUSE_MIDDLE;
}

void Canvas::OnMouseWheel(wxMouseEvent& event) {
	if (event.GetWheelRotation()<0)
		mouse_cb(ZMB_WHEEL_UP,ZMB_DOWN,event.GetX(),event.GetY());
	else
		mouse_cb(ZMB_WHEEL_DOWN,ZMB_DOWN,event.GetX(),event.GetY());
}

void Canvas::OnMouseRightDown (wxMouseEvent & event) {
	mouse_buttons|=MOUSE_RIGHT;
	mouse_cb(ZMB_RIGHT,ZMB_DOWN,event.GetX(),event.GetY());
}

void Canvas::OnMouseRightUp (wxMouseEvent & event) {
	mouse_cb(ZMB_RIGHT,ZMB_UP,event.GetX(),event.GetY());
	mouse_buttons&=~MOUSE_RIGHT;
}

void Canvas::OnMouseLeftDown (wxMouseEvent & event) {
	mouse_buttons|=MOUSE_LEFT;
	mouse_cb(ZMB_LEFT,ZMB_DOWN,event.GetX(),event.GetY());
}

void Canvas::OnMouseLeftUp (wxMouseEvent & event) {
	mouse_cb(ZMB_LEFT,ZMB_UP,event.GetX(),event.GetY());
	mouse_buttons&=~MOUSE_LEFT;
}

void Canvas::OnIdle (wxIdleEvent & event) {
	idle_func();
}

void Canvas::OnKeyDown (wxKeyEvent & event) {
	int key=event.GetKeyCode();
	switch (key) {
	case WXK_SHIFT: modifiers|=MODIFIER_SHIFT; break;
	case WXK_ALT: modifiers|=MODIFIER_ALT; break;
	case WXK_CONTROL: modifiers|=MODIFIER_CTRL; break;
	default: 
		if (key>=WXK_F1&&key<=WXK_F12) keyboard_esp_cb(key);
		else if (key==WXK_LEFT||key==WXK_RIGHT||key==WXK_END||key==WXK_HOME) keyboard_esp_cb(key);
		else event.Skip();
	}
}

void Canvas::OnKeyUp (wxKeyEvent & event) {
	switch (event.GetKeyCode()) {
	case WXK_SHIFT: modifiers&=~MODIFIER_SHIFT; break;
	case WXK_ALT: modifiers&=~MODIFIER_ALT; break;
	case WXK_CONTROL: modifiers&=~MODIFIER_CTRL; break;
	default: event.Skip();
	}
}

void Canvas::OnChar (wxKeyEvent & event) {
	keyboard_cb(event.GetKeyCode());
}

void Canvas::OnMouseDClick (wxMouseEvent & event) {
	mouse_dcb(event.GetX(),event.GetY());
}

void Canvas::SetModifiers (unsigned int mods) {
	modifiers = mods;
}

