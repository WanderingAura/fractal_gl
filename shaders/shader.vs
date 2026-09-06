#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in float aGreen;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out float green;

void main() {
    gl_Position = model * vec4(aPos, 1.0);
    green = aGreen;
}
