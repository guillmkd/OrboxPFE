/**
 * Created by age2pierre on 05/02/17.
 */
const config = require('../../config'),
  fs = require('fs'),
  RoisUtil = require('../modules/RoisUtil');

module.exports = class ObjectsUtil {
  /**
   *
   * @returns {[Number]}
   */
  static getIndexes() {
    let data = fs.readFileSync(config.dataPath + 'indexObjects.json');
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
    let data = fs.readFileSync(config.dataPath + 'objects/' + id + '.json');
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
   * @param objData
   */
  static serializeData(objData) {
    fs.writeFileSync(config.dataPath + 'objects/' + objData.self_id + '.json',
      JSON.stringify(objData, null, 2));
  }

  /**
   *
   * @param name
   * @param msg
   * @returns {{self_id: number, name: *, audio_fr: *, rois: Array}}
   */
  static createObject(name, msg) {
    let newId = Math.max(...this.getIndexes()) + 1;
    let data = fs.readFileSync(config.dataPath + 'indexObjects.json');
    let jData = JSON.parse(data);
    jData["index"].push(newId);
    fs.writeFileSync(config.dataPath + 'indexObjects.json', JSON.stringify(jData, null, 2));
    return {
      self_id: newId,
      name: name,
      audio_fr: msg,
      rois: []
    }
  }

  /**
   *
   * @param id
   */
  static deleteObject(id) {
    if (this._idExists(id)) {
      fs.unlinkSync(config.dataPath + 'objects/' + id + '.json');
      let data = fs.readFileSync(config.dataPath + 'indexObjects.json');
      let jData = JSON.parse(data);
      let index = jData["index"].indexOf(id);
      jData["index"].splice(index, 1);
      fs.writeFileSync(config.dataPath + 'indexObjects.json', JSON.stringify(jData, null, 2));
    }
  }

  static updateObject(objData, name, msg, suppressedRois) {
    if (typeof suppressedRois === 'string')
      suppressedRois = [suppressedRois];
    for (let strid of suppressedRois) {
      let id = parseInt(strid);
      RoisUtil.deleteRoi(id);
      let index = objData["rois"].indexOf(id);
      if (index > -1)
        objData["rois"].splice(index, 1);
    }
    if (name.length > 0)
      objData["name"] = name;
    if (msg.length > 0)
      objData["audio_fr"] = msg;
    this.serializeData(objData);
  }
};