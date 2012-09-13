#version 120

void main(){

	gl_TexCoord[0] = gl_MultiTexCoord0;

	//get our current vertex position so we can modify it
	gl_Position = ftransform();//gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;


}
