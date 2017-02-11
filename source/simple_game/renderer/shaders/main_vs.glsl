#version 410 // -*- c++ -*-

// Attributes
in vec3           position;
in vec3           normal;
in vec2           texCoord;
in vec4           tangent;

// Interpolated outputs
out Varying {
    vec3          normal;
    vec2          texCoord;
    vec3          position;
    vec3          tangent;
    flat float    tangentYSign;
} vertexOutput;

// Uniforms
layout(shared) uniform Uniform {
    mat4x4        world;
    mat4x4        worldInvTranspose;
	mat4x4        projViewWorld;
	vec3          light;
	vec3          cameraPosition;
} object;

void main () {
	vertexOutput.normal     = normalize(mat3x3(object.worldInvTranspose) * normal);
    vertexOutput.texCoord   = texCoord;
    vertexOutput.position   = (object.world * vec4(position, 1.0)).xyz;
    vertexOutput.tangent    = normalize(mat3x3(object.world) * tangent.xyz);
    vertexOutput.tangentYSign = tangent.w;

    gl_Position = object.projViewWorld * vec4(position, 1.0);
}
