#version 120

#extension GL_ARB_texture_rectangle : enable

uniform sampler2DRect cameraInput0;
uniform sampler2DRect cameraInput1;
uniform sampler2DRect cameraInput2;
uniform sampler2DRect cameraInput3;
uniform sampler2DRect cameraInput4;
uniform sampler2DRect cameraInput5;

void main()
{
    vec2 position = vec2(gl_TexCoord[0].x, gl_TexCoord[0].y);
    vec4 source = texture2DRect(cameraInput0, position);

    sampler2DRect targetSource;




    gl_FragColor = source;
}
