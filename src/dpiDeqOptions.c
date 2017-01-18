//-----------------------------------------------------------------------------
// Copyright (c) 2016, 2017 Oracle and/or its affiliates.  All rights reserved.
// This program is free software: you can modify it and/or redistribute it
// under the terms of:
//
// (i)  the Universal Permissive License v 1.0 or at your option, any
//      later version (http://oss.oracle.com/licenses/upl); and/or
//
// (ii) the Apache License v 2.0. (http://www.apache.org/licenses/LICENSE-2.0)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// dpiDeqOptions.c
//   Implementation of AQ dequeue options.
//-----------------------------------------------------------------------------

#include "dpiImpl.h"

//-----------------------------------------------------------------------------
// dpiDeqOptions__create() [INTERNAL]
//   Create a new subscription structure and return it. In case of error NULL
// is returned.
//-----------------------------------------------------------------------------
int dpiDeqOptions__create(dpiDeqOptions *options, dpiConn *conn,
        dpiError *error)
{
    sword status;

    // retain a reference to the connection
    if (dpiGen__setRefCount(conn, error, 1) < 0)
        return DPI_FAILURE;
    options->conn = conn;

    // create the OCI handle
    status = OCIDescriptorAlloc(conn->env->handle, (dvoid**) &options->handle,
            OCI_DTYPE_AQDEQ_OPTIONS, 0, 0);
    return dpiError__check(error, status, options->conn,
            "allocate descriptor");
}


