#include <stdint.h>
#include <stdio.h>
#include "gmp.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


static const unsigned char sbox[256] = {
  0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
  0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
  0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
  0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
  0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
  0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
  0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
  0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
  0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
  0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
  0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
  0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
  0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
  0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
  0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
  0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 
};

static const unsigned char Invsbox[256] = {
	0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
	0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
	0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
	0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
	0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
	0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
	0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
	0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
	0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
	0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
	0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
	0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
	0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
	0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
	0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
	0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d
};

static const unsigned char gmul9[] = {
  0x00,0x09,0x12,0x1b,0x24,0x2d,0x36,0x3f,0x48,0x41,0x5a,0x53,0x6c,0x65,0x7e,0x77,
  0x90,0x99,0x82,0x8b,0xb4,0xbd,0xa6,0xaf,0xd8,0xd1,0xca,0xc3,0xfc,0xf5,0xee,0xe7,
  0x3b,0x32,0x29,0x20,0x1f,0x16,0x0d,0x04,0x73,0x7a,0x61,0x68,0x57,0x5e,0x45,0x4c,
  0xab,0xa2,0xb9,0xb0,0x8f,0x86,0x9d,0x94,0xe3,0xea,0xf1,0xf8,0xc7,0xce,0xd5,0xdc,
  0x76,0x7f,0x64,0x6d,0x52,0x5b,0x40,0x49,0x3e,0x37,0x2c,0x25,0x1a,0x13,0x08,0x01,
  0xe6,0xef,0xf4,0xfd,0xc2,0xcb,0xd0,0xd9,0xae,0xa7,0xbc,0xb5,0x8a,0x83,0x98,0x91,
  0x4d,0x44,0x5f,0x56,0x69,0x60,0x7b,0x72,0x05,0x0c,0x17,0x1e,0x21,0x28,0x33,0x3a,
  0xdd,0xd4,0xcf,0xc6,0xf9,0xf0,0xeb,0xe2,0x95,0x9c,0x87,0x8e,0xb1,0xb8,0xa3,0xaa,
  0xec,0xe5,0xfe,0xf7,0xc8,0xc1,0xda,0xd3,0xa4,0xad,0xb6,0xbf,0x80,0x89,0x92,0x9b,
  0x7c,0x75,0x6e,0x67,0x58,0x51,0x4a,0x43,0x34,0x3d,0x26,0x2f,0x10,0x19,0x02,0x0b,
  0xd7,0xde,0xc5,0xcc,0xf3,0xfa,0xe1,0xe8,0x9f,0x96,0x8d,0x84,0xbb,0xb2,0xa9,0xa0,
  0x47,0x4e,0x55,0x5c,0x63,0x6a,0x71,0x78,0x0f,0x06,0x1d,0x14,0x2b,0x22,0x39,0x30,
  0x9a,0x93,0x88,0x81,0xbe,0xb7,0xac,0xa5,0xd2,0xdb,0xc0,0xc9,0xf6,0xff,0xe4,0xed,
  0x0a,0x03,0x18,0x11,0x2e,0x27,0x3c,0x35,0x42,0x4b,0x50,0x59,0x66,0x6f,0x74,0x7d,
  0xa1,0xa8,0xb3,0xba,0x85,0x8c,0x97,0x9e,0xe9,0xe0,0xfb,0xf2,0xcd,0xc4,0xdf,0xd6,
  0x31,0x38,0x23,0x2a,0x15,0x1c,0x07,0x0e,0x79,0x70,0x6b,0x62,0x5d,0x54,0x4f,0x46
};

