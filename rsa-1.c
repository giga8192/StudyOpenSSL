/*
 * $Id: rsa-1.c,v 1.3 2005/02/19 16:01:53 68user Exp $
 *
 * OpenSSL を使った RSA の実装 (1)
 *   素数生成・RSA 鍵生成・暗号化・復号化を全てライブラリにまかせる版。
 * written by 68user  http://X68000.q-e-d.net/~68user/
 */

// #define OPENSSL_API_COMPAT 0x00900000L
// /usr/lib/x86_64-linux-gnu/libcrypto.so
// 静的にリンクした. libcrypto.a以外に　-ldl と-lpthreadは指定する必要があった: gcc -o rsa_static rsa-1.c /usr/lib/x86_64-linux-gnu/libcrypto.a  -ldl -lpthread
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/rsa.h>
#include <openssl/engine.h>
#include <openssl/err.h>

// #define BN_PUT(bn) { printf(#bn "=%s (0x%s)\n", BN_bn2dec(bn), BN_bn2hex(bn)); }
#define BN_PUT(bn) { printf(#bn "=%s (0x%s)\n", BN_bn2dec(bn), BN_bn2hex(bn)); }
#define KEYBIT_LEN 1024

int
main(){
    unsigned char plain_buf[]="hogehoge";
    unsigned char *crypted_buf;
    unsigned char *decrypted_buf;
    int crypted_len;
    int decrypted_len;
    char errbuf[1024];
    RSA *rsa; // RSAのパラメータを格納する構造体

    ERR_load_crypto_strings();

    /* RSA 鍵生成　第一引数は鍵長, 第二引数はe,第三、第四引数は乱数生成にかかるものだが、NULLにしておけばopensslが適切に対応してくれる　*/
    rsa = RSA_generate_key(KEYBIT_LEN, 65537, NULL, NULL);
    if ( rsa == NULL ){
        printf("in generate_key: err=%s\n", ERR_error_string(ERR_get_error(), errbuf));
        return 1;
    }

    /* opensslの現在のバージョンでは、下記のように構造体のメンバーにアクセスすることはできなくなった。
     * その変わりとしてRSA_get0_keyを使用する
    BN_PUT(rsa->p);
    BN_PUT(rsa->q);
    BN_PUT(rsa->n);
    BN_PUT(rsa->e);
    BN_PUT(rsa->d);
    */
    BN_PUT(RSA_get0_p(rsa));
    BN_PUT(RSA_get0_q(rsa));
    BN_PUT(RSA_get0_n(rsa));
    BN_PUT(RSA_get0_e(rsa));
    BN_PUT(RSA_get0_d(rsa));
    

    /* 暗号文・復号文の領域を確保 */
    //RSA_sizeはrsa暗号のmodulusのサイズを返す。これにより暗号文のために用意するバッファのサイズが確認できる
    printf("Debug: RSA_size(rsa) = %d\n",RSA_size(rsa));
    crypted_buf = malloc(RSA_size(rsa)); 
    if ( crypted_buf == NULL ){
        perror("malloc");
        goto ERR;
    }
    decrypted_buf = malloc(RSA_size(rsa));
    if ( decrypted_buf == NULL ){
        perror("malloc");
        goto ERR;
    }

    /* (秘密鍵を使用)暗号化 */
    // 引数は　1:平文の長さ, 2: 平文バッファ, 3:暗号化データを格納するバッファ, 4:rsa構造体, 5: パディング方式
    //  PKCS#1 によってパディング方式が定められている。
    crypted_len = RSA_private_encrypt(strlen(plain_buf), plain_buf, crypted_buf, rsa, RSA_PKCS1_PADDING);
    if ( crypted_len == -1 ){
        printf("in encrypt: err=%s\n", ERR_error_string(ERR_get_error(), errbuf));
        goto ERR;
    }

    /* 復号 */
    decrypted_len = RSA_public_decrypt(crypted_len, crypted_buf, decrypted_buf, rsa, RSA_PKCS1_PADDING);
    if ( decrypted_len == -1 ){
        printf("in decrypt: err=%s\n", ERR_error_string(ERR_get_error(), errbuf));
        goto ERR;
    }

    printf("平文=[%s]\n", plain_buf);
    {
        int i;
        printf("暗号文=[0x");
        for ( i=0 ; i<crypted_len ; i++ ){
            printf("%X", crypted_buf[i]);
        }
        printf("] (16進数)\n");
    }
    printf("暗号文の復号結果=[%.*s]\n", decrypted_len, decrypted_buf);

    /*  */
    if ( strncmp(plain_buf, decrypted_buf, decrypted_len) != 0 ){
        printf("一致しません\n");
        goto ERR;
    }

    RSA_free(rsa); // RSA構造体で確保したメモリを解放
    return 0;

 ERR:
    RSA_free(rsa); // RSA構造体で確保したメモリを解放
    return 1;
}
