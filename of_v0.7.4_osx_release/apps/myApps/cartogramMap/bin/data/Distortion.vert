#version 120

//attribute vec4 gl_Vertex;
//uniform mat4 gl_ModelViewProjectionMatrix;
//attribute vec4 gl_MultiTexCoord0; 

varying vec4 gl_TexCoord[ ];

void main()
{
    gl_TexCoord[0] = gl_MultiTexCoord0;
    gl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;
}