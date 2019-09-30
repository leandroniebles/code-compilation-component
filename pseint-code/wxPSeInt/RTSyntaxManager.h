#ifndef RTSYNTAXMANAGER_H
#define RTSYNTAXMANAGER_H
#include <wx/process.h>

class mxSource;
class wxTimer;

/**
* Esta clase gestiona el subproceso del int�rprete (pseint.bin) que se utiliza de 
* fondo para analizar la sintaxis (extrayendo tambien la lista de variables y los
* bloques para resaltar en amarillo). Hay una sola instancia, que es privada para
* que se manejo mediante los m�todos est�ticos, y estos m�todos controlan que 
* sea una y siempre que se requiera exista (similar a un singleton). La comunicacion
* con el proceso hijo va por tuberias, y el parseo de la salida lo hace la 
* funci�n ContinueProcessing. mxSource llama a Process(algo) cuando el c�digo 
* cambia y pasa cierto tiempo (controlado por su timer). Process setea todo en 
* cero, env�a el c�digo por la tuber�a y llama por primera vez a ContinueProcessing
* para que analice la respuesta. Si no hay respuesta rapida (el pseudoc�digo puede
* ser muy largo, la pc muy lenta, o lo que sea), usa un timer propio para relanzarse
* dentro de poco (el evento lo recibe la mainwindow y lo redirecciona mediante
* el m�todo Process(NULL), el mismo que antes pero con NULL como argumento).
* Esto es as� porque el wxYield (la facil ser�a un bucle con ContinueProcessing 
* y wxYield() adentro) puede traer problemas si justo se quiere ejecutar otra 
* cosa en medio (como el mism�simo algoritmo en la terminal).
**/

enum RTArgs { RTA_NULL, RTA_DEFINE_VAR, RTA_RENAME_VAR };

class RTSyntaxManager:public wxProcess {
	wxCSConv conv;
	wxTimer *timer;
	static RTSyntaxManager *the_one;
	static int lid;
	int id, pid, fase_num;
	bool restart,running,processing;
	mxSource *src;
	RTSyntaxManager();
	~RTSyntaxManager();
	void ContinueProcessing();
	void OnTerminate(int pid, int status);
public:
	struct Info {
		RTArgs action;
		wxString sarg;
		int iarg;
		Info() : action(RTA_NULL) {}
		void SetForVarDef(int line, const wxString &vname) {
			action=RTA_DEFINE_VAR; sarg=vname; iarg=line;
		}
		void SetForVarRename(int line, const wxString &vname) {
			action=RTA_RENAME_VAR; sarg=vname; iarg=line;
		}
	};
	static Info extra_args;
	
	static void Start();
	static void Restart();
	static void Stop();
	static bool IsLoaded();
	static bool Process(mxSource *src, Info *args=NULL);
	static void OnSourceClose(mxSource *src);
};

#endif

