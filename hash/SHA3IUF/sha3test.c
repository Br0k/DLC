/* -------------------------------------------------------------------------
 * Works when compiled for either 32-bit or 64-bit targets, optimized for 
 * 64 bit.
 *
 * Canonical implementation of Init/Update/Finalize for SHA-3 byte input. 
 *
 * SHA3-256, SHA3-384, SHA-512 are implemented. SHA-224 can easily be added.
 *
 * Based on code from http://keccak.noekeon.org/ .
 *
 * I place the code that I wrote into public domain, free to use. 
 *
 * I would appreciate if you give credits to this work if you used it to 
 * write or test * your code.
 *
 * Aug 2015. Andrey Jivsov. crypto@brainhub.org
 * ---------------------------------------------------------------------- */

/* *************************** Self Tests ************************ */

/* 
 * There are two set of mutually exclusive tests, based on SHA3_USE_KECCAK,
 * which is undefined in the production version.
 *
 * Known answer tests are from NIST SHA3 test vectors at
 * http://csrc.nist.gov/groups/ST/toolkit/examples.html
 *
 * SHA3-256:
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-256_Msg0.pdf
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-256_1600.pdf
 * SHA3-384: 
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-384_1600.pdf 
 * SHA3-512: 
 *   http://csrc.nist.gov/groups/ST/toolkit/documents/Examples/SHA3-512_1600.pdf 
 *
 * These are refered to as [FIPS 202] tests.
 *
 * -----
 *
 * A few Keccak algorithm tests (when M and not M||01 is hashed) are
 * added here. These are from http://keccak.noekeon.org/KeccakKAT-3.zip,
 * ShortMsgKAT_256.txt for sizes even to 8. There is also one test for 
 * ExtremelyLongMsgKAT_256.txt.
 *
 * These will work with this code when SHA3_USE_KECCAK converts Finalize
 * to use "pure" Keccak algorithm.
 *
 *
 * These are referred to as [Keccak] test.
 *
 * -----
 *
 * In one case the input from [Keccak] test was used to test SHA3
 * implementation. In this case the calculated hash was compared with
 * the output of the sha3sum on Fedora Core 20 (which is Perl's based).
 *
 */

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include "sha3.h"
#include <inttypes.h>
char *sha3_keccak(char * chaine){
//int main(){
    uint8_t buf[200];
    sha3_context c;
    const uint8_t *hash;
    unsigned i;
    const uint8_t c1 = 0xa3;

    /* [FIPS 202] KAT follow */
    const static uint8_t sha3_256_empty[256 / 8] = {
        0xa7, 0xff, 0xc6, 0xf8, 0xbf, 0x1e, 0xd7, 0x66,
	0x51, 0xc1, 0x47, 0x56, 0xa0, 0x61, 0xd6, 0x62,
	0xf5, 0x80, 0xff, 0x4d, 0xe4, 0x3b, 0x49, 0xfa, 
	0x82, 0xd8, 0x0a, 0x4b, 0x80, 0xf8, 0x43, 0x4a
    };
    const static uint8_t sha3_256_0xa3_200_times[256 / 8] = {
        0x79, 0xf3, 0x8a, 0xde, 0xc5, 0xc2, 0x03, 0x07,
        0xa9, 0x8e, 0xf7, 0x6e, 0x83, 0x24, 0xaf, 0xbf,
        0xd4, 0x6c, 0xfd, 0x81, 0xb2, 0x2e, 0x39, 0x73,
        0xc6, 0x5f, 0xa1, 0xbd, 0x9d, 0xe3, 0x17, 0x87
    };
    const static uint8_t sha3_384_0xa3_200_times[384 / 8] = {
        0x18, 0x81, 0xde, 0x2c, 0xa7, 0xe4, 0x1e, 0xf9,
        0x5d, 0xc4, 0x73, 0x2b, 0x8f, 0x5f, 0x00, 0x2b,
        0x18, 0x9c, 0xc1, 0xe4, 0x2b, 0x74, 0x16, 0x8e,
        0xd1, 0x73, 0x26, 0x49, 0xce, 0x1d, 0xbc, 0xdd,
        0x76, 0x19, 0x7a, 0x31, 0xfd, 0x55, 0xee, 0x98,
        0x9f, 0x2d, 0x70, 0x50, 0xdd, 0x47, 0x3e, 0x8f
    };
    const static uint8_t sha3_512_0xa3_200_times[512 / 8] = {
        0xe7, 0x6d, 0xfa, 0xd2, 0x20, 0x84, 0xa8, 0xb1,
        0x46, 0x7f, 0xcf, 0x2f, 0xfa, 0x58, 0x36, 0x1b,
        0xec, 0x76, 0x28, 0xed, 0xf5, 0xf3, 0xfd, 0xc0,
        0xe4, 0x80, 0x5d, 0xc4, 0x8c, 0xae, 0xec, 0xa8,
        0x1b, 0x7c, 0x13, 0xc3, 0x0a, 0xdf, 0x52, 0xa3,
        0x65, 0x95, 0x84, 0x73, 0x9a, 0x2d, 0xf4, 0x6b,
        0xe5, 0x89, 0xc5, 0x1c, 0xa1, 0xa4, 0xa8, 0x41,
        0x6d, 0xf6, 0x54, 0x5a, 0x1c, 0xe8, 0xba, 0x00
    };

    memset(buf, c1, sizeof(buf));

    /* ---- "pure" Keccak algorithm begins; from [Keccak] ----- */
    sha3_HashBuffer(256, SHA3_FLAGS_KECCAK, chaine, strlen(chaine), buf, sizeof(buf)) ;
    //sha3_HashBuffer(256, SHA3_FLAGS_KECCAK, "abc", 3, buf, sizeof(buf)) ;
    char *resultat = malloc (128*sizeof(char));
 	//resultat = "";
	for (int i = 0 ; i < 64; i++)
		resultat[i] = '\0';
	int  x;
	char *y = malloc(10 *sizeof(char));
	for( int i = 0 ; i < 32 ; i++){  //    32 * 8 = 256 <==>  sortie du keccak 256
		//printf("%d  % " PRIx8 ,i, buf[i]);
		x = buf[i];
		sprintf(y,"%02x",x);
		//printf("  %s\n",y);
		//strcat(resultat,y);
		sprintf(resultat,"%s%02x",resultat,x);			
		//sprintf(resultat,"%s%s",resultat,y);
  
	}
	printf("%s\n",resultat);
	return resultat;
	//return 0;
}
