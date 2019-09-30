// * Este ejemplo es para observar c�mo se realiza la entrada y
// salida de cada tipo.
// * Si hay varios mecanismos y alguno es com�n a todos los tipos
// se prefiere dicho mecanismo. Por ejemplo, en C++ los string se 
// pueden leer con cin>>s o getline(cin,s). Ambos generan problemas
// en casos particulares, pero siendo ambos imperfectos se prefiere
// el primero porque funciona tambi�n y sin problemas para los
// dem�s tipos. 
//* En caso de haber problemas o limitaciones es las lecturas, 
// aclarar cuales son y cuando se presentan para que el c�digo
// generado al exportar incluya esta aclaraci�n como comentario.

Proceso sin_titulo
	definir a como entero;
	definir b como real;
	definir c como cadena;
	definir d como logico;
	// leer de a uno
	leer a;
	leer b;
	leer c;
	leer d;
	// escribir de a uno
	Escribir a;
	Escribir b;
	Escribir c;
	Escribir d;
	// lecturas m�ltiples
	leer a,b,c,d;
	// escrituras m�ltiples
	Escribir a,b,c,d;
	// escritura sin salto de linea al final
	Escribir sin saltar "Esta linea no lleva enter al final";
FinProceso
