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
  settings.total = data.columns[0].length - 1;
  settings.begin = settings.total - (settings.total >> 2);
  settings.end = settings.total;
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
  this.clearView(this.settings.preview);
  this.renderView(this.settings.preview, 1, this.settings.total);

  // Frame view
  this.clearView(this.settings.view);
  this.drawLabels(this.settings.view, this.settings.begin, this.settings.end);
  this.renderView(this.settings.view, this.settings.begin, this.settings.end);

};

// Calculate extremes for given data range
Chart.prototype.getExtremes = function (begin, end) {

  begin = typeof begin !== "undefined" ? begin : this.settings.begin;
  end = typeof end !== "undefined" ? end : this.settings.end;

  var i,
      j,
      column,
      column_key,
      value,
      minY,
      maxY,
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

Chart.prototype.clearView = function (view) {
  var context = this.context;
  context.clearRect(view.x0, view.y0, view.width, -view.height);
};

Chart.prototype.renderView = function (view, begin, end) {

  var context = this.context;
  var extremes = this.getExtremes(begin, end);
  var xRatio = view.width / extremes.xDelta;
  var yRatio = view.height / extremes.yDelta;
  var xOffset = -extremes.minX * xRatio;
  var yOffset = extremes.maxY * yRatio + view.y1;
  var xStep = Math.floor( (end - begin) / view.width ) || 1;

  context.save();
  context.lineWidth = view.lineWidth;

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

Chart.prototype.drawLabels = function (view, begin, end) {

  var context = this.context;
  var extremes = this.getExtremes(begin, end);
  var xRatio = view.width / extremes.xDelta;
  var yRatio = view.height / extremes.yDelta;
  var xOffset = -extremes.minX * xRatio;
  var yOffset = extremes.maxY * yRatio + view.y1;

  // Draw labels
  context.save();
  context.font = "12px sans-serif";
  context.textBaseline = "bottom";
  context.strokeStyle = "#eee";
  context.fillStyle = "#aaa";
  context.lineWidth = 1;
  var yStep = Math.round(extremes.yDelta / view.labels);
  var power = Math.abs(yStep).toString().length - 1;
  yStep = Math.round( yStep / Math.pow(10, power) ) * Math.pow(10, power);
  var y = Math.round(extremes.minY / yStep) * yStep;
  while ( y < extremes.maxY) {
    context.save();
    context.setTransform(xRatio, 0, 0, -yRatio, xOffset, yOffset);
    context.beginPath();
    var x0 = extremes.minX;
    var x1 = extremes.maxX;
    context.moveTo(x0, y);
    context.lineTo(x1, y);
    context.restore();
    if (y === 0) {
      context.strokeStyle = "#ccc";
    } else {
      context.strokeStyle = "#eee";
    }
    context.stroke();
    var labelPosition = transform(x0, y, xRatio, -yRatio, xOffset, yOffset);
    context.fillText(y, labelPosition[0], labelPosition[1] - 5);
    y = y + yStep;
  }
  context.restore();
};

function transform(x, y, xRatio, yRatio, xOffset, yOffset) {
  return [
    x * xRatio + xOffset,
    y * yRatio + yOffset
  ];
}
