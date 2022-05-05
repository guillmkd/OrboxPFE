const config = require('../../config'),
  fs = require('fs'),
  ScreenshotsUtil = require('../modules/ScreenshotsUtil'),
  ObjectsUtil = require('../modules/ObjectsUtil');

module.exports = class RoisUtil {
  /**
   *
   * @returns {[Number]}
   */
  static getIndexes() {
    let data = fs.readFileSync(config.dataPath + 'indexRois.json');
    return (JSON.parse(data)).index;
  }

  /**
   *
   * @param id
   * @returns {boolean}
   * @private
   */
  static _idExists(id) {
    for (let it of this.getIndexes())
      if (it == id)
        return true;
    return false;
  }

  /**
   *
   * @param id
   * @returns {{}}
   */
  static getData(id) {
    if (!this._idExists(id))
      return {};
    let data = fs.readFileSync(config.dataPath + 'rois/' + id + '.json');
    return JSON.parse(data);
  }

  /**
   *
   * @returns {Array}
   */
  static getAll() {
    let rslt = [];
    for (let id of this.getIndexes())
      rslt.push(this.getData(id));
    return rslt;
  }

  /**
   *
   * @param roiData
   * @param classId
   * @returns {*}
   */
  static setClass(roiData, classId) {
    let intClassId = parseInt(classId)
    roiData["class_id"] = intClassId;
    let objData = ObjectsUtil.getData(classId);
    if(objData["rois"].indexOf(roiData.self_id) < 0) {
      objData["rois"].push(roiData.self_id);
      ObjectsUtil.serializeData(objData);
    }
    if (!this._idExists(roiData.self_id)) {
      let indexData = JSON.parse(fs.readFileSync(config.dataPath + 'indexRois.json'));
      indexData["index"].push(roiData.self_id);
      fs.writeFileSync(config.dataPath + 'indexRois.json', JSON.stringify(indexData, null, 2));
    }
    return roiData;
  }

  /**
   *
   * @param roiData
   */
  static serializeData(roiData) {
    fs.writeFileSync(config.dataPath + 'rois/' + roiData.self_id + '.json',
      JSON.stringify(roiData, null, 2));
  }

  /**
   *
   * @param id
   */
  static deleteRoi(id) {
    if (this._idExists(id)) {
      let roiData = this.getData(id);
      // delete image files
      fs.unlinkSync(config.dataPath + roiData.color_pic_path);
      fs.unlinkSync(config.dataPath + roiData.mask_pic_path);
      // remove from parent screenshot files
      ScreenshotsUtil.serializeData(
        ScreenshotsUtil.removeRoi(
          ScreenshotsUtil.getData(roiData.parent_id),
          roiData.self_id
        )
      );
      // delete self json file
      fs.unlinkSync(config.dataPath + 'rois/' + id + '.json');
      // remove from index
      let data = fs.readFileSync(config.dataPath + 'indexRois.json');
      let jData = JSON.parse(data);
      let index = jData["index"].indexOf(id);
      jData["index"].splice(index, 1);
      fs.writeFileSync(config.dataPath + 'indexRois.json', JSON.stringify(jData, null, 2));

    }
  }
};