function sin_titulo() {
	var c;
	c = Number(prompt());
	switch (c) {
	case 1:
		document.write("1",'<BR/>');
		break;
	case 3: case 5: case 9:
		document.write("3, 5 o 9",'<BR/>');
		break;
	case 7:
		document.write("7",'<BR/>');
		break;
	default:
		document.write("ni 1, ni 3, ni 5, ni 7, ni 9",'<BR/>');
	}
}
