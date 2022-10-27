// BIGNUMライブラリのテスト
// gcc -o bnTest01 bnTest01.c -lcrypto
#include<stdio.h>
#include<openssl/bn.h>


int main(void){
  BIGNUM *a,*b, *c;
  char *c_str;
  BN_CTX *ctx;
  BN_ULONG w = 1;

  printf("sizeof(int):%lu\n", sizeof(int));
  ctx = BN_CTX_new(); // ライブラリが内部的に使用する
  a = BN_new(); // BIGNUM構造体を割り当てて初期化する(0)
  if (a == NULL) {
    fprintf(stderr, "割当失敗\n");
    exit(1);
  }

  //  unsinged integerとの足し算
  BN_ULONG value1 = 0xdeadbeef;
  BN_add_word(a, value1);
  int size = BN_num_bytes(a);
  printf("size: %d\n", size);
  unsigned char *to = malloc(size);
  BN_bn2bin(a, to); // BN_bn2bin BIGNUM-> ビッグエンディアン形式でtoに書き込み(gdbで確認)
  printf("0x%lu = 0x%s\n", value1, BN_bn2hex(a)); // BN_bn2hex()でBIGNUMを16進数表示

  char hexStr1[] = "000102030405060708090a0b0c0d0e0f101112131415161718191a1b1c1d1e1f";
  c = BN_new();
  printf("16進数文字列%sからBIGNUMを生成する.生成先c:%p\n",hexStr1, c);
  BN_hex2bn(&c, hexStr1);
  
  // 
  b = BN_new();
  if (b == NULL) {
    fprintf(stderr, "割当失敗\n");
    exit(1);
  }

  BN_clear(a); // 引数の内容をクリアする
  BN_free(a); // 割り当てたメモリを解放する
  BN_clear(b); // 引数の内容をクリアする
  BN_free(b); // 割り当てたメモリを解放する
  BN_clear(c);
  BN_free(c);
  BN_CTX_free(ctx);
  
  return 0;
}
