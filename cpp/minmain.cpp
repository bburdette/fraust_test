#include <stdio.h>
#include "minimal.h"
#include <math.h>

int main(int argc, char* argv[]) {

  fraust_init(44100);

  float *mahbuf = new float[500];
  float *outbuf = new float[500];

  float f = 0.0;
  for (int i = 0; i < 500; ++i)
  {
    f = f + 0.01;
    mahbuf[i] = sin(f);
  }

  int count = 500;

  fraust_compute(count, &mahbuf, &outbuf);
  // compute(5, pmeh, pout);

  printf("blah!");

  for (int i = 0; i < count; ++i)
  {
    printf("%f\n", outbuf[i]);
  }

  return 0;
}
