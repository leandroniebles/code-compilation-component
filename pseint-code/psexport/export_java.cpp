#include "export_java.h"
#include "exportexp.h"
#include "../pseint/utils.h"
#include "../pseint/new_funciones.h"

#define _buf_reader_line "\t\tBufferedReader bufEntrada = new BufferedReader(new InputStreamReader(System.in));"


JavaExporter::JavaExporter():CppExporter() {
	include_cmath=false;
	have_subprocesos=false;
#ifdef DEBUG
	// solo para evitar warnings con cpp-check
	use_esperar_tiempo=false; use_esperar_tecla=false; use_reader=false;
#endif
}

void JavaExporter::borrar_pantalla(t_output &prog, string param, string tabs){
	if (for_test)
		insertar(prog,tabs+"System.out.println(\"\");");
	else
		insertar(prog,tabs+"System.out.println(\"\"); // no hay forma directa de borrar la consola en Java");
}

void JavaExporter::esperar_tecla(t_output &prog, string param, string tabs) {
	use_esperar_tecla=true;
	if (for_test)
		insertar(prog,tabs+"System.in.read();");
	else
		insertar(prog,tabs+"System.in.read(); // a diferencia del pseudoc�digo, espera un Enter, no cualquier tecla");
}

void JavaExporter::esperar_tiempo(t_output &prog, string tiempo, bool mili, string tabs) {
	tipo_var t; tiempo=expresion(tiempo,t); // para que arregle los nombres de las variables
	use_esperar_tiempo=true;
	stringstream inst;
	inst<<"Thread.sleep(";
	string pre,post;
	if (!es_numerica_entera_constante(tiempo)) { pre="(long)("; post=")"; }
	if (mili) {
		inst<<pre<<tiempo<<post; 
	} else {
		inst<<pre<<colocarParentesis(tiempo)<<"*1000"<<post;
	}
	inst<<");";
	insertar(prog,tabs+inst.str());
}

void JavaExporter::escribir(t_output &prog, t_arglist args, bool saltar, string tabs){
	string arglist;
	t_arglist_it it=args.begin();
	while (it!=args.end()) {
		if (arglist.size()) arglist+="+";
		tipo_var t;
		arglist+=expresion(*it,t);
		++it;
	}
	if (saltar)
		insertar(prog,tabs+"System.out.println("+arglist+");");
	else
		insertar(prog,tabs+"System.out.print("+arglist+");");
}

void JavaExporter::leer(t_output &prog, t_arglist args, string tabs) {
	use_reader=true;
	t_arglist_it it=args.begin();
	while (it!=args.end()) {
		tipo_var t;
		string varname=expresion(*it,t);
		if (t==vt_numerica && t.rounded) insertar(prog,tabs+varname+" = Integer.parseInt(bufEntrada.readLine());");
		else if (t==vt_numerica) insertar(prog,tabs+varname+" = Double.parseDouble(bufEntrada.readLine());");
		else if (t==vt_logica) insertar(prog,tabs+varname+" = Boolean.parseBoolean(bufEntrada.readLine());");
		else { read_strings=true; insertar(prog,tabs+varname+" = bufEntrada.readLine();"); }
		++it;
	}
}


void JavaExporter::paracada(t_output &out, t_proceso_it r, t_proceso_it q, string tabs) {
	string var=ToLower((*r).par2), aux=ToLower((*r).par1);
	const int *dims=memoria->LeerDims(var);
	if (!dims) { insertar(out,string("ERROR: ")+var+" NO ES UN ARREGLO"); return; }
	int n=dims[0];
	
	if (n==1) { // en java, el reemplazo de foreach anda solo para la primer dimension (las demas parecen no ser referencias a la matriz original, sirven para solo lectura)
		string stipo=translate_tipo(memoria->LeerTipo(var));
		insertar(out,tabs+"for ("+stipo+" "+aux+":"+var+") {");
		bloque(out,++r,q,tabs+"\t");
		insertar(out,tabs+"}");
	} else {
		
		string *auxvars=new string[n];
		for(int i=0;i<n;i++) auxvars[i]=get_aux_varname("aux_index_");
		
		string vname=var;
		for(int i=0;i<n;i++) { 
			string idx=auxvars[i];
			insertar(out,tabs+"for (int "+idx+"=0;"+idx+"<"+vname+".length;"+idx+"++) {");
			vname+="["+idx+"]";
			tabs+="\t";
		}
		
		for(int i=n-1;i>=0;i--) release_aux_varname(auxvars[i]);
		delete []auxvars;
		
		t_output aux_out;
		bloque(aux_out,++r,q,tabs);
		replace_var(aux_out,aux,vname);
		insertar_out(out,aux_out);
		for(int i=0;i<n;i++) { 
			tabs.erase(tabs.size()-1);
			insertar(out,tabs+"}");
		}
	}
}



