#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>
#include <wx/scrolbar.h>
#include "mxDebugWindow.h"
#include "mxStatusBar.h"
#include "ids.h"
#include "DebugManager.h"
#include "mxMainWindow.h"
#include "ConfigManager.h"
#include "mxProcess.h"
#include "mxHelpWindow.h"
#include "mxDesktopTestPanel.h"
#include "mxEvaluateDialog.h"
#include "mxSubtitles.h"
#include "mxBitmapButton.h"

mxDebugWindow *debug_panel = NULL;

BEGIN_EVENT_TABLE(mxDebugWindow,wxPanel)
	EVT_BUTTON(mxID_DEBUG_BUTTON, mxDebugWindow::OnDebugButton)
	EVT_BUTTON(mxID_DEBUG_PAUSE, mxDebugWindow::OnDebugPause)
	EVT_BUTTON(mxID_DEBUG_STEP, mxDebugWindow::OnDebugStep)
	EVT_BUTTON(mxID_DEBUG_HELP, mxDebugWindow::OnDebugHelp)
	EVT_BUTTON(mxID_DEBUG_EVALUATE, mxDebugWindow::OnDebugEvaluate)
	EVT_CHECKBOX(mxID_DEBUG_DESKTOP_VARS, mxDebugWindow::OnDebugDesktopTest)
	EVT_CHECKBOX(mxID_DEBUG_STEP_IN, mxDebugWindow::OnDebugCheckStepIn)
	EVT_CHECKBOX(mxID_DEBUG_SUBTITLES, mxDebugWindow::OnDebugCheckSubtitles)
END_EVENT_TABLE()	

