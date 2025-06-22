#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include <png.h>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <GLFW/glfw3.h>

#ifndef SOYA_LIB_H
#define SOYA_LIB_H

// FORWARD DECLARATIONS

static inline GLuint syLoadDefaultShaderProgram();
/* static inline void sySetDefaultShaderUniforms(GLuint shader); */

////////////////////////////////////////////////////////////
//
// DEFAULTS
//

#define SY_DEFAULT_GL_VERSION_MAJOR 4
#define SY_DEFAULT_GL_VERSION_MINOR 3
#define SY_DEFAULT_WINDOW_WIDTH 1280
#define SY_DEFAULT_WINDOW_HEIGHT 720
#define SY_DEFAULT_WINDOW_SAMPLES 8

#define SY_BLACK 0, 0, 0, 1
#define SY_BLUE 0, 0, 1, 1
#define SY_CYAN 0, 1, 1, 1
#define SY_GREEN 0, 1, 0, 1
#define SY_MAGENTA 1, 0, 1, 1
#define SY_RED 1, 0, 0, 1
#define SY_WHITE 1, 1, 1, 1
#define SY_YELLOW 1, 1, 0, 1

////////////////////////////////////////////////////////////
//
// TYPE ALIASES
//

typedef uint64_t u64;
typedef size_t usize;

////////////////////////////////////////////////////////////
//
// syRenderer
//

typedef struct syRenderer syRenderer;
static inline void syRendererInit(syRenderer *r, int width, int height);

////////////////////////////////////////////////////////////
//
// syApp
//

typedef struct syApp syApp;
static inline void syAppPreConfigure(syApp *app);

////////////////////////////////////////////////////////////
//
// DRAWING
//

static inline void syClear(float r, float g, float b, float a);

static inline void sySetColor(syApp *app, float r, float g, float b, float a);

static inline void syDrawUnindexed(syApp *app, float *vertices, float *colors,
                                   usize n, GLenum mode);

static inline void syDrawTriangle(syApp *app, float x1, float y1, float z1,
                                  float x2, float y2, float z2, float x3,
                                  float y3, float z3);

static inline void syDrawQuad(syApp *app, float x, float y, float width,
                              float height);

static inline void syDrawLine(syApp *app, float x1, float y1, float z1,
                              float x2, float y2, float z2);

static inline void syDrawLines(syApp *app, float *vertices, int n);

static inline void syDrawPolygon(syApp *app, float x, float y, float z,
                                 float radius, usize numSides);

////////////////////////////////////////////////////////////
//
// TRANSFORMATIONS
//

static inline void syTranslate(syApp *app, float x, float y, float z);

// @param angle Angle in degrees
static inline void syRotate(syApp *app, float angle, float x, float y, float z);

// Reset model matrix to identity (default)
static inline void syResetTransformations(syApp *app);

////////////////////////////////////////////////////////////
//
// syShader
//

// OpenGL Shader handle
typedef GLuint syShader;

static inline void syShaderSetRendererUniforms(syRenderer *r, syShader s);

static inline void syShaderSetUniformMat4fv(GLuint shader,
                                            const char *uniformName,
                                            const float *value);

////////////////////////////////////////////////////////////
//
// BUFFERS
//

static inline void syWriteBuffer(GLenum target, GLuint buffer, GLsizeiptr size,
                                 const void *data, GLenum usage);

static inline void syWriteArrayBuffer(GLuint buffer, size_t size, void *data);

////////////////////////////////////////////////////////////
//
// VERTEX ATTRIBUTES
//

static inline void syVertexAttribute(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void *pointer);

static inline void syVertexAttribute2f(GLuint index);

static inline void syVertexAttribute3f(GLuint index);

static inline void syVertexAttribute4f(GLuint index);

////////////////////////////////////////////////////////////
//
// IO - file reading and writing
//

static inline const char *syReadFile(const char *filePath);

static inline int syWriteFrameRGB(const char *filename, int width, int height);

static inline int syWritePngRGB(const char *filename, void *data, int width,
                                int height);

////////////////////////////////////////////////////////////
//
// USER IMPLEMENTED
//

extern void configure(syApp *app);
extern void setup(syApp *app);
extern void loop(syApp *app);

////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
//
// syRenderer
//