string JavaExporter::function(string name, string args) {
	if (name=="SEN") {
		include_cmath=true;
		return string("Math.sin")+args;
	} else if (name=="TAN") {
		include_cmath=true;
		return string("Math.tan")+args;
	} else if (name=="ASEN") {
		include_cmath=true;
		return string("Math.asin")+args;
	} else if (name=="ACOS") {
		include_cmath=true;
		return string("Math.acos")+args;
	} else if (name=="COS") {
		include_cmath=true;
		return string("Math.cos")+args;
	} else if (name=="RAIZ") {
		include_cmath=true;
		return string("Math.sqrt")+args;
	} else if (name=="RC") {
		include_cmath=true;
		return string("Math.sqrt")+args;
	} else if (name=="ABS") {
		include_cmath=true;
		return string("Math.abs")+args;
	} else if (name=="LN") {
		include_cmath=true;
		return string("Math.log")+args;
	} else if (name=="EXP") {
		include_cmath=true;
		return string("Math.exp")+args;
	} else if (name=="AZAR") {
		include_cmath=true;
		return string("Math.floor(Math.random()*")+colocarParentesis(get_arg(args,1))+")";
	} else if (name=="ATAN") {
		include_cmath=true;
		return string("Math.atan")+args;
	} else if (name=="TRUNC") {
		include_cmath=true;
		return string("Math.floor")+args;
	} else if (name=="REDON") {
		include_cmath=true;
		return string("Math.round")+args;
	} else if (name=="CONCATENAR") {
		return get_arg(args,1)+".concat("+get_arg(args,2)+")";
	} else if (name=="MAYUSCULAS") {
		return get_arg(args,1)+".toUpperCase()";
	} else if (name=="MINUSCULAS") {
		return get_arg(args,1)+".toLowerCase()";
	} else if (name=="LONGITUD") {
		return get_arg(args,1)+".length()";
	} else if (name=="SUBCADENA") {
		string desde=get_arg(args,2);
		if (!input_base_zero_arrays) desde=sumarOrestarUno(desde,false);
		string hasta=get_arg(args,3);
		if (input_base_zero_arrays) hasta=sumarOrestarUno(hasta,true);
		return get_arg(args,1)+".substring("+desde+","+hasta+")";
	} else if (name=="CONVERTIRATEXTO") {
		return string("Double.toString")+args;
	} else if (name=="CONVERTIRANUMERO") {
		return string("String.valueOf")+args;
	} else {
		return ToLower(name)+args; // no deberia pasar esto
	}
}

// funcion usada por declarar_variables para las internas de una funcion
// y para obtener los tipos de los argumentos de la funcion para las cabeceras
string JavaExporter::get_tipo(map<string,tipo_var>::iterator &mit, bool for_func, bool by_ref) {
	string stipo=translate_tipo(mit->second)+" ";
	if (mit->second.dims) {
		return stipo+ToLower(mit->first)+make_dims(mit->second.dims,"[","][","]",false);;
	} else {
//		if (by_ref) stipo+="*";
		return stipo+ToLower(mit->first);
	}
}

//// resolucion de tipos (todo lo que acceda a cosas privadas de memoria tiene que estar en esta clase porque es la unica amiga)
//void JavaExporter::declarar_variables(t_output &prog, string tab) {
//	map<string,tipo_var>::iterator mit=memoria->GetVarInfo().begin(), mit2=memoria->GetVarInfo().end();
//	while (mit!=mit2) {
//		string str=get_tipo(mit,false);
//		if (str.size()) prog.push_back(tab+str+";");
//		++mit;
//	}
//}

void JavaExporter::header(t_output &out) {
	// cabecera
	init_header(out,"/* "," */");
	if (!for_test) {
		out.push_back("// En java, el nombre de un archivo fuente debe coincidir con el nombre de la clase que contiene,");
		out.push_back(string("// por lo que este archivo deber�a llamarse \"")+main_process_name+".java.\"");
		out.push_back("");
	}
	if (have_subprocesos && !for_test) {
		out.push_back("// Hay dos errores que se pueden generar al exportar un algoritmo con subprocesos desde PSeint a Java:");
		out.push_back("// 1) En java no se puede elegir entre pasaje por copia o por referencia. T�cnicamente solo existe el");
		out.push_back("// pasaje por copia, pero los identificadores de objetos representan en realidad referencias a los");
		out.push_back("// objetos. Entonces, el pasaje para tipos nativos act�a como si fuera por copia, mientras que el");
		out.push_back("// pasaje para objetos (como por ejemplo String) act�a como si fuera por referencia. Esto puede llevar");
		out.push_back("// a que el algoritmo exportado no se comporte de la misma forma que el algoritmo original, en cuyo");
		out.push_back("// caso deber�n modificarse algunos m�todos (subprocesos exportados) para corregir el problema.");
		out.push_back("// 2) Las funciones que hacen lectura por teclado deben lazar una excepci�n. Si una funci�n A es");
		out.push_back("// invocada por otra B, B tambi�n debe manejar (lanzar en este caso) las execpciones que lance A.");
		out.push_back("// Esto no se cumple en el c�digo generado autom�ticamante: las funciones que realizan lecturas");
		out.push_back("// directamente incluyen el c�digo que indica que pueden generar dicha excepci�n, pero las que");
		out.push_back("// lo hacen indirectamente (invocando a otras que s� lo hacen), puede que no, y deber�n ser");
		out.push_back("// corregidas manualmente.");
		out.push_back("");
	}
	out.push_back("import java.io.*;");
	if (include_cmath) out.push_back("import java.math.*;");
	if (!for_test) out.push_back("");
	out.push_back(string("public class ")+ToLower(main_process_name)+" {");
	if (!for_test) out.push_back("");
}

