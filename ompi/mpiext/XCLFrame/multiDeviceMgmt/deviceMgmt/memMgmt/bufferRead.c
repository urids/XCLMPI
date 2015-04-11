#include "bufferFunctions.h"
#define DEBUG 0


int readEntitiesBuffer(int numTasks,int  HbufferSize,int MPIentityTypeSize,void ** entitiesbuffer) {
	int i,j;
	int status=0;


	void* tmpBuffer = *entitiesbuffer;
	for (i = 0; i < numTasks; i++) {

		debug_print("read Request: %d \n" ,DbufferSize[i]);
		debug_print("tmpBuffer %d \n" ,tmpBuffer);
		status = clEnqueueReadBuffer(taskList[i].device[0].queue,
				taskList[i].device[0].memHandler[0], CL_TRUE, 0, DbufferSize[i],
				tmpBuffer, 0, NULL, NULL);
		tmpBuffer+=DbufferSize[0];
		chkerr(status, "Reading mem Buffers", __FILE__, __LINE__);
	}

	return status;
}
