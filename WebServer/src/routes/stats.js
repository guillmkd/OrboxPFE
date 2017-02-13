/**
 * Created by age2pierre on 05/02/17.
 */
let router = require('express').Router();
let fs = require('fs');
const StatUtil = require('../modules/StatUtil');
const ObjectsUtil = require('../modules/ObjectsUtil');
let config = require('../../config');

router.get('/', (req, res, next) => {
  let statData = StatUtil.retrieveData();
  for (let it of statData)
    it.class_name = ObjectsUtil.getData(it.class_id).name
  res.render('stats', {
    classStats: statData,
    objIds: ObjectsUtil.getIndexes()
  });
});

module.exports = router;