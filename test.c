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

/*
This test program accepts the name of the file to hash as a command line argument. Then you can check the computed hash with other libraries.
*/

#include<stdio.h>
#include"hash_file.h"

void main(int argc ,char *argv[])
{	FILE *fp;
	fp=fopen(argv[1],"rb");
	struct hash hs=hash_file(fp);
	print_hash(&hs);
	fclose(fp);
}
