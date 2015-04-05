
#include "tskMgmt.h"




int createTaskList(int devSelection){ //There exist only one taskList per Node runtime and each task has a device.
int i;

switch (devSelection){
	case ALL_DEVICES:
		numTasks=clXplr.numDevices;
		taskList=(XCLtask*)malloc(sizeof(XCLtask)*numTasks);

			for(i=0;i<numTasks;i++){
				taskList[i].device=(Device*)malloc(sizeof(Device));
			}

			/*for(i=0;i<clXplr.numCPUS;i++){
				taskList[i].device=cpu[i];
			}*/

			taskList[0].device=cpu[0];
			//taskList[0].localID=0;
			taskList[1].device=gpu[0];
			//taskList[0].localID=1;

			/* TODO: check assignment of task to devices
			for (i = clXplr.numCPUS; i <= clXplr.numGPUS; i++) {
				taskList[i].device = gpu[i];
			}
			for (i = 0; i <= clXplr.numACCEL; i++) {
				taskList[i].device = accel[i];
			}*/

		break;

	case CPU_DEVICES: //TODO: missing implementation
		numTasks = clXplr.numCPUS;
		break;

	case GPU_DEVICES: //TODO: missing implementation
		numTasks = clXplr.numGPUS;
		break;

	case ACC_DEVICES: //TODO: missing implementation
		numTasks = clXplr.numACCEL;
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
