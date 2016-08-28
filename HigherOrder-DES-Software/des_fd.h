#include "common.h"

typedef unsigned long conf[2];

void refreshConf(conf a[], int n);
void xorConfig(conf a, conf b);
void randConf(conf a);
void divideConf(conf a, conf shares[], int n);
void adjustConf(conf a, byte xi);
void sbox_fd(int ii,byte *a,int n);
void sbox_fd_word(int ii,byte *a,int n);
