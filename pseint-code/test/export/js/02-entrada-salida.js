function sin_titulo() {
	var a = new Number();
	var b = new Number();
	var c = new String();
	var d = new Boolean();
	a = Number(prompt());
	b = Number(prompt());
	c = prompt();
	d = Boolean(prompt());
	document.write(a,'<BR/>');
	document.write(b,'<BR/>');
	document.write(c,'<BR/>');
	document.write(d,'<BR/>');
	a = Number(prompt());
	b = Number(prompt());
	c = prompt();
	d = Boolean(prompt());
	document.write(a,b,c,d,'<BR/>');
	document.write("Esta linea no lleva enter al final");
}
