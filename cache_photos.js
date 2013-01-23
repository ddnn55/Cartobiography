var im = require('imagemagick');

function pathToGeoPhoto(path, callback)
{
  console.log("Opening " + path);
  im.readMetadata(path, function(err, metadata){
    if (err) throw err;
    
    // gpsLatitude: '39/1, 4419/100, 0/1',
    // gpsLatitudeRef: 'N',
    // gpsLongitude: '75/1, 3267/100, 0/1',
    // gpsLongitudeRef: 'W',
    
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
  
    callback({
      'path' : path,
      'location' : location,
      'time' : metadata.exif.dateTime
    });
  });
}

var arg = process.argv[2];

pathToGeoPhoto(arg, function(photo){
  console.log(photo);
});
