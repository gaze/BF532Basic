#include <cdefBF532.h>
#include <stdio.h>

#define PROMPT ".oO "

void help(){
	uprintf("Commands:\n");
	uprintf("help\tPrint this message\n");
	uprintf("shelltest\tPrint out argc and argv to this command\n");
}

void shelltest(int argc, char **argv){
	int i;
	uprintf("argc: %i\n", argc);
	for(i=0;i<argc;i++){
		uprintf("%i: %s\n", i, argv[i]);
	}
}

int main(){
	uart_init(9600);

	char uart_buffer[255];

	uprintf("StarDrive Monitor 0.01\nCopyright 2011 Evan Zalys\n");
	help();

	while(1){
		int i, chrs;
		char *argv[16];
		int argc;
		
		uprintf(PROMPT);

		chrs = console_read_line(uart_buffer);
		if(chrs == 0){
			continue;
		}
		
		argc = console_tokenize_args(uart_buffer, argv);

		if(strcmp(argv[0], "help") == 0)
			help();
		else if(strcmp(argv[0], "shelltest") == 0)
			shelltest(argc, argv);
		else
			uprintf("Unrecognized command\n");
	}
}
