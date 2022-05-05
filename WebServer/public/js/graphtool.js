/**
 * Created by age2pierre on 05/02/17.
 */
window.onload = () => {
  d3.json('/data/indexObjects.json', (jsonIndexObjects) => {
    for (let id of jsonIndexObjects["index"]) {
      d3.json('/data/objects/' + id + '.json', (jsonObject) => {
        d3.json('/data/reports/histgraph.json', (histData) => {
          MG.data_graphic({
            title: ("Hue histogram of \"" + jsonObject.name + "\""),
            data: histData['hist_of_' + id],
            width: 750,
            height: 400,
            right: 40,
            min_y: 0,
            max_y: 255,
            area: false,
            target: document.getElementById('histograph_' + id),
            show_secondary_x_label: false,
            x_accessor: 'beam_index',
            y_accessor: 'beam_mean',
            show_confidence_band: ['beam_lower', 'beam_upper'],
            x_extended_ticks: false
          });
        });
      });
    }
  });

  d3.json('/data/reports/cldPt.json', (cldPtData) => {
    MG.data_graphic({
      title: "Cloud points for perimeter vs area",
      data: cldPtData['index'],
      chart_type: 'point',
      width: 750,
      height: 750,
      right: 10,
      target: '#cldpt_perimeter_area',
      x_accessor: 'perimeter',
      y_accessor: 'area',
      x_label: 'PERIMETER',
      y_label: 'AREA',
      color_accessor: 'class_id',
      color_type: 'category',
      y_rug: true,
      x_rug: true
    });

    MG.data_graphic({
      title: "Cloud points for min height vs min width",
      data: cldPtData['index'],
      chart_type: 'point',
      width: 750,
      height: 750,
      right: 10,
      target: '#cldpt_min_w_h',
      x_accessor: 'minHeight',
      y_accessor: 'minWidth',
      x_label: 'HEIGHT',
      y_label: 'WIDTH',
      color_accessor: 'class_id',
      color_type: 'category',
      y_rug: true,
      x_rug: true
    });
  });
};
