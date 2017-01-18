//-----------------------------------------------------------------------------
// Copyright (c) 2016 Oracle and/or its affiliates.  All rights reserved.
// This program is free software: you can modify it and/or redistribute it
// under the terms of:
//
// (i)  the Universal Permissive License v 1.0 or at your option, any
//      later version (http://oss.oracle.com/licenses/upl); and/or
//
// (ii) the Apache License v 2.0. (http://www.apache.org/licenses/LICENSE-2.0)
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// TestConvertNumbers.c
//   Tests conversion of numbers to strings and strings to numbers.
//-----------------------------------------------------------------------------

#include "Test.h"

#define SQL_TEXT                        "select :1 from dual"

static const char *numbersToConvert[] = {
    "0",
    "1",
    "-1",
    "10",
    "-10",
    "100",
    "-100",
    "0.1",
    "-0.1",
    "0.01",
    "-0.01",
    "0.001",
    "-0.001",
    ".100004",
    "-.100004",
    "1234567890123456789012345678901234567891",
    "-1234567890123456789012345678901234567891",
    "1.2345E20",
    "-1.2345E+20",
    "9e125",
    "-9e125",
    "9e-130",
    "-9e-130",
    NULL
};

//-----------------------------------------------------------------------------
// main()
//-----------------------------------------------------------------------------
int main(int argc, char **argv)
{
    uint32_t numQueryColumns, bufferRowIndex, ix;
    dpiData *inputValue, *outputValue;
    dpiVar *inputVar, *outputVar;
    const char *inputStringValue;
    dpiStmt *stmt;
    dpiConn *conn;
    int found;

    // connect to database
    conn = GetConnection(1, NULL);
    if (!conn)
        return -1;

    // create variables for the input and output values
    if (dpiConn_newVar(conn, DPI_ORACLE_TYPE_NUMBER, DPI_NATIVE_TYPE_BYTES,
            1, 0, 0, 0, NULL, &inputVar, &inputValue) < 0)
        return ShowError();
    if (dpiConn_newVar(conn, DPI_ORACLE_TYPE_NUMBER, DPI_NATIVE_TYPE_BYTES,
            1, 0, 0, 0, NULL, &outputVar, &outputValue) < 0)
        return ShowError();

    // prepare and execute statement for each of the numbers to convert
    if (dpiConn_prepareStmt(conn, 0, SQL_TEXT, strlen(SQL_TEXT), NULL, 0,
            &stmt) < 0)
        return ShowError();
    if (dpiStmt_setFetchArraySize(stmt, 1) < 0)
        return ShowError();
    if (dpiStmt_bindByPos(stmt, 1, inputVar) < 0)
        return ShowError();

    // perform query for each string in the array
    ix = 0;
    while (1) {
        inputStringValue = numbersToConvert[ix++];
        if (!inputStringValue)
            break;
        printf(" INPUT: |%s|\n", inputStringValue);

        if (dpiVar_setFromBytes(inputVar, 0, inputStringValue,
                strlen(inputStringValue)) < 0)
            return ShowError();
        if (dpiStmt_execute(stmt, 0, &numQueryColumns) < 0)
            return ShowError();
        if (dpiStmt_define(stmt, 1, outputVar) < 0)
            return ShowError();

        // fetch rows
        while (1) {
            if (dpiStmt_fetch(stmt, &found, &bufferRowIndex) < 0)
                return ShowError();
            if (!found)
                break;
            printf("OUTPUT: |%.*s|\n", outputValue->value.asBytes.length,
                    outputValue->value.asBytes.ptr);
        }

    }

    // clean up
    dpiVar_release(inputVar);
    dpiVar_release(outputVar);
    dpiStmt_release(stmt);
    dpiConn_release(conn);

    printf("Done.\n");
    return 0;
}

