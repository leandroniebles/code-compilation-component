# code-compilation-component (Versión en español)
> Este es un servidor web de compilación de lenguajes que está diseñado para ser usado en un host de Ubuntu (Máquina virtual de Linux), y ha sido implementado usando las tecnologías de Node.js y Express. Los lenguajes soportados son: Java, Python 3, C++ y el pseudointérprete PSeInt (Perfil UNINORTE). Este proyecto es una modificación de [este](https://github.com/kykungz/compilr-java), el cual fue diseñado originalmente para que funcionara en un ambiente Docker, pero por ahora ya no es así.

## Versión soportada de Ubuntu (Linux)
- La versión recomendada de esta implementación es la ```19.04```. No puedo garantizar que esta implementación funcione en otras versiones de Linux, porque no he probado esta plataforma en otras versiones, aunque creo no debería haber mucha dificultad en la implementación si se usase una versión ligeramente antigua o diferente de Linux.

## Instalación
En el host de Linux se requiere instalar tanto el programa NodeJS (con npm), como los compiladores que se mostrarán a continuación.

### Paso # 1 - Instalando NodeJS and npm
- La versión requerida de ```node``` es la ```v12.9.1```
- La versión requerida de ```npm``` es la ```6.10.12```
No obstante, es posible que también puedan usarse versiones anteriores, aunque se recomienda estos que se muestran aquí.

Para instalar NodeJS, se recomienda revisar el siguiente [enlace](https://tecadmin.net/install-latest-nodejs-npm-on-ubuntu/). Alternativamente, se podrían ejecutar los siguientes comandos en bash o en una Shell de Linux:
 ```
sudo apt-get install curl
curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
sudo apt-get install -y nodejs
 ```
Por otro lado, puede ser útil revisar los sitios web de [NodeJS](https://nodejs.org/es/), [Express API](https://expressjs.com/es/api.html) y [npmjs](https://www.npmjs.com/) para temas de documentación y/o descargas.

### Paso # 2 - Instalando el compilador Java (OpenJDK 11)
- Las versiones requeridas de ```java``` y ```javac``` son ```11.0.4```

- Instalando el compilador java
```
sudo apt update
apt install default-jre
apt install default-jdk
```
- Verificando la versión del compilador java
```
java -version
javac -version
```
### Paso # 3 - Instalando el compilador de Python (Python 3)
- La versión requerida de ```python3``` es la ```3.7.3```. Aunque no haya soporte actualmente para compilación de ```python``` (que es MUY diferente a la de ```python3```), este debería ser fácil de añadir, teniendo en cuenta que su última versión para el compilador ```python``` es la ```2.7.16```

- Preparando el compilador python3 (Bueno... Algo así)
```
sudo apt update
```
- En realidad ```python``` and ```python3``` deberían venir por defecto en la versión anteriormente mencionada de Ubuntu (Linux). En caso de requerir la documentación de Python, [aquí está](https://www.python.org/).

- Verificando las versiones de Python:
https://askubuntu.com/questions/505081/what-version-of-python-do-i-have
```
python --version
python3 --version
```

### Paso # 4 - Instalando el compilador gcc (C/C++)
C aún no es soportado como una alternativa de lenguaje de programación en la plataforma, sino sólo C++ (Aunque instalar el soporte para C no debería ser demasiado difícil tampoco).
- La versión requerida de ```gcc``` es la ```8.3.0```
```
sudo apt-get install build-essential
```
- Verificando la versión gcc
```
gcc -v
gcc version 8.3.0
```

En cuanto al soporte para el lenguaje pseudocódigo PSeInt, no se requiere instalación adicional. Sin embargo, para documentarse mejor sobre PSeInt, se recomienda la revisión de los siguientes enlaces:

- Sitio web oficial de [PSeInt](http://pseint.sourceforge.net/)
- Enlace de descarga del código fuente de PSeInt versión 20190311: [Descarga](https://downloads.sourceforge.net/project/pseint/20190311/pseint-src-20190311.tgz)
- Enlace de descarga del programa PSeInt: Download 20190311 PSeInt program [Descarga](https://downloads.sourceforge.net/project/pseint/20190311/pseint-l64-20190311.tgz)

Con esta instalación, es posible configurar el ambiente (environment) al editar el archivo `config.js`. Ver [Configuración](#configuration)
abajo.

---

## Uso
### Ejecución del servidor web
```
sudo npm start
```

### Petición (Request)
Se creará un simple servidor API por defecto en el puerto 8080. Debería ser posible compilar y ejecutar el código al enviar una petición o request HTTP `POST` a la ruta https://localhost:8080/compile, con un JSON request body similar a:
```javascript
{ "files": [
  {
    "name": <class_name>,
    "content": <source_code>
  },
  {
    "name": <class_name>,
    "content": <source_code>
  }
] }
```
*Ejemplo:*
```javascript
{ "files": [
 {
     "name": "Run",
     "content": "public class Run {public static void main(String[] arg) {System.out.println(Hello World!);}}"
 }
] }
```
### Respuesta
La respuesta será dada en formato JSON con la siguiente estructura:
```javascript
{
    "success": <boolean>,
    "output": <runnig_result>
}
```
*Ejemplo:*
```javascript
{
    "success": true,
    "output": "hello world!10\n"
}
```
Si hubiere un error de compilación o de ejecución, el campo (field) `sucess` será `false` y la salida de error (error output) será mostrada en pantalla.

---
***Este proyecto, por supuesto, ofrece un editor frontend muy simple en https://localhost:8080***

![frontend image](https://github.com/leandroniebles/code-compilation-component/blob/master/demo-image.png)

# Configuración
`config.js`

| Clave     | Detalle     |
| :------------- | :------------- |
| TIMEOUT       | Límite de tiempo CPU de la terminal (en segundos)       |
| LOG       | Logging (booleano)       |

# code-compilation-component (English version)
> This is a compilation web server meant to be hosted in a Ubuntu (Linux) Virtual Machine, and implemented using Node.js & Express - Supports Java, Python 3, C++ and PSeInt (UNINORTE Profile) language compilation - This project was a modification from: (https://github.com/kykungz/compilr-java), and was originally meant to be used in a Docker environment, but this is no longer the case.

## Supported Ubuntu (Linux) version
- The working Ubuntu version is ```19.04```. I cannot guarantee that this implementation will work in previous versions, since I haven't tested it in previous Ubuntu versions.

## Installation
- The following installations are required:

### Step # 1 - Installing NodeJS and npm
- The required ```node``` version is ```v12.9.1```
- The required ```npm``` version is ```6.10.12```
However, they might also work with previous versions.

In order to install NodeJS, you might want to check out this [link](https://tecadmin.net/install-latest-nodejs-npm-on-ubuntu/). Alternatively, you could use the following commands in the Linux bash.
 ```
sudo apt-get install curl
curl -sL https://deb.nodesource.com/setup_12.x | sudo -E bash -
sudo apt-get install -y nodejs
 ```

Also, you might want to check the [NodeJS](https://nodejs.org/es/) website, the [Express API](https://expressjs.com/es/api.html) website and the [npmjs](https://www.npmjs.com/) website for documentation and/or downloading purposes.

### Step # 2 - Installing Java compiler (OpenJDK 11)
- The required ```java``` and ```javac``` versions are both: ```11.0.4```

- Installing the java compiler
```
sudo apt update
apt install default-jre
apt install default-jdk
```
- Checking for java version
```
java -version
javac -version
```
### Step # 3 - Installing Python (Python 3) compiler
- The required ```python3``` version is ```3.7.3```. Even if there's currently no support Support for ```python``` compiling, this one might be easily be added, taking into account that its latest version should be ```2.7.16```

- Setting up the python3 compiler (Well... Sort of.)
```
sudo apt update
```
- You might want to check the [Python](https://www.python.org/) website for details about python3. However, both ```python``` and ```python3``` should already be installed in the aforementioned Ubuntu version already.

- Checking for Python versions:
https://askubuntu.com/questions/505081/what-version-of-python-do-i-have
```
python --version
python3 --version
```

### Step # 4 - Installing gcc (C/C++) compiler - C is not supported yet as a language choice, though; only C++
- The required ```gcc``` version is ```8.3.0```
```
sudo apt-get install build-essential
```
- Checking for gcc versions
```
gcc -v
gcc version 8.3.0
```

- For the PSeInt language support, no further installation is needed. However, you might want to check the following:

- The [PSeInt](http://pseint.sourceforge.net/) official website
- Download 20190311 PSeInt source code [here](https://downloads.sourceforge.net/project/pseint/20190311/pseint-src-20190311.tgz)
- Download 20190311 PSeInt program [here](https://downloads.sourceforge.net/project/pseint/20190311/pseint-l64-20190311.tgz)

With this installation, you can configure the environment by editing `config.js` file. See [Configuration](#configuration)
below.

---

## Usage
### Web server
```
sudo npm start
```

### Request
This project will create a simple API server on port 8080 by default. You can compile and run your code by sending a HTTP `POST` request to your https://localhost:8080/compile route, with a JSON request body similar to:
```javascript
{ "files": [
  {
    "name": <class_name>,
    "content": <source_code>
  },
  {
    "name": <class_name>,
    "content": <source_code>
  }
] }
```
*Example:*
```javascript
{ "files": [
 {
     "name": "Run",
     "content": "public class Run {public static void main(String[] arg) {System.out.println(Hello World!);}}"
 }
] }
```
### Response
The response will be in JSON format with structure:
```javascript
{
    "success": <boolean>,
    "output": <runnig_result>
}
```
*Example:*
```javascript
{
    "success": true,
    "output": "hello world!10\n"
}
```
If there is a compile/run error, success field will be `false` and the error output will be shown.

---
***This project, of course, provides a simple code editor frontend on your https://localhost:8080***

![frontend image](https://github.com/leandroniebles/code-compilation-component/blob/master/demo-image.png)

# Configuration
`config.js`

| Key     | Detail     |
| :------------- | :------------- |
| TIMEOUT       | Terminal cpu time limit (in seconds)       |
| LOG       | Logging (boolean)       |
