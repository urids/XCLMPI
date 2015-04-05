
#ifndef __dvMgmt_H__
#define __dvMgmt_H__

#include <stdlib.h>
#include <stdio.h>
#include <dlfcn.h>

#include "../../../multiDeviceMgmt/deviceMgmt/Device.h"
#include "../../../multiDeviceMgmt/deviceMgmt/deviceExploration.h"
#include "../../../multiDeviceMgmt/deviceMgmt/devicesInitialization.h"

void deviceXploration(CLxplorInfo * clXplr);
void initializeDevices(CLxplorInfo*  xplorInfo);

#endif //__dvMgmt_H__
