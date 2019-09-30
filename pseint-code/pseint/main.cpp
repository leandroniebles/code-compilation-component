     // -------------------------------------- //
     //           PSeudo Inerprete             //
     //      http://pseint.sourceforge.net     //
     // -------------------------------------- //

#include "version.h"
#include "zcurlib.h"
#include "common.h"
#include "intercambio.h"
#include "new_memoria.h"

#include "global.h"
#include "utils.h"
#include <csignal>
#include "new_evaluar.h"
#include "SynCheck.h"
#include "Ejecutar.h"
#include "new_funciones.h"
#include "new_programa.h"
#include "case_map.h"
using namespace std;

void on_signal(int s) {
	Inter.SetFinished(true);
	Inter.UnInit();
	exit(s);
}

void checksum(Programa &p) {
	if (p.GetSize()==3) {
		string &s=programa[1].instruccion;
		int n=0,p=1;
		for(unsigned int i=0;i<s.size();i++) { 
			n+=s[i]; p=(p*s[i])%1000000;
		}
		if (n==839 && p==102912) { // daba 730880, por que cambio???
			programa[1].instruccion[9]-=11;
			programa[1].instruccion[10]+=10;
			programa[1].instruccion[11]-=7;
			programa[1].instruccion.insert(12,",\"!\"");
		}
	} else if (p.GetSize()==4) {
		string &s1=programa[1].instruccion;
		string &s2=programa[2].instruccion;
		int n1=0,n2=0,p1=1,p2=1;
		for(unsigned int i=0;i<s1.size();i++) { 
			n1+=s1[i]; p1=(p1*s1[i])%1000000;
		}
		for(unsigned int i=0;i<s2.size();i++) { 
			n2+=s2[i]; p2=(p2*s2[i])%1000000;
		}
		if (n1==619 && p1==956160 && n2==921 && p2==673152) {
			string s = s2+s2+s2; 
			int x[]={-59,-51,-35,-50,-41,-34,-41,-50,63,-63,-46,-52,
				     -27,-23,12,28,-42,-34,-25,-13,-50,35,44,19,15,48,
				     -36,-50,-34,-72,-41,-34,-41,-50,60,22,17,11,-18};
			for(int i=0;i<s.size();i++) s[i]+=x[i]; predef_input.push(s);
		}
	}
}

LangSettings lang(LS_DO_NOT_INIT);

