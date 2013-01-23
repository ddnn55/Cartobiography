/*var ExifImage = require('exif').ExifImage;

try {
    var path = process.argv[2];
    console.log("Opening " + path);
    new ExifImage({ image : path }, function (error, image) {
        if (error)
            console.log('Error: '+error.message);
        else
            console.log(image.gps); // Do something with your data!
    });
} catch (error) {
    console.log('Error: ' + error);
}*/


var im = require('imagemagick');
var path = process.argv[2];
console.log("Opening " + path);
im.readMetadata(path, function(err, metadata){
  if (err) throw err;
  
  // gpsLatitude: '39/1, 4419/100, 0/1',
  // gpsLatitudeRef: 'N',
  // gpsLongitude: '75/1, 3267/100, 0/1',
  // gpsLongitudeRef: 'W',
  
  var location = [metadata.exif.gpsLongitude, metadata.exif.gpsLatitude].map(function(str){
    var parts = str.split(',');
    var degrees = parseInt(parts[0].split('/')[0]);
    var minutesFraction = parts[1].split('/').map(function(n){return parseInt(n); });
    var minutes = minutesFraction[0] / minutesFraction[1];
    degrees += minutes / 60;
    return degrees;
  });
  if(metadata.exif.gpsLatitudeRef == 'S')
    location[1] *= -1;
  if(metadata.exif.gpsLongitudeRef == 'W')
    location[0] *= -1;

  console.log(location);
})
