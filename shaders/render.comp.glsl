/* =======================================================================
   $File: shader.comp.glsl
   $Date: 17/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_GOOGLE_include_directive : enable

#include "constants.comp.glsl"
#include "data_structures.comp.glsl"
#include "shading.comp.glsl"

layout( local_size_x = WORKGROUP_SIZE, local_size_y = WORKGROUP_SIZE,
        local_size_z = 1 ) in;

// Where the final computed ( ray-traced image ) will be stored
layout( binding = 0, rgba8 ) uniform image2D resultImage;

// Buffer of spheres passed from client application
layout( std140, binding = 1 ) buffer Spheres {
  Sphere spheres[];
};

// Buffer of planes passed from client appliction
layout( std140, binding = 2 ) buffer Planes {
  Plane planes[];
};

// Buffer of rectangles passed from client applictaion
layout( std140, binding = 4 ) buffer Rectangles {
  Rectangle rects[];
};

// Buffer of boxes passed from client applictaion
layout( std140, binding = 5 ) buffer Boxes {
  Box boxes[];
};


// Uniform Buffer Object, passed from client application
// This is used to pass arbitary data to the shader
layout( binding = 3 ) uniform UBO {

  // Has the user pressed any keyboard/mouse key?
  int anyKeyIsPressed;
  // Flags to enable/disable render of various shapes
  // This flags are defined in scene_*.json files
  // we use int instead of boolean values, bacause
  // bool was causing some issues with MVSC compilers
  int spheres_are_enabled;
  int planes_are_enabled;
  int rects_are_enabled;
  int boxes_are_enabled;

  float randomSeed;
  // pseudo 'anti-aliasing' strength
  float aaStrength;
  // Number of samples per pixels
  uint samplesPerPixel;
  // Ray bounches depth, there is no reason more than 16
  uint rayTraceDepth;
  // The (pseudo)ambient color of our scene
  float currentSmaples;
  vec3 ambient;
  // Camera init origin
  vec3 origin;
  // Camera init lookat
  vec3 lookat;
  // Camera init field of view
  float fov;
}
ubo;


/*
Using globals and avoindg passing values to functions
is performance-wise practice with shaders. Passing arguments to
functions generally is more expensive than it looks in shaders.
*/

Camera g_camera;

// Basic float swap function, nothing special
void fswap( inout float lhs, inout float rhs ) {
  float temp = lhs;
  lhs = rhs;
  rhs = temp;
}

// Sphere intersection test rutine
bool SphereIntersect( in Sphere sphere, in Ray ray, float tMin, float tMax,
                      out InteractionRecord rec ) {

  // This is basically a quadratic equation
  vec3 oc = ray.origin - sphere.center;
  float a = dot( ray.direction, ray.direction );
  float b = dot( oc, ray.direction );
  float c = dot( oc, oc ) - sphere.radius * sphere.radius;

  float discriminant = b * b - a * c;

  // Don't take square root of negative number
  if ( discriminant < 0.0f )
    return ( false );

  discriminant = sqrt( discriminant );

  float t0 = ( -b - discriminant ) / a;
  float t1 = ( -b + discriminant ) / a;

  if ( t0 < tMax && t0 > tMin ) {
    rec.t = t0;
    return ( true );
  } else if ( t1 < tMax && t1 > tMin ) {
    rec.t = t1;
    return ( true );
  } else {
    return ( false );
  }
}

// Plane intersection test rutine
bool PlaneIntersect( in Plane plane, in Ray ray, float tMin, float tMax,
                     out InteractionRecord rec ) {

  float t = -( dot( ray.origin, normalize( plane.normal ) ) + plane.distance ) /
            dot( ray.direction, normalize( plane.normal ) );

  if ( t <= tMin )
    return ( false );
  if ( t >= tMax )
    return ( false );

  rec.t = t;

  return ( true );
}

