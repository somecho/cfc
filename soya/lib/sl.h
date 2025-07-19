/**
 * @file sl.h
 * @brief DSL for creating GLSL shaders inline
 *
 * ### Example Usage
 * @include sysl.c
 * */

#ifndef _SOYA_SL_H
#define _SOYA_SL_H

#define SYSL_DELIM ";\n"

/**
 * @brief Expands to `"#version version\n"`
 * */
#define SYSL_VERSION(version) "#version " #version "\n"

/**
 * @brief Expands to `"uniform type identifier;\n"`
 * */
#define SYSL_UNIFORM(type, identifier) \
  "uniform " #type " " #identifier SYSL_DELIM

/**
 * @brief Expands to `"uniform float identifier;\n"`
 * */
#define SYSL_UNIFORM_FLOAT(identifier) SYSL_UNIFORM(float, identifier)

/**
 * @brief Expands to `"uniform vec2 identifier;\n"`
 * */
#define SYSL_UNIFORM_VEC2(identifier) SYSL_UNIFORM(vec2, identifier)

/**
 * @brief Expands to `"uniform vec3 identifier;\n"`
 * */
#define SYSL_UNIFORM_VEC3(identifier) SYSL_UNIFORM(vec3, identifier)

/**
 * @brief Expands to `"uniform vec4 identifier;\n"`
 * */
#define SYSL_UNIFORM_VEC4(identifier) SYSL_UNIFORM(vec4, identifier)

/**
 * @brief Expands to `"out type identifier;\n"`
 * */
#define SYSL_OUT(type, identifier) "out " #type " " #identifier SYSL_DELIM

/**
 * @brief Expands to `"out vec4 identifier;"`
 * */
#define SYSL_OUT_VEC4(identifier) SYSL_OUT(vec4, identifier)

#define SYSL_IN(type, identifier) "in " #type " " #identifier SYSL_DELIM

#define SYSL_IN_VEC4(identifier) SYSL_IN(vec4, identifier)

/**
 * @brief Expands to `"layout (location = loc) statement"`
 *
 * ### Example usage:
 * ```
 * SYSL_LOCATION(0, SYSL_OUT_VEC4(FragColor))
 * // Expands to
 * "layout (location = 0) out vec4 FragColor;\n"
 * ```
 * */
#define SYSL_LOCATION(loc, statement) "layout (location = " #loc ") " statement

/**
 * @brief Declares main glsl block
 *
 * ### Example usage:
 * ```
 * SYSL_MAIN(
 *   vec3 red = vec3(1., 0., 0.);
 *   FragColor = vec4(red, 1.);
 * )
 * // Expands to
 * "void main(){vec3 red = vec3(1., 0., 0.); FragColor = vec4(red, 1.);}"
 * ```
 * */
#define SYSL_MAIN(s) "void main(){" #s "}"

#define SYSL_FN(fn) #fn

#define SYSL_PREAMBLE(...) __VA_ARGS__

#define SYSL(...) __VA_ARGS__

#endif  // _SOYA_SL_H
