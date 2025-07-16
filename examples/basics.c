//
// Example: basics.c
// Description:
// This example shows how to use Soya to create a window.
//

#include <soya/soya.h>

//
// To begin using Soya, there are 3 functions you have to implement. All 3
// functions take an `syApp` pointer called `app`. This gives you access to data
// related to the app that you might want to use in the different functions.
//
//
// The first function is the `configure` function. This is for configuring the
// window. You can leave it blank to use the default configuration. To know what
// the defaults are, check out the `syAppPreconfigure` function in `soyalib.h`.
//
void configure(syApp *app) {
  //
  // Setting window width and height
  //
  app->width = 800;
  app->height = 600;
  puts("Configure function called");
}

//
// The second function is the `setup` function. This function is run once, after
// `configure` has been called. If there's anything you need to do only once,
// this is the place to do it.
//
// The difference between configure and setup is that at the setup stage, the
// app's window has been created and the OpenGL context has been set to it. This
// means that raw OpenGL functions can be used in `setup`.
//
void setup(syApp *app) {
  //
  // This function is run once.
  //
  puts("Setup function called");
  printf("Winow size: %ix%i:", app->width, app->height);
}

//
// Finally, we have the `loop` function. It is run every frame.
//
void loop(syApp *app) {
  syClear(SY_BLUE);
  printf("Frame number: %zu @ %.1f fps\n", app->frameNum, app->fps);
}
