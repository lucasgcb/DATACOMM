#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
#define TAMANHO_PAYLOAD 16
/* AES key for Encryption and Decryption */
const static unsigned char aes_key[]={'k','k','e','a','e','m','e','n',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
 
/* Print Encrypted and Decrypted data packets */
void print_data(const char *tittle, const void* data, int len);
 
int descriptografarPacote(unsigned char *aes_input, int len)
{
        /* Input data to encrypt */
	printf("%d", sizeof(aes_key));
        /* Init vector */
        unsigned char iv[AES_BLOCK_SIZE];
        memset(iv, 0x00, AES_BLOCK_SIZE);

        /* Buffers for Encryption and Decryption */
        unsigned char dec_out[len];

        /* AES-128 bit CBC Encryption */
        AES_KEY enc_key, dec_key;
        AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
        AES_cbc_encrypt(aes_input, dec_out, len, &dec_key, iv, AES_DECRYPT);

        /* Printing and Verifying */
        print_data("\n Original ",aes_input, len); // you can not print data as a string, because after Encryption its not ASCII


        print_data("\n Decrypted",dec_out, sizeof(dec_out));
        return atoi(dec_out);
}
 
void print_data(const char *tittle, const void* data, int len)
{
        printf("%s : ",tittle);
        const unsigned char * p = (const unsigned char*)data;
        int i = 0;

        for (; i<len; ++i)
                printf("%d ", *p++);

        printf("\n");
}

