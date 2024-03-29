'use strict'

// require modules
const ip = require('ip')
const winston = require('winston')
const bodyParser = require('body-parser')
const express = require('express')
const fs = require('fs-extra')
const config = require('./config')
const compilr = require('./compilr')

// Constants
const PORT = 8080
const app = express()

if (config.LOG) {
  winston.configure({
    transports: [
      new (winston.transports.Console)(),
      new (winston.transports.File)({filename: 'server-logs.log'})
    ]
  })
}

compilr.init(winston)

app.use(bodyParser.json())        // to support JSON-encoded bodies
app.use(bodyParser.urlencoded({   // to support URL-encoded bodies
  extended: true
}))
app.use(express.static('./dist'))

// root
app.get('/', (req, res) => {
  res.sendFile('./dist/index.html', {root: __dirname})
})

app.post('/compile/', async (req, res, next) => {
  const files = req.body.files
  let dirname

  try {
    // create temp directory
    dirname = await fs.mkdtemp('./run/tmp_')
    winston.log('info', `created directory ${dirname}`)

    // create files
    for (let file of files) {
      if (file.language === "Java"){
        await fs.writeFile(`${dirname}/${file.name}.java`, file.content)
        winston.log('info', `created file ${dirname}/${file.name}.java`)
      } else if (file.language === "Python 3"){
        await fs.writeFile(`${dirname}/${file.name}.py`, file.content)
        winston.log('info', `created file ${dirname}/${file.name}.py`)
      } else if (file.language === "C++"){
        await fs.writeFile(`${dirname}/${file.name}.cpp`, file.content)
        winston.log('info', `created file ${dirname}/${file.name}.cpp`)
      } else if (file.language === "PSeInt (Perfil UNINORTE)"){
        await fs.writeFile(`${dirname}/${file.name}.psc`, file.content)
        winston.log('info', `created file ${dirname}/${file.name}.psc`)
      }
      
      // create .txt input files (if needed)
      if (!(file.input === "")){
        // has input
        await fs.writeFile(`${dirname}/input.txt`, file.input)
        winston.log('info', `created file ${dirname}/input.txt`)
      }
    }    

    // compile and run
    let result = await compilr.compile(dirname, files)
    res.send(result)
  } catch (e) {
    next(e)
  } finally {
    await fs.remove(dirname)
    winston.log('info', `removed directory ${dirname}`)
  }
})

// Error Handling Middleware
app.use((err, req, res, next) => {
  winston.log('error', err.message)
  res.status(500).json({
    message: err.message,
    error: err
  })
/*
  res.status(500).render('error', {
    message: err.message,
    error: err
  })
*/
})

// Start server
app.listen(PORT, async () => {
  try {
    await fs.mkdir('./run')
    console.log(`created run folder`)
  } catch (e) {
    console.log('run directory already exists')
  } finally {
    console.log(`Running on http://${ip.address()}:${PORT}`)
  }
})
