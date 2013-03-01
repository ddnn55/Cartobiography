var gridWidth = 1024,
    padding = 0.2;

var fs = require('fs'),
    path = require('path');

var path = process.argv[2];

fs.readFile(path, 'utf-8', function(err, str) {
  if (err) throw err;

  var photos = JSON.parse(str);

  var left   = photos.map(function(p){ return p.lon; }).reduce(function(a, b) { return a < b ? a : b; });
  var right  = photos.map(function(p){ return p.lon; }).reduce(function(a, b) { return a > b ? a : b; });
  var bottom = photos.map(function(p){ return p.lat; }).reduce(function(a, b) { return a < b ? a : b; });
  var top    = photos.map(function(p){ return p.lat; }).reduce(function(a, b) { return a > b ? a : b; });
  //process.stderr.write('bounds', left, right, bottom, top);

  var width  = right - left,
      height = top - bottom;

  var aspect = width / height;
  //process.stderr.write('aspect', aspect);

  var gridHeight = //Math.floor(gridWidth / aspect);
                   gridWidth;
  //process.stderr.write('grid size', gridWidth, gridHeight);

  var gridLeft   = Math.floor(gridWidth * padding),
      gridRight  = Math.floor(gridWidth - gridWidth * padding),
      gridBottom = Math.floor(gridHeight * padding),
      gridTop    = Math.floor(gridHeight - gridHeight * padding);
  //process.stderr.write('grid inside', gridLeft, gridRight, gridBottom, gridTop);

  var internalGridWidth  = gridRight - gridLeft,
      internalGridHeight = gridTop - gridBottom;

  var averageDensity = photos.length / ( internalGridWidth * internalGridHeight );
  process.stderr.write('averageDensity: ' + averageDensity);

  var col = [];
  for(var y = 0; y < gridHeight; y++) {
    col.push(-1);
  }

  var grid = new Array(gridWidth);
  for(var x = 0; x < gridWidth; x++) {
    grid[x] = new Array(gridHeight);
    for(var y = 0; y < gridHeight; y++) {
      grid[x][y] = -1;
    }
  }
  
  for(var x = gridLeft; x <= gridRight; x++) {
    for(var y = gridBottom; y <= gridTop; y++) {
      grid[x][y] = averageDensity;
    }
  }
  
  photos.forEach(function(p) {
    var normalX = (p.lon - left)   / width,
        normalY = (p.lat - bottom) / height;

    var gridX = Math.floor(gridLeft + normalX * internalGridWidth),
        gridY = Math.floor(gridBottom + normalY * internalGridHeight);

    grid[gridX][gridY] += 1.0;

    process.stderr.write(grid[gridX][gridY]+'\n');
  });

  for(var x = 0; x < gridWidth; x++) {
    for(var y = 0; y < gridHeight; y++) {
      if(grid[x][y] == -1)
        grid[x][y] = 2.0 * averageDensity;
    }
  }

  for(var y = 0; y < gridHeight; y++) {
    var lastValue = -1;
    for(var x = 0; x < gridWidth; x++) {
      process.stdout.write(grid[x][y] + ' ');
      if(lastValue > -1 && lastValue != grid[x][y])
        process.stderr.write('saw new value within this row\n');
      //else
      //  process.stderr.write('saw same value within this row\n');
        
      lastValue = grid[x][y];
    }
    process.stdout.write("\n");
  }

});

