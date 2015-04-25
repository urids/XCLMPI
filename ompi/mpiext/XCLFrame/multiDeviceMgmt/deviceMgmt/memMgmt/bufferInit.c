#include "bufferFunctions.h"
#define DEBUG 0

int numEntitiesInHost;
float entitiesPerTask;
int* DbufferSize; //Device BufferSize global variable declared in bufferFunctions.h

int initNewBuffer(int taskIdx, int trayIdx, int bufferSize ){

	cl_mem * tmpMemHandler=NULL;
	int status;

	int memIdx=taskList[taskIdx].device[0].numcl_memObj; //here we query how many buffers exist in this device

	if (memIdx < trayIdx) { //strictly less to avoid reallocations
		cl_mem * tmpMemHandler;
		tmpMemHandler = realloc(taskList[taskIdx].device[0].memHandler,
				(trayIdx + 1) * sizeof(cl_mem)); //(tray + 1) to avoid dereferencing issues.
		if (tmpMemHandler != NULL){
			taskList[taskIdx].device[0].memHandler = tmpMemHandler;
			taskList[taskIdx].device[0].numcl_memObj=(trayIdx+1); //here we inform to the runtime that this device has increased its number of memObjs
		}else{
			free(taskList[taskIdx].device[0].memHandler);
			printf("Error AT:(re)allocating memory %s ,%s",__FILE__,__LINE__);
			exit(1);
		}
		//free(tmpMemHandler); TODO: is it now ready to free?
	} if (trayIdx==0 && memIdx==0){ //if this is the first tray to be initialized.
		taskList[taskIdx].entitiesbuffer = malloc(sizeof(XCLbuffer));
		taskList[taskIdx].device[0].memHandler = malloc(1 * sizeof(cl_mem)); //init the first device mem space.
		taskList[taskIdx].device[0].numcl_memObj=1;
	}


    taskList[taskIdx].entitiesbuffer->size = bufferSize;
	taskList[taskIdx].entitiesbuffer->bufferMode = CL_MEM_READ_WRITE;

	taskList[taskIdx].device[0].memHandler[trayIdx] = clCreateBuffer(
			taskList[taskIdx].device[0].context,
			CL_MEM_READ_WRITE,
			bufferSize,
			NULL,
			&status);

	chkerr(status, "Error at: Creating new Mem Buffer, %s: %d", __FILE__, __LINE__);

return status;

}

// HbufferSize &  MPIentityTypeSize enables determine the hostBuffer offset for each task
int  ALL_TASKS_initBuffer(int numTasks,int trayIdx, int HbufferSize, int MPIentityTypeSize) {
	int status;
	int i,j;//index variables

	numEntitiesInHost = (int) HbufferSize / MPIentityTypeSize;
	entitiesPerTask = (float) numEntitiesInHost / (float) numTasks;
	DbufferSize = (int*) malloc(numTasks * sizeof(int)); //Saves the Device BufferSize



/*
	for (i = 0; i < numTasks; i++) {
		if(DbufferSize<=taskList[i].device->maxAllocMemSize){
			printf("enough Space =) %d in Host  %d Max Alloc in Device %d Max Device Space \n"
				 ,HbufferSize,taskList[i].device->maxAllocMemSize,
				 taskList[i].device->globalMemSize);
		}
	}
*/

	for(j=0;j<numTasks;j++){ //TODO: make this more readable: This instruction sets the size of the device memory buffer for each task.
		(j==(numTasks-1))?(DbufferSize[j]=(int)floorf(entitiesPerTask)*MPIentityTypeSize):(DbufferSize[j]=(int)ceilf(entitiesPerTask)*MPIentityTypeSize);
	}

//cl_mem* memObjects = malloc(numTasks*sizeof(cl_mem));
	for (i = 0; i < numTasks; i++) {
		int memIdx = taskList[i].device[0].numcl_memObj; //here we query how many buffers exist in this device

		if (memIdx < trayIdx){ //strictly less to avoid reallocations
			cl_mem * tmpMemHandler;
			tmpMemHandler = realloc(taskList[i].device[0].memHandler,
					(trayIdx+1) * sizeof(cl_mem)); //(tray + 1) to avoid dereferencing issues.
			if (tmpMemHandler != NULL){
				taskList[i].device[0].memHandler = tmpMemHandler;
				taskList[i].device[0].numcl_memObj=(trayIdx+1);
			}
			else {
				free(taskList[i].device[0].memHandler);
				printf("Error AT:(re)allocating memory %s:%d",__FILE__,__LINE__);
				exit(1);
			}
			//free(tmpMemHandler); TODO: is it now ready to free?
		}
		 if (trayIdx==0 && memIdx==0){ //if this is the first tray to be initialized.
			taskList[i].entitiesbuffer = malloc(sizeof(XCLbuffer));
			taskList[i].device[0].memHandler = malloc(1 * sizeof(cl_mem)); //init the first device mem space.
			taskList[i].device[0].numcl_memObj=1;
		}

		taskList[i].entitiesbuffer->size = DbufferSize[i];//device buffer size for the i-th task.
		taskList[i].entitiesbuffer->bufferMode = CL_MEM_READ_WRITE;

		taskList[i].device[0].memHandler[trayIdx] = clCreateBuffer(
				taskList[i].device[0].context,
				taskList[i].entitiesbuffer->bufferMode,
				taskList[i].entitiesbuffer->size, NULL, &status);

		chkerr(status, "Error at: Creating mem Buffers", __FILE__, __LINE__);
		debug_print("allocated: %zd \n", taskList[i].entitiesbuffer->size);

		//memObjects[i]=taskList[i].device[0].memHandler[memIdx];

		//taskList[i].device[0].numcl_memObj++; //here we inform to the runtime that this device has increased in one its number of memObjs

	}

	return status;
}
