/*
 * Telegram contest: Charts
 *
 * Copyright 2019 Roman Karpov (roman.karpov@gmail.com)
 * Released under the MIT license
 *
 * Date: 2019-03-11T09:59Z
 */

function Chart(data, canvas) {
  if (!canvas.getContext) {
    throw new Error("Your browser does not support canvas");
  }
  var context = canvas.getContext("2d");
  this.data = data;
  this.canvas = canvas;
  this.context = context;

  var settings = {};
  settings.displayed = Object.keys(data.names);
  settings.totalPoints = data.columns[0].length - 1;
  settings.begin = settings.totalPoints - (settings.totalPoints >> 2),
  settings.end = settings.totalPoints,
  settings.preview = {
    x0: 0,
    y0: canvas.height,
    x1: canvas.width,
    y1: Math.floor(canvas.height - canvas.height / 10),
    lineWidth: 1,
    width: canvas.width,
    height: Math.floor(canvas.height / 10)
  };
  settings.view = {
    x0: 0,
    y0: canvas.height - 2 * settings.preview.height,
    x1: canvas.width,
    y1: 0,
    width: canvas.width,
    height: canvas.height - 2 * settings.preview.height,
    lineWidth: 2,
    labels: 5
  };
  this.settings = settings;

  this.draw();
}

Chart.prototype.draw = function () {
  // Preview
  //this.drawPreview();
  this.displayInViewport(this.settings.preview);

  // Frame view
  this.displayInViewport(this.settings.view);
};

Chart.prototype.drawPreview = function () {

  var context = this.context;
  var begin = 1;
  var end = this.settings.totalPoints;
  var extremes = this.getExtremes(begin, end);
  var xRatio = this.settings.preview.width / extremes.xDelta;
  var yRatio = this.settings.preview.height / extremes.yDelta;
  var xOffset = -extremes.minX * xRatio;
  var yOffset = extremes.maxY * yRatio + this.settings.preview.y1;
  var xStep = Math.floor( (end - begin) / this.settings.preview.width ) || 1;

  context.save();
  context.lineWidth = this.settings.preview.lineWidth;

  var i, j, column_key, column, x0, y0, x, y;
  for (i = 0, column; (column = this.data.columns[i]); i++) {
    column_key = column[0];
    if (column_key === "x" || this.settings.displayed.indexOf(column_key) < 0) {
      continue;
    }
    context.strokeStyle = this.data.colors[column_key];
    context.save();
    context.setTransform(xRatio, 0, 0, -yRatio, xOffset, yOffset);
    x0 = this.data.columns[0][begin];
    y0 = column[begin];
    context.beginPath();
    context.moveTo(x0, y0);
    for (j = begin; j <= end; j += xStep) {
      x = this.data.columns[0][j];
      y = column[j];
      context.lineTo(x, y);
    }
    context.restore();
    context.stroke();
  }
  context.restore();
};

Chart.prototype.drawView = function () {

};

// Calculate extremes for given data range
Chart.prototype.getExtremes = function (begin, end) {
  var i,
      j,
      column,
      column_key,
      value,
      minY,
      maxY,
      begin = typeof begin !== "undefined" ? begin : this.settings.begin,
      end = typeof end !== "undefined" ? end : this.settings.end,
      extremes = {};

  for (i = 0, column; (column = this.data.columns[i]); i++) {
    column_key = column[0];
    if (column_key === "x") {
      extremes.minX = column[begin];
      extremes.maxX = column[end];
      continue;
    } else if (this.settings.displayed.indexOf(column_key) < 0) {
      continue;
    }
    minY = maxY = 0;
    for (j = begin; j <= end; j++) {
      value = column[j];
      minY = value < minY ? value : minY;
      maxY = value > maxY ? value : maxY;
    }
    extremes[column_key] = [minY, maxY];
    extremes.minY = (typeof extremes.minY === "undefined" || minY < extremes.minY) ? minY : extremes.minY;
    extremes.maxY = (typeof extremes.maxY === "undefined" || maxY > extremes.maxY) ? maxY : extremes.maxY;
    extremes.xDelta = extremes.maxX - extremes.minX;
    extremes.yDelta = extremes.maxY - extremes.minY;
  }
  return extremes;
};

// Display chart or its part in given viewport of canvas
Chart.prototype.displayInViewport = function (opts) {
  var context = this.context,
      data = this.data,
      columns = data.columns,
      x0 = opts.x0,
      x1 = opts.x1,
      y0 = opts.y0,
      y1 = opts.y1,
      width = opts.width,
      height = opts.height,
      begin = opts.begin || 1,
      end = opts.end || this.settings.totalPoints,
      lineWidth = opts.lineWidth || 1,
      labels = opts.labels || 0,
      xStep = Math.floor( (end - begin) / width ) || 1;

  context.clearRect(x0, y0, width, -height);

  // Get extremes, calculate ratios for given data range & viewport
  var extremes = this.getExtremes(begin, end);
  var xRatio = width / extremes.xDelta;
  var yRatio = height / extremes.yDelta;

  // Draw labels
  context.save();
  context.font = "12px sans-serif";
  context.textBaseline = "bottom";
  context.strokeStyle = "#eee";
  context.fillStyle = "#aaa";
  context.lineWidth = 1;
  var labelStep = Math.round( (extremes.maxY - extremes.minY) / labels);
  var power = Math.abs(labelStep).toString().length - 1;
  labelStep = Math.round( labelStep / Math.pow(10, power) ) * Math.pow(10, power);
  var yLine = Math.round(extremes.minY / labelStep) * labelStep;
  while ( yLine < extremes.maxY) {
    context.save();
    context.setTransform(xRatio, 0, 0, -yRatio, -extremes.minX * xRatio, extremes.maxY * yRatio + y1);
    context.setTransform(xRatio, 0, 0, -yRatio, -extremes.minX * xRatio, extremes.maxY * yRatio + y1);
    context.beginPath();
    var xBeg = extremes.minX;
    var xEnd = extremes.maxX;
    context.moveTo( xBeg, yLine );
    context.lineTo( xEnd, yLine );
    context.restore();
    context.stroke();
    var labelPosition = transform(xBeg, yLine, xRatio, -yRatio, -extremes.minX * xRatio, extremes.maxY * yRatio + y1);
    context.fillText(yLine, labelPosition[0], labelPosition[1] - 5);
    yLine = yLine + labelStep;
  }
  context.restore();

  // Display data range in given viewport
  context.save();
  for (var columnIndex = 0, column; (column = columns[columnIndex]); columnIndex++) {
    var column_key = column[0];
    if (column_key === "x") { continue; }
    context.save();
    context.setTransform(xRatio, 0, 0, -yRatio, -extremes.minX * xRatio, extremes.maxY * yRatio + y1);
    context.beginPath();
    var x = columns[0][begin];
    var y = column[begin];
    context.moveTo(x, y);
    for (var i = begin; i <= end; i += xStep) {
      x = columns[0][i];
      y = column[i];
      context.lineTo(x, y);
    }
    context.restore();
    context.strokeStyle = data.colors[column_key];
    context.lineWidth = lineWidth;
    context.stroke();
  }
  context.restore();
};

function transform(x, y, xRatio, yRatio, xOffset, yOffset) {
  return [
    x * xRatio + xOffset,
    y * yRatio + yOffset
  ];
}
