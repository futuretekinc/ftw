/******************************************************************************
*
*               COPYRIGHT (c) 2008-2010 GainSpan Corporation
*                         All Rights Reserved
*
* The source code contained or described herein and all documents
* related to the source code ("Material") are owned by GainSpan
* Corporation or its licensors.  Title to the Material remains
* with GainSpan Corporation or its suppliers and licensors.
*
* The Material is protected by worldwide copyright and trade secret
* laws and treaty provisions. No part of the Material may be used,
* copied, reproduced, modified, published, uploaded, posted, transmitted,
* distributed, or disclosed in any way except in accordance with the
* applicable license agreement.
*
* No license under any patent, copyright, trade secret or other
* intellectual property right is granted to or conferred upon you by
* disclosure or delivery of the Materials, either expressly, by
* implication, inducement, estoppel, except in accordance with the
* applicable license agreement.
*
* Unless otherwise agreed by GainSpan in writing, you may not remove or
* alter this notice or any other notice embedded in Materials by GainSpan
* or GainSpan's suppliers or licensors in any way.
*
* $RCSfile: s2w_os_specific.h,v $
*
* Description : This file defines s2w specific structure.
*****************************************************************************/

#ifndef FTW_OS_SPECIFIC_H
#define FTW_OS_SPECIFIC_H

/*****************************************************************************
  * File Inclusions
  ****************************************************************************/
//#include "gsn_soft_timer.h"
#include "gsn_includes.h"

typedef struct FTW_TIMER
{
    UINT8 inited;
    UINT32 timeout;
    void (*handler)(void *);
    void *data;
    GSN_SOFT_TMR_T timer;
    GSN_SOFT_TMR_HANDLE_T handle;
} FTW_TIMER_T;


#endif
