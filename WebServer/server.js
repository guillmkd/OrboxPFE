/**
 * Created by age2pierre on 27/01/17.
 */
'use strict';

const express = require('express');
const bodyParser = require('body-parser');
const config = require('./config');
const app = express();

// view engine setup
app.set('view engine', 'pug');
app.set('views', './views');

// set up middlewares
app.use('/static', express.static('public'));
app.use('/data', express.static(config.dataPath));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: true }));

// set up routes
app.get('/', (req, res) => {
  res.redirect('/home');
});
app.use('/home', require('./src/routes/home'));
app.use('/stats', require('./src/routes/stats'));
app.use('/objects', require('./src/routes/objects'));

// Last level error handler
app.use((err, req, res, next) => {
  res.status(err.status || 500)
    .render('error', {
      message: err.message,
      error: {}
    });
});

// 404 handler
app.use(function (req, res, next) {
  res.status(404)
    .render('error', {
      message: "Sorry can't find this page!",
      error: {}
    });
});

// run server
app.listen(config.port);
console.log('Running on http://localhost: ' + config.port);
