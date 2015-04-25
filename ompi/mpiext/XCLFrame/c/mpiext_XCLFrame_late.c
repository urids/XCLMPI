
#include "ompi_config.h"

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdarg.h>

#include "ompi/mpi/c/bindings.h"
#include "ompi/mpiext/mpiext.h"
#include "hiddenComms.h"
#include "ompi/mpiext/XCLFrame/c/mpiext_XCLFrame_c.h"
#include "taskMap.h"


XCLtask* taskList; // Global Variable declared in task.h
taskInfo* g_taskList; //Global Variable declared at taskMap.h
int numTasks;//Global variable declared in tskMgmt.h
int numRanks, myRank, HostNamelen;

int OMPI_collectDevicesInfo(int devSelection, MPI_Comm comm){

	int i,j,k; //indx variables.


	char hostname[MPI_MAX_PROCESSOR_NAME];

	MPI_Comm_rank(comm, &myRank);
	MPI_Comm_size(comm,&numRanks);
	MPI_Get_processor_name(hostname, &HostNamelen);


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


	int* RKS =(int*)malloc(numRanks*sizeof(int));
	MPI_Allgather(&numTasks,1,MPI_INT,RKS,1,MPI_INT,comm);

	int g_numTasks;
	for(g_numTasks=0,i=0;i<numRanks;i++){
		g_numTasks+=RKS[i];
	}

	g_taskList=(taskInfo*)malloc(g_numTasks*sizeof(taskInfo)); //TODO: find the space for deallocation.

	for(i=0;i<g_numTasks;i++)
		g_taskList[i].g_taskIdx=i;


	 //this section creates the global Rank map structure.
	for(i=0,k=0;i<numRanks;i++){
		for(j=0;j<RKS[i];j++){
			g_taskList[k].r_rank=i;
			g_taskList[k].l_taskIdx=j;
			k++;
		}
	}

	return g_numTasks;
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

int OMPI_XclExecKernel(MPI_Comm communicator, int g_selTask, int globalThreads,
		int localThreads, const char * fmt, ...) {
	//Select the appropriate local task if any.
	if (myRank == g_taskList[g_selTask].r_rank) {

		int l_selTask= g_taskList[g_selTask].l_taskIdx;

		void *dlhandle;

		int (*XclExecKernel)(MPI_Comm, int selTask, int, int, const char *,
				va_list);
		char *error;

		dlhandle = dlopen("libtskMgmt.so", RTLD_LAZY);
		if (!dlhandle) {
			fputs(dlerror(), stderr);
			exit(1);
		}

		XclExecKernel = dlsym(dlhandle, "XclExecKernel");

		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			exit(1);
		}
		int err;

		va_list argptr;
		va_start(argptr, fmt);
		err = (*XclExecKernel)(communicator, l_selTask, globalThreads,
				localThreads, fmt, argptr);
		va_end(argptr);

		dlclose(dlhandle);
	}
	return MPI_SUCCESS;
}


