/**
 * @file sl.h
 * @brief GLSL shader creation macros
 *
 * ### Example Usage
 * @include sysl.c
 * */

#pragma once

/**
 * Semicolon followed by a linebreak.
 * */
#define SYSL_DELIM ";\n"

/**
 * GLSL version declaration.
 *
 * ```
 * SYSL_VERSION(430)
 * ```
 * expands to
 * ```
 * "#version version\n"
 * ```
 * @since 0.1.0
 * */
#define SYSL_VERSION(version) "#version " #version "\n"

/**
 * GLSL uniform declaration.
 *
 * ```
 * SYSL_UNIFORM(vec2, uRes)
 * ```
 * expands to
 * ```
 * "uniform vec2 uRes;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_UNIFORM(type, identifier) \
  "uniform " #type " " #identifier SYSL_DELIM

/**
 * GLSL float uniform declaration.
 * ```
 * SYSL_UNIFORM_FLOAT(uTime)
 * ```
 * Expands to
 * ```
 * "uniform float uTime;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_UNIFORM_FLOAT(identifier) SYSL_UNIFORM(float, identifier)

/**
 * GLSL `vec2` uniform declaration.
 * ```
 * SYSL_UNIFORM_VEC2(uRes)
 * ```
 * expands to
 * ```
 * "uniform vec2 uRes;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_UNIFORM_VEC2(identifier) SYSL_UNIFORM(vec2, identifier)

/**
 * GLSL `vec3` uniform declaration.
 * ```
 * SYSL_UNIFORM_VEC3(uCol)
 * ```
 * expands to
 * ```
 * "uniform vec3 uCol;"
 * ````
 * @since 0.1.0
 * */
#define SYSL_UNIFORM_VEC3(identifier) SYSL_UNIFORM(vec3, identifier)

/**
 * GLSL `vec4` uniform declaration.
 * ```
 * SYSL_UNIFORM_VEC4(uRot)
 * ```
 * expands to
 * ```
 * "uniform vec4 uRot;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_UNIFORM_VEC4(identifier) SYSL_UNIFORM(vec4, identifier)

/**
 * GLSL output declaration.
 * ```
 * SYSL_OUT(vec3, vPos)
 * ```
 * expands to
 * ```
 * "out vec3 vPos;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_OUT(type, identifier) "out " #type " " #identifier SYSL_DELIM

/**
 * GLSL `vec4` output declaration.
 * ```
 * SYSL_OUT_VEC4(fragCol)
 * ```
 * expands to
 * ```
 * "out vec4 fragCol;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_OUT_VEC4(identifier) SYSL_OUT(vec4, identifier)

/**
 * GLSL input declaration.
 * ```
 * SYSL_IN(vec3, vPos)
 * ```
 * expands to
 * ```
 * "in vec3 vPos;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_IN(type, identifier) "in " #type " " #identifier SYSL_DELIM

/**
 * GLSL `vec4` input declaration.
 * ```
 * SYSL_IN_VEC4(vPos)
 * ```
 * expands to
 * ```
 * "in vec4 vPos;"
 * ```
 * @since 0.1.0
 * */
#define SYSL_IN_VEC4(identifier) SYSL_IN(vec4, identifier)

/**
 * GLSL location declaration.
 *
 * ```
 * SYSL_LOCATION(0, SYSL_OUT_VEC4(FragColor))
 * ```
 * expands to
 * ```
 * "layout (location = 0) out vec4 FragColor;\n"
 * ```
 * @since 0.1.0
 * */
#define SYSL_LOCATION(loc, statement) "layout (location = " #loc ") " statement

/**
 * GLSL main block declaration.
 *
 * ```
 * SYSL_MAIN(
 *   vec3 red = vec3(1., 0., 0.);
 *   FragColor = vec4(red, 1.);
 * )
 * ```
 * expands to
 * ```
 * "void main(){vec3 red = vec3(1., 0., 0.); FragColor = vec4(red, 1.);}"
 * ```
 * @since 0.1.0
 * */
#define SYSL_MAIN(s) "void main(){" #s "}"

/**
 * GLSL statement block. Can be used to declare and define functions within @ref SYSL_PREAMBLE.
 * @since 0.1.0
 * */
#define SYSL_FN(fn) #fn

/**
 * GLSL preamble block declaration. Version, uniforms, inputs and outputs are declared here. @ref SYSL_FN can also be used here.
 * @since 0.1.0
 * */
#define SYSL_PREAMBLE(...) __VA_ARGS__

/**
 * GLSL shader declaration. This arguments to this macro must contain both and
 * only @ref SYSL_PREAMBLE and @ref SYSL_MAIN, in that order.
 * @since 0.1.0
 * */
#define SYSL(...) __VA_ARGS__
