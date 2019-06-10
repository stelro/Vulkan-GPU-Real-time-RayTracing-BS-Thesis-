#!/usr/bin/env bash
../bin/glslangValidator -V texture.frag.glsl -o texture.frag.spv
../bin/glslangValidator -V texture.vert.glsl -o texture.vert.spv
../bin/glslangValidator -V render.comp.glsl -o render.comp.spv
../bin/glslangValidator -V gui.vert.glsl -o gui.vert.spv
../bin/glslangValidator -V gui.frag.glsl -o gui.frag.spv
