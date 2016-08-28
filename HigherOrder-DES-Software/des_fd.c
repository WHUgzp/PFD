#include "des_fd.h"
#include "des.h"

#include "share.h"

#include <string.h>

#define K 64

typedef unsigned int word;

conf configSeq[32] = {{0x1e2d81f6, 0x6097de89}, {0xf168a61d, 0x4b5ac4b3}, {0x596aa695, 0xc639b1d2}, {0xe71992c3, 0x583eed90}, {0x129d6ed2, 0x4b69b325}, {0x32c53c3e, 0x9a56c5a9}, {0x65969e92, 0x9e69712c}, {0xd43e3986, 0xb38146f9}, 
{0x94f8f906, 0x492b97e4}, {0xc3f92946, 0x2ed1562e}, {0x92ed439c, 0x30699d6e}, {0x8e733496, 0xf1688b35}, {0xd4ba0f49, 0x639cd0e3}, {0x5b6849b6, 0x3c87d34a}, {0x2f81d279, 0x582fa956}, {0x30fc8f13, 0xe51a68e5}, 
{0xbd641e92, 0xd8e1e30d}, {0x97c362c9, 0x683c9f16}, {0x529e2f64, 0xac61569b}, {0xe970532e, 0x318e96d3}, {0x69d66689, 0x8539da96}, {0x965967a8, 0x6b1c9963}, {0xc3c26799, 0x669b3961}, {0x96a5569a, 0x8d69729c}, 
{0x986e6761, 0x5e92b961}, {0x9a65c396, 0x76986987}, {0xcb0939d6, 0x96b49669}, {0x7c1a09b7, 0x98e7c349}, {0x712c879e, 0x56b3b942}, {0xda962593, 0xa96d522d}, {0x669b6934, 0x1f86e349}, {0x9d68e31c, 0x24bd5e83}}; 


void sbox_fd(int ii,byte *a,int n) // 4+n+4
{	
	conf confShares[20]; // n*64 bits = n*8 bytes
	int i, j, k, bits; //4 bytes
	byte b[20] = { 0 }; //n bytes

	// Memory:  10*n+24  bytes

	for (bits = 0; bits < 4; bits++)
	{
		divideConf(configSeq[8 * bits + ii], confShares, n);

		for (i = 0; i < n - 1; i++)
		{
			for (j = 0; j < n; j++)
			{
				adjustConf(confShares[j], a[i]);
			}
			refreshConf(confShares, n);
		}

		for (k = 0; k < n; k++)
		{
			b[k] += ((confShares[k][a[i] >> 5] >> (31 - a[i] & 0x1f)) & 1) << bits;
		}
	}
	for (i = 0; i < n; i++)
	{
		a[i] = b[i];
	}
	
}


void refreshConf(conf a[], int n)
{
	int i;//4 bytes
	conf tmp; //8 bytes

	for (i = 1; i < n; i++)
	{
		randConf(tmp); //rand();
		xorConfig(a[0], tmp);
		xorConfig(a[i], tmp);
	}
}

void xorConfig(conf a, conf b)
{
	int i;

	for (i = 0; i < 2; i++)
	{
		a[i] = a[i] ^ b[i];
	}
}

void randConf(conf a) {
	int i;

	for (i = 0; i < 2; i++)
	{
		a[i] = xorshf96();
	}
}

void divideConf(conf a, conf shares[], int n)
{
	int i;

	xorConfig(shares[0], shares[0]);
	xorConfig(shares[0], a);
	
	for (i = 1; i < n; i++)
	{
		xorConfig(shares[i], shares[i]);
	}
	
	/*
	for (i = 1; i < n; i++)
	{
		randConf(shares[i]);
		xorConfig(shares[0], shares[i]);
	}
	*/	
}

void adjustConf(conf a, byte xi)
{
	int i;
	conf tmp = {0};

	for (i = 0; i < K/2; i++)
	{
		tmp[0] += (((a[(i ^ xi) >> 5] >> (31 - (i ^ xi) & 0x1f)) & 1) << (31 - i));
	}
	for (;i < K; i++)
	{
		tmp[1] += (((a[(i ^ xi) >> 5] >> (31 - (i ^ xi) & 0x1f)) & 1) << (63 - i));
	}
	for (i = 0; i < 2; i++)
	{
		a[i] = tmp[i];
	}
}

/*
void refreshword(word a[],int n)
{
  int i;
  word tmp;
  for(i=1;i<n;i++)
  {
    tmp=xorshf96(); //rand();
    a[0]=a[0] ^ tmp;
    a[i]=a[i] ^ tmp;
  }
}

void sbox_fd_word(int ii,byte *a,int n)
{
  int w=2*sizeof(word); // number of nibbles to store in a word
  word T[K/w][n];   // n*64 bytes
  word Tp[K/w][n];  // n*64 bytes
  int i,k,k2,j;     // 16 bytes
  word r;           // 4 bytes
  word b[n];        // 4*n bytes (for 32-bit registers)
  byte x;           // 1 byte
  byte c[n];        // n bytes

  // Memory: 133*x+21

  byte *S=sbox+ii*64;

  for(k=0;k<K/w;k++)
  {
    r=0;
    for(j=w-1;j>=0;j--)
    {
      r=r << 4;
      r^=S[k*w+j];
    }
    T[k][0]=r;
    for(i=1;i<n;i++)
      T[k][i]=0;
  }

  for(i=0;i<(n-1);i++)
  {
    k2=(a[i] & 63)/w;
    for(k=0;k<K/w;k++)
      memcpy(Tp[k],T[k ^ k2],n*sizeof(word));
    for(k=0;k<K/w;k++)
    {
      memcpy(T[k],Tp[k],n*sizeof(word));
      refreshword(T[k],n);
    }
  }

  memcpy(b,T[(a[n-1] & 63)/w],n*sizeof(word));

  byte Ts[w][n];
  byte Tsp[w][n];

  for(k=0;k<w;k++)
    for(i=0;i<n;i++)
      Ts[k][i]=(b[i] >> (k*4)) & 15;
  
  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<w;k++)
      memcpy(Tsp[k],Ts[k ^ (a[i] & (w-1))],n);
    for(k=0;k<w;k++)
    {
      memcpy(Ts[k],Tsp[k],n);
      refresh(Ts[k],n);
    }
  }
  memcpy(a,Ts[a[n-1] & (w-1)],n);
  refresh(a,n);
}
*/
