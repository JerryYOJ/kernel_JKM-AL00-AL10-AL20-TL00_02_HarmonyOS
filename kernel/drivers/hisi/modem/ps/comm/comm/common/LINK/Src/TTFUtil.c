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

#include "TTFUtil.h"
#include "PsTypeDef.h"
#include "TTFComm.h"
#include "mdrv.h"
#include "securec.h"




/*****************************************************************************
    协议栈打印打点方式下的.C文件宏定义
*****************************************************************************/
/*lint -e767*/
#define    THIS_FILE_ID        PS_FILE_ID_TTF_UTIL_C
/*lint +e767*/



/******************************************************************************
   2 外部函数变量声明
******************************************************************************/

/******************************************************************************
   3 私有定义
******************************************************************************/


/******************************************************************************
   4 全局变量定义
******************************************************************************/


/******************************************************************************
   5 函数实现
******************************************************************************/
/*lint -save -e958 */

VOS_VOID TTF_QLink(VOS_UINT32 ulPid, VOS_VOID *pItem, TTF_Q_LINK_ST *pLink)
{
    if (VOS_NULL_PTR == pLink)
    {
        return;
    }

    pLink->pNext  = VOS_NULL_PTR;
} /* TTF_QLink */



VOS_VOID TTF_QInit(VOS_UINT32 ulPid, TTF_Q_ST *pQ)
{
    if (VOS_NULL_PTR == pQ)
    {
        return;
    }

    pQ->stHdr.pHead     = (TTF_Q_LINK_ST *)(&pQ->stHdr);
    pQ->stHdr.pTail     = (TTF_Q_LINK_ST *)(&pQ->stHdr);
    pQ->ulCnt           = 0;

} /* TTF_QInit */



VOS_UINT32 TTF_QIn(VOS_UINT32 ulPid, TTF_Q_ST *pQ, TTF_Q_LINK_ST *pLink)
{

    if ( (VOS_NULL_PTR == pQ) || (VOS_NULL_PTR == pLink ))
    {
        return VOS_ERR;
    }

    pLink->pNext            = (TTF_Q_LINK_ST *)&pQ->stHdr;
    pQ->stHdr.pTail->pNext  = pLink;
    pQ->stHdr.pTail         = pLink;
    pQ->ulCnt++;

    return VOS_OK;
} /*TTF_QIn*/



VOS_VOID *TTF_QOut(VOS_UINT32 ulPid, TTF_Q_ST *pQ)
{
    TTF_Q_LINK_ST  *pLink;
    TTF_Q_LINK_ST  *pRtn    = VOS_NULL_PTR;


    if (VOS_NULL_PTR == pQ)
    {
        return VOS_NULL_PTR;
    }

    pLink = pQ->stHdr.pHead;

    if ( pQ->ulCnt > 0 )
    {
        pQ->stHdr.pHead = pLink->pNext;

        if (pLink->pNext == (TTF_Q_LINK_ST *)pQ)
        {
            pQ->stHdr.pTail = (TTF_Q_LINK_ST *)(&pQ->stHdr);
        }

        pQ->ulCnt--;

        pLink->pNext = VOS_NULL_PTR;
        pRtn = pLink;
    }

    return (VOS_VOID *)pRtn;
} /* TTF_QOut */



TTF_MBUF_ST *TTF_MbufNew(VOS_UINT32 ulPid, VOS_UINT16 usLen)
{
    TTF_MBUF_ST    *pMbuf;
    VOS_UINT32      ulMbufLen;    

    /*========================*/    /*参数检查*/
    if (0 == usLen)
    {
        return VOS_NULL_PTR;
    }

    /*========================*/    /*申请内存*/
    pMbuf = (TTF_MBUF_ST *)VOS_MemAlloc(ulPid, DYNAMIC_MEM_PT,
        sizeof(TTF_MBUF_ST) + usLen);

    if (VOS_NULL_PTR == pMbuf)
    {
        return VOS_NULL_PTR;
    }

    ulMbufLen   = sizeof(TTF_MBUF_ST) + usLen;

    /*========================*/    /*设置内存单元的参数*/
    PSACORE_MEM_SET(pMbuf, ulMbufLen, 0, ulMbufLen);
    TTF_QLink(ulPid, (VOS_VOID *)pMbuf, &pMbuf->stLink);
    pMbuf->pNext    = VOS_NULL_PTR;
    pMbuf->usLen    = usLen;
    pMbuf->usUsed   = 0;
    pMbuf->pData    = (VOS_UINT8 *)(pMbuf + 1);

    return pMbuf;
} /*TTF_MbufNew*/



