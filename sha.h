/*==========================================================================================
||	
||	Copyright (C) 2013  Ravi Malik
||	
||	This program is free software; you can redistribute it and/or
||	modify it under the terms of the GNU General Public License
||	as published by the Free Software Foundation; either version 2
||	of the License, or (at your option) any later version.
||	
||	This program is distributed in the hope that it will be useful,
||	but WITHOUT ANY WARRANTY; without even the implied warranty of
||	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
||	GNU General Public License for more details.
||	
||	You should have received a copy of the GNU General Public License
||	along with this program; if not, write to the Free Software
||	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
||	
||
*==========================================================================================*/

#include<stdio.h>
#include<stdlib.h>

/******************************************

Functional interfaces within the module.
1. void init_hash(struct hash *hs);
2. void compress(struct hash *hs,struct message *ms);

******************************************/

/*############# Raw Data #################*/

unsigned int K[64]={
	     0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
             0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
             0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
             0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
             0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
             0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
             0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
             0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

/*########## Data Structures ############*/

// SHA-2 Hash represented by an array of 32 bit words (unsigned ints) and size 8. Total size 256 bits.
struct hash
{	unsigned int h[8];
};

// Message, i.e. the data whose hash is to be generated. Represented by an array of 32 bit words and size 16. total size 512 bits. 
struct message
{	unsigned int m[16];
};

/*########### Primitive Functions #############*/

char* int_to_hex_string(unsigned int a)
{	int r;
	char *s = malloc(sizeof(char)*9);
	int i=7;	
	while(a!=0)
	{	r=a%16;
		a=a/16;
		if(r>=10)
		{	switch(r)
			{	case 10:
					s[i]='a';
					break;
				case 11:
					s[i]='b';
					break;
				case 12:
					s[i]='c';
					break;
				case 13:
					s[i]='d';
					break;
				case 14:
					s[i]='e';
					break;
				case 15:
					s[i]='f';
					break;
			}
		}
		else
		{	s[i]=(char)(r+48);
		}
		i--;
	}
	s[8]='\0';
	return s;
}

unsigned long file_size_SHA(FILE *fp)
{	fseek(fp,0,SEEK_END);
	unsigned long size=ftell(fp);
	fseek(fp,0,SEEK_SET);
	return size;
}

unsigned int modAdd(int a, int b)
{	long c;
	unsigned int r;	
	c=a+b;
	r=(unsigned int)(c%(4294967296));
	return r;
}

// Right Rotate x by n bits. Works obly for 32 bit words.
unsigned int S(const unsigned int *x,int n)
{	return (*x>>n | *x<<(32-n));
}

unsigned int Ch(const unsigned int *x,const unsigned int *y,const unsigned int *z)
{	return (*x & *y)^(~(*x) & *z);
}

unsigned int Maj(const unsigned int *x,const unsigned int *y,const unsigned int *z)
{	return (*x & *y)^(*x & *z)^(*y & *z);
}

unsigned int Sum0(const unsigned int *x)
{	return S(x,2)^S(x,13)^S(x,22);
}

unsigned int Sum1(const unsigned int *x)
{	return S(x,6)^S(x,11)^S(x,25);
}

unsigned int Sig0(const unsigned int *x)
{	return S(x,7)^S(x,18)^(*x>>3);
}

unsigned int Sig1(const unsigned int *x)
{	return S(x,17)^S(x,19)^(*x>>10);
}

/*########## Functions working on Data ############*/

void print_hash(struct hash *hs)
{	printf("Hash:");
	int i;
	for(i=0;i<8;i++)
	{	printf("%x ",hs->h[i]);
	}
	printf("\n");
}

void print_message(struct message *ms)
{	printf("Message:");	
	int i;
	for(i=0;i<16;i++)
	{	printf("%x ",ms->m[i]);
	}
	printf("\n");
}

void hash_to_hex_string(struct hash *hs, char *str)
{	unsigned int a;			// Temp variable to store hash element
	int i=63;		// Counter to string
	int j;			// Counter for hash	
	int r;			// Digit to be converted to hexadecimal	
	char *temp;
	int k;	
	for(j=7;j>=0;j--)
	{	a=hs->h[j];	
		temp=int_to_hex_string(a);
		for(k=7;k>=0;k--)
		{	str[i]=temp[k];
			i--;
		}
		free(temp);
	}
	str[64]='\0';
}

unsigned char* hash_to_string(struct hash *hs)
{
	unsigned char *str = malloc(sizeof(char)*33);
	int i=0;		// Counter on String
	int j;			// Counter on the hash element which is an int
	int k=0;		// Counter on hash
	while(i<32)
	{	for(j=3;j>=0;j--)
		{	str[i]=(unsigned char)(hs->h[k]>>(8*j));
			i++;	
		}
		k++;
	}
	str[32]='\0';
	return str;
}

// Initialise the hash to be computed.
void init_hash(struct hash *hs)
{	hs->h[0]=0x6a09e667; 
	hs->h[1]=0xbb67ae85; 
	hs->h[2]=0x3c6ef372; 
	hs->h[3]=0xa54ff53a; 
	hs->h[4]=0x510e527f; 
	hs->h[5]=0x9b05688c; 
	hs->h[6]=0x1f83d9ab; 
	hs->h[7]=0x5be0cd19;
}

// SHA-2 256 bit compression function + round hash evaluator.
void compress(struct hash *hs, struct message *ms)
{	unsigned int a=hs->h[0],b=hs->h[1],c=hs->h[2],d=hs->h[3],e=hs->h[4],f=hs->h[5],g=hs->h[6],h=hs->h[7];
	unsigned int W[64];
	int i;
	unsigned int T1,T2; 
	for(i=0;i<16;i++)
	{	W[i]=ms->m[i];
	}
	for(i=16;i<64;i++)
	{	W[i]=modAdd(Sig1(&W[i-2]),modAdd(W[i-7],modAdd(Sig0(&W[i-15]),W[i-16])));
	}
	for(i=0;i<64;i++)
	{	T1=modAdd(h,modAdd(Sum1(&e),modAdd(Ch(&e,&f,&g),modAdd(K[i],W[i]))));
		T2=modAdd(Sum0(&a),Maj(&a,&b,&c));
		h=g;
		g=f;
		f=e;
		e=modAdd(d,T1);
		d=c;
		c=b;
		b=a;
		a=modAdd(T1,T2);
	}
	hs->h[0]=modAdd(a,hs->h[0]);
	hs->h[1]=modAdd(b,hs->h[1]);
	hs->h[2]=modAdd(c,hs->h[2]);
	hs->h[3]=modAdd(d,hs->h[3]);
	hs->h[4]=modAdd(e,hs->h[4]);
	hs->h[5]=modAdd(f,hs->h[5]);
	hs->h[6]=modAdd(g,hs->h[6]);
	hs->h[7]=modAdd(h,hs->h[7]);
}


