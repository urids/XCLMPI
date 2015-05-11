
#include "tskMgmt.h"


/* int XclExecKernel(MPI_Comm communicator, const char * fmt, ...)
    __attribute__((format (printf, 2, 3)));*/

//#define OMPI_XclExecKernel(dataFilePath , fmt, ...) _OMPI_XclExecKernel(dataFilePath , fmt,  ##__VA_ARGS__)

 int XclExecKernel(MPI_Comm comm, int selTask, int globalThreads, int localThreads, const char * fmt, va_list arg) {
	int numParam = 0;
	const char *p;
	int j, k; //index exclusive
	int i;
	int tray;
	double dd;
	float f;
	unsigned u;
	char *s;

	cl_mem* memObj=malloc(sizeof(cl_mem));

	void *dlhandle;
	int (*setKernelArgs)(int, int, int, void*);
	int (*setKernelmemObj)(int,int,int,int);
	int (*enqueueKernel)(int numTasks,int selTask, const size_t globalThreads, const size_t localThreads);
	char *error;

	///home/uriel/Dev/mpiApps/FTWrkDistr/multiDeviceMgmt/Debug/
	dlhandle =dlopen("libmultiDeviceMgmt.so",RTLD_LAZY);

	if (!dlhandle) {
		fputs(dlerror(), stderr);
		exit(1);
	}

	setKernelArgs = dlsym(dlhandle, "setKernelArgs");
	setKernelmemObj=dlsym(dlhandle, "setKernelmemObj");
	enqueueKernel= dlsym(dlhandle,"enqueueKernel");

	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}


  int numConsts=0;

	for(p = fmt; *p != '\0'; p++) {
		if(*p != '%') {
			continue;
		}
		switch(*++p) {
		case 'c':
			i = va_arg (arg, int);
			(*setKernelArgs)(selTask,numParam,sizeof(int),&i);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: int, arg number: %d, value: %d \n",numParam,i);
			break;

		case 'd':
			i = va_arg(arg, int);
			(*setKernelArgs)(selTask,numParam,sizeof(int),&i);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: int, arg number: %d, value: %d \n",numParam,i);
			break;
		case 'f':
			dd = va_arg(arg, double);
			f=(float)dd;
			(*setKernelArgs)(selTask,numParam,sizeof(float),&f);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: float, arg number: %d, value: %lf \n",numParam,f);
			break;

		case 's':
			s = va_arg(arg, char *);
			(*setKernelArgs)(selTask, numParam, sizeof(char*), s);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: string, arg number: %d, value: %s \n",numParam, s);
			break;
		case 'T':
			tray = va_arg(arg, int);
			(*setKernelmemObj)(selTask, numParam, sizeof(cl_mem), tray);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: cl_mem, arg number: %d, value: abstract  \n",numParam);
			break;

		case '%':
			putchar('%');
			break;
		}
	}

	(*enqueueKernel)(numTasks,selTask, globalThreads, localThreads);

  return 0;

}


int XclWaitAllTasks(MPI_Comm comm){
 	void *dlhandle;

 			int (*XclWaitAllTasks)(int numTasks,MPI_Comm comm);
 			char *error;

 			dlhandle = dlopen("libmultiDeviceMgmt.so", RTLD_LAZY);
 			if (!dlhandle) {
 				fputs(dlerror(), stderr);
 				exit(1);
 			}

 			XclWaitAllTasks = dlsym(dlhandle, "XclWaitAllTasks");

 			if ((error = dlerror()) != NULL) {
 				fputs(error, stderr);
 				exit(1);
 			}
 			int err;
 			err = (*XclWaitAllTasks)(numTasks,comm);

 			dlclose(dlhandle);
 	 return MPI_SUCCESS;
 }
