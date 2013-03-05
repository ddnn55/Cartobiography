var step = 5;
var gridSize = 1025;
var padding = 0.0;

var fs = require('fs');

var path = process.argv[2];
var photosPath = process.argv[3];

var photos = JSON.parse(fs.readFileSync(photosPath, 'utf-8'));
  var latLngLeft   = photos.map(function(p){ return p.lon; }).reduce(function(a, b) { return a < b ? a : b; });
  var latLngRight  = photos.map(function(p){ return p.lon; }).reduce(function(a, b) { return a > b ? a : b; });
  var latLngBottom = photos.map(function(p){ return p.lat; }).reduce(function(a, b) { return a < b ? a : b; });
  var latLngTop    = photos.map(function(p){ return p.lat; }).reduce(function(a, b) { return a > b ? a : b; });
  //process.stderr.write('bounds', left, right, bottom, top);

  var latLngWidth  = latLngRight - latLngLeft,
      latLngHeight = latLngTop - latLngBottom;

  var latLngAspect = latLngWidth / latLngHeight;
  //process.stderr.write('aspect', aspect);

  var gridLeft = 0,
      gridBottom = 0,
      gridRight = gridSize-1,
      gridTop = gridSize-1
  /*var gridLeft   = Math.floor(gridSize * padding),
      gridRight  = Math.floor(gridSize - 1 - gridSize * padding),
      gridBottom = Math.floor(gridSize * padding),
      gridTop    = Math.floor(gridSize - 1 - gridSize * padding);*/
  //process.stderr.write('grid inside', gridLeft, gridRight, gridBottom, gridTop);

  //var internalGridSize  = gridRight - gridLeft;
  var internalGridSize = gridSize;


fs.readFile(path, 'utf-8', function(err, str) {
  var entries = str.split('\n');
  entries = entries.slice(0, entries.length-1);
  var points = entries.map(function(entry){
    return entry.split(' ').map(parseFloat);
  });

  var grid = new Array(gridSize);
  for(var x = 0; x < gridSize; x++)
    grid[x] = new Array(gridSize);
  var p = 0;
  for(var r = 0; r < gridSize; r++) {
    for(var c = 0; c < gridSize; c++) {
      grid[c][r] = points[p];
      p++;
    }
  }

  console.log('<svg xmlns="http://www.w3.org/2000/svg" version="1.1" width="'+gridSize+'" height="'+gridSize+'">');
  
  for(var x = 0; x < gridSize; x += step) {
    console.log('<polyline fill="none" stroke="black" stroke-width="0.1px" points="');
    for(var y = 0; y < gridSize; y += step) {
      var p = grid[x][y]; 
      process.stdout.write(p[0] + ',' + (gridSize - p[1]) + ' ');
      //console.log('<rect x="'+p[0]+'" y="'+(1024-p[1])+'" width="1" height="1" fill="black" stroke="none"/>');
    }
    console.log('"/>');
  }
  for(var y = 0; y < gridSize; y += step) {
    console.log('<polyline fill="none" stroke="black" stroke-width="0.1px" points="');
    for(var x = 0; x < gridSize; x += step) {
      var p = grid[x][y]; 
      process.stdout.write(p[0] + ',' + (gridSize - p[1]) + ' ');
      //console.log('<rect x="'+p[0]+'" y="'+(1024-p[1])+'" width="1" height="1" fill="black" stroke="none"/>');
    }
    console.log('"/>');
  }

  /*for(i in points) {
    var y = Math.floor(i / 1025);
    var x = i - y * 1025;
    if( ( x % step == 0 ) || ( y % step == 0 ) ) {
      var p = points[i];
      console.log('<rect x="'+p[0]+'" y="'+(1024-p[1])+'" width="1" height="1" fill="black" stroke="none"/>');
    }
  }*/
 
  process.stderr.write('internalGridSize: ' + internalGridSize);

  var project = function(pt){
    pt[0] =             (gridSize-1) * (pt[0] - latLngLeft)   / latLngWidth;
    pt[1] = (gridSize-1) - (gridSize-1) * (pt[1] - latLngBottom) / latLngHeight;

    var left   = Math.max(Math.floor(pt[0]), 0),
        right  = Math.min(left + 1, gridSize-1),
        bottom = Math.max(Math.floor(pt[1]), 0),
        top    = Math.min(bottom + 1, gridSize-1);
    var xFraction = pt[0] - left;
    var yFraction = pt[1] - bottom;
    
    var sw = grid[left][bottom];
    var nw = grid[left][top];
    var se = grid[right][bottom];
    var ne = grid[right][top];

    var dSW = Math.sqrt(        xFraction  *        xFraction  +        yFraction  *        yFraction );
    var dNW = Math.sqrt(        xFraction  *        xFraction  + (1.0 - yFraction) * (1.0 - yFraction) );
    var dSE = Math.sqrt( (1.0 - xFraction) * (1.0 - xFraction) +        yFraction  *        yFraction );
    var dNE = Math.sqrt( (1.0 - xFraction) * (1.0 - xFraction) + (1.0 - yFraction) * (1.0 - yFraction) );
    var totalWeight = 1.0/dSW + 1.0/dNW + 1.0/dSE + 1.0/dNE;

    process.stderr.write('pt: ' + pt[0] + ', ' + pt[1] + ' : ' + left +', '+ right +', '+ top +', '+ bottom + '\n');

    return [
      (sw[0]/dSW + se[0]/dSE + ne[0]/dNE + nw[0]/dNW) / totalWeight,
      (sw[1]/dSW + se[1]/dSE + ne[1]/dNE + nw[1]/dNW) / totalWeight
    ];
  };
  
  photos.forEach(function(p){
    var pt = project([p.lon, p.lat]);
    console.log('<rect x="'+pt[0]+'" y="'+pt[1]+'" width="3" height="3" fill="red" stroke="none"/>');
  });

  console.log('</svg>');

});
