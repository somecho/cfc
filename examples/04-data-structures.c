//
// Example: 04-data-structures.c
// Description:
// Soya has some simple data structures to help with some tasks you might need
// to do often. Data structures shown here:
// - syArrayF32
//

#include "../soyalib.h"

// 32 bit float array
syArrayF32 a;

void configure(syApp *app)
{
  (void)app;
}

void setup(syApp *app)
{
  (void)app;
  syArrayInit(a);    // Arrays must always be initialized.
  syArrayPush(a, 1); // Push puts a value at the next available location.
  syArrayPush(a, 2); // It reallocates memory if it needs more.
  syArrayPush(a, 3);
  syArrayPush(a, 4);
  syArrayPrint(a); // Helper for print debugging

  // Push and Destroy are macros to enable different kinds of arrays
  syArrayPush(a, 4);
  syArrayDestroy(a); // Deallocates memory used
}

void loop(syApp *app)
{
  (void)app;
}
