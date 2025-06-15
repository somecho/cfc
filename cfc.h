// C Headers
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Dependency Headers
#include <cglm/struct.h>
#include <epoxy/gl.h>
#include <epoxy/glx.h>

#include <GLFW/glfw3.h>

#ifndef CC_HEADER
#define CC_HEADER

typedef struct ccRenderer {
  GLuint vao;
  GLuint vbo; // float x y z
  GLuint cbo; // float r g b a
  GLuint ibo; // unsigned int
  GLuint shaderProgram;
  uint32_t *indices;
  float *vertices;
  float *colors;
  size_t numIndices;
  size_t numVertices;
  size_t numColors;
  float color[4];

} ccRenderer;

// GLOBALS

// Using GL 4.3 for compute shader support. If compute shader is not needed and
// a lower version is needed, redefine.
#ifndef CC_GL_VERSION_MAJOR
#define CC_GL_VERSION_MAJOR 4
#endif
#ifndef CC_GL_VERSION_MINOR
#define CC_GL_VERSION_MINOR 3
#endif

static const int CC_DEFAULT_WINDOW_WIDTH = 1200;
static const int CC_DEFAULT_WINDOW_HEIGHT = 800;
static int CC_CURRENT_WINDOW_WIDTH = CC_DEFAULT_WINDOW_WIDTH;
static int CC_CURRENT_WINDOW_HEIGHT = CC_DEFAULT_WINDOW_HEIGHT;
static GLFWwindow *CC_MAIN_WINDOW;

static mat4s CC_DEFAULT_PROJECTION_MATRIX;
static mat4s CC_DEFAULT_VIEW_MATRIX;
static mat4s CC_DEFAULT_MODEL_MATRIX;

const char *CC_DEFAULT_VERTEX_SHADER =
    "#version 430 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec4 aCol;\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "out vec4 vCol;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
    "   vCol = aCol;\n"
    "}\0";

const char *CC_DEFAULT_FRAGMENT_SHADER = "#version 430 core\n"
                                         "in vec4 vCol;\n"
                                         "out vec4 FragColor;\n"
                                         "void main()\n"
                                         "{\n"
                                         "   FragColor = vCol;\n"
                                         "}\n\0";

static ccRenderer CC_MAIN_RENDERER;

// DEFAULT GLFW CALLBACKS

void _ccOnError(int error_code, const char *description) {
  perror(description);
}

void ccOnFrameBufferSize(GLFWwindow *window, int width, int height) {
  glViewport(0, 0, width, height);
  CC_DEFAULT_PROJECTION_MATRIX = glms_ortho(0, width, 0, height, 0.1, 100.0);
}

void ccOnKey(GLFWwindow *window, int key, int scancode, int action, int mods) {
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
    glfwSetWindowShouldClose(window, 1);
  }
}

// CC API

// Reads the contents of a file and returns it as a null-terminated string.
// The caller is responsible for freeing the returned string.
const char *ccReadFile(const char *filePath) {
  FILE *file = fopen(filePath, "rb");
  if (!file) {
    perror("Failed to open file");
    return NULL;
  }

  // Seek to end to determine file size
  fseek(file, 0, SEEK_END);
  long fileSize = ftell(file);
  rewind(file);

  // Allocate memory (+1 for null terminator)
  char *buffer = (char *)malloc(fileSize + 1);
  if (!buffer) {
    perror("Failed to allocate buffer");
    fclose(file);
    return NULL;
  }

  // Read the file into the buffer
  size_t bytesRead = fread(buffer, 1, fileSize, file);
  buffer[bytesRead] = '\0'; // Null-terminate

  fclose(file);
  return buffer;
}

void ccSetWindowSize(int width, int height) {
  glfwSetWindowSize(CC_MAIN_WINDOW, width, height);
  // TODO: recenter window
}

int ccGetWidth() { return CC_CURRENT_WINDOW_WIDTH; }
int ccGetHeight() { return CC_CURRENT_WINDOW_HEIGHT; }

