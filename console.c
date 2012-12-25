#include "uart.h"

// Parts stolen from LK by Travis Geiselbrecht

int console_tokenize_args(char *inbuf, char **argv){
	enum{
		WHITESPACE,
		TOKEN
	} state;

	unsigned int argc = 0;
	int idx = 0;

	if(inbuf[idx] == ' '){
		state = WHITESPACE;
	}else{
		state = TOKEN;
	}

	for(;;){
		switch(state){
		case WHITESPACE:
			for(;;){
				if(inbuf[idx] == '\0')
					goto out;
				if(inbuf[idx] == ' ')
					inbuf[idx++] = '\0';
				else
					break;
			}

			state = TOKEN;
			break;
		case TOKEN:
			argv[argc++] = &(inbuf[idx]);
			while(inbuf[idx] != ' ' && inbuf[idx] != '\0') idx++;

			if(inbuf[idx] == '\0')
				goto out;

			state = WHITESPACE;
			break;
		}
	}

out:

	return argc;
}


int console_read_line(char *outbuffer){
	unsigned int pos = 0;
	char c;

	for(;;){
		c = ugetchar();

		switch(c){
			case '\r':
			case '\n':
				uputchar('\n');
				goto out;

			case 0x7f: // backspace or delete
			case 0x8:
				if(pos > 0){
					pos--;
					uprintf("\x1b[1D"); // move to the left one
					uputchar(' ');		// overwrite
					uprintf("\x1b[1D"); // move to the left one
				}
				break;

			default:
				outbuffer[pos++] = c;
				uputchar(c);
		}
	}

out:
	outbuffer[pos] = '\0';
	return pos;
}

