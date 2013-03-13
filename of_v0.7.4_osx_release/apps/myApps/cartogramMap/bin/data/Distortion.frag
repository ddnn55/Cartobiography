#version 120

//vec4 gl_FragColor;
//varying vec4 gl_TexCoord[ ]

void main()
{
    gl_FragColor = gl_TexCoord[0];
}