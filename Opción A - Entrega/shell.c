#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/**
 *	Returns input from stdin
 */
char* getInput(){
	fflush(stdin);
	char* input = NULL;
	size_t size;

	getline(&input,&size,stdin);

	return input;
}

/**
 *	Returns an array compound of each word of an input string
 *	@param input input string
 */
char** toStringArray(char* input){
	char* p = strtok(input, " ");
	char** res = NULL;
	int n_spaces = 0,i;

	while (p) {
		res = realloc (res, sizeof (char*) * ++n_spaces);

		if (res == NULL)
			exit (-1);

		res[n_spaces-1] = p;

		p = strtok (NULL, " ");
	}

	res = realloc (res, sizeof (char*) * (n_spaces+1));
	res[n_spaces] = 0;

	return res;
}

/**
 *	Returns a string with the last character removed
 *	@param input string
 */
void removeLastChar(char* input){
	input[strlen(input)-1] = '\0';
}

/**
 *	Returns a string with the first character removed
 *	@param input string
 */
char* removeFirstChar(char* string){
	return string+1;
}

/**
 *	Checks if input string is empty
 *	@param input string
 */
int isEmpty(char* input){
	removeLastChar(input);
	return strlen(input) == 0;
}

/**
 *	Checks if command executes in background
 *	@param input string representing a command
 */
int checkBackgroundProcess(char* input){
	return input[strlen(input)-1] == '&';
}

/**
 * 	Shifts every element of an array one position to the left,
 *	starting from i.
 *	@param res array
 *	@param i position
 */
void shiftLeftArray(char** res,int i){
	for (; res[i] != '\0'; i++)
		res[i] = res[i+1];
}

/**
 *	Locate the string preceded by token.
 *	@param res String array
 *	@param token token char
 */
char* getTokenString(char** res, char token){
	int i;
	for (i = 0; res[i] != '\0'; i++){
		if (res[i][0] == token)
		{
			char* tmp = res[i];
			shiftLeftArray(res,i);
			return removeFirstChar(tmp);
		}
	}

	return 0;
}

/**
 * returns ->
 *   0 - if input is not redirected
 *   !=0 - path of input file
 *	@param res String array (from input)
 */
char* isInputRedirected(char** res){
	return getTokenString(res, '<');
}

/** 
 * returns ->
 *   0 - if input is not redirected
 *   !=0 - path of input file
 *	@param res String array (from input)
 */
char* isOutputRedirected(char** res){
	return getTokenString(res, '>');
}

/*
 * Executes main functionality ->
 * Reads input, creates child and executes read command.
 */
void shell(){

	int background = 0;

	while(1){

		printf("$ ");

		char* input = getInput();

		if (isEmpty(input))
			continue;

		if (!strcmp(input,"exit")) return;

		if (checkBackgroundProcess(input)){
			background = 1;
			removeLastChar(input);
		}

		int son;
		if ((son = fork()))
		{

			if (background){
				background = 0;
				printf("[%d]\n",son);
				continue;
			}

			int status;
			waitpid(son,&status,0);

		}else{

			char** res = toStringArray(input);
			char* path;
			int fd;

			if (path = isInputRedirected(res)){
				freopen(path,"r",stdin);
			}

			if (path = isOutputRedirected(res)){
				freopen(path,"w",stdout);
			}

			execvp(res[0],res);
			perror("Error: ");
		}
		free(input);
	}
}

int main(){
	shell();
}