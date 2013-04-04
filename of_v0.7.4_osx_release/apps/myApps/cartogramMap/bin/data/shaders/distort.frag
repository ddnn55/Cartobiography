uniform sampler2D colormap;
uniform sampler2D distortion;

uniform int derivative;

varying vec2 MapTexCoord;
varying vec2 DistortionTexCoord;

uniform vec2 mapSize;

void main(void) {
    
    if(derivative == 0)
        gl_FragColor = texture2D(colormap, MapTexCoord);
    else
    {
        float scale = 1.0 + 100.0 * (abs(dFdx(MapTexCoord.x)) + abs(dFdy(MapTexCoord.y)));
        //gl_FragColor = scale * texture2D(colormap, MapTexCoord);
        gl_FragColor = vec4( 1000.0 * dFdx(MapTexCoord.x) * vec3(1.0, 1.0, 1.0), 1.0 );
        
        //float scale = 0.0001;
        //gl_FragColor = vec4(scale / dFdx(MapTexCoord.x), scale / dFdy(MapTexCoord.y), 0.1, 1.0);
    }
    

}