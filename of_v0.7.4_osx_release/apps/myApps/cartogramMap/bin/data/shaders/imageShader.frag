uniform float imageRadius;

uniform sampler2D image;
uniform vec2 normalImageSize;

varying vec2 pos;

void main(void) {

    //gl_FragColor = texture2D(image, vec2( (pos.x + imageRadius) / (2.0 * imageRadius), (pos.y + imageRadius) / (2.0 * imageRadius) ));
    gl_FragColor = texture2D(image, vec2(
                                         0.5 + (0.5 * pos.x/imageRadius)/normalImageSize.x,
                                         0.5 + (0.5 * pos.y/imageRadius)/normalImageSize.y
                                        ));

    //gl_FragColor = vec4( 255, 255, 255, 255 );
    
}