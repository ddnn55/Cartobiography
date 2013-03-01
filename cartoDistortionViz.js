var fs = require('fs');

var path = process.argv[2];

fs.readFile(path, 'utf-8', function(err, str) {
  var entries = str.split('\n');
  var points = entries.map(function(entry){
    
  });
});
