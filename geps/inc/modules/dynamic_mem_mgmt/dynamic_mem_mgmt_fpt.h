/*******************************************************************************
*
*               COPYRIGHT (c) 2012-2013 GainSpan Corporation
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
* $RCSfile: dynamic_mem_mgmt_fpt.h,v $
*
* Description : This file contains the patch type defination.
*******************************************************************************/
#ifndef _GSN_DYNAMIC_MEM_MGMT_FPT_
#define _GSN_DYNAMIC_MEM_MGMT_FPT_

typedef VOID* (*GsnDynMemMgmt_Alloc_FP_T)( UINT32 size, UINT32 line, UINT32 moduleId );
extern _RPST_ GsnDynMemMgmt_Alloc_FP_T GsnDynMemMgmt_Alloc_FP;

typedef VOID* (*GsnDynMemMgmt_Realloc_FP_T)( VOID* ptr, UINT32 size, UINT32 line, UINT32 moduleId );
extern _RPST_ GsnDynMemMgmt_Realloc_FP_T GsnDynMemMgmt_Realloc_FP;

typedef VOID (*GsnDynMemMgmt_Free_FP_T)( VOID *ptr );
extern _RPST_ GsnDynMemMgmt_Free_FP_T GsnDynMemMgmt_Free_FP;

#endif /* _GSN_DYNAMIC_MEM_MGMT_FPT_ */