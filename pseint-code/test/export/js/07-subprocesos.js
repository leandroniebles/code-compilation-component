function sin_titulo() {
	var c;
	document.write(funciondoble(5),'<BR/>');
	noretornanada(3,9);
	c = 0;
	porreferencia(c);
	document.write(c,'<BR/>');
	var a = new Array(10);
	var b = new Array(3); for (var aux_index_0=0;aux_index_0<3;aux_index_0++) { b[aux_index_0] = new Array(4); }
	recibevector(a);
	recibematriz(b);
}
function funciondoble(b) {
	var a;
	a = 2*b;
	return a;
}
function noretornanada(a, b) {
	document.write(a+b,'<BR/>');
}
function porreferencia(b) {
	b = 7;
}
function recibevector(v) {
	v[0] = prompt();
}
function recibematriz(m) {
	m[0][0] = prompt();
}
