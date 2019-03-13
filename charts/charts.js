/*
 * Telegram contest: Charts
 *
 * Copyright 2019 Roman Karpov (roman.karpov@gmail.com)
 * Released under the MIT license
 *
 * Date: 2019-03-11T09:59Z
 */

function Chart(canvas, data) {
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
  var previewHeight = Math.floor(height / 10);

  this.displayChart({
    x0: 0,
    y0: height,
    x1: width,
    y1: height - previewHeight,
    lineWidth: 1
  });

  this.displayChart({
    x0: 0,
    y0: height - previewHeight,
    x1: width,
    y1: 0,
    lineWidth: 2,
    start: 1,
    end: 100
  });
}

var proto = Chart.prototype;

// Display chart or its part in given viewport of canvas
proto.displayChart = function (opts) {
  var context = this.context,
      data = this.data,
      columns = data.columns,
      totalPoints = columns[0].length - 1;
      x0 = opts.x0,
      x1 = opts.x1,
      y0 = opts.y0,
      y1 = opts.y1,
      start = opts.start || 1,
      end = opts.end || totalPoints,
      lineWidth = opts.lineWidth || 1,
      viewportWidth = x1 - x0,
      viewportHeight = y1 - y0,
      extremes = {
        minY: Infinity,
        maxY: -Infinity
      };

  // Calculate extremes & ratios for given data range & viewport
  for (var columnIndex = 0, column; (column = columns[columnIndex]); columnIndex++) {
    var column_key = column[0];
    if (column_key === "x") { continue; }
    var minY = Infinity,
        maxY = -Infinity;
    for (var i = start; i <= end; i++) {
      var value = column[i];
      minY = value < minY ? value : minY;
      maxY = value > maxY ? value : maxY;
    }
    extremes[column_key] = [minY, maxY];
    extremes.minX = columns[0][1];
    extremes.maxX = columns[0][i-1];
    extremes.minY = minY < extremes.minY ? minY : extremes.minY;
    extremes.maxY = maxY > extremes.maxY ? maxY : extremes.maxY;
    extremes.xRatio = viewportWidth / (extremes.maxX - extremes.minX);
    extremes.yRatio = viewportHeight / extremes.maxY;
  }

  // Display chart for given data range
  for (var columnIndex = 0, column; (column = columns[columnIndex]); columnIndex++) {
    var column_key = column[0];
    if (column_key === "x") { continue; }
    context.strokeStyle = data.colors[column_key];
    context.lineWidth = lineWidth;
    context.beginPath();
    var xBegin = columns[0][1];
    var yBegin = column[1];
    var translatedBegin = translate(xBegin, yBegin, extremes.xRatio, extremes.yRatio, -extremes.minX * extremes.xRatio, -extremes.maxY * extremes.yRatio + y1);
    context.moveTo(translatedBegin[0], translatedBegin[1]);
    for (var i = start; i <= end; i++) {
      var x = columns[0][i];
      var y = column[i];
      var translated = translate(x, y, extremes.xRatio, extremes.yRatio, -extremes.minX * extremes.xRatio, -extremes.maxY * extremes.yRatio + y1);
      context.lineTo(translated[0], translated[1]);
    }
    context.stroke();
  }

  console.log(extremes);
}

function translate(x, y, xRatio, yRatio, dx, dy) {
  var x1 = x * xRatio + dx;
  var y1 = y * yRatio + dy;
  return [x1, y1];
}

function zip() {
  var argC = arguments.length;
  var i = 0;
  var zipped = [];
  while(arguments[0][i]) {
    zipped[i] = [];
    for (var j = 0; j < argC; j++) {
      zipped[i][j] = arguments[j][i];
    }
    i++;
  }
  return zipped;
}
