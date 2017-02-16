const config = require('../../config');
const fs = require('fs');

module.exports = class ScreenshotsUtil {
  /**
   * 
   * @returns {*}
   */
  static getNewId() {
    let rslt;
    do {
      rslt = Math.floor(Math.random() * 10000000) + 10000000 ;
    } while (this._idExists(rslt));
    return rslt;
  }

  /**
   *
   * @returns {[Number]}
   */
  static getIndexes()  {
    let data = fs.readFileSync(config.dataPath + 'indexScreenshots.json');
    return (JSON.parse(data)).index;
  }

  /**
   *
   * @param id
   * @returns {boolean}
   * @private
   */
  static _idExists(id)  {
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
  static getData(id)  {
    if (!this._idExists(id))
      return {};
    let data = fs.readFileSync(config.dataPath + 'screenshots/' + id + '.json');
    return JSON.parse(data);
  }

  /**
   * 
   * @param screenshotData
   * @param id
   * @returns {*}
   */
  static removeRoi(screenshotData, id) {
    let index = screenshotData["rois_id"].indexOf(id);
    if (index > -1)
      screenshotData["rois_id"].splice(index, 1);
    return screenshotData;
  }

  /**
   * 
   * @param screenshotData
   */
  static serializeData(screenshotData) {
    fs.writeFileSync(config.dataPath + 'screenshots/' + screenshotData.self_id + '.json',
      JSON.stringify(screenshotData, null, 2));
  }
};