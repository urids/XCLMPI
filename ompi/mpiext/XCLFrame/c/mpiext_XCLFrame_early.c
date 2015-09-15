
#include "ompi_config.h"

#include "ompi/mpi/c/bindings.h"
#include "ompi/mpiext/mpiext.h"
#include "mpiext_XCLFrame_c.h"
#include "binding/dvMgmt/commsBench.h" //TODO: must this be here to keep architecture schema?


/* 
 * If an extension would like to have init/fini, in
 * addition to providing the hooks below, adding the line in
 * configure.m4  is also required.
 */


//these elements have extern linkage and lives in the runtime system memory

/*
 * Here we put the definition of global variables that CAN NOT be queried in the user application
 *
 */

/* ==================================
 * | INIT OF GLOBAL INITIALIZATIONS |
 * ==================================
 * */

CLxplorInfo clXplr; //Global Variable Declared in deviceExploration.h
cpudev*   cpu; // Global Variable declared in localDevices.h
gpudev*   gpu; // Global Variable declared in localDevices.h
acceldev* accel; // Global Variable declared in localDevices.h

/* ==================================
 * | END OF GLOBAL INITIALIZATIONS  |
 * ==================================
 * */




static int XCLFrame_init(void)
{
    void *dvMgmt_dlhandle;

	void (*deviceXploration)(CLxplorInfo *);
	void (*initializeDevices)(CLxplorInfo*);
	int (*commsBenchmark)(commsInfo*);


	char *error;

		dvMgmt_dlhandle = dlopen ("libdvMgmt.so", RTLD_LAZY);

	if (!dvMgmt_dlhandle ) {
		printf("Details:");
		fputs(dlerror(), stderr);
		exit(1);
	}

	deviceXploration = dlsym(dvMgmt_dlhandle, "deviceXploration");
	initializeDevices = dlsym(dvMgmt_dlhandle, "initializeDevices");
	commsBenchmark=dlsym(dvMgmt_dlhandle, "commsBenchmark");

	if ((error = dlerror()) != NULL ) {
		fputs(error, stderr);
		exit(1);
	}



    (*deviceXploration)(&clXplr);
    (*initializeDevices)(&clXplr);


    //After each device has been initialized we can perform communication benchmarks.
    commsInfo cmf;
    cmf.BW_Mtx=NULL;
    //(*commsBenchmark)(&cmf);
    //printf("%8.8f",cmf.BW_Mtx[0]);


    dlclose(dvMgmt_dlhandle);

    
    return OMPI_SUCCESS;
}

static int XCLFrame_fini(void)
{
       return OMPI_SUCCESS;
}

/*
 * Similar to Open MPI components, a well-known struct provides
 * function pointers to the extension's init/fini hooks.  The struct
 * must be a global symbol of the form ompi_mpiext_<ext_name> and be
 * of type ompi_mpiext_component_t.
 */


ompi_mpiext_component_t ompi_mpiext_XCLFrame = {
    XCLFrame_init,
    XCLFrame_fini
};
