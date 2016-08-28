#include "aes.h"

typedef unsigned long conf[8];

void subbyte_fd_1(byte *a, int n);
void refreshConf(conf a[], int n);
void xorConfig(conf a, conf b);
void randConf(conf a);
void divideConf(conf a, conf shares[], int n);
void adjustConf(conf a, byte xi);
void subbyte_fd_word(byte *a, int n);