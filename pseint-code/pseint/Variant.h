#ifndef VARIANT_H
#define VARIANT_H

#include <cassert>
#include <iostream>

#if defined(NDEBUG) || defined(__APPLE__)
#	define Assert(x) 
#else
    /// para usar como el assert de c, pero "int3" da paso al depurador
#	define Assert(x) if(!(x)) { asm("int3"); asm("nop"); } else asm("nop")
#endif

/**
* Esta clase auxiliar me da el indice de un tipo del Variant4 (en qu� posicion
* de la lista de tipos est�, base 1). Lo hace mediante especializaci�n parcial,
* por eso es clase y no funcion, ya que en c++98 no se puede hacer una 
* especializaci�n parcial de una funcion o un m�todo, solo de una clase.
**/
template<typename T, typename T1, typename T2, typename T3, typename T4>
struct Variant4Helper { static int ToInt(); };
template<typename T1, typename T2, typename T3, typename T4>
struct Variant4Helper<T1,T1,T2,T3,T4> { static int ToInt() { return 1; } };
template<typename T1, typename T2, typename T3, typename T4>
struct Variant4Helper<T2,T1,T2,T3,T4> { static int ToInt() { return 2; } };
template<typename T1, typename T2, typename T3, typename T4>
struct Variant4Helper<T3,T1,T2,T3,T4> { static int ToInt() { return 3; } };
template<typename T1, typename T2, typename T3, typename T4>
struct Variant4Helper<T4,T1,T2,T3,T4> { static int ToInt() { return 4; } };


/**
* @brief Clase para almacenar eficientemente un objeto de un tipo a seleccionar
*        en tiempo de ejecuci�n de entre 4 posibles.
*
* El variant4 tienen memoria suficiente como para contener cualquiera de los 4 
* tipos. El tipo puede ir variando arbitrariamente entre esos 4 y un estado
* indefinido en el cual no hay en realidad ning�n dato en el variant.
* Al solicitar el valor (con el m�todo As), se lo debe hacer especificando el
* tipo, y este tipo debe coincidir con el que efectivamente se haya almacenado 
* previamente (con el m�tod Set). En versi�n debug esto se verfica mediante un 
* assert.
* A diferencia de un union, la clase se encarga de construir y destruir 
* correctamente el dato/objeto almacenado al definir o cambiar el tipo, y puede 
* adem�s no contener ning�n dato. Ese es el estado por defecto.
* A diferencia un sistema basado en herencia, aqu� no hay alocaci�n din�mica de 
* memoria, sino que siempre se utiliza un buffer interno est�tico. El buffer 
* ser� al menos tan grande como el mayor de los 4 tipos.
**/
template<typename T1, typename T2, typename T3, typename T4>
class Variant4 {
	/// indica cual tipo contiene actualmente (x si contiena un Tx, 0 si no contiene nada)
	int m_type;
	/// tipo auxiliar, no se instancia, es solo para calcular el tama�o del buffer
	union aux_union_type {char a[sizeof(T1)],b[sizeof(T2)],c[sizeof(T3)],d[sizeof(T4)];};
	/// el buffer donde se construir� un objeto de tipo Tx
	char m_data[sizeof(aux_union_type)];
	/// funci�n auxiliar para convertir un tipo Tx en un entero x
	template<typename T> int TypeIndex() const { return Variant4Helper<T,T1,T2,T3,T4>::ToInt(); }
public:
		
	/// constructor por defecto, inicialmente no contiene nada, no se inicializa el buffer
	Variant4() : m_type(0) {}
	
	/// construcci�n de copia, ver operador de asignaci�n
	Variant4(const Variant4<T1,T2,T3,T4> &other) :m_type(0) { *this = other; }
	
	/// determina si actualmente contiene algo (true) o est� vac�o (false)
	bool IsDefined() const { return m_type!=0; }
	/// determina si el contenido actual es del tipo T
	template<typename T> bool Is() const { return m_type==TypeIndex<T>(); }
	
	/// setea el tipo de contenido actual al tipo T, inicializando el objeto con 
	/// el ctor por defecto de T (no debe contener nada al invocar este m�todo)
	template<typename T> void Set() { Assert(m_type==0); m_type = TypeIndex<T>(); new (m_data) T(); }
	/// setea el tipo de contenido actual al tipo T, inicializando el objeto con 
	/// el ctor de copia de T (no debe contener nada al invocar este m�todo)
	template<typename T> void Set(const T &value) { Assert(m_type==0); m_type = TypeIndex<T>(); new (m_data) T(value); }
	/// asigna un valor de tipo T, ajustando el tipo de contenido si no coincide con T
	template<typename T> void ForceSet(const T &value) { 
		if (Variant4<T1,T2,T3,T4>::template Is<T>()) Variant4<T1,T2,T3,T4>::template As<T>()=value;
		else { Variant4<T1,T2,T3,T4>::Clear(); Variant4<T1,T2,T3,T4>::template Set<T>(value); }
	}
	
	/// retorna el contenido, suponiendo que sea de tipo T (debe serlo, sino ser� comportamiento indefinido)
	template<typename T> T &As() { Assert(m_type==TypeIndex<T>()); T *p = reinterpret_cast<T*>(m_data); return *p; }
	/// retorna el contenido, suponiendo que sea de tipo T (debe serlo, sino ser� comportamiento indefinido)
	template<typename T> const T &As() const{ Assert(m_type==TypeIndex<T>()); const T* p =reinterpret_cast<const T*>(m_data); return *p; }
	
	/// destruye el contenido, y vuelve al estado inicial, donde no contiene nada
	void Clear() { 
		switch (m_type) {
		case 0: return;
		case 1: As<T1>().~T1(); break;
		case 2: As<T2>().~T2(); break;
		case 3: As<T3>().~T3(); break;
		case 4: As<T4>().~T4(); break;
		}
		m_type = 0;
	}
	/// operador de asignaci�n, necesario para la correcta construcci�n del tipo en el buffer interno
	Variant4<T1,T2,T3,T4> &operator=(const Variant4<T1,T2,T3,T4> &other) {
			 if (other.Is<T1>()) { ForceSet<T1>(other.As<T1>()); }
		else if (other.Is<T2>()) { ForceSet<T2>(other.As<T2>()); }
		else if (other.Is<T3>()) { ForceSet<T3>(other.As<T3>()); }
		else if (other.Is<T4>()) { ForceSet<T4>(other.As<T4>()); }
		else { Assert(!other.IsDefined()); Clear(); }
		return *this;
	}
	
	/// destructor, si conten�a algo, invoca al destructor del tipo adecuado de ese contenido
	~Variant4() { Variant4<T1,T2,T3,T4>::Clear(); }
};


// operador <<, para que al hacer "cout<<v" se invoque a la sobrecarga del <<
// que se corresponda con el verdadero tipo del contenido de v
//template<typename T1, typename T2, typename T3, typename T4>
//std::ostream &operator<<(std::ostream &o, const Variant4<T1,T2,T3,T4> &v) {
//	if (v.template Is<T1>()) return o << v.template As<T1>();
//	if (v.template Is<T2>()) return o << v.template As<T2>();
//	if (v.template Is<T3>()) return o << v.template As<T3>();
//	if (v.template Is<T4>()) return o << v.template As<T4>();
//	Assert(v.IsDefined()); return o;
//}

// Algunas cosas se podr�an implementar m�s f�cilmente si no tuviera que mantener
// compatibilidad con C++98 culpa de usar compiladores viejos en las versiones
// para ubuntu y mac

#endif

