#ifndef _SOYA_DEFAULTSHADERS_H
#define _SOYA_DEFAULTSHADERS_H

// Attributes:
// - location 0 - vec3 aPos
// - location 1 - vec4 aColor
//
// Uniforms:
// - mat4 projection
// - mat4 model
// - mat4 view
//
// Outputs:
// - vec4 vColor
// - vec4 vPos
static const char *SY_DEFAULT_VERTEX_SHADER =
    "#version 430 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "uniform mat4 modelViewProjectionMatrix;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "out vec4 vColor;\n"
    "out vec4 vPos; \n"
    "void main()\n"
    "{\n"
    /* "   vec4 outPos = projection * view * model * vec4(aPos, 1.0);" */
    "   vec4 outPos = modelViewProjectionMatrix * vec4(aPos, 1.0);"
    "   gl_Position = outPos;\n"
    "   vPos = outPos;\n"
    "   vColor = aColor;\n"
    "}\0";

// Inputs:
// - vec4 vColor
//
// Outputs:
// - location 0 - vec4 FragColor
static const char *SY_DEFAULT_FRAGMENT_SHADER =
    "#version 430 core\n"
    "in vec4 vColor;\n"
    "layout (location = 0) out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vColor;\n"
    "}\n\0";

#endif  // _SOYA_DEFAULTSHADERS_H