void JavaExporter::footer(t_output &out) {
	if (!for_test) out.push_back("");
	out.push_back("}");
	if (!for_test) out.push_back("");
}


void JavaExporter::translate_single_proc(t_output &out, Funcion *f, t_proceso &proc) {
	
	//cuerpo del proceso
	t_output out_proc;
	use_esperar_tiempo=false; use_esperar_tecla=false; use_reader=false;
	bloque(out_proc,++proc.begin(),proc.end(),"\t\t");
	
	// excepciones que puede tirar el m�todo y deben declararse en el prototipo
	string exceptions;
	if ((use_esperar_tecla||use_reader) && use_esperar_tiempo) {
		exceptions = "throws IOException, InterruptedException ";
	} else if (use_reader||use_esperar_tecla) {
		exceptions = "throws IOException ";
	} else if (use_esperar_tiempo) {
		exceptions = "throws InterruptedException ";
	}
	
	// cabecera del proceso
	string ret; // sentencia "return ..." de la funcion
	if (!f) {
		out.push_back(string("\tpublic static void main(String args[]) ")+exceptions+"{");
	} else {
		have_subprocesos=true;
		string dec="\tpublic static ";
		if (f->nombres[0]=="") {
			dec+="void "; 
		} else {
			ret=CppExporter::get_tipo(f->nombres[0],false,false);
			dec+=ret.substr(0,ret.find(" ")+1);
			ret=string("return")+ret.substr(ret.find(" "));
		}
		dec+=ToLower(f->id)+"(";
		for(int i=1;i<=f->cant_arg;i++) {
			if (i!=1) dec+=", ";
			string var_dec=CppExporter::get_tipo(f->nombres[i],f->pasajes[i]==PP_REFERENCIA);
			dec+=var_dec;
		}
		dec+=") "+exceptions+"{";
		out.push_back(dec);
	}
	
	if (use_reader) insertar(out,_buf_reader_line);
	
	declarar_variables(out,"\t\t");
	
	insertar_out(out,out_proc);
	
	// cola del proceso
	if (ret.size()) out.push_back(string("\t\t")+ret+";");
	out.push_back("\t}");
	if (!for_test) out.push_back("");
	
}


string JavaExporter::get_operator(string op, bool for_string) {
	if (for_string) {
		if (for_string) {
			if (op=="=") return "func arg1.equals(arg2)"; 
			if (op=="<>") return "func !arg1.equals(arg2)"; 
			if (op=="<") return "func arg1.compareTo(arg2)<0"; 
			if (op==">") return "func arg1.compareTo(arg2)>0"; 
			if (op=="<=") return "func arg1.compareTo(arg2)<=0"; 
			if (op==">=") return "func arg1.compareTo(arg2)>=0"; 
		}
	} else {
		if (op=="^") { include_cmath=true; return "func Math.pow(arg1,arg2)"; }
	}
	return CppExporter::get_operator(op,false);
}


void JavaExporter::dimension(t_output &prog, t_arglist &args, string tabs) {
	ExporterBase::dimension(prog,args,tabs);
	t_arglist_it it=args.begin();
	while (it!=args.end()) {
		// obtener nombre y dimensiones
		string name,dims; crop_name_and_dims(*it,name,dims,"[","][","]");
		// armar la linea que hace el new
		string stipo=translate_tipo(memoria->LeerTipo(name));
		insertar(prog,tabs+ToLower(name)+" = new "+stipo+dims+";");
		++it;
	}
}

string JavaExporter::translate_tipo(const tipo_var &t) {
//	if (t==vt_caracter) { stipo="String"; }
	if (t==vt_numerica) return t.rounded?"int":"double";
	if (t==vt_logica) return "boolean";
	return "String";
}

string JavaExporter::get_constante(string name) {
	if (name=="PI") { include_cmath=true; return "Math.PI"; }
	if (name=="VERDADERO") return "true";
	if (name=="FALSO") return "false";
	return name;
}

void JavaExporter::translate(t_output &out, t_programa &prog) {
	CppExporter::translate(out,prog);
	int c=0;
	t_output_it it=out.begin();
	while (it!=out.end()) {
		if (*it==_buf_reader_line) c++;
		++it;
	}
	if (c>1) {
		it=out.begin();
		while (it!=out.end()) {
			if (*it==_buf_reader_line) it=out.erase(it);
			else ++it;
		}
		it=out.begin();
		while (it!=out.end()) {
			if (it->size()>13 && it->substr(0,13)=="public class ") {
				++it;
				string s=_buf_reader_line; s.erase(0,1);
				out.insert(it,s);
				break;
			}
			++it;
		}	
	}
}

void JavaExporter::translate_all_procs (t_output & out, t_programa & prog, string tabs) {
	ExporterBase::translate_all_procs(out,prog,"\t");
}

