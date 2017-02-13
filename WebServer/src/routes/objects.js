/**
 * Created by age2pierre on 11/02/17.
 */
const router = require('express').Router(),
  ObjectsUtil = require('../modules/ObjectsUtil'),
  RoisUtil = require('../modules/RoisUtil');

router.get('/', (req, res, next) => {
  let objs = ObjectsUtil.getAll();
  res.render('objects', {
    objs: objs
  });
});

router.post('/', (req, res) => {
  ObjectsUtil.serializeData(ObjectsUtil.createObject(req.body.name, req.body.audiofr));
  let objs = ObjectsUtil.getAll();
  res.render('objects', {
    objs: objs
  });
});

router.get('/:objId', (req, res, next) => {
  let obj = ObjectsUtil.getData(req.params.objId);
  let roiIds = obj.rois;
  let roisData = [];
  for (let i = 0; i < roiIds.length; i++)
    roisData.push(RoisUtil.getData(roiIds[i]));
  obj["rois"] = roisData;
  res.render('editObject', {
    obj: obj
  });
});

router.post('/:objId/update', (req, res, next) => {
  ObjectsUtil.updateObject(
    ObjectsUtil.getData(req.params.objId),
    req.body.name,
    req.body.audiofr,
    req.body.suppressedrois
  );
  res.redirect('/objects');
});

router.post('/:objId/delete', (req, res, next) => {
  ObjectsUtil.deleteObject(req.params.objId);
  res.redirect('/objects');
});

module.exports = router;