// Axis - Aligned rectangle in XY Plane which is defined by its Z value ( k
// here, z = k) An axis aligned rectangle is defined by lines x = x0, x = x1 , y
// = y0, y = y1
bool XYRect_Intersect( in Rectangle rect, in Ray ray, float tMin, float tMax,
                       out InteractionRecord rec ) {

  float t = ( rect.k - ray.origin.z ) / ray.direction.z;

  if ( t < tMin || t > tMax )
    return ( false );

  float x = ray.origin.x + t * ray.direction.x;
  float y = ray.origin.y + t * ray.direction.y;

  if ( x < rect.aabb.x || x > rect.aabb.y || y < rect.aabb.z ||
       y > rect.aabb.w )
    return ( false );

  rec.t = t;
  rec.normal = ( rect.reverse_normals ) ? vec3( 0.0f, 0.0f, -1.0f )
                                        : vec3( 0.0f, 0.0f, 1.0f );

  return ( true );
}

// ditto
bool XZRect_Intersect( in Rectangle rect, in Ray ray, float tMin, float tMax,
                       out InteractionRecord rec ) {

  float t = ( rect.k - ray.origin.y ) / ray.direction.y;

  if ( t < tMin || t > tMax )
    return ( false );

  float x = ray.origin.x + t * ray.direction.x;
  float z = ray.origin.z + t * ray.direction.z;

  if ( x < rect.aabb.x || x > rect.aabb.y || z < rect.aabb.z ||
       z > rect.aabb.w )
    return ( false );

  rec.t = t;
  rec.normal = ( rect.reverse_normals ) ? vec3( 0.0f, -1.0f, 0.0f )
                                        : vec3( 0.0f, 1.0f, 0.0f );

  return ( true );
}

// ditto
bool YZRect_Intersect( in Rectangle rect, in Ray ray, float tMin, float tMax,
                       out InteractionRecord rec ) {

  float t = ( rect.k - ray.origin.x ) / ray.direction.x;

  if ( t < tMin || t > tMax )
    return ( false );

  float y = ray.origin.y + t * ray.direction.y;
  float z = ray.origin.z + t * ray.direction.z;

  if ( y < rect.aabb.x || y > rect.aabb.y || z < rect.aabb.z ||
       z > rect.aabb.w )
    return ( false );

  rec.t = t;
  rec.normal = ( rect.reverse_normals ) ? vec3( -1.0f, 0.0f, 0.0f )
                                        : vec3( 1.0f, 0.0f, 0.0f );

  return ( true );
}


bool RectangleIntersect( in Rectangle rect, in Ray ray, float tMin, float tMax,
                         out InteractionRecord rec ) {

  switch ( rect.side ) {
    case 0:
      return XZRect_Intersect( rect, ray, tMin, tMax, rec );
    case 1:
      return XYRect_Intersect( rect, ray, tMin, tMax, rec );
    case 2:
      return YZRect_Intersect( rect, ray, tMin, tMax, rec );
    default:
      return ( false );
  }

  return ( true );
}

