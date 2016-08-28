#include "aes.h"

typedef bool conf[256];



void subbyte_fd(byte *a, int n);
void refreshConf(conf a[], int n);
void xorConfig(conf a, conf b);
void randConf(conf a);
void divideConf(conf a, conf shares[], int n);
void adjustConf(conf a, byte xi);
void subbyte_fd_word(byte *a, int n);