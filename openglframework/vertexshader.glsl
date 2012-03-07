#version 120
varying vec3 N;
varying vec3 V;

void main(){
	//The view vector
	V = (gl_ModelViewMatrix * gl_Vertex).xyz;
	//The normal vector
	N = gl_NormalMatrix * gl_Normal;

	gl_Position = ftransform();
}
