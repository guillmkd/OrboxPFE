const config = require('../../config'),
  fs = require('fs'),
  ScreenshotsUtil = require('../modules/ScreenshotsUtil');

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