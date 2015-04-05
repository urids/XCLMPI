

#include "bufferFunctions.h"


int writeEntitiesBuffer(int numTasks,int  HbufferSize,int MPIentityTypeSize, void ** entitiesbuffer) {
	int i,j; //index variables
	int status;

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
				status = clEnqueueWriteBuffer(taskList[i].device[0].queue,
								taskList[i].device[0].memHandler[0], CL_TRUE,0,
								DbufferSize[i],
								tmpBuffer+i*DbufferSize[0],
								0, NULL, NULL);

			chkerr(status, "Writing mem Buffers", __FILE__, __LINE__);
			//tmpBuffer+=DbufferSize[0];
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


/*

#define GlobalSize 1024
#define GROUP_SIZE 128



//TODO: this can not be here because is related to the APP
#include "/home/uriel/Dev/mpiApps/FTWrkDistr/Applications/src/bodies/simComponents.h"
//TODO: this can not be here because is related to the APP
//#include "/home/uriel/Dev/mpiApps/FTWrkDistr/Applications/src/bodies/bodiesFunctions.h"


float myrand(float randMax, float randMin) {
	float result;
	result = (float) rand() / (float) RAND_MAX;
	return ((1.0f - result) * randMin + result * randMax);
}

int readNBodyPositions(simComponents* bodySim) {
	int status = 0;
	cl_int numParticles = 1024;
	int numBodies = numParticles;
	int groupSize = GROUP_SIZE;
	// make sure numParticles is multiple of group size
	numParticles = (cl_int) (
			((size_t) numParticles < groupSize) ? groupSize : numParticles);
	numParticles = (cl_int) ((numParticles / groupSize) * groupSize);
	numBodies = numParticles;

	int i, j;

		bodySim->initPos = (cl_float4*) malloc(numBodies * sizeof(cl_float4));
		bodySim->initVel = (cl_float3*) malloc(numBodies * sizeof(cl_float3));


	// initialization of mass and position of each body
	for (i = 0; i < numBodies; ++i) {
		/*int index = 4 * i;
		for (j = 0; j < 3; ++j) { // First 3 values are position in x,y and z direction
			bodySim->initPos[index + j] = myrand(3, 50);
		}
			bodySim->initPos[4*i]=(cl_float4){{(myrand(3, 50),myrand(3, 50),myrand(3, 50),myrand(1, 100))}};
		/*
			bodySim->initPos->x = myrand(3, 50);
			bodySim->initPos->y = myrand(3, 50);
			bodySim->initPos->z = myrand(3, 50);

		//bodySim->initPos[index + 3] = myrand(1, 1000); // Mass value
			bodySim->initPos->w = myrand(1, 1000);
	}


	for (i = 0; i < numBodies; ++i) {
			/*int index = 3 * i;
			for (j = 0; j < 3; ++j) { //velocities in x,y and z direction
				bodySim->initVel[index + j] = myrand(1, 10);
			}


		bodySim->initVel[3*i]=(cl_float3){{(myrand(3, 50),myrand(3, 50),myrand(3, 50),myrand(1, 1000))}};

		}
	return status;
}




int enqueueWriteBuffer(CLtask *task) {
	int status;

//TODO: this can not be here because is related to the APP
	simComponents* bodySim = malloc(sizeof(simComponents));
	readNBodyPositions(bodySim);

	int i;
		for(i=0;i<10;i++){
			printf("init data:= %f %f %f %f \n",bodySim->initPos[4*i].x,bodySim->initPos[4*i].y,bodySim->initPos[4*i].z,bodySim->initPos[4*i].w);
		}

	status = clEnqueueWriteBuffer(task->device->queue,
			task->device->memHandler[0], CL_TRUE, 0,
			task->kernel[0].buffer[0].size * sizeof(cl_float4),
			bodySim->initPos, 0, NULL, NULL);


	//setup the initial velocity to zero on device memory with map-buffer
	float* p = (float*) clEnqueueMapBuffer(task->device->queue,  //map memory object on device to memory in host.
			task->device->memHandler[1], CL_TRUE,
			CL_MAP_WRITE, 0, task->kernel[0].buffer[0].size * sizeof(cl_float3),
			0, NULL, NULL, &status);

	memset(p, 0, task->kernel[0].buffer[0].size*sizeof(cl_float3));
	status |= clEnqueueUnmapMemObject(task->device->queue, task->device->memHandler[1], p, 0, NULL,NULL);

	chkerr(status, "Writing Buffers", __FILE__, __LINE__);
	return status;
}
*/
