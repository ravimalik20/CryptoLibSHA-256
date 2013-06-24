#include<stdio.h>
#include"sha.h"

/* Implemenatation Help Comments....

	1. File size multiple of 64
	2. File size not multiple of 64
		2.1 File size > 64
			It can be broken as File size multiple of 64 and then File size <64
		2.2 File size < 64 (Working Fine)

	Hence there id only 2 cases to consider:
		1. File Size multiple of 64
		2. File size < 64 

*/

/*************************************************************************
*	Functional interface provided by this module for other programs:
*	1. struct hash hash_file(FILE *fp);
**************************************************************************/

/*****************************************************************************************
*	Functional interfaces for use within this module:
*	1. void hash_64(struct hash *hs, FILE *fp);
*	2. void hash_lt_64(struct hash *hs, int size, FILE *fp, unsigned long file_size);
*******************************************************************************************/

void hash_64(struct hash *hs,FILE *fp)
{	// Function to compute hash out of 64 byte message block.
	int i,j;
	unsigned char c;
	struct message ms;	
	for(i=0;i<16;i++)
	{	//fread(&ms.m[i],sizeof(ms.m[i]),1,fp);
		for(j=0;j<4;j++)
		{	fread(&c,sizeof(c),1,fp);	
			ms.m[i]=(ms.m[i]<<8)|(c);
		}
	}
	compress(hs,&ms);
}

void hash_lt_64(struct hash *hs,int size,FILE *fp,unsigned long file_sz)
{	// Function to compute hash out message less than 64 bytes.
	// Here the size represents the remainder of the message left to hash and file_sz represents the size of the file being hashed(both in bytes). 
	struct message ms;	
	int i=0;
	int j,k;
	unsigned char c;	
	int a,b;
	a=size/4;
	b=size%4;
	for(j=0;j<a;j++)
	{	//fread(&ms.m[i],sizeof(ms.m[i]),1,fp);
		for(k=0;k<4;k++)
		{	fread(&c,sizeof(c),1,fp);	
			ms.m[i]=(ms.m[i]<<8)|(c);
		}
		i++;
	}
	if(i>=14)
	{	unsigned char tm;	
		for(j=0;j<b;j++)
		{	fread(&tm,sizeof(tm),1,fp);
			ms.m[i]=ms.m[i]<<8;
			ms.m[i]=ms.m[i]|tm;
			printf("%x ",ms.m[i]);
		}
		if(b==3)
		{	ms.m[i]=(ms.m[i]<<8)|(0x80);
		}
		else if(b==2)
		{	ms.m[i]=(ms.m[i]<<16)|(0x8000);
		}
		else if(b==1)
		{	ms.m[i]=(ms.m[i]<<24)|(0x800000);
		}
		else if(b==0)
		{	ms.m[i]=0x80000000;
		}
		i++;
		for(;i<16;i++)
		{	ms.m[i]=0x00;
		}
		compress(hs,&ms);
		for(j=0;j<14;j++)
		{	ms.m[j]=0x00;
		}
		file_sz=file_sz*8;
		ms.m[14]=(unsigned int)(file_sz>>32);
		ms.m[15]=(unsigned int)(file_sz);
		compress(hs,&ms);
	}
	else
	{	unsigned char tm;	
		for(j=0;j<b;j++)
		{	fread(&tm,sizeof(tm),1,fp);
			ms.m[i]=ms.m[i]<<8;
			ms.m[i]=ms.m[i]|tm;
		}
		if(b==3)
		{	ms.m[i]=(ms.m[i]<<8)|(0x80);
		}
		else if(b==2)
		{	ms.m[i]=(ms.m[i]<<16)|(0x8000);
		}
		else if(b==1)
		{	ms.m[i]=(ms.m[i]<<24)|(0x800000);
		}
		else if(b==0)
		{	ms.m[i]=0x80000000;
		}
		i++;
		for(;i<14;i++)
		{	ms.m[i]=0x00;
		}
		file_sz=file_sz*8;
		ms.m[14]=(unsigned int)(file_sz>>32);
		ms.m[15]=(unsigned int)(file_sz);
		compress(hs,&ms);	
	}
}

struct hash hash_file(FILE *fp)
{	struct hash hs;
	init_hash(&hs);
	struct message ms;	
	unsigned long size=file_size(fp);
	int a,b;
	int i,j;
	a=size/64;	// Number of 64 byte blocks fetchable
	b=size%64;	// Remainder of the Message left after fetching 64 bit blocks or the length of the message itself.
	if(a!=0 && b==0)
	{	//When length of the file is a multiple of 64 bytes.	
		for(i=0;i<a;i++)
		{	hash_64(&hs,fp);
		}
		ms.m[0]=0x80000000;
		for(i=1;i<14;i++)
		{	ms.m[i]=0x00;
		}
		size=size*8;
		ms.m[14]=(unsigned int)(size>>32);
		ms.m[15]=(unsigned int)(size);
		compress(&hs,&ms);
		return hs;
	}
	else if(a!=0 && b!=0)
	{	// When the file length is > 64 bytes and not a multiple of 64 bytes.	
		for(j=0;j<a;j++)
		{	hash_64(&hs,fp);
		}	
		hash_lt_64(&hs,b,fp,size);
		return hs;
	}
	else if(a==0 && b!=0)
	{	// When length of the file is < 64.	
		hash_lt_64(&hs,b,fp,size);
		return hs;
	}
	else if(a==0 && b==0)
	{	// When length of the file is 0.	
		ms.m[0]=0x80000000;
		for(j=1;j<16;j++)
		{	ms.m[j]=0x00;
		}
		compress(&hs,&ms);
		return hs;
	}
}
