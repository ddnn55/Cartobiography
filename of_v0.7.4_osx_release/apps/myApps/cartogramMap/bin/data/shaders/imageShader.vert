uniform sampler2D image;
uniform float imageRadius;

varying vec2 pos;

void main(void) {
    
    pos = gl_Vertex.xy;
    
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    
}