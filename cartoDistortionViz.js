var step = 25;

var fs = require('fs');

var path = process.argv[2];

fs.readFile(path, 'utf-8', function(err, str) {
  var entries = str.split('\n');
  entries = entries.slice(0, entries.length-1);
  var points = entries.map(function(entry){
    return entry.split(' ').map(parseFloat);
  });
  
  console.log('<svg xmlns="http://www.w3.org/2000/svg" version="1.1">');
  for(i in points) {
    var y = Math.floor(i / 1025);
    var x = i - y * 1025;
    if( ( x % step == 0 ) || ( y % step == 0 ) ) {
      var p = points[i];
      console.log('<rect x="'+p[0]+'" y="'+(1024-p[1])+'" width="1" height="1" fill="black" stroke="none"/>');
    }
  }
  console.log('</svg>');

});
