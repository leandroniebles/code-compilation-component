function sin_titulo() {
	var s, x;
	x = 1.5;
	document.write(Math.sqrt(x),'<BR/>');
	document.write(Math.abs(x),'<BR/>');
	document.write(Math.log(x),'<BR/>');
	document.write(Math.exp(x),'<BR/>');
	document.write(Math.sin(x),'<BR/>');
	document.write(Math.cos(x),'<BR/>');
	document.write(Math.tan(x),'<BR/>');
	document.write(Math.asin(x),'<BR/>');
	document.write(Math.acos(x),'<BR/>');
	document.write(Math.atan(x),'<BR/>');
	document.write(Math.round(x),'<BR/>');
	document.write(Math.trunc(x),'<BR/>');
	document.write(Math.floor(Math.random()*15),'<BR/>');
	s = "Hola";
	document.write(s.length,'<BR/>');
	document.write(String.toLowerCase(s),'<BR/>');
	document.write(String.toUpperCase(s),'<BR/>');
	document.write(String.substring(s,1,3),'<BR/>');
	document.write(String.concat(s," Mundo"),'<BR/>');
	document.write(String.concat("Mundo ",s),'<BR/>');
	document.write(Number("15.5"),'<BR/>');
	document.write(String(15.5),'<BR/>');
}
