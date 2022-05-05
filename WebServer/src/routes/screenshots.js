/**
 * Created by age2pierre on 16/02/17.
 */
const router = require('express').Router(),
  child_process = require('child_process'),
  RoisUtil = require('../modules/RoisUtil'),
  ObjectsUtil = require('../modules/ObjectsUtil'),
  config = require('../../config'),
  fs = require('fs'),
  ScreenshotsUtil = require('../modules/ScreenshotsUtil');

router.get('/', (req, res, next) => {
    res.render('screenshots');
  }
);

router.get('/snap', (req, res, next) => {
    let newId = ScreenshotsUtil.getNewId();
    console.log("screenid : " + newId);
    console.log(config.buildPath + 'ScreenshotUtil/');
    child_process.execSync('./screenshotUtil -id ' + newId, {
      cwd: (config.buildPath + 'ScreenshotUtil/')
    });
    let scrData = ScreenshotsUtil.getData(newId);
    let data = JSON.parse(fs.readFileSync(config.dataPath + 'unreviewedRois.json'));
    data["index"] = data["index"].concat(scrData["rois_id"]);
    fs.writeFileSync(config.dataPath + 'unreviewedRois.json', JSON.stringify(data, null, 2));
    res.render('screenshots', {
      screenPicPath: scrData.undist_lightOn
    });
  }
);

router.get('/review', (req, res, next) => {
  let data = JSON.parse(fs.readFileSync(config.dataPath + 'unreviewedRois.json'));
  let roiToReview = [];
  for(let id of data["index"])
    roiToReview.push(RoisUtil.getData(id));
  res.render('review', {
    rois: roiToReview,
    objs: ObjectsUtil.getAll()
  });
  }
);

router.post('/review', (req, res, next) => {
  let data = JSON.parse(fs.readFileSync(config.dataPath + 'unreviewedRois.json'));
  for(let roiId in req.body) {
    let val = req.body[roiId];
    if (val == "dismiss")
      RoisUtil.deleteRoi(id);
    else if(val != "todo")
      RoisUtil.serializeData(RoisUtil.setClass(RoisUtil.getData(roiId),val));

    if(val != "todo") {
      let index = data["index"].indexOf(id);
      data["index"].splice(index, 1);
    }
  }
  fs.writeFileSync(config.dataPath + 'unreviewedRois.json', JSON.stringify(data, null, 2));
    res.render('screenshots');
  }
);

router.get('/predict', (req, res, next) => {
    let newId = ScreenshotsUtil.getNewId();
    console.log("screenid : " + newId);
    child_process.execSync('./screenshotUtil -id ' + newId, {
      cwd: (config.buildPath + 'ScreenshotUtil/')
    });
    let scrData = ScreenshotsUtil.getData(newId);
    let predictionRslts = {};
    for(let roisId of scrData["rois_id"]) {
      let strClassRslt = child_process.execSync('./predictUtil -id ' + roisId, {
        cwd: (config.buildPath + 'PredictUtil/')
      });
      if(!(strClassRslt in predictionRslts))
        predictionRslts[strClassRslt] = 0;
      predictionRslts[strClassRslt] += 1;
    }
    let audioOutput = '\"Il y a ';
    for(let classId in predictionRslts) {
      let objData = ObjectsUtil.getData(parseInt(classId));
      audioOutput += (predictionRslts[classId] + ' ' + objData["audio_fr"] + ", ");
    }
    audioOutput += '\"';
    child_process.execSync('pico2wave -l fr-FR -w tts.wav ' + audioOutput + " && aplay tts.wav && rm tts.wav", {
      cwd: config.dataPath
    });
    res.redirect('/screenshots/');
  }
);

module.exports = router;
