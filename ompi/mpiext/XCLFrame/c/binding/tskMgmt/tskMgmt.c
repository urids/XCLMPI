
#include "tskMgmt.h"

//this file performs Task<->Device matching


int createTaskList(int devSelection){ //There exist only one taskList per Node and each task has a device.
int i;

	switch (devSelection) {
	case ALL_DEVICES:

		numTasks = clXplr.numDevices;
		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);
		int assigned = 0;

		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(1*sizeof(Device)); //each task has only 1 device.
		}

		for (i = 0; i < clXplr.numGPUS; i++, assigned++) {
			taskList[assigned].device = cpu[i];
			taskList[assigned].device->numcl_memObj=0; //init the number of device memBuffers to zero
		}

		for (i = 0; i < clXplr.numCPUS; i++, assigned++) {
			taskList[assigned].device = gpu[i];
			taskList[assigned].device->numcl_memObj=0;
		}

		for (i = 0; i < clXplr.numACCEL; i++, assigned++) {
			taskList[assigned].device = accel[i];
			taskList[assigned].device->numcl_memObj=0;
		}
		break;

	case CPU_DEVICES: //TODO: missing multithread.
		numTasks = clXplr.numCPUS;
		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);

		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(sizeof(Device));
			taskList[i].device = cpu[i];
			taskList[i].device->numcl_memObj=0;//init the number of device memBuffers to zero
		}
		taskList[i].device->numcl_memObj=0;
		break;

	case GPU_DEVICES:
		numTasks = clXplr.numGPUS;
		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);

		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(sizeof(Device));
			taskList[i].device = gpu[i];
			taskList[i].device->numcl_memObj=0;//init the number of device memBuffers to zero
		}

		break;

	case ACC_DEVICES:
		numTasks = clXplr.numACCEL;
		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);

		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(sizeof(Device));
			taskList[i].device = accel[i];
			taskList[i].device->numcl_memObj=0;//init the number of device memBuffers to zero
		}

		break;

	default:
		break;
	}
return 0;


}


int XclCreateKernel(MPI_Comm comm,char* srcPath,char* kernelName,int numTasks){


	int i,j,k;
	void *dlhandle;
	int (*createProgram)(char*,int);
	int (*buildProgram)(int);
	int (*createKernel)(char*,int);
	int (*kernelXplor)(int);

	char *error;

	dlhandle =dlopen("libmultiDeviceMgmt.so",RTLD_LAZY);
	if (!dlhandle) {
		fputs(dlerror(), stderr);
		exit(1);
	}

	createProgram = dlsym(dlhandle, "createProgram");
	buildProgram = dlsym(dlhandle, "buildProgram");
	createKernel = dlsym(dlhandle, "createKernel");
	kernelXplor = dlsym(dlhandle, "kernelXplor");
	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}


		int err;
		err=(*createProgram)(srcPath,numTasks);
		err|=(*buildProgram)(numTasks);
		err|=(*createKernel)(kernelName,numTasks);
		err|=(*kernelXplor)(numTasks);


	dlclose(dlhandle);

return 0;


}
