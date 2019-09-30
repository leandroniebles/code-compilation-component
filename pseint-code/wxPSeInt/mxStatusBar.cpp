#include <wx/dcclient.h>
#include "mxStatusBar.h"
#include <wx/textfile.h>
#include <wx/event.h>
#include "mxMainWindow.h"
#include <wx/utils.h>
#include <wx/settings.h>
#include "ConfigManager.h"
#include "string_conversions.h"

BEGIN_EVENT_TABLE(mxStatusBar,wxPanel)
	EVT_PAINT(mxStatusBar::OnPaint)
	EVT_LEFT_DOWN(mxStatusBar::OnClick)
END_EVENT_TABLE();
	
struct st_aux {
	wxString text;
	wxColour col;
	st_aux(){}
	st_aux(const wxColour &c, const wxString &t):text(t),col(c){}
};

static st_aux texts[STATUS_COUNT];
mxStatusBar *status_bar = NULL;
	
mxStatusBar::mxStatusBar(wxWindow *parent):wxPanel(parent,wxID_ANY,wxDefaultPosition,wxDefaultSize) {
	wxColour negro(0,0,0),rojo(128,0,0),verde(0,75,0),azul(0,0,128);
	font = wxFont(11,wxFONTFAMILY_DEFAULT,wxFONTSTYLE_NORMAL,wxFONTWEIGHT_NORMAL);
	bg_color=wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE);
	wxTextFile fil("version"); 
	if (fil.Exists()) {
		fil.Open();
		texts[STATUS_WELCOME]=st_aux(verde,
			wxString(_Z("Bienvenido a PSeInt v"))<<fil.GetFirstLine()<<
			wxString(_Z(" (su perfil actual es: "))<< _S2W(cfg_lang.name) <<_Z(")"));
		fil.Close();
	} else {
		texts[STATUS_WELCOME]=st_aux(rojo,_Z("A ocurrido un error al iniciar el editor."));
	}
	texts[STATUS_PROFILE]             = st_aux( verde ,_Z("Su perfil actual es: "));
	texts[STATUS_SYNTAX_OK]           = st_aux( verde ,_Z("El pseudoc�digo es correcto. Presione F9 para ejecutarlo."));
	texts[STATUS_SYNTAX_ERROR]        = st_aux( rojo  ,_Z("El pseudoc�digo contiene errores. Presione F9 para una mejor descripci�n."));
	texts[STATUS_SYNTAX_ERROR_DETAIL] = st_aux( rojo  ,_Z("El pseudoc�digo contiene errores. Seleccione un error para ver su descripci�n."));
	texts[STATUS_SYNTAX_CHECK_OK]     = st_aux( verde ,_Z("La sintaxis es correcta."));
	texts[STATUS_SYNTAX_CHECK_ERROR]  = st_aux( azul  ,_Z("Seleccione un error para ver su descripci�n."));
	texts[STATUS_FLOW]                = st_aux( azul  ,_Z("Este pseudoc�digo est� siendo editado como diagrama de flujo."));
	texts[STATUS_FLOW_CHANGED]        = st_aux( azul  ,_Z("El diagrama de flujo ha sido modificado. Click aqu� para actualizar el pseudoc�digo."));
	texts[STATUS_RUNNING]             = st_aux( azul  ,_Z("El pseudoc�digo est� siendo ejecutado."));
	texts[STATUS_RUNNING_CHANGED]     = st_aux( azul  ,_Z("El pseudoc�digo ha cambiado. Presione F9 para ver los cambios en la ejecuci�n."));
	texts[STATUS_RUNNING_UPDATED]     = st_aux( azul  ,_Z("La ejecuci�n se ha actualizado para reflejar los cambios."));
	texts[STATUS_RUNNED_OK]           = st_aux( azul  ,_Z("La ejecuci�n ha finalizado sin errores."));
	texts[STATUS_RUNNED_INT]          = st_aux( rojo  ,_Z("La ejecuci�n ha sido interrumpida."));
	texts[STATUS_EXAMPLE]             = st_aux( azul  ,_Z("Este es un ejemplo, no puede modificarlo."));
	texts[STATUS_DEBUG_RUNNING]       = st_aux( azul  ,_Z("El pseudoc�digo est� siendo ejecutado paso a paso."));
	texts[STATUS_DEBUG_PAUSED]        = st_aux( azul  ,_Z("La ejecuci�n paso a paso ha sido pausada."));
	texts[STATUS_DEBUG_STOPPED]       = st_aux( azul  ,_Z("La ejecuci�n paso a paso ha sido detenida."));
	texts[STATUS_DEBUG_ENDED]         = st_aux( azul  ,_Z("La ejecuci�n paso a paso ha finalizado."));
	texts[STATUS_UPDATE_CHECKING]     = st_aux( azul  ,_Z("Buscando actualizaciones..."));
	texts[STATUS_UPDATE_ERROR]        = st_aux( rojo  ,_Z("Error al conectarse al servidor para buscar actualizaciones."));
	texts[STATUS_UPDATE_NONEWS]       = st_aux( azul  ,_Z("No hay actualizaciones disponibles."));
	texts[STATUS_UPDATE_FOUND]        = st_aux( azul  ,_Z("Hay una nueva versi�n para descargar!"));
	texts[STATUS_NEW_SOURCE]          = st_aux( azul  ,_Z("Puede utilizar el panel de comandos y estructuras para a�adir instrucciones."));
	texts[STATUS_COMMAND]             = st_aux( azul  ,_Z("Debe completar los campos marcados con rect�ngulos."));
	status = STATUS_WELCOME;
}

