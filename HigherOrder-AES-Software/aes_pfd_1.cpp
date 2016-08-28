#include "aes_fd_1.h"
#include "share.h"

#include <string.h>

#define K 256

typedef unsigned int word;

//config[0][0] = conf[0] || conf[1] || ... || conf[31]  32 bits total
conf configSeq[8] = { { 0xb7763690, 0xe67b728d, 0xc85ed351, 0x5fd2d516, 0xad576003, 0x084dbd08, 0x205e24f6, 0x9cb578f2 },
			{ 0xbe3fca32, 0xbe0075de, 0x5ef681e9, 0x7a325867, 0x2d612ccf, 0x74d0a256, 0x951b5729, 0x02e90e13 },
			{ 0x632d12dc, 0xdfe1c685, 0x24231425, 0x459615c4, 0xdffc30dc, 0x0726beea, 0x3f74736b, 0x036d9c35 },
			{ 0x561abc98, 0x45040908, 0xbe4a15ff, 0x745716a4, 0xd836356f, 0x39215b97, 0xd8df4913, 0x6edbb972 },
			{ 0x788d1d4a, 0xee9f0d43, 0x1af0c732, 0x925e8fef, 0x2d37123d, 0x614d2464, 0xca74e273, 0x75c5084f },
			{ 0xfebb19b6, 0xdefa201f, 0x255e1eb0, 0x725543d6, 0x10798c60, 0xe233b1be, 0xf6a4f2d0, 0xc8124d2a },
			{ 0xff2dbf43, 0x433c5019, 0x06a4d5cf, 0xdcd8a127, 0x9aadb0cd, 0x89d3d6fc, 0x41e89aa4, 0xcc1d0784 },
			{ 0x090ee9fd, 0xe374553a, 0x41355661, 0xf1435de7, 0x94c05369, 0x80eecd32, 0x87c7221d, 0xe7b9ec4a } };

void subbyte_fd_1(byte *a, int n)// n+4 bytes
{
	conf confShares[20]; // n*256 bits = n*32 bytes
	int i, j, k, bits; //4 bytes
	byte b[20] = { 0 }; //n bytes

	// Memory:  34*n+44  bytes

	for (bits = 0; bits < 8; bits++)
	{
		divideConf(configSeq[bits], confShares, n);

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
			//b[k] += ((confShares[k][a[i] / 32] >> (31 - a[i] % 32)) & 1) << bits;
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
	conf tmp; //32 bytes

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

	for (i = 0; i < 8; i++)
	{
		a[i] = a[i] ^ b[i];
	}
}

void randConf(conf a) {
	int i;

	for (i = 0; i < 8; i++)
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

	for (i = 0; i < K; i++)
	{
		//tmp[i / 32] += (((a[(i ^ xi) / 32] >> (31 - (i ^ xi) % 32)) & 1) << (31 - i % 32));
		tmp[i >> 5] += (((a[(i ^ xi) >> 5] >> (31 - (i ^ xi) & 0x1f)) & 1) << (31 - i & 0x1f));

	}
	for (i = 0; i < 8; i++)
	{
		a[i] = tmp[i];
	}
}

/*
void subbyte_fd_word(byte *a, int n)  // n+4 bytes
{
int w = sizeof(word); // number of bytes to store in a word
word T[K][20];  // n*256 bytes
word Tp[K][20]; // n*256 bytes
int i, k, k2, j;    // 16 bytes
word r;          // 4 bytes
word b[20];       // 4*n bytes (for 32-bit registers)
byte x;          // 1 bytes
byte c[20];       // n bytes

// Memory:  518*n+25

for (k = 0; k<K / w; k++)
{
r = 0;
for (j = w - 1; j >= 0; j--)
{
r = r << 8;
r ^= sbox[k*w + j];
}
T[k][0] = r;
for (i = 1; i<n; i++)
T[k][i] = 0;
}

for (i = 0; i<(n - 1); i++)
{
k2 = a[i] / w;
for (k = 0; k<K / w; k++)
memcpy(Tp[k], T[k ^ k2], n*sizeof(word));
for (k = 0; k<K / w; k++)
{
memcpy(T[k], Tp[k], n*sizeof(word));
refreshword(T[k], n);
}
}

memcpy(b, T[a[n - 1] / w], n*sizeof(word));
refreshword(b, n);

// not counted above because the tables T and Tp can be discarded
byte Ts[8][20];   // 4*n bytes
byte Tsp[8][20];  // 4*n bytes

for (k = 0; k<w; k++)
for (i = 0; i<n; i++)
Ts[k][i] = b[i] >> (k * 8);

for (i = 0; i<(n - 1); i++)
{
for (k = 0; k<w; k++)
memcpy(Tsp[k], Ts[k ^ (a[i] & (w - 1))], n);
for (k = 0; k<w; k++)
{
memcpy(Ts[k], Tsp[k], n);
refresh(Ts[k], n);
}
}
memcpy(a, Ts[a[n - 1] & (w - 1)], n);
refresh(a, n);
}
*/



