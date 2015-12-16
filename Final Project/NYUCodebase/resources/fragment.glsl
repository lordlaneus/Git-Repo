
uniform sampler2D diffuse;

varying vec2 texCoordVar;
varying vec2 varPosition;

float attenuate(float dist) {
	return (70-dist)/70;
	}

void main() {

	vec3 brightness = vec3(1, 1, 1);
	brightness *= attenuate(distance(vec2(80,50), varPosition));
	
	vec4 color = texture2D(diffuse, texCoordVar);
	gl_FragColor.xyz =color.xyz * brightness;
    gl_FragColor.a = color.a;
}