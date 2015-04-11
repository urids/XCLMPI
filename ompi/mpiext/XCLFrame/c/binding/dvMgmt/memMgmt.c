

#include "dvMgmt.h"

void initEntitiesBuffer(int numtasks,int entitiesbufferSize,int MPIentityTypeSize){

	void * meminitHandle=NULL;
	void (*initEntitiesBuffer)(int,int,int);
	char *error;

	meminitHandle=dlopen("libmultiDeviceMgmt.so",RTLD_LAZY);

	if(!meminitHandle){
		printf("library not found or could not be opened %d, %d" ,__FILE__, __LINE__);
		exit(1);
	}

	initEntitiesBuffer = dlsym(meminitHandle, "initEntitiesBuffer");
	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}

	(*initEntitiesBuffer)(numtasks,entitiesbufferSize,MPIentityTypeSize);

}


void writeEntitiesBuffer(int numtasks,int entitiesbufferSize,int MPIentityTypeSize, void ** entitiesbuffer ){


	void * meminitHandle=NULL;
	void (*writeEntitiesBuffer)(int,int,int,void**);
	char *error;

	meminitHandle=dlopen("libmultiDeviceMgmt.so",RTLD_LAZY);

	if(!meminitHandle){
		printf("library not found or could not be opened %d, %d" ,__FILE__, __LINE__);
		exit(1);
	}

	writeEntitiesBuffer = dlsym(meminitHandle, "writeEntitiesBuffer");
	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}

	(*writeEntitiesBuffer)(numtasks,entitiesbufferSize,MPIentityTypeSize,entitiesbuffer);
}
void readEntitiesBuffer(int numtasks,int entitiesbufferSize,int MPIentityTypeSize, void ** entitiesbuffer){

	void * memreadHandle=NULL;
	void (*readEntitiesBuffer)(int,int,int,void**);
	char *error;

	memreadHandle=dlopen("libmultiDeviceMgmt.so",RTLD_LAZY);

	if(!memreadHandle){
		printf("library not found or could not be opened %d, %d" ,__FILE__, __LINE__);
		exit(1);
	}

	readEntitiesBuffer = dlsym(memreadHandle, "readEntitiesBuffer");
	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}

	(*readEntitiesBuffer)(numtasks,entitiesbufferSize,MPIentityTypeSize,entitiesbuffer);

}