static const unsigned char gmul11[] = {
  0x00,0x0b,0x16,0x1d,0x2c,0x27,0x3a,0x31,0x58,0x53,0x4e,0x45,0x74,0x7f,0x62,0x69,
  0xb0,0xbb,0xa6,0xad,0x9c,0x97,0x8a,0x81,0xe8,0xe3,0xfe,0xf5,0xc4,0xcf,0xd2,0xd9,
  0x7b,0x70,0x6d,0x66,0x57,0x5c,0x41,0x4a,0x23,0x28,0x35,0x3e,0x0f,0x04,0x19,0x12,
  0xcb,0xc0,0xdd,0xd6,0xe7,0xec,0xf1,0xfa,0x93,0x98,0x85,0x8e,0xbf,0xb4,0xa9,0xa2,
  0xf6,0xfd,0xe0,0xeb,0xda,0xd1,0xcc,0xc7,0xae,0xa5,0xb8,0xb3,0x82,0x89,0x94,0x9f,
  0x46,0x4d,0x50,0x5b,0x6a,0x61,0x7c,0x77,0x1e,0x15,0x08,0x03,0x32,0x39,0x24,0x2f,
  0x8d,0x86,0x9b,0x90,0xa1,0xaa,0xb7,0xbc,0xd5,0xde,0xc3,0xc8,0xf9,0xf2,0xef,0xe4,
  0x3d,0x36,0x2b,0x20,0x11,0x1a,0x07,0x0c,0x65,0x6e,0x73,0x78,0x49,0x42,0x5f,0x54,
  0xf7,0xfc,0xe1,0xea,0xdb,0xd0,0xcd,0xc6,0xaf,0xa4,0xb9,0xb2,0x83,0x88,0x95,0x9e,
  0x47,0x4c,0x51,0x5a,0x6b,0x60,0x7d,0x76,0x1f,0x14,0x09,0x02,0x33,0x38,0x25,0x2e,
  0x8c,0x87,0x9a,0x91,0xa0,0xab,0xb6,0xbd,0xd4,0xdf,0xc2,0xc9,0xf8,0xf3,0xee,0xe5,
  0x3c,0x37,0x2a,0x21,0x10,0x1b,0x06,0x0d,0x64,0x6f,0x72,0x79,0x48,0x43,0x5e,0x55,
  0x01,0x0a,0x17,0x1c,0x2d,0x26,0x3b,0x30,0x59,0x52,0x4f,0x44,0x75,0x7e,0x63,0x68,
  0xb1,0xba,0xa7,0xac,0x9d,0x96,0x8b,0x80,0xe9,0xe2,0xff,0xf4,0xc5,0xce,0xd3,0xd8,
  0x7a,0x71,0x6c,0x67,0x56,0x5d,0x40,0x4b,0x22,0x29,0x34,0x3f,0x0e,0x05,0x18,0x13,
  0xca,0xc1,0xdc,0xd7,0xe6,0xed,0xf0,0xfb,0x92,0x99,0x84,0x8f,0xbe,0xb5,0xa8,0xa3
};

static const unsigned char gmul13[] = {
  0x00,0x0d,0x1a,0x17,0x34,0x39,0x2e,0x23,0x68,0x65,0x72,0x7f,0x5c,0x51,0x46,0x4b,
  0xd0,0xdd,0xca,0xc7,0xe4,0xe9,0xfe,0xf3,0xb8,0xb5,0xa2,0xaf,0x8c,0x81,0x96,0x9b,
  0xbb,0xb6,0xa1,0xac,0x8f,0x82,0x95,0x98,0xd3,0xde,0xc9,0xc4,0xe7,0xea,0xfd,0xf0,
  0x6b,0x66,0x71,0x7c,0x5f,0x52,0x45,0x48,0x03,0x0e,0x19,0x14,0x37,0x3a,0x2d,0x20,
  0x6d,0x60,0x77,0x7a,0x59,0x54,0x43,0x4e,0x05,0x08,0x1f,0x12,0x31,0x3c,0x2b,0x26,
  0xbd,0xb0,0xa7,0xaa,0x89,0x84,0x93,0x9e,0xd5,0xd8,0xcf,0xc2,0xe1,0xec,0xfb,0xf6,
  0xd6,0xdb,0xcc,0xc1,0xe2,0xef,0xf8,0xf5,0xbe,0xb3,0xa4,0xa9,0x8a,0x87,0x90,0x9d,
  0x06,0x0b,0x1c,0x11,0x32,0x3f,0x28,0x25,0x6e,0x63,0x74,0x79,0x5a,0x57,0x40,0x4d,
  0xda,0xd7,0xc0,0xcd,0xee,0xe3,0xf4,0xf9,0xb2,0xbf,0xa8,0xa5,0x86,0x8b,0x9c,0x91,
  0x0a,0x07,0x10,0x1d,0x3e,0x33,0x24,0x29,0x62,0x6f,0x78,0x75,0x56,0x5b,0x4c,0x41,
  0x61,0x6c,0x7b,0x76,0x55,0x58,0x4f,0x42,0x09,0x04,0x13,0x1e,0x3d,0x30,0x27,0x2a,
  0xb1,0xbc,0xab,0xa6,0x85,0x88,0x9f,0x92,0xd9,0xd4,0xc3,0xce,0xed,0xe0,0xf7,0xfa,
  0xb7,0xba,0xad,0xa0,0x83,0x8e,0x99,0x94,0xdf,0xd2,0xc5,0xc8,0xeb,0xe6,0xf1,0xfc,
  0x67,0x6a,0x7d,0x70,0x53,0x5e,0x49,0x44,0x0f,0x02,0x15,0x18,0x3b,0x36,0x21,0x2c,
  0x0c,0x01,0x16,0x1b,0x38,0x35,0x22,0x2f,0x64,0x69,0x7e,0x73,0x50,0x5d,0x4a,0x47,
  0xdc,0xd1,0xc6,0xcb,0xe8,0xe5,0xf2,0xff,0xb4,0xb9,0xae,0xa3,0x80,0x8d,0x9a,0x97
};

