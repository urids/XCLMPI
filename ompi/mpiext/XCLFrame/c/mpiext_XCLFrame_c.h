
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
//TODO: maybe we also must implement something like myRank.
OMPI_DECLSPEC int OMPI_collectDevicesInfo(int devSelection, MPI_Comm comm); //first parameter for this represents "ALL" or "GPUs" this function returns
OMPI_DECLSPEC int OMPI_XclCreateKernel(MPI_Comm comm,char* srcPath,char* kernelName);
//OMPI_DECLSPEC int OMPI_XclExecKernel(MPI_Comm communicator,int selTask,int globalThreads, int localThreads,const char * fmt, ...) __attribute__((format (printf, 5, 6)));
OMPI_DECLSPEC int OMPI_XclExecKernel(MPI_Comm communicator,int selTask,int workDim, size_t * globalThreads, size_t * localThreads,const char * fmt, ...) __attribute__((format (printf, 6, 7)));
OMPI_DECLSPEC int OMPI_XclScatter(const char* datafileName, int* count, MPI_Datatype MPIentityType, void* hostbuffer, int trayIdx, MPI_Comm comm);
OMPI_DECLSPEC int OMPI_XclGather(int trayIdx, int count, MPI_Datatype MPIentityType,void **hostbuffer, const char* datafileName , MPI_Comm comm);
OMPI_DECLSPEC int OMPI_XclSend(int trayIdx, int count, MPI_Datatype MPIentityType, int src_task, int dest_task, int TAG, MPI_Comm comm);
OMPI_DECLSPEC int OMPI_XclRecv(int trayIdx, int count, MPI_Datatype MPIentityType, int src_task, int dest_task, int TAG,MPI_Comm comm);
OMPI_DECLSPEC int OMPI_XclSendRecv(int src_task, int src_trayIdx, int dest_task, int dest_trayIdx, int count, MPI_Datatype MPIentityType, MPI_Comm comm);
OMPI_DECLSPEC int OMPI_XclReadTaskBuffer(int taskIdx, int trayIdx, int bufferSize, void * hostBuffer, MPI_Comm comm);
OMPI_DECLSPEC int OMPI_XclWriteTaskBuffer(int taskIdx, int trayIdx, int bufferSize, void * hostBuffer, MPI_Comm comm);
//TODO: implement Free routines.
//OMPI_DECLSPEC int OMPI_XclFreeTaskBuffer(int taskIdx, int trayIdx, MPI_Comm comm);
//OMPI_DECLSPEC int OMPI_XclFreeAllBuffers(int taskIdx, int trayIdx, MPI_Comm comm);
//OMPI_DECLSPEC int OMPI_XclFreeTask(int taskIdx, int trayIdx, MPI_Comm comm);
//OMPI_DECLSPEC int OMPI_XclFreeAllTasks(int taskIdx, int trayIdx, MPI_Comm comm);

OMPI_DECLSPEC int OMPI_XclWaitAllTasks(MPI_Comm comm);
