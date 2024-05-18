#ifndef _outputdata_h
#define _outputdata_h

extern float OutData[4];
unsigned short CRC_CHECK(unsigned char* Buf, unsigned char CRC_CNT);

void output_Data(void);

#endif
