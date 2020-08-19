#include <stdlib.h>
#include <stdio.h>

#define TS_RL_BUFSIZE 1024

int ts_execute(char **args){
	//TODO
}

char **ts_split_line(char *line){
	//TODO
}

char *ts_read_line(void){
	int bufsize = TS_RL_BUFSIZE;
	int position = 0;
	char *buffer = malloc(sizeof(char) * bufsize);
	int c = 0;

	if(!buffer){
		fprintf(stderr, "ts: allocation error\n");
		exit(EXIT_FAILURE);
	}

	while(1){
		// Read a character
		c = getchar();

		// If we hit EOF, replace it with a null character and return
		if(c == EOF || c == '\n'){
			buffer[position] = '\0';
			return buffer;
		}
		else{
			buffer[position] = c;
		}

		position++;

		// If we have exceeded the buffer, reallocate
		if(position >= bufsize){
			bufsize += TS_RL_BUFSIZE;
			buffer = realloc(buffer, bufsize);
			if(!buffer){
				fprintf(stderr, "ts: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}
	}
}

void ts_loop(){
  char *line;
	char **args;
	int status;

	do{
		printf(">");
		line = ts_read_line();
		args = ts_split_line(line);
		status = ts_execute(args);

		free(line);
		free(args);
	}while(status);
}

int main(int argc, char **argv){
  // load config files

  // command loop
  ts_loop();
  // cleanup

  return EXIT_SUCCESS;
}
