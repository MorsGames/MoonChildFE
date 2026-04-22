/*==========================================================================
 *
 *  Copyright (C) 1995-1996 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       ffent.h
 *  Content:    FastFile entry definition (internal)
 *
 * THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
 * EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTBILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
 *
 ***************************************************************************/

// set DO_XORDATA to 1 to XOR all data in the fastfile with the value XORVALUE
#define DO_XORDATA      0
#define XORVALUE        0xFF

typedef struct {
    int        offset;
    char        name[16];
} FILEENTRY, *LPFILEENTRY;