static const unsigned char gmul14[] = {
  0x00,0x0e,0x1c,0x12,0x38,0x36,0x24,0x2a,0x70,0x7e,0x6c,0x62,0x48,0x46,0x54,0x5a,
  0xe0,0xee,0xfc,0xf2,0xd8,0xd6,0xc4,0xca,0x90,0x9e,0x8c,0x82,0xa8,0xa6,0xb4,0xba,
  0xdb,0xd5,0xc7,0xc9,0xe3,0xed,0xff,0xf1,0xab,0xa5,0xb7,0xb9,0x93,0x9d,0x8f,0x81,
  0x3b,0x35,0x27,0x29,0x03,0x0d,0x1f,0x11,0x4b,0x45,0x57,0x59,0x73,0x7d,0x6f,0x61,
  0xad,0xa3,0xb1,0xbf,0x95,0x9b,0x89,0x87,0xdd,0xd3,0xc1,0xcf,0xe5,0xeb,0xf9,0xf7,
  0x4d,0x43,0x51,0x5f,0x75,0x7b,0x69,0x67,0x3d,0x33,0x21,0x2f,0x05,0x0b,0x19,0x17,
  0x76,0x78,0x6a,0x64,0x4e,0x40,0x52,0x5c,0x06,0x08,0x1a,0x14,0x3e,0x30,0x22,0x2c,
  0x96,0x98,0x8a,0x84,0xae,0xa0,0xb2,0xbc,0xe6,0xe8,0xfa,0xf4,0xde,0xd0,0xc2,0xcc,
  0x41,0x4f,0x5d,0x53,0x79,0x77,0x65,0x6b,0x31,0x3f,0x2d,0x23,0x09,0x07,0x15,0x1b,
  0xa1,0xaf,0xbd,0xb3,0x99,0x97,0x85,0x8b,0xd1,0xdf,0xcd,0xc3,0xe9,0xe7,0xf5,0xfb,
  0x9a,0x94,0x86,0x88,0xa2,0xac,0xbe,0xb0,0xea,0xe4,0xf6,0xf8,0xd2,0xdc,0xce,0xc0,
  0x7a,0x74,0x66,0x68,0x42,0x4c,0x5e,0x50,0x0a,0x04,0x16,0x18,0x32,0x3c,0x2e,0x20,
  0xec,0xe2,0xf0,0xfe,0xd4,0xda,0xc8,0xc6,0x9c,0x92,0x80,0x8e,0xa4,0xaa,0xb8,0xb6,
  0x0c,0x02,0x10,0x1e,0x34,0x3a,0x28,0x26,0x7c,0x72,0x60,0x6e,0x44,0x4a,0x58,0x56,
  0x37,0x39,0x2b,0x25,0x0f,0x01,0x13,0x1d,0x47,0x49,0x5b,0x55,0x7f,0x71,0x63,0x6d,
  0xd7,0xd9,0xcb,0xc5,0xef,0xe1,0xf3,0xfd,0xa7,0xa9,0xbb,0xb5,0x9f,0x91,0x83,0x8d
};

