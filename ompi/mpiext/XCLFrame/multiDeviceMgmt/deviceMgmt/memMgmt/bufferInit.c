#include "bufferFunctions.h"


int initEntitiesBuffer(int numTasks, int HbufferSize,int MPIentityTypeSize) {
	int status;
	int i,j;//index variables
	//printf(" Debug: %d , %d",numTasks,bufferSize);

	int numEntitiesInHost=(int)HbufferSize/MPIentityTypeSize;

	float relation=(float)numEntitiesInHost/(float)numTasks;

	int* DbufferSize=(int*)malloc(numTasks*sizeof(int)); //Device BufferSize
	for(j=0;j<numTasks;j++){
	(j==(numTasks-1))?((int)floorf(relation)*MPIentityTypeSize):((int)ceilf(relation)*MPIentityTypeSize);
	}


	for (i = 0; i < numTasks; i++) {
		//int numBuffers = taskList[j].kernel[0].numArgs - num_consts;
		taskList[i].entitiesbuffer = malloc(sizeof(XCLbuffer));
		taskList[i].device[0].memHandler=malloc(sizeof(cl_mem));

			//taskList[i].entitiesbuffer->size  = DbufferSize[i]+5000;
			taskList[i].entitiesbuffer->size  = HbufferSize;
			taskList[i].entitiesbuffer->bufferMode = CL_MEM_READ_WRITE;

			taskList[i].device[0].memHandler[0] = clCreateBuffer(
					taskList[i].device[0].context,
					taskList[i].entitiesbuffer->bufferMode,
					taskList[i].entitiesbuffer->size, NULL, &status);


		chkerr(status, "Error at: Creating mem Buffers", __FILE__, __LINE__);

	}

	return status;
}
