/*
* Copyright (C) Huawei Technologies Co., Ltd. 2012-2015. All rights reserved.
* foss@huawei.com
*
* If distributed as part of the Linux kernel, the following license terms
* apply:
*
* * This program is free software; you can redistribute it and/or modify
* * it under the terms of the GNU General Public License version 2 and
* * only version 2 as published by the Free Software Foundation.
* *
* * This program is distributed in the hope that it will be useful,
* * but WITHOUT ANY WARRANTY; without even the implied warranty of
* * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* * GNU General Public License for more details.
* *
* * You should have received a copy of the GNU General Public License
* * along with this program; if not, write to the Free Software
* * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307, USA
*
* Otherwise, the following license terms apply:
*
* * Redistribution and use in source and binary forms, with or without
* * modification, are permitted provided that the following conditions
* * are met:
* * 1) Redistributions of source code must retain the above copyright
* *    notice, this list of conditions and the following disclaimer.
* * 2) Redistributions in binary form must reproduce the above copyright
* *    notice, this list of conditions and the following disclaimer in the
* *    documentation and/or other materials provided with the distribution.
* * 3) Neither the name of Huawei nor the names of its contributors may
* *    be used to endorse or promote products derived from this software
* *    without specific prior written permission.
*
* * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
* ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
* INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
* CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
* ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
* POSSIBILITY OF SUCH DAMAGE.
*
*/

/*****************************************************************************
  1 头文件包含
*****************************************************************************/
#include "AtParse.h"
#include "ATCmdProc.h"
#include "AtDeviceCmd.h"
#include "AtCheckFunc.h"
#include "mdrv.h"
#include "AtCmdMsgProc.h"
#include "AtInputProc.h"
#include "AtTestParaCmd.h"

#include "at_lte_common.h"
#include "nv_stru_lps.h"

#include "nv_stru_cas.h"

#include "AtCmdMiscProc.h"


#include <linux/random.h>

#include "mdrv_slt.h"


/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
#define    THIS_FILE_ID        PS_FILE_ID_AT_DEVICECMD_C


/*****************************************************************************
  2 全局变量定义
*****************************************************************************/

/* 命令受限标志宏定义说明
#define CMD_TBL_E5_IS_LOCKED        (0x00000001)     不受E5锁定控制的命令
#define CMD_TBL_PIN_IS_LOCKED       (0x00000002)     不受PIN码锁定控制的命令
#define CMD_TBL_IS_E5_DOCK          (0x00000004)     E5 DOCK命令
#define CMD_TBL_CLAC_IS_INVISIBLE   (0x00000008)     +CLAC命令中不输出显示的命令
*/

VOS_UINT32                 g_ulNVRD = 0;
VOS_UINT32                 g_ulNVWR = 0;