void InvKeyExpansion(unsigned char* key, unsigned char* keys, int Nk, int Nr){
    uint8_t tmp[4];
    int k;

    for (int i = 0; i < Nk; ++i)
    {
        keys[(i*4)+0] = key[(i*4)+0];
        keys[(i*4)+1] = key[(i*4)+1];
        keys[(i*4)+2] = key[(i*4)+2];
        keys[(i*4)+3] = key[(i*4)+3];
    }

    for (int i = Nk; i < 4 * (Nr+1); ++i)
    {
        k = (i-1) * 4;
        tmp[0] = keys[k+0];
        tmp[1] = keys[k+1];
        tmp[2] = keys[k+2];
        tmp[3] = keys[k+3];

        if (i % Nk == 0)
        {
            const uint8_t var_tmp = tmp[0];
            tmp[0] = tmp[1];
            tmp[1] = tmp[2];
            tmp[2] = tmp[3];
            tmp[3] = var_tmp;

            tmp[0] = sbox[tmp[0]];
            tmp[1] = sbox[tmp[1]];
            tmp[2] = sbox[tmp[2]];
            tmp[3] = sbox[tmp[3]];

            tmp[0] = tmp[0] ^ rcon[i/Nk];
        }

        if (Nk==8)
        {
            if (i % Nk ==4)
            {
                tmp[0] = sbox[tmp[0]];
                tmp[1] = sbox[tmp[1]];
                tmp[2] = sbox[tmp[2]];
                tmp[3] = sbox[tmp[3]];
            }
        }

        int j = i * 4;
        int k = (i-Nk) * 4;

        keys[j+0] = keys[k+0] ^ tmp[0];
        keys[j+1] = keys[k+1] ^ tmp[1];
        keys[j+2] = keys[k+2] ^ tmp[2];
        keys[j+3] = keys[k+3] ^ tmp[3];
    }
}

void affichage(unsigned char* txt, int lenKey, unsigned char* msg){
	printf("%s = ", txt);

	for (int i = 0; i < lenKey; ++i)
	{
		printf("%02X ",(unsigned char) *(msg+i));
	}
	printf("\n");
}


void addRoundKey(int round, unsigned char* msg, unsigned char* key, int compteur){

	unsigned char tmp[17];
  sprintf((char*)tmp, "round[%2d].ik_sch", compteur);
	affichage(tmp,16, key);
	for (int i = 0; i < 16; ++i)
	{
		msg[i] ^= key[i];
	}
}

void invMixColumns(unsigned char* travail) {
	for (int col=0; col<4; col++) {
		unsigned char r[4];

		// Make a copy of the current colun to manipulate
		unsigned char a[4];
		memcpy(a, travail + col*4, 4);

		// Perform the matrix multiplication
		r[0] = gmul14[a[0]] ^ gmul11[a[1]] ^ gmul13[a[2]] ^ gmul9[a[3]];
		r[1] = gmul9[a[0]] ^ gmul14[a[1]] ^ gmul11[a[2]] ^ gmul13[a[3]];
		r[2] = gmul13[a[0]] ^ gmul9[a[1]] ^ gmul14[a[2]] ^ gmul11[a[3]];
		r[3] = gmul11[a[0]] ^ gmul13[a[1]] ^ gmul9[a[2]] ^ gmul14[a[3]];

		// Copy the answer back to the travail
		memcpy(travail + col*4, r, 4);
	}
}

void invShiftRows(unsigned char* travail) {
	unsigned char tmp[16];
	tmp[0] = travail[0];
	tmp[4] = travail[4];
	tmp[8] = travail[8];
	tmp[12] = travail[12];

	tmp[1] = travail[13];
	tmp[5] = travail[1];
	tmp[9] = travail[5];
	tmp[13] = travail[9];

	tmp[2] = travail[10];
	tmp[6] = travail[14];
	tmp[10] = travail[2];
	tmp[14] = travail[6];

	tmp[3] = travail[7];
	tmp[7] = travail[11];
	tmp[11] = travail[15];
	tmp[15] = travail[3];

	for (int i = 0; i < 16; ++i)
	{
		travail[i] = tmp[i];
	}
}

void invSubBytes(unsigned char* travail) {
	for (int i = 0; i < 16; ++i)
	{
		travail[i] = Invsbox[travail[i]];
	}
}

