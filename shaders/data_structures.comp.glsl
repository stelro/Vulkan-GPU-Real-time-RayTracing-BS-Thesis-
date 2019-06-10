/* =======================================================================
   $File: data_structures.glsl
   $Date: 17/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

struct Ray {

  vec3      origin;
  vec3      direction;

};

struct Camera {

  vec3      origin;
  vec3      horizontal;
  vec3      vertical;
  vec3      bottom_left_corner;

};

struct Sphere {

  vec3      center;
  vec3      diffuse;
  float     radius;
  int       material;
  int       object_id;
  float     ref_index;
  float     fuzz;

};


struct Plane {

  vec3      normal;
  vec3      diffuse;
  float     distance;
  int       material;
  int       object_id;
  float     ref_index;
  float     fuzz;

};

struct Rectangle {

  vec4      aabb;
  vec3      diffuse;
  float     k;
  int       side;
  int       material;
  int       object_id;
  float     ref_index;
  float     fuzz;
  bool      reverse_normals;

};

struct Box {

  vec3      vmin;
  vec3      vmax;
  vec3      diffuse;
  int       material;
  int       object_id;
  float     ref_index;
  float     fuzz;

};

struct InteractionRecord {

  /**
   * Interaction Record, is a object that represents local information
   * at a point on a surface.
  **/

  vec3      normal;
  float     t;
  vec3      position;
  vec3      albedo;
  vec3      emitted;
  int       material;
  float     ref_index;
  float     fuzz;
  vec2      random;

};

// Camera - Ray Helper function defenitions
Camera ConstructCamera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {

  vec3 u, v, w;
  float theta = vfov * gc_PI/180;
  float half_height = tan(theta/2);
  float half_width = aspect * half_height;


  Camera cam;
  cam.origin = lookfrom;
  w = normalize(lookfrom - lookat);
  u = normalize(cross(vup, w));
  v = cross(w, u);
  cam.bottom_left_corner = cam.origin - half_width * u - half_height*v - w;
  cam.horizontal = 2 * half_width * u;
  cam.vertical = 2 * half_height * v;


  return cam;
}

Ray GetCameraRay(in Camera camera, vec2 uv) {
  return Ray(camera.origin,
         camera.bottom_left_corner + uv.x * camera.horizontal + uv.y * camera.vertical - camera.origin );
}

vec3 GetPointAt(in Ray ray, float t) {
  return ray.origin + t * ray.direction;
}

// Spahes Helper functions defenitions
vec3 SphereNormal(in Sphere sphere, in vec3 position) {
  return (position - sphere.center) / sphere.radius;
}

// Simple function that returns the normals of a box
// of each side.
vec3 getBoxNormal(Box box, vec3 point) {
  vec3 normal;
  float min = 0.0001f;

  if ( abs(point.x - box.vmin.x ) < min )
    return vec3(-1.0f, 0.0f, 0.0f);
  if ( abs(point.x - box.vmax.x ) < min )
    return vec3(1.0f, 0.0f, 0.0f);
  if ( abs(point.y - box.vmin.y ) < min )
    return vec3(0.0f, -1.0f, 0.0f);
  if ( abs(point.y - box.vmax.y ) < min )
    return vec3(0.0f, 1.0f, 0.0f);
  if ( abs(point.z - box.vmin.z ) < min )
    return vec3(0.0f, 0.0f, -1.0f);
  if ( abs(point.z - box.vmax.z ) < min )
    return vec3(0.0f, 0.0f, 1.0f);

}
