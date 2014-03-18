#ifndef CPROTOCOL_H__
#define CPROTOCOL_H__

  
	class CProtocol
	{
	public:
		static unsigned char *packHeader(unsigned char *buff,unsigned int size,unsigned short version,unsigned int *d_size);
		static unsigned char *unpackHeader(unsigned char *buff,unsigned int *d_size); 
	};
#endif