void ccClearWindow(float r, float g, float b, float a) {
  glClearColor(r, g, b, a);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void ccClearWindowU8(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
  glClearColor((float)r / 255.f, (float)g / 255.f, (float)b / 255.f,
               (float)a / 255.f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Checks whether a shader has compiled successfully.
bool ccShaderCompileSuccess(const GLuint shader) {
  int success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  return success == 1;
}

// Calls glGetShaderInfoLog and returns result
const char *ccGetShaderInfoLog(const GLuint shader) {
  char *buffer = (char *)malloc(512);
  glGetShaderInfoLog(shader, 512, NULL, buffer);
  return buffer;
}

GLuint ccLoadShaderFromSource(const char *shaderSource, GLenum shaderType) {
  GLuint shader = glCreateShader(shaderType);
  glShaderSource(shader, 1, &shaderSource, NULL);
  glCompileShader(shader);
  if (!ccShaderCompileSuccess(shader)) {
    perror(ccGetShaderInfoLog(shader));
  }
  return shader;
}

// Loads, compiles and returns a shader
GLuint ccLoadShader(const char *shaderPath, GLenum shaderType) {
  const char *src = ccReadFile(shaderPath);
  GLuint shader = ccLoadShaderFromSource(src, shaderType);
  free((void *)src);
  return shader;
}

GLuint ccLoadDefaultShaderProgram() {
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

void ccResetRendererData(ccRenderer *renderer) {
  renderer->colors = NULL;
  renderer->vertices = NULL;
  renderer->indices = NULL;
  renderer->numColors = 0;
  renderer->numIndices = 0;
  renderer->numVertices = 0;
}

void ccCreateMainRenderer(ccRenderer *renderer) {
  glGenVertexArrays(1, &renderer->vao);
  renderer->shaderProgram = ccLoadDefaultShaderProgram();
  glGenBuffers(1, &renderer->vbo);
  glGenBuffers(1, &renderer->cbo);
  glGenBuffers(1, &renderer->ibo);
  ccResetRendererData(renderer);
  for (size_t i = 0; i < 4; i++) {
    renderer->color[i] = 1.0;
  }
}

void ccSetRenderColor(float r, float g, float b, float a) {
  CC_MAIN_RENDERER.color[0] = r;
  CC_MAIN_RENDERER.color[1] = g;
  CC_MAIN_RENDERER.color[2] = b;
  CC_MAIN_RENDERER.color[3] = a;
}

void ccDrawTriangle(float x1, float y1, float z1, float x2, float y2, float z2,
                    float x3, float y3, float z3) {
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
  for (size_t i = 0; i < 3; i++) {
    triangleIndices[i] += CC_MAIN_RENDERER.numVertices - 3;
  }
  memcpy(&newIndices[CC_MAIN_RENDERER.numIndices], triangleIndices,
         sizeof(uint32_t) * 3);
  free(CC_MAIN_RENDERER.indices);
  CC_MAIN_RENDERER.indices = newIndices;
  CC_MAIN_RENDERER.numIndices += 3;
}

void ccRenderData() {
  glBindVertexArray(CC_MAIN_RENDERER.vao);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CC_MAIN_RENDERER.ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               CC_MAIN_RENDERER.numIndices * sizeof(uint32_t),
               CC_MAIN_RENDERER.indices, GL_DYNAMIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, CC_MAIN_RENDERER.vbo);
  glBufferData(GL_ARRAY_BUFFER,
               CC_MAIN_RENDERER.numVertices * sizeof(float) * 3,
               CC_MAIN_RENDERER.vertices, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(0);

  glBindBuffer(GL_ARRAY_BUFFER, CC_MAIN_RENDERER.cbo);
  glBufferData(GL_ARRAY_BUFFER, CC_MAIN_RENDERER.numColors * sizeof(float) * 4,
               CC_MAIN_RENDERER.colors, GL_DYNAMIC_DRAW);
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glEnableVertexAttribArray(1);

  glBindVertexArray(CC_MAIN_RENDERER.vao);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CC_MAIN_RENDERER.ibo);
  glDrawElements(GL_TRIANGLES, CC_MAIN_RENDERER.numIndices, GL_UNSIGNED_INT, 0);
}

#ifndef CC_NO_MAIN

// USER IMPLEMENTED FUNCTIONS

void setup();
void loop();

int main() {
  // GLFW SETUP
  glfwSetErrorCallback(_ccOnError);

  if (!glfwInit()) {
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, CC_GL_VERSION_MAJOR);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, CC_GL_VERSION_MINOR);

  CC_MAIN_WINDOW = glfwCreateWindow(
      CC_DEFAULT_WINDOW_WIDTH, CC_DEFAULT_WINDOW_HEIGHT, "GLFW", NULL, NULL);
  if (!CC_MAIN_WINDOW) {
    glfwTerminate();
    return -1;
  }
  glfwMakeContextCurrent(CC_MAIN_WINDOW);

  glfwSetFramebufferSizeCallback(CC_MAIN_WINDOW, ccOnFrameBufferSize);
  glfwSetKeyCallback(CC_MAIN_WINDOW, ccOnKey);
  glViewport(0, 0, CC_DEFAULT_WINDOW_WIDTH, CC_DEFAULT_WINDOW_HEIGHT);

  CC_DEFAULT_PROJECTION_MATRIX = glms_ortho(
      0, CC_DEFAULT_WINDOW_WIDTH, 0, CC_DEFAULT_WINDOW_HEIGHT, 0.1, 100.0);
  CC_DEFAULT_VIEW_MATRIX = glms_translate_make((vec3s){{0, 0, -1}});
  CC_DEFAULT_MODEL_MATRIX = glms_mat4_identity();

  ccCreateMainRenderer(&CC_MAIN_RENDERER);

  glUseProgram(CC_MAIN_RENDERER.shaderProgram);
  GLint umodel = glGetUniformLocation(CC_MAIN_RENDERER.shaderProgram, "model");
  GLint uview = glGetUniformLocation(CC_MAIN_RENDERER.shaderProgram, "view");
  GLint uprojection =
      glGetUniformLocation(CC_MAIN_RENDERER.shaderProgram, "projection");

  setup();
  while (!glfwWindowShouldClose(CC_MAIN_WINDOW)) {
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
  };

  glfwTerminate();
  return 0;
}
#endif

#endif // CC_HEADER