void TTF_MbufFree(VOS_UINT32 ulPid, TTF_MBUF_ST *pMbuf)
{
    TTF_MBUF_ST    *pNext;
    TTF_MBUF_ST    *pFree   = pMbuf;


    while(VOS_NULL_PTR != pFree)
    {
        pNext = pFree->pNext;
        (VOS_VOID)VOS_MemFree(ulPid, pFree);
        pFree = pNext;
    }
} /*TTF_MbufFree*/



VOS_UINT16 TTF_MbufGetLen(VOS_UINT32 ulPid, TTF_MBUF_ST *pMbuf)
{
    VOS_UINT16      usLen   = 0;
    TTF_MBUF_ST    *pTmp    = pMbuf;


    while(VOS_NULL_PTR != pTmp)
    {
        usLen   += pTmp->usUsed;
        pTmp    = pTmp->pNext;
    }

    return usLen;
} /*TTF_MbufGetLen*/


/******************************************************************************
 Function:       TTF_LenStr2IpAddr
 Description:    将字符串格式的IP地址转化为SM协议IP地址格式.完全继承V100R001
 Calls:
 Data Accessed:
 Data Updated:
 Input:          pucStr - 字符串格式的IP地址
 Output:         pucIpAddr - SM协议定义的IP地址
 Return:         TAF_SUCCESS - 转化成功
                 TAF_FAILURE - 转化失败
 Others:
******************************************************************************/
VOS_UINT32  TTF_LenStr2IpAddr(VOS_UINT8* pucStr, VOS_UINT8 *pucIpAddr)
{
    VOS_UINT8 i, j = 0;
    VOS_UINT8 ucValue = 0;

    if (0 == pucStr[0])
    {   /*ADDR长度为0，直接长度赋值返回*/
        pucIpAddr[0] = 0;
        pucIpAddr[1] = 0;
        pucIpAddr[2] = 0;
        pucIpAddr[3] = 0;
        return PS_SUCC;
    }

    if (pucStr[0] == '.')
    {   /*如果第1个有效字符是'.'，IP地址是非法的*/
        return PS_FAIL;
    }


    for (i = 0; (i <= TTF_MAX_IPV4_ADDR_LEN) && (0 != pucStr[i]) ; i++)
    {   /*从第1个有效字符开始检查*/
        if (((pucStr[i] < 0x30) || (pucStr[i] > 0x39)) && (pucStr[i] != '.'))
        {   /*超出'0'-'9'的字符非法*/
            return PS_FAIL;
        }
        if (pucStr[i] != '.')
        {   /*如果是有效字符，转化为数字*/
            if (((ucValue * 10) + (pucStr[i] - 0x30)) <= 255)
            {   /*字符串转化为有效IP段位值*/
                ucValue = (VOS_UINT8)((ucValue * 10) + (pucStr[i] - 0x30));
            }
            else
            {   /*超过255出错*/
                return PS_FAIL;
            }
        }
        else
        {   /*如果字符是'.'，前一位段值已经计算出来*/
            if (j <= 3)
            {   /*本版本只支持IPV4地址*/
                pucIpAddr[j] = ucValue;
                ucValue = 0;
                j++;  /*开始下一个有效字符段的长度累计*/
            }
            else
            {   /*超出4个IP位段，非法*/
                return PS_FAIL;
            }
        }
    }

    if (j == 3)
    {
        pucIpAddr[j] = ucValue;
        return PS_SUCC;
    }
    else
    {
        return PS_FAIL;
    }
}

