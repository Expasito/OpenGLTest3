#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 2) in vec2 textCords;
layout(location = 3) in vec3 translation;
layout(location = 4) in vec3 rotation;
layout(location = 5) in vec3 scalation;
layout(location = 6) in vec3 color;
layout(location = 7) in float TextureLocation;

mat4 scale(float x, float y, float z) {
    return mat4(
        vec4(x, 0.0, 0.0, 0.0),
        vec4(0.0, y, 0.0, 0.0),
        vec4(0.0, 0.0, z, 0.0),
        vec4(0.0, 0.0, 0.0, 1.0)
    );
}

mat4 translate(float x, float y, float z) {
    return mat4(
        vec4(1.0, 0.0, 0.0, 0.0),
        vec4(0.0, 1.0, 0.0, 0.0),
        vec4(0.0, 0.0, 1.0, 0.0),
        vec4(x, y, z, 1.0)
    );
}

mat4 RotateX(float phi) {
    return mat4(
        vec4(1., 0., 0., 0),
        vec4(0., cos(phi), -sin(phi), 0.),
        vec4(0., sin(phi), cos(phi), 0.),
        vec4(0., 0., 0., 1.));
}

mat4 RotateY(float theta) {
    return mat4(
        vec4(cos(theta), 0., -sin(theta), 0),
        vec4(0., 1., 0., 0.),
        vec4(sin(theta), 0., cos(theta), 0.),
        vec4(0., 0., 0., 1.));
}

mat4 RotateZ(float psi) {
    return mat4(
        vec4(cos(psi), -sin(psi), 0., 0),
        vec4(sin(psi), cos(psi), 0., 0.),
        vec4(0., 0., 1., 0.),
        vec4(0., 0., 0., 1.));
}

out vec2 TexCord;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

//uniform vec3 color;
out float textLoc;
out vec3 col;

void main(){
    vec3 translateData = translation;
    vec3 rotateData = rotation;
    vec3 scaleData = scalation;

    mat4 trans=translate(translateData.x,translateData.y,translateData.z);
    mat4 rotX = RotateX(rotateData.x);
    mat4 rotY = RotateY(rotateData.y);
    mat4 rotZ = RotateZ(rotateData.z);
    mat4 scal = scale(scaleData.x, scaleData.y, scaleData.z);
    vec4 localPos = trans * rotX * rotY * rotZ * scal * vec4(position.xyz, 1);
    //vec4 localPos = vec4(position.xyz, 1);
    gl_Position = projection * view * model * localPos;
    col = color;
    textLoc = TextureLocation;
	TexCord = vec2(textCords.x,textCords.y*1);
};


#shader fragment
#version 330 core 

out vec4 FragColor;
in vec2 TexCord;
in vec3 col;
in float textLoc;
uniform sampler2D u_Textures[32];


void main(){
    int lo = int(textLoc+.02);
    if (lo == 0) {
        FragColor = vec4(col, 1);
    }
    else {
        FragColor = texture(u_Textures[lo], TexCord)*vec4(col,1);
    }
};
