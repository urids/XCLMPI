#include "bufferFunctions.h"
#define DEBUG 0

int numEntitiesInHost;
float entitiesPerTask;
int* DbufferSize; //Device BufferSize

int initEntitiesBuffer(int numTasks, int HbufferSize,int MPIentityTypeSize) {
	int status;
	int i,j;//index variables

	numEntitiesInHost = (int) HbufferSize / MPIentityTypeSize;
	entitiesPerTask = (float) numEntitiesInHost / (float) numTasks;
	DbufferSize = (int*) malloc(numTasks * sizeof(int)); //Saves the Device BufferSize


	for (i = 0; i < numTasks; i++) {
		if(DbufferSize<=taskList[i].device->maxAllocMemSize){
			printf("enough Space =) %d in Host  %d Max Alloc in Device %d Max Device Space \n",HbufferSize,taskList[i].device->maxAllocMemSize, taskList[i].device->globalMemSize);
		}
	}


	for(j=0;j<numTasks;j++){
		(j==(numTasks-1))?(DbufferSize[j]=(int)floorf(entitiesPerTask)*MPIentityTypeSize):(DbufferSize[j]=(int)ceilf(entitiesPerTask)*MPIentityTypeSize);
	}


	for (i = 0; i < numTasks; i++) {
		taskList[i].entitiesbuffer = malloc(sizeof(XCLbuffer));
		taskList[i].device[0].memHandler=malloc(sizeof(cl_mem));

			taskList[i].entitiesbuffer->size  = DbufferSize[i];
			taskList[i].entitiesbuffer->bufferMode = CL_MEM_READ_WRITE;

			taskList[i].device[0].memHandler[0] = clCreateBuffer(
					taskList[i].device[0].context,
					taskList[i].entitiesbuffer->bufferMode,
					taskList[i].entitiesbuffer->size, NULL, &status);


		chkerr(status, "Error at: Creating mem Buffers", __FILE__, __LINE__);
		debug_print("allocated: %zd \n" ,taskList[i].entitiesbuffer->size);
	}

	return status;
}
