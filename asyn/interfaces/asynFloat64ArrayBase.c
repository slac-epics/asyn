/*  asynFloat64ArrayBase.c */
/***********************************************************************
* Copyright (c) 2002 The University of Chicago, as Operator of Argonne
* National Laboratory, and the Regents of the University of
* California, as Operator of Los Alamos National Laboratory, and
* Berliner Elektronenspeicherring-Gesellschaft m.b.H. (BESSY).
* asynDriver is distributed subject to a Software License Agreement
* found in file LICENSE that is included with this distribution.
***********************************************************************/

/*  11-OCT-2004 Marty Kraimer
*/

#include <asynDriver.h>
#include <epicsTypes.h>

#define epicsExportSharedSymbols

#include "asynFloat64Array.h"

static asynStatus initialize(const char *portName, asynInterface *pfloat64ArrayInterface);

static asynFloat64ArrayBase float64ArrayBase = {initialize};
epicsShareDef asynFloat64ArrayBase *pasynFloat64ArrayBase = &float64ArrayBase;

static asynStatus writeDefault(void *drvPvt, asynUser *pasynUser,
                               epicsFloat64 *value, size_t nelem);
static asynStatus readDefault(void *drvPvt, asynUser *pasynUser,
                              epicsFloat64 *value, size_t nelem, size_t *nIn);
static asynStatus registerInterruptUser(void *drvPvt,asynUser *pasynUser,
                               interruptCallbackFloat64Array callback, void *userPvt,
                               void **registrarPvt);
static asynStatus cancelInterruptUser(void *registrarPvt, asynUser *pasynUser);


asynStatus initialize(const char *portName, asynInterface *pfloat64ArrayInterface)
{
    asynFloat64Array *pasynFloat64Array = (asynFloat64Array *)pfloat64ArrayInterface->pinterface;

    if(!pasynFloat64Array->write) pasynFloat64Array->write = writeDefault;
    if(!pasynFloat64Array->read) pasynFloat64Array->read = readDefault;
    if(!pasynFloat64Array->registerInterruptUser)
        pasynFloat64Array->registerInterruptUser = registerInterruptUser;
    if(!pasynFloat64Array->cancelInterruptUser) 
        pasynFloat64Array->cancelInterruptUser = cancelInterruptUser;
    return pasynManager->registerInterface(portName,pfloat64ArrayInterface);
}

static asynStatus writeDefault(void *drvPvt, asynUser *pasynUser,
    epicsFloat64 *value, size_t nelem)
{
    const char *portName;
    asynStatus status;
    int        addr;
    
    status = pasynManager->getPortName(pasynUser,&portName);
    if(status!=asynSuccess) return status;
    status = pasynManager->getAddr(pasynUser,&addr);
    if(status!=asynSuccess) return status;
    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
        "write is not supported\n");
    asynPrint(pasynUser,ASYN_TRACE_ERROR,
        "%s %d write is not supported\n",portName,addr);
    return asynError;
}

static asynStatus readDefault(void *drvPvt, asynUser *pasynUser,
    epicsFloat64 *value, size_t nelem, size_t *nIn)
{
    const char *portName;
    asynStatus status;
    int        addr;
    
    status = pasynManager->getPortName(pasynUser,&portName);
    if(status!=asynSuccess) return status;
    status = pasynManager->getAddr(pasynUser,&addr);
    if(status!=asynSuccess) return status;
    epicsSnprintf(pasynUser->errorMessage,pasynUser->errorMessageSize,
        "write is not supported\n");
    asynPrint(pasynUser,ASYN_TRACE_ERROR,
        "%s %d read is not supported\n",portName,addr);
    return asynError;
}


static asynStatus registerInterruptUser(void *drvPvt,asynUser *pasynUser,
                               interruptCallbackFloat64Array callback, void *userPvt,
                               void **registrarPvt)
{
    const char *portName;
    asynStatus status;
    int        addr;
    interruptNode *pinterruptNode;
    asynFloat64ArrayInterrupt *pasynFloat64ArrayInterrupt;
    void *pinterruptPvt;

    status = pasynManager->getPortName(pasynUser,&portName);
    if(status!=asynSuccess) return status;
    status = pasynManager->getAddr(pasynUser,&addr);
    if(status!=asynSuccess) return status;
    status = pasynManager->getInterruptPvt(portName, asynFloat64ArrayType,
                                           &pinterruptPvt);
    if(status!=asynSuccess) return status;
    pasynFloat64ArrayInterrupt = pasynManager->memMalloc(
                                             sizeof(asynFloat64ArrayInterrupt));
    pinterruptNode = pasynManager->createInterruptNode(pinterruptPvt);
    if(status!=asynSuccess) return status;
    pasynFloat64ArrayInterrupt = (asynFloat64ArrayInterrupt *)pinterruptNode->drvPvt;
    pasynFloat64ArrayInterrupt->addr = addr;
    pasynFloat64ArrayInterrupt->reason = pasynUser->reason;
    pasynFloat64ArrayInterrupt->drvUser = pasynUser->drvUser;
    pasynFloat64ArrayInterrupt->callback = callback;
    pasynFloat64ArrayInterrupt->userPvt = userPvt;
    *registrarPvt = pinterruptNode;
    asynPrint(pasynUser,ASYN_TRACE_FLOW,
        "%s %d registerInterruptUser\n",portName,addr);
    return pasynManager->addInterruptUser(pinterruptNode);
}

static asynStatus cancelInterruptUser(void *registrarPvt, asynUser *pasynUser)
{
    interruptNode *pinterruptNode = (interruptNode *)registrarPvt;
    asynStatus status;
    const char *portName;
    int        addr;
    
    status = pasynManager->getPortName(pasynUser,&portName);
    if(status!=asynSuccess) return status;
    status = pasynManager->getAddr(pasynUser,&addr);
    if(status!=asynSuccess) return status;
    asynPrint(pasynUser,ASYN_TRACE_FLOW,
        "%s %d cancelInterruptUser\n",portName,addr);
    pasynManager->memFree(pinterruptNode->drvPvt, sizeof(asynFloat64ArrayInterrupt));
    status = pasynManager->removeInterruptUser(pinterruptNode);
    return status;
}