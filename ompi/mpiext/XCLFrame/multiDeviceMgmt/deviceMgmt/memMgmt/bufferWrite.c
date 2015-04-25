
#include "bufferFunctions.h"
#define DEBUG 0

int writeBuffer(int taskId, int trayIdx, int bufferSize, void * hostBuffer){
	int status;
		status = clEnqueueWriteBuffer(taskList[taskId].device[0].queue,
				taskList[taskId].device[0].memHandler[trayIdx], CL_TRUE, 0, bufferSize,
				hostBuffer, 0, NULL, NULL);
		chkerr(status, "Writing the new mem Buffer", __FILE__, __LINE__);
		//*devHandler=taskList[taskId].device[0].memHandler[trayIdx];
	return status;

}


int ALL_TASKS_writeBuffer(int numTasks, int trayIdx, void * hostBuffer ) {
	int i,j; //index variables
	int status;

	void* tmpBuffer=hostBuffer;
	for (i = 0; i < numTasks; i++) {

				status = clEnqueueWriteBuffer(taskList[i].device[0].queue,
								taskList[i].device[0].memHandler[trayIdx], CL_TRUE,0,
								DbufferSize[i], //extern DbuffersSize is declared in bufferFunctions.
								tmpBuffer,
								0, NULL, NULL);
				tmpBuffer+=DbufferSize[0]; //each task writing must do an offset.
			chkerr(status, "Writing mem Buffers", __FILE__, __LINE__);
			debug_print("copied: %zd \n" ,taskList[i].entitiesbuffer->size);
	}

	/*ADVANCED DATA COPY.
	 *
	 * This type of copy is favorable for the CPU because is a 0 copy strategy.
	 *


	void **p;
	p=(void**)malloc(numTasks*sizeof(void*));

	for (i = 0; i < numTasks; i++) {
		p[i] = (void *) clEnqueueMapBuffer(taskList[i].device[0].queue,  //map memory object on device to memory in host.
				taskList[i].device[0].memHandler[0], CL_TRUE,
				CL_MAP_WRITE, 0, bufferSize,
				0, NULL, NULL, &status);

		//memset(p, 0, task->kernel[0].buffer[0].size*sizeof(cl_float3));
		p[i]=*entitiesbuffer;
		status |= clEnqueueUnmapMemObject(taskList[i].device[0].queue, taskList[i].device[0].memHandler[0], p[i], 0, NULL,NULL);
		chkerr(status, "error at Writing Buffers", __FILE__, __LINE__);
	}
*/


		return status;
}



