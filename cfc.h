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
// GL Helpers
//

// Binds `buffer` to `target` and writes `data` to it.
static inline void ccWriteBuffer(GLenum target, GLuint buffer, GLsizeiptr size,
                                 const void *data, GLenum usage)
{
  glBindBuffer(target, buffer);
  glBufferData(target, size, data, usage);
}

// Shorthand for calling `glVertexAttribPointer` and
// `glEnableVertexAttribArray`.
static inline void ccVertexAttribute(GLuint index, GLint size, GLenum type,
                                     GLboolean normalized, GLsizei stride,
                                     const void *pointer)
{
  glVertexAttribPointer(index, size, type, normalized, stride, pointer);
  glEnableVertexAttribArray(index);
}

//
// DRAWING
//

#define CC_BLACK 0, 0, 0, 1
#define CC_BLUE 0, 0, 1, 1
#define CC_CYAN 0, 1, 1, 1
#define CC_GREEN 0, 1, 0, 1
#define CC_MAGENTA 1, 0, 1, 1
#define CC_RED 1, 0, 0, 1
#define CC_WHITE 1, 1, 1, 1
#define CC_YELLOW 1, 1, 0, 1

typedef struct ccGeometry
{
  // XYZ
  float *vertices;
  // RGBA
  float *colors;
  uint32_t *indices;
  uint32_t numVertices;
  uint32_t numIndices;
} ccGeometry;

static GLuint CC_MAIN_VAO;
static GLuint CC_MAIN_VBO;
static GLuint CC_MAIN_CBO;
static GLuint CC_MAIN_IBO;
static GLuint CC_CURRENT_SHADER_PROGRAM;
static float CC_CURRENT_RENDER_COLOR[4];

// Clears the window screen with the given color.
// @param r,g,b,a Normalized values from 0-1.
static inline void ccClearWindow(float r, float g, float b, float a)
{
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Sets the color the main renderer will use for subsequent draw calls.
// @param r,g,b,a Normalized values from 0-1.
static inline void ccSetColor(float r, float g, float b, float a)
{
  CC_CURRENT_RENDER_COLOR[0] = r;
  CC_CURRENT_RENDER_COLOR[1] = g;
  CC_CURRENT_RENDER_COLOR[2] = b;
  CC_CURRENT_RENDER_COLOR[3] = a;
}

// Draws a triangle to the screen.
// @param x1,y1,z1 The first corner of the triangle.
// @param x2,y2,z2 The second corner of the triangle.
// @param x3,y3,z3 The third corner of the triangle.
static inline void ccDrawTriangle(float x1, float y1, float z1, float x2,
                                  float y2, float z2, float x3, float y3,
                                  float z3)
{
  glBindVertexArray(CC_MAIN_VAO);

  float vertices[] = {x1, y1, z1, x2, y2, z2, x3, y3, z3};
  ccWriteBuffer(GL_ARRAY_BUFFER, CC_MAIN_VBO, sizeof(vertices), vertices,
                GL_DYNAMIC_DRAW);
  ccVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

  float colors[12];
  for (size_t i = 0; i < 3; i++)
  {
    memcpy(&colors[i * 4], CC_CURRENT_RENDER_COLOR, sizeof(float) * 4);
  }
  ccWriteBuffer(GL_ARRAY_BUFFER, CC_MAIN_CBO, sizeof(colors), colors,
                GL_DYNAMIC_DRAW);
  ccVertexAttribute(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);

  glDrawArrays(GL_TRIANGLES, 0, 3);
}

static inline void ccDrawGeometry(ccGeometry *geom, GLenum mode)
{
  glBindVertexArray(CC_MAIN_VAO);

  ccWriteBuffer(GL_ELEMENT_ARRAY_BUFFER, CC_MAIN_IBO,
                geom->numIndices * sizeof(uint32_t), geom->indices,
                GL_DYNAMIC_DRAW);

  ccWriteBuffer(GL_ARRAY_BUFFER, CC_MAIN_VBO,
                geom->numVertices * 3 * sizeof(float), geom->vertices,
                GL_DYNAMIC_DRAW);
  ccVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

  ccWriteBuffer(GL_ARRAY_BUFFER, CC_MAIN_IBO,
                geom->numVertices * 4 * sizeof(float), geom->colors,
                GL_DYNAMIC_DRAW);
  ccVertexAttribute(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CC_MAIN_IBO);
  glDrawElements(mode, geom->numIndices, GL_UNSIGNED_INT, 0);
}

static inline void ccDrawGeometryUnindexed(ccGeometry *geom, GLenum mode)
{
  glBindVertexArray(CC_MAIN_VAO);

  ccWriteBuffer(GL_ARRAY_BUFFER, CC_MAIN_VBO,
                geom->numVertices * 3 * sizeof(float), geom->vertices,
                GL_DYNAMIC_DRAW);
  ccVertexAttribute(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), NULL);

  ccWriteBuffer(GL_ARRAY_BUFFER, CC_MAIN_CBO,
                geom->numVertices * 4 * sizeof(float), geom->colors,
                GL_DYNAMIC_DRAW);
  ccVertexAttribute(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), NULL);
  glDrawArrays(mode, 0, geom->numVertices);
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

  printf("Initializing GLFW\n");
  if (!glfwInit())
  {
    return -1;
  }

  printf("GL Version: %i.%i\n", CC_GL_VERSION_MAJOR, CC_GL_VERSION_MINOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CC_GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CC_GL_VERSION_MINOR);
  printf("Creating window\n");
  CC_MAIN_WINDOW = glfwCreateWindow(CC_CURRENT_WINDOW_WIDTH,
                                    CC_CURRENT_WINDOW_HEIGHT, "", NULL, NULL);
  if (!CC_MAIN_WINDOW)
  {
    glfwTerminate();
    return -1;
  }
  glfwSetFramebufferSizeCallback(CC_MAIN_WINDOW, ccOnFrameBufferSize);
  printf("Obtaining context\n");
  glfwMakeContextCurrent(CC_MAIN_WINDOW);

  glfwSetKeyCallback(CC_MAIN_WINDOW, ccOnKey);
  glViewport(0, 0, CC_CURRENT_WINDOW_WIDTH, CC_CURRENT_WINDOW_HEIGHT);

  glGenVertexArrays(1, &CC_MAIN_VAO);
  glGenBuffers(1, &CC_MAIN_VBO);
  glGenBuffers(1, &CC_MAIN_CBO);
  glGenBuffers(1, &CC_MAIN_IBO);
  CC_CURRENT_SHADER_PROGRAM = ccLoadDefaultShaderProgram();
  glUseProgram(CC_CURRENT_SHADER_PROGRAM);

  GLint umodel = glGetUniformLocation(CC_CURRENT_SHADER_PROGRAM, "model");
  GLint uview = glGetUniformLocation(CC_CURRENT_SHADER_PROGRAM, "view");
  GLint uprojection =
      glGetUniformLocation(CC_CURRENT_SHADER_PROGRAM, "projection");

  CC_DEFAULT_PROJECTION_MATRIX = glms_ortho(
      0, CC_CURRENT_WINDOW_WIDTH, 0, CC_CURRENT_WINDOW_HEIGHT, 0.1f, 100.0);
  CC_DEFAULT_VIEW_MATRIX = glms_translate_make((vec3s){{0, 0, -1}});
  CC_DEFAULT_MODEL_MATRIX = glms_mat4_identity();

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
