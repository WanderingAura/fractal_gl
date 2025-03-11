#version 330 core

layout(location = 0) in vec3 aPos;

uniform vec3 triangleVert1;
uniform vec3 triangleVert2;
uniform vec3 triangleVert3;

// TODO: understand how pseudo-rngs work.
// ref: https://stackoverflow.com/questions/35376941/a-pseudo-random-number-generator-based-on-2-inputs
float rand(uint x, uint y) {
    /* mix around the bits in x: */
    x = x * 3266489917 + 374761393;
    x = (x << 17) | (x >> 15);

    /* mix around the bits in y and mix those into x: */
    x += y * 3266489917;

    /* Give x a good stir: */
    x *= 668265263;
    x ^= x >> 15;
    x *= 2246822519;
    x ^= x >> 13;
    x *= 3266489917;
    x ^= x >> 16;

    /* trim the result and scale it to a float in [0,1): */
    return (x & 0x00ffffff) * (1.0f / 0x1000000);
}

vec3 randomTrianglePoint() {
    float r1 = rand(gl_VertexID, gl_InstanceID);
    float r2 = rand(gl_VertexID + 3, gl_InstanceID + 7);

    float u = 1.0 - sqrt(r1);
    float v = sqrt(r1) * (1.0 - r2);
    float w = sqrt(r1) * r2;

    return u * triangleVert1 + v * triangleVert2 + w * triangleVert3;
}

void main() {
    vec3 point = randomTrianglePoint();
    gl_Position = 0.5 * (point + aPos); // midpoint between random point and vertex
}
