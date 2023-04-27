#include "../../inc/client.h"

char* mx_hash_sha256(const char *password){
  
    EVP_MD_CTX *mdctx = EVP_MD_CTX_new();
    const EVP_MD *md = EVP_sha256();
    EVP_DigestInit_ex(mdctx, md, NULL);
    EVP_DigestUpdate(mdctx, password, strlen(password));

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int hash_len;
    EVP_DigestFinal_ex(mdctx, hash, &hash_len);

    char *hash_str = malloc(hash_len * 2 + 1);
    for (unsigned int i = 0; i < hash_len; i++) {
        sprintf(&hash_str[i * 2], "%02x", hash[i]);
    }

    EVP_MD_CTX_free(mdctx);

    return hash_str;
}

