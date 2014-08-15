#ifdef __GNUC__

	typedef unsigned int uint32;
	typedef unsigned short int uint16;
	typedef unsigned char uint8;
	typedef int int32;
	typedef short int int16;
	typedef char int8 ;
	
#else	

	typedef unsigned long uint32;
	typedef unsigned short uint16;
	typedef unsigned char uint8;
	typedef long int32;
	typedef short int16;
	typedef char int8;

#endif