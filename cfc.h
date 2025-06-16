#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <cglm/cglm.h>
#include <cglm/struct.h>

#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <GLFW/glfw3.h>

#ifndef CC_HEADER
#define CC_HEADER

#ifndef CC_GL_VERSION_MAJOR
#define CC_GL_VERSION_MAJOR 4
#endif
#ifndef CC_GL_VERSION_MINOR
#define CC_GL_VERSION_MINOR 3
#endif

//
// UTILITIES
//

// Opens the file at `filePath` and returns the content as a `const char *`
// owned by the caller. Returns `NULL` if this fails.
static const char *ccReadFile(const char *filePath)
{
  FILE *file = fopen(filePath, "rb");
  if (!file)
  {
    perror("Failed to open file");
    return NULL;
  }

  // Seek to end to determine file size
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);
  if (fileSize < 0)
  {
    perror("Failed to determine file size");
    return NULL;
  }

  // Allocate memory (+1 for null terminator)
  char *buffer = (char *)malloc((size_t)fileSize + 1);
  if (!buffer)
  {
    perror("Failed to allocate buffer");
    fclose(file);
    return NULL;
  }

  // Read the file into the buffer
  size_t bytesRead = fread(buffer, 1, (size_t)fileSize, file);
  buffer[bytesRead] = '\0'; // Null-terminate

  fclose(file);
  return buffer;
}

// FORWARD DECLARATIONS

static GLuint ccLoadDefaultShaderProgram();

// WINDOW MANAGEMENT

static const int CC_DEFAULT_WINDOW_WIDTH = 1200;
static const int CC_DEFAULT_WINDOW_HEIGHT = 800;
static int CC_CURRENT_WINDOW_WIDTH = CC_DEFAULT_WINDOW_WIDTH;
static int CC_CURRENT_WINDOW_HEIGHT = CC_DEFAULT_WINDOW_HEIGHT;
static GLFWwindow *CC_MAIN_WINDOW;

static inline int ccGetWidth()
{
  return CC_CURRENT_WINDOW_WIDTH;
}

static inline int ccGetHeight()
{
  return CC_CURRENT_WINDOW_HEIGHT;
}

static inline void ccSetWindowSize(int width, int height)
{
  glfwSetWindowSize(CC_MAIN_WINDOW, width, height);
  // TODO: recenter window
}

//
// FRAME STATS
//

// Direct variable holding the current frame number
static uint64_t CC_CURRENT_FRAMENUM = 0;

// Direct variable holding the current framerate
static double CC_CURRENT_FPS = 0.0;

// @returns the current frame number
static inline uint64_t ccGetFrameNum()
{
  return CC_CURRENT_FRAMENUM;
}

// @returns the current framerate
static inline double ccGetFps()
{
  return CC_CURRENT_FPS;
}
//
// RENDERER
//

//
typedef struct ccRenderer
{
  // Vertex Array Object
  GLuint vao;
  // Vertex Buffer Object. Internally `GL_ARRAY_BUFFER`.
  GLuint vbo;
  // Color Buffer Object. Internally `GL_ARRAY_BUFFER`.
  GLuint cbo;
  // Index Buffer Object. Internally `GL_ELEMENTT_ARRAY_BUFFER`.
  GLuint ibo;
  GLuint shaderProgram;
  GLenum renderMode;
  uint32_t *indices;
  float *vertices;
  float *colors;
  uint32_t numIndices;
  uint32_t numVertices;
  uint32_t numColors;
  float color[4];
} ccRenderer;

// The main renderer used in the main loop
static ccRenderer CC_MAIN_RENDERER;

// Frees `vertices`, `colors` and `indices`, setting them to `NULL` and resets
// `numVertices`, `numColors` and `numIndices` to `0`.
static void ccResetRendererData(ccRenderer *renderer)
{
  free(renderer->vertices);
  renderer->vertices = NULL;
  renderer->numVertices = 0;
  free(renderer->colors);
  renderer->colors = NULL;
  renderer->numColors = 0;
  free(renderer->indices);
  renderer->indices = NULL;
  renderer->numIndices = 0;
}

// Initializes `renderer` by creating OpenGL resources. A vertex array is
// generated for `vao`. A buffer is generated for `vbo`, `cbo`, `ibo`.
// `ccResetRendererData` is called. `color` is set to white (1,1,1,1) and
// `renderMode` is set to `GL_TRIANGLES`.
static void ccCreateMainRenderer(ccRenderer *renderer)
{
  glGenVertexArrays(1, &renderer->vao);
  renderer->shaderProgram = ccLoadDefaultShaderProgram();
  glGenBuffers(1, &renderer->vbo);
  glGenBuffers(1, &renderer->cbo);
  glGenBuffers(1, &renderer->ibo);
  ccResetRendererData(renderer);
  for (size_t i = 0; i < 4; i++)
  {
    renderer->color[i] = 1.0;
  }
  renderer->renderMode = GL_TRIANGLES;
}

