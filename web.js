// Server setup
var express = require("express");
var app = express();

// Template and Static Files
var fs = require('fs');
app.use('/', express.static('www'));

/*var indexHtml = fs.readFileSync('index.html', 'utf8');
app.get('/', function(request, response) {
  response.send(indexHtml);
});*/

// Start it up
var port = process.env.PORT || 5000;
app.listen(port, function() {
  console.log("Listening on " + port);
});
