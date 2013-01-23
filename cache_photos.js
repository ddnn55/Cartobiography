var im = require('imagemagick'),
    fs = require('fs'),
    path = require('path'),
    async = require('async');

var geoPhotos = [];
function loadGeoPhotoWorker(path, done)
{
  console.log("Opening " + path);
  im.readMetadata(path, function(err, metadata){
    if (err) throw err;
    
    // gpsLatitude: '39/1, 4419/100, 0/1',
    // gpsLatitudeRef: 'N',
    // gpsLongitude: '75/1, 3267/100, 0/1',
    // gpsLongitudeRef: 'W',
    
    if(metadata.exif.gpsLatitude === undefined) {
      done();
      return;
    }

    var location = [metadata.exif.gpsLatitude, metadata.exif.gpsLongitude].map(function(str){
      var parts = str.split(',');
      var degrees = parseInt(parts[0].split('/')[0]);
      var minutesFraction = parts[1].split('/').map(function(n){return parseInt(n); });
      var minutes = minutesFraction[0] / minutesFraction[1];
      degrees += minutes / 60;
      return degrees;
    });
    if(metadata.exif.gpsLatitudeRef == 'S')
      location[0] *= -1;
    if(metadata.exif.gpsLongitudeRef == 'W')
      location[1] *= -1;
  
    geoPhotos.push({
      'path' : path,
      'location' : location,
      'time' : metadata.exif.dateTime
    });

    done();
  });
}

var dir = process.argv[2];

fs.readdir(dir, function(err, files) {
  jpgs = files.filter(function(file) { return /jpg/i.test(file); });
  jpgs = jpgs.map(function(file) { return path.join(dir, file); });
  console.log('jpgs: ', jpgs);
  
  var q = async.queue(loadGeoPhotoWorker, 32);
  q.drain = function() {
    console.log('All photos have been processed:');
    console.log(geoPhotos);
  };

  jpgs.forEach(function(path) {
    q.push(path, function(err) {
      console.log('Finished processing ' + path);
    });
  });

});
