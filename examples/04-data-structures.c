//
// Example: 04-data-structures.c
// Description:
// Soya has some simple data structures to help with some tasks you might need
// to do often. Data structures shown here:
// - Generic Array
//

#define SY_NO_CONFIGURE
#include "../soyalib.h"

//
// This macro declares an array struct. It takes a name and a type.
//
SY_DEFARRAY(macroarray,
            float) // Declare an array named `macroarray` of type `float`.

void setup(syApp *app)
{
  (void)app;
  // Arrays need to be initialized before usage
  syArrayInit(macroarray, float);
}

void loop(syApp *app)
{
  // Push a value into the array every frame
  syArrayPush(macroarray, app->frameNum);

  if (app->frameNum == 12)
  {
    // Helper function to view array data
    syArrayPrint(macroarray);

    // Free resources used by the array
    syArrayDestroy(macroarray);

    glfwSetWindowShouldClose(app->window, true);
  }
}
