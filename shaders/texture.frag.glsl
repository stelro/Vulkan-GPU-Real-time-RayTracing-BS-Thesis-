#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (binding = 0) uniform sampler2D samplerColor;

layout (location = 0) in vec2 inUV;

layout (push_constant) uniform PushCostants {
  float Sample;
  int anyKeyPressed;
};

layout (location = 0) out vec4 outFragColor;

void main() 
{
    //outFragColor = vec4(texture(samplerColor, vec2(inUV.s, 1.0 - inUV.t)).rgb, 1.0f);
    vec3 fianlColor = texture(samplerColor, vec2(inUV.s, 1.0 - inUV.t)).rgb;

    if ( anyKeyPressed == 0) {
      fianlColor /= Sample + 1.0f;
    }

    outFragColor = vec4(fianlColor, 1.0f);

}