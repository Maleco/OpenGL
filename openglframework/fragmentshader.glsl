#version 120
varying vec3 N;
varying vec3 V;

void main () {
	//The normal vector
	vec3 normalN = normalize(N);
	//The reflected vector of V and N
	vec3 R = reflect(normalize(V), normalN);
	//The light vector
	vec3 L = normalize(vec3(gl_LightSource[0].position));

	//The ambiant light calculation
	vec4 ambient = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
	//The diffuse light calculation
	vec4 diffuse = max(dot(L, N), 0.0) * gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
	//The specular light calculation
	vec4 specular= pow(max(dot(R,L), 0.0), 0.7*gl_FrontMaterial.shininess) * gl_LightSource[0].specular * gl_FrontMaterial.specular;

	//Determening the color using al the calculations
	gl_FragColor = vec4(ambient + diffuse + specular);
}