/*****************************************************************************
 Function   : TTF_SetByBit
 Description: set value from startBit to endBit
 Input      : ulOrgValue -- original value to set
            : ucStartBit -- startBit in 32bit,lower edge
            : ucEndBit  --- endBit in 32bit,upper edge
            : ulSetValue -- the value to set
 Return     : void
 Other      :
 *****************************************************************************/
VOS_UINT32 TTF_SetByBit(VOS_UINT32 ulPid, VOS_UINT32 ulOrgValue, VOS_UINT8 ucStartBit, VOS_UINT8 ucEndBit,VOS_UINT32 ulSetValue)
{
    VOS_UINT32                          ulOrgMask;
    VOS_UINT32                          ulDataMask;


    /*check parameter*/

    if ((ucStartBit > ucEndBit) || (ucStartBit > TTF_REG_MAX_BIT) || (ucEndBit > TTF_REG_MAX_BIT) )
    {
        TTF_LOG4(ulPid, DIAG_MODE_COMM, PS_PRINT_WARNING, "TTF_SetByBit para err ,ulOrgValue 0x%x, startBit %d, endBit %d, value %d\r\n",
            (VOS_INT32)ulOrgValue, ucStartBit, ucEndBit, (VOS_INT32)ulSetValue);
        return ulOrgValue;
    }

    /*to get the mask form startBit to endbit*/
    ulDataMask  = 0xFFFFFFFF;
    ulDataMask  = ulDataMask >> (ucStartBit);
    ulDataMask  = ulDataMask << (TTF_REG_MAX_BIT - (ucEndBit - ucStartBit));
    ulDataMask  = ulDataMask >> (TTF_REG_MAX_BIT - ucEndBit);
    ulOrgMask   = ~ulDataMask;

    /*set 0 from startBit to endBit*/
    ulOrgValue  &= ulOrgMask;

    /*move setValue to position*/
    ulSetValue  = ulSetValue << ucStartBit;
    ulSetValue &= ulDataMask;

    /*set value to reg*/
    ulSetValue |= ulOrgValue;

    return ulSetValue;
}


/*****************************************************************************
 Function   : TTF_GetByBit
 Description: read value  from startBit to endBit
 Input      : ulOrgValue -- original value to get
            : ucStartBit -- startBit in 32bit,lower edge
            : ucEndBit  --- endBit in 32bit,upper edge
 Return     : value
 Other      :
 *****************************************************************************/
MODULE_EXPORTED VOS_UINT32 TTF_GetByBit(VOS_UINT32 ulPid, VOS_UINT32 ulOrgValue, VOS_UINT8 ucStartBit, VOS_UINT8 ucEndBit)
{
    VOS_UINT32                          ulOrgMask;


    /*check parameter*/

    if ((ucStartBit > ucEndBit) || (ucStartBit > TTF_REG_MAX_BIT) || (ucEndBit > TTF_REG_MAX_BIT) )
    {
        TTF_LOG3(ulPid, DIAG_MODE_COMM, PS_PRINT_WARNING, "TTF_GetByBit para err ,ulOrgValue 0x%x, startBit %d, endBit %d\r\n",
            (VOS_INT32)ulOrgValue, ucStartBit, ucEndBit);
        return 0;
    }

    /*to get the mask form startBit to endbit*/
    ulOrgMask  = 0xFFFFFFFF;
    ulOrgMask  = ulOrgMask >> (ucStartBit);
    ulOrgMask  = ulOrgMask << (TTF_REG_MAX_BIT - (ucEndBit - ucStartBit));
    ulOrgMask  = ulOrgMask >> (TTF_REG_MAX_BIT - ucEndBit);

    /*get value from startBit to endBit*/
    ulOrgValue  &= ulOrgMask;

    return (ulOrgValue >> ucStartBit);
}