typedef struct syRenderer
{
  GLuint vao, vbo, cbo, ibo;
  syShader shader;
  mat4s modelMatrix, viewMatrix, projectionMatrix;
  /* mat4s modelViewProjectionMatrix; */
  float color[4];
} syRenderer;

static inline void syRendererInit(syRenderer *r, int width, int height)
{
  glGenVertexArrays(1, &r->vao);
  glGenBuffers(1, &r->vbo);
  glGenBuffers(1, &r->cbo);
  glGenBuffers(1, &r->ibo);
  r->shader = syLoadDefaultShaderProgram();
  r->color[0] = 1;
  r->color[1] = 1;
  r->color[2] = 1;
  r->color[3] = 1;
  glUseProgram(r->shader);

  r->projectionMatrix = glms_ortho(0, width, 0, height, 0.1f, 100.0);
  r->viewMatrix = glms_translate_make((vec3s){{0, 0, -1}});
  r->modelMatrix = glms_mat4_identity();

  /* r->modelViewProjectionMatrix = */
  /*     glms_mul(projMat, (glms_mul(viewMat, modelMat))); */
}

////////////////////////////////////////////////////////////
//
// syApp
//

typedef struct syApp
{
  // Window width. Default: 1280
  int width;

  // Window height. Default: 720
  int height;

  // Pointer to app's window
  GLFWwindow *window;

  // Window title. Default: empty string
  const char *title;

  // Major OpenGL version number. Default: 4
  int glVersionMajor;

  // Major OpenGL version number. Default: 3
  int glVersionMinor;

  // Number of samples used for multisampling. Default: 8
  int samples;

  // Current frame number.
  u64 frameNum;

  // Current frame rate.
  float fps;

  // Time in seconds since initialization.
  double time;

  syRenderer renderer;
} syApp;

static inline void syAppPreConfigure(syApp *app)
{
  app->width = SY_DEFAULT_WINDOW_WIDTH;
  app->height = SY_DEFAULT_WINDOW_HEIGHT;
  app->samples = SY_DEFAULT_WINDOW_SAMPLES;
  app->title = "";
  app->glVersionMajor = SY_DEFAULT_GL_VERSION_MAJOR;
  app->glVersionMinor = SY_DEFAULT_GL_VERSION_MINOR;
}

////////////////////////////////////////////////////////////
//
// DRAWING
//

