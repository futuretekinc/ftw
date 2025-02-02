/*******************************************************************************
 *
 *               COPYRIGHT (c) 2009-2010 GainSpan Corporation
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
 * $RCSfile: app_fwup.c,v $
 *
 * Description : s2w GEPS interface file
 *******************************************************************************/
/*******************************************************************************
 * File Inclusions
 ******************************************************************************/
#ifdef ADK_OTAFU_ENABLE
#include "gsn_otafu.h"
#endif
#include "gsn_ncm.h"
#include "config/app_cfg.h"
#include "fs_if/app_fsIf.h"
#include "battery/app_batt_chk.h"
#include "main/app_main_ctx.h"
#include "app_events.h"
#include "app_debug.h"

#ifdef ADK_OTAFU_ENABLE
GSN_OTAFU_CTX_T otafuCtx;

PUBLIC VOID
AppOtafu_Cb(VOID *ctx, UINT8 event)
{
    if (event == GSN_OTAFU_EVENT_FWUP_SUCCESS)
    {
        UINT32 msg = APP_EVENT_FWUP_SUCCESS;
	//    APP_MAIN_CTX_T* appCtx = (APP_MAIN_CTX_T*) ctx;
	    /* Notify APP */
	    AppMainCtx_TaskNotify(msg,0);
    }
}
PUBLIC VOID AppOtafu_Init(APP_MAIN_CTX_T* pAppCtx)
{
    AppDbg_Printf("[%s] Entered\r\n", __FUNCTION__);
    otafuCtx.cbFn =  AppOtafu_Cb;
    otafuCtx.cbFnCtx = pAppCtx;
    GsnOtafu_Init(&otafuCtx);
}
#endif
