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

#include"sha.h"
#include<stdio.h>
#include<string.h>

/*
This file will accept a string and convert that string to the message object and operate the hash on it.
This is also similar to the case in which we hashed the file.
*/

void hash_64(struct hash *hs,char *s,int *index,int a)
{	struct message ms;	
	int j;
	int *i=index;	// Index of the String		
	int z=a;	
	for(z=1;z<=a;z++)		
	{	int b=0;
		int m=0;
		for(;*i<(64*z);*i=*i+1)
		{	if(b==4)
			{	b=0;
				m++;
			}
			ms.m[m]=(ms.m[m]<<8);
			ms.m[m]=ms.m[m]|s[*i];
			b++;
		}
		compress(hs,&ms);
	}
}

void hash_lt_64(struct hash *hs,char *s,int *index,unsigned long size)
{	struct message ms;	
	int i=0;		// Counter of the message
	int b=0;
	int *j=index;	// Index of the String
	while(s[*j]!=NULL)
	{	if(b==4)
		{	b=0;
			i++;
		}
		ms.m[i]=(ms.m[i]<<8);
		ms.m[i]=(ms.m[i]|s[*j]);
		b++;
		*j=*j+1;
	}
	if(i<14)
	{	if(b==0)
		{	ms.m[i]=0x80000000;
		}
		else if(b==4)
		{	i++;
			ms.m[i]=0x80000000;
		}
		else if(b==1)
		{	ms.m[i]=(ms.m[i]<<24)|(0x800000);
		}
		else if(b==2)
		{	ms.m[i]=(ms.m[i]<<16)|(0x8000);
		}
		else if(b==3)
		{	ms.m[i]=(ms.m[i]<<8)|(0x80);
		}
		i++;
		for(;i<14;i++)
		{	ms.m[i]=0x00;
		}
		size=8*size;
		ms.m[14]=(unsigned int)(size>>32);
		ms.m[15]=(unsigned int)(size);
		compress(hs,&ms);
	} 
	else
	{	if(b==0)
		{	ms.m[i]=0x80000000;
		}
		else if(b==4)
		{	i++;
			ms.m[i]=0x80000000;
		}
		else if(b==1)
		{	ms.m[i]=(ms.m[i]<<24)|(0x800000);
		}
		else if(b==2)
		{	ms.m[i]=(ms.m[i]<<16)|(0x8000);
		}
		else if(b==3)
		{	ms.m[i]=(ms.m[i]<<24)|(0x80);
		}
		i++;
		for(;i<16;i++)
		{	ms.m[i]=0x00;
		}
		compress(hs,&ms);
		for(i=0;i<14;i++)
		{	ms.m[i]=0x00;
		}
		ms.m[14]=(unsigned int)(size>>32);
		ms.m[15]=(unsigned int)(size);
		compress(hs,&ms);
	}
}

struct hash hash_string(char *s)
{	unsigned long size=strlen(s);
	int a,b;
	a=(int)(size/64);
	b=(int)(size%64);
	struct hash hs;
	init_hash(&hs);
	if(a!=0 && b==0)
	{	struct message ms;	
		int j;
		int i=0;	// Index of the String		
		hash_64(&hs,s,&i,a);
		ms.m[0]=0x80000000;
		for(j=1;j<14;j++)
		{	ms.m[j]=0x00;
		}
		size=8*size;
		ms.m[14]=(unsigned int)(size>>32);
		ms.m[15]=(unsigned int)(size);
		compress(&hs,&ms);
		return hs;
	}
	else if(a==0 && b!=0)
	{	int ind=0;	
		hash_lt_64(&hs,s,&ind,size);	
		return hs;	
	}
	else if(a!=0 && b!=0)
	{	int i;
		int index=0;
		hash_64(&hs,s,&index,a);
		hash_lt_64(&hs,s,&index,size);
		return hs;	
	}
	else if(a==0 && b==0)
	{	struct message ms;	
		int i;
		ms.m[0]=0x80000000;
		for(i=1;i<16;i++)
		{	ms.m[i]=0x00;
		}
		compress(&hs,&ms);
		return hs;
	}
}
