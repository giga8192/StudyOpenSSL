#include <string.h>
#include <stdio.h>
#include <openssl/evp.h>
#include <openssl/aes.h>

void PrintBytes(const unsigned char* bytes, const size_t length)
{
    int i;


    for(i=0; i<length; i++)
    {
        printf("%02x", bytes[i]);
    }
    printf("\n");
}


unsigned char* Encrypt(const char* key, const char* data, const size_t datalen, const unsigned char* iv, unsigned char* dest, const size_t destlen)
{
    EVP_CIPHER_CTX en;
    int i, f_len=0;
    int c_len = destlen;


    memset(dest, 0x00, destlen);


    EVP_CIPHER_CTX_init(&en);
    EVP_EncryptInit_ex(&en, EVP_aes_128_cbc(), NULL, (unsigned char*)key, iv);


    EVP_EncryptUpdate(&en, dest, &c_len, (unsigned char *)data, datalen);
    //EVP_EncryptFinal_ex(&en, (unsigned char *)(dest + c_len), &f_len);


    printf("c_len: %d\n", c_len);
    printf("f_len: %d\n", f_len);
    PrintBytes(dest, destlen);


    EVP_CIPHER_CTX_cleanup(&en);


    return dest;
}

unsigned char* Decrypt(const char* key, const unsigned char* data, const size_t datalen, const unsigned char* iv, char* dest, const size_t destlen)
{
    EVP_CIPHER_CTX de;
    int f_len = 0;
    int p_len = datalen;


    memset(dest, 0x00, destlen);


    EVP_CIPHER_CTX_init(&de);
    EVP_DecryptInit_ex(&de, EVP_aes_128_cbc(), NULL, (unsigned char*)key, iv);


    EVP_DecryptUpdate(&de, (unsigned char *)dest, &p_len, data, datalen);
    //EVP_DecryptFinal_ex(&de, (unsigned char *)(dest + p_len), &f_len);


    EVP_CIPHER_CTX_cleanup(&de);


    printf("p_len: %d\n", p_len);
    printf("f_len: %d\n", f_len);
    printf("%s\n", dest);
    PrintBytes(dest, destlen);


    return dest;
}

int main()
{
    const char key[] =  "abcdefghijklmnop";  // 暗号化に使う鍵。16バイト。
    const char data[] = "hello, OpenSSL! 123456789012345\0";  // 暗号化するデータ。ここでは32バイト。
    const unsigned char iv = "abcdefghijklmnop";  // 初期ベクトル。16バイト。


    unsigned char encode[32] = {'\0'};  // 暗号化したデータを入れる場所。
    char decode[32] = {'\0'};  // 複合したデータを入れる場所。


    printf("%s\n", data);
    PrintBytes(data, sizeof(data)-1);


    Encrypt(key, data, sizeof(data), iv, encode, sizeof(encode));
    Decrypt(key, encode, sizeof(encode), iv, decode, sizeof(decode));


    return 0;
}
