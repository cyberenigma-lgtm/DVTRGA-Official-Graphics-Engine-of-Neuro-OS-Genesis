
#define WINGDIAPI __declspec(dllimport)
#define APIENTRY __stdcall
#define CALLBACK __stdcall

/* Typedefs that gl.h might need if windows.h is missing */
/* Actually gl.h typically defines its own GL types. */
/* But it might need wchar_t or something if it includes internal stuff? */
/* Let's try minimal first. */

#include <GL/gl.h>
#include <stdio.h>

int main() {
  printf("Manual GL Defines worked.\n");
  return 0;
}