static inline void syClear(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static inline void sySetColor(syApp *app, float r, float g, float b, float a)
{
  app->renderer.color[0] = r;
  app->renderer.color[1] = g;
  app->renderer.color[2] = b;
  app->renderer.color[3] = a;
}

static inline void syDrawUnindexed(syApp *app, float *vertices, float *colors,
                                   usize n, GLenum mode)
{
  glBindVertexArray(app->renderer.vao);
  syWriteArrayBuffer(app->renderer.vbo, sizeof(float) * n * 3, vertices);
  syVertexAttribute3f(0);

  if (colors == 0)
  {
    float *c = calloc(n * 4, sizeof(float));
    for (size_t i = 0; i < n; i++)
    {
      memcpy(&c[i * 4], app->renderer.color, sizeof(app->renderer.color));
    }
    syWriteArrayBuffer(app->renderer.cbo, sizeof(float) * n * 4, c);
    free(c);
  }
  else
  {
    syWriteArrayBuffer(app->renderer.cbo, sizeof(float) * n * 4, colors);
  }
  syVertexAttribute4f(1);

  syShaderSetRendererUniforms(&app->renderer, app->renderer.shader);
  glDrawArrays(mode, 0, n);
}

static inline void syDrawTriangle(syApp *app, float x1, float y1, float z1,
                                  float x2, float y2, float z2, float x3,
                                  float y3, float z3)
{
  float vertices[] = {x1, y1, z1, x2, y2, z2, x3, y3, z3};
  syDrawUnindexed(app, vertices, NULL, 3, GL_TRIANGLES);
}

static inline void syDrawQuad(syApp *app, float x, float y, float width,
                              float height)
{
  float vertices[] = {x,         y,          0, x + width, y,          0,
                      x + width, y - height, 0, x,         y - height, 0};
  syDrawUnindexed(app, vertices, NULL, 4, GL_TRIANGLE_FAN);
}

static inline void syDrawLine(syApp *app, float x1, float y1, float z1,
                              float x2, float y2, float z2)
{
  float vertices[] = {x1, y1, z1, x2, y2, z2};
  syDrawUnindexed(app, vertices, NULL, 2, GL_LINES);
}

static inline void syDrawLines(syApp *app, float *vertices, int n)
{
  syDrawUnindexed(app, vertices, NULL, n, GL_LINE_STRIP);
}

static inline void syDrawPolygon(syApp *app, float x, float y, float z,
                                 float radius, usize numSides)
{
  if (numSides < 3)
  {
    perror("numSides must be greater than 3");
  }
  usize numItems = numSides + 2; // 1 extra for the center and 1 for the end
  float *vertices = (float *)calloc(numItems * 3, sizeof(float));

  vertices[0] = x;
  vertices[1] = y;
  vertices[2] = z;

  float t = GLM_PI * 2.f / (float)numSides;
  for (size_t i = 0; i < numSides; i++)
  {
    vertices[(i + 1) * 3] = cosf(t * i) * radius + x;
    vertices[(i + 1) * 3 + 1] = sinf(t * i) * radius + y;
    vertices[(i + 1) * 3 + 2] = 0;
  }

  vertices[numItems * 3 - 3] = vertices[3];
  vertices[numItems * 3 - 2] = vertices[4];
  vertices[numItems * 3 - 1] = vertices[5];

  syDrawUnindexed(app, vertices, NULL, numSides + 2, GL_TRIANGLE_FAN);
}

////////////////////////////////////////////////////////////
//
// TRANSFORMATIONS
//

static inline void syTranslate(syApp *app, float x, float y, float z)
{
  app->renderer.modelMatrix =
      glms_translate(app->renderer.modelMatrix, (vec3s){{x, y, z}});
}

static inline void syRotate(syApp *app, float angle, float x, float y, float z)
{

  app->renderer.modelMatrix =
      glms_rotate(app->renderer.modelMatrix, angle, (vec3s){{x, y, z}});
}

static inline void syResetTransformations(syApp *app)
{
  app->renderer.modelMatrix = glms_mat4_identity();
}

////////////////////////////////////////////////////////////
//
// syShader
//

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

static inline void syShaderSetRendererUniforms(syRenderer *r, syShader s)
{
  auto mat =
      glms_mul(r->projectionMatrix, (glms_mul(r->viewMatrix, r->modelMatrix)));
  syShaderSetUniformMat4fv(s, "modelViewProjectionMatrix", (float *)&mat);
}

static inline void syShaderSetUniformMat4fv(GLuint shader,
                                            const char *uniformName,
                                            const float *value)
{
  GLint u = glGetUniformLocation(shader, uniformName);
  glUniformMatrix4fv(u, 1, GL_FALSE, value);
}

static inline void syShaderSetUniformTexture(GLuint shader, const char *name,
                                             GLuint texture)
{
  auto uTex = glGetUniformLocation(shader, name);
  glUniform1i(uTex, texture);
  glActiveTexture(GL_TEXTURE0 + texture);
  glBindTexture(GL_TEXTURE_2D, texture);
}

////////////////////////////////////////////////////////////
//
// BUFFERS
//

static inline void syWriteBuffer(GLenum target, GLuint buffer, GLsizeiptr size,
                                 const void *data, GLenum usage)
{
  glBindBuffer(target, buffer);
  glBufferData(target, size, data, usage);
}

static inline void syWriteArrayBuffer(GLuint buffer, size_t size, void *data)
{
  syWriteBuffer(GL_ARRAY_BUFFER, buffer, size, data, GL_DYNAMIC_DRAW);
}

////////////////////////////////////////////////////////////
//
// VERTEX ATTRIBUTES
//

// Shorthand for calling `glVertexAttribPointer` and
// `glEnableVertexAttribArray`.
static inline void syVertexAttribute(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void *pointer)
{
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);
}

static inline void syVertexAttribute2f(GLuint index)
{
  syVertexAttribute(index, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), NULL);
}

static inline void syVertexAttribute3f(GLuint index)
{
  syVertexAttribute(index, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);
}

