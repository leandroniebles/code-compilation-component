// * Este ejemplo es para ver c�mo se declaran y definen variables 
// y constantes de distintos tipos. En pseudoc�digo flexible no hace 
// falta explicitar el tipo, pero en muchos lenguajes s�. 
// * Si el lenguaje permite abreviar las acciones de declarar y asignar
// el valor en una sola, es preferible que la traducci�n no utilice
// ese recurso. Ej: en C++ preferir�a "int a; a=1;" antes que "int a=1;"
// * Tambi�n es para observar c�mo se escriben las constantes
// de los diferentes tipos, y c�mo se realizan las asignaciones.
// * Si el lenguaje permite diferenciar entero de real, a debe ser entero
// y b real.
// * Si hay varias formas de operar con cadenas, pensar con cual es
// m�s f�cil realizar la entrada/salida, y aplicar los operadores.
// Por ejemplo, en C++ usando la clase string, los operadores se
// comportan como en pseudoc�digo, mientras que usando arreglos
// de chars hay que invocar funciones.
// * Alcarar adem�s si para las cadenas literales puede llegar a ser
// necesario agregar caracteres de escape (por ej, en C/C++ si est�
// el caracter de escape "\" hay que agregar otra ("\\").
// * Por �ltimo, el caso de la variable p, es un caso donde
// no se puede determinar el tipo, ya que depender� de qu� ingrese
// el usuario cuando ejecute el programa. Indicar si el lenguaje es
// tipado o permite hacer esto. Si es tipado, indicar si existe
// algun mecanismo simple para que el programa compile asumiendo que
// es un n�mero, pero permita al usuario ver f�cilmente el problema
// y cambiar el tipo. Por ejemplo, en C/C++ uso "#define SIN_TIPO float"
// para luego declarar las variables como "SIN_TIPO x;".

Proceso sin_titulo
	definir a como entero;
	a<-1;
	b<-2.5;
	c<-verdadero;
	d<-Falso;
	e<-"Hola";
	f<-"Hola Mundo";
	
	Leer p;
	Escribir p;
FinProceso