MODULE_EXPORTED VOS_VOID TTF_InsertSortAsc16bit
(
    VOS_UINT32                          ulPid,
    VOS_UINT16                          ausSortElement[],
    VOS_UINT32                          ulElementCnt,
    VOS_UINT32                          ulMaxCnt
)
{
    VOS_UINT16                          usTemp;
    VOS_UINT32                          ulElementCntLoop;
    VOS_INT32                           j;


    if ( 0 == ulElementCnt )
    {
        return;
    }

    if ( ulElementCnt > ulMaxCnt  )
    {
        TTF_LOG2(ulPid, 0, PS_PRINT_NORMAL,
            "TTF_InsertSortAsc16bit::ulElementCnt is more than MaxCnt!<1>ulElementCnt,<2>ulMaxCnt",
            (VOS_INT32)ulElementCnt, (VOS_INT32)ulMaxCnt);
        ulElementCnt    = ulMaxCnt;
    }

    for ( ulElementCntLoop = 1; ulElementCntLoop < ulElementCnt; ulElementCntLoop++ )
    {
        if( ausSortElement[ulElementCntLoop] < ausSortElement[ulElementCntLoop-1] )/* 后一个数 < 前一个数 */
        {
            usTemp  = ausSortElement[ulElementCntLoop];
            j       = (VOS_INT32)(ulElementCntLoop - 1);

            do
            {
                ausSortElement[(VOS_INT32)(j+1)]= ausSortElement[j];
                j--;
                if (j < 0)
                {
                    break;
                }
            }while (usTemp < ausSortElement[j]);

            ausSortElement[(VOS_INT32)(j+1)]  = usTemp;
        }
    }

}




MODULE_EXPORTED VOS_VOID TTF_RemoveDupElement16bit
(
    VOS_UINT32                          ulPid,
    VOS_UINT16                          ausSortElement[],
    VOS_UINT32                         *pulElementCnt,
    VOS_UINT32                          ulMaxCnt
)
{
    VOS_UINT32                          ulElementCntLoop;
    VOS_UINT32                          ulFilterAfterCnt;
    VOS_UINT32                          ulDupCnt = 0;
    VOS_UINT32                          j;


    if ( 0 == *pulElementCnt )
    {
        return;
    }

    if ( *pulElementCnt > ulMaxCnt  )
    {
        TTF_LOG2(ulPid, 0, PS_PRINT_NORMAL,
            "TTF_InsertSortAsc16bit::ulElementCnt is more than MaxCnt!<1>ulElementCnt,<2>ulMaxCnt",
            (VOS_INT32)(*pulElementCnt), (VOS_INT32)ulMaxCnt);
        *pulElementCnt  = ulMaxCnt;
    }

    ulFilterAfterCnt    = *pulElementCnt;

    for ( ulElementCntLoop = 1; ulElementCntLoop < ulFilterAfterCnt; ulElementCntLoop++ )
    {
        if ( ausSortElement[ulElementCntLoop] == ausSortElement[ulElementCntLoop-1] )
        {
            for ( j= ulElementCntLoop; j< ulFilterAfterCnt; j++ )
            {
                ausSortElement[j-1] = ausSortElement[j];
            }
            ulElementCntLoop--;
            ulFilterAfterCnt--;
            ulDupCnt++;
        }
    }

    *pulElementCnt  = *pulElementCnt - ulDupCnt;

    return;
}