bool BoxIntersect( in Box box, in Ray ray, float tMin, float tMax,
                   out InteractionRecord rec ) {

  // This is actually a simple AABB box, made of 3 slabs for each axis

  // Get min intersection and max intersection of the
  // slab in the X axis
  float tmin = ( box.vmin.x - ray.origin.x ) / ray.direction.x;
  float tmax = ( box.vmax.x - ray.origin.x ) / ray.direction.x;

  // Get the neareset point of the intersection with the
  // slab on the X axis
  if ( tmin > tmax )
    fswap( tmin, tmax );

  float tymin = ( box.vmin.y - ray.origin.y ) / ray.direction.y;
  float tymax = ( box.vmax.y - ray.origin.y ) / ray.direction.y;

  // Get the nearest point of the intersetction with the
  // slab on the Y axis
  if ( tymin > tymax )
    fswap( tymin, tymax );

  // There is no intersectin with the X-Y
  // Slabs
  if ( ( tmin > tymax ) || ( tymin > tmax ) )
    return ( false );

  // Interchange the the points and get the nearest
  // and the longer away, the points
  // that are actually inside the box,  to perform test with the
  // Z slab
  tmin = max( tymin, tmin );
  tmax = min( tymax, tmax );


  float tzmin = ( box.vmin.z - ray.origin.z ) / ray.direction.z;
  float tzmax = ( box.vmax.z - ray.origin.z ) / ray.direction.z;

  if ( tzmin > tzmax )
    fswap( tzmin, tzmax );

  if ( ( tmin > tzmax ) || ( tzmin > tmax ) )
    return ( false );

  tmin = max( tzmin, tmin );
  tmax = min( tzmax, tmax );

  if ( tmin < tMin || tmax > tMax )
    return ( false );

  rec.t = tmin;


  if ( rec.t <= 0.0 ) {
    rec.t = tmax;
    if ( rec.t <= 0.0 )
      return ( false );
  }

  return ( true );
}

// Main intersection rutine
bool intersect( in Ray ray, float tMin, float tMax,
                out InteractionRecord rec ) {


  InteractionRecord temp_rec;

  bool hasHit = false;

  float t = tMax;


  if ( ubo.spheres_are_enabled == 1 ) {
    // Spheres Intersection test
    for ( int i = 0; i < spheres.length(); i++ ) {

      if ( SphereIntersect( spheres[ i ], ray, tMin, t, temp_rec ) ) {

        hasHit = true;
        t = temp_rec.t;
        rec = temp_rec;
        rec.position = GetPointAt( ray, t );
        rec.normal = SphereNormal( spheres[ i ], rec.position );
        rec.albedo = spheres[ i ].diffuse;
        rec.material = spheres[ i ].material;
        rec.ref_index = spheres[ i ].ref_index;
        rec.fuzz = spheres[ i ].fuzz;
      }
    }
  }


  if ( ubo.planes_are_enabled == 1 ) {
    // Planes Intersection test
    for ( int i = 0; i < planes.length(); i++ ) {

      if ( PlaneIntersect( planes[ i ], ray, tMin, t, temp_rec ) ) {
        hasHit = true;
        t = temp_rec.t;
        rec = temp_rec;
        rec.position = GetPointAt( ray, t );
        rec.normal = planes[ i ].normal;
        rec.albedo = planes[ i ].diffuse;
        rec.material = planes[ i ].material;
        rec.ref_index = planes[ i ].ref_index;
        rec.fuzz = planes[ i ].fuzz;
      }
    }
  }


  if ( ubo.rects_are_enabled == 1 ) {
    // Rectangles Intersection test
    for ( int i = 0; i < rects.length(); i++ ) {

      if ( RectangleIntersect( rects[ i ], ray, tMin, t, temp_rec ) ) {
        hasHit = true;
        t = temp_rec.t;
        rec = temp_rec;
        rec.position = GetPointAt( ray, t );
        rec.albedo = rects[ i ].diffuse;
        rec.material = rects[ i ].material;
        rec.ref_index = rects[ i ].ref_index;
        rec.fuzz = rects[ i ].fuzz;
      }
    }
  }

  if ( ubo.boxes_are_enabled == 1 ) {
    // Boxes Intersection test
    for ( int i = 0; i < boxes.length(); i++ ) {

      if ( BoxIntersect( boxes[ i ], ray, tMin, t, temp_rec ) ) {
        hasHit = true;
        t = temp_rec.t;
        rec = temp_rec;
        rec.position = GetPointAt( ray, t );
        rec.normal = getBoxNormal( boxes[ i ], rec.position );
        rec.albedo = boxes[ i ].diffuse;
        rec.material = boxes[ i ].material;
        rec.ref_index = boxes[ i ].ref_index;
        rec.fuzz = boxes[ i ].fuzz;
      }
    }
  }


  return ( hasHit );
}