AT_PAR_CMD_ELEMENT_STRU g_astAtDeviceCmdTbl[] = {
    {AT_CMD_GTIMER,
    AT_SetGTimerPara,    AT_NOT_SET_TIME,    AT_QryGTimerPara,      AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^GTIMER",   (VOS_UINT8*)"(0-429496728)"},

    {AT_CMD_RSIM,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    AT_QryRsimPara,        AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^RSIM",     VOS_NULL_PTR},

    {AT_CMD_PHYNUM,
    AT_SetPhyNumPara,    AT_NOT_SET_TIME,    AT_QryPhyNumPara,      AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PHYNUM",   VOS_NULL_PTR},

    {AT_CMD_CSVER,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    At_QryCsVer,           AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^CSVER",    VOS_NULL_PTR},

    {AT_CMD_QOS,
    At_SetQosPara,       AT_NOT_SET_TIME,    At_QryQosPara,         AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
   AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^QOS",      VOS_NULL_PTR},

    {AT_CMD_SDOMAIN,
    At_SetSDomainPara,   AT_NOT_SET_TIME,    At_QrySDomainPara,     AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SDOMAIN",  VOS_NULL_PTR},

    {AT_CMD_PLATFORM,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    At_QryPlatForm,        AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PLATFORM", VOS_NULL_PTR},

    {AT_CMD_FCHAN,
    At_SetFChanPara,     AT_SET_PARA_TIME,   At_QryFChanPara,       AT_QRY_PARA_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_FCHAN_OTHER_ERR  ,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FCHAN",    (VOS_UINT8*)"(0-7),(0-255),(0-4294967295),(0-1)"},

    {AT_CMD_CLT,
    At_SetCltPara,     AT_SET_PARA_TIME,   VOS_NULL_PTR,       AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS  ,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^CLT",    (VOS_UINT8*)"(0,1)"},

    {AT_CMD_DCXOTEMPCOMP,
    AT_SetDcxotempcompPara,  AT_SET_PARA_TIME,   AT_QryDcxotempcompPara,   AT_QRY_PARA_TIME,    VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^DCXOTEMPCOMP",    (VOS_UINT8 *)"(0,1)"},

    {AT_CMD_FTXON,
    At_SetFTxonPara,     AT_SET_PARA_TIME,   At_QryFTxonPara,       AT_QRY_PARA_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_FTXON_OTHER_ERR  ,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FTXON",    (VOS_UINT8*)"(0,1,2)"},

    {AT_CMD_CLTINFO,
    VOS_NULL_PTR,       AT_NOT_SET_TIME,   At_QryCltInfo,       AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,       AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^CLTINFO",    VOS_NULL_PTR},

    {AT_CMD_FDAC,
    AT_SetFDac,          AT_SET_PARA_TIME,   AT_QryFDac,            AT_NOT_SET_TIME,    At_TestFdacPara , AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FDAC",     (VOS_UINT8*)"(0-65536)"},

    {AT_CMD_FRXON,
    At_SetFRxonPara,     AT_SET_PARA_TIME,   At_QryFRxonPara,       AT_QRY_PARA_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_FRXON_OTHER_ERR  ,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FRXON",    (VOS_UINT8*)"(0-1)"},

    {AT_CMD_FPA,
    At_SetFpaPara,       AT_SET_PARA_TIME,   At_QryFpaPara,         AT_NOT_SET_TIME,    VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_FPA_OTHER_ERR  ,      CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FPA",      (VOS_UINT8*)"(0-255)"},


    {AT_CMD_FLNA,
    At_SetFlnaPara,      AT_SET_PARA_TIME,   At_QryFlnaPara,        AT_QRY_PARA_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_FLNA_OTHER_ERR  ,     CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FLNA",     (VOS_UINT8*)"(0-255)"},

    {AT_CMD_FRSSI,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    At_QryFrssiPara,       AT_QRY_PARA_TIME,  VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FRSSI",    VOS_NULL_PTR},

    {AT_CMD_MDATE,
    AT_SetMDatePara,     AT_NOT_SET_TIME,    AT_QryMDatePara,       AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^MDATE",    (VOS_UINT8*)"(@time)"},

    {AT_CMD_FACINFO,
    AT_SetFacInfoPara,   AT_NOT_SET_TIME,    AT_QryFacInfoPara,     AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_ERROR,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FACINFO",  (VOS_UINT8*)"(0,1),(@valueInfo)"},


    {AT_CMD_SD,
    At_SetSD,            AT_NOT_SET_TIME,    At_QrySD,              AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_SD_CARD_OTHER_ERR ,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SD",       (VOS_UINT8*)"(0-4),(0-429496728),(0-3)"},

    {AT_CMD_GPIOPL,
    At_SetGPIOPL,        AT_SET_PARA_TIME,   At_QryGPIOPL,          AT_QRY_PARA_TIME,  VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^GPIOPL",   (VOS_UINT8*)"(@GPIOPL)"},


    /* 生产NV恢复 */
    {AT_CMD_INFORRS,
    At_SetInfoRRS,       AT_SET_PARA_TIME,    VOS_NULL_PTR,          AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^INFORRS",  VOS_NULL_PTR},

    {AT_CMD_DATALOCK,
    At_SetDataLock,      AT_SET_PARA_TIME,   At_QryDataLock,        AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^DATALOCK", (VOS_UINT8*)"(@nlockCode)"},

    {AT_CMD_VERSION,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    At_QryVersion,         AT_QRY_PARA_TIME,  VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^VERSION",  VOS_NULL_PTR},



    {AT_CMD_SIMLOCK,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    At_QrySimLockPlmnInfo, AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SIMLOCK",  VOS_NULL_PTR},

    {AT_CMD_MAXLCK_TIMES,
    At_SetMaxLockTimes,  AT_SET_PARA_TIME,   At_QryMaxLockTimes,    AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^MAXLCKTMS",(VOS_UINT8*)"(0-429496728)"},

    {AT_CMD_CALLSRV,
    At_SetCallSrvPara,   AT_NOT_SET_TIME,    At_QryCallSrvPara,     AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^CALLSRV",  (VOS_UINT8*)"(0,1)"},

    {AT_CMD_CSDFLT,
    At_SetCsdfltPara,    AT_NOT_SET_TIME,    At_QryCsdfltPara,      AT_QRY_PARA_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_ERROR,                       CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^CSDFLT",   (VOS_UINT8*)"(0,1)"},

    {AT_CMD_SECUBOOTFEATURE,
    VOS_NULL_PTR,    AT_NOT_SET_TIME,    At_QrySecuBootFeaturePara, AT_NOT_SET_TIME,  VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SECUBOOTFEATURE",  (VOS_UINT8*)"(0-3)"},

    {AT_CMD_SECUBOOT,
    At_SetSecuBootPara,  AT_SET_PARA_TIME,    At_QrySecuBootPara,    AT_QRY_PARA_TIME,  VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SECUBOOT", (VOS_UINT8*)"(0-3)"},

    {AT_CMD_TMMI,
    AT_SetTmmiPara,      AT_NOT_SET_TIME,    AT_QryTmmiPara,        AT_NOT_SET_TIME,   At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^TMMI",  (VOS_UINT8*)"(0,1)"},

    /* V7R1要求使用"=?"查询充电使能状态 */
    {AT_CMD_TCHRENABLE,
    AT_SetChrgEnablePara,AT_NOT_SET_TIME,    AT_QryChrgEnablePara,  AT_NOT_SET_TIME,   AT_TestChrgEnablePara, AT_SET_PARA_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^TCHRENABLE",(VOS_UINT8*)"(0,1,4)"},

    {AT_CMD_TCHRINFO,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    AT_QryChrgInfoPara,    AT_NOT_SET_TIME,   At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^TCHRINFO",VOS_NULL_PTR},

    {AT_CMD_TSCREEN,
    AT_SetTestScreenPara,AT_NOT_SET_TIME,    VOS_NULL_PTR,          AT_NOT_SET_TIME,   At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^TSCREEN",  (VOS_UINT8*)"(0-255),(0-255)"},

    {AT_CMD_BATVOL,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    AT_QryBatVolPara,      AT_QRY_PARA_TIME,  At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^TBATVOLT",  VOS_NULL_PTR},

    {AT_CMD_WUPWD,
    AT_SetWebPwdPara,    AT_NOT_SET_TIME,    VOS_NULL_PTR,          AT_NOT_SET_TIME,   At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WUPWD",  (VOS_UINT8*)"(0,1),(@WUPWD)"},

    {AT_CMD_PRODTYPE,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    AT_QryProdTypePara,    AT_QRY_PARA_TIME,  At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PRODTYPE",  VOS_NULL_PTR},

    {AT_CMD_FEATURE,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    AT_QryFeaturePara,     AT_QRY_PARA_TIME,  At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SFEATURE",  VOS_NULL_PTR},

    {AT_CMD_PRODNAME,
    AT_SetProdNamePara,  AT_NOT_SET_TIME,    AT_QryProdNamePara,    AT_NOT_SET_TIME,   At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PRODNAME",  (VOS_UINT8*)"(@ProductName)"},

    {AT_CMD_FWAVE,
    AT_SetFwavePara,     AT_SET_PARA_TIME,   VOS_NULL_PTR,          AT_NOT_SET_TIME,   At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FWAVE",  (VOS_UINT8*)"(0-7),(0-65535)"},

    {AT_CMD_EQVER,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,    AT_QryEqverPara,       AT_NOT_SET_TIME,   VOS_NULL_PTR,     AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^EQVER",  VOS_NULL_PTR},

    {AT_CMD_WIENABLE,
    AT_SetWiFiEnablePara, AT_NOT_SET_TIME, AT_QryWiFiEnablePara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIENABLE",(VOS_UINT8*)"(0,1,2)"},

    {AT_CMD_WIMODE,
    AT_SetWiFiModePara,   AT_NOT_SET_TIME, AT_QryWiFiModePara,   AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIMODE",  (VOS_UINT8*)"(0,1,2,3,4)"},

    {AT_CMD_WIBAND,
    AT_SetWiFiBandPara,   AT_NOT_SET_TIME, AT_QryWiFiBandPara,   AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIBAND",  (VOS_UINT8*)"(0,1)"},

    {AT_CMD_WIFREQ,
    AT_SetWiFiFreqPara,   AT_NOT_SET_TIME, AT_QryWiFiFreqPara,   AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIFREQ",  (VOS_UINT8*)"(0-65535),(@offset)"},

    {AT_CMD_WIRATE,
    AT_SetWiFiRatePara,   AT_NOT_SET_TIME, AT_QryWiFiRatePara,   AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIDATARATE",  (VOS_UINT8*)"(0-65535)"},

    {AT_CMD_WIPOW,
    AT_SetWiFiPowerPara,  AT_NOT_SET_TIME, AT_QryWiFiPowerPara,  AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIPOW",    (VOS_UINT8*)"(0-65535)"},

    {AT_CMD_WITX,
    AT_SetWiFiTxPara,     AT_NOT_SET_TIME, AT_QryWiFiTxPara,     AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WITX",     (VOS_UINT8*)"(0,1)"},

    {AT_CMD_WIRX,
    AT_SetWiFiRxPara,     AT_NOT_SET_TIME, AT_QryWiFiRxPara,     AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIRX",     (VOS_UINT8*)"(0,1),(@smac),(@dmac)"},

    {AT_CMD_WIRPCKG,
    AT_SetWiFiPacketPara, AT_NOT_SET_TIME, AT_QryWiFiPacketPara, AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIRPCKG",  (VOS_UINT8*)"(0)"},

    {AT_CMD_SSID,
    AT_SetWiFiSsidPara,   AT_NOT_SET_TIME, AT_QryWiFiSsidPara,   AT_NOT_SET_TIME, AT_TestSsidPara, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SSID",  (VOS_UINT8*)"(0-3),(@SSID)"},

    {AT_CMD_WIKEY,
    AT_SetWiFiKeyPara,    AT_NOT_SET_TIME, AT_QryWiFiKeyPara,    AT_NOT_SET_TIME, AT_TestWikeyPara, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIKEY",  (VOS_UINT8*)"(0-3),(@WIKEY)"},

    {AT_CMD_WILOG,
    AT_SetWiFiLogPara,    AT_NOT_SET_TIME, AT_QryWiFiLogPara,    AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WILOG",  (VOS_UINT8*)"(0,1),(@content)"},

    {AT_CMD_WIINFO,
    AT_SetWifiInfoPara,   AT_NOT_SET_TIME, VOS_NULL_PTR,         AT_NOT_SET_TIME, At_CmdTestProcERROR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_ERROR, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIINFO",  (VOS_UINT8*)"(0,1),(0,1)"},

    {AT_CMD_WIPARANGE,
    AT_SetWifiPaRangePara, AT_NOT_SET_TIME, AT_QryWifiPaRangePara, AT_NOT_SET_TIME, AT_TestWifiPaRangePara, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^WIPARANGE",  (VOS_UINT8*)"(@gainmode)"},

    {AT_CMD_NVRD,
    AT_SetNVReadPara,     AT_SET_PARA_TIME,  VOS_NULL_PTR,        AT_NOT_SET_TIME,  VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_ERROR, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^NVRD",(VOS_UINT8*)"(0-65535)"},



    {AT_CMD_CURC,
    At_SetCurcPara,      AT_NOT_SET_TIME,     At_QryCurcPara,     AT_QRY_PARA_TIME, VOS_NULL_PTR,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_ERROR, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^CURC", (VOS_UINT8*)"(0-2)"},

    {AT_CMD_TBAT,
    AT_SetTbatPara,     AT_SET_PARA_TIME,   AT_QryTbatPara,  AT_QRY_PARA_TIME,  VOS_NULL_PTR, AT_SET_PARA_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^TBAT",    (VOS_UINT8 *)"(0,1),(0,1),(0-65535),(0-65535)"},

    {AT_CMD_PSTANDBY,
    AT_SetPstandbyPara,     AT_SET_PARA_TIME,   VOS_NULL_PTR,  AT_QRY_PARA_TIME,  VOS_NULL_PTR, AT_SET_PARA_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^PSTANDBY",    (VOS_UINT8 *)"(0-65535),(0-65535)"},

    {AT_CMD_WIWEP,
    AT_SetWiwepPara,        AT_SET_PARA_TIME,   AT_QryWiwepPara,  AT_QRY_PARA_TIME,  AT_TestWiwepPara, AT_SET_PARA_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^WIWEP",    (VOS_UINT8 *)"(0-3),(@wifikey),(0-3)"},

    {AT_CMD_CMDLEN,
    AT_SetCmdlenPara,        AT_SET_PARA_TIME,   AT_QryCmdlenPara,  AT_QRY_PARA_TIME,  At_CmdTestProcOK, AT_SET_PARA_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^CMDLEN",    (VOS_UINT8 *)"(0-65535),(0-65535)"},

    {AT_CMD_HUK,
    AT_SetHukPara,              AT_SET_PARA_TIME,   VOS_NULL_PTR,   AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^HUK",         (VOS_UINT8 *)"(@huk)"},

    {AT_CMD_FACAUTHPUBKEYEX,
    VOS_NULL_PTR,               AT_NOT_SET_TIME,    VOS_NULL_PTR,   AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FACAUTHPUBKEYEX",       (VOS_UINT8 *)"(1-20),(1-20),(@Pubkey)"},

    {AT_CMD_IDENTIFYSTART,
    AT_SetIdentifyStartPara,    AT_SET_PARA_TIME,   VOS_NULL_PTR,   AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^IDENTIFYSTART",       (VOS_UINT8 *)"(@Rsa)"},

    {AT_CMD_IDENTIFYEND,
    AT_SetIdentifyEndPara,      AT_SET_PARA_TIME,   VOS_NULL_PTR,   AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^IDENTIFYEND",       VOS_NULL_PTR},



    {AT_CMD_SIMLOCKDATAWRITEEX,
    VOS_NULL_PTR,               AT_NOT_SET_TIME,    VOS_NULL_PTR,   AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SIMLOCKDATAWRITEEX",    (VOS_UINT8 *)SIMLOCKDATAWRITEEX_CMD_PARA_STRING},
    {AT_CMD_SIMLOCKDATAREADEX,
    AT_SimLockDataReadExPara,  AT_SET_PARA_TIME,    VOS_NULL_PTR,   AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SIMLOCKDATAREADEX",    (VOS_UINT8 *)SIMLOCKDATAREADEX_CMD_PARA_STRING},

    {AT_CMD_SIMLOCKDATAWRITE,
    VOS_NULL_PTR,               AT_NOT_SET_TIME,    VOS_NULL_PTR,   AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SIMLOCKDATAWRITE",    (VOS_UINT8 *)"(@SimlockData)"},

    {AT_CMD_PHONESIMLOCKINFO,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,    AT_QryPhoneSimlockInfoPara, AT_QRY_PARA_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PHONESIMLOCKINFO",    VOS_NULL_PTR},

    {AT_CMD_SIMLOCKDATAREAD,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,    AT_QrySimlockDataReadPara,  AT_QRY_PARA_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SIMLOCKDATAREAD",     VOS_NULL_PTR},

    {AT_CMD_PHONEPHYNUM,
    AT_SetPhonePhynumPara,  AT_SET_PARA_TIME,    AT_QryPhonePhynumPara,  AT_QRY_PARA_TIME,   AT_TestHsicCmdPara,   AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PHONEPHYNUM",         (VOS_UINT8 *)"(@type),(@Phynum),(@Hmac)"},

    {AT_CMD_PORTCTRLTMP,
    AT_SetPortCtrlTmpPara,          AT_SET_PARA_TIME,   AT_QryPortCtrlTmpPara,  AT_NOT_SET_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PORTCTRLTMP",    (VOS_UINT8 *)"(@password)"},

    {AT_CMD_PORTATTRIBSET,
    AT_SetPortAttribSetPara,          AT_SET_PARA_TIME,   AT_QryPortAttribSetPara,  AT_QRY_PARA_TIME,  AT_TestHsicCmdPara,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PORTATTRIBSET",    (VOS_UINT8 *)"(0,1),(@password)"},

    {AT_CMD_SIMLOCKUNLOCK,
    AT_SetSimlockUnlockPara, AT_SET_PARA_TIME,  VOS_NULL_PTR,   AT_NOT_SET_TIME,    AT_TestSimlockUnlockPara, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SIMLOCKUNLOCK",    (VOS_UINT8 *)"(\"NET\",\"NETSUB\",\"SP\"),(pwd)"},

    {AT_CMD_FPLLSTATUS,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,    AT_QryFPllStatusPara,  AT_QRY_PARA_TIME,  VOS_NULL_PTR,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FPLLSTATUS",     (VOS_UINT8 *)"(0,1),(0,1)"},

    {AT_CMD_FPOWDET,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,    AT_QryFpowdetTPara,  AT_QRY_PARA_TIME,  VOS_NULL_PTR,  AT_NOT_SET_TIME,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^FPOWDET",     VOS_NULL_PTR},


    {AT_CMD_MEID,
    AT_SetMeidPara,    AT_SET_PARA_TIME,    AT_QryMeidPara,    AT_SET_PARA_TIME,    VOS_NULL_PTR,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,    AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^MEID",   (VOS_UINT8*)"(@number)"},

    {AT_CMD_DOSYSEVENT,
    AT_SetEvdoSysEvent,    AT_SET_PARA_TIME,    VOS_NULL_PTR,    AT_NOT_SET_TIME,    VOS_NULL_PTR,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,    AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^DOSYSEVENT",   (VOS_UINT8*)"(0-4294967295)"},

    {AT_CMD_DOSIGMASK,
    AT_SetDoSigMask,    AT_SET_PARA_TIME,    VOS_NULL_PTR,    AT_NOT_SET_TIME,    VOS_NULL_PTR,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,    AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8 *)"^DOSIGMASK",   (VOS_UINT8*)"(0-4294967295)"},



    {AT_CMD_SOCID,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,   AT_QrySocidPara,   AT_NOT_SET_TIME,   VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^SOCID", (VOS_UINT8*)"(@SocidString)"},

    {AT_CMD_RFICID,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,   AT_QryRficDieIDPara,   AT_QRY_PARA_TIME,   VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^RFICID", VOS_NULL_PTR},

    {AT_CMD_PMUDIESN,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,   AT_QryPmuDieSNPara,   AT_QRY_PARA_TIME,   VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,   AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^PMUDIESN", VOS_NULL_PTR},


    {AT_CMD_BSN,
    At_SetBsn,           AT_NOT_SET_TIME,    At_QryBsn,             AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^BSN",      VOS_NULL_PTR},

    {AT_CMD_TMODE,
    At_SetTModePara,     AT_SET_PARA_TIME,   At_QryTModePara,       AT_QRY_PARA_TIME ,  At_TestTmodePara , AT_TEST_PARA_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^TMODE",    (VOS_UINT8*)"(0,1,2,3,4,11,12,13,14,15,16,17,18,19)"},

    {AT_CMD_INFORBU,
    atSetNVFactoryBack,  AT_SET_PARA_TIME,    VOS_NULL_PTR,          AT_NOT_SET_TIME,   VOS_NULL_PTR ,    AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS,    CMD_TBL_PIN_IS_LOCKED,
    (VOS_UINT8*)"^INFORBU",  VOS_NULL_PTR},

    {AT_CMD_SN,
    At_SetSnPara,        AT_NOT_SET_TIME,    VOS_NULL_PTR,           AT_NOT_SET_TIME,    At_CmdTestProcOK, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_DEVICE_OTHER_ERROR, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^SN",       VOS_NULL_PTR},

   {AT_CMD_MIPIRD,
    AT_SetMipiRdPara,  AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^MIPIRD",  (TAF_UINT8*)"(0-9),(0-1),(0-15),(0-255)"},

    {AT_CMD_MIPIWREX,
    AT_SetMipiWrParaEx,  AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^MIPIWREX",  (TAF_UINT8*)"(0,1),(0-15),(0-15),(0-255),(1,2,4),(0-4294967295)"},/*extend_flag, mipi_id, slave_id, reg_addr, byte_cnt, value*/

   {AT_CMD_MIPIRDEX,
    AT_SetMipiRdParaEx,  AT_SET_PARA_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME, VOS_NULL_PTR, AT_NOT_SET_TIME,
    VOS_NULL_PTR,        AT_NOT_SET_TIME,
    AT_CME_INCORRECT_PARAMETERS, CMD_TBL_PIN_IS_LOCKED,
    (TAF_UINT8*)"^MIPIRDEX",  (TAF_UINT8*)"(0,1),(0-15),(0-15),(0-255),(1,2,4),(0-65535)"},/*extend_flag, mipi_id, slave_id, reg_addr, byte_cnt, speed_type,*/


};


/* 示例: ^CMDX 命令是不受E5密码保护命令，且在+CLAC列举所有命令时不显示，第一个参数是不带双引号的字符串,
        第二个参数是带双引号的字符串，第三个参数是整数型参数

   !!!!!!!!!!!注意: param1和param2是示例，实际定义命令时应尽量定义的简短(可提高解析效率)!!!!!!!!!!!!!

    {AT_CMD_CMDX,
    At_SetCmdxPara, AT_SET_PARA_TIME, At_QryCmdxPara, AT_QRY_PARA_TIME, At_TestCmdxPara, AT_NOT_SET_TIME,
    AT_ERROR, CMD_TBL_E5_IS_LOCKED | CMD_TBL_CLAC_IS_INVISIBLE,
    (VOS_UINT8*)"^CMDX", (VOS_UINT8*)"(@param1),(param2),(0-255)"},
*/


/*****************************************************************************
  3 函数实现
*****************************************************************************/


VOS_UINT32 At_QrySecuBootFeaturePara( VOS_UINT8 ucIndex )
{
    VOS_UINT8                          usSecBootSupportedFlag;
    usSecBootSupportedFlag = 0;

    /* 调用底软接口 */
    if(MDRV_OK != mdrv_crypto_secboot_supported(&usSecBootSupportedFlag))
    {
        AT_WARN_LOG("At_QrySecuBootFeaturePara: get mdrv_crypto_secboot_supported() failed");
        return AT_ERROR;
    }


    /* 打印输出 */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                            (VOS_CHAR *)pgucAtSndCodeAddr,
                                            (VOS_CHAR *)pgucAtSndCodeAddr,
                                            "%s: %d",
                                            g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                            usSecBootSupportedFlag);

    return AT_OK;
}


VOS_INT32 AT_SetSecDbgState(VOS_VOID)
{
    VOS_INT32               usResult;

    /*1.执行SECUREDEBUG=3,设置安全DEBUG授权由安全证书控制*/
    usResult = mdrv_efuse_ioctl(MDRV_EFUSE_IOCTL_CMD_SET_SECUREDEBUG,
                                AT_SECUREDEBUG_VALUE,
                                NULL,
                                0);

    /*设置接口只有三种返回值，<0,执行错误；=0，执行正确；=1，重复设置；
      所以这里只判断返回错误情况*/
    if(MDRV_OK > usResult)
    {
        AT_WARN_LOG("AT_SetSecuState: set SECUREDEBUG state error.\n");
        return AT_ERROR;
    }

    /*2.执行SECDBGRESET=1和CSRESET=1,在对安全世界和Coresight调试时，
        临时复位SecEngine*/
    usResult = mdrv_efuse_ioctl(MDRV_EFUSE_IOCTL_CMD_SET_CSRESET,
                                AT_CSRESET_VALUE,
                                NULL,
                                0);
    if(MDRV_OK > usResult)
    {
        AT_WARN_LOG("AT_SetSecuState: set CSRESET state error.\n");
        return AT_ERROR;
    }

    /*3.执行DFTSEL=1 可以通过密码验证方式开启安全DFT功能*/
    usResult = mdrv_efuse_ioctl(MDRV_EFUSE_IOCTL_CMD_SET_DFTSEL,
                                AT_DFTSEL_VALUE,
                                NULL,
                                0);
    if(MDRV_OK > usResult)
    {
        AT_WARN_LOG("AT_SetSecuState: set CSRESET state error.\n");
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_SetSecuBootPara(VOS_UINT8 ucIndex)
{
    if ((1 != gucAtParaIndex)
        || (0 == gastAtParaList[0].usParaLen))
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (AT_DX_SECURE_STATE < gastAtParaList[0].ulParaValue)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    switch(gastAtParaList[0].ulParaValue)
    {
        case AT_NO_DX_SECU_ENABLE_STATE:
        {
            if (MDRV_OK != mdrv_crypto_start_secure())
            {
                AT_WARN_LOG("At_SetSecuBootPara: mdrv_crypto_start_secure() failed");
                return AT_ERROR;
            }
            break;
        }
        case AT_DX_RMA_STATE:
        {
            /*先获取下芯片状态，如果已经设置为RMA状态，直接返回ok*/
            if(AT_DRV_STATE_RMA == mdrv_efuse_ioctl(MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE,
                                                    0, NULL, 0))
            {
                AT_WARN_LOG("At_SetSecuBootPara: chip is already set to RMA state");
                return AT_OK;
            }

            if (MDRV_OK != mdrv_efuse_ioctl(MDRV_EFUSE_IOCTL_CMD_SET_SECURESTATE,
                                            AT_SET_RMA_STATE, NULL, 0))
            {
                AT_WARN_LOG("At_SetSecuBootPara: SET_SECURE_DISABLED_STATE  failed");
                return AT_ERROR;
            }
            break;
        }
        case AT_DX_SECURE_STATE:
        {
            /*安全状态已经在Fastboot里设置，这里check下，以保证产线流程OK*/
            if(AT_DRV_STATE_SECURE != mdrv_efuse_ioctl(MDRV_EFUSE_IOCTL_CMD_GET_SECURESTATE,
                                                       0, NULL, 0))
            {
                AT_WARN_LOG("At_SetSecuBootPara: chip don't set to SECURE state");
                return AT_ERROR;
            }
            if (AT_OK != AT_SetSecDbgState())
            {
                return AT_ERROR;
            }
            break;
        }
        default:
        {
            return AT_CME_INCORRECT_PARAMETERS;
        }
    }

    return AT_OK;
}


VOS_UINT32 At_QrySecuBootPara(VOS_UINT8 ucIndex)
{
    VOS_UINT8                           usSecBootStartedFlag = 0;

    if(MDRV_OK != mdrv_crypto_sec_started(&usSecBootStartedFlag))
    {
        AT_WARN_LOG("At_QrySecuBootPara: get mdrv_crypto_sec_started() failed");
        return AT_ERROR;
    }

/*对于支持DX安全引擎的，需要查询芯片状态*/

    /* 打印输出 */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                            (VOS_CHAR *)pgucAtSndCodeAddr,
                                            (VOS_CHAR *)pgucAtSndCodeAddr,
                                            "%s: %d",
                                            g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                            usSecBootStartedFlag);

     return AT_OK;

}



VOS_UINT32 At_TestTmodePara(VOS_UINT8 ucIndex)
{
    VOS_UINT16                          usLength;

    usLength = 0;

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                  (VOS_CHAR *)pgucAtSndCodeAddr,
                                  (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                  "%s: %d",
                                  g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                  g_stAtDevCmdCtrl.ucCurrentTMode);

    gstAtSendData.usBufLen = usLength;

    return AT_OK;

}


VOS_UINT32 At_TestFdacPara(VOS_UINT8 ucIndex)
{
    VOS_UINT16                          usLength;

    usLength  = 0;

    if ((AT_RAT_MODE_WCDMA == g_stAtDevCmdCtrl.ucDeviceRatMode)
     || (AT_RAT_MODE_AWS == g_stAtDevCmdCtrl.ucDeviceRatMode))
    {
        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                          "%s: (0-2047)",
                                          g_stParseContext[ucIndex].pstCmdElement->pszCmdName);
    }
    else
    {
        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                          "%s: (0-1023)",
                                          g_stParseContext[ucIndex].pstCmdElement->pszCmdName);
    }
    gstAtSendData.usBufLen = usLength;

    return AT_OK;

}


VOS_UINT32 At_RegisterDeviceCmdTable(VOS_VOID)
{
    return AT_RegisterCmdTable(g_astAtDeviceCmdTbl, sizeof(g_astAtDeviceCmdTbl)/sizeof(g_astAtDeviceCmdTbl[0]));
}



VOS_UINT32 AT_TestSsidPara(VOS_UINT8 ucIndex)
{
    if (BSP_MODULE_UNSUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI))
    {
        return AT_ERROR;
    }

    gstAtSendData.usBufLen = (TAF_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                    (TAF_CHAR *)pgucAtSndCodeAddr,
                                                    (TAF_CHAR *)pgucAtSndCodeAddr,
                                                    "%s:%d",
                                                    g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                    AT_WIFI_MAX_SSID_NUM);
    return AT_OK;
}


VOS_UINT32 AT_TestWikeyPara(VOS_UINT8 ucIndex)
{
    if (BSP_MODULE_UNSUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI) )
    {
        return AT_ERROR;
    }

    gstAtSendData.usBufLen = (TAF_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                    (TAF_CHAR *)pgucAtSndCodeAddr,
                                                    (TAF_CHAR *)pgucAtSndCodeAddr,
                                                    "%s:%d",
                                                    g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                    AT_WIFI_KEY_NUM);
    return AT_OK;
}



VOS_UINT32 AT_SetTbatPara(VOS_UINT8 ucIndex)
{
    return atSetTBATPara(ucIndex);

    /*
    根据参数不同进行下述操作:
    1.  支持用户输入AT^TBAT=1,0获取电池电压数字值；
    调用底软/OM接口获取电池数字值
    2.  支持用户输入AT^TBAT=1,1,<value1>,<value2>设置电池电压数字值，用于电池校准；
    写校准电压到NVID 90(en_NV_Item_BATTERY_ADC)，此处有疑问待确认，
    en_NV_Item_BATTERY_ADC中记录的是下述两个值，而AT命令仅一个参数，如何映射
    3.4V 电压对应的ADC值
    4.2V 电压对应的ADC值
    */
}


VOS_UINT32 AT_QryTbatPara(VOS_UINT8 ucIndex)
{
    /*
    调用底软接口获取电池安装方式:
    <mount type> 电池安装方式
    0 无电池
    1 可更换电池
    2 内置一体化电池
    */
    /*命令状态类型检查*/
    if (AT_CMD_OPT_READ_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_DEVICE_OTHER_ERROR;
    }

    if (BSP_MODULE_UNSUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_CHARGE) )
    {
        return AT_ERROR;
    }

    if (TAF_SUCCESS == AT_FillAndSndAppReqMsg(gastAtClientTab[ucIndex].usClientId,
                                               gastAtClientTab[ucIndex].opId,
                                               DRV_AGENT_TBAT_QRY_REQ,
                                               VOS_NULL_PTR,
                                               0,
                                               I0_WUEPS_PID_DRV_AGENT))
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_TBAT_QRY;             /*设置当前操作模式 */
        return AT_WAIT_ASYNC_RETURN;                                                              /* 等待异步事件返回 */
    }
    else
    {
        return AT_ERROR;
    }
}


VOS_UINT32 AT_SetPstandbyPara(VOS_UINT8 ucIndex)
{
    DRV_AGENT_PSTANDBY_REQ_STRU         stPstandbyInfo;


    TAF_MMA_PHONE_MODE_PARA_STRU        stPhoneModePara;


    /* ^PSTANDBY设置命令有且仅有2个参数: 进入待机状态的时间长度和单板进入待机状态的切换时间 */
    if(AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((0 == gastAtParaList[0].usParaLen)
     || (0 == gastAtParaList[1].usParaLen)
     || (2 != gucAtParaIndex))
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    At_FormatResultData(ucIndex, AT_OK); /*需要先回复OK*/

    /*保证返回数据发送完成*/
    VOS_TaskDelay(10);

    /*
    调用底软和OM接口使单板进入待机状态:
    ACPU上完成的操作：
    1、下电WIFI
    2、LED下电
    3、USB PowerOff

    发消息到C核，指示CCPU上完成的操作：
    1、通信模块下电
    2、关定时器
    3、关中断
    4、调用底软接口进入深睡
    */
    stPstandbyInfo.ulStandbyTime = gastAtParaList[0].ulParaValue;
    stPstandbyInfo.ulSwitchTime   = gastAtParaList[1].ulParaValue;

    DRV_PWRCTRL_STANDBYSTATEACPU(stPstandbyInfo.ulStandbyTime, stPstandbyInfo.ulSwitchTime);

    /*发送消息到c核*/
    if (TAF_SUCCESS != AT_FillAndSndAppReqMsg(gastAtClientTab[ucIndex].usClientId,
                           gastAtClientTab[ucIndex].opId,
                           DRV_AGENT_PSTANDBY_SET_REQ,
                           &stPstandbyInfo,
                           sizeof(stPstandbyInfo),
                           I0_WUEPS_PID_DRV_AGENT))
    {
        AT_ERR_LOG("AT_SetPstandbyPara: AT_FillAndSndAppReqMsg fail.");
    }

    /* V7R2采用关机进入低功耗流程流程 */

    stPhoneModePara.PhMode = TAF_PH_MODE_MINI;

    if (VOS_TRUE == TAF_MMA_PhoneModeSetReq(WUEPS_PID_AT, gastAtClientTab[ucIndex].usClientId, 0, &stPhoneModePara))
    {
        /* 设置当前操作类型 */
        gastAtClientTab[ucIndex].CmdCurrentOpt = (AT_CMD_CURRENT_OPT_ENUM)AT_CMD_PSTANDBY_SET;

        return AT_WAIT_ASYNC_RETURN;    /* 返回命令处理挂起状态 */
    }

    return AT_SUCCESS;
}


VOS_UINT32 AT_WriteWiWep(
    VOS_UINT32                          ulIndex,
    VOS_UINT8                           aucWiWep[],
    VOS_UINT16                          usWiWepLen,
    TAF_AT_MULTI_WIFI_SEC_STRU         *pstWifiSecInfo,
    VOS_UINT8                           ucGroup
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulLoop;
    VOS_UINT8                          *paucWifiWepKey;

    /* 因为仅修改en_NV_Item_WIFI_KEY中的WIFI KEY子项，所以需要先获取全部NV值，然后更新修改部分 */
    ulRet = TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_MULTI_WIFI_KEY, pstWifiSecInfo, sizeof(TAF_AT_MULTI_WIFI_SEC_STRU));
    if (NV_OK != ulRet)
    {
        AT_WARN_LOG("AT_WriteWiWep: Fail to read en_NV_Item_WIFI_KEY.");
        return AT_ERROR;
    }

    /* 根据index获取NV中保存的密码 */
    paucWifiWepKey = ((0 == ulIndex) ? pstWifiSecInfo->aucWifiWepKey1[ucGroup] :
                      ((1 == ulIndex) ? pstWifiSecInfo->aucWifiWepKey2[ucGroup] :
                      ((2 == ulIndex) ? pstWifiSecInfo->aucWifiWepKey3[ucGroup] : pstWifiSecInfo->aucWifiWepKey4[ucGroup])));

    /* 判断新的WIFI KEY与NV中记录的是否一致 */
    for (ulLoop = 0; ulLoop < AT_NV_WLKEY_LEN; ulLoop++)
    {
        if (paucWifiWepKey[ulLoop] != aucWiWep[ulLoop])
        {
            break;
        }
    }

    /* 判断新的WIFI KEY与NV中记录的一致则直接返回操作完成*/
    if (AT_NV_WLKEY_LEN == ulLoop)
    {
        return AT_OK;
    }

    /* 更新数据到NV项en_NV_Item_WIFI_KEY */
    TAF_MEM_SET_S(paucWifiWepKey, AT_WIFI_KEY_LEN_MAX, 0x00, AT_NV_WLKEY_LEN);

    TAF_MEM_CPY_S(paucWifiWepKey, AT_WIFI_KEY_LEN_MAX, aucWiWep, usWiWepLen);

    ulRet = TAF_ACORE_NV_WRITE(MODEM_ID_0, en_NV_Item_MULTI_WIFI_KEY, pstWifiSecInfo, sizeof(TAF_AT_MULTI_WIFI_SEC_STRU));
    if (NV_OK != ulRet)
    {
         AT_WARN_LOG("AT_WriteWiWep: Fail to write NV en_NV_Item_WIFI_KEY.");
         return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetWiwepPara(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulIndex;
    VOS_UINT8                           ucGroup;
    VOS_UINT32                          ulRet;
    TAF_AT_MULTI_WIFI_SEC_STRU         *pstWifiSecInfo = VOS_NULL_PTR;

    if (BSP_MODULE_UNSUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI))
    {
        return AT_ERROR;
    }

    /* 输入参数检查: 有且仅有< index >和< content >两个参数 */
    if(AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_ERROR;
    }

    if ((0 == gastAtParaList[0].usParaLen)
     || (0 == gastAtParaList[1].usParaLen)
     || (0 == gastAtParaList[2].usParaLen))
    {
        return AT_ERROR;
    }

    /* WIFI key 操作受DATALOCK保护 */
    if (VOS_TRUE == g_bAtDataLocked)
    {
        return AT_ERROR;
    }

    /* < index >必须在0-3范围内，< content >长度小于NV_WLKEY_LEN */
    ulIndex = gastAtParaList[0].ulParaValue;
    if (ulIndex > AT_WIWEP_CARD_WIFI_KEY_TOTAL)
    {
        return AT_ERROR;
    }

    if (gastAtParaList[1].usParaLen > AT_NV_WLKEY_LEN)
    {
        return AT_ERROR;
    }

    ucGroup = (VOS_UINT8)gastAtParaList[2].ulParaValue;

    if (ucGroup >= AT_WIFI_MAX_SSID_NUM)
    {
        return AT_ERROR;
    }

    /* 组装WIFI KEY到NV结构并更新数据到NV项en_NV_Item_WIFI_KEY */
    /*lint -save -e830 */
    pstWifiSecInfo = (TAF_AT_MULTI_WIFI_SEC_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT,
                                                  sizeof(TAF_AT_MULTI_WIFI_SEC_STRU));
    /*lint -restore */
    if (VOS_NULL_PTR == pstWifiSecInfo)
    {
        return AT_ERROR;
    }

    ulRet = AT_WriteWiWep(ulIndex,
                          gastAtParaList[1].aucPara,
                          gastAtParaList[1].usParaLen,
                          pstWifiSecInfo,
                          ucGroup);
    /*lint -save -e830 */
    PS_MEM_FREE(WUEPS_PID_AT, pstWifiSecInfo);
    /*lint -restore */
    return ulRet;
}


VOS_UINT32 AT_QryWiwepPara(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulLoop;
    VOS_UINT16                          usLength;
    TAF_AT_MULTI_WIFI_SEC_STRU         *pstWifiSecInfo;
    VOS_UINT8                           aucWifiWepKey[AT_NV_WLKEY_LEN + 1];
    VOS_UINT8                           aucWepKeyLen1[AT_WIFI_MAX_SSID_NUM];
    VOS_UINT8                           aucWepKeyLen2[AT_WIFI_MAX_SSID_NUM];
    VOS_UINT8                           aucWepKeyLen3[AT_WIFI_MAX_SSID_NUM];
    VOS_UINT8                           aucWepKeyLen4[AT_WIFI_MAX_SSID_NUM];
    VOS_UINT8                           ucWepKeyNum;

    if (BSP_MODULE_UNSUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI))
    {
        return AT_ERROR;
    }

    /* 为读取WIFI KEY申请内存，读NV项en_NV_Item_WIFI_KEY获取WIFI KEY信息 */
    pstWifiSecInfo = (TAF_AT_MULTI_WIFI_SEC_STRU *)PS_MEM_ALLOC(WUEPS_PID_AT,
                                                      sizeof(TAF_AT_MULTI_WIFI_SEC_STRU));
    if (VOS_NULL_PTR == pstWifiSecInfo)
    {
        return AT_ERROR;
    }

    ulRet = TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_MULTI_WIFI_KEY, pstWifiSecInfo, sizeof(TAF_AT_MULTI_WIFI_SEC_STRU));
    if (NV_OK != ulRet)
    {
        AT_WARN_LOG("AT_QryWiwepPara: Fail to read en_NV_Item_WIFI_KEY.");
        PS_MEM_FREE(WUEPS_PID_AT, pstWifiSecInfo);
        return AT_ERROR;
    }

    ucWepKeyNum = 0;
    TAF_MEM_SET_S(aucWepKeyLen1, sizeof(aucWepKeyLen1), 0x00, sizeof(aucWepKeyLen1));
    TAF_MEM_SET_S(aucWepKeyLen2, sizeof(aucWepKeyLen2), 0x00, sizeof(aucWepKeyLen2));
    TAF_MEM_SET_S(aucWepKeyLen3, sizeof(aucWepKeyLen3), 0x00, sizeof(aucWepKeyLen3));
    TAF_MEM_SET_S(aucWepKeyLen4, sizeof(aucWepKeyLen4), 0x00, sizeof(aucWepKeyLen4));

    for (ulLoop = 0; ulLoop < AT_WIFI_MAX_SSID_NUM; ulLoop++)
    {
        /* KEY1对应的NV不空表示KEY1有效 */
        aucWepKeyLen1[ulLoop] = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)pstWifiSecInfo->aucWifiWepKey1[ulLoop]);
        if (0 != aucWepKeyLen1[ulLoop])
        {
            ucWepKeyNum++;
        }

        aucWepKeyLen2[ulLoop] = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)pstWifiSecInfo->aucWifiWepKey2[ulLoop]);
        if (0 != aucWepKeyLen2[ulLoop])
        {
            ucWepKeyNum++;
        }

        aucWepKeyLen3[ulLoop] = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)pstWifiSecInfo->aucWifiWepKey3[ulLoop]);
        if (0 != aucWepKeyLen3[ulLoop])
        {
            ucWepKeyNum++;
        }

        aucWepKeyLen4[ulLoop] = (VOS_UINT8)VOS_StrLen((VOS_CHAR*)pstWifiSecInfo->aucWifiWepKey4[ulLoop]);
        if (0 != aucWepKeyLen4[ulLoop])
        {
            ucWepKeyNum++;
        }
    }

    /* 未解锁时，需要返回已定制0组 */
    if (VOS_TRUE == g_bAtDataLocked)
    {
        ucWepKeyNum = 0;
        TAF_MEM_SET_S(aucWepKeyLen1, sizeof(aucWepKeyLen1), 0x00, sizeof(aucWepKeyLen1));
        TAF_MEM_SET_S(aucWepKeyLen2, sizeof(aucWepKeyLen2), 0x00, sizeof(aucWepKeyLen2));
        TAF_MEM_SET_S(aucWepKeyLen3, sizeof(aucWepKeyLen3), 0x00, sizeof(aucWepKeyLen3));
        TAF_MEM_SET_S(aucWepKeyLen4, sizeof(aucWepKeyLen4), 0x00, sizeof(aucWepKeyLen4));
    }

    /*
    满足工位对接要求，需要输出工位支持的全部数20个WiFi WEP（WIFI的KEY）
    打印四组单板支持的WIFI KEY信息
    填充16行^WIWEP: <index>,与工位要求的20个的WiFi WEP（WIFI的KEY）一致，
    */
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                     (VOS_CHAR *)pgucAtSndCodeAddr,
                                     (VOS_CHAR *)pgucAtSndCodeAddr,
                                     "%s:%d%s",
                                     g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                     ucWepKeyNum,
                                     gaucAtCrLf);

    for (ulLoop = 0; ulLoop < AT_WIFI_MAX_SSID_NUM; ulLoop++)
    {
        if (0 != aucWepKeyLen1[ulLoop])
        {
            /* wifikey1 */
            TAF_MEM_SET_S(aucWifiWepKey, sizeof(aucWifiWepKey), 0x00, (VOS_SIZE_T)(AT_NV_WLKEY_LEN + 1));

            TAF_MEM_CPY_S(aucWifiWepKey,
                       sizeof(aucWifiWepKey),
                       pstWifiSecInfo->aucWifiWepKey1[ulLoop],
                       AT_NV_WLKEY_LEN);

            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s:%d,%s,%d%s",
                                              g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                              0,
                                              aucWifiWepKey,
                                              ulLoop,
                                              gaucAtCrLf);
        }

        if (0 != aucWepKeyLen2[ulLoop])
        {
            /* wifikey1 */
            TAF_MEM_SET_S(aucWifiWepKey, sizeof(aucWifiWepKey), 0x00, (VOS_SIZE_T)(AT_NV_WLKEY_LEN + 1));

            TAF_MEM_CPY_S(aucWifiWepKey,
                       sizeof(aucWifiWepKey),
                       pstWifiSecInfo->aucWifiWepKey2[ulLoop],
                       AT_NV_WLKEY_LEN);

            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s:%d,%s,%d%s",
                                              g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                              1,
                                              aucWifiWepKey,
                                              ulLoop,
                                              gaucAtCrLf);
        }

        if (0 != aucWepKeyLen3[ulLoop])
        {
            /* wifikey1 */
            TAF_MEM_SET_S(aucWifiWepKey, sizeof(aucWifiWepKey), 0x00, (VOS_SIZE_T)(AT_NV_WLKEY_LEN + 1));

            TAF_MEM_CPY_S(aucWifiWepKey,
                       sizeof(aucWifiWepKey),
                       pstWifiSecInfo->aucWifiWepKey3[ulLoop],
                       AT_NV_WLKEY_LEN);

            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s:%d,%s,%d%s",
                                              g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                              2,
                                              aucWifiWepKey,
                                              ulLoop,
                                              gaucAtCrLf);
        }

        if (0 != aucWepKeyLen4[ulLoop])
        {
            /* wifikey1 */
            TAF_MEM_SET_S(aucWifiWepKey, sizeof(aucWifiWepKey), 0x00, (VOS_SIZE_T)(AT_NV_WLKEY_LEN + 1));

            TAF_MEM_CPY_S(aucWifiWepKey,
                       sizeof(aucWifiWepKey),
                       pstWifiSecInfo->aucWifiWepKey4[ulLoop],
                       AT_NV_WLKEY_LEN);

            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                              (VOS_CHAR *)pgucAtSndCodeAddr,
                                              (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                              "%s:%d,%s,%d%s",
                                              g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                              3,
                                              aucWifiWepKey,
                                              ulLoop,
                                              gaucAtCrLf);
        }
    }

    gstAtSendData.usBufLen = usLength - (VOS_UINT16)VOS_StrLen((VOS_CHAR *)gaucAtCrLf);

    PS_MEM_FREE(WUEPS_PID_AT, pstWifiSecInfo);

    return AT_OK;
}


VOS_UINT32 AT_TestWiwepPara(VOS_UINT8 ucIndex)
{
    if (BSP_MODULE_UNSUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI) )
    {
        return AT_ERROR;
    }

    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   "%s:%d",
                                                   g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                   AT_WIWEP_TOOLS_WIFI_KEY_TOTAL);

    return AT_OK;
}


VOS_UINT32 AT_TestWifiPaRangePara (VOS_UINT8 ucIndex)
{
    AT_WIFI_MODE_ENUM_UINT8             ucWifiMode;

    if (BSP_MODULE_UNSUPPORT == mdrv_misc_support_check(BSP_MODULE_TYPE_WIFI) )
    {
        return AT_ERROR;
    }

    /* 初始化 */
    ucWifiMode                          = (VOS_UINT8)WIFI_GET_PA_MODE();

    /* 查询单板支持的模式：如果只支持PA模式，没有NO PA模式，则只返回h即可，如果两种模式都支持，则返回h,l。*/
    if (AT_WIFI_MODE_ONLY_PA == ucWifiMode)
    {
        gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                                "%s:%s",
                                                g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                "h");
    }
    else if (AT_WIFI_MODE_ONLY_NOPA == ucWifiMode)
    {
        gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                                "%s:%s",
                                                g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                "l");
    }
    else if (AT_WIFI_MODE_PA_NOPA == ucWifiMode)
    {
        gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                                (VOS_CHAR *)pgucAtSndCodeAddr,
                                                "%s:%s",
                                                g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                "h,l");
    }
    else
    {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_VOID AT_GetTseLrfLoadDspInfo(
    AT_TSELRF_PATH_ENUM_UINT32          enPath,
    VOS_BOOL                           *pbLoadDsp,
    DRV_AGENT_TSELRF_SET_REQ_STRU      *pstTseLrf
)
{
    /* ^TSELRF命令设置的射频通路编号为GSM且当前已经LOAD了该通路，无需LOAD */
    if (AT_TSELRF_PATH_GSM == enPath)
    {
        if ((AT_RAT_MODE_GSM == g_stAtDevCmdCtrl.ucDeviceRatMode)
         && (VOS_TRUE == g_stAtDevCmdCtrl.bDspLoadFlag))
        {
            *pbLoadDsp = VOS_FALSE;
        }
        else
        {
            pstTseLrf->ucLoadDspMode     = VOS_RATMODE_GSM;
            pstTseLrf->ucDeviceRatMode   = AT_RAT_MODE_GSM;
            *pbLoadDsp                   = VOS_TRUE;
        }
        return;
    }

    /* ^TSELRF命令设置的射频通路编号为WCDMA主集且当前已经LOAD了该通路，无需LOAD */
    if (AT_TSELRF_PATH_WCDMA_PRI == enPath)
    {
        if (((AT_RAT_MODE_WCDMA == g_stAtDevCmdCtrl.ucDeviceRatMode)
          || (AT_RAT_MODE_AWS == g_stAtDevCmdCtrl.ucDeviceRatMode))
         && (VOS_TRUE == g_stAtDevCmdCtrl.bDspLoadFlag))
        {
            *pbLoadDsp = VOS_FALSE;
        }
        else
        {
            pstTseLrf->ucLoadDspMode     = VOS_RATMODE_WCDMA;
            pstTseLrf->ucDeviceRatMode   = AT_RAT_MODE_WCDMA;
            *pbLoadDsp                   = VOS_TRUE;
        }
        return;
    }

    *pbLoadDsp = VOS_FALSE;

    AT_WARN_LOG("AT_GetTseLrfLoadDspInfo: enPath only support GSM or WCDMA primary.");

    return;
}



VOS_UINT32 AT_SetCmdlenPara(VOS_UINT8 ucIndex)
{
    if(AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if ((0 == gastAtParaList[0].usParaLen)
     || (0 == gastAtParaList[1].usParaLen)
     || (2 != gucAtParaIndex))
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    if (gastAtParaList[0].ulParaValue > (AT_COM_BUFF_LEN - sizeof("AT")))
    {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_QryCmdlenPara(VOS_UINT8 ucIndex)
{
    /*
      输出单板最大可以直接接收AT命令字符个数(AT_CMD_MAX_LEN - sizeof("AT"))，
      该字符个数均不包含AT这两个字符，以及单板通过AT一次最大可以响应的字符个数，
      BALONG产品响应字符串长度可以分
      多次上报，没有最大值概念，工位对该值无处理，此处输出一次上报的最大值AT_CMD_MAX_LEN。
    */
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    "%s:%d,%d",
                                                    g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                    (AT_COM_BUFF_LEN - VOS_StrLen("AT")),
                                                    AT_CMD_MAX_LEN);

    return AT_OK;
}


VOS_UINT32 AT_UpdateMacPara(
    VOS_UINT8                           aucMac[],
    VOS_UINT16                          usMacLength
)
{
    VOS_UINT32                          ulRet;
    VOS_UINT32                          ulLoop;
    VOS_UINT32                          ulWifiGlobalMacOffset;
    VOS_UINT32                          ulPhyNumMacOffset;
    VOS_UINT8                           aucWifiGlobalMac[AT_MAC_ADDR_LEN];

    /* MAC地址长度检查: 必须12位 */
    if (AT_PHYNUM_MAC_LEN != usMacLength)
    {
        return AT_PHYNUM_LENGTH_ERR;
    }

    /* MAC地址格式匹配: 7AFEE22111E4=>7A:FE:E2:21:11:E4*/
    ulWifiGlobalMacOffset = 0;
    ulPhyNumMacOffset     = 0;
    for (ulLoop = 0; ulLoop < (AT_PHYNUM_MAC_COLON_NUM + 1); ulLoop++)
    {
        TAF_MEM_CPY_S(&aucWifiGlobalMac[ulWifiGlobalMacOffset],
                   AT_MAC_ADDR_LEN - ulWifiGlobalMacOffset,
                   &aucMac[ulPhyNumMacOffset],
                   AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS);
        ulWifiGlobalMacOffset += AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS;
        ulPhyNumMacOffset     += AT_WIFIGLOBAL_MAC_LEN_BETWEEN_COLONS;
        aucWifiGlobalMac[ulWifiGlobalMacOffset] = ':';
        ulWifiGlobalMacOffset++;
    }

    aucWifiGlobalMac[AT_PHYNUM_MAC_LEN + AT_PHYNUM_MAC_COLON_NUM] = '\0';

    /* 更新MAC地址到NV */
    ulRet = TAF_ACORE_NV_WRITE(MODEM_ID_0, en_NV_Item_WIFI_MAC_ADDR, aucWifiGlobalMac, AT_MAC_ADDR_LEN);
    if (NV_OK != ulRet)
    {
         AT_WARN_LOG("AT_UpdateMacPara: Fail to write NV.");
         return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 AT_SetTmodeAutoPowerOff(VOS_UINT8 ucIndex)
{

    TAF_MMA_PHONE_MODE_PARA_STRU        stPhModeSet;

    VOS_UINT8                          *pucSystemAppConfig = VOS_NULL_PTR;


    TAF_MEM_SET_S(&stPhModeSet, sizeof(stPhModeSet), 0x00, sizeof(TAF_MMA_PHONE_MODE_PARA_STRU));

    pucSystemAppConfig                    = AT_GetSystemAppConfigAddr();

    if ( SYSTEM_APP_WEBUI == *pucSystemAppConfig)
    {
        /* 发消息给C核通知关机并下电 */
        /* 先通知用户AT命令已执行，用户通过检测端口是否消失来确认任务是否正确执行
           此命令执行完成后已下电，无需等待异步返回
        */

        stPhModeSet.PhMode  = TAF_PH_MODE_POWEROFF;

        if (VOS_TRUE == TAF_MMA_PhoneModeSetReq(WUEPS_PID_AT, gastAtClientTab[ucIndex].usClientId, 0, &stPhModeSet))
        {
            return AT_OK;
        }
        else
        {
            return AT_ERROR;
        }

    }

    return AT_ERROR;

}



VOS_UINT32 AT_SDParamErrCode(VOS_VOID)
{
    if(0 == g_stATParseCmd.ucParaCheckIndex)
    {
        return AT_SD_CARD_OPRT_NOT_ALLOWED;
    }
    else if(1 == g_stATParseCmd.ucParaCheckIndex)
    {
        return AT_SD_CARD_ADDR_ERR;
    }
    else
    {
        return AT_SD_CARD_OTHER_ERR;
    }
}



VOS_VOID AT_GetSpecificPort(
    VOS_UINT8                           ucPortType,
    VOS_UINT8                           aucRewindPortStyle[],
    VOS_UINT32                         *pulPortPos,
    VOS_UINT32                         *pulPortNum
)
{
    VOS_UINT32                          ulLoop;

    *pulPortPos = AT_DEV_NONE;
    for (ulLoop = 0; ulLoop < AT_SETPORT_PARA_MAX_LEN; ulLoop++)
    {
        if (0 == aucRewindPortStyle[ulLoop])
        {
            break;
        }

        /* 端口值为ucPortType，表示找到指定端口，记录ulLoop为端口在NV项中的偏移 */
        if (ucPortType == aucRewindPortStyle[ulLoop])
        {
            *pulPortPos = ulLoop;
        }
    }

    /* 端口值为0或到达循环上限，ulLoop即为单板的端口总数 */
    *pulPortNum = ulLoop;

    return;
}


VOS_UINT32 AT_ExistSpecificPort(VOS_UINT8 ucPortType)
{
    AT_DYNAMIC_PID_TYPE_STRU            stDynamicPidType;
    VOS_UINT32                          ulPortPos;
    VOS_UINT32                          ulPortNum;


    TAF_MEM_SET_S(&stDynamicPidType, sizeof(stDynamicPidType), 0x00, sizeof(stDynamicPidType));

    ulPortPos = 0;
    ulPortNum = 0;


    /* 读NV项en_NV_Item_Huawei_Dynamic_PID_Type获取当前的端口状态 */
    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0,
                                   en_NV_Item_Huawei_Dynamic_PID_Type,
                                   &stDynamicPidType,
                                   sizeof(AT_DYNAMIC_PID_TYPE_STRU)))
    {
        AT_WARN_LOG("AT_ExistSpecificPort: Read NV fail!");
        return VOS_FALSE;
    }

    /* 判断DIAG端口是否已经打开: 已经打开则直接返回AT_OK */
    if (VOS_TRUE == stDynamicPidType.ulNvStatus)
    {
        /* 查询NV项en_NV_Item_Huawei_Dynamic_PID_Type中是否已经存在DIAG口 */
        AT_GetSpecificPort(ucPortType,
                           stDynamicPidType.aucRewindPortStyle,
                           &ulPortPos,
                           &ulPortNum);

        if (AT_DEV_NONE != ulPortPos)
        {
            return VOS_TRUE;
        }

    }

    return VOS_FALSE;
}



VOS_UINT32 AT_OpenDiagPort(VOS_VOID)
{
    AT_DYNAMIC_PID_TYPE_STRU            stDynamicPidType;
    VOS_UINT32                          ulPortPos;
    VOS_UINT32                          ulPortNum;


    TAF_MEM_SET_S(&stDynamicPidType, sizeof(stDynamicPidType), 0x00, sizeof(stDynamicPidType));

    ulPortPos = 0;
    ulPortNum = 0;


    /* 读NV项en_NV_Item_Huawei_Dynamic_PID_Type获取当前的端口状态 */
    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0,
                                   en_NV_Item_Huawei_Dynamic_PID_Type,
                                   &stDynamicPidType,
                                   sizeof(AT_DYNAMIC_PID_TYPE_STRU)))
    {
        AT_WARN_LOG("AT_OpenDiagPort: Read NV fail!");
        return AT_ERROR;
    }

    /* 判断DIAG端口是否已经打开: 已经打开则直接返回AT_OK */
    if (VOS_TRUE == stDynamicPidType.ulNvStatus)
    {
        /* 查询NV项en_NV_Item_Huawei_Dynamic_PID_Type中是否已经存在DIAG口 */
        AT_GetSpecificPort(AT_DEV_DIAG,
                           stDynamicPidType.aucRewindPortStyle,
                           &ulPortPos,
                           &ulPortNum);

        if (AT_DEV_NONE != ulPortPos)
        {
            return AT_OK;
        }

    }
    else
    {
        AT_WARN_LOG("AT_OpenDiagPort: en_NV_Item_Huawei_Dynamic_PID_Type is inactive!");
        return AT_OK;
    }

    /* DIAG端口操作权限未获取: 直接返回AT_OK */
    if (AT_E5_RIGHT_FLAG_NO == g_enATE5RightFlag)
    {
        return AT_OK;
    }

    /* 追加DIAG端口到切换后端口集 */
    if (AT_SETPORT_PARA_MAX_LEN == ulPortNum)
    {
        return AT_OK;
    }

    stDynamicPidType.aucRewindPortStyle[ulPortNum] = AT_DEV_DIAG;

    /* 更新端口集合数据到NV项en_NV_Item_Huawei_Dynamic_PID_Type */
    if (NV_OK != TAF_ACORE_NV_WRITE(MODEM_ID_0,
                                    en_NV_Item_Huawei_Dynamic_PID_Type,
                                    &stDynamicPidType,
                                    sizeof(AT_DYNAMIC_PID_TYPE_STRU)))
    {
        AT_ERR_LOG("AT_OpenDiagPort: Write NV fail");
        return AT_ERROR;
    }
    else
    {
        return AT_OK;
    }
}


VOS_UINT32 AT_CloseDiagPort(VOS_VOID)
{
    AT_DYNAMIC_PID_TYPE_STRU            stDynamicPidType;
    VOS_UINT32                          ulPortPos;
    VOS_UINT32                          ulPortNum;
    VOS_UINT32                          ulLoop;


    TAF_MEM_SET_S(&stDynamicPidType, sizeof(stDynamicPidType), 0x00, sizeof(stDynamicPidType));

    ulPortPos = 0;
    ulPortNum = 0;


    /* 读NV项en_NV_Item_Huawei_Dynamic_PID_Type获取当前的端口状态 */
    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0,
                                   en_NV_Item_Huawei_Dynamic_PID_Type,
                                   &stDynamicPidType,
                                   sizeof(AT_DYNAMIC_PID_TYPE_STRU)))
    {
        AT_ERR_LOG("AT_CloseDiagPort: Read NV fail!");
        return AT_ERROR;
    }

    /* 判断DIAG端口是否已经打开: 已经打开则直接返回AT_OK */
    if (VOS_TRUE == stDynamicPidType.ulNvStatus)
    {
        /* 查询NV项en_NV_Item_Huawei_Dynamic_PID_Type中是否已经存在DIAG口 */
        AT_GetSpecificPort(AT_DEV_DIAG,
                           stDynamicPidType.aucRewindPortStyle,
                           &ulPortPos,
                           &ulPortNum);

        if (AT_DEV_NONE == ulPortPos)
        {
            return AT_OK;
        }
    }
    else
    {
        return AT_OK;
    }

    /* DIAG端口操作权限未获取: 直接返回AT_OK */
    if (AT_E5_RIGHT_FLAG_NO == g_enATE5RightFlag)
    {
        return AT_OK;
    }

    /* 删除NV项中的DIAG端口 */
    stDynamicPidType.aucRewindPortStyle[ulPortPos] = 0;
    ulPortNum--;
    for (ulLoop = ulPortPos; ulLoop < ulPortNum; ulLoop++)
    {
        stDynamicPidType.aucRewindPortStyle[ulLoop] = stDynamicPidType.aucRewindPortStyle[ulLoop + 1UL];
    }
    stDynamicPidType.aucRewindPortStyle[ulPortNum] = 0;

    /* 端口异常数据保护: 切换后的设备形态中，第一个设备不能为MASS设备(0xa1,0xa2) */
    if (0 != ulPortNum)
    {
        if ((AT_DEV_CDROM == stDynamicPidType.aucRewindPortStyle[0])
         || (AT_DEV_SD == stDynamicPidType.aucRewindPortStyle[0]))
        {
            return AT_OK;
        }
    }

    /* 更新端口集合数据到NV项en_NV_Item_Huawei_Dynamic_PID_Type */
    if (NV_OK != TAF_ACORE_NV_WRITE(MODEM_ID_0,
                                    en_NV_Item_Huawei_Dynamic_PID_Type,
                                    &stDynamicPidType,
                                    sizeof(AT_DYNAMIC_PID_TYPE_STRU)))
    {
        AT_ERR_LOG("AT_CloseDiagPort: Write NV fail");
        return AT_ERROR;
    }
    else
    {
        return AT_OK;
    }
}


VOS_UINT32 AT_ExistSpecificPortChange(
    VOS_UINT8                           ucPortType,
    VOS_UINT8                           aucOldRewindPortStyle[],
    VOS_UINT8                           aucNewRewindPortStyle[]
)
{
    VOS_UINT32                          ulOldPortPos;
    VOS_UINT32                          ulNewPortPos;
    VOS_UINT32                          ulPortNum;


    ulOldPortPos = 0;
    ulNewPortPos = 0;
    ulPortNum    = 0;

    AT_GetSpecificPort(ucPortType, aucOldRewindPortStyle, &ulOldPortPos, &ulPortNum);
    AT_GetSpecificPort(ucPortType, aucNewRewindPortStyle, &ulNewPortPos, &ulPortNum);

    /* 新增一个指定端口 */
    if ((AT_DEV_NONE == ulOldPortPos)
     && (AT_DEV_NONE != ulNewPortPos))
    {
        return VOS_TRUE;
    }

    /* 删除一个指定端口 */
    if ((AT_DEV_NONE != ulOldPortPos)
     && (AT_DEV_NONE == ulNewPortPos))
    {
        return VOS_TRUE;
    }

    return VOS_FALSE;

}


VOS_UINT32 AT_CheckSetPortRight(
    VOS_UINT8                           aucOldRewindPortStyle[],
    VOS_UINT8                           aucNewRewindPortStyle[]
)
{
    VOS_UINT32                          ulRet;

    ulRet = AT_ExistSpecificPortChange(AT_DEV_DIAG,
                                       aucOldRewindPortStyle,
                                       aucNewRewindPortStyle);

    if ((VOS_TRUE == ulRet)
     && (AT_E5_RIGHT_FLAG_NO == g_enATE5RightFlag))
    {
        return AT_ERROR;
    }

    return AT_OK;
}




VOS_UINT32 AT_TestHsicCmdPara(VOS_UINT8 ucIndex)
{
    /* 通道检查 */
    if (VOS_FALSE == AT_IsApPort(ucIndex))
    {
        return AT_ERROR;
    }

    return AT_OK;
}


VOS_UINT32 At_TestTdsScalibPara(VOS_UINT8 ucIndex)
{
    VOS_UINT16                           usLength;

    usLength = 0;
    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR*)pgucAtSndCodeAddr,
                                        (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                        "%s:%s", g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                        AT_TDS_SCALIB_TEST_STR);
    gstAtSendData.usBufLen = usLength;
    return AT_OK;
}


VOS_UINT32 AT_TestSimlockUnlockPara( VOS_UINT8 ucIndex )
{
    /* 通道检查 */
    if (VOS_FALSE == AT_IsApPort(ucIndex))
    {
        return AT_ERROR;
    }

    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    "%s: (\"NET\",\"NETSUB\",\"SP\")",
                                                    g_stParseContext[ucIndex].pstCmdElement->pszCmdName);

    return AT_OK;
}


VOS_UINT32 AT_String2Hex( VOS_UINT8 *nptr,VOS_UINT16 usLen, VOS_UINT32 *pRtn)
{
    VOS_UINT32                          c     = 0;         /* current Char */
    VOS_UINT32                          total = 0;         /* current total */
    VOS_UINT32                          Length = 0;        /* current Length */

    for (Length = 0; Length < usLen; Length++)
    {
        c = (VOS_UINT32)(*nptr);    /* get next Char */
        nptr++;

        if( (c  >= '0') && (c  <= '9') )
        {
            c  = c  - '0';
        }
        else if( (c  >= 'a') && (c  <= 'f') )
        {
            c  = c  - 'a' + 10;
        }
        else if( (c  >= 'A') && (c  <= 'F') )
        {
            c  = c  - 'A' + 10;
        }
        else
        {
            return VOS_ERR;
        }

        if(total > 0x0FFFFFFF)              /* 发生反转 */
        {
            return VOS_ERR;
        }
        else
        {
            total = (total << 4) + c;       /* accumulate digit */
        }
    }

     /* return result, negated if necessary */
    *pRtn = total;
    return AT_SUCCESS;
}


VOS_UINT32 AT_NVWRGetItemValue( VOS_UINT8 *pucPara,  VOS_UINT8 *pucValue,  VOS_UINT8 **pucParaDst)
{
    VOS_UINT32                          ulTmp = 0;
    VOS_UINT8                          *pucStart = pucPara;
    VOS_UINT16                          usLen = 0;
    VOS_UINT8                          *pucEnd = VOS_NULL_PTR;
    VOS_UINT32                          ulRet;

    /* 调用的地方保证pucPara,pucValue,pucParaDst不为NULL */

    /* 直到第一个不是' '的字符 */
    while(' ' == *pucStart)
    {
        pucStart++;
    }

    if(' ' == *(pucStart+1))
    {
        usLen = 1;
        pucEnd = pucStart+2;
    }
    else if(' ' == *(pucStart+2))
    {
        usLen = 2;
        pucEnd = pucStart+3;
    }
    else
    {
        return VOS_ERR;
    }

    ulRet = AT_String2Hex(pucStart, usLen, &ulTmp);
    if((VOS_OK != ulRet) || (ulTmp > 0xff))
    {
        return VOS_ERR;
    }

    *pucValue = (VOS_UINT8)ulTmp;
    *pucParaDst = pucEnd;

    return VOS_OK;
}


VOS_UINT32 AT_NVWRGetParaInfo( AT_PARSE_PARA_TYPE_STRU * pstPara, VOS_UINT8 * pu8Data, VOS_UINT32 * pulLen)
{
    VOS_UINT32                          ulNum = 0;
    VOS_UINT8                          *pu8Start   = VOS_NULL_PTR;
    VOS_UINT8                          *pu8ParaTmp = VOS_NULL_PTR;
    VOS_UINT16                          usLen = 0;
    VOS_UINT32                          ulRet;
    VOS_UINT16                          i = 0;

    /* 调用的地方保证pstPara,pu8Data不为NULL */

    pu8Start = pstPara->aucPara;
    usLen= pstPara->usParaLen;

    if(usLen < AT_PARA_MAX_LEN)
    {
        pstPara->aucPara[usLen] = ' ';
    }
    else
    {
        return VOS_ERR;
    }

    ulNum = 0;
    for(i = 0; i < usLen; )
    {
        ulRet = AT_NVWRGetItemValue(pu8Start, (pu8Data+ulNum), &pu8ParaTmp);
        if(ERR_MSP_SUCCESS != ulRet)
        {
            return ulRet;
        }

        ulNum++;

        /* 最多128个 */
        /*MAX_NV_NUM_PER_PARA */
        if(ulNum == 128)
        {
            break;
        }

        if(pu8ParaTmp >= (pu8Start+usLen))
        {
            break;
        }

        i += (VOS_UINT16)(pu8ParaTmp - pu8Start);
        pu8Start = pu8ParaTmp;
    }

    *pulLen = ulNum;
    return VOS_OK;
}


VOS_UINT32 AT_SetNVReadPara(VOS_UINT8 ucIndex)
{
    VOS_UINT16                          usNvId  = 0;
    VOS_UINT32                          ulNvLen = 0;
    VOS_UINT8                          *pucData = VOS_NULL_PTR;
    VOS_UINT32                          i       = 0;
    MODEM_ID_ENUM_UINT16                enModemId = MODEM_ID_0;
    VOS_UINT32                          ulRet;

    /* 参数检查 */
    if(AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        g_ulNVRD = 1;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数过多 */
    if(gucAtParaIndex > 1)
    {
        g_ulNVRD = 2;
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数为空 */
    if(0 == gastAtParaList[0].usParaLen)
    {
        g_ulNVRD = 3;
        return AT_CME_INCORRECT_PARAMETERS;
    }
    else
    {
        usNvId = (VOS_UINT16)gastAtParaList[0].ulParaValue;
    }

    ulRet = TAF_ACORE_NV_GET_LENGTH(usNvId, &ulNvLen);

    if(VOS_OK != ulRet)
    {
        g_ulNVRD = 4;
        return AT_ERROR;
    }

    /* 长度大于128，只取前128个字节*/
    /*lint -save -e516 */
    pucData = (VOS_UINT8*)PS_MEM_ALLOC(WUEPS_PID_AT, ulNvLen);
    /*lint -restore */
    if(VOS_NULL_PTR == pucData)
    {
        g_ulNVRD = 5;
        return AT_ERROR;
    }

    ulRet = AT_GetModemIdFromClient(ucIndex, &enModemId);

    if (VOS_OK != ulRet)
    {
        AT_ERR_LOG("AT_SetNVReadPara:Get modem id fail");
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, pucData);
        /*lint -restore */
        g_ulNVWR =8;
        return AT_ERROR;
    }

    ulRet = TAF_ACORE_NV_READ(enModemId, usNvId, (VOS_VOID*)pucData, ulNvLen);

    if(VOS_OK != ulRet)
    {
        /*lint -save -e516 */
        PS_MEM_FREE(WUEPS_PID_AT, pucData);
        /*lint -restore */
        g_ulNVRD = 6;
        return AT_ERROR;
    }
    gstAtSendData.usBufLen = 0;
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN, (VOS_CHAR *)pgucAtSndCodeAddr,(VOS_CHAR*)pgucAtSndCodeAddr,
        "^NVRD: %d,", ulNvLen);

    for(i = 0; i < ulNvLen; i++)
    {
        if(0 == i)
        {
            gstAtSendData.usBufLen += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                (VOS_CHAR *)pgucAtSndCodeAddr,(VOS_CHAR*)pgucAtSndCodeAddr + gstAtSendData.usBufLen,"%02X", pucData[i]);
        }
        else
        {
            gstAtSendData.usBufLen += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                (VOS_CHAR *)pgucAtSndCodeAddr,(VOS_CHAR*)pgucAtSndCodeAddr + gstAtSendData.usBufLen," %02X", pucData[i]);
        }
    }
    /*lint -save -e516 */
    PS_MEM_FREE(WUEPS_PID_AT, pucData);
    /*lint -restore */
    g_ulNVRD = 7;
    return AT_OK;
}



VOS_VOID AT_GetNvRdDebug(VOS_VOID)
{
    PS_PRINTF("\n g_ulNVRD=0x%x \n",g_ulNVRD);
    PS_PRINTF("\n g_ulNVWR=0x%x \n",g_ulNVWR);
}


VOS_UINT32 AT_QryFPllStatusPara(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulReceiverPid;
    AT_PHY_RF_PLL_STATUS_REQ_STRU      *pstMsg;
    VOS_UINT32                          ulLength;
    VOS_UINT16                          usMsgId;
    if ((AT_RAT_MODE_FDD_LTE == g_stAtDevCmdCtrl.ucDeviceRatMode)
            ||(AT_RAT_MODE_TDD_LTE == g_stAtDevCmdCtrl.ucDeviceRatMode))
    {
        return atQryFPllStatusPara(ucIndex);
    }
    /*判断当前接入模式，只支持G/W*/
    if (AT_RAT_MODE_WCDMA == g_stAtDevCmdCtrl.ucDeviceRatMode)
    {
        ulReceiverPid = AT_GetDestPid(ucIndex, I0_DSP_PID_WPHY);
        usMsgId       = ID_AT_WPHY_RF_PLL_STATUS_REQ;
    }
    else if ( (AT_RAT_MODE_GSM == g_stAtDevCmdCtrl.ucDeviceRatMode)
            ||(AT_RAT_MODE_EDGE == g_stAtDevCmdCtrl.ucDeviceRatMode) )
    {
        ulReceiverPid = AT_GetDestPid(ucIndex, I0_DSP_PID_GPHY);
        usMsgId       = ID_AT_GPHY_RF_PLL_STATUS_REQ;
    }

    else
    {
        return AT_DEVICE_MODE_ERROR;
    }

    /* 申请AT_PHY_RF_PLL_STATUS_REQ_STRU消息 */
    ulLength = sizeof(AT_PHY_RF_PLL_STATUS_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e516 */
    pstMsg   = (AT_PHY_RF_PLL_STATUS_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, ulLength);
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_QryFPllStatusPara: Alloc msg fail!");
        return AT_ERROR;
    }

    /* 填充消息 */
    pstMsg->ulReceiverPid = ulReceiverPid;
    pstMsg->usMsgID       = usMsgId;
    pstMsg->usRsv1        = 0;
    pstMsg->usDspBand     = g_stAtDevCmdCtrl.stDspBandArfcn.usDspBand;
    pstMsg->usRsv2        = 0;

    /* 向对应PHY发送消息 */
    if (VOS_OK != PS_SEND_MSG(WUEPS_PID_AT, pstMsg))
    {
        AT_WARN_LOG("AT_QryFPllStatusPara: Send msg fail!");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_FPLLSTATUS_QRY;
    g_stAtDevCmdCtrl.ucIndex               = ucIndex;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID At_RfPllStatusCnfProc(PHY_AT_RF_PLL_STATUS_CNF_STRU *pstMsg)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT16                          usLength;

    /* 获取本地保存的用户索引 */
    ucIndex = g_stAtDevCmdCtrl.ucIndex;

    if (AT_CMD_FPLLSTATUS_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("At_RfPllStatusCnfProc: CmdCurrentOpt is not AT_CMD_FPLLSTATUS_QRY!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       "%s: %d,%d",
                                       g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                       pstMsg->usTxStatus,
                                       pstMsg->usRxStatus);

    gstAtSendData.usBufLen = usLength;

    At_FormatResultData(ucIndex, AT_OK);

    return;
}


VOS_UINT32 AT_QryFpowdetTPara(VOS_UINT8 ucIndex)
{
    AT_PHY_POWER_DET_REQ_STRU          *pstMsg;
    VOS_UINT32                          ulLength;
    VOS_UINT32                          ulResult;
    VOS_UINT8                           ucIsLteFlg;

    ucIsLteFlg = VOS_FALSE;

    /*判断当前接入模式，只支持W*/
    if ( (AT_RAT_MODE_WCDMA != g_stAtDevCmdCtrl.ucDeviceRatMode)
      && (AT_RAT_MODE_CDMA != g_stAtDevCmdCtrl.ucDeviceRatMode)
      && (AT_RAT_MODE_GSM != g_stAtDevCmdCtrl.ucDeviceRatMode)
      && (AT_RAT_MODE_FDD_LTE != g_stAtDevCmdCtrl.ucDeviceRatMode)
      && (AT_RAT_MODE_TDD_LTE != g_stAtDevCmdCtrl.ucDeviceRatMode) )
    {
        return AT_DEVICE_MODE_ERROR;
    }

    /* 申请AT_PHY_POWER_DET_REQ_STRU消息 */
    ulLength = sizeof(AT_PHY_POWER_DET_REQ_STRU) - VOS_MSG_HEAD_LENGTH;
    /*lint -save -e516 */
    pstMsg   = (AT_PHY_POWER_DET_REQ_STRU *)PS_ALLOC_MSG(WUEPS_PID_AT, ulLength);
    /*lint -restore */
    if (VOS_NULL_PTR == pstMsg)
    {
        AT_WARN_LOG("AT_QryFpowdetTPara: Alloc msg fail!");
        return AT_ERROR;
    }

    /* CDMA的话，发送给UPHY_PID_CSDR_1X_CM */
    if (AT_RAT_MODE_CDMA == g_stAtDevCmdCtrl.ucDeviceRatMode)
    {
        pstMsg->ulReceiverPid = UPHY_PID_CSDR_1X_CM;
    }
    else
    if (AT_RAT_MODE_GSM == g_stAtDevCmdCtrl.ucDeviceRatMode)
    {
        pstMsg->ulReceiverPid = AT_GetDestPid(ucIndex, I0_DSP_PID_GPHY);
    }
    else if (AT_RAT_MODE_WCDMA == g_stAtDevCmdCtrl.ucDeviceRatMode)
    {
        pstMsg->ulReceiverPid = AT_GetDestPid(ucIndex, I0_DSP_PID_WPHY);
    }
    else
    {
        ucIsLteFlg = VOS_TRUE;
    }

    if (VOS_FALSE == ucIsLteFlg)
    {
        pstMsg->usMsgID       = ID_AT_PHY_POWER_DET_REQ;
        pstMsg->usRsv         = 0;

        /* 向对应PHY发送消息 */
        ulResult = PS_SEND_MSG(WUEPS_PID_AT, pstMsg);
    }
    else
    {
        /*lint --e{516,830} */
        PS_FREE_MSG(WUEPS_PID_AT, pstMsg);
        ulResult = AT_FillAndSndAppReqMsg(gastAtClientTab[ucIndex].usClientId,
                                          gastAtClientTab[ucIndex].opId,
                                          ID_AT_MTA_POWER_DET_QRY_REQ,
                                          VOS_NULL_PTR,
                                          0,
                                          I0_UEPS_PID_MTA);
    }

    if (VOS_OK != ulResult)
    {
        AT_WARN_LOG("AT_QryFpowdetTPara: Send msg fail!");
        return AT_ERROR;
    }

    /* 设置当前操作类型 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_FPOWDET_QRY;
    g_stAtDevCmdCtrl.ucIndex               = ucIndex;

    /* 返回命令处理挂起状态 */
    return AT_WAIT_ASYNC_RETURN;
}


VOS_VOID At_RfFpowdetTCnfProc(PHY_AT_POWER_DET_CNF_STRU *pstMsg)
{
    VOS_UINT8                           ucIndex;
    VOS_UINT16                          usLength;

    /* 获取本地保存的用户索引 */
    ucIndex = g_stAtDevCmdCtrl.ucIndex;

    if (AT_CMD_FPOWDET_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("At_RfFPOWDETCnfProc: CmdCurrentOpt is not AT_CMD_FPOWDET_QRY!");
        return;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* 应物理层要求，如果返回值为0x7FFF则为无效值，项查询者返回ERROR */
    if(0x7FFF == pstMsg->sPowerDet)
    {
        gstAtSendData.usBufLen = 0;
        At_FormatResultData(ucIndex, AT_ERROR);
    }
    else
    {
        usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           "%s: %d",
                                           g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                           pstMsg->sPowerDet);

        gstAtSendData.usBufLen = usLength;

        At_FormatResultData(ucIndex, AT_OK);
    }

    return;
}


VOS_UINT32 AT_RcvMtaPowerDetQryCnf(VOS_VOID *pMsg)
{
    /* 定义局部变量 */
    AT_MTA_MSG_STRU                    *pstMtaMsg;
    MTA_AT_POWER_DET_QRY_CNF_STRU      *pstPowerDetQryCnf;
    PHY_AT_POWER_DET_CNF_STRU           stPowerNetMsg;

    /* 初始化消息变量 */
    pstMtaMsg           = (AT_MTA_MSG_STRU *)pMsg;
    pstPowerDetQryCnf   = (MTA_AT_POWER_DET_QRY_CNF_STRU *)pstMtaMsg->aucContent;

    TAF_MEM_SET_S(&stPowerNetMsg, sizeof(stPowerNetMsg), 0x00, sizeof(PHY_AT_POWER_DET_CNF_STRU));

    if (MTA_AT_RESULT_NO_ERROR == pstPowerDetQryCnf->enResult)
    {
        stPowerNetMsg.sPowerDet = pstPowerDetQryCnf->sPowerDet;
    }
    else
    {
        stPowerNetMsg.sPowerDet = 0x7FFF;
    }

    At_RfFpowdetTCnfProc(&stPowerNetMsg);

    return VOS_OK;
}



VOS_BOOL AT_IsNVWRAllowedNvId(VOS_UINT16 usNvId)
{
    TAF_NV_NVWR_SEC_CTRL_STRU           stNvwrSecCtrlNV;
    VOS_UINT8                           ucLoop;
    VOS_UINT8                           ucBlackListNum;

    /* 参数初始化 */
    TAF_MEM_SET_S(&stNvwrSecCtrlNV, sizeof(stNvwrSecCtrlNV), 0x00, sizeof(stNvwrSecCtrlNV));

    /* 读取安全控制NV */
    if (NV_OK != TAF_ACORE_NV_READ(MODEM_ID_0, en_NV_Item_NVWR_SEC_CTRL, &stNvwrSecCtrlNV, sizeof(stNvwrSecCtrlNV)))
    {
        AT_ERR_LOG("AT_IsNVWRAllowedNvId: TAF_ACORE_NV_READ fail!");
        return VOS_FALSE;
    }

    switch (stNvwrSecCtrlNV.ucSecType)
    {
        case AT_NVWR_SEC_TYPE_OFF:
            return VOS_TRUE;

        case AT_NVWR_SEC_TYPE_ON:
            return VOS_FALSE;

        case AT_NVWR_SEC_TYPE_BLACKLIST:
            ucBlackListNum = (stNvwrSecCtrlNV.ucBlackListNum <= TAF_NV_BLACK_LIST_MAX_NUM) ?
                             stNvwrSecCtrlNV.ucBlackListNum : TAF_NV_BLACK_LIST_MAX_NUM;
            for (ucLoop = 0; ucLoop < ucBlackListNum; ucLoop++)
            {
                if (usNvId == stNvwrSecCtrlNV.ausBlackList[ucLoop])
                {
                    return VOS_FALSE;
                }
            }
            return VOS_TRUE;

        default:
            AT_ERR_LOG1("AT_IsNVWRAllowedNvId: Error SecType:", stNvwrSecCtrlNV.ucSecType);
            break;
    }

    return VOS_FALSE;
}



VOS_UINT32 AT_AsciiToHex(
    VOS_UINT8                          *pucSrc,
    VOS_UINT8                          *pucDst
)
{

    if (( *pucSrc >= '0') && (*pucSrc <= '9')) /* the number is 0-9 */
    {
        *pucDst = (VOS_UINT8)(*pucSrc - '0');
    }
    else if ( (*pucSrc >= 'a') && (*pucSrc <= 'f') ) /* the number is a-f */
    {
        *pucDst = (VOS_UINT8)(*pucSrc - 'a') + 0x0a;
    }
    else if ( (*pucSrc >= 'A') && (*pucSrc <= 'F') ) /* the number is A-F */
    {
        *pucDst = (VOS_UINT8)(*pucSrc - 'A') + 0x0a;
    }
    else
    {
        return VOS_ERR;
    }

    return VOS_OK;
}



VOS_UINT32 AT_AsciiToHexCode_Revers(
    VOS_UINT8                          *pucSrc,
    VOS_UINT16                          usDataLen,
    VOS_UINT8                          *pucDst
)
{
    VOS_INT16                           sLoop1;
    VOS_UINT16                          usLoop2;
    VOS_UINT8                           ucTemp1;
    VOS_UINT8                           ucTemp2;
    VOS_UINT32                          ulRslt;

    sLoop1 = (VOS_INT16)(usDataLen - 1);
    for (usLoop2 = 0; sLoop1 >= 0; sLoop1--, usLoop2++)
    {
        ulRslt = AT_AsciiToHex(&(pucSrc[sLoop1]), &ucTemp1);
        if (VOS_ERR == ulRslt)
        {
            return VOS_ERR;
        }

        sLoop1--;

        if (sLoop1 < 0)
        {
            AT_ERR_LOG("AT_AsciiToHexCode_Revers: sLoop1 is invalid.");

            return VOS_ERR;
        }

        ulRslt = AT_AsciiToHex(&(pucSrc[sLoop1]), &ucTemp2);
        if (VOS_ERR == ulRslt)
        {
            return VOS_ERR;
        }

        pucDst[usLoop2] = (VOS_UINT8)((ucTemp2 << 4) | ucTemp1);
    }

    return VOS_OK;
}


VOS_UINT32 AT_Hex2Ascii_Revers(
    VOS_UINT8                           aucHex[],
    VOS_UINT32                          ulLength,
    VOS_UINT8                           aucAscii[]
)
{
    VOS_INT32                           lLoopSrc;
    VOS_UINT32                          ulLoopDest;
    VOS_UINT8                           ucTemp;

    lLoopSrc = (VOS_INT32)(ulLength - 1);
    for (ulLoopDest = 0; lLoopSrc >= 0; lLoopSrc--, ulLoopDest++)
    {
        ucTemp = (aucHex[lLoopSrc]>>4) & 0x0F;
        if (ucTemp < 10)
        {
            /* 0~9 */
            aucAscii[ulLoopDest] = ucTemp + 0x30;
        }
        else
        {
            /* a~f */
            aucAscii[ulLoopDest] = ucTemp + 0x37;
        }

        ulLoopDest++;
        ucTemp = aucHex[lLoopSrc] & 0x0F;
        if (ucTemp < 10)
        {
            /* 0~9 */
            aucAscii[ulLoopDest] = ucTemp + 0x30;
        }
        else
        {
            /* a~f */
            aucAscii[ulLoopDest] = ucTemp + 0x37;
        }
    }

    return VOS_OK;
}


VOS_UINT32 AT_SetMeidPara(VOS_UINT8 ucIndex)
{
    AT_MTA_MEID_SET_REQ_STRU            stMeIdReq;
    VOS_UINT32                          ulRslt;

    TAF_MEM_SET_S(&stMeIdReq, sizeof(stMeIdReq), 0x00, sizeof(AT_MTA_MEID_SET_REQ_STRU));

    /* 命令状态检查 */
    if (AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不为1或者字符串长度不为14 */
    if ((1 != gucAtParaIndex)
     || (14 != gastAtParaList[0].usParaLen))
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ulRslt = AT_AsciiToHexCode_Revers(gastAtParaList[0].aucPara,
                                      gastAtParaList[0].usParaLen,
                                      stMeIdReq.aucMeid);

    if (VOS_OK != ulRslt)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    ulRslt = AT_FillAndSndAppReqMsg(gastAtClientTab[ucIndex].usClientId,
                                    gastAtClientTab[ucIndex].opId,
                                    ID_AT_MTA_MEID_SET_REQ,
                                    &stMeIdReq,
                                    sizeof(stMeIdReq),
                                    I0_UEPS_PID_MTA);

    if (TAF_SUCCESS == ulRslt)
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_MEID_SET;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    }
    else
    {
        AT_WARN_LOG("AT_SetMeidPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

}


VOS_UINT32 AT_QryMeidPara(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulResult;

    /* 参数检查 */
    if (AT_CMD_OPT_READ_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_ERROR;
    }

    /* 发送消息*/
    ulResult = AT_FillAndSndAppReqMsg(gastAtClientTab[ucIndex].usClientId,
                                      gastAtClientTab[ucIndex].opId,
                                      ID_AT_MTA_MEID_QRY_REQ,
                                      VOS_NULL_PTR,
                                      0,
                                      I0_UEPS_PID_MTA);

    if (TAF_SUCCESS == ulResult)
    {
        gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_MEID_QRY;

        /* 返回命令处理挂起状态 */
        return AT_WAIT_ASYNC_RETURN;
    }
    else
    {
        AT_WARN_LOG("AT_QryMeidPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }
}


VOS_UINT32 AT_RcvMtaMeidSetCnf(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                    *pstRcvMsg;
    MTA_AT_RESULT_CNF_STRU             *pstSetCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulRslt;

    /* 初始化 */
    pstRcvMsg       = (AT_MTA_MSG_STRU *)pMsg;
    pstSetCnf       = (MTA_AT_RESULT_CNF_STRU *)pstRcvMsg->aucContent;
    ucIndex         = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过ClientId获取ucIndex */
    if (AT_FAILURE == At_ClientIdToUserId(pstRcvMsg->stAppCtrl.usClientId, &ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaMeidSetCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaMeidSetCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_MEID_SET */
    if (AT_CMD_MEID_SET != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvMtaMeidSetCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* 格式化AT^MEID命令返回 */
    gstAtSendData.usBufLen = 0;

    switch (pstSetCnf->enResult)
    {
        case MTA_AT_RESULT_INCORRECT_PARAMETERS:
            ulRslt = AT_DEVICE_INVALID_PARAMETERS;
            break;

        case MTA_AT_RESULT_DEVICE_SEC_NV_ERROR:
            ulRslt = AT_DEVICE_NV_WRITE_FAIL_UNKNOWN;
            break;

        case MTA_AT_RESULT_NO_ERROR:
            ulRslt = AT_OK;
            break;

        default:
            ulRslt = AT_ERROR;
            break;
    }

    At_FormatResultData(ucIndex, ulRslt);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaMeidQryCnf(
    VOS_VOID                           *pMsg
)
{
    AT_MTA_MSG_STRU                    *pstRcvMsg = VOS_NULL_PTR;
    MTA_AT_MEID_QRY_CNF_STRU           *pstQryCnf = VOS_NULL_PTR;
    VOS_UINT8                           ucIndex;
    VOS_UINT8                           aucMeId[2*MTA_AT_MEID_NV_DATA_LEN_NEW + 1];
    VOS_UINT8                           aucpEsn[2*MTA_AT_PESN_NV_DATA_LEN + 1];
    VOS_UINT8                           aucpUimID[2*UIMID_DATA_LEN + 1];

    TAF_MEM_SET_S(aucMeId, sizeof(aucMeId), 0x00, sizeof(aucMeId));

    /* 初始化 */
    pstRcvMsg       = (AT_MTA_MSG_STRU *)pMsg;
    pstQryCnf       = (MTA_AT_MEID_QRY_CNF_STRU *)pstRcvMsg->aucContent;
    ucIndex         = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /* 通过ClientId获取ucIndex */
    if (AT_FAILURE == At_ClientIdToUserId(pstRcvMsg->stAppCtrl.usClientId, &ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaMeidQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaMeidQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_MEID_QRY */
    if (AT_CMD_MEID_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvMtaMeidQryCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* 格式化AT^MEID命令返回 */
    gstAtSendData.usBufLen = 0;

    /* 读取en_NV_Item_MEID */
    if (NV_OK != pstQryCnf->ulMeIDReadRst)
    {
        AT_WARN_LOG("AT_RcvMtaMeidQryCnf:Read en_NV_Item_MEID Nvim Failed");
        return AT_DEVICE_NV_READ_FAILURE;
    }

    /* 读取en_NV_Item_PESN */
    if (NV_OK != pstQryCnf->ulPEsnReadRst)
    {
        AT_WARN_LOG("AT_RcvMtaMeidQryCnf:Read en_NV_Item_PESN Nvim Failed");
        return AT_DEVICE_NV_READ_FAILURE;
    }

    /* 读取UIMID失败 */
    if (pstQryCnf->enResult != MTA_AT_RESULT_NO_ERROR)
    {
        AT_WARN_LOG("AT_RcvMtaMeidQryCnf:Read UIMID Failed");
    }

    AT_Hex2Ascii_Revers(&(pstQryCnf->aucEFRUIMID[1]), UIMID_DATA_LEN, aucpUimID);
    aucpUimID[2*UIMID_DATA_LEN] = '\0';

    AT_Hex2Ascii_Revers(&(pstQryCnf->aucPEsn[0]), MTA_AT_PESN_NV_DATA_LEN, aucpEsn);
    aucpEsn[2*MTA_AT_PESN_NV_DATA_LEN] = '\0';

    AT_Hex2Ascii_Revers(&(pstQryCnf->aucMeID[0]), MTA_AT_MEID_NV_DATA_LEN_NEW, aucMeId);
    aucMeId[2*MTA_AT_MEID_NV_DATA_LEN_NEW] = '\0';

    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    "%s: %s,%s,%s",
                                                    g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                    aucMeId,
                                                    aucpEsn,
                                                    aucpUimID);

    At_FormatResultData(ucIndex, AT_OK);

    return VOS_OK;
}



VOS_UINT32 AT_QryRficDieIDPara(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulRst;

    if(AT_CMD_OPT_READ_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询RFIC IDE ID*/
    ulRst = AT_FillAndSndAppReqMsg(gastAtClientTab[ucIndex].usClientId,
                                   gastAtClientTab[ucIndex].opId,
                                   ID_AT_MTA_RFIC_DIE_ID_QRY_REQ,
                                   VOS_NULL_PTR,
                                   0,
                                   I0_UEPS_PID_MTA);

    if (TAF_SUCCESS != ulRst)
    {
        AT_WARN_LOG("AT_QryRficDieIDPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_RFIC_DIE_ID_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaRficDieIDQryCnf( VOS_VOID *pMsg )
{
    AT_MTA_MSG_STRU                    *pstRcvMsg;
    MTA_AT_RFIC_DIE_ID_REQ_CNF_STRU    *pstRficDieIDReqCnf;
    VOS_UINT8                           ucIndex;
    VOS_UINT16                          usLength;

    /*初始化局部变量*/
    pstRcvMsg                 = (AT_MTA_MSG_STRU *)pMsg;
    pstRficDieIDReqCnf        = (MTA_AT_RFIC_DIE_ID_REQ_CNF_STRU *)pstRcvMsg->aucContent;
    ucIndex                   = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    usLength                  = 0;

    /*通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(pstRcvMsg->stAppCtrl.usClientId, &ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaRficDieIDQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaRficDieIDQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_RFIC_DIE_ID_QRY */
    if (AT_CMD_RFIC_DIE_ID_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvMtaRficDieIDQryCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /*格式化上报命令*/
    if (MTA_AT_RESULT_NO_ERROR != pstRficDieIDReqCnf->enResult)
    {
        /*命令结果 *AT_ERROR*/
        gstAtSendData.usBufLen = 0;
        At_FormatResultData(ucIndex, AT_ERROR);
    }
    else
    {
        /*命令结果 *AT_OK*/
        if (VOS_TRUE == pstRficDieIDReqCnf->usRfic0DieIdValid)
        {
            usLength = AT_RficDieIDOut((VOS_UINT8*)pstRficDieIDReqCnf->ausRfic0DieId, 0, usLength, ucIndex);
        }

        if (VOS_TRUE == pstRficDieIDReqCnf->usRfic1DieIdValid)
        {
            usLength = AT_RficDieIDOut((VOS_UINT8*)pstRficDieIDReqCnf->ausRfic1DieId, 1, usLength, ucIndex);
        }

        if (VOS_TRUE == pstRficDieIDReqCnf->usRfic2DieIdValid)
        {
            usLength = AT_RficDieIDOut((VOS_UINT8*)pstRficDieIDReqCnf->ausRfic2DieId, 2, usLength, ucIndex);
        }
        if (VOS_TRUE == pstRficDieIDReqCnf->usRfic3DieIdValid)
        {
            usLength = AT_RficDieIDOut((VOS_UINT8*)pstRficDieIDReqCnf->ausRfic3DieId, 3, usLength, ucIndex);
        }

        gstAtSendData.usBufLen = usLength;
        At_FormatResultData(ucIndex, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT16 AT_RficDieIDOut(
    VOS_UINT8                          *pMsg,
    VOS_UINT32                          RficNum,
    VOS_UINT16                          usLength,
    VOS_UINT8                           ucIndex
)
{
    VOS_UINT32                          i;           /*循环用*/
    VOS_UINT16                          usLengthtemp;

    usLengthtemp = usLength;

    usLengthtemp += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr + usLengthtemp,
                                       "%s: %d,\"",
                                       g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                       RficNum);

    /*RFIC ID 使用低八位数据*/
    for (i =  0; i < (MTA_AT_MAX_DIE_ID_LEN * 2); i += 2)
    {
        usLengthtemp += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                           (VOS_CHAR *)pgucAtSndCodeAddr,
                                           (VOS_CHAR *)pgucAtSndCodeAddr + usLengthtemp,
                                           "%02x",
                                           pMsg[i]);
    }

    usLengthtemp += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr + usLengthtemp,
                                       "\"%s",
                                       gaucAtCrLf);
    return usLengthtemp;
}


VOS_UINT32 AT_QryPmuDieSNPara(VOS_UINT8 ucIndex)
{
    VOS_UINT32                          ulRst;

    if(AT_CMD_OPT_READ_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_ERROR;
    }

    /* 发送跨核消息到C核, 查询RFIC IDE ID*/
    ulRst = AT_FillAndSndAppReqMsg(gastAtClientTab[ucIndex].usClientId,
                                   gastAtClientTab[ucIndex].opId,
                                   ID_AT_MTA_PMU_DIE_SN_QRY_REQ,
                                   VOS_NULL_PTR,
                                   0,
                                   I0_UEPS_PID_MTA);

    if (TAF_SUCCESS != ulRst)
    {
        AT_WARN_LOG("AT_QryPmuDieSNPara: AT_FillAndSndAppReqMsg fail.");
        return AT_ERROR;
    }

    /* 设置AT模块实体的状态为等待异步返回 */
    gastAtClientTab[ucIndex].CmdCurrentOpt = AT_CMD_PMU_DIE_SN_QRY;

    return AT_WAIT_ASYNC_RETURN;
}


VOS_UINT32 AT_RcvMtaPmuDieSNQryCnf( VOS_VOID *pMsg )
{
    AT_MTA_MSG_STRU                    *pstRcvMsg;
    MTA_AT_PMU_DIE_SN_REQ_CNF_STRU     *pstPmuDieSNReqCnf;
    VOS_INT32                           i;        /*循环数*/
    VOS_UINT16                          usLength;
    VOS_UINT8                           ucIndex;

    /*初始化局部变量*/
    pstRcvMsg                 = (AT_MTA_MSG_STRU *)pMsg;
    pstPmuDieSNReqCnf         = (MTA_AT_PMU_DIE_SN_REQ_CNF_STRU *)pstRcvMsg->aucContent;
    ucIndex                   = AT_BROADCAST_CLIENT_INDEX_MODEM_0;
    usLength                  = 0;

    /*通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(pstRcvMsg->stAppCtrl.usClientId, &ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaPmuDieSNQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaPmuDieSNQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_PMU_DIE_SN_QRY */
    if (AT_CMD_PMU_DIE_SN_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvMtaPmuDieSNQryCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /*格式化上报命令*/
    if (MTA_AT_RESULT_NO_ERROR != pstPmuDieSNReqCnf->enResult)
    {
        /*命令结果 *AT_ERROR*/
        gstAtSendData.usBufLen = 0;
        At_FormatResultData(ucIndex, AT_ERROR);
    }
    else
    {
        /*命令结果 *AT_OK*/

        /* 最高位,高4 BIT置0 */
        pstPmuDieSNReqCnf->aucDieSN[MTA_PMU_MAX_DIE_SN_LEN - 1] = (pstPmuDieSNReqCnf->aucDieSN[MTA_PMU_MAX_DIE_SN_LEN - 1] & 0x0F);

        /* 格式化输出查询结果 */
        usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          "%s: 0x",
                                          g_stParseContext[ucIndex].pstCmdElement->pszCmdName);

        for (i = (MTA_PMU_MAX_DIE_SN_LEN-1); i >= 0; i--)
        {
            usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                               (VOS_CHAR *)pgucAtSndCodeAddr,
                                               (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                               "%02x",
                                               pstPmuDieSNReqCnf->aucDieSN[i]);
        }

        gstAtSendData.usBufLen = usLength;
        At_FormatResultData(ucIndex, AT_OK);
    }

    return VOS_OK;
}



VOS_UINT32 AT_RcvMtaTasTestCfgCnf( VOS_VOID *pMsg )
{
    AT_MTA_MSG_STRU                    *pstRcvMsg;
    MTA_AT_TAS_TEST_CFG_CNF_STRU       *pstMtaAtTasTestCfgCnf = VOS_NULL_PTR;

    VOS_UINT8                           ucIndex;
    VOS_UINT32                          ulResult;

    /*初始化局部变量*/
    pstRcvMsg                 = (AT_MTA_MSG_STRU *)pMsg;
    pstMtaAtTasTestCfgCnf     = (MTA_AT_TAS_TEST_CFG_CNF_STRU *)pstRcvMsg->aucContent;
    ucIndex                   = AT_BROADCAST_CLIENT_INDEX_MODEM_0;

    /*通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(pstRcvMsg->stAppCtrl.usClientId, &ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaTasTestCfgCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaTasTestCfgCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_TAS_TEST_CFG_SET */
    if (AT_CMD_TAS_TEST_SET != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvMtaTasTestCfgCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /*格式化上报命令*/
    if (MTA_AT_RESULT_NO_ERROR == pstMtaAtTasTestCfgCnf->enResult)
    {
        /*命令结果 *AT_OK*/
        ulResult    = AT_OK;
    }
    else
    {
        /*命令结果 *AT_ERROR*/
        ulResult    = AT_ERROR;
    }

    gstAtSendData.usBufLen = 0;
    At_FormatResultData(ucIndex, ulResult);

    return VOS_OK;
}


VOS_UINT32 AT_RcvMtaTasTestQryCnf( VOS_VOID *pMsg )
{
    AT_MTA_MSG_STRU                    *pstRcvMsg;
    MTA_AT_TAS_TEST_QRY_CNF_STRU       *pstMtaAtTasTestReqCnf;
    VOS_UINT16                          usLength;
    VOS_UINT8                           ucIndex;

    /* 初始化局部变量 */
    pstRcvMsg                 = (AT_MTA_MSG_STRU *)pMsg;
    pstMtaAtTasTestReqCnf     = (MTA_AT_TAS_TEST_QRY_CNF_STRU *)pstRcvMsg->aucContent;
    usLength                  = 0;
    ucIndex                   = 0;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(pstRcvMsg->stAppCtrl.usClientId, &ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaTasTestQryCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaTasTestQryCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 判断当前操作类型是否为AT_CMD_RFIC_DIE_ID_QRY */
    if (AT_CMD_TAS_TEST_QRY != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvMtaTasTestQryCnf: NOT CURRENT CMD OPTION!");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* 格式化上报命令 */
    if (MTA_AT_RESULT_NO_ERROR != pstMtaAtTasTestReqCnf->enResult)
    {
        /* 命令结果 *AT_ERROR */
        gstAtSendData.usBufLen = 0;
        At_FormatResultData(ucIndex, AT_ERROR);
    }
    else
    {
        /* 命令结果 *AT_OK */
        usLength = AT_TasTestOut(pstMtaAtTasTestReqCnf);
        gstAtSendData.usBufLen = usLength;
        At_FormatResultData(ucIndex, AT_OK);
    }

    return VOS_OK;
}


VOS_UINT16 AT_TasTestOut(
    MTA_AT_TAS_TEST_QRY_CNF_STRU       *pstMtaAtTasTestReqCnf
)
{
    VOS_UINT32                          i;           /*循环用*/
    VOS_UINT16                          usLength;

    usLength = 0;

    usLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      (VOS_CHAR *)pgucAtSndCodeAddr,
                                      "level index: %d (%d, %d, %d, %d)%s",
                                       pstMtaAtTasTestReqCnf->ulCurrLevel,
                                       pstMtaAtTasTestReqCnf->stLevelInfo.uhwSrcAntTimeLength,
                                       pstMtaAtTasTestReqCnf->stLevelInfo.uhwDestAntTimeLength,
                                       pstMtaAtTasTestReqCnf->stLevelInfo.shwSrcAntTxPowerGain,
                                       pstMtaAtTasTestReqCnf->stLevelInfo.shwDestAntTxPowerGain,
                                       gaucAtCrLf);

    usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                       (VOS_CHAR *)pgucAtSndCodeAddr,
                                       (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                       "level table:%s",
                                       gaucAtCrLf);

    if (pstMtaAtTasTestReqCnf->stUsedTable.uhwLevelNum > MTA_AT_MAX_STATEII_LEVEL_ITEM)
    {
        pstMtaAtTasTestReqCnf->stUsedTable.uhwLevelNum = MTA_AT_MAX_STATEII_LEVEL_ITEM;
    }

    /* 打印表格 */
    for (i =  0; i < pstMtaAtTasTestReqCnf->stUsedTable.uhwLevelNum; i++)
    {
        usLength += (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr + usLength,
                                          "            (%d, %d, %d, %d)%s",
                                           pstMtaAtTasTestReqCnf->stUsedTable.astLevelItem[i].uhwSrcAntTimeLength,
                                           pstMtaAtTasTestReqCnf->stUsedTable.astLevelItem[i].uhwDestAntTimeLength,
                                           pstMtaAtTasTestReqCnf->stUsedTable.astLevelItem[i].shwSrcAntTxPowerGain,
                                           pstMtaAtTasTestReqCnf->stUsedTable.astLevelItem[i].shwDestAntTxPowerGain,
                                           gaucAtCrLf);
    }
    return usLength;
}


VOS_UINT32 AT_RcvMtaSetMipiReadCnf(VOS_VOID *pstMsg)
{
    AT_MTA_MSG_STRU                    *pRcvMsg             = VOS_NULL_PTR;
    MTA_AT_MIPI_READ_CNF_STRU          *pstSetMipiReadCnf   = VOS_NULL_PTR;
    VOS_UINT16                          ulLength;
    VOS_UINT8                           ucIndex;

    /* 初始化 */
    pRcvMsg             = (AT_MTA_MSG_STRU*)pstMsg;
    pstSetMipiReadCnf   = (MTA_AT_MIPI_READ_CNF_STRU*)pRcvMsg->aucContent;
    ucIndex             = 0;

    /* 通过clientid获取index */
    if (AT_FAILURE == At_ClientIdToUserId(pRcvMsg->stAppCtrl.usClientId, &ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaSetMipiReadCnf: WARNING:AT INDEX NOT FOUND!");
        return VOS_ERR;
    }

    if (AT_IS_BROADCAST_CLIENT_INDEX(ucIndex))
    {
        AT_WARN_LOG("AT_RcvMtaSetMipiReadCnf: AT_BROADCAST_INDEX.");
        return VOS_ERR;
    }

    /* 当前AT是否在等待该命令返回 */
    if (AT_CMD_MIPIREAD_SET != gastAtClientTab[ucIndex].CmdCurrentOpt)
    {
        AT_WARN_LOG("AT_RcvMtaSetMipiReadCnf: NOT AT_CMD_MIPIREAD_SET.");
        return VOS_ERR;
    }

    /* 复位AT状态 */
    AT_STOP_TIMER_CMD_READY(ucIndex);

    /* 判断回复消息中的错误码 */
    if (MTA_AT_RESULT_NO_ERROR == pstSetMipiReadCnf->enResult)
    {
        /* 命令结果 *AT_OK */
        ulLength = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          (VOS_CHAR *)pgucAtSndCodeAddr,
                                          "%s:%d",
                                          g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                          pstSetMipiReadCnf->ulValue);

        gstAtSendData.usBufLen = ulLength;
        At_FormatResultData(ucIndex, AT_OK);
    }
    else
    {
        /* 命令结果 *AT_ERROR */
        gstAtSendData.usBufLen = 0;
        At_FormatResultData(ucIndex, AT_ERROR);
    }

    return VOS_OK;
}



VOS_UINT32  At_SetCltPara(VOS_UINT8 ucIndex)
{
    /* 状态检查 */
    if (AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数不符合要求 */
    if (1 != gucAtParaIndex)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /*  不是非信令模式下发返回错误 */
    if (AT_TMODE_FTM != g_stAtDevCmdCtrl.ucCurrentTMode)
    {
        return AT_DEVICE_MODE_ERROR;
    }

    g_stAtDevCmdCtrl.enCltEnableFlg = (AT_DSP_CLT_ENABLE_ENUM_UINT8) gastAtParaList[0].ulParaValue;

    return AT_OK;    /* 返回命令处理处理成功 */

}

/*****************************************************************************
 函 数 名  : AT_SetDcxotempcompPara
 功能描述  : ^DCXOTEMPCOMP设置命令处理函数
 输入参数  : VOS_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : VOS_UINT32
 调用函数  :
 被调函数  :
*****************************************************************************/
VOS_UINT32 AT_SetDcxotempcompPara(VOS_UINT8 ucIndex)
{
    /* 参数检查 */
    if (AT_CMD_OPT_SET_PARA_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }

    /* 参数个数检查 */
    if (1 != gucAtParaIndex)
    {
        return AT_CME_INCORRECT_PARAMETERS;
    }
    /*  不是非信令模式下发返回错误 */
    if (AT_TMODE_FTM != g_stAtDevCmdCtrl.ucCurrentTMode)
    {
        return AT_DEVICE_MODE_ERROR;
    }

    g_stAtDevCmdCtrl.enDcxoTempCompEnableFlg = (AT_DCXOTEMPCOMP_ENABLE_ENUM_UINT8) gastAtParaList[0].ulParaValue;

    return AT_OK;
}

/*****************************************************************************
 函 数 名  : AT_QryDcxotempcompPara
 功能描述  : ^DCXOTEMPCOMP的查询命令处理函数
 输入参数  : TAF_UINT8 ucIndex
 输出参数  : 无
 返 回 值  : TAF_UINT32
 调用函数  :
 被调函数  :
*****************************************************************************/
VOS_UINT32  AT_QryDcxotempcompPara(VOS_UINT8 ucIndex )
{

    /* 参数检查 */
    if (AT_CMD_OPT_READ_CMD != g_stATParseCmd.ucCmdOptType)
    {
        return AT_ERROR;
    }
    /*  不是非信令模式下发返回错误 */
    if (AT_TMODE_FTM != g_stAtDevCmdCtrl.ucCurrentTMode)
    {
        return AT_DEVICE_MODE_ERROR;
    }
    gstAtSendData.usBufLen = (VOS_UINT16)At_sprintf(AT_CMD_MAX_LEN,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                   (VOS_CHAR *)pgucAtSndCodeAddr,
                                                    "%s: %d",
                                                    g_stParseContext[ucIndex].pstCmdElement->pszCmdName,
                                                    g_stAtDevCmdCtrl.enDcxoTempCompEnableFlg);

    return AT_OK;
}


