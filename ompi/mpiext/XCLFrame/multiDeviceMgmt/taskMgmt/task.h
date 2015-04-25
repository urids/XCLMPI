/*
 * task.h
 *
 *  Created on: May 7, 2014
 *      Author: uriel
 */

#ifndef TASK_H_
#define TASK_H_

#include "CL/cl.h"
#include "../deviceMgmt/Device.h"

#include "taskProcedures.h"


typedef struct Xcode_st{
	char *source_str;
	size_t source_size;
}XCLcode;


typedef struct Xbuffer_st{
	int bufferMode;
	size_t size;
	void ** data;

}XCLbuffer;


typedef struct Xkernel_st{
	cl_kernel kernel;
	cl_uint numArgs;
	char* kernelName;
	//int** argsLengths;
	//void** argsValues;

}XCLkernel;

typedef struct XCLtask_st{

	int localID;

	//every task MUST have an associated device
	Device* device;

	//every task MUST have its code
	XCLcode* code;
	cl_program* CLprogram;
	//and have their kernel List
	int numKernels; //must be passed as parameter at task startup TODO: will be useful for multiKernels.
	XCLkernel* kernel;

	XCLbuffer *entitiesbuffer; //buffer for the entities assigned to this task.

}XCLtask;


extern XCLtask* taskList; //This will be defined in late.c and is property of the runtime system.


#endif /* TASK_H_ */
