#include "aes_htable.h"
#include "share.h"

#include <string.h>

#define K 256

typedef unsigned int word;

void subbyte_htable(byte *a,int n)
{
  byte T[K][20];
  byte Tp[K][20];
  byte b[20];
  int i,j,k;
 
  for(k=0;k<K;k++)
    share(sbox[k],T[k],n);

  for(i=0;i<(n-1);i++)
  {
    for(k=0;k<K;k++)
      for(j=0;j<n;j++)
	Tp[k][j]=T[k ^ a[i]][j];

    for(k=0;k<K;k++)
    {
      for(j=0;j<n;j++) 
	T[k][j]=Tp[k][j];
      refresh(T[k],n);
    }
  }
  
  for(j=0;j<n;j++)
    b[j]=T[a[n-1]][j];

  refresh(b,n);
  for(j=0;j<n;j++) a[j]=b[j];
}

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

void subbyte_htable_word(byte *a,int n)  // n+4 bytes
{
  int w=sizeof(word); // number of bytes to store in a word
  word T[K][20];  // n*256 bytes
  word Tp[K][20]; // n*256 bytes
  int i,k,k2,j;    // 16 bytes
  word r;          // 4 bytes
  word b[20];       // 4*n bytes (for 32-bit registers)
  byte x;          // 1 bytes
  byte c[20];       // n bytes

  // Memory:  518*n+25

  for(k=0;k<K/w;k++)
  {
    r=0;
    for(j=w-1;j>=0;j--)
    {
      r=r << 8;
      r^=sbox[k*w+j];
    }
    T[k][0]=r;
    for(i=1;i<n;i++)
      T[k][i]=0;
  }

  for(i=0;i<(n-1);i++)
  {
    k2=a[i]/w;
    for(k=0;k<K/w;k++)
      memcpy(Tp[k],T[k ^ k2],n*sizeof(word));
    for(k=0;k<K/w;k++)
    {
      memcpy(T[k],Tp[k],n*sizeof(word));
      refreshword(T[k],n);
    }
  }

  memcpy(b,T[a[n-1]/w],n*sizeof(word));
  refreshword(b,n);
  
  // not counted above because the tables T and Tp can be discarded
  byte Ts[8][20];   // 4*n bytes
  byte Tsp[8][20];  // 4*n bytes

  for(k=0;k<w;k++)
    for(i=0;i<n;i++)
      Ts[k][i]=b[i] >> (k*8);
  
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
