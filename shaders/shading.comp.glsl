/* =======================================================================
   $File: shading.glsl
   $Date: 17/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */
#include "sampling.comp.glsl" 
vec3 Reflect(in vec3 v, in vec3 n) {
  // Basic reflection method used for smooth metals
  // ( Perfect Mirror )
  return ( v - 2 * dot(n,v) * n );
}

bool Refract(in vec3 v, in vec3 n, float ni_over_nt, inout vec3 refracted) {

    // Refraction formula, described by Snell's law
    vec3 uv = normalize(v);
    float dt = dot(uv, n);
    float discriminant = 1.0f - ni_over_nt * ni_over_nt * (1 - dt * dt);

    if ( discriminant > 0 ) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return ( true );
    } else {
        return ( false );
    }
}

float schlick(float cosine, float ref_index) {

   float r0 = (1 - ref_index) / (1 + ref_index);
   r0 = r0 * r0;
   return ( r0 + ( 1 - r0 ) * pow(( 1 - cosine ), 5) );
}


bool ScatterLambertian(in Ray r_in, in InteractionRecord rec, inout vec3 attenuation, inout Ray scattered) {

  if ( rec.material != gc_MaterialLambertian ) return ( false );

  vec3 target = rec.position + rec.normal + cos_weighted_hemisphere(rec.normal,rec.random);
  scattered = Ray(rec.position, target - rec.position);
  attenuation = rec.albedo;

  return ( true );

}

bool ScatterMetal(in Ray r_in, in InteractionRecord rec, inout vec3 attenuation, inout Ray scattered) {

  if ( rec.material != gc_MaterialMetal ) return ( false );

  // How much shine or matte will be the material, 0.0f is usually mirror
  float fuzz = (rec.material != gc_MaterialEmitter) ? rec.fuzz : 0.0f;

  vec3 reflected = Reflect(normalize(r_in.direction), rec.normal);
  scattered = Ray(rec.position, reflected + fuzz * cos_weighted_hemisphere(rec.normal,rec.random));
  attenuation = rec.albedo;

  return ( dot(scattered.direction, rec.normal) > 0 );

}


bool ScatterDialectric(in Ray r_in, in InteractionRecord rec, inout vec3 attenuation, inout Ray scattered, vec4 state) {

  if ( rec.material != gc_MaterialDialectric ) return ( false );

  vec3 outward_normal;
  vec3 reflected = Reflect(r_in.direction, rec.normal);
  float ni_overt_nt;
  attenuation = vec3(1.0f);
  vec3 refracted;
  float reflect_prob;
  float cosine;

  if ( dot(r_in.direction, rec.normal) > 0 ) {
    outward_normal = -rec.normal;
    ni_overt_nt = rec.ref_index;
    cosine = rec.ref_index * dot(r_in.direction, rec.normal) / length(r_in.direction);
  } else {
    outward_normal = rec.normal;
    ni_overt_nt = 1.0f / rec.ref_index;
    cosine = -dot(r_in.direction, rec.normal) / length(r_in.direction);
  }

  if ( Refract(r_in.direction, outward_normal, ni_overt_nt, refracted) ) {
    // Propability of reflection
    reflect_prob = schlick(cosine, rec.ref_index);
  } else {
    scattered = Ray(rec.position, reflected);
    reflect_prob = 1.0f;
  }

  scattered = Ray(rec.position, refracted);

  if ( gpu_rnd(state) < reflect_prob ) {
    scattered = Ray(rec.position, reflected);
  } else {
    scattered = Ray(rec.position, refracted);
  }

  return ( true );

}