static inline void syVertexAttribute4f(GLuint index)
{
  syVertexAttribute(index, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
}

////////////////////////////////////////////////////////////
//
// IO - file reading and writing
//

static inline const char *syReadFile(const char *filePath)
{
  FILE *file = fopen(filePath, "rb");
  if (!file)
  {
    perror("Failed to open file");
    return NULL;
  }

  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);
  if (fileSize < 0)
  {
    perror("Failed to determine file size");
    return NULL;
  }

  char *buffer = (char *)malloc((size_t)fileSize + 1);
  if (!buffer)
  {
    perror("Failed to allocate buffer");
    fclose(file);
    return NULL;
  }

  size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
  buffer[bytesRead] = '\0'; // Null-terminate

  fclose(file);
  return buffer;
}

static inline int syWriteFrameRGB(const char *filename, int width, int height)
{
  int success = -1;
  void *pixels = malloc(width * height * 3 * sizeof(int));
  if (!pixels)
  {
    return success;
  }
  glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
  success = syWritePngRGB(filename, pixels, width, height);
  free(pixels);
  pixels = NULL;
  return success;
}

static inline int syWritePngRGB(const char *filename, void *data, int width,
                                int height)
{
  FILE *fp = fopen(filename, "wb");
  if (!fp)
  {
    perror("fopen");
    return 1;
  }

  png_structp png =
      png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png)
  {
    fclose(fp);
    return 1;
  }

  png_infop info = png_create_info_struct(png);
  if (!info)
  {
    png_destroy_write_struct(&png, NULL);
    fclose(fp);
    return 1;
  }

  if (setjmp(png_jmpbuf(png)))
  {
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 1;
  }

  png_init_io(png, fp);

  png_set_IHDR(png, info, width, height, 8, PNG_COLOR_TYPE_RGB,
               PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_BASE,
               PNG_FILTER_TYPE_BASE);

  png_write_info(png, info);

  png_bytep d = (png_bytep)data;

  png_bytep *row_pointers = malloc(sizeof(png_bytep) * height);
  if (!row_pointers)
  {
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 1;
  }

  for (int y = 0; y < height; y++)
  {
    row_pointers[y] = d + y * width * 3;
  }

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  // Cleanup
  free(row_pointers);
  png_destroy_write_struct(&png, &info);
  fclose(fp);

  return 0;
}

////////////////////////////////////////////////////////////
//
// LIFECYLE HOOKS
//

// Called at the very beginning, before `syAppPreConfigure`, `configure`,
// `syMainPostConfigure` and `setup`.
static inline bool syMainInit();

// Called after `syMainInit`, `syAppPreConfigure`, `configure` and before
// `setup`. After this is called, the default window GL context becomes
// available.
static inline bool syMainPostConfigure(syApp *app);

typedef struct syGeometry
{
  // XYZ
  float *vertices;
  // RGBA
  float *colors;
  uint32_t *indices;
  uint32_t numVertices;
  uint32_t numIndices;
} syGeometry;

static inline void syGeometry_Destroy(syGeometry *g)
{
  free(g->vertices);
  free(g->colors);
  free(g->indices);
  g->vertices = NULL;
  g->colors = NULL;
  g->indices = NULL;
}

typedef struct syPolyline
{
  float *vertices;
  // num floats
  size_t size;
  // num floats
  size_t capacity;
} syPolyline;

static inline void syPolyline_init(syPolyline *l)
{
  size_t initialCapacity = 30;
  l->vertices = (float *)malloc(sizeof(float) * initialCapacity);
  l->size = 0;
  l->capacity = initialCapacity;
}

static inline void syPolyline_addVertex(syPolyline *l, float x, float y,
                                        float z)
{
  if (l->size + 3 >= l->capacity)
  {
    l->vertices =
        (float *)realloc(l->vertices, l->capacity * 2 * sizeof(float));
    l->capacity *= 2;
  }
  l->vertices[l->size] = x;
  l->vertices[l->size + 1] = y;
  l->vertices[l->size + 2] = z;
  l->size += 3;
}

