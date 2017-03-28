#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char *optarg;
int optind;
int qflag = 0;
int rflag = 0;
int g_argc;
int* g_pids;
int size;
char** g_argv;

/*
 * Checks if fileName is a directory
 * 
 * @param fileName name of the file
 * @return 1 if fileName is a directory, 0 otherwise.
 *
 */
int isDirectory(char* fileName){
	struct stat st;

	if (stat(fileName, &st) != 0) {
		return 0;
	}

	if (S_ISDIR(st.st_mode))
	{
		return 1;
	}else{
		return 0;
	}
}

/*
 * Checks if file exists
 *
 * @param fileName name of the file
 * @return 1 if its exists, 0 otherwise
 *
 */
 
int checkExistence(char* fileName){
	struct stat st;	

	if (stat(fileName, &st) == 0) {
		return 1;
	}

	return 0;
}

/*
 * Checks if there are arguments problems
 *	
 * @return 0 if there are no problems, 1 otherwise.
 * 
 */
int checkArguments(){
	int c,err = 0;
	int response;

	while ((c = getopt(g_argc, g_argv, "qr")) != -1){
		switch (c) { 
			case 'q': 
				qflag = 1;
				break;
			case 'r': 
				rflag = 1;
				break;
			case '?': 
				err = 1; 
				break;
		}
	}

	if((optind+2) > g_argc){

		fprintf(stderr, "At least two arguments must be introduced.\n");
		return -1;

	}else if (err){

		fprintf(stderr, "Given options are invalid..\n");
		return -1;
	}

	if (!checkExistence(g_argv[g_argc-1]))
	{
		fprintf(stderr, "Given directory doesn't exist. [%s]\n", g_argv[g_argc-1]);
		return -1;
	}


	if (!isDirectory(g_argv[g_argc-1]))
	{
		fprintf(stderr, "Last argument is not a directory. [%s]\n", g_argv[g_argc-1]);
		return -1;
	}
	return 0;
}

/*
 * Asks user to retry copy or not
 * 
 * @param fileName name of the file.
 * @return 1 retry ,0 otherwise.
 *
 */
int proceed(char* fileName){
	char response;

	do{
		printf("¿Desea volver a intentar la copia de \"%s\"?[S/N]: ",fileName);
		response = toupper(getchar());
		getchar(); //consume "ENTER"

	}while((response != 'S') && (response != 'N'));

	if (response == 'N'){
		return 0;
	}

	return 1;
}


/*
 * Creates a process to execute requested copy
 * 
 * @param i position of the son in pids array
 *
 */ 
void executeCopy(int i){
	int son;
	if (son = fork())
	{
		g_pids[i] = son;
	}else{
		if(qflag) freopen("/dev/null","w",stderr);

		execlp("cp","cp",g_argv[optind+i], g_argv[g_argc-1],NULL);
		perror("Error: ");
	}
}

/*
 *	Retry failed copies
 */
void waitAndRetry(){
	int i = 0;
	for (; i < size; i++){
		int status;
		waitpid(g_pids[i],&status,0);

		if (WEXITSTATUS(status) && proceed(g_argv[optind+i]))
		{
			executeCopy(i);
			i--;
		}
	}
}

/*
 *	Wait for each son
 */
void waitAndStop(){
	int i = 0;
	for (; i < size; i++){
		int status;
		waitpid(g_pids[i],&status,0);
	}
}

void waitChildren(){
	if (rflag)
	{
		waitAndRetry();
	}else{
		waitAndStop();
	}
}

int main(int argc, char* argv[]){
	g_argc = argc;
	g_argv = argv;

	if (checkArguments()) return;
	
	size = argc-optind-1;
	int pids[size];
	g_pids = pids;

	int i = 0;
	for (; i < size; i++) executeCopy(i);
		
	waitChildren();
}