MODULE_EXPORTED VOS_VOID TTF_RingBufWrite(VOS_UINT32 ulPid, VOS_UINT32 ulDstRingBufBaseAddr, VOS_UINT16 usOffset,
    VOS_UINT8 *pucSrcData, VOS_UINT16 usDataLen, VOS_UINT16 usModLen)
{
    VOS_UINT16  usBufLeftLen;
    VOS_UINT8  *pucDst;

    if (usDataLen >= usModLen)
    {
        TTF_LOG2(ulPid, PS_SUBMOD_NULL, PS_PRINT_WARNING,
            "TTF_RingBufWrite, ulDataLen <1> wrong with ulModLen <2>",
            usDataLen, usModLen);
        return;
    }

    if (usOffset >= usModLen)
    {
        TTF_LOG2(ulPid, PS_SUBMOD_NULL, PS_PRINT_WARNING,
            "TTF_RingBufWrite, usOffset <1> wrong with ulModLen <2>",
            usOffset, usModLen);
        return;
    }

    usBufLeftLen    = usModLen - usOffset;
    pucDst          = (VOS_UINT8 *)((VOS_UINT_PTR)(ulDstRingBufBaseAddr + usOffset));

    if (usBufLeftLen >= usDataLen)
    {
        (VOS_VOID)VOS_MemCpy_s(pucDst, usBufLeftLen, pucSrcData, usDataLen);
    }
    else
    {
        (VOS_VOID)VOS_MemCpy_s(pucDst, usBufLeftLen, pucSrcData, usBufLeftLen);
        (VOS_VOID)VOS_MemCpy_s((VOS_UINT8 *)(VOS_UINT_PTR)ulDstRingBufBaseAddr, usModLen,
            (VOS_UINT8 *)(VOS_UINT_PTR)pucSrcData + usBufLeftLen, usDataLen - usBufLeftLen);
    }

    return;
} /* TTF_RingBufWrite */


VOS_VOID TTF_RingBufRead
(
    VOS_UINT32                          ulPid,
    VOS_UINT32                          ulSrcRingBufBaseAddr,
    VOS_UINT32                          usOffset,
    VOS_UINT8                          *pucDstData,
    VOS_UINT16                          usDataLen,
    VOS_UINT32                          usModLen
)
{
    VOS_UINT32                           usLeft;
    VOS_UINT8                           *pucSrc;

    if (usDataLen >= usModLen)
    {
        TTF_LOG(ulPid, PS_SUBMOD_NULL, PS_PRINT_WARNING,
            "TTF_RingBufRead, ulDataLen wrong with ulModLen ");
        return;
    }

    if (usOffset >= usModLen)
    {
        TTF_LOG(ulPid, PS_SUBMOD_NULL, PS_PRINT_WARNING,
            "TTF_RingBufRead, usOffset wrong with ulModLen ");

        return;
    }

    usLeft = usModLen - usOffset;
    pucSrc = (VOS_UINT8 *)(VOS_UINT_PTR)(ulSrcRingBufBaseAddr + usOffset);

    if (usDataLen > usLeft)
    {

        (VOS_VOID)VOS_MemCpy_s( pucDstData, usDataLen, pucSrc, usLeft);
        (VOS_VOID)VOS_MemCpy_s((VOS_UINT8 *)( pucDstData + usLeft),
                      (usDataLen - usLeft),
                      (VOS_UINT8 *)(VOS_UINT_PTR)ulSrcRingBufBaseAddr,
                      (usDataLen - usLeft));
    }
    else
    {
        (VOS_VOID)VOS_MemCpy_s(pucDstData, usDataLen, pucSrc, usDataLen);
    }

    return;
} /* TTF_RingBufRead */


/*lint -e429*/
VOS_VOID PSACORE_MEM_SET_EX(VOS_VOID *ToSet, VOS_SIZE_T ulDestSize, VOS_CHAR Char, VOS_SIZE_T Count,
                                        VOS_UINT32 ulFileNo, VOS_UINT32 ulLineNo)
{
    VOS_VOID                                *pRslt  = VOS_NULL_PTR;
    pRslt = V_MemSet_s(ToSet, ulDestSize, Char, Count, ulFileNo, (VOS_INT32)ulLineNo);
    if (VOS_NULL_PTR == pRslt)
    {
    }
}
/*lint +e429*/