static inline void syPolyline_Print(syPolyline *l)
{
  printf("Num vertices: %zu\n", l->size / 3);
  printf("Capacity: %zu\n", l->capacity / 3);
  printf("Data: ");
  for (size_t i = 0; i < l->size / 3; i++)
  {
    printf("[%f, ", l->vertices[i * 3]);
    printf("%f, ", l->vertices[i * 3 + 1]);
    printf("%f], ", l->vertices[i * 3 + 2]);
  }
  printf("\n");
}

static inline void syPolyline_Destroy(syPolyline *l)
{
  free(l->vertices);
  l->vertices = NULL;
}

// @returns `true` if `shader` has compiled susyessfully. Otherwise `false`.
static inline bool syShaderCompileSuccess(const GLuint shader)
{
  int susyess;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &susyess);
  return susyess == 1;
}

// @returns the info log to `shader` as a `const char *` owned by the caller.
static inline const char *syGetShaderInfoLog(const GLuint shader)
{
  char *buffer = (char *)malloc(512);
  glGetShaderInfoLog(shader, 512, NULL, buffer);
  return buffer;
}

// @returns a `GLuint` representing the shader compiled, given the
// `shaderSource` string and the `shaderType`. This operation may fail.
static inline GLuint syLoadShaderFromSource(const char *shaderSource,
                                            GLenum shaderType)
{
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  if (!syShaderCompileSuccess(shader))
  {
    perror(syGetShaderInfoLog(shader));
  }
  return shader;
}

// @returns a `GLuint` representing the shader compiled from `shaderPath` and
// given the `shaderType`. This operation may fail.
static inline GLuint syLoadShader(const char *shaderPath, GLenum shaderType)
{
  const char *src = syReadFile(shaderPath);
  GLuint shader = syLoadShaderFromSource(src, shaderType);
  free((void *)src);
  return shader;
}

// @returns a shader compiled with the shader source strings. If either
// `fragmentShaderSource` or `vertexShaderSource` is `NULL`, the respective
// default shaders will be used instead.
static inline GLuint syCreateShaderProgramFromSource(
    const char *fragmentShaderSource, const char *vertexShaderSource)
{
  const char *fsSrc = fragmentShaderSource == NULL ? SY_DEFAULT_FRAGMENT_SHADER
                                                   : fragmentShaderSource;
  const char *vsSrc = vertexShaderSource == NULL ? SY_DEFAULT_VERTEX_SHADER
                                                 : vertexShaderSource;

  const GLuint fs = syLoadShaderFromSource(fsSrc, GL_FRAGMENT_SHADER);
  const GLuint vs = syLoadShaderFromSource(vsSrc, GL_VERTEX_SHADER);

  const GLuint shader = glCreateProgram();

  glAttachShader(shader, vs);
  glAttachShader(shader, fs);
  glLinkProgram(shader);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return shader;
}

// Creates and returns the default shader program used by the main renderer.
static inline GLuint syLoadDefaultShaderProgram()
{
  return syCreateShaderProgramFromSource(SY_DEFAULT_FRAGMENT_SHADER,
                                         SY_DEFAULT_VERTEX_SHADER);
}

static inline void sySetUniform1f(GLuint shader, const char *uniformName,
                                  float f)
{
  GLint u = glGetUniformLocation(shader, uniformName);
  glUniform1f(u, f);
}

static inline void sySetUniform2f(GLuint shader, const char *uniformName,
                                  float f1, float f2)
{
  GLint u = glGetUniformLocation(shader, uniformName);
  glUniform2f(u, f1, f2);
}

static inline void sySetUniformMat4fv(GLuint shader, const char *uniformName,
                                      bool transpose, const float *value)
{
  GLint u = glGetUniformLocation(shader, uniformName);
  glUniformMatrix4fv(u, 1, transpose, value);
}

static inline void sySetUniformTexture(GLuint shader, const char *name,
                                       GLuint texture)
{
  auto uTex = glGetUniformLocation(shader, name);
  glUniform1i(uTex, texture);
  glActiveTexture(GL_TEXTURE0 + texture);
  glBindTexture(GL_TEXTURE_2D, texture);
}

////////////////////////////////////////////////////////////
//
// GLFW CALLBACKS
//

static inline void syOnError(int error_code, const char *description)
{
  (void)error_code;
  perror(description);
}

