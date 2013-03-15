uniform sampler2D colormap;
uniform sampler2D distortion;

varying vec2 MapTexCoord;
varying vec2 DistortionTexCoord;

uniform vec2 mapSize;

void main(void) {

    //vec2 mapLocation = texture2D(distortion, DistortionTexCoord).st;
    
    //mapLocation.y = 1.0 - mapLocation.y;

    //mapLocation.x *= mapSize.x;
    //mapLocation.y *= mapSize.y;
    
    gl_FragColor = texture2D(colormap, MapTexCoord);
    
    //gl_FragColor = vec4(DistortionTexCoord, 0.0, 1.0);
}