#include<stdio.h>
#include<stdlib.h>

int main(void){

    FILE *fout;
    char binStr[] = "\x01\x02\x03\x04\xff\xfe\xfd\xfc";

    if (!(fout = fopen("temp.bin","wb")))  {
	fprintf(stderr, "error\n");
	exit(1);
    }
    fwrite(binStr, 1,8,fout);

    fclose(fout);
    printf("temp.binに書き込み完了\n");
      
    return 0;
}
