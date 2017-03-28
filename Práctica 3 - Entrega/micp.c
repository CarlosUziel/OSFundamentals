#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

char* getFileName(char* fileName);
char *optarg;
int optind;
int fflag;
char* directoryPath;


/*
 * Programa principal: obtiene los ficheros a copiar de la lista de argumentos, comprueba si existen errores en ellos
 * y en el caso de ausencia de los mismos procede a su copia a bajo nivel.
 *
 * @param argc: número de argumentos.
 * @param argv[]: lista de argumentos.
 * return 1 si termina con éxito, 0 en caso contrario.
 *
 */
main(int argc,char* argv[]){
	optind = 0;
	fflag = 0;

	if (checkArguments(argc,argv) == -1)
	{
		printf("Programa detenido.\n");
		return(EXIT_FAILURE);
	}
	
	directoryPath = malloc(strlen(argv[argc-1]) + 2);
	strcat(directoryPath,argv[argc-1]);
	strcat(directoryPath,"/");

	for (; optind < argc-1; optind++)
	{
		executeCopy(argv[optind]);
	}
	
	free(directoryPath);

	return(EXIT_SUCCESS);
}

/* 
 * Crea una copia a partir de un fichero fuente en otro fichero destino.
 * 
 * @param fileName: nombre del fichero a copiar.
 * @return 0  si la copia ha finalizado con éxito.  
 * 
 */
int executeCopy(char* fileName){
	char* newFileName = malloc(strlen(directoryPath) + strlen(fileName) + 1);
	strcpy(newFileName,directoryPath);
	char* newFileName_1 = getFileName(fileName);
	strcat(newFileName,newFileName_1);

	if (fileEquals(fileName,newFileName))
	{
		fprintf(stderr,"El fichero de origen y destino es el mismo [%s]\n", fileName);
		return -1;
	}

	int originFile = executeIO(fileName,0,0);

	if (originFile == -1)
	{
		printf("Copia de [%s] interrumpida.\n", fileName);
		return -1;
	}

	int newFile = executeIO(newFileName,1,getPermissions(fileName));

	if (newFile == -1)
	{
		printf("Copia de [%s] interrumpida.\n", fileName);
		return -1;
	}

	copyFile(originFile,newFile,100);

	close(originFile);
	close(newFile);

	free(newFileName);
	free(newFileName_1);
	return 0;
}

/* 
 * Ejecuta la copia del contenido de un fichero en otro mediante buffers.
 * 
 * @param originFile: descriptor de archivo del fichero a copiar.
 * @param newFile: descriptor de archivo del fichero destino.
 * @param bufferSize: tamaño del buffer.
 * @return 0. 
 * 
 */
int copyFile(int originFile, int newFile, int bufferSize){
	void* buffer = malloc(bufferSize);
	int bytesRead;

	do{
		bytesRead = read(originFile,buffer,bufferSize);
		write(newFile,buffer,bytesRead);

	}while(bytesRead == bufferSize);

	free(buffer);

	return 0;
}

/* 
 * Abre un fichero indicando el modo de apertura.
 * 
 * @param fileName: nombre del fichero a abrir.
 * @param mode: modo de apertura.
 * @return descriptor si el fichero se ha abierto, y -1 en caso contrario.
 * 
 */
int openFile(char* fileName, int mode){
	return open(fileName,mode);
}

/* 
 * Crea un fichero especificando los permisos.
 * 
 * @param fileName: nombre del fichero a crear.
 * @param permissions: permisos del nuevo fichero.
 * @return descriptor si el fichero se ha abierto, y -1 en caso contrario.
 * 
 */
int createFile(char* fileName, int permissions){
	return creat(fileName, permissions);
}

/*
 * Lee o crea un fichero. 
 *	
 * @param fileName: fichero a leer o crear.
 * @param operation: 0 -> leer , 1 -> crear.
 * @param permissions: permisos concedidos al fichero en cuestión.
 * @return descriptor si el fichero se ha leído o creado correctamente, y -1 en caso contrario.
 * 
 */
