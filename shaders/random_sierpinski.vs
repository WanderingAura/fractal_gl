#version 330 core

layout(location = 0) in vec3 aPos;

uniform vec3 triangleVert1;
uniform vec3 triangleVert2;
uniform vec3 triangleVert3;

// TODO: understand how pseudo-rngs work.
// ref: https://stackoverflow.com/questions/35376941/a-pseudo-random-number-generator-based-on-2-inputs
float rand(uint x, uint y) {
    /* mix around the bits in x: */
    x = x * 3266489917u + 374761393u;
    x = (x << 17) | (x >> 15);

    /* mix around the bits in y and mix those into x: */
    x += y * 3266489917u;

    /* Give x a good stir: */
    x *= 668265263u;
    x ^= x >> 15;
    x *= 2246822519u;
    x ^= x >> 13;
    x *= 3266489917u;
    x ^= x >> 16;

    /* trim the result and scale it to a float in [0,1): */
    return float(x & uint(0x00ffffff)) * (1.0f / float(0x1000000));
}

// FIX: this method of generating random points seems buggy
// either try fix or use parallelogram+reflection method.
vec3 randomTrianglePoint() {
    uint uVertID = uint(gl_VertexID);
    uint uInstID = uint(gl_InstanceID);
    float r1 = rand(uVertID, uInstID);
    float r2 = rand(uVertID + 3u, uInstID + 7u);

    float u = 1.0 - sqrt(r1);
    float v = sqrt(r1) * (1.0 - r2);
    float w = sqrt(r1) * r2;

    return u * triangleVert1 + v * triangleVert2 + w * triangleVert3;
}

void main() {
    vec3 point = randomTrianglePoint();
    gl_Position = vec4(0.5 * (point + aPos), 1.0f); // midpoint between random point and vertex
}
