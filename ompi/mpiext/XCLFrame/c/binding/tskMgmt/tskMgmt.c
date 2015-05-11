
#include "tskMgmt.h"
#define DEBUG 0
//This file performs Task<->Device matching
device_Task_Info* taskDevMap;
int createTaskList(int devSelection){ //There exist only one taskList per Node and each task has a device.
int i,j;

taskDevMap=malloc(clXplr.numDevices*sizeof(device_Task_Info));

int matched=0;

	for (i = 0; i < clXplr.numCPUS; i++) { //TODO:CPUS are set first because their enqueue calls are non blocking

		taskDevMap[matched].mappedDevice = cpu[i];
		//devtaskMap[matched].l_deviceIdx = matched;
		taskDevMap[matched].min_tskIdx = 0;
		taskDevMap[matched].max_tskIdx = 1;
		matched++;
	}

	for (i = 0; i < clXplr.numGPUS; i++) {

		taskDevMap[matched].mappedDevice = gpu[i];
		//devtaskMap[matched].l_deviceIdx = matched;
		taskDevMap[matched].min_tskIdx = 2;
		taskDevMap[matched].max_tskIdx = 7;
		matched++;
	}

	/*for (i = 0; i < clXplr.numACCEL; i++) {

		taskDevMap[matched].mappedDevice = accel[i];
		//devtaskMap[matched].l_deviceIdx = matched;
		taskDevMap[matched].min_tskIdx = 5;
		taskDevMap[matched].max_tskIdx = 5;
		matched++;
	}*/


//taskList[assigned].device = accel[i];
//taskList[assigned].numTrays = 0;

	//cpu[0]->memHandler= (cl_mem**)malloc(4 * sizeof(cl_mem*));
	switch (devSelection) {
	case ALL_DEVICES:

		numTasks = 8;

		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);
		int assigned = 0;

		//Each task needs a handler (pointer) to its device
		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(1*sizeof(Device)); //each task has only 1 device.
		}

		for (i = 0; i < clXplr.numDevices; i++) {
			for (j = taskDevMap[i].min_tskIdx; j <= taskDevMap[i].max_tskIdx;j++) {
				debug_print("-----matching task %d ------\n",j);

				taskList[j].device = taskDevMap[i].mappedDevice;
				taskList[j].numTrays = 0;

				//here we query how many racks has this device.
				int rackIdx = taskList[j].device[0].numRacks;
				if (rackIdx == 0) {
					taskList[j].device[0].memHandler = malloc(1 * sizeof(cl_mem*));
					//taskList[j].device[0].memHandler[0] = malloc(1 * sizeof(cl_mem));
					taskList[j].Rack = rackIdx;
					taskList[j].device[0].numRacks++;
				} else {
					cl_mem** tmpRack;
					tmpRack = (cl_mem**) realloc(taskList[j].device[0].memHandler,(rackIdx + 1) * sizeof(cl_mem*));
					if (tmpRack != NULL) {
						taskList[j].Rack = rackIdx;
						taskList[j].device[0].memHandler = tmpRack;
						taskList[j].device[0].numRacks++;
					} else {
						printf("ERROR AT: Reallocating racks. %d , %d",	__FILE__, __LINE__);
					}
				}
			}
		}

		break;



	case CPU_DEVICES: //TODO: missing multithread.
		numTasks = 2;
		printf("CPU EXEC\n");
		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);


		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(sizeof(Device));
			taskList[i].device = cpu[0];//TODO: switch device number.
			taskList[i].numTrays = 0; //init the number of device memBuffers to zero

			int rackIdx = taskList[i].device[0].numRacks;
			if (rackIdx == 0) {
				taskList[i].device[0].memHandler = malloc(1 * sizeof(cl_mem*));
				taskList[i].Rack = rackIdx;
				taskList[i].device[0].numRacks++;
			} else {
				cl_mem** tmpRack;
				tmpRack = (cl_mem**) realloc(taskList[i].device[0].memHandler,
						(rackIdx + 1) * sizeof(cl_mem*));
				if (tmpRack != NULL) {
					taskList[i].Rack = rackIdx;
					taskList[i].device[0].memHandler = tmpRack;
					taskList[i].device[0].numRacks++;
				} else {
					printf("ERROR AT: Reallocating racks. %d , %d", __FILE__,
							__LINE__);
				}
			}

			//taskList[i].Rack=i;
			//taskList[i].device[0].numRacks++;
		}
		break;

	case GPU_DEVICES:
		numTasks = 1;
		printf("GPU EXEC\n");
		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);

		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(sizeof(Device));
			taskList[i].device = gpu[0]; //TODO: switch device number.
			taskList[i].numTrays = 0; //init the number of device memBuffers to zero

			//here we query how many racks has this device.
			int rackIdx = taskList[i].device[0].numRacks;
			if (rackIdx == 0) {
				taskList[i].device[0].memHandler = malloc(1 * sizeof(cl_mem*));
				//taskList[i].device[0].memHandler[0]=malloc(1*sizeof(cl_mem));
				taskList[i].Rack = rackIdx;
				taskList[i].device[0].numRacks++;
			} else {
				cl_mem** tmpRack;
				tmpRack = (cl_mem**) realloc(taskList[i].device[0].memHandler,
						(rackIdx + 1) * sizeof(cl_mem*));
				if (tmpRack != NULL) {
					taskList[i].Rack = rackIdx;
					taskList[i].device[0].memHandler = tmpRack;
					taskList[i].device[0].numRacks++;
				} else {
					printf("ERROR AT: Reallocating racks. %d , %d", __FILE__,
							__LINE__);
				}
			}
		}

		break;

	case ACC_DEVICES:
		numTasks = clXplr.numACCEL;
		taskList = (XCLtask*) malloc(sizeof(XCLtask) * numTasks);

		for (i = 0; i < numTasks; i++) {
			taskList[i].device = (Device*) malloc(sizeof(Device));
			taskList[i].device = accel[i];
			taskList[i].numTrays=0;//init the number of device memBuffers to zero
			//here we query how many racks has this device.
			int rackIdx = taskList[i].device[0].numRacks;
			if (rackIdx == 0) {
				taskList[i].device[0].memHandler = malloc(1 * sizeof(cl_mem*));
				//taskList[i].device[0].memHandler[0]=malloc(1*sizeof(cl_mem));
				taskList[i].Rack = rackIdx;
				taskList[i].device[0].numRacks++;
			} else {
				cl_mem** tmpRack;
				tmpRack = (cl_mem**) realloc(taskList[i].device[0].memHandler,
						(rackIdx + 1) * sizeof(cl_mem*));
				if (tmpRack != NULL) {
					taskList[i].Rack = rackIdx;
					taskList[i].device[0].memHandler = tmpRack;
					taskList[i].device[0].numRacks++;
				} else {
					printf("ERROR AT: Reallocating racks. %d , %d", __FILE__,
							__LINE__);
				}
			}

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