static void ccRenderData()
{
  glBindVertexArray(CC_MAIN_RENDERER.vao);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CC_MAIN_RENDERER.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               (GLsizeiptr)(CC_MAIN_RENDERER.numIndices * sizeof(uint32_t)),
               CC_MAIN_RENDERER.indices, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, CC_MAIN_RENDERER.vbo);
  glBufferData(GL_ARRAY_BUFFER,
               (GLsizeiptr)(CC_MAIN_RENDERER.numVertices * sizeof(float) * 3),
               CC_MAIN_RENDERER.vertices, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, CC_MAIN_RENDERER.cbo);
  glBufferData(GL_ARRAY_BUFFER,
               (GLsizeiptr)(CC_MAIN_RENDERER.numColors * sizeof(float) * 4),
               CC_MAIN_RENDERER.colors, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(CC_MAIN_RENDERER.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CC_MAIN_RENDERER.ibo);
  glDrawElements(CC_MAIN_RENDERER.renderMode,
                 (GLsizei)CC_MAIN_RENDERER.numIndices, GL_UNSIGNED_INT, 0);
}

// Sets the render mode of the main renderer.
static void ccSetRenderMode(GLenum mode)
{
  CC_MAIN_RENDERER.renderMode = mode;
}

//
// DRAWING
//

typedef struct ccGeometry
{
  float *vertices;
  float *colors;
  uint32_t *indices;
  uint32_t numVertices;
  uint32_t numIndices;
} ccGeometry;

