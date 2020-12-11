#pragma once

#include <openssl/aes.h>
#include <openssl/evp.h>
#include <openssl/rand.h>

void conditionalEncryptionOrDecryption(int conditionEncryption, FILE *iInputFile, FILE *iOutputFile, unsigned char *iAesKey, unsigned char *iIvec) {

    unsigned char *bufferToRead = new unsigned char[1024];
    unsigned char *bufferToCipher;
    unsigned blockSize;
    int outputLength;

    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    EVP_CipherInit(ctx, EVP_aes_256_cbc(), iAesKey, iIvec, conditionEncryption);

    blockSize = EVP_CIPHER_CTX_block_size(ctx);
    bufferToCipher = new unsigned char[1024 + blockSize];

    while (true) {
        // Read in data in blocks until EOF. Update the ciphering with each read.
        int charactersRead = fread(bufferToRead, sizeof(unsigned char), 1024, iInputFile);
        EVP_CipherUpdate(ctx, bufferToCipher, &outputLength, bufferToRead, charactersRead);
        fwrite(bufferToCipher, sizeof(unsigned char), outputLength, iOutputFile);
        if (charactersRead < 1024)
		{
			EVP_CipherFinal(ctx, bufferToCipher, &outputLength);
			EVP_CIPHER_CTX_free(ctx);
    		fwrite(bufferToCipher, sizeof(unsigned char), outputLength, iOutputFile);
            break;
        }
    }

    free(bufferToCipher);
    free(bufferToRead);
}