#include <cdefBF532.h>
#include <stdarg.h>

#include "uart.h"
#include "printf.h"

void uart_init(long baudrate){
	unsigned char inchar;

	//	Set line control reg - 8 bits, 1 stop, no parity
	*pUART_LCR = 0x0083;

	//	Set baud rate
	//	  SCLK = 130.5MHz/16 = 8,156,250
	//	  8156250/9600 = 772
	//	  8156250/57600 = 142
	if(0) {
		*pUART_DLL = (8156250/baudrate)%256;
		*pUART_DLH = (8156250/baudrate)/256;
	}
	//	If SCLK is 101.25 then 101.25/16 = 6328125
	else if(0) {
		*pUART_DLL = (6328125/baudrate)%256;
		*pUART_DLH = (6328125/baudrate)/256;
	}
	//	If SCLK is 126 then 126/16 = 787500
	else if(1) {
		*pUART_DLL = (7875000/baudrate)%256;
		*pUART_DLH = (7875000/baudrate)/256;
	}
	//	If SCLK is 94.5 then 94.5/16 = 590625
	else if(0) {
		*pUART_DLL = (5906250/baudrate)%256;
		*pUART_DLH = (5906250/baudrate)/256;
	}

	//	*pUART_DLL = 0x00b2;
	//	*pUART_DLH = 0x0001;

	//	Enable access to RBR,RBR and IER
	*pUART_LCR = 0x0003;

	// Clear Rx buffer register
	inchar = *pUART_RBR;

	// Enable UART
	*pUART_GCTL = UCEN;

}

void uputchar(unsigned char data) {
	while(!(*pUART_LSR&0x0040)) ;
	
	*pUART_THR = data;
}

unsigned char ugetchar_async(void){
	unsigned char inchar;
	
	if(*pUART_LSR&0x0001) {
		inchar = *pUART_RBR;
		return inchar;
	}else{
		return 0;
	}
}

unsigned char ugetchar(void){
	unsigned char inchar;
	while(!(inchar=ugetchar_async()));
	return inchar;
}


int uprintf(const char *fmt, ...){
	
	va_list ap;
	int retval;
	
	va_start(ap, fmt);
	retval = kvprintf(fmt, uputchar, NULL, 10, ap);
	va_end(ap);
	
	return (retval);
}


