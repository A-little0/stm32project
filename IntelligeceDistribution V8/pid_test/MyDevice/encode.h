#ifndef __ENCODE_H
#define __ENCODE_H
#include "main.h"

typedef enum{
	ENCODEMA=1,
	ENCODEMB,
	ENCODEMC,
	ENCODEMD
}Encode;

extern int encodeMA;
extern int encodeMB;
extern int encodeMC;
extern int encodeMD;

void Encode_Init(void);
int  Read_MX_Encoder(uint8_t TIMX);
void Clear_Encode(int encodex); 
#endif