MODULE_EXPORTED VOS_VOID PSACORE_MEM_CPY_EX(VOS_VOID *Dest, VOS_SIZE_T ulDestSize, const VOS_VOID *Src, VOS_SIZE_T Count,
                                        VOS_UINT32 ulFileNo, VOS_UINT32 ulLineNo)
{
    VOS_VOID                                *pRslt          = VOS_NULL_PTR;
    const VOS_VOID                          *pDestChk       = Dest;
    const VOS_VOID                          *pSrcChk        = Src;

    if (0 == Count)
    {
        return;
    }


    /*  情况1: 从内存高地址向低地址拷贝，并且是从列表的头部开始拷贝，这种拷贝是允许的。所以，针对这种拷贝直接使用MemMove，这
               样做可以兼容现有代码，目前代码中存在这种内存拷贝情况，且是正常现象，没有必要复位，如果一一排查都改成MemMove，
               难免有遗漏。

        情况2: 从内存高地址向低地址拷贝，并且是从列表的尾部开始拷贝，这种拷贝是不允许的，会导致内存数据异常。因为MemCpy函数
               拷贝的实现都是从列表头部开始拷贝的，所以目前代码中这种情况不会出现。*/
    if (( pSrcChk > pDestChk ) && ( (VOS_VOID *)((VOS_UINT8 *)pDestChk + Count) > pSrcChk ))
    {
        /*lint -e668 -e613*/
        pRslt = V_MemMove_s( Dest, ulDestSize, Src, Count, ulFileNo, (VOS_INT32)ulLineNo );
        /*lint +e668 +e613*/
        if (VOS_NULL_PTR == pRslt)
        {
        }

        return;
    }

    /*  情况3: 从内存低地址向高地址拷贝，并且是从列表的头部开始拷贝，这种拷贝是不允许的，会导致内存数据异常。这种情况返回
               空指针，直接复位单板，暴露现有代码的问题。

        情况4: 从内存低地址向高地址拷贝，并且是从列表的尾部开始拷贝，这种拷贝是允许的，不会导致数据异常。因为MemCpy函数
               拷贝的实现都是从列表头部开始拷贝的，所以目前代码中这种情况不会出现。*/
    /* 工程组自定义了安全函数的检查规则，-sem(V_MemCpy_s,1p>=4n,1p,3p),
       要求第一个参数和第三个参数不为空指针，函数V_MemCpy_s实现中已经做了空指针检查，因此这里屏蔽处理 */
    /*lint -e{668, 613}*/
    if ((Count > ulDestSize) || (VOS_OK != memcpy_s( Dest, ulDestSize, Src, Count))) /* [false alarm]: Count大于ulDestSize时if判断会短路,不会执行memcpy_s*/
    {
    }
}


VOS_VOID PSACORE_MEM_MOVE_EX(VOS_VOID *Dest, VOS_SIZE_T ulDestSize, const VOS_VOID *Src, VOS_SIZE_T Count,
                                        VOS_UINT32 ulFileNo, VOS_UINT32 ulLineNo)
{
    VOS_VOID                                *pRslt  = VOS_NULL_PTR;

    if (0 == Count)
    {
        return;
    }
    
    /*lint -e668 -e613*/
    pRslt = V_MemMove_s( Dest, ulDestSize, Src, Count, ulFileNo, (VOS_INT32)ulLineNo );
    /*lint +e668 +e613*/
    if (VOS_NULL_PTR == pRslt)
    {
    }
}
/*lint -restore */

/*
 * TTF 安全函数返回值校验，校验失败会触发复位。
 * 支持函数： memset_s memcpy_s memmove_s
 */
MODULE_EXPORTED VOS_VOID TtfSfChk(VOS_BOOL result, VOS_UINT32 fileNo, VOS_UINT32 lineNo)
{    
    if (result) {
        mdrv_om_system_error((VOS_INT32)TTF_MEM_CPY_FAIL_ERROR, (VOS_INT32)fileNo, (VOS_INT32)lineNo, VOS_NULL_PTR, 0);
    }
}

/*
 * TTF Acore安全函数返回值校验，校验失败会打印错误，不能复位。
 * 支持函数： memset_s memcpy_s memmove_s
 */
MODULE_EXPORTED VOS_VOID TtfAcoreSfChk(VOS_BOOL result, VOS_UINT32 fileNo, VOS_UINT32 lineNo)
{    
    if (result) {
        TTF_LOG3(VOS_CPU_ID_ACPU, 0, PS_PRINT_ERROR, "SafeFunc return value check fail!", result, fileNo, lineNo);
    }
}



