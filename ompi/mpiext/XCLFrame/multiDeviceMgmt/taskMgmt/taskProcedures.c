#include "taskProcedures.h"
#include "task.h"
#include "CL/cl.h"
#include <stdlib.h>
#include <stdio.h>


#include "../errorMgmt/errorHandling.h"
#include "../deviceMgmt/localDevices.h"
#include "../deviceMgmt/memMgmt/bufferFunctions.h"

#define DEBUG 0
#define PROFILE 1

int createProgram(char* srcPath,int numTasks){
	int status;
	int i;
	FILE *fp;
	for (i = 0; i < numTasks; i++) {

		fp = fopen(srcPath, "r");
		if (!fp) {
			fprintf(stderr, "Failed to load kernels source file.\n");
			exit(1);
		}

		fseek(fp, 0L, SEEK_END);
		int sz = ftell(fp);
		fseek(fp, 0L, SEEK_SET);

		taskList[i].code = malloc(sizeof(XCLcode));
		taskList[i].code[0].source_str = (char*) malloc(sz+1);
		taskList[i].code[0].source_size = fread(taskList[i].code[0].source_str,
				1, sz, fp);

		taskList[i].CLprogram = malloc(sizeof(cl_program));

		taskList[i].CLprogram[0] = clCreateProgramWithSource(
				taskList[i].device[0].context, 1,
				(const char **) &taskList[i].code[0].source_str,
				(const size_t *) &taskList[i].code[0].source_size, &status);

		chkerr(status, "Creating Program ", __FILE__, __LINE__);
	fclose(fp);
	}

	return status;
}

int buildProgram(int numTasks) {
	int status;
	int i;

	for (i = 0; i < numTasks; i++) {
		status = clBuildProgram(taskList[i].CLprogram[0], 1,
				&taskList[i].device[0].deviceId, NULL, NULL, NULL);

		if (status == CL_BUILD_PROGRAM_FAILURE) {
			// Determine the size of the log
			size_t log_size;
			clGetProgramBuildInfo(taskList[i].CLprogram[0], taskList[i].device[0].deviceId, CL_PROGRAM_BUILD_LOG,
					0, NULL, &log_size);

			// Allocate memory for the log
			char *log = (char *) malloc(log_size+1);
			// Get the log
			clGetProgramBuildInfo(taskList[i].CLprogram[0], taskList[i].device[0].deviceId, CL_PROGRAM_BUILD_LOG,
					log_size, log, NULL);
			log[log_size+1] = '\0';
			// Print the log
			printf("log Description: %s\n", log);
		}

		chkerr(status, "Building Program ", __FILE__, __LINE__);
	}

	return status;
}

int createKernel(char* kernelName, int numTasks){  //TODO: here we will need a list of kernels.
	int status;
	int i;
	for(i=0;i<numTasks;i++){
	taskList[i].kernel=malloc(sizeof(XCLkernel));
	taskList[i].kernel[0].kernel = clCreateKernel(taskList[i].CLprogram[0], kernelName, &status);
	chkerr(status, "error at: creating the kernel:", __FILE__, __LINE__);
	}
	return status;
}


int kernelXplor(int numTasks){
	int status;
	int i;
	size_t kernNameSize;
	cl_uint numArgs;
	for(i=0;i<numTasks;i++){

	status=clGetKernelInfo(taskList[i].kernel[0].kernel,CL_KERNEL_FUNCTION_NAME,0,NULL,&kernNameSize);
	char* kernName=malloc(kernNameSize*sizeof(char));
	status=clGetKernelInfo(taskList[i].kernel[0].kernel,CL_KERNEL_FUNCTION_NAME,kernNameSize,(char*)kernName,NULL);

	status=clGetKernelInfo(taskList[i].kernel[0].kernel,CL_KERNEL_NUM_ARGS,sizeof(cl_uint),&numArgs,NULL);

	chkerr(status, "error at: getting kernel info", __FILE__, __LINE__);

	debug_print("\n Debug:  %s, has %u args\n",kernName,numArgs);

	taskList[i].kernel[0].numArgs=numArgs;
	}

return status;
}

int setKernelArgs(int numTasks,int numparameter,int paramSize,void* paramValue){
	int status;
	int i;

	if(numparameter==0){ // TODO: the parameter 0 is always the entities buffer???
		for(i=0;i<numTasks;i++){
			debug_print("\n Debug: setting arg: %d \n",numparameter);
			status = clSetKernelArg(taskList[i].kernel[0].kernel, 0, sizeof(cl_mem),
						   & taskList[i].device[0].memHandler[0]);
			chkerr(status, "error at: Setting entity buffer Arg.", __FILE__, __LINE__);
		}
	}else{
		for(i=0;i<numTasks;i++){
			debug_print("\n Debug: setting arg: %d \n",numparameter);
				status = clSetKernelArg(taskList[i].kernel[0].kernel, numparameter, paramSize,
					 (void *) paramValue);
				chkerr(status, "error at: Setting other kernel Args", __FILE__, __LINE__);
		}
	}
	debug_print("setArgs successful..\n");
	return status;
}


int enqueueKernel(int numTasks, const size_t globalThreads, const size_t localThreads) {
	int status;
	int i;


	for(i=0;i<numTasks;i++){
	cl_event kernelProfileEvent;
	cl_ulong time_start, time_end;
	double total_time;

	status = clEnqueueNDRangeKernel(taskList[i].device[0].queue, taskList[i].kernel[0].kernel, 1, NULL,
			&globalThreads, &localThreads, 0, NULL, &kernelProfileEvent);
	chkerr(status, "Enqueuing Kernels ", __FILE__, __LINE__);

	debug_print("Enqueuing Kernel successful..\n");


	#if PROFILE
	clWaitForEvents(1 , &kernelProfileEvent);
	clGetEventProfilingInfo(kernelProfileEvent, CL_PROFILING_COMMAND_START, sizeof(time_start), &time_start, NULL);
	clGetEventProfilingInfo(kernelProfileEvent, CL_PROFILING_COMMAND_END, sizeof(time_end), &time_end, NULL);
	total_time = time_end - time_start;
	profile_print("Execution time of task: %d -->  %0.3f ms\n",i,(total_time / 1000000.0));
	#endif //if PROFILE

	}



	return status;
}



