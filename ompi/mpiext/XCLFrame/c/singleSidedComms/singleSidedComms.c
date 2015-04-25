
#include "singleSidedComms.h"
///TODO: should I enable pass a rank where we want to read such data?
///TODO: should I enable pass a rank where host buffer lives?
int OMPI_XclReadTaskBuffer(int g_taskIdx, int trayIdx, int bufferSize, void * hostBuffer, MPI_Comm comm){
	int myRank;
	MPI_Comm_rank(comm, &myRank);
	if (myRank == g_taskList[g_taskIdx].r_rank) {
		void * memReadHandle = NULL;
		int (*readBuffer)(int taskIdx, int trayIdx, int bufferSize,
				void * hostBuffer);
		char *error;
		memReadHandle = dlopen("libmultiDeviceMgmt.so", RTLD_NOW);

		if (!memReadHandle) {
			perror("library not found or could not be opened AT: OMPI_XclSend");
			exit(1);
		}

		readBuffer = dlsym(memReadHandle, "readBuffer");
		if ((error = dlerror()) != NULL) {
			printf("err:");
			fputs(error, stderr);
			exit(1);
		}

		//convert the global task Id into a local to recover the data.
		//we already have verified that this data lives in this rank with the first IF upstream=)!!
		int l_taskIdx= g_taskList[g_taskIdx].l_taskIdx;
		(*readBuffer)(l_taskIdx, trayIdx, bufferSize, hostBuffer);
	}

	return 0;
}

//TODO should I pass a rank to enable say on which task lives host buffer?
int OMPI_XclWriteTaskBuffer(int g_taskIdx, int trayIdx, int bufferSize,void * hostBuffer, MPI_Comm comm){
	int myRank;
	MPI_Comm_rank(comm, &myRank);
	if (myRank == g_taskList[g_taskIdx].r_rank) {
		void * memWrtHandle = NULL; //function pointer
		int (*initNewBuffer)(int l_taskIdx, int trayIdx, int bufferSize);
		int (*writeBuffer)(int l_taskIdx, int trayIdx, int bufferSize,
				void * hostBuffer);

		char *error;
		memWrtHandle = dlopen("libmultiDeviceMgmt.so", RTLD_NOW);

		if (!memWrtHandle) {
			perror("library not found or could not be opened AT: OMPI_XclRecv");
			exit(1);
		}

		initNewBuffer = dlsym(memWrtHandle, "initNewBuffer");
		writeBuffer = dlsym(memWrtHandle, "writeBuffer");
		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			exit(1);
		}

		int l_taskIdx= g_taskList[g_taskIdx].l_taskIdx;
		(*initNewBuffer)(l_taskIdx, trayIdx, bufferSize);
		(*writeBuffer)(l_taskIdx, trayIdx, bufferSize, hostBuffer);
	}
	return 0;
}


int OMPI_XclSend(int trayIdx, int count, MPI_Datatype MPIentityType, int g_src_task, int g_dest_task, int TAG, MPI_Comm comm){
	int myRank;
	MPI_Comm_rank(comm, &myRank);
	if (myRank == g_taskList[g_src_task].r_rank) {
		void* tmpBuffData;
		int tmpBuffSz = count * MPIentityTypeSize;
		tmpBuffData = (void*) malloc(tmpBuffSz);

		void * memSendHandle = NULL;
		int (*readBuffer)(int taskId, int bufferSize, int memIdx,
				void * entitiesbuffer);
		char *error;
		memSendHandle = dlopen("libmultiDeviceMgmt.so", RTLD_NOW);

		if (!memSendHandle) {
			perror("library not found or could not be opened AT: OMPI_XclSend");
			exit(1);
		}

		readBuffer = dlsym(memSendHandle, "readBuffer");
		if ((error = dlerror()) != NULL) {
			printf("err:");
			fputs(error, stderr);
			exit(1);
		}

		//GET the data from the device.
		int l_src_task = g_taskList[g_src_task].l_taskIdx;
		(*readBuffer)(l_src_task, trayIdx, tmpBuffSz, tmpBuffData);
		//Send to the appropriate rank.
		int dest_rank = g_taskList[g_dest_task].r_rank;
		MPI_Send(tmpBuffData, count, MPIentityType, dest_rank, TAG, comm);
	}
	//TODO:free tmpBuffData.
	return 0;

}


int OMPI_XclRecv(int trayIdx, int count, MPI_Datatype MPIentityType, int g_src_task, int g_recv_task, int TAG,MPI_Comm comm) {
	int myRank;
	MPI_Comm_rank(comm, &myRank);
	if (myRank == g_taskList[g_recv_task].r_rank) {
		void* tmpBuffData;
		int tmpBuffSz = count * MPIentityTypeSize; //buffer size in bytes.
		tmpBuffData = (void*) malloc(tmpBuffSz);

		//recv from the appropriate rank.
		int src_rank = g_taskList[g_src_task].r_rank;
		MPI_Recv(tmpBuffData, count, MPIentityType, src_rank, TAG, comm,
				MPI_STATUS_IGNORE );

		void * memRecvHandle = NULL; //function pointer
		int (*initNewBuffer)(int taskIdx, int trayIdx, int bufferSize);
		int (*writeBuffer)(int taskId, int trayIdx, int bufferSize,
				void * hostBuffer);

		char *error;
		memRecvHandle = dlopen("libmultiDeviceMgmt.so", RTLD_NOW);

		if (!memRecvHandle) {
			perror("library not found or could not be opened AT: OMPI_XclRecv");
			exit(1);
		}

		initNewBuffer = dlsym(memRecvHandle, "initNewBuffer");
		writeBuffer = dlsym(memRecvHandle, "writeBuffer");
		if ((error = dlerror()) != NULL) {
			fputs(error, stderr);
			exit(1);
		}

		int l_recv_task = g_taskList[g_recv_task].l_taskIdx;
		(*initNewBuffer)(l_recv_task, trayIdx, tmpBuffSz);
		(*writeBuffer)(l_recv_task, trayIdx, tmpBuffSz, tmpBuffData);
	}
	return 0;
}

int OMPI_XclSendRecv(int src_task, int src_trayIdx,
		             int dest_task, int dest_trayIdx,
		             int count, MPI_Datatype MPIentityType,
		             MPI_Comm comm) {
	int TAG = 0;

	OMPI_XclSend(src_trayIdx, count, MPIentityType, src_task, dest_task, TAG, comm);
	OMPI_XclRecv(dest_trayIdx, count, MPIentityType, src_task, dest_task, TAG, comm);

	return 0;

}