//-----------------------------------------------------------------------------
// dpiDeqOptions__free() [INTERNAL]
//   Free the memory for a dequeue options structure.
//-----------------------------------------------------------------------------
void dpiDeqOptions__free(dpiDeqOptions *options, dpiError *error)
{
    if (options->handle) {
        OCIDescriptorFree(options->handle, OCI_DTYPE_AQDEQ_OPTIONS);
        options->handle = NULL;
    }
    if (options->conn) {
        dpiGen__setRefCount(options->conn, error, -1);
        options->conn = NULL;
    }
    free(options);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions__getAttrValue() [INTERNAL]
//   Get the attribute value in OCI.
//-----------------------------------------------------------------------------
static int dpiDeqOptions__getAttrValue(dpiDeqOptions *options,
        uint32_t attribute, const char *fnName, dvoid *value,
        uint32_t *valueLength)
{
    dpiError error;
    sword status;

    if (dpiGen__startPublicFn(options, DPI_HTYPE_DEQ_OPTIONS, fnName,
            &error) < 0)
        return DPI_FAILURE;
    status = OCIAttrGet(options->handle, OCI_DTYPE_AQDEQ_OPTIONS, value,
            valueLength, attribute, error.handle);
    return dpiError__check(&error, status, options->conn,
            "get attribute value");
}


//-----------------------------------------------------------------------------
// dpiDeqOptions__setAttrValue() [INTERNAL]
//   Set the attribute value in OCI.
//-----------------------------------------------------------------------------
static int dpiDeqOptions__setAttrValue(dpiDeqOptions *options,
        uint32_t attribute, const char *fnName, const void *value,
        uint32_t valueLength)
{
    dpiError error;
    sword status;

    if (dpiGen__startPublicFn(options, DPI_HTYPE_DEQ_OPTIONS, fnName,
            &error) < 0)
        return DPI_FAILURE;
    status = OCIAttrSet(options->handle, OCI_DTYPE_AQDEQ_OPTIONS,
            (dvoid*) value, valueLength, attribute, error.handle);
    return dpiError__check(&error, status, options->conn,
            "set attribute value");
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_addRef() [PUBLIC]
//   Add a reference to the dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_addRef(dpiDeqOptions *options)
{
    return dpiGen__addRef(options, DPI_HTYPE_DEQ_OPTIONS, __func__);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getCondition() [PUBLIC]
//   Return condition associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getCondition(dpiDeqOptions *options, const char **value,
        uint32_t *valueLength)
{
    return dpiDeqOptions__getAttrValue(options, OCI_ATTR_DEQCOND, __func__,
            (void*) value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getConsumerName() [PUBLIC]
//   Return consumer name associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getConsumerName(dpiDeqOptions *options, const char **value,
        uint32_t *valueLength)
{
    return dpiDeqOptions__getAttrValue(options, OCI_ATTR_CONSUMER_NAME,
            __func__, (void*) value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getCorrelation() [PUBLIC]
//   Return correlation associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getCorrelation(dpiDeqOptions *options, const char **value,
        uint32_t *valueLength)
{
    return dpiDeqOptions__getAttrValue(options, OCI_ATTR_CORRELATION,
            __func__, (void*) value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getMode() [PUBLIC]
//   Return mode associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getMode(dpiDeqOptions *options, dpiDeqMode *value)
{
    uint32_t ociValue;

    if (dpiDeqOptions__getAttrValue(options, OCI_ATTR_DEQ_MODE, __func__,
            &ociValue, NULL) < 0)
        return DPI_FAILURE;
    *value = ociValue;
    return DPI_SUCCESS;
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getMsgId() [PUBLIC]
//   Return message id associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getMsgId(dpiDeqOptions *options, const char **value,
        uint32_t *valueLength)
{
    OCIRaw *rawValue;

    if (dpiDeqOptions__getAttrValue(options, OCI_ATTR_DEQ_MSGID, __func__,
            &rawValue, NULL) < 0)
        return DPI_FAILURE;
    *value = (const char*) OCIRawPtr(options->env->handle, rawValue);
    *valueLength = OCIRawSize(options->env->handle, rawValue);
    return DPI_SUCCESS;
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getNavigation() [PUBLIC]
//   Return navigation associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getNavigation(dpiDeqOptions *options,
        dpiDeqNavigation *value)
{
    uint32_t ociValue;

    if (dpiDeqOptions__getAttrValue(options, OCI_ATTR_NAVIGATION, __func__,
            &ociValue, NULL) < 0)
        return DPI_FAILURE;
    *value = ociValue;
    return DPI_SUCCESS;
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getTransformation() [PUBLIC]
//   Return transformation associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getTransformation(dpiDeqOptions *options, const char **value,
        uint32_t *valueLength)
{
    return dpiDeqOptions__getAttrValue(options, OCI_ATTR_TRANSFORMATION,
            __func__, (void*) value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getVisibility() [PUBLIC]
//   Return visibility associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getVisibility(dpiDeqOptions *options, dpiVisibility *value)
{
    uint32_t ociValue;

    if (dpiDeqOptions__getAttrValue(options, OCI_ATTR_VISIBILITY,
            __func__, &ociValue, NULL) < 0)
        return DPI_FAILURE;
    *value = ociValue;
    return DPI_SUCCESS;
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_getWait() [PUBLIC]
//   Return the number of seconds to wait for a message when dequeuing.
//-----------------------------------------------------------------------------
int dpiDeqOptions_getWait(dpiDeqOptions *options, uint32_t *value)
{
    return dpiDeqOptions__getAttrValue(options, OCI_ATTR_WAIT, __func__, value,
            NULL);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_release() [PUBLIC]
//   Release a reference to the dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_release(dpiDeqOptions *options)
{
    return dpiGen__release(options, DPI_HTYPE_DEQ_OPTIONS, __func__);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setCondition() [PUBLIC]
//   Set condition associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setCondition(dpiDeqOptions *options, const char *value,
        uint32_t valueLength)
{
    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_DEQCOND, __func__,
            value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setConsumerName() [PUBLIC]
//   Set consumer name associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setConsumerName(dpiDeqOptions *options, const char *value,
        uint32_t valueLength)
{
    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_CONSUMER_NAME,
            __func__, value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setCorrelation() [PUBLIC]
//   Set correlation associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setCorrelation(dpiDeqOptions *options, const char *value,
        uint32_t valueLength)
{
    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_CORRELATION, __func__,
            value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setDeliveryMode() [PUBLIC]
//   Set the delivery mode associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setDeliveryMode(dpiDeqOptions *options,
        dpiMessageDeliveryMode value)
{
    uint16_t ociValue = value;

    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_MSG_DELIVERY_MODE,
            __func__, &ociValue, 0);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setMode() [PUBLIC]
//   Set the mode associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setMode(dpiDeqOptions *options, dpiDeqMode value)
{
    uint32_t ociValue = value;

    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_DEQ_MODE, __func__,
            &ociValue, 0);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setMsgId() [PUBLIC]
//   Set the message id associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setMsgId(dpiDeqOptions *options, const char *value,
        uint32_t valueLength)
{
    OCIRaw *rawValue = NULL;
    dpiError error;
    sword status;

    if (dpiGen__startPublicFn(options, DPI_HTYPE_DEQ_OPTIONS, __func__,
            &error) < 0)
        return DPI_FAILURE;
    status = OCIRawAssignBytes(options->env->handle, error.handle,
            (const ub1*) value, valueLength, &rawValue);
    if (dpiError__check(&error, status, options->conn, "set raw buffer") < 0)
        return DPI_FAILURE;
    status = OCIAttrSet(options->handle, OCI_DTYPE_AQDEQ_OPTIONS,
            (dvoid*) rawValue, valueLength, OCI_ATTR_DEQ_MSGID, error.handle);
    OCIRawResize(options->env->handle, error.handle, 0, &rawValue);
    return dpiError__check(&error, status, options->conn, "set value");
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setNavigation() [PUBLIC]
//   Set navigation associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setNavigation(dpiDeqOptions *options, dpiDeqNavigation value)
{
    uint32_t ociValue = value;

    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_NAVIGATION, __func__,
            &ociValue, 0);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setTransformation() [PUBLIC]
//   Set transformation associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setTransformation(dpiDeqOptions *options, const char *value,
        uint32_t valueLength)
{
    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_TRANSFORMATION,
            __func__, value, valueLength);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setVisibility() [PUBLIC]
//   Set visibility associated with dequeue options.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setVisibility(dpiDeqOptions *options, dpiVisibility value)
{
    uint32_t ociValue = value;

    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_VISIBILITY, __func__,
            &ociValue, 0);
}


//-----------------------------------------------------------------------------
// dpiDeqOptions_setWait() [PUBLIC]
//   Set the number of seconds to wait for a message when dequeuing.
//-----------------------------------------------------------------------------
int dpiDeqOptions_setWait(dpiDeqOptions *options, uint32_t value)
{
    return dpiDeqOptions__setAttrValue(options, OCI_ATTR_WAIT, __func__,
            &value, 0);
}

