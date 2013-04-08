uniform sampler2D colormap;
uniform sampler2D distortion;

varying vec2 MapTexCoord;

uniform vec2 mapSize;
uniform vec2 meshSize;
uniform vec2 distortionSize;

uniform float distortionAmount;

uniform int derivative;

void main(void) {
    
    vec2 normalPos = vec2(gl_Vertex.x / meshSize.x, gl_Vertex.y / meshSize.y);
    
    MapTexCoord = vec2(mapSize.x * normalPos.x, mapSize.y * normalPos.y);
    //DistortionTexCoord = gl_MultiTexCoord1.st;

    //vec4 bumpColor = texture2D(bumpmap, TexCoord);
    //float df = 0.30*bumpColor.x + 0.59*bumpColor.y + 0.11*bumpColor.z;
    //vec4 newVertexPos = vec4(gl_Normal * df * float(maxHeight), 0.0) + gl_Vertex;

    vec2 DistortionTexCoord = vec2(distortionSize.x * normalPos.x, distortionSize.y * normalPos.y);
    
    vec4 distortedVertex = vec4(texture2D(distortion, DistortionTexCoord).xy, 0.0, 1.0);
    distortedVertex.y = 1.0 - distortedVertex.y;
    distortedVertex.x *= meshSize.x;
    distortedVertex.y *= meshSize.y;
    
    vec4 distortedPosition = gl_ModelViewProjectionMatrix * distortedVertex;
    vec4 undisortedPosition = gl_ModelViewProjectionMatrix * gl_Vertex;
    
    gl_Position = mix(distortedPosition, undisortedPosition, distortionAmount);
}