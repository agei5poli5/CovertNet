#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

class encryption{
public: 
	encryption();
	int aes_init(unsigned char *key_data, int key_data_len, unsigned char *salt, EVP_CIPHER_CTX *e_ctx, 
             EVP_CIPHER_CTX *d_ctx);
	unsigned char* aes_encrypt(EVP_CIPHER_CTX *e, unsigned char *plaintext, int *len);
    

};