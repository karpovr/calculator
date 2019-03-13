/*
 * Telegram contest: Charts
 *
 * Copyright 2019 Roman Karpov (roman.karpov@gmail.com)
 * Released under the MIT license
 *
 * Date: 2019-03-11T09:59Z
 */

function Chart (canvas, data) {
  if (!canvas.getContext) {
    throw new Error("Your browser does not support canvas");
  }

  var self = this;
  var context = canvas.getContext("2d");
  this.data = data;
  this.canvas = canvas;
  this.context = context;

  var height = canvas.height;
  var width = canvas.width;
  var previewWidth = width;
  var previewHeight = height >> 3;
  var mainWidth = width;
  var mainHeight = height - previewHeight;
  previewO = {x: 0, y: height};
  mainO = {x: 0, y: height};
  var frameEndIndex = data.columns[0].length - 1;
  var frameStartIndex = frameEndIndex - (frameEndIndex >> 3) ;

  var columns = data.columns;
  var totalColumns = data.columns.length;

  var totalPoints = data.columns[0].length - 1;
  var xRatio = Math.floor(totalPoints / width);

  var extremes = {
    min: Infinity,
    max: -Infinity
  };
  for (var columnIndex = 0, column; (column = columns[columnIndex]); columnIndex++) {
    var column_key = column[0];
    if (column_key === "x") { continue; }
    var min = Infinity,
        max = -Infinity;
    for (var i = 1; i < totalPoints; i++) {
      var value = column[i];
      min = value < min ? value : min;
      max = value > max ? value : max;
    }
    extremes[column_key] = [min, max];
    extremes.min = min < extremes.min ? min : extremes.min;
    extremes.max = max > extremes.max ? max : extremes.max;
    extremes.previewRatio = previewHeight / extremes.max;
  }
  this.data.extremes = extremes;
  console.log(extremes);

  for (var columnIndex = 0, column; (column = columns[columnIndex]); columnIndex++) {
    var column_key = column[0];
    if (column_key === "x") { continue; }
    context.beginPath();
    context.moveTo(0, 0);
    //for (var i = 1; i < totalPoints; i += xRatio) {
    //  var x = data.columns[0][i];
    //  var y = data.columns[columnIndex][i] * extremes.previewRatio;
    //  context.lineTo(x, y);
    //}
    context.closePath();
    context.stroke();
  }
}
