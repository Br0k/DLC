#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

char *md5digest(const char *str) {

	unsigned char digest[16];

	MD5_CTX context;
	MD5_Init(&context);
	MD5_Update(&context, str, strlen(str));
	MD5_Final(digest, &context);

	char *mdString = malloc(sizeof(char) * 32);

	// Affichage hexadecimal
	for (int i = 0; i < 16; i++)
		sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

	return mdString;

    }

char *sha1digest(const char *str) {

	unsigned char digest[20];

	SHA_CTX context;
	SHA1_Init(&context);
	SHA1_Update(&context, str, strlen(str));
	SHA1_Final(digest, &context);

	char *shaString = malloc(sizeof(char) * 41);

	for (int i = 0; i < 20; i++)
		sprintf(&shaString[i*2], "%02x", (unsigned int)digest[i]);

	return shaString;

    }

char *hmac_sha1(char *key, const char *str){
    
    unsigned char* digest;

    digest = HMAC(EVP_sha1(), key, strlen(key), (unsigned char*)str, strlen(str), NULL, NULL);    

    char *hmacString = malloc(sizeof(char) * 41);

    for(int i = 0; i < 20; i++)
         sprintf(&hmacString[i*2], "%02x", (unsigned int)digest[i]);
 
    return hmacString;
}