// Clears the window screen with the given color.
// @param r,g,b,a Normalized values from 0-1.
static void ccClearWindow(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Sets the color the main renderer will use for subsequent draw calls.
// @param r,g,b,a Normalized values from 0-1.
static inline void ccSetColor(float r, float g, float b, float a)
{
  CC_MAIN_RENDERER.color[0] = r;
  CC_MAIN_RENDERER.color[1] = g;
  CC_MAIN_RENDERER.color[2] = b;
  CC_MAIN_RENDERER.color[3] = a;
}

// Queues a triangle to be drawn to the screen.
// @param x1,y1,z1 The first corner of the triangle.
// @param x2,y2,z2 The second corner of the triangle.
// @param x3,y3,z3 The third corner of the triangle.
static inline void ccDrawTriangle(float x1, float y1, float z1, float x2,
                                  float y2, float z2, float x3, float y3,
                                  float z3)
{
  size_t curVerticesSz = CC_MAIN_RENDERER.numVertices * sizeof(float) * 3;
  size_t newVerticesSz = curVerticesSz + sizeof(float) * 9;
  float *newVertices = (float *)malloc(newVerticesSz);

  float triangle[9] = {x1, y1, z1, x2, y2, z2, x3, y3, z3};
  memcpy(newVertices, CC_MAIN_RENDERER.vertices, curVerticesSz);
  memcpy(&newVertices[CC_MAIN_RENDERER.numVertices * 3], triangle,
         sizeof(float) * 9);

  free(CC_MAIN_RENDERER.vertices);
  CC_MAIN_RENDERER.vertices = newVertices;
  CC_MAIN_RENDERER.numVertices += 3;

  size_t curColorsSz = CC_MAIN_RENDERER.numColors * sizeof(float) * 4;
  size_t newColorsSz = curColorsSz + sizeof(float) * 12;
  float *newColors = (float *)malloc(newColorsSz);

  memcpy(newColors, CC_MAIN_RENDERER.colors, curColorsSz);
  memcpy(&newColors[CC_MAIN_RENDERER.numColors * 4], CC_MAIN_RENDERER.color,
         sizeof(float) * 4);
  memcpy(&newColors[CC_MAIN_RENDERER.numColors * 4 + 4], CC_MAIN_RENDERER.color,
         sizeof(float) * 4);
  memcpy(&newColors[CC_MAIN_RENDERER.numColors * 4 + 8], CC_MAIN_RENDERER.color,
         sizeof(float) * 4);
  free(CC_MAIN_RENDERER.colors);
  CC_MAIN_RENDERER.colors = newColors;
  CC_MAIN_RENDERER.numColors += 3;

  size_t curIndicesSz = CC_MAIN_RENDERER.numIndices * sizeof(uint32_t);
  size_t newIndicesSz = curIndicesSz + sizeof(uint32_t) * 3;
  uint32_t *newIndices = (uint32_t *)malloc(newIndicesSz);
  memcpy(newIndices, CC_MAIN_RENDERER.indices, curIndicesSz);
  uint32_t triangleIndices[3] = {0, 1, 2};
  for (size_t i = 0; i < 3; i++)
  {
    triangleIndices[i] += CC_MAIN_RENDERER.numVertices - 3;
  }
  memcpy(&newIndices[CC_MAIN_RENDERER.numIndices], triangleIndices,
         sizeof(uint32_t) * 3);
  free(CC_MAIN_RENDERER.indices);
  CC_MAIN_RENDERER.indices = newIndices;
  CC_MAIN_RENDERER.numIndices += 3;
}

// Queues a geometry for drawing to the screen.
static void ccDrawGeometry(ccGeometry *geom)
{
  size_t vertexSz = sizeof(float) * 3;
  size_t curVerticesSz = CC_MAIN_RENDERER.numVertices * vertexSz;
  size_t geomVerticesSz = geom->numVertices * vertexSz;
  size_t newVerticesSz = curVerticesSz + geomVerticesSz;
  float *newVertices = (float *)malloc(newVerticesSz);
  memcpy(newVertices, CC_MAIN_RENDERER.vertices, curVerticesSz);
  memcpy(&newVertices[CC_MAIN_RENDERER.numVertices * 3], geom->vertices,
         geomVerticesSz);
  free(CC_MAIN_RENDERER.vertices);
  CC_MAIN_RENDERER.vertices = newVertices;
  CC_MAIN_RENDERER.numVertices += geom->numVertices;

  size_t colorSz = sizeof(float) * 4;
  size_t curColorsSz = CC_MAIN_RENDERER.numColors * colorSz;
  size_t geomColorsSz = geom->numVertices * colorSz;
  size_t newColorsSz = curColorsSz + geomColorsSz;
  float *newColors = (float *)malloc(newColorsSz);
  memcpy(newColors, CC_MAIN_RENDERER.colors, curColorsSz);
  memcpy(&newColors[CC_MAIN_RENDERER.numColors * 4], geom->colors,
         geomColorsSz);
  free(CC_MAIN_RENDERER.colors);
  CC_MAIN_RENDERER.colors = newColors;
  CC_MAIN_RENDERER.numColors += CC_MAIN_RENDERER.numVertices;

  size_t curIndicesSz = CC_MAIN_RENDERER.numIndices * sizeof(uint32_t);
  size_t geomIndicesSz = geom->numIndices * sizeof(uint32_t);
  size_t newIndicesSz = curIndicesSz + geomIndicesSz;
  uint32_t *newIndices = (uint32_t *)malloc(newIndicesSz);
  memcpy(newIndices, CC_MAIN_RENDERER.indices, curIndicesSz);
  for (size_t i = 0; i < geom->numIndices; i++)
  {
    newIndices[i + CC_MAIN_RENDERER.numIndices] =
        geom->indices[i] + CC_MAIN_RENDERER.numVertices - geom->numIndices;
  }
  free(CC_MAIN_RENDERER.indices);
  CC_MAIN_RENDERER.indices = newIndices;
  CC_MAIN_RENDERER.numIndices += geom->numIndices;
}

//
// SHADERS
//

// The default projection matrix passed to the default shader program.
// Automatically changes when the GLFW callback `onFrameBufferSize` is called.
// By default an orthographic projection matrix with near set to `0.1` and far
// to `100.0`.
static mat4s CC_DEFAULT_PROJECTION_MATRIX;

// The default view matrix passed to the default shader program. By default a
// translation of (0,0,-1).
static mat4s CC_DEFAULT_VIEW_MATRIX;

// The default model matrix. By default an identity matrix.
static mat4s CC_DEFAULT_MODEL_MATRIX;

// The default vertex shader used by the default shader program.
static const char *CC_DEFAULT_VERTEX_SHADER =
    "#version 430 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aColor;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "out vec4 vColor;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   vColor = aColor;\n"
    "}\0";

// The default fragment shader used by the default shader program.
static const char *CC_DEFAULT_FRAGMENT_SHADER = "#version 430 core\n"
                                                "in vec4 vColor;\n"
                                                "out vec4 FragColor;\n"
                                                "void main()\n"
                                                "{\n"
                                                "   FragColor = vColor;\n"
                                                "}\n\0";

// @returns `true` if `shader` has compiled successfully. Otherwise `false`.
static bool ccShaderCompileSuccess(const GLuint shader)
{
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  return success == 1;
}

// @returns the info log to `shader` as a `const char *` owned by the caller.
static const char *ccGetShaderInfoLog(const GLuint shader)
{
  char *buffer = (char *)malloc(512);
  glGetShaderInfoLog(shader, 512, NULL, buffer);
  return buffer;
}

// @returns a `GLuint` representing the shader compiled, given the
// `shaderSource` string and the `shaderType`. This operation may fail.
static GLuint ccLoadShaderFromSource(const char *shaderSource,
                                     GLenum shaderType)
{
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  if (!ccShaderCompileSuccess(shader))
  {
    perror(ccGetShaderInfoLog(shader));
  }
  return shader;
}

// @returns a `GLuint` representing the shader compiled from `shaderPath` and
// given the `shaderType`. This operation may fail.
static inline GLuint ccLoadShader(const char *shaderPath, GLenum shaderType)
{
  const char *src = ccReadFile(shaderPath);
  GLuint shader = ccLoadShaderFromSource(src, shaderType);
  free((void *)src);
  return shader;
}

// Creates and returns the default shader program used by the main renderer.
static GLuint ccLoadDefaultShaderProgram()
{
  GLuint vs =
      ccLoadShaderFromSource(CC_DEFAULT_VERTEX_SHADER, GL_VERTEX_SHADER);
  GLuint fs =
      ccLoadShaderFromSource(CC_DEFAULT_FRAGMENT_SHADER, GL_FRAGMENT_SHADER);
  GLuint shader = glCreateProgram();
  glAttachShader(shader, vs);
  glAttachShader(shader, fs);
  glLinkProgram(shader);
  glDeleteShader(vs);
  glDeleteShader(fs);
  return shader;
}

#ifndef CC_NO_MAIN

//
// GLFW CALLBACKS
//

static void ccOnError(int error_code, const char *description)
{
  (void)error_code;
  perror(description);
}

static void ccOnFrameBufferSize(GLFWwindow *window, int width, int height)
{
  (void)window;
  glViewport(0, 0, width, height);
  CC_DEFAULT_PROJECTION_MATRIX =
      glms_ortho(0, (float)width, 0, (float)height, 0.1f, 100.0);
}

static void ccOnKey(GLFWwindow *window, int key, int scancode, int action,
                    int mods)
{
  (void)scancode;
  (void)mods;
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(window, 1);
  }
}

