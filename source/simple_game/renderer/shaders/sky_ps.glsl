#version 410

out vec3 pixelColor;

uniform vec3  light;
uniform vec2  resolution;
uniform mat4  worldMatrix;
uniform mat4  invProjectionMatrix;

float hash(vec2 p) { return fract(1e4 * sin(17.0 * p.x + p.y * 0.1) * (0.1 + abs(sin(p.y * 13.0 + p.x)))); }

float noise(vec2 x) {
	vec2 i = floor(x);
	float a = hash(i);
	float b = hash(i + vec2(1.0, 0.0));
	float c = hash(i + vec2(0.0, 1.0));
	float d = hash(i + vec2(1.0, 1.0));

	vec2 f = fract(x);
	vec2 u = f * f * (3.0 - 2.0 * f);
	return mix(a, b, u.x) + (c - a) * u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

float fbm(vec2 p) {
	const mat2 m2 = mat2(0.8, -0.6, 0.6, 0.8);
	float f = 0.5000 * noise(p); p = m2 * p * 2.02;
	f += 0.2500 * noise(p); p = m2 * p * 2.03;
	f += 0.1250 * noise(p); p = m2 * p * 2.01;
	f += 0.0625 * noise(p);
	return f / 0.9375;
}

vec3 render(in vec3 sun, in vec3 ro, in vec3 rd, in float resolution) {
	vec3 col;
	if (rd.y < 0.0) {
		// Ground
		float t = -ro.y / rd.y;
		vec2 P = ro.xz + t * rd.xz + vec2(0.5);
		vec2 Q = floor(P);
		// 1m^2 grid
		P = mod(P, 1.0);

		const float gridLineWidth = 0.1;
		float res = clamp(3000.0 / resolution, 1.0, 4.0);
		P = 1.0 - abs(P - 0.5) * 2.0;
		float d = clamp(min(P.x, P.y) / (gridLineWidth * clamp(t + res * 2.0, 1.0, 3.0)) + 0.5, 0.0, 1.0);
		float shade = mix(hash(100.0 + Q * 0.1) * 0.4, 0.3, min(t * t * 0.00001 / max(-rd.y, 0.001), 1.0)) + 0.6;
		col = vec3(pow(d, clamp(150.0 / (pow(max(t - 2.0, 0.1), res) + 1.0), 0.1, 15.0))) * shade + 0.1;
	} else {
		// Sky
		col = vec3(0.3, 0.55, 0.8) * (1.0 - 0.8 * rd.y) * 0.9;
		float sundot = clamp(dot(rd, sun), 0.0, 1.0);
		col += 0.25 * vec3(1.0, 0.7, 0.4) * pow(sundot, 8.0);
		col += 0.75 * vec3(1.0, 0.8, 0.5) * pow(sundot, 64.0);
		col = mix(col, vec3(1.0, 0.95, 1.0), 0.5 * smoothstep(0.5, 0.8, fbm((ro.xz + rd.xz * (250000.0 - ro.y) / rd.y) * 0.000008)));
	}
	return mix(col, vec3(0.7, 0.75, 0.8), pow(1.0 - max(abs(rd.y), 0.0), 8.0));
}

void main() {
	vec3 rd = normalize(mat3(worldMatrix) * vec3((invProjectionMatrix * vec4(gl_FragCoord.xy / resolution.xy * 2.0 - 1.0, -1.0, 1.0)).xy, -1.0));
	pixelColor = render(light, worldMatrix[3].xyz, rd, resolution.x);
}
