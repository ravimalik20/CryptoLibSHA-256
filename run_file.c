#include<stdio.h>
#include"hash_file.h"

void main(int argc ,char *argv[])
{	FILE *fp;
	fp=fopen(argv[1],"rb");
	struct hash hs=hash_file(fp);
	print_hash(&hs);
	printf("%lu",ftell(fp));
	fclose(fp);
}
