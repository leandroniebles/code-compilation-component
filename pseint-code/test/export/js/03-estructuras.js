function sin_titulo() {
	var c;
	c = 0;
	while (c<=10) {
		c = c+1;
		document.write(c,'<BR/>');
	}
	c = 0;
	do {
		c = c+1;
		document.write(c,'<BR/>');
	} while (c!=10);
	c = 0;
	do {
		c = c+1;
		document.write(c,'<BR/>');
	} while (c<10);
	if (c==10) {
		document.write("Si",'<BR/>');
	}
	if (c==10) {
		document.write("Si",'<BR/>');
	} else {
		document.write("No",'<BR/>');
	}
}