int executeIO(char* fileName, int operation, int permissions){
	int fileDescriptor;
	char* mode;

	if (operation)
	{

		if(checkExistence(fileName)){
			if (!askOverwriting(fileName))
			{
				return -1;
			}

			chmod(fileName, permissions);
			fileDescriptor = openFile(fileName, O_WRONLY | O_TRUNC);

		}else{
			
			errno = 0;
			fileDescriptor = createFile(fileName, permissions);
			mode = "crear";
		}

	}else{

		if (isDirectory(fileName))
		{
			fprintf(stderr,"Error. La ruta especificada, \"%s\", es un directorio.\n",fileName);
			close(fileDescriptor);
			return -1;
		}
		errno = 0;
		fileDescriptor = openFile(fileName,O_RDONLY);
		mode = "abrir";
	}
	
	if (errno != 0)
	{
		fprintf(stderr,"Error al %s el archivo \"%s\".\n",mode,fileName);
		perror("Cause of failure");
		errno = 0;
		return -1;
	}

	return fileDescriptor;
}

/*
 * Comprueba si los argumentos cumplen las especificaciones.
 *	
 * @param argc: número de argumentos.
 * @param argv[]: lista de argumentos.
 * @return 0 si los argumentos son correctos y -1 en caso contrario.
 * 
 */
int checkArguments(int argc,char* argv[]){
	int c,err = 0;
	int response;

	while ((c = getopt(argc, argv, "f")) != -1){
		switch (c) { 
			case 'f': 
				fflag = 1;
				break;
			case '?': 
				err = 1; 
				break;
		}
	}

	if((optind+2) > argc){

		fprintf(stderr, "No se han introducido los argumentos necesarios.\n");
		return -1;

	}else if (err){

		fprintf(stderr, "Error de opciones.\n");
		return -1;
	}

	if (!checkExistence(argv[argc-1]))
	{
		fprintf(stderr, "El directorio no existe. [%s]\n", argv[argc-1]);
		return -1;
	}


	if (!isDirectory(argv[argc-1]))
	{
		fprintf(stderr, "El último argumento no es un directorio. [%s]\n", argv[argc-1]);
		return -1;
	}
	return 0;
}

/*
 * Comprueba si dos ficheros son iguales
 *	
 * @param fileName_0 : fichero a comparar.
 * @param fileName_1 : fichero a comparar.
 * @return 1 si son iguales y 0 en caso contrario
 * 
 */
int fileEquals(char* fileName_0, char* fileName_1){
	struct stat st_0;
	struct stat st_1;

	if (stat(fileName_0, &st_0) != 0) {
		return 0;
	}

	if (stat(fileName_1, &st_1) != 0) {
		return 0;
	}

	if ((int)st_0.st_ino == (int)st_1.st_ino)
	{		
		if ((int)st_0.st_dev == (int)st_1.st_dev)
		{
			return 1;
		}
	}
	return 0;
}

/*
 * Obtiene los permisos de un fichero para asignárselo a otro
 * 
 * @param fileName: nombre del fichero que contiene los permisos a copiar.
 * @return los permisos del fichero pasado como parámetro.
 *
 */
int getPermissions(char* fileName){
	struct stat st;

	if (stat(fileName, &st) != 0) {
		return -1;
	}

	return (st.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO));
}

/*
 * Comprueba si el nombre de fichero representa a un directorio.
 * 
 * @param fileName: fichero a comprobar
 * @return 1 si es un directorio y 0 en caso contrario.
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
 * Comprueba si el fichero existe. 
 * 
 * @param fileName: nombre del fichero.
 * @return 1 si si existe y 0 en caso contrario.
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
 * En caso de que el fichero exista, se encarga de preguntar al usuario si desea sobrescribirlo.
 * (Si en los argumentos hay un "-f" sobrescribe directamente).
 * 
 * @param fileName: nombre del fichero.
 * @return 1 si sobrescribe y 0 en caso contrario.
 *
 */
int askOverwriting(char* fileName){
	if (fflag == 1)
	{
		return 1;
	}

	char response;

	do{
		printf("¿Desea sobrescribir \"%s\"?[S/N]: ",fileName);
		response = toupper(getchar());
		getchar(); //consume "ENTER"

	}while((response != 'S') && (response != 'N'));

	if (response == 'N'){
		return 0;
	}

	return 1;
}

/*
 * Obtiene el nombre de un fichero, obviando el directorio.
 *  
 * @param fileName: nombre o ruta del fichero.
 * @return nombre del fichero.
 *
 */
char* getFileName(char* fileName){
	int i = strlen(fileName) - 1;

	while((i > 0) && (fileName[i] != '/')){
		i--;
	}

	char* newFileName = malloc(strlen(fileName) - i);
	strcpy(newFileName,fileName+i);

	return newFileName;
}
