/***************************************************************************

    file                 : jz.cpp
    created              : Thu Jan 21 11:41:38 CET 2016
    copyright            : (C) 2002 Jure Zbontar

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifdef _WIN32
#include <windows.h>
#endif

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <math.h>
#include <err.h>

#include <tgf.h> 
#include <track.h> 
#include <car.h> 
#include <raceman.h> 
#include <robottools.h>
#include <robot.h>

#include <tgfclient.h>

extern "C" {
	#include "lua.h"
	#include "lualib.h"
	#include "lauxlib.h"
}
#include "luaT.h"
#include "TH.h"

static tTrack	*curTrack;

static void initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s); 
static void newrace(int index, tCarElt* car, tSituation *s); 
static void drive(int index, tCarElt* car, tSituation *s); 
static void endrace(int index, tCarElt *car, tSituation *s);
static void shutdown(int index);
static int  InitFuncPt(int index, void *pt); 

static lua_State *L = NULL;

/* 
 * Module entry point  
 */ 
extern "C" int 
jz(tModInfo *modInfo) 
{
	L = luaL_newstate();
	luaL_openlibs(L);
	if (luaL_loadfile(L, "/home/jure/build/torcs-1.3.6/src/drivers/jz/main.lua")) {
		err(0, "luaL_loadfile");
	}
	if (lua_pcall(L, 0, 0, 0)) {
		err(0, "lua_pcall");
	}

    memset(modInfo, 0, 10*sizeof(tModInfo));

    modInfo->name    = strdup("jz");		/* name of the module (short) */
    modInfo->desc    = strdup("");	/* description of the module (can be long) */
    modInfo->fctInit = InitFuncPt;		/* init function */
    modInfo->gfId    = ROB_IDENT;		/* supported framework version */
    modInfo->index   = 1;

    return 0; 
} 

/* Module interface initialization. */
static int 
InitFuncPt(int index, void *pt) 
{ 
    tRobotItf *itf  = (tRobotItf *)pt; 

    itf->rbNewTrack = initTrack; /* Give the robot the track view called */ 
				 /* for every track change or new race */ 
    itf->rbNewRace  = newrace; 	 /* Start a new race */
    itf->rbDrive    = drive;	 /* Drive during race */
    itf->rbPitCmd   = NULL;
    itf->rbEndRace  = endrace;	 /* End of the current race */
    itf->rbShutdown = shutdown;	 /* Called before the module is unloaded */
    itf->index      = index; 	 /* Index used if multiple interfaces */
    return 0; 
} 

/* Called for every track change or new race. */
static void  
initTrack(int index, tTrack* track, void *carHandle, void **carParmHandle, tSituation *s) 
{ 
    curTrack = track;
    *carParmHandle = NULL; 
} 

/* Start a new race. */
static void  
newrace(int index, tCarElt* car, tSituation *s) 
{ 
} 

extern tRmInfo *ReInfo;
void reMovieCapture(void *);

#define WIDTH 160
#define HEIGHT 120
unsigned char img[3 * WIDTH * HEIGHT];
unsigned long long tick1;

/* Drive during race. */
static void  
drive(int index, tCarElt* car, tSituation *s) 
{ 
	float angle;

	glReadPixels(0, 0, WIDTH, WIDTH, GL_RGB, GL_UNSIGNED_BYTE, (GLvoid*)img);

	THByteStorage *storage = THByteStorage_newWithData(img, 3 * WIDTH * HEIGHT);
	THByteTensor *tensor = THByteTensor_newWithStorage1d(storage, 0, 3 * WIDTH * HEIGHT, 1);
	lua_getglobal(L, "drive");
	luaT_pushudata(L, (void *)tensor, "torch.ByteTensor");
	lua_pcall(L, 1, 1, 0);
	angle = lua_tonumber(L, -1);

	memset((void *)&car->ctrl, 0, sizeof(tCarCtrl)); 
	car->ctrl.steer = angle;
	car->ctrl.gear = 1;
	car->ctrl.accelCmd = 0.3;
	car->ctrl.brakeCmd = 0.0;

/*
	angle = RtTrackSideTgAngleL(&(car->_trkPos)) - car->_yaw;
	NORM_PI_PI(angle);
	angle -= (car->_trkPos.toMiddle / car->_trkPos.seg->width);

	// predict with nn
	car->targets[0] = angle / car->_steerLock;

	// actual driving command
	float d = sin((double)tick1 / 500) / 2.;
	angle = (angle + d) / car->_steerLock;

	memset((void *)&car->ctrl, 0, sizeof(tCarCtrl)); 
	car->ctrl.steer = angle;
	car->ctrl.gear = 1;
	car->ctrl.accelCmd = 0.3;
	car->ctrl.brakeCmd = 0.0;
*/

	tick1++;
}


/* End of the current race */
static void
endrace(int index, tCarElt *car, tSituation *s)
{
}

/* Called before the module is unloaded */
static void
shutdown(int index)
{
}