unsigned char* AES_decrypt(unsigned char* cipher, unsigned char* keys, int Nr, int expandedLenght){

    unsigned char travail[16];
    for (int i = 0; i < 16; ++i)
    {
    	travail[i] = cipher[i];
    }
    int lentravail = sizeof(travail) / sizeof(travail[0]);

    int compteur = 0;
    unsigned char tmp[17];
    sprintf((char*)tmp, "round[%2d].iinput", compteur);

	  affichage(tmp, lentravail, travail);
    addRoundKey(Nr,travail,keys+(16*Nr),compteur);

    compteur++;
    sprintf((char*)tmp, "round[%2d].istart", compteur);
    affichage(tmp, lentravail, travail);
    

    for (int round = Nr-1; round >= 1; round--)
    {
    	invShiftRows(travail);
    	sprintf((char*)tmp, "round[%2d].is_row", compteur);
		  affichage(tmp, lentravail, travail);


	    invSubBytes(travail);
	    sprintf((char*)tmp, "round[%2d].is_box", compteur);
	    affichage(tmp, lentravail, travail);
		
    	addRoundKey(round,travail,keys+(16*round), compteur);

	    invMixColumns(travail);
	    sprintf((char*)tmp, "round[%2d].ik_add", compteur);
	    affichage(tmp, lentravail, travail);
	    compteur++;
      printf("\n");
    }

    sprintf((char*)tmp, "round[%2d].istart", compteur);
	  affichage(tmp, lentravail, travail);

    invShiftRows(travail);
    sprintf((char*)tmp, "round[%2d].is_row", compteur);
	  affichage(tmp, lentravail, travail);

    invSubBytes(travail);
    sprintf((char*)tmp, "round[%2d].is_box", compteur);
    affichage(tmp, lentravail, travail);


    addRoundKey(0,travail,keys, compteur);
    
    uint8_t* plaintext = malloc(sizeof(uint8_t*)*16);
    memcpy(plaintext,travail,16);
    sprintf((char*)tmp, "round[%2d].output", compteur);
    affichage(tmp,lentravail,plaintext);

    return plaintext;
}

uint8_t**  InvMain_AES(char* AES,uint8_t **message,uint8_t * key,int *len,uint8_t *IV)
{
  int Nk=0,Nr=0,lenExpendKey=0; //Nb=4
  int lenAES;

  if(strcmp(AES,"AES 128")==0 || strcmp(AES,"AES 128 CBC")==0)
  {
    Nk=4;
    Nr=10;
    lenAES=16;
    lenExpendKey=176;
  }else if (strcmp(AES,"AES 192")==0 || strcmp(AES,"AES 192 CBC")==0)
  {
    Nk=6;
    Nr=12;
    lenAES=24;
    lenExpendKey=208;
  }else
  {
    Nk=8;
    Nr=14;
    lenAES=32;
    lenExpendKey=240;
  }

  uint8_t **essai;
  if (*len == 1)
  {
    essai=malloc(sizeof(uint8_t *)*1);
    essai[0]=malloc(sizeof(uint8_t *)*16);
  }
  else {
    essai=malloc(sizeof(uint8_t *)*(*len));
    for (int i = 0; i < (*len); ++i)
    {
      essai[i]=malloc(sizeof(uint8_t *)*16);
    }
  }

  char* cs;
  int compteur = 0;

  for (int i = 0; (cs = strtok ((char*)message, " ")); ++i)
  {
    int tmp = hexadecimalToDecimal(cs); 
    essai[i]=tmp;      
    message = NULL;
    compteur++;
  }

  *len = compteur;

  uint8_t **MsgToAES; 
  MsgToAES=malloc(sizeof(uint8_t*)*compteur/16);

  uint8_t **plaintext;
  plaintext=malloc(sizeof(uint8_t *)*compteur/16);

  for (int i = 0; i < compteur/16; i++)
  {
    plaintext[i]=malloc(sizeof(uint8_t*)*16);
  }
  
  for (int i = 0; i < compteur/16; ++i)
  {
    MsgToAES[i]=malloc(sizeof(uint8_t*)*16);
    for (int j = 0; j < 16; ++j)
    {
      MsgToAES[i][j] = (uint8_t)*(essai+(i*16)+j);
    }
  }
  
  unsigned char expandedKeys[lenExpendKey];

  InvKeyExpansion(key, expandedKeys, Nk, Nr);

  printf("KEY expanded\n");
  int saut = 0;
  for (int i = 0; i < lenExpendKey; i++)
  {
    printf("%02X ",(unsigned char) *(expandedKeys+i));
    saut++;
    if (saut >= lenAES)
    {
      printf("\n");
      saut = 0;
    } 
  }
    
  int pos=0;
  for (int i = 0; i < compteur/16; i++)
  {   
    plaintext[pos] = AES_decrypt(MsgToAES[i],expandedKeys,Nr,lenExpendKey);
    
    if (i == 0 && IV!=NULL)
    {
      for (int j = 0; j < 16; j++)
      {
        plaintext[pos][j] ^= IV[j];
      }
    }
    if((i<compteur/16 && IV!=NULL)&& i != 0){
      for (int j = 0; j < 16; j++) 
      {
        printf("i %d, j %d\n", i, j);
        plaintext[i][j]=plaintext[pos][j]^MsgToAES[i-1][j];
      }
    }
    print_hex(plaintext[pos]);
    pos++;
  }

  free(MsgToAES);
  free(essai);


  return plaintext;
}