// Scatter rutine, this function returns the final color of the pixel
vec3 scatter( in Ray ray, in vec4 seed ) {

  Ray cur_ray = ray;
  vec3 cur_attenuation = vec3( 1.0f );
  vec3 attenuation;
  Ray scattered;


  for ( int i = 0; i < ubo.rayTraceDepth; i++ ) {

    InteractionRecord rec;
    rec.t = -1.0f;
    rec.position = vec3( 0.0f );
    rec.albedo = vec3( 1.0f, 0.0f, 0.0f );
    rec.material = 0;

    float rand_a = gpu_rnd( seed );
    float rand_b = gpu_rnd( seed );


    if ( ( intersect( cur_ray, gc_epsilon, gc_interMaxLength, rec ) ) ) {

      rec.random = vec2( rand_a, rand_b );

      if ( ScatterLambertian( cur_ray, rec, attenuation, scattered ) ||
           ScatterMetal( cur_ray, rec, attenuation, scattered ) ||
           ScatterDialectric( cur_ray, rec, attenuation, scattered, seed ) ) {

        cur_attenuation *= attenuation;
        cur_ray = scattered;

      } else {
        // If the object is emitter ( light source ) , then return just
        // the color of the object ( emission value ), otherwise return
        // just black background ( the scene is black if no color occurs )
        return ( rec.material == gc_MaterialEmitter ) ? rec.albedo
                                                      : vec3( 1.0f );
      }
    }

    else {
      return cur_attenuation * ubo.ambient;
    }
  }

  return vec3( 0.0f );
}


void main() {

  ivec2 dim = imageSize( resultImage );


  /*
  In order to fit the work into workgroups, some unnecessary threads are
  launched. We terminate those threads.
  */
  if ( gl_GlobalInvocationID.x >= dim.x || gl_GlobalInvocationID.y >= dim.y )
    return;


  g_camera = ConstructCamera( ubo.origin, ubo.lookat, vec3( 0.0f, 1.0f, 0.0f ),
                              ubo.fov, float( dim.x ) / float( dim.y ) );

  vec3 finalColor = vec3( 0.0f );

  float t = ubo.randomSeed;

  vec2 uv = vec2( ( gl_GlobalInvocationID.x ) / ( dim.x - 1.0 ),
                  ( gl_GlobalInvocationID.y ) / ( dim.y - 1.0 ) );

  vec4 seed = {uv.x + t * 41.13, uv.y + t * 113.0, uv.x - t * 7.57,
               uv.y - t * 67.0};

  for ( int i = 0; i < ubo.samplesPerPixel; i++ ) {

    // Jittering the uv-coordinates a litle bit to get
    // anti-aliasing by choosing a randon point in a
    // pixel and send a new ray from that point
    vec2 jitter = {gpu_rnd( seed ), gpu_rnd( seed )};
    jitter = jitter * 2.0 - 1.0;
    uv += ( jitter / dim ) * ubo.aaStrength;

    Ray ray = GetCameraRay( g_camera, uv );

    finalColor += scatter( ray, seed );
  }


  // Gamma correction
  finalColor /= ubo.samplesPerPixel;
  finalColor =
      vec3( sqrt( finalColor.x ), sqrt( finalColor.y ), sqrt( finalColor.z ) );

  vec3 prev_frame =
      imageLoad( resultImage, ivec2( gl_GlobalInvocationID.xy ) ).rgb;
  vec3 curr_frame = finalColor;

  if ( ubo.anyKeyIsPressed == 1 ) {
    imageStore( resultImage, ivec2( gl_GlobalInvocationID.xy ),
                vec4( curr_frame, 1.0f ) );
  } else {
    imageStore( resultImage, ivec2( gl_GlobalInvocationID.xy ),
                vec4( prev_frame + curr_frame, 1.0f ) );
  }
}

