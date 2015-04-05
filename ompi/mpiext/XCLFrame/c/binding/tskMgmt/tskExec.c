
#include "tskMgmt.h"
#include "../../XclScatter.h"

/* int XclExecKernel(MPI_Comm communicator, const char * fmt, ...)
    __attribute__((format (printf, 2, 3)));*/

//#define OMPI_XclExecKernel(dataFilePath , fmt, ...) _OMPI_XclExecKernel(dataFilePath , fmt,  ##__VA_ARGS__)

 int XclExecKernel(MPI_Comm comm, int globalThreads, int localThreads, const char * fmt, va_list arg) {
	int numParam = 0;
	const char *p;
	int j, k; //index exclusive
	int i;
	double dd;
	float f;
	unsigned u;
	char *s;


	void *dlhandle;
	int (*setKernelArgs)(int, int, int, void*);
	int (*enqueueKernel)(int numTasks, const size_t globalThreads, const size_t localThreads);
	char *error;

	///home/uriel/Dev/mpiApps/FTWrkDistr/multiDeviceMgmt/Debug/
	dlhandle =dlopen("libmultiDeviceMgmt.so",RTLD_LAZY);

	if (!dlhandle) {
		fputs(dlerror(), stderr);
		exit(1);
	}

	setKernelArgs = dlsym(dlhandle, "setKernelArgs");
	enqueueKernel= dlsym(dlhandle,"enqueueKernel");

	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}

int a=10; //TODO: fix this.
	(*setKernelArgs)(numTasks,numParam,sizeof(int),&a); //this is the call to set the entities buffer
	numParam++;

  int numConsts=0;

	for(p = fmt; *p != '\0'; p++) {
		if(*p != '%') {
			continue;
		}
		switch(*++p) {
		case 'c':
			i = va_arg (arg, int);
			(*setKernelArgs)(numTasks,numParam,sizeof(int),&i);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: int, arg number: %d, value: %d \n",numParam,i);
			break;

		case 'd':
			i = va_arg(arg, int);
			(*setKernelArgs)(numTasks,numParam,sizeof(int),&i);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: int, arg number: %d, value: %d \n",numParam,i);
			break;
		case 'f':
			dd = va_arg(arg, double);
			f=(float)dd;
			(*setKernelArgs)(numTasks,numParam,sizeof(float),&f);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: float, arg number: %d, value: %lf \n",numParam,f);
			break;

		case 's':
			s = va_arg(arg, char *);
			(*setKernelArgs)(numTasks, numParam, sizeof(char*), s);
			numParam++;
			numConsts++;
			debug_print("\n Debug: argument type: string, arg number: %d, value: %s \n",numParam, s);
			break;

		case '%':
			putchar('%');
			break;
		}
	}

	(*enqueueKernel)(numTasks, globalThreads, localThreads);

  return 0;

}
