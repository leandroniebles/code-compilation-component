'use strict'

// require modules
const config = require('./config')
const exec = require('child-process-promise').exec

const compilr = {}

compilr.init = (logger) => {
  if (logger) {
    this.logger = logger
  } else {
    this.logger = require('winston')
  }
}

compilr.compile = async (dirname, files) => {
  try {
    // compile files
    for (let file of files) {
      //await exec(`javac ${dirname}/${file.name}.java`)
      if (file.language === "Java"){
        // sudo is required because this command will create a (.class) file
        await exec(`sudo javac ${dirname}/${file.name}.java`)
        this.logger.log('info', `compiled ${dirname}/${file.name}.java`)
      } else if (file.language == "Python 3"){
	// do nothing: python3 does not require a preparation phase for its code to be executed
      } else if (file.language == "C++"){
        // sudo is required because this command will create an executable file with no extension
        await exec(`sudo g++ -o ${dirname}/${file.name} ${dirname}/${file.name}.cpp`)
        this.logger.log('info', `generated executable file of ${dirname}/${file.name}.cpp`)
      } else if (file.language == "PSeInt (Perfil UNINORTE)"){
	// do nothing: PSeInt interpreter does not require a preparation phase for its code to be executed
      }
    }

    const commands = files.map((file) => {
      //return `java -cp ${dirname} ${file.name}`
      if (file.language === "Java"){
        if (!(file.input === "")){
          return `java -cp ${dirname} ${file.name} < ${dirname}/input.txt`
        } else {
          return `java -cp ${dirname} ${file.name}`
        }
      } else if (file.language == "Python 3"){
        if (!(file.input === "")){
          return `python3 ${dirname}/${file.name}.py < ${dirname}/input.txt`
        } else {
          return `python3 ${dirname}/${file.name}.py`
        }
      } else if (file.language == "C++"){
        if (!(file.input === "")){
          return `${dirname}/${file.name} < ${dirname}/input.txt`
        } else {
          return `${dirname}/${file.name}`
        }
      } else if (file.language == "PSeInt (Perfil UNINORTE)"){
        if (!(file.input === "")){
          return `pseint-code/bin/pseint --nouser --force_init_vars=1 --force_define_vars=1 --force_semicolon=1 --allow_concatenation=0 --allow_word_operators=0 --base_zero_arrays=1 --allow_dinamyc_dimensions=0 --lazy_syntax=0 --coloquial_conditions=0 --overload_equal=1 ${dirname}/${file.name}.psc < ${dirname}/input.txt`
        } else {
          return `pseint-code/bin/pseint --nouser --force_init_vars=1 --force_define_vars=1 --force_semicolon=1 --allow_concatenation=0 --allow_word_operators=0 --base_zero_arrays=1 --allow_dinamyc_dimensions=0 --lazy_syntax=0 --coloquial_conditions=0 --overload_equal=1 ${dirname}/${file.name}.psc`
        }
      }
    }).join('|')

    // run files (echo to prevent terminal from freezing on input)
/*
    this.logger.log('info', `executed command: ulimit -t ${config.TIMEOUT};echo "" | ${commands}`)
    const result = await exec(`ulimit -t ${config.TIMEOUT};echo "" | ${commands}`)
    this.logger.log('info', `executed command: timeout/timeout -t ${config.TIMEOUT} ${commands}`)
    const result = await exec(`timeout/timeout -t ${config.TIMEOUT} ${commands}`)
*/
    this.logger.log('info', `executed command: ulimit -t ${config.TIMEOUT};${commands}`)
    const result = await exec(`ulimit -t ${config.TIMEOUT};${commands}`)
    return {
      success: true,
      output: result.stdout
    }
  } catch (err) {
    return {
      success: false,
      output: err.stdout + err.stderr
    }
  }
}

module.exports = compilr
