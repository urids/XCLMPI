#include "bufferFunctions.h"



int readEntitiesBuffer(int numTasks,int  HbufferSize,int MPIentityTypeSize,void ** entitiesbuffer) {
	int i,j;
	int status=0;

	int numEntitiesInHost=(int)HbufferSize/MPIentityTypeSize;

	float relation=(float)numEntitiesInHost/(float)numTasks;
	int remainEntites=(numEntitiesInHost%numTasks!=0)?(numEntitiesInHost%numTasks):((int)ceilf(relation));

	int* DbufferSize=(int*)malloc(numTasks*sizeof(int)); //Device BufferSize in bytes
	for(j=0;j<numTasks;j++){
		DbufferSize[j]= (int)(ceil(relation)*MPIentityTypeSize);
		//DbufferSize[j]=(j==(numTasks-1))?(remainEntites*MPIentityTypeSize):(((int)ceilf(relation))*MPIentityTypeSize);
	}

	void* tmpBuffer=*entitiesbuffer;
	for (i = 0; i < numTasks; i++) {
					status = clEnqueueReadBuffer(taskList[i].device[0].queue,
									taskList[i].device[0].memHandler[0], CL_TRUE, 0,
									DbufferSize[i],
									tmpBuffer+i*DbufferSize[0],
									0, NULL, NULL);

				chkerr(status, "Reading mem Buffers", __FILE__, __LINE__);
				//tmpBuffer+=DbufferSize[0];
			}


/*	for (i = 0; i < numTasks; i++) {
				status = clEnqueueReadBuffer(taskList[i].device[0].queue,
								taskList[i].device[0].memHandler[0], CL_TRUE, 0,
								DbufferSize[i],
								tmpBuffer,
								0, NULL, NULL);

			chkerr(status, "Reading mem Buffers", __FILE__, __LINE__);
			//tmpBuffer+=DbufferSize[0];
		}
*/
	return status;
}
