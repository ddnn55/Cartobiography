var fs = require('fs');

// Server setup
var express = require("express");
var app = express(express.logger());

app.get('/f', function(request, response) {
  response.send(request.query.path);
});

// Static Files
app.use('/', express.static('www'));

// Start it up
var port = process.env.PORT || 5000;
app.listen(port, function() {
  console.log("Listening on " + port);
});
