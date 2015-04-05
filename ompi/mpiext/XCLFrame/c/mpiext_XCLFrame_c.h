
#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>
#include "CL/cl.h"

#include "../multiDeviceMgmt/deviceMgmt/Device.h"
#include "../multiDeviceMgmt/deviceMgmt/localDevices.h"
#include "../multiDeviceMgmt/taskMgmt/task.h"


#include "../multiDeviceMgmt/deviceMgmt/deviceExploration.h"
#include "../multiDeviceMgmt/deviceMgmt/devicesInitialization.h"



#define DEBUG 0

//OMPI_DECLSPEC int OMPI_commit_EntityType(int blockcount, int* blocklen, MPI_Aint* displacements, MPI_Datatype* basictypes, MPI_Datatype * newDatatype);

//this function collect in root node the information of all devices in each node of the communicator.
OMPI_DECLSPEC int OMPI_collectDevicesInfo(int devSelection, MPI_Comm comm); //first parameter for this represents "ALL" or "GPUs"
OMPI_DECLSPEC int OMPI_XclCreateKernel(MPI_Comm comm,char* srcPath,char* kernelName);
OMPI_DECLSPEC int OMPI_XclScatter(MPI_Comm communicator, const char* datafileName, MPI_Datatype MPIentityType, void **entitiesbuffer,int size, int* ePerRank);
OMPI_DECLSPEC int OMPI_XclExecKernel(MPI_Comm communicator,int globalThreads, int localThreads,const char * fmt, ...) __attribute__((format (printf, 4, 5)));
OMPI_DECLSPEC int OMPI_XclGather(MPI_Comm comm, int entitiesBufferSize,void **entitiesbuffer);