void mxStatusBar::OnPaint (wxPaintEvent & event) {
	wxPaintDC dc(this);
	PrepareDC(dc);
	dc.SetBackground(bg_color);
	dc.SetTextForeground(texts[status].col);
	dc.Clear();
	dc.SetFont(font);
//	wxString text;
	dc.DrawText(texts[status].text,5,3);
}

void mxStatusBar::OnClick (wxMouseEvent & event) {
	if (status==STATUS_SYNTAX_OK||status==STATUS_SYNTAX_ERROR) {
		wxCommandEvent evt;
		main_window->RunCurrent(false);
	} else if (status==STATUS_UPDATE_FOUND) {
		wxLaunchDefaultBrowser("http://pseint.sourceforge.net?page=descargas.php");
	} else if (status==STATUS_DEBUG_RUNNING||status==STATUS_DEBUG_PAUSED||status==STATUS_DEBUG_ENDED||status==STATUS_DEBUG_STOPPED) {
		main_window->ShowDebugPanel(true);
	} else if (status==STATUS_NEW_SOURCE) {
		main_window->ShowCommandsPanel(true);
	} else if (status==STATUS_RUNNING_CHANGED) {
		if (main_window->GetCurrentSource()) main_window->GetCurrentSource()->UpdateRunningTerminal();
	} else if (status==STATUS_FLOW_CHANGED) {
		if (main_window->GetCurrentSource()) main_window->GetCurrentSource()->UpdateFromFlow();
	}
}

void mxStatusBar::SetStatus (int what) {
//	if ((what==STATUS_NEW_SOURCE || what==STATUS_SYNTAX_OK) && (status<STATUS_NEW_SOURCE)) return;
//	else 
		if (what==STATUS_SYNTAX_ERROR && main_window->QuickHelp().IsVisible()) what=STATUS_SYNTAX_ERROR_DETAIL;
	else if (what==STATUS_PROFILE) {
		texts[STATUS_PROFILE].text =
			texts[STATUS_PROFILE].text.BeforeFirst(':') + _Z(": ") + _S2W(cfg_lang.name);
	}
	status=what;
	Refresh();
}

