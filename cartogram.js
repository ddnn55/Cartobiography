var maxPhotos = 0,
    minDensity = 1.0;

var fs = require('fs'),
    path = require('path');

var path = process.argv[2],
    gridSize = parseInt(process.argv[4]);

fs.readFile(path, 'utf-8', function(err, str) {
  if (err) throw err;

  var photos = JSON.parse(str);
  if(maxPhotos > 0)
    photos = photos.slice(0, maxPhotos);
  process.stderr.write('photo count: ' + photos.length + "\n");

  var left   = photos.map(function(p){ return p.lon; }).reduce(function(a, b) { return a < b ? a : b; });
  var right  = photos.map(function(p){ return p.lon; }).reduce(function(a, b) { return a > b ? a : b; });
  var bottom = photos.map(function(p){ return p.lat; }).reduce(function(a, b) { return a < b ? a : b; });
  var top    = photos.map(function(p){ return p.lat; }).reduce(function(a, b) { return a > b ? a : b; });
  //process.stderr.write('bounds', left, right, bottom, top);

  var width  = right - left,
      height = top - bottom;

  var aspect = width / height;
  //process.stderr.write('aspect', aspect);

  var gridHeight = //Math.floor(gridSize / aspect);
                   gridSize;
  //process.stderr.write('grid size', gridSize, gridHeight);

  //process.stderr.write('grid inside', gridLeft, gridRight, gridBottom, gridTop);

  var averageDensity = photos.length / ( gridSize * gridSize );
  process.stderr.write('averageDensity: ' + averageDensity);

  var grid = new Array(gridSize);
  for(var x = 0; x < gridSize; x++) {
    grid[x] = new Array(gridHeight);
    for(var y = 0; y < gridHeight; y++) {
      grid[x][y] = minDensity;
    }
  }
  
  photos.forEach(function(p) {
    var normalX = (p.lon - left)   / width,
        normalY = (p.lat - bottom) / height;

    var gridX = Math.floor(normalX * gridSize),
        gridY = Math.floor(normalY * gridHeight);

    if(gridX == gridSize) gridX = gridSize - 1;
    if(gridY == gridSize) gridY = gridSize - 1;

    grid[gridX][gridY] += 1.0;
  });

  var densityString = "";

  for(var y = 0; y < gridHeight; y++) {
    for(var x = 0; x < gridSize; x++) {
      densityString += (grid[x][y] + ' ');
    }
    densityString += "\n";
  }

  outName = "data/" + process.argv[3] + ".density."+gridSize;
  fs.writeFile(outName + ".dat", densityString);
  fs.writeFile(outName + ".meta.json",
    JSON.stringify({
      "left" : left,
      "right" : right,
      "bottom" : bottom,
      "top" : top,
      "grid_size" : gridSize
    })
  );

});

