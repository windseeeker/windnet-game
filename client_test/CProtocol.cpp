#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "CProtocol.h"

//#include "CCStdC.h"
#define MIN(a,b)            (((a) < (b)) ? (a) : (b))
#pragma pack(1)  
typedef struct tag_protocol_header
{
	int sync;
	unsigned short version;
	unsigned int size;
}protocol_header;
#pragma pack()
unsigned char *CProtocol::packHeader(unsigned char *buff,unsigned int size,unsigned short version,unsigned int *d_size)
{
	protocol_header header;
	int srand=0;
	int header_length=sizeof(protocol_header);
	memcpy((void*)&(header.sync),(const void*)"xxmp",4);
	header.version=version;
	header.size=size;
	srand=header.sync%version*size;
	*d_size=size+header_length;
	unsigned char *new_buff=(unsigned char *)malloc(*d_size);
	unsigned char *p_new_buff;
	p_new_buff=new_buff;
	memcpy(p_new_buff,&header,header_length);
	p_new_buff+=header_length;
	unsigned int count=0;
	while(count<size)
	{
		char a=MIN(4,size-count);
		volatile int v=0;
		memcpy((void*)&v,(buff+count),a);
		v^=srand;
		memcpy((void*)p_new_buff,(const void*)&v,a);
		p_new_buff+=a;
		count+=a;
	}
	return new_buff;
}

unsigned char *CProtocol::unpackHeader(unsigned char *buff,unsigned int *d_size)
{
	protocol_header header;
	int srand=0;
	int header_length=sizeof(protocol_header);
	memcpy((void*)&header,buff,header_length);
	srand=header.sync%header.version*header.size;
	*d_size=header.size;
	buff+=header_length;
	unsigned char *new_buff=(unsigned char *)malloc(header.size);
	unsigned char *p_new_buff;
	p_new_buff=new_buff;
	unsigned int count=0;
	while(count<header.size)
	{
		char a=MIN(4,header.size-count);
		volatile int v=0;
		memcpy((void*)(&v),(buff+count),a);
		v^=srand;
		memcpy((void*)p_new_buff,(const void*)&v,a);
		p_new_buff+=a;
		count+=a;
	}
	return new_buff;
}