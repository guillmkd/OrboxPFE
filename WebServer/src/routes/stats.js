/**
 * Created by age2pierre on 05/02/17.
 */
const router = require('express').Router(),
  fs = require('fs'),
  StatUtil = require('../modules/StatUtil'),
  ObjectsUtil = require('../modules/ObjectsUtil'),
  config = require('../../config'),
  child_process = require("child_process");

router.get('/', (req, res, next) => {
  let statData = StatUtil.retrieveData();
  for (let it of statData)
    it.class_name = ObjectsUtil.getData(it.class_id).name
  res.render('stats', {
    classStats: statData,
    objIds: ObjectsUtil.getIndexes()
  });
});

router.get('/updatestat', (req, res, next) => {
  child_process.execSync('./statUtil', {
    cwd: (config.buildPath + 'ImgProcML/StatUtil/')
  });
  res.redirect('/stats');
});

router.get('/updateperf', (req, res, next) => {
  child_process.execSync('./perfTestUtil', {
    cwd: (config.buildPath + 'ImgProcML/PerfTestUtil/')
  });
  res.redirect('/stats');
});

module.exports = router;