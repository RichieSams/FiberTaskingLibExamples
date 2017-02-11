#version 410

void main() {
	gl_Position = vec4(gl_VertexID & 1, gl_VertexID >> 1, 0.0, 0.5) * 4.0 - 1.0;
}