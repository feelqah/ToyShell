#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define TS_RL_BUFSIZE 1024
#define TS_TOK_BUFSIZE 64
#define TS_TOK_DELIM " \t\r\n\a"

/* Function declarations for builtin shell commands */
int ts_cd(char **args);
int ts_help(char **args);
int ts_exit(char **args);

/* List of builtin commands, and their functions */
char *builtin_str[] = {
	"cd",
	"help",
	"exit"
};

int (*builtin_func[]) (char **) = {
	&ts_cd,
	&ts_help,
	&ts_exit
};

int ts_num_builtins(){
	return sizeof(builtin_str) / sizeof(char *);
}

/* Builtin function implementations  */
int ts_cd(char **args){
	if(args[1] == NULL){
		fprintf(stderr, "ts: expected argument to \"cd\"\n");
	}
	else{
		if(chdir(args[1] != 0)){
			perror("ts");
		}
	}
	return 1;
}

int ts_help(char **args){
	int i;
	printf("Toy Shell\n");
	printf("Enter program names and arguments and hit enter\n");
	printf("Following commands are builtin:\n");

	for(i=0;i<ts_num_builtins();i++){
		printf(" %s\n", builtin_str[i]);
	}

	printf("Use the man command for information on other programs\n");
	return 1;
}

int ts_exit(char **args){
	return 0;
}

int ts_launch(char **args){
	pid_t pid, wpid;
	int status;

	pid = fork();

	if(pid == 0){
		// Child process
		if(execvp(args[0], args) == -1){
			perror("ts");
		}
		exit(EXIT_FAILURE);
	}
	else if(pid < 0){
		// Error forking
		perror("ts");
	}
	else{
		// Parent process
		do{
			wpid = waitpid(pid, &status, WUNTRACED);
		}while(!WIFEXITED(status) && !WIFSIGNALED(status));
	}

	return 1;
}

int ts_execute(char **args){
	int i;

	if(args[0] == NULL){
		// empty command
		return 1;
	}

	for(i=0;i<ts_num_builtins();i++){
		if(strcmp(args[0], builtin_str[i]) == 0){
			return (*builtin_func[i])(args);
		}
	}

	return ts_launch(args);
}

char **ts_split_line(char *line){
	int bufsize = TS_TOK_BUFSIZE;
	int position = 0;
	char **tokens = malloc(bufsize * sizeof(char*));
	char *token;

	if(!tokens){
		fprintf(stderr, "ts: allocation error\n");
		exit(EXIT_FAILURE);
	}

	token = strtok(line, TS_TOK_DELIM);
	while(token != NULL){
		tokens[position] = token;
		position++;

		if(position >= bufsize){
			bufsize += TS_TOK_BUFSIZE;
			tokens = realloc(tokens, bufsize * sizeof(char*));

			if(!tokens){
				fprintf(stderr, "ts: allocation error\n");
				exit(EXIT_FAILURE);
			}
		}

		token = strtok(NULL, TS_TOK_DELIM);
	}
	tokens[position] = NULL;
	return tokens;
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
