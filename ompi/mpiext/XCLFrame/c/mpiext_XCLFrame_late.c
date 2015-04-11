
#include "ompi_config.h"

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/mpiext/mpiext.h"
#include "hiddenComms.h"
#include "ompi/mpiext/XCLFrame/c/mpiext_XCLFrame_c.h"


XCLtask* taskList; // Global Variable declared in task.h
int numTasks;//Global variable declared in tskMgmt.h

int OMPI_collectDevicesInfo(int devSelection, MPI_Comm comm){

	int numprocess, myRank, len2;
	char hostname[MPI_MAX_PROCESSOR_NAME];

	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);
	MPI_Get_processor_name(hostname, &len2);


	void *tskMgmt_dlhandle;
	int (*createTaskList)(int);
	char *error;
	tskMgmt_dlhandle = dlopen ("libtskMgmt.so", RTLD_LAZY);

	if (!tskMgmt_dlhandle ) {
			fputs(dlerror(), stderr);
			exit(1);
		}

	createTaskList = dlsym(tskMgmt_dlhandle, "createTaskList");
		if ((error = dlerror()) != NULL ) {
			fputs(error, stderr);
			exit(1);
		}

	(*createTaskList)(devSelection);

/*//TODO: This section is for delegating master node for load balancing.
	if (myRank == 0) {

		rcvXploreInfo(comm);

	} else {

		sendXploreInfo(comm);

	}
*/
	dlclose(tskMgmt_dlhandle);
	return numTasks;
}


int OMPI_XclCreateKernel(MPI_Comm comm,char* srcPath, char* kernelName){

int i;

	void *dlhandle;
	int (*XclCreateKernel)(MPI_Comm comm,char* srcPath,char* kernelName,int numTasks);
	char *error;

	dlhandle =dlopen("libtskMgmt.so",RTLD_LAZY);
	if (!dlhandle) {
		fputs(dlerror(), stderr);
		exit(1);
	}

	XclCreateKernel = dlsym(dlhandle, "XclCreateKernel");

	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}

	int err;
	//take care here because clXplr will become the global and unique xploreInfo "object" maybe I should make it const

	//TODO: this is only if devSelection=ALL_DEVICES; otherwise maybe we need a switch.
	err=(*XclCreateKernel)(comm,srcPath,kernelName,numTasks);
	//err|=(*XclCreateKernel)(comm,srcPath,kernelName,& clXplr,gpu,&taskSet);
	//err|=(*XclCreateKernel)(comm,srcPath,kernelName,& clXplr,accel,&taskSet);
	dlclose(dlhandle);

	return MPI_SUCCESS;
}

int OMPI_XclExecKernel(MPI_Comm communicator,int globalThreads, int localThreads,const char * fmt, ...){
	void *dlhandle;
		int (*XclExecKernel)(MPI_Comm, int, int ,const char * ,  va_list );
		char *error;

		dlhandle =dlopen("libtskMgmt.so",RTLD_LAZY);
		if (!dlhandle) {
			fputs(dlerror(), stderr);
			exit(1);
		}

		XclExecKernel = dlsym(dlhandle, "XclExecKernel");

		if ((error = dlerror()) != NULL ) {
			fputs(error, stderr);
			exit(1);
		}
		int err;

		 va_list argptr;
		  va_start(argptr,fmt);
		  err=(*XclExecKernel)(communicator,globalThreads,localThreads,fmt,argptr);
		  va_end(argptr);



		dlclose(dlhandle);

		return MPI_SUCCESS;
}


