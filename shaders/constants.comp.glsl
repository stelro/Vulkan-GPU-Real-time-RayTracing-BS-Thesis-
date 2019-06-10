/* =======================================================================
   $File: constants.glsl
   $Date: 17/6/2018
   $Revision:
   $Creator: Rostislav Orestis Stelmach
   $Notice:  This file is a part of Thesis project ( stracer ) for
                 the Technical Educational Institute of Western Macedonia
                 Supervisor: Dr. George Sisias
   ======================================================================== */

// Defines some global constants need by the project
// gc_ prefix stands for " global constant "

/* Configuration Constants */
const int       gc_textureDimension           = 1024;

/* Math Constants */
const float     gc_PI                         = 3.141592653589793;

/* Raytracing Constants */
// The maximum 't' intersection point to start with
const float     gc_interMaxLength             = 10000.0f;
// The minimum 't' intersection point of the ray
const float     gc_epsilon                    = 0.001f;

/* Objects Materials */
const int       gc_MaterialLambertian         = 0;
const int       gc_MaterialMetal              = 1;
const int       gc_MaterialDialectric         = 2;
const int       gc_MaterialEmitter            = 3;

const int       gc_RectXZSide                 = 0;
const int       gc_RectYZSide                 = 1;
const int       gc_RectXYSide                 = 2;

const vec3      gc_ambient                    = vec3(0.01f, 0.01f, 0.01f);
const float     gc_specularStrength           = 0.5f;

// Compute Shader Workgroup Size
#define WORKGROUP_SIZE 8
