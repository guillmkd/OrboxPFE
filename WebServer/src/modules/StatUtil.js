/**
 * Created by age2pierre on 05/02/17.
 */
const config = require('../../config'),
  fs = require('fs');

module.exports = class StatUtil{
  /**
   * Read json file and return content
   * @returns {[*]} 
   */
  static retrieveData() {
    let data = fs.readFileSync(config.dataPath + 'reports/stats.json');
    return (JSON.parse(data)).index;
  }

  /**
   * Read json file and return content
   * @returns {[*]}
   */
  static retrieveHistogram() {
    let data = fs.readFileSync(config.dataPath + 'reports/histgraph.json');
    return JSON.parse(data);
  }
};