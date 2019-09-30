# code-compilation-component
> This is a compilation web server meant to be hosted in a Ubuntu (Linux) Virtual Machine, and implemented using Node.js & Express - Supports Java, Python 3, C++ and PSeInt (UNINORTE Profile) language compilation - This project was a modification from: (https://github.com/kykungz/compilr-java), and was originally meant to be used in a Docker environment, but this is no longer the case.

## Supported Ubuntu (Linux) version
- The working Ubuntu version is ```19.04```. I cannot guarantee that this implementation will work in previous versions, since I haven't tested it in previous Ubuntu versions.

## Installation
- The following installations are required:

### Step # 1 - Installing NodeJS and npm
- The required ```node``` version is ```v12.9.1```
- The required ```npm``` version is ```6.10.12```
However, they might also work with previous versions.

Sadly, I don't remember how to do that, but I can provide the following [link](https://hostadvice.com/how-to/how-to-install-node-js-on-ubuntu-18-04/), since it actually helped me somewhat. On that regard, apparently [NodeJS comes with npm](https://stackoverflow.com/questions/20128584/npm-comes-with-node-now-what-does-this-mean). Also, you might want to check the [NodeJS](https://nodejs.org/es/) website, the [Express API](https://expressjs.com/es/api.html) website and the [npmjs](https://www.npmjs.com/) website for documentation and/or downloading purposes.

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

- Setting up the python3 compiler
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
Compilr will create a simple API server on port 8080 by default. You can compile and run your code by sending a HTTP `POST` request to your https://localhost:8080/compile route, with a JSON request body similar to:
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
