#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/aes.h>
 
/* AES key for Encryption and Decryption */
const static unsigned char aes_key[]={'k','k','e','a','e','m','e','n',0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
 
/* Print Encrypted and Decrypted data packets */
void print_data(const char *tittle, const void* data, int len);
 
int main( )
{
	/* Input data to encrypt */
//	unsigned char aes_input[]={172,1,186,230,235,2,9,105,175,215,56,148,212,10,124,176};
	unsigned char aes_input[]={49,56,49,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
	
	/* Init vector */
	unsigned char iv[AES_BLOCK_SIZE];
	memset(iv, 0x00, AES_BLOCK_SIZE);
	
	/* Buffers for Encryption and Decryption */
	unsigned char enc_out[sizeof(aes_input)];
	unsigned char dec_out[sizeof(aes_input)];
	
	/* AES-128 bit CBC Encryption */
	AES_KEY enc_key, dec_key;
	AES_set_encrypt_key(aes_key, sizeof(aes_key)*8, &enc_key);
	AES_cbc_encrypt(aes_input, enc_out, sizeof(aes_input), &enc_key, iv, AES_ENCRYPT);
	/* AES-128 bit CBC Decryption */
	memset(iv, 0x00, AES_BLOCK_SIZE); // don't forget to set iv vector again, else you can't decrypt data properly
	AES_set_decrypt_key(aes_key, sizeof(aes_key)*8, &dec_key); // Size of key is in bits
	AES_cbc_encrypt(enc_out, dec_out, sizeof(aes_input), &dec_key, iv, AES_DECRYPT);
	
	/* Printing and Verifying */
	print_data("\n Original ",aes_input, sizeof(aes_input)); // you can not print data as a string, because after Encryption its not ASCII
	
	print_data("\n Encrypted",enc_out, sizeof(enc_out));
	
	print_data("\n Decrypted",dec_out, sizeof(dec_out));
	
	return 0;
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
