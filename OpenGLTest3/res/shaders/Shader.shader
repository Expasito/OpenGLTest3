#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 colorData;

out vec3 outColor;

void main(){
	gl_Position = vec4(position,1);
	outColor = colorData;
};


#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 outColor;
uniform vec4 color;

void main(){

	FragColor = vec4(outColor,1);

};
