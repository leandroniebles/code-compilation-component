function sin_titulo() {
	var c, i, j;
	var a = new Array(10);
	for (i=1;i<=10;i++) {
		a[i-1] = i*10;
	}
	for (var aux_index_0=0;aux_index_0<a.length;aux_index_0++) {
		document.write(a[aux_index_0],'<BR/>');
	}
	var b = new Array(3); for (var aux_index_0=0;aux_index_0<3;aux_index_0++) { b[aux_index_0] = new Array(6); }
	c = 0;
	for (var aux_index_0=0;aux_index_0<b.length;aux_index_0++) {
		for (var aux_index_1=0;aux_index_1<b[aux_index_0].length;aux_index_1++) {
			c = c+1;
			b[aux_index_0][aux_index_1] = c;
		}
	}
	for (i=3;i>=1;i--) {
		for (j=1;j<=5;j+=2) {
			document.write(b[i-1][j-1],'<BR/>');
		}
	}
}
