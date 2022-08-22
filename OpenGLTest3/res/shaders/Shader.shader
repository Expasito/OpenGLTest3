#shader vertex
#version 330 core

layout(location =0) in vec2 position;
layout(location =1) in vec3 color3;
layout(location =2) in float light;

out vec3 color1;
out float light1;

void main(){
	gl_Position = vec4(position.x,position.y,1,1);
	color1=color3;
	light1 = light;
};


#shader fragment
#version 330 core

out vec4 color;
in vec3 color1;
in float light1;

void main(){

	color = vec4(color1.x*light1,color1.y*light1,color1.z*light1,1);

};