mxDebugWindow::mxDebugWindow(wxWindow *parent):wxPanel(parent,wxID_ANY) {
	
	wxString ipath = DIR_PLUS_FILE_2(config->images_path,"paso",config->big_icons?"30":"20");
	wxBitmap bmp_pausar  (DIR_PLUS_FILE(ipath,"pausar.png"), wxBITMAP_TYPE_PNG), 
		     bmp_comenzar(DIR_PLUS_FILE(ipath,"iniciar.png"),wxBITMAP_TYPE_PNG), 
		     bmp_un_paso (DIR_PLUS_FILE(ipath,"un_paso.png"),wxBITMAP_TYPE_PNG), 
		     bmp_detener (DIR_PLUS_FILE(ipath,"detener.png"),wxBITMAP_TYPE_PNG),
		     bmp_cerrar  (DIR_PLUS_FILE(ipath,"cerrar.png"), wxBITMAP_TYPE_PNG);
	
	dp_bmps[BMP_COMENZAR] = mxBitmapButton::GenerateButtonImage("Comenzar",&bmp_comenzar);
	dp_bmps[BMP_PAUSAR] = mxBitmapButton::GenerateButtonImage("Pausar",&bmp_pausar);
	dp_bmps[BMP_PRIMER_PASO] = mxBitmapButton::GenerateButtonImage("Primer Paso",&bmp_un_paso);
	dp_bmps[BMP_FINALIZAR] = mxBitmapButton::GenerateButtonImage("Detener",&bmp_detener);
	dp_bmps[BMP_CONTINUAR] = mxBitmapButton::GenerateButtonImage("Continuar",&bmp_comenzar);
	dp_bmps[BMP_UN_PASO] = mxBitmapButton::GenerateButtonImage("Avanzar un Paso",&bmp_un_paso);
	dp_bmps[BMP_CERRAR] = mxBitmapButton::GenerateButtonImage("Cerrar",&bmp_cerrar);
	
	evaluate_window = NULL;
	wxBoxSizer *sizer = new wxBoxSizer(wxVERTICAL);
	sizer->AddSpacer(5);
//	sizer->Add(new wxStaticText(this,wxID_ANY,_T(" Estado:")),wxSizerFlags().Proportion(0).Expand().Border(wxTOP,10));
//	debug_status = new wxStaticText(this,wxID_ANY,_T("No iniciada"),wxDefaultPosition,wxDefaultSize,wxALIGN_CENTRE|wxST_NO_AUTORESIZE);
//	sizer->Add(debug_status,wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	sizer->Add(dp_button_run = new wxBitmapButton(this,mxID_DEBUG_BUTTON,dp_bmps[BMP_COMENZAR]),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	sizer->Add(dp_button_pause = new wxBitmapButton(this,mxID_DEBUG_PAUSE,dp_bmps[BMP_PAUSAR]),wxSizerFlags().Proportion(0).Expand());
	sizer->Add(dp_button_step = new wxBitmapButton(this,mxID_DEBUG_STEP,dp_bmps[BMP_PRIMER_PASO]),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	sizer->Add(dp_button_evaluate = new wxButton(this,mxID_DEBUG_EVALUATE,_T("Evaluar...")),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	dp_button_evaluate->SetToolTip(utils->FixTooltip("Puede utilizar este bot�n para evaluar una expresi�n o conocer el valor de una variable durante la ejecuci�n paso a paso. Para ello debe primero pausar el algoritmo."));
	sizer->Add(new wxStaticText(this,wxID_ANY,_T(" Velocidad:")),wxSizerFlags().Proportion(0).Expand().Border(wxTOP,10));
	debug_speed=new wxScrollBar(this,mxID_DEBUG_SLIDER);
	debug_speed->SetScrollbar(0,1,100,10);
	debug_speed->SetToolTip(utils->FixTooltip("Con este slider puede variar la velocidad con la que avanza autom�ticamente la ejecuci�n paso a paso."));
	debug_speed->SetThumbPosition(config->stepstep_tspeed);
	sizer->Add(debug_speed,wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	dp_check_step_in=new wxCheckBox(this,mxID_DEBUG_STEP_IN,"Entrar en subprocesos");
	dp_check_step_in->SetToolTip(utils->FixTooltip("Cuando esta opci�n est� activada y el proceso llega a la llamada de una funci�n entra en dicha funci�n y muestra pasa a paso c�mo se ejecuta la misma, mientras que si est� desactivada ejecuta la llamada completa en un solo paso sin mostrar la ejecuci�n de la misma."));
	dp_check_step_in->SetValue(true);
	if (!cfg_lang[LS_ENABLE_USER_FUNCTIONS]) dp_check_step_in->Hide();
	sizer->Add(dp_check_step_in,wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10)); 
	
//	sizer->AddSpacer(20);
	sizer->Add(dp_check_desktop_test = new wxCheckBox(this,mxID_DEBUG_DESKTOP_VARS,_T("Prueba de Escritorio")),wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10));
	dp_check_desktop_test->SetToolTip(utils->FixTooltip("Con esta opci�n puede configurar una tabla con un conjunto de variables o expresiones para que ser�n evaluadas en cada paso de la ejecuci�n paso a paso y registradas en dicha tabla autom�ticamente para analizar luego la evoluci�n de los datos y el algoritmo."));
#ifdef __WIN32__
	dp_check_subtitles=new wxCheckBox(this,mxID_DEBUG_SUBTITLES,"Explicar en detalle c/paso");
#else
	dp_check_subtitles=new wxCheckBox(this,mxID_DEBUG_SUBTITLES,"Explicar con detalle\ncada paso");
#endif
	dp_check_subtitles->SetToolTip(utils->FixTooltip("Haga click en \"Comenzar\" para iniciar la ejecuci�n paso a paso y leer en este panel los detalles de cada acci�n que realiza el int�rprete."));
	dp_check_subtitles->SetValue(false);
	sizer->Add(dp_check_subtitles,wxSizerFlags().Proportion(0).Expand().Border(wxBOTTOM,10)); 
	sizer->Add(new wxButton(this,mxID_DEBUG_HELP,_T("Ayuda...")),wxSizerFlags().Proportion(0).Expand().Border(wxTOP,10));
	SetState(DS_STOPPED);
	this->SetSizerAndFit(sizer);
	evaluate_window = new mxEvaluateDialog(parent);
}

void mxDebugWindow::SetSpeed(int speed) {
	debug_speed->SetThumbPosition(config->stepstep_tspeed=speed);
}

void mxDebugWindow::SetState(ds_enum state) {
	switch (state) {
	case DS_STARTING:
		main_window->EnableDebugButton(false);
		dp_button_run->SetBitmapLabel(dp_bmps[BMP_FINALIZAR]);
		dp_button_run->SetToolTip(utils->FixTooltip("Utilice este bot�n para detener definitivamente la ejecuci�n del algoritmo y abandonar el modo de ejecuci�n paso a paso."));
		subtitles->text->SetValue("Haga click en \"Continuar\" para leer en este panel los detalles las pr�ximas acciones que realize el int�rprete.");
//		debug_status->SetLabel(_T("Iniciando"));
		dp_button_step->Disable();
		dp_button_step->SetBitmapLabel(dp_bmps[BMP_UN_PASO]);
		dp_button_step->SetToolTip(utils->FixTooltip("Utilice este bot�n para avanzar ejecutar solamente la siguiente instrucci�n del algoritmo."));
		subtitles->button_next->Disable();
		subtitles->button_next->SetLabel("Continuar");
		dp_check_desktop_test->Disable();
		desktop_test_panel->SetEditable(false);
		break;
	case DS_STOPPED:
		if (main_window) main_window->EnableDebugButton(true);
		subtitles->button_next->SetLabel(_T("Comenzar"));
		subtitles->text->SetValue("Haga click en \"Comenzar\" para iniciar la ejecuci�n paso a paso y leer en este panel los detalles de cada acci�n que realiza el int�rprete.");
		subtitles->button_next->Enable();
		dp_button_run->SetBitmapLabel(dp_bmps[BMP_COMENZAR]);
		dp_button_run->SetToolTip(utils->FixTooltip("Utilice este bot�n para que el algoritmo comience a ejecutarse autom�ticamente y paso a paso, se�alando cada instrucci�n que ejecuta, seg�n la velocidad definida en el men� configuraci�n."));
		dp_button_step->SetBitmapLabel(dp_bmps[BMP_PRIMER_PASO]);
		dp_button_step->SetToolTip(utils->FixTooltip("Utilice este bot�n para ejecutar solo la primer instrucci�n del algoritmo y pausar inmediatamente la ejecuci�n del mismo."));
		dp_button_step->Enable();
		dp_check_desktop_test->Enable();
		if (desktop_test_panel) desktop_test_panel->SetEditable(true);
		dp_button_evaluate->Disable();
		dp_button_step->Enable();
		dp_button_pause->Disable();
		if (debug&&debug->source) if (ds_state!=DS_FINALIZED) debug->source->SetStatus(STATUS_DEBUG_STOPPED);
//		debug_status->SetLabel(_T("No Iniciada"));
		break;
	case DS_FINALIZED:
		subtitles->button_next->SetLabel(_T("Cerrar"));
		subtitles->button_next->Enable();
		dp_button_run->SetBitmapLabel(dp_bmps[BMP_CERRAR]);
		dp_button_run->SetToolTip(utils->FixTooltip("Ha finalizado la ejecuci�n del algoritmo. Utilice este bot�n para cerrar la ventana de la ejecuci�n del mismo."));
		dp_button_pause->Disable();
		dp_button_step->Disable();
		if (debug&&debug->source) debug->source->SetStatus(STATUS_DEBUG_ENDED);
		break;
	case DS_PAUSED:
		dp_button_step->Enable();
		subtitles->button_next->Enable();
		dp_button_pause->Enable();
		dp_button_pause->SetBitmapLabel(dp_bmps[BMP_CONTINUAR]);
		dp_button_pause->SetToolTip(utils->FixTooltip("Utilice este bot�n para que el algoritmo contin�e avanzando paso a paso autom�ticamente."));
		dp_button_evaluate->Enable();
		if (debug&&debug->source) debug->source->SetStatus(STATUS_DEBUG_PAUSED);
		break;
	case DS_RESUMED:
		dp_button_step->Disable();
		subtitles->button_next->Disable();
		dp_button_pause->Enable();
		dp_button_pause->SetBitmapLabel(dp_bmps[BMP_PAUSAR]);
		dp_button_pause->SetToolTip(utils->FixTooltip("Utilice este bot�n para detener temporalmente la ejecuci�n del algoritmo. Al detener el algoritmo puede observar el valor de las variables con el bot�n Evaluar."));
		dp_button_evaluate->Disable();
		if (debug&&debug->source) debug->source->SetStatus(STATUS_DEBUG_RUNNING);
		break;
	case DS_STEP:
		dp_button_pause->Disable();
		dp_button_evaluate->Disable();
		dp_button_step->Disable();
		subtitles->button_next->Disable();
		if (debug&&debug->source) debug->source->SetStatus(STATUS_DEBUG_RUNNING);
		break;
	default:
		break;
	}
	ds_state = state;
	if (evaluate_window && evaluate_window->IsShown()) evaluate_window->Evaluate();
}

void mxDebugWindow::OnDebugButton(wxCommandEvent &evt) {
	if (debug->debugging)
		debug->Stop();
	else
		DebugStartFromGui(true);
}

void mxDebugWindow::DebugStartFromGui(bool from_psdraw) {
	if (debug->debugging) { main_window->ShowDebugPanel(true,true); return; }
	mxSource *src = main_window->GetCurrentSource();
	if (!src) return;
	main_window->ReorganizeForDebugging();
	if (!from_psdraw && src->GetFlowSocket()) {
		src->GetFlowSocket()->Write("send debug\n",11);
		return;
	}
	
//	if (evaluate_window->IsShown()) evaluate_window->Hide();
//	if (evaluate_window->IsShown()) evaluate_window->Evaluate();
//	if (debug->debugging) {
//		debug->Stop();
//	} else {
//		mxSource *src=main_window->GetCurrentSource();
		if (src) StartDebugging(src,false);
//	}
}

void mxDebugWindow::OnDebugPause(wxCommandEvent &evt) {
//	if (evaluate_window->IsShown()) evaluate_window->Hide();
//	if (evaluate_window->IsShown()) evaluate_window->Evaluate();
	if (ds_state==DS_STEP) return;
	debug->Pause();
}

void mxDebugWindow::OnDebugStep(wxCommandEvent &evt) {
//	if (evaluate_window->IsShown()) evaluate_window->Hide();
	if (evaluate_window->IsShown()) evaluate_window->Evaluate();
	if (ds_state==DS_STEP) return;
	if (debug->debugging) {
		debug->Step();
	} else {
		main_window->ReorganizeForDebugging();
		mxSource *src=main_window->GetCurrentSource();
		if (src) StartDebugging(src,true);
	}
}

void mxDebugWindow::StartDebugging(mxSource *source, bool paused) {
	wxString fname = source->SaveTemp();
	debug->should_pause = paused;
	if ( (new mxProcess(source))->Debug(fname, true) )
		SetState(DS_STARTING);
}

void mxDebugWindow::OnDebugHelp(wxCommandEvent &evt) {
	if (!helpw) helpw = new mxHelpWindow();
	helpw->ShowHelp(_T("debug.html"));
}

void mxDebugWindow::OnDebugDesktopTest(wxCommandEvent &evt) {
	main_window->ShowDesktopTestPanel(dp_check_desktop_test->GetValue(),true);
}
void mxDebugWindow::OnDebugEvaluate(wxCommandEvent &evt) {
	evaluate_window->Show();
}

void mxDebugWindow::ProfileChanged ( ) {
	dp_check_step_in->Show(cfg_lang[LS_ENABLE_USER_FUNCTIONS]);
	Layout();
}

void mxDebugWindow::OnDebugCheckStepIn(wxCommandEvent &evt) {
	evt.Skip();
	debug->SetStepIn(dp_check_step_in->GetValue());
}

void mxDebugWindow::OnDebugCheckSubtitles(wxCommandEvent &evt) {
	evt.Skip();
	debug->SetSubtitles(dp_check_subtitles->GetValue());
	main_window->ShowSubtitles(dp_check_subtitles->GetValue(),true);
}

void mxDebugWindow::ShowInEvaluateDialog(wxString s) {
	evaluate_window->Show();
	evaluate_window->EvaluateExpression(s);
}

void mxDebugWindow::SetSubtitles(bool on) {
	dp_check_subtitles->SetValue(on);
	wxCommandEvent evt;
	OnDebugCheckSubtitles(evt);
	debug->SetSubtitles(true);
}

ds_enum mxDebugWindow::GetState ( ) {
	return ds_state;
}

void mxDebugWindow::OnDesktopTestPanelHide ( ) {
	dp_check_desktop_test->SetValue(false);
}

bool mxDebugWindow::IsDesktopTestEnabled ( ) {
	return dp_check_desktop_test->GetValue();
}