static inline void syOnFrameBufferSize(GLFWwindow *window, int width,
                                       int height)
{
  (void)window;
  glViewport(0, 0, width, height);
  /* SY_DEFAULT_PROJECTION_MATRIX = */
  /*     glms_ortho(0, (float)width, 0, (float)height, 0.1f, 100.0); */
}

static inline void syOnKey(GLFWwindow *window, int key, int scancode,
                           int action, int mods)
{
  (void)scancode;
  (void)mods;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, 1);
  }
}

////////////////////////////////////////////////////////////
//
// LIFECYLE HOOKS
//

static inline bool syMainInit()
{
  glfwSetErrorCallback(syOnError);
  puts("Initializing GLFW");
  if (!glfwInit())
  {
    return false;
  }
  return true;
}

static inline bool syMainPostConfigure(syApp *app)
{
  printf("GL Version: %i.%i\n", app->glVersionMajor, app->glVersionMinor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, app->glVersionMajor);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, app->glVersionMinor);
  glfwWindowHint(GLFW_SAMPLES, 8);

  puts("Creating window");
  app->window =
      glfwCreateWindow(app->width, app->height, app->title, NULL, NULL);

  if (!app->window)
  {
    glfwTerminate();
    return false;
  }

  puts("Obtaining context");
  glfwMakeContextCurrent(app->window);
  glViewport(0, 0, app->width, app->height);
  return true;
}

int main()
{
  int success = -1;
  if (!syMainInit())
  {
    return success;
  }

  syApp app = {0};
  syAppPreConfigure(&app);
  configure(&app);
  if (!syMainPostConfigure(&app))
  {
    return success;
  };
  syRendererInit(&app.renderer, app.width, app.height);
  glPixelStorei(GL_PACK_ALIGNMENT, 2);
  setup(&app);

  //
  // Callbacks
  //
  glfwSetFramebufferSizeCallback(app.window, syOnFrameBufferSize);
  glfwSetKeyCallback(app.window, syOnKey);

  /* glGenVertexArrays(1, &SY_MAIN_VAO); */
  /* glGenBuffers(1, &SY_MAIN_VBO); */
  /* glGenBuffers(1, &SY_MAIN_CBO); */
  /* glGenBuffers(1, &SY_MAIN_IBO); */
  /* SY_CURRENT_SHADER_PROGRAM = syLoadDefaultShaderProgram(); */
  /* SY_DEFAULT_SHADER_PROGRAM = SY_CURRENT_SHADER_PROGRAM; */
  /* glUseProgram(SY_CURRENT_SHADER_PROGRAM); */

  /* SY_DEFAULT_PROJECTION_MATRIX = glms_ortho( */
  /*     0, SY_CURRENT_WINDOW_WIDTH, 0, SY_CURRENT_WINDOW_HEIGHT, 0.1f, 100.0);
   */
  /* SY_DEFAULT_VIEW_MATRIX = glms_translate_make((vec3s){{0, 0, -1}}); */
  /* SY_DEFAULT_MODEL_MATRIX = glms_mat4_identity(); */

  double prevTime = glfwGetTime();
  while (!glfwWindowShouldClose(app.window))
  {
    /* GLint umodel = glGetUniformLocation(SY_CURRENT_SHADER_PROGRAM, "model");
     */
    /* GLint uview = glGetUniformLocation(SY_CURRENT_SHADER_PROGRAM, "view"); */
    /* GLint uprojection = */
    /*     glGetUniformLocation(SY_CURRENT_SHADER_PROGRAM, "projection"); */

    /* glUniformMatrix4fv(umodel, 1, GL_FALSE, */
    /*                    (float *)&SY_DEFAULT_MODEL_MATRIX.raw); */
    /* glUniformMatrix4fv(uview, 1, GL_FALSE, */
    /*                    (float *)&SY_DEFAULT_VIEW_MATRIX.raw); */
    /* glUniformMatrix4fv(uprojection, 1, GL_FALSE, */
    /*                    (float *)&SY_DEFAULT_PROJECTION_MATRIX.raw); */

    loop(&app);
    glfwSwapBuffers(app.window);
    glfwPollEvents();
    app.frameNum++;
    app.time = glfwGetTime();
    app.fps = 1.0 / (app.time - prevTime);
    prevTime = app.time;
  };
  glfwTerminate();
  return 0;
}

#endif // SOYA_LIB_H
