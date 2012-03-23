#version 120
varying vec3 N;
varying vec3 V;

#define ALPHA 0.5
#define BETA 0.5
#define BLUE 0.6
#define YELLOW 0.3

void main () {
		//The normal vector
		vec3 normalN = normalize(N);
		//The reflected vector of V and N
		vec3 R = reflect(normalize(V), normalN);
		//The light vector
		vec3 L = normalize(vec3(gl_LightSource[0].position));

		vec4 kCool = vec4(0.0,0.0,BLUE,1.0) + ALPHA*gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse ;
		vec4 kWarm = vec4(YELLOW,YELLOW,0.0,1.0) + BETA*gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;

		//The diffuse light calculation
		vec4 diffuse = kCool *(1 - dot(N,L))/2 + kWarm * (1 + dot(N,L))/2;

		//The specular light calculation
		vec4 specular= pow(max(dot(R,L), 0.0), 0.7*gl_FrontMaterial.shininess) * gl_LightSource[0].specular * gl_FrontMaterial.specular;

		//Determening the color using al the calculations
	gl_FragColor = vec4(diffuse + specular);
}
