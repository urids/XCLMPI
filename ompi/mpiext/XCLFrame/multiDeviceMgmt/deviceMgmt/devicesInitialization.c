/*
 * devicesInitialization.c
 *
 *  Created on: May 9, 2014
 *      Author: uriel
 */
#include "devicesInitialization.h"


int createContext(Device* device) {
	int status;
	device->cprops[0] = CL_CONTEXT_PLATFORM;
	device->cprops[1] = (cl_context_properties) device->platform;
	device->cprops[2] = 0;

	device->context = clCreateContext(device->cprops, 1, &device->deviceId, 0,
			0, &status);
	chkerr(status, "Building Context", __FILE__, __LINE__);
	return status;
}


int createQueue(Device* device) {
	int status;
	device->queue = clCreateCommandQueue(device->context, device->deviceId,
			CL_QUEUE_PROFILING_ENABLE, &status);
	chkerr(status, "Building Queue", __FILE__, __LINE__);
	return status;
}



void devicesInitialization(CLxplorInfo* xpInfo){
	int i,j;
	cl_uint deviceCount;

	   cpu = malloc(xpInfo->numCPUS * sizeof(Device*));
	   gpu = malloc(xpInfo->numGPUS * sizeof(Device*));
	   accel=malloc(xpInfo->numACCEL * sizeof(Device*));

	int m = 0, n = 0, o = 0;
	for (i = 0; i < xpInfo->platformCount; i++) {
		clGetDeviceIDs(xpInfo->platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
		for (j = 0; j < deviceCount; j++) {

			cl_device_type devType;

			clGetDeviceInfo(xpInfo->devices[i][j], CL_DEVICE_TYPE,
					sizeof(cl_device_type), &devType, NULL );



			if (CL_DEVICE_TYPE_CPU == devType) {
				cpu[m] = malloc(sizeof(Device));
				cpu[m]->deviceId = xpInfo->devices[i][j];
				cpu[m]->platform = (cl_context_properties) xpInfo->platforms[i];
				createContext(cpu[m]);
				createQueue(cpu[m]);
				m++;
			}
			if (CL_DEVICE_TYPE_GPU == devType) {
				gpu[n] = malloc(sizeof(Device));
				gpu[n]->deviceId = xpInfo->devices[i][j];
				gpu[n]->platform = (cl_context_properties) xpInfo->platforms[i];
				createContext(gpu[n]);
				createQueue(gpu[n]);
				n++;
			}
			if (CL_DEVICE_TYPE_ACCELERATOR == devType) {
				accel[o] = malloc(sizeof(Device));
				accel[o]->deviceId = xpInfo->devices[i][j];
				accel[o]->platform = (cl_context_properties) xpInfo->platforms[i];
				createContext(accel[o]);
				createQueue(accel[o]);
				o++;
			}
		}
	}


/* Demo printing proposes!!*/
	for (i = 0; i < xpInfo->platformCount; i++) {
		clGetDeviceIDs(xpInfo->platforms[i], CL_DEVICE_TYPE_ALL, 0, NULL, &deviceCount);
		for (j = 0; j < deviceCount; j++) {
			char *devVendor;
			size_t devVendorSz;
			clGetDeviceInfo(xpInfo->devices[i][j], CL_DEVICE_NAME, 0, NULL,
					&devVendorSz);
			devVendor = (char*) malloc(devVendorSz * sizeof(char));

			clGetDeviceInfo(xpInfo->devices[i][j], CL_DEVICE_NAME, devVendorSz, devVendor,
					NULL );

			printf("Device Name: %s \n", devVendor);

		}
	}

}
