#shader vertex
#version 330 core

layout(location = 0) in vec2 position;


out vec3 color1;
out float light1;

void main() {
	gl_Position = vec4(position.x,position.y, 1, 1);
};


#shader fragment
#version 330 core

out vec4 color;

void main() {

	color = vec4(255,255,255,1);

};