//----------------------------------------------------------------------------
// ************************* Programa Principal ******************************
//-----------------------------------------------------------------------------
int main(int argc, char* argv[]) {

	signal(2,on_signal);
	signal(6,on_signal);
	signal(9,on_signal);
	signal(15,on_signal);

	_handle_version_query("PSeInt");

	lang.Reset();
	
	bool 
		check=true, // checkear syntaxis?
		run=true, // ejecutar?
		user=true, // 
		log_file=false, // guardar errores en un archivo de log
		error=false, 
		for_draw=false, // generar entrada para psdraw
		for_export=false, // generar entrada para psexport
		write_positions=false,
		real_time_syntax=false; // indica que espera eternamente codigo desde std, para usar de fondo para el checkeo de sintaxis en tiempo real en la gui
	int 
		forced_seed=-1; // semilla a utilizar para inicializar la generaci�n de n�meros aleatorios (si es -1 se usa el reloj)
	
	int fil_count=0,delay=0; char *fil_args[5];
	for (int i=1;i<argc;i++) {
		if (argv[i][0]=='-') {
			string str(argv[i]);
			if (str.substr(0,7)=="--seed=")
				forced_seed=atoi(str.substr(7).c_str());
			else if (str.substr(0,8)=="--delay=")
				delay=atoi(str.substr(8).c_str());
			else if (str.substr(0,8)=="--input=")
				predef_input.push(str.substr(8));
			else if (str.substr(0,7)=="--port=")
				Inter.SetPort(atoi(str.substr(7).c_str()));
			else if (str=="--rawerrors")
				raw_errors=true;
			else if (str=="--fortest")
				for_test=true;
			else if (str=="--preservecomments")
				preserve_comments=true;
			else if (str=="--norun")
				run=false;
			else if (str=="--color")
				colored_output=true;
			else if (str=="--nocheck")
				check=false;
			else if (str=="--nouser")
				user=false;
			else if (str=="--noinput")
				noinput=true;
			else if (str=="--fixwincharset")
				fix_win_charset=true;
			else if (str=="--draw") {
				InitCaseMap();
				ignore_logic_errors = for_draw = true;
				run=false;
			} else if (str=="--export") {
//				InitCaseMap();
				for_export = for_draw = true;
				run=false;
			} else if (str=="--easteregg") {
				cerr<<"Bazinga!"<<endl;
				return 0;
			} else if (str=="--forrealtimesyntax") {
				InitCaseMap();
				real_time_syntax=true;
			} else if (str=="--forpseintterminal") {
				for_pseint_terminal=true;
			} else if (str=="--writepositions") {
				write_positions=true;
			} else if (str=="--withioreferences") {
				with_io_references=true;
			} else if (str=="--foreval") {
				for_eval=true;
			} else if (str.substr(0,2)=="--" && !lang.ProcessConfigLine(str.substr(2))) {
				error=true;
			}
		} else {
			fil_args[fil_count++]=argv[i];
		}
	}
	error = error&&((fil_count<(for_draw?2:1))||(fil_count>(for_draw?3:2)));
	log_file = fil_count>(for_draw?2:1);
#ifdef __APPLE__
	wait_key=false;
#else
	wait_key=user&&!for_pseint_terminal;
#endif

	// comprobar parametros
	if (error || (fil_count==0 && !real_time_syntax)) {
		cout<<"Use: pseint FileName.psc [<options>] [LogFile]\n";
		cout<<"     pseint FileName.psc --draw DrawTempFile.psd [LogFile]\n";
		cout<<" <options> puede ser una o mas de las siguientes:"<<endl;
		cout<<"      --color                utilizar colores para formatear la salida"<<endl;
		cout<<"      --nocheck              no revisar la sintaxis"<<endl;
		cout<<"      --norun                no ejecutar"<<endl;
		cout<<"      --nouser               no mostrar mensajes de estado ni esperar un tecla al final"<<endl;
		cout<<"      --port=<num>           define el puerto tpc para comunicar controlar la depuracion"<<endl;
		cout<<"      --fortest              ignora algunas instrucciones particulares para evitar ciertas entradas/salidas"<<endl;
		cout<<"      --rawerrors            muestra los errores sin descripcion, para testing automatizado"<<endl;
		cout<<"      --noinput              en lugar realizar las lecturas desde el teclado, lo hace desde los argumentos"<<endl;
		cout<<"      --input=<str>          sirve para predefinir uno o m�s valores de entrada para acciones LEER"<<endl;
		cout<<"      --fixwincharset        corrige la codificaci�n de algunos caracteres para que se muestren correctamente"<<endl;
		cout<<"                             en la consola de Windows"<<endl;
		cout<<"      --writepositions       al generar el archivo parseado para el editor de diagrams de flujo incluye los"<<endl;
		cout<<"                             numeros de linea e instrucci�n necesarios para marcar la ejecuci�n paso a paso"<<endl;
		cout<<"      --delay=<num>          define el retardo entre instrucciones para la ejecucion paso a paso"<<endl;
		cout<<"      --seed=<num>           semilla para el generador de numeros aleatorios"<<endl;
		cout<<"      --profile=<archivo>    archivo de perfil a utilizar para configurar el int�rprete"<<endl;
		cout<<"      --<opt>=?              cambia la opcion <opt> del perfil, ? puede ser 0 o 1"<<endl;
		exit(1);
	}
	
	if (for_eval) { colored_output=false; user=wait_key=false; }
	
	if (colored_output) { 
#ifndef __APPLE__
		setBackColor(COLOR_BLACK); 
#endif
		clrscr();
	}
	
	if (log_file) {
		ExeInfo.open(fil_args[for_draw?2:1]);
		if (ExeInfo.is_open()) ExeInfoOn=true;
	}

	// inicializaciones varias
	int errores;
	LoadFunciones();
	if (forced_seed==-1) srand(time(NULL)); else srand(forced_seed);
	
	if (real_time_syntax) {
		while (true) {
//			memoria->HardReset();
			programa.HardReset();
			UnloadSubprocesos();
			string line;
			int lcount=0;
			while (cin) {
				getline(cin,line); lcount++;
				if (line=="<!{[EXIT]}!>") { UnloadFunciones(); return 0; }
				if (line=="<!{[END_OF_INPUT]}!>") break;
				programa.PushBack(line);
			}
			SynCheck();
			cout<<"<!{[END_OF_OUTPUT]}!>"<<endl;
			map<string,Funcion*>::iterator it1=subprocesos.begin(), it2=subprocesos.end();
			while (it1!=it2) {
				Funcion *func=it1->second;
				for(int j=0;j<=func->cant_arg;j++) { // agregar argumentos y valor de retorno de la funcion
					if (func->nombres[j].size() && !func->memoria->Existe(func->nombres[j]))
						func->memoria->DefinirTipo(func->nombres[j],vt_desconocido);
				}
				if (it1->first!=main_process_name) cout<<"SUB";
				cout<<"PROCESO ";
				if (case_map) cout<<(*case_map)[it1->first];
				else cout<<it1->first;
				cout<<":"<<func->userline_start-1<<':'<<func->userline_end-1<<endl;
				func->memoria->ListVars(case_map);
				++it1;
			}
			cout<<"<!{[END_OF_VARS]}!>"<<endl;
			if (lcount) {
				int n=programa.GetInstSize();
				int *bk=new int[lcount], *st=new int[n+1], stn=0;
				for(int i=0;i<lcount;i++) bk[i]=-1;
				for(int i=0;i<n;i++) { 
					Instruccion &in=programa[i];
					if (
						LeftCompare(in.instruccion,"SI ") ||
						LeftCompare(in.instruccion,"PARA ") ||
						LeftCompare(in.instruccion,"PARACADA ") ||
						LeftCompare(in.instruccion,"MIENTRAS ") ||
						LeftCompare(in.instruccion,"SEGUN ") ||
						in.instruccion=="REPETIR")
							st[stn++]=in.num_linea;
					else if (stn&& (
						in.instruccion=="FINSI" ||
						in.instruccion=="FINMIENTRAS" ||
						in.instruccion=="FINSEGUN" ||
						in.instruccion=="FINPARA" ||
						LeftCompare(in.instruccion,"HASTA QUE ")||
						LeftCompare(in.instruccion,"MIENTRAS QUE ")))
							bk[st[--stn]]=in.num_linea;
				}
				for(int i=0;i<lcount;i++) { 
					if (bk[i]!=-1) cout<<i<<" "<<bk[i]<<endl;
				}
				delete []bk; delete []st;
			}
			cout<<"<!{[END_OF_BLOCKS]}!>"<<endl;
		}
		return 0; // nunca se llega hasta aqui
	}
	
	// Leer el archivo	
	char *filename=fil_args[0];
	ifstream archivo(filename);
	if (!archivo.is_open()) {
		if (colored_output) setForeColor(COLOR_ERROR);
		cout<<"ERROR: No se pudo abrir el archivo \""<<filename<<"\""<<endl;
		if (ExeInfoOn) {
			ExeInfo<<"ERROR: No se pudo abrir el archivo \""<<filename<<"\""<<endl;
			if (user) {
				hideCursor();
				if (wait_key) getKey();
				showCursor();
			}
		}
		exit(1);
	}
	
	if (user && check) {
		show_user_info("Leyendo archivo y comprobando sintaxis...");
	}
	string buffer;
	while (getline(archivo,buffer)) {
		programa.PushBack(buffer);
		Inter.AddLine(buffer);
	}
	archivo.close();

	// comprobar sintaxis y ejecutar
	errores=SynCheck();
	Inter.InitDebug(delay);
	
	if (for_pseint_terminal) { cout<<"\033[zt"<<main_process_name<<"\n"; }

	//ejecutar
	if (errores==0) {
	// salida para diagrama
		if (for_draw) {
			if (case_map) CaseMapPurge();
			ofstream dibujo(fil_args[1]);
			for (int i=0;i<programa.GetSize();i++) {
				if (!for_export && programa[i].type==IT_ASIGNAR) { 
					// sacar los parentesis adicionales
					int p = programa[i].instruccion.find("<-");
					programa[i].instruccion.erase(p+2,1);
					programa[i].instruccion.erase(programa[i].instruccion.size()-2,1);
					// unir varias asignaciones en una sola linea si asi estaban originalmente y esto va para psdraw
					while (i+1<programa.GetSize() && programa[i+1].type==IT_ASIGNAR && programa[i].num_linea==programa[i+1].num_linea) 
					{
						++i;
						// sacar los parentesis adicionales
						int p = programa[i].instruccion.find("<-");
						programa[i].instruccion.erase(p+2,1);
						programa[i].instruccion.erase(programa[i].instruccion.size()-2,1);
						// unir
						programa[i].instruccion = 
							programa[i-1].instruccion +" "+ programa[i].instruccion;
					}
				}
				if (case_map && (!preserve_comments || !LeftCompare(programa[i].instruccion,"#")))
					CaseMapApply(programa[i].instruccion,!for_export);
				if (write_positions) 
					dibujo<<"#pos "<<programa[i].num_linea<<":"<<programa[i].num_instruccion<<endl;
#ifdef _DEBUG
				cerr<<programa[i].instruccion<<endl;
#endif
				dibujo<<programa[i].instruccion<<endl;
			}
			dibujo.close();
			if (user) show_user_info("Dibujo guardado.");
			ExeInfo.close();
			ExeInfoOn=false;
		} else if (run) {
			if (ExeInfoOn) if (user) ExeInfo<<"*** Ejecucion Iniciada. ***"<<endl;
			if (user) show_user_info("*** Ejecuci�n Iniciada. ***");
			map<string,Funcion*>::iterator it1=subprocesos.begin(), it2=subprocesos.end();
			while (it1!=it2) (it1++)->second->memoria->FakeReset();
			Inter.SetStarted();
			checksum(programa);
			const Funcion *main_func=EsFuncion(main_process_name,true);
			memoria=main_func->memoria;
			Ejecutar(main_func->line_start);
			Inter.SetFinished();
			if (ExeInfoOn) ExeInfo<<"*** Ejecucion Finalizada. ***";
			if (user) show_user_info("*** Ejecuci�n Finalizada. ***");
		}
	} else {
		if (user) cout<<endl;
		if (errores==1) {
			if (ExeInfoOn) if (user) ExeInfo<<"*** Se encontro 1 error. ***";
			if (user) show_user_info("*** Se encontr� 1 error. ***");
		} else {
			if (ExeInfoOn) if (user) ExeInfo<<"*** Se encontraron "<<errores<<" errores. ***";
			if (user) show_user_info("*** Se encontraron ",errores," errores. ***");
		}
	}
	// esperar un enter si es necesario
	if (ExeInfoOn) {   
		ExeInfo.close();
	}
	if (user && !for_draw) {
		hideCursor();
		if (wait_key) getKey();
		showCursor();
	}

#ifdef _DEBUG	
	UnloadFunciones();
	UnloadSubprocesos();
#endif
	return 0;
}


//--------------------------------------------------------------------------------------

