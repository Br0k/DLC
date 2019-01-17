#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>

#define MAXCHAR 1000

/*
void readfile(char* filename ){

	FILE *fp;
	char str[MAXCHAR];

	fp = fopen(filename, "r");
	while (fgets(str, MAXCHAR, fp) != NULL)
		printf("%s\n", str);
	fclose(fp);
	//return 0;
}

*/

char *md5string(const char *str) {

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

char *sha1string(const char *str) {

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

char *hmacsha1str(char *key, const char *str){
    
    unsigned char* digest;

    digest = HMAC(EVP_sha1(), key, strlen(key), (unsigned char*)str, strlen(str), NULL, NULL);    

    char *hmacString = malloc(sizeof(char) * 41);

    for(int i = 0; i < 20; i++)
         sprintf(&hmacString[i*2], "%02x", (unsigned int)digest[i]);
 
    return hmacString;
}

int main(int argc, char **argv) {

	/*
	char data[BUFSIZ];
	printf("String to hash : ");
	scanf("%[^\n],s", data);

    char key[] = "Jefe";
    printf("Choosen key : %s\n", key);
    printf("Data to hash : %s\n", data);

    char *output = hmacsha1str(key, data);
    printf("HMAC-SHA1 Digest : %s\n", output);
    printf("length : %ld\n", strlen(output));
    */
	char filename[] = "/home/gohzr/Documents/M2_CRYPTIS/Projets/DLC/notes";
    //readfile(path);

	long len;
    char * buf = 0;
	
	FILE * file = fopen(filename, "rb");

	if (file)
	{
	  fseek (file, 0, SEEK_END);
	  len = ftell (file);
	  fseek (file, 0, SEEK_SET);
	  buf = malloc (sizeof(char) * len);
	  if (buf)
	  {
	    fread (buf, 1, len, file);
	  }
	  fclose (file);
	}

	if (buf){
		char *md5 = md5string(buf);
		char *sha1 = sha1string(buf);
		char key[] = "TheMasterHmacKey";
		char *hmac = hmacsha1str(key,buf);
		printf("MD5 Digest : %s\n", md5);
		printf("SHA1 Digest : %s\n", sha1);
		printf("HMAC-SHA1 Digest : %s\n", hmac);
	}
	
	/*
	char buffer[100];
	FILE *fp = fopen(filename, "r");                 // do not use "rb"
	while (fgets(buffer, sizeof(buffer), fp)) {
		char *output = md5string(buffer);
		printf("%s\n", output);
	}
	fclose(fp);
	*/

    return 0;
}