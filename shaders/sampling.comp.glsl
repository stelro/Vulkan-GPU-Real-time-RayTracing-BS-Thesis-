/* =======================================================================
   $File: sampling.glsl
   $Date: 17/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */


float gpu_rnd(inout vec4 state) {

	const vec4 q = vec4(1225, 1585, 2457, 2098);
	const vec4 r = vec4(1112, 367, 92, 265);
	const vec4 a = vec4(3423, 2646, 1707, 1999);
	const vec4 m = vec4(4194287, 4194277, 4194191, 4194167);

	vec4 beta = floor(state / q);
	vec4 p = a * mod(state, q) - beta * r;
	beta = (sign(-p) + vec4(1.0)) * vec4(0.5) * m;
	state = p + beta;

	return fract(dot(state / m, vec4(1.0, -1.0, 1.0, -1.0)));
}

vec3 cos_weighted_hemisphere(in vec3 normal, vec2 random) {

	//http://www.amietia.com/lambertnotangent.html
	float phi = gc_PI * 2 * random.x;
	float cosTheta = 2.0*random.y - 1.0;
	return normalize( normal + vec3(sqrt(1.0-cosTheta*cosTheta) * vec2(cos(phi), sin(phi)), cosTheta) );

}