// Must be defined by the user. Called only once after all the internal setup
// has completed and directly before the draw loop is started.
void setup();

// Must be defined by the user. Called each frame in the application's main
// loop.
void loop();

int main()
{
  glfwSetErrorCallback(ccOnError);
  if (!glfwInit())
  {
    return -1;
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CC_GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CC_GL_VERSION_MINOR);
  CC_MAIN_WINDOW = glfwCreateWindow(
      CC_DEFAULT_WINDOW_WIDTH, CC_DEFAULT_WINDOW_HEIGHT, "GLFW", NULL, NULL);
  if (!CC_MAIN_WINDOW)
  {
    glfwTerminate();
    return -1;
  }
  glfwSetFramebufferSizeCallback(CC_MAIN_WINDOW, ccOnFrameBufferSize);
  glfwMakeContextCurrent(CC_MAIN_WINDOW);

  glfwSetKeyCallback(CC_MAIN_WINDOW, ccOnKey);
  glViewport(0, 0, CC_DEFAULT_WINDOW_WIDTH, CC_DEFAULT_WINDOW_HEIGHT);

  CC_DEFAULT_PROJECTION_MATRIX =
      glms_ortho(0, (float)CC_DEFAULT_WINDOW_WIDTH, 0,
                 (float)CC_DEFAULT_WINDOW_HEIGHT, 0.1f, 100.0);
  CC_DEFAULT_VIEW_MATRIX = glms_translate_make((vec3s){{0, 0, -1}});
  CC_DEFAULT_MODEL_MATRIX = glms_mat4_identity();

  ccCreateMainRenderer(&CC_MAIN_RENDERER);

  glUseProgram(CC_MAIN_RENDERER.shaderProgram);
  GLint umodel = glGetUniformLocation(CC_MAIN_RENDERER.shaderProgram, "model");
  GLint uview = glGetUniformLocation(CC_MAIN_RENDERER.shaderProgram, "view");
  GLint uprojection =
      glGetUniformLocation(CC_MAIN_RENDERER.shaderProgram, "projection");

  setup();
  double prevTime = glfwGetTime();
  while (!glfwWindowShouldClose(CC_MAIN_WINDOW))
  {
    glUniformMatrix4fv(umodel, 1, GL_FALSE,
                       (float *)&CC_DEFAULT_MODEL_MATRIX.raw);
    glUniformMatrix4fv(uview, 1, GL_FALSE,
                       (float *)&CC_DEFAULT_VIEW_MATRIX.raw);
    glUniformMatrix4fv(uprojection, 1, GL_FALSE,
                       (float *)&CC_DEFAULT_PROJECTION_MATRIX.raw);
    loop();
    ccRenderData();
    ccResetRendererData(&CC_MAIN_RENDERER);
    glfwSwapBuffers(CC_MAIN_WINDOW);
    glfwPollEvents();
    CC_CURRENT_FRAMENUM++;
    double frameTime = glfwGetTime();
    CC_CURRENT_FPS = 1.0 / (frameTime - prevTime);
    prevTime = frameTime;
  };

  glfwTerminate();
  return 0;
}
#endif

#endif // CC_HEADER
