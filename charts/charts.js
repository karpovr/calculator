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
  var frameWidth = 0.25;

  // Preview
  this.displayInViewport({
    x0: 0,
    y0: height,
    x1: width,
    y1: height - previewHeight,
    lineWidth: 1
  });

  // Frame view
  this.displayInViewport({
    x0: 0,
    y0: height - previewHeight,
    x1: width,
    y1: 0,
    lineWidth: 2,
    start: Math.floor(data.columns[0].length * (1 - frameWidth)),
    end: data.columns[0].length,
    labels: 5
  });

}

var proto = Chart.prototype;

// Display chart or its part in given viewport of canvas
proto.displayInViewport = function (opts) {
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
      labels = opts.labels || 0,
      viewportWidth = x1 - x0,
      viewportHeight = y1 - y0,
      deltaX = Math.floor(totalPoints / viewportWidth) || 1,
      extremes = {
        minY: Infinity,
        maxY: -Infinity
      };

  context.clearRect(x0, y0, viewportWidth, viewportHeight);

  // Calculate extremes & ratios for given data range & viewport
  for (var columnIndex = 0, column; (column = columns[columnIndex]); columnIndex++) {
    var column_key = column[0];
    if (column_key === "x") { continue; }
    var minY = Infinity,
        maxY = -Infinity;
    for (var i = start; i <= end && i <= totalPoints; i++) {
      var value = column[i];
      minY = value < minY ? value : minY;
      maxY = value > maxY ? value : maxY;
    }
    extremes[column_key] = [minY, maxY];
    extremes.minX = columns[0][start];
    extremes.maxX = columns[0][i-1];
    extremes.minY = minY < extremes.minY ? minY : extremes.minY;
    extremes.maxY = maxY > extremes.maxY ? maxY : extremes.maxY;
    extremes.xRatio = viewportWidth / (extremes.maxX - extremes.minX);
    extremes.yRatio = viewportHeight / extremes.maxY;
  }
  console.log(extremes);

  context.save();
  context.setTransform(extremes.xRatio, 0, 0, extremes.yRatio, -extremes.minX * extremes.xRatio, -extremes.maxY * extremes.yRatio + y1);
  for (var i = 0; i < labels; i++) {
    context.beginPath();
    var xa = extremes.minX;
    var xb = extremes.maxX;
    var ya = extremes.maxY / (i + 1);
    var yb = extremes.maxY / (i + 1);
    context.moveTo( extremes.minX, extremes.maxY / (i + 1) );
    context.lineTo( extremes.maxX, extremes.maxY / (i + 1) );
  }
  context.restore();
  context.strokeStyle = '#eee';
  context.lineWidth = 1;
  context.stroke();

  // Display chart for given data range
  for (var columnIndex = 0, column; (column = columns[columnIndex]); columnIndex++) {
    var column_key = column[0];
    if (column_key === "x") { continue; }
    context.save();
    context.setTransform(extremes.xRatio, 0, 0, extremes.yRatio, -extremes.minX * extremes.xRatio, -extremes.maxY * extremes.yRatio + y1);
    context.beginPath();
    var x = columns[0][start];
    var y = column[start];
    context.moveTo(x, y);
    for (var i = start; i <= end && i <= totalPoints; i += deltaX) {
      x = columns[0][i];
      y = column[i];
      context.lineTo(x, y);
    }
    context.restore();
    context.strokeStyle = data.colors[column_key];
    context.lineWidth = lineWidth;
    context.stroke();
  }
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
