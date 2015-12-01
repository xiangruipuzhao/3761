/******************************************************************************

                  版权所有 (C), 2001-2011, 国网电科院通信与用电技术分公司
                  
 ******************************************************************************
  文 件 名   : main.c
  版 本 号   : 初稿
  作    者   : liming
  生成日期   : 2013年4月17日 星期三
  最近修改   :
  功能描述   : 3761和协议适配层测试代码
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月17日 星期三
    作    者   : liming
    修改内容   : 创建文件

******************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <stdarg.h>
#include "con3761adp.h"
#include "fmprint.h"
#include <math.h>


#define PACK_MEM_SIZE  10240
#define OUT_BUF_LEN    10240

#define BUF_LEN_MAX   (1024*1024*50)
UINT8 g_ucPackMem[PACK_MEM_SIZE] = {0}; // 测试封装命令的参数的内存空间
UINT8 g_ucOutBuf[OUT_BUF_LEN]    = {0}; // 存放输出报文
UINT8 g_ucInBuf[OUT_BUF_LEN]     = {0};
UINT8 g_ucOutBufColor[OUT_BUF_LEN]    = {0}; // 存放输出报文

smtPack * getSmtPackPoint()
{
    memset((void*)g_ucPackMem, 0, PACK_MEM_SIZE);
    return (smtPack *)g_ucPackMem;
}

void show_main_name(char *str)
{
      fm_print(str, FR_GREEN|FM_UNDERLINE);
}

void show_sub_name(char * str)
{
    fm_print(str,FR_YELLOW|BK_NORMAL);
}

void show_item_value(char *str)
{
    fm_print(str,FR_WHITE|BK_MAGENTA);
}

void show_error(char *str)
{
     fm_print(str,FR_RED|FM_BOLD|FM_FLASH);
}

void printf_buffer_color(char * pBuf, UINT16 usBufLen)
{

    memset(g_ucOutBufColor, 0,PACK_MEM_SIZE);
    
    if(!pBuf)
    {
       show_error("error print_buffer pointer null!\n");
        return;
    }

    int Len = 0;
    int i = 0;
    Len = (int)usBufLen;
    
    fm_print("pack length is: ", FR_YELLOW|FM_BOLD);
    char buf[10] = {0};
    sprintf(buf,"%d\n\n", Len);
    fm_print(buf,FR_WHITE|BK_MAGENTA|FM_BOLD);
  
    if(Len != 0)
    {       
        for(i = 0; i < Len; i++)
        {   
            memset(buf, 0,10);
            sprintf(buf,"%02X ", (UINT8)pBuf[i]);
            fm_print(buf,FR_WHITE|BK_MAGENTA|FM_BOLD);
        }

        printf("\n\n\n");
    }

}

/*****************************************************************************
 函 数 名  : print_buffer
 功能描述  : 打印报文
 输入参数  : UINT8 * pBuf      
             UINT16* usBufLen  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
void print_buffer(UINT8 * pBuf, UINT16* pusBufLen)
{

    if(!pBuf || !pusBufLen)
    {
        printf("error print_buffer pointer null!");
        return;
    }

    int Len = 0;
    int i = 0;
    Len = (int)*pusBufLen;

    printf("pack length is %d\n", Len);
    if(Len != 0)
    {       
        for(i = 0; i < Len; i++)
        {
            printf("%02X ", (UINT8)pBuf[i]);
        }

        printf("\n");
    }

}



void show_mt_error(eMtErr eErr)
{
    char *str = NULL;
    switch(eErr)
    {
        case MT_OK:
        str = "MT_OK 无错误";
        break;

        case MT_ERR_NULL:
        str = "MT_NULL 指针为空";
        break;
        
        case MT_ERR_OUTRNG:
        str = "MT_OUT_RNG 参数越界";
        break;

        case MT_ERR_NONE:
        str = "MT_NONE 不存在,没有找到";
        break;

        case MT_ERR_IO:
        str = "MT_ERR_IO IO错误";
        break;

        case MT_ERR_RES:
        str = "MT_ERR_RES 资源不足";
        break;

        case MT_ERR_INIT:
        str = "MT_ERR_INIT 没有初始化";
        break;
        
        case MT_ERR_PARA:
        str = "MT_ERR_PARA 参数错误";
        break;

        case MT_ERR_FN:
        str = "MT_ERR_FN 指定Afn 中不支持的 Fn";
        break;

        case MT_ERR_PN:
        str = "MT_ERR_PN 指定Afn Fn 中不支持的 Pn";
        break;

        case MT_ERR_ADDR:
        str = "MT_ERR_ADDR 无效地址";
        break;
        
        case MT_ERR_CTRL:
        str = "MT_ERR_CTRL 控制域错误";
        break;

        case MT_ERR_SEQ:
        str = "MT_ERR_SEQ SEQ域错误";
        break;
        
        case MT_ERR_SEQ2CS:
        str = "MT_ERR_SEQ2CS";
        break;

        case MT_ERR_AFN:
        str = "MT_ERR_AFN AFN 错误";
        break;

        case MT_ERR_UNCOMP:
        str = "MT_ERR_UNCOMP 不完整的协议包";
        break;

        case MT_ERR_0x68:
        str = "MT_ERR_0x68 错误的报文起始";
        break;
        
        case MT_ERR_PROTO:
        str = "MT_ERR_PROTO 错误的协议标识";
        break;
        
        case MT_ERR_CS:
        str = "MT_ERR_CS 错误的检测和";
        break;

        case MT_ERR_0x16:
        str = "MT_ERR_0x16 错误的结尾符";
        break;

        case MT_ERR_LOGIC:
        str = "MT_ERR_LOGIC 错误的处理逻辑";
        break;
        
        case MT_ERR_PACK:
        str = "MT_ERR_PACK 不是一个有效的包";
        break;
        case MT_ERR_PFC:
            str = "序列号不对应";
            break;
        case MT_ERR_TIMEOUT:
            str = "报文超时";
            break;
        case MT_ERR_PRM:
            str = "启动方向错误";
            break;

       case MT_ERR_TEAM:
            str = "不属于同一组";
            break;
        case MT_ERR_ENCRYPT:
            str = "加密错误";
            break;
        case MT_ERR_DECRYPT:
            str = "解密错误";
            break;
        default:
        str = "未知错误";
        break;
        
    }


    show_error(str);
    printf("\n");
}



/*****************************************************************************
 函 数 名  : test_pack_1
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0001()
{

    return MT_OK;
}


eMtErr test_pack_0001_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_00_CONF;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_0_F1_ALL_OK;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

#if 0

/*****************************************************************************
 函 数 名  : test_pack_1
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0001()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;
    sProtoInit.ucPermitDelayMinutes = 0;
    memcpy(sProtoInit.aucPw,"1234567890abcde",16);
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 123456;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code
    psPack->eMainCmd = AFN_00_CONF;

    psPack->sCtrl.eRPM = MT_PRM_PASIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir, psPack->sCtrl.eRPM, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_0_FN_1_ALL_OK;
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;

    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

 return MT_OK;
}



/*****************************************************************************
 函 数 名  : test_pack_0002
 功能描述  : 测试例0002 全部否认
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0002()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code
    psPack->eMainCmd = AFN_00_CONF;
    psPack->sCtrl.eRPM = MT_PRM_PASIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir, MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_0_FN_2_ALL_DENY;
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;

    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
 
}










/*****************************************************************************
 函 数 名  : test_pack_0101
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0101()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_01_RSET;                              // (2)这里需要修改
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_1_FN_1_HARD_INIT;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // app_data                                                  // (4)这里需要修改
    #if 0   // 无应用数据
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    #endif




    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);
    return MT_OK;
 
}













/*****************************************************************************
 函 数 名  : test_pack_0101
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0102()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_01_RSET;                              // (2)这里需要修改
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,psPack->sCtrl.eRPM, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_1_FN_2_DATA_INIT;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // app_data                                                  // (4)这里需要修改
    #if 0   // 无应用数据
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    #endif






    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);

     return MT_OK;
}



/*****************************************************************************
 函 数 名  : test_pack_0101
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0103()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_01_RSET;                              // (2)这里需要修改
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_PASIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_1_FN_3_FACTORY_RESET;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // app_data                                                  // (4)这里需要修改
    #if 0   // 无应用数据
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    #endif






    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);

     return MT_OK;
}


/*****************************************************************************
 函 数 名  : test_pack_0101
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0104()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_01_RSET;                              // (2)这里需要修改
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_PASIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_1_FN_1_HARD_INIT;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    
    // app_data                                                  // (4)这里需要修改
    #if 0
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    #endif




    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);
    return MT_OK;
 
}




/*****************************************************************************
 函 数 名  : test_pack_0201
 功能描述  : 测试例
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0201()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_02_LINK;                              // (2)这里需要修改
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,psPack->sCtrl.eRPM, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_2_FN_1_LOG_IN;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    
    // app_data                                                  // (4)这里需要修改
    #if 0
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    #endif




    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);

    return MT_OK;
}





/*****************************************************************************
 函 数 名  : test_pack_0202
 功能描述  : 测试例
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0202()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_02_LINK;                              // (2)这里需要修改
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,psPack->sCtrl.eRPM, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_2_FN_2_LOG_OUT;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    
    // app_data                                                  // (4)这里需要修改
    #if 0
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    #endif




    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);

    return MT_OK;
}




/*****************************************************************************
 函 数 名  : test_pack_0203
 功能描述  : 测试例
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0203()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_02_LINK;                              // (2)这里需要修改
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    // ctrl_filed
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,psPack->sCtrl.eRPM, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_2_FN_3_HEART_BEAT;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    
    // app_data                                                  // (4)这里需要修改
    #if 0
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    #endif




    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);

    return MT_OK;
}

/*****************************************************************************
 函 数 名  : test_pack_1
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0401()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_04_SETP;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_4_FN_1_CON_UP_CFG;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // app_data                                                  // (4)这里需要修改
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTS = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucRTM = 0x23;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucReSendTimes = 0x34;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.ucHeartBeat = 0x12;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.usTmWaitTimeoutS = 0x1234;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_1 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_2 = FALSE;
    psPack->sSub[0].uAppData.sAfn04F1_ConUpPortPara.bAutoReportAsk_3 = FALSE;
    




    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}




/*****************************************************************************
 函 数 名  : test_pack_1
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0402()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_04_SETP;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_4_FN_2_CON_UP_WAY_ETH;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // app_data                                                  // (4)这里需要修改
    int Num = 16;
    int i = 0;
    
    psPack->sSub[0].uAppData.sAfn04F2_ConUpPortParaWireless.bPermit = TRUE;
    psPack->sSub[0].uAppData.sAfn04F2_ConUpPortParaWireless.ucAddrCount = Num;
 
    for(i = 0; i < Num; i++)
    { 
       psPack->sSub[0].uAppData.sAfn04F2_ConUpPortParaWireless.usAddress[i] = i;
    }

  


    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}







/*****************************************************************************
 函 数 名  : test_pack_0403
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0403()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_04_SETP;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_4_FN_3_MST_IP_PORT;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // app_data                                                  // (4)这里需要修改

    
    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.ulMainIP   = 0x12345678;
    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.usMainPort = 0x1234;

    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.ulBackIP   = 0x987654321;
    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.usBackPort = 0x4321;
    memcpy((void*)(psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.ucascAPN), "1234567890abcdef",16);


    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}



/*****************************************************************************
 函 数 名  : test_pack_0403
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0404()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_04_SETP;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_4_FN_3_MST_IP_PORT;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // app_data                                                  // (4)这里需要修改

    
    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.ulMainIP   = 0x12345678;
    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.usMainPort = 0x1234;

    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.ulBackIP   = 0x987654321;
    psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.usBackPort = 0x4321;
    memcpy((void*)(psPack->sSub[0].uAppData.sAfn04F3_MasterIpPort.ucascAPN), "1234567890abcdef",16);


    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}





/*****************************************************************************
 函 数 名  : test_pack_0501
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0501()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    memcpy(sProtoInit.aucPw,"1234567890abcde",16);
      
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_05_CTRL;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_5_FN_1_REMOTE_SWITCH_OFF;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  1024;
    psPack->sSub[0].usLen     =  0;


    // pw
    memcpy(psPack->aucPW, "1234567890abcde", 15);
    
    // app_data                                                  // (4)这里需要修改
    psPack->sSub[0].uAppData.sAfn05F1_RemoteCtrlTrip.ucLimitPowerTime = 15;
    psPack->sSub[0].uAppData.sAfn05F1_RemoteCtrlTrip.ucHoldAlarmTime  = 15; 

    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}




/*****************************************************************************
 函 数 名  : test_pack_0601
 功能描述  : 测试例
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0601()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_MASTER;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    memcpy(sProtoInit.aucPw,"1234567890abcde",16);
      
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_06_AUTH;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_6_FN_1_ID_AUTH_Q;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // pw
    //memcpy(psPack->aucPW, "1234567890abcde", 15);

    char *strCiphertext = "abcdef1234567890";
    // app_data                                                  // (4)这里需要修改
    memcpy(psPack->sSub[0].uAppData.sAfn06F1_IdAuthQ.aCiphertext, strCiphertext, 16);


    
    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}





/*****************************************************************************
 函 数 名  : test_pack_0602
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0602()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    memcpy(sProtoInit.aucPw,"1234567890abcde",16);
      
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_06_AUTH;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_PASIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_PASIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_6_FN_2_ID_AUTH_A;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // pw
    //memcpy(psPack->aucPW, "1234567890abcde", 15);

    char *strCiphertext = "abcdef1234567890";
    // app_data                                                  // (4)这里需要修改
    memcpy(psPack->sSub[0].uAppData.sAfn06F2_IdAuthA.aCiphertext, strCiphertext, 16);


    
    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}





/*****************************************************************************
 函 数 名  : test_pack_0603
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0603()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    memcpy(sProtoInit.aucPw,"1234567890abcde",16);
      
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_06_AUTH;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_ACTIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_ACTIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_6_FN_3_RAND_Q;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // pw
    //memcpy(psPack->aucPW, "1234567890abcde", 15);

    char *strCiphertext = "abcdef1234567890";
    // app_data                                                  // (4)这里需要修改
    memcpy(psPack->sSub[0].uAppData.sAfn06F2_IdAuthA.aCiphertext, strCiphertext, 16);


    
    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}






/*****************************************************************************
 函 数 名  : test_pack_0604
 功能描述  : 测试例1
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0604()
{

    /* 1 定义变量 */ 
    eMtErr eRet = MT_OK;
    smtPack *psPack = NULL;
    sMtProtoInit sProtoInit = {0};
    eMtDir eDir = MT_DIR_UNKOWN;
    UINT16 usBuflen = 0;
    psPack = getSmtPackPoint();


    /* 2 环境初始化 */
    sProtoInit.eRole = MT_ROLE_CONTOR;                          // (1)这里需要修改
    sProtoInit.ucPermitDelayMinutes = 0;
    memcpy(sProtoInit.aucPw,"1234567890abcde",16);
      
    eRet = eMtProtoInit(&sProtoInit);
    if(eRet != MT_OK)
    {
        printf("eMtProtoInit failed code = %d\n", eRet);
        return eRet;
    }

    // 确定报文的传送方向
    if(sProtoInit.eRole == MT_ROLE_CONTOR)
    {
        eDir = MT_DIR_S2M;
    }
    else if(sProtoInit.eRole == MT_ROLE_MASTER)
    {
        eDir = MT_DIR_M2S;
    }

    
    /* 3 封装参数 */

    // address
    UINT8 strRegionCode[5] = "1100"; 
    memcpy((void*)psPack->sAddress.acRegionCode, (void*)strRegionCode, strlen(strRegionCode));
    psPack->sAddress.usTAddress = 0x12345678;
    psPack->sAddress.ucMAddress = 123;
    psPack->sAddress.bTeamAddr = FALSE;

    // main_code 
    psPack->eMainCmd = AFN_06_AUTH;                              // (2)这里需要修改

    
    // ctrl_filed
    psPack->sCtrl.eRPM = MT_PRM_PASIVE;
    eRet = eMtGetCtrl(psPack->eMainCmd, eDir,MT_PRM_PASIVE, &(psPack->sCtrl));
    

    // subCode
    psPack->ucSubNum = 1;
    psPack->sSub[0].eSubCmd   =  CMD_AFN_6_FN_4_RAND_A;      // (3)这里需要修改
    psPack->sSub[0].usPN      =  0;
    psPack->sSub[0].usLen     =  0;


    // pw
    //memcpy(psPack->aucPW, "1234567890abcde", 15);

    char *strCiphertext = "abcdef1234567890";
    // app_data                                                  // (4)这里需要修改
    memcpy(psPack->sSub[0].uAppData.sAfn06F2_IdAuthA.aCiphertext, strCiphertext, 16);


    
    /* 4 调用函数 */ 
    eRet = emtPack(psPack, &usBuflen, (UINT8 *)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("emtPack error code %d\n", eRet);
        return eRet;
    }

    /* 5 输出结果 */ 
    printf_buffer_color(g_ucOutBuf, usBuflen);


    return MT_OK;
}


#endif


/*****************************************************************************
 函 数 名  : test_pack_0433
 功能描述  : 测试设置非电气量封装函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月30日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_0421()
{

#if 0
    /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 12345678;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CFG_NOT_ELEC;
    pscmPacket->sCmdData[0].usLen = 0;
    pscmPacket->sCmdData[0].usPN  = 123;
    
    pscmPacket->sCmdData[0].uAppData.sCfgNotElec.eType = MT_NELEC_TEMP;
    pscmPacket->sCmdData[0].uAppData.sCfgNotElec.dMin  = -99999.99;
    pscmPacket->sCmdData[0].uAppData.sCfgNotElec.dMax  = 99999.99;

    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return;
    }
    


    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

#endif
     return MT_OK;
}


/*****************************************************************************
 函 数 名  : test_pack_afn05_f90
 功能描述  : 测试设置非电气量封装函数
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月30日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn05_f90()
{

#if 0

    /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 12345678;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_NOT_ELEC_VALUE;
    pscmPacket->sCmdData[0].usLen = 0;
    pscmPacket->sCmdData[0].usPN  = 123;
    
    pscmPacket->sCmdData[0].uAppData.sNotElecVale.eType = MT_NELEC_TEMP;
    pscmPacket->sCmdData[0].uAppData.sNotElecVale.dValue= 123.456;


    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    
    #endif
    return MT_OK;
}


/*****************************************************************************
 函 数 名  : test_pack_afn00_f3
 功能描述  : 测试逐个确认和否认
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月9日
    作    者   : 李明
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn00_f3()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }

    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_ONE_BY_ONE;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;
    
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.ucNum = 1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[0].eCmd = CMD_CFG_TML_UP;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[0].usPN = 0;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[0].bOk  = TRUE;


    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[1].eCmd = CMD_CFG_TML_POWER;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[1].usPN = 0;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[1].bOk  = TRUE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[2].eCmd = CMD_CFG_ASK_1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[2].usPN = 0;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[2].bOk  = TRUE;


    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[3].eCmd = CMD_CFG_ASK_2;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[3].usPN = 0;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[3].bOk  = FALSE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[4].eCmd = CMD_CFG_AUTO_1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[4].usPN = 1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[4].bOk  = TRUE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[5].eCmd = CMD_CFG_AUTO_2;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[5].usPN = 2;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[5].bOk  = FALSE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[6].eCmd = CMD_CFG_AUTO_1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[6].usPN = 2;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[6].bOk  = TRUE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[7].eCmd = CMD_CFG_AUTO_2;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[7].usPN = 1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[7].bOk  = FALSE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[8].eCmd = CMD_CFG_AUTO_1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[8].usPN = 5;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[8].bOk  = FALSE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[9].eCmd = CMD_CFG_AUTO_2;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[9].usPN = 6;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[9].bOk  = TRUE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[10].eCmd = CMD_GO_AUTO_1;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[10].usPN = 5;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[10].bOk  = FALSE;

    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[11].eCmd = CMD_GO_AUTO_2;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[11].usPN = 6;
    pscmPacket->sCmdData[0].uAppData.scmOneByOne.sOne[11].bOk  = TRUE;

    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}


/*****************************************************************************
 函 数 名  : test_pack_afn02f1
 功能描述  : 测试封装登录命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn02f1()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_LOG_IN;
    pscmPacket->sCmdData[0].bApp  = FALSE;
    pscmPacket->sCmdData[0].usPN  = 0;
    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}



/*****************************************************************************
 函 数 名  : test_pack_afn04f1
 功能描述  : 测试封装登录命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f1()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F1_TML_UP_CFG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;
    
    pscmPacket->sData[0].uApp.sTmlUpCfg.bAutoReportAsk_1 = TRUE;
    pscmPacket->sData[0].uApp.sTmlUpCfg.bAutoReportAsk_2 = TRUE;
    pscmPacket->sData[0].uApp.sTmlUpCfg.bAutoReportAsk_3 = TRUE;
    pscmPacket->sData[0].uApp.sTmlUpCfg.ucRTS = 1;
    pscmPacket->sData[0].uApp.sTmlUpCfg.ucRTM = 1;
    pscmPacket->sData[0].uApp.sTmlUpCfg.ucReSendTimes = 3;
    pscmPacket->sData[0].uApp.sTmlUpCfg.ucHeartBeat = 1;
    pscmPacket->sData[0].uApp.sTmlUpCfg.usTmWaitTimeoutS = 1;

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}


/*****************************************************************************
 函 数 名  : test_pack_afn04f10
 功能描述  : 测试封装登录命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f10()
{
    eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;

    // 数据1
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F10_TML_POWER_CFG;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    
    pscmPacket->sData[0].uApp.sTmlPowerCfg.usNum = 2;
    
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].busMeterID = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].busPn = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].bsBaudPort = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].beProto = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].bacMeterAddr = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].bacPwd = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].bucPayRateNum = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].bsDigit = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].bacGathrAddr = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].bsUserClass = TRUE;


    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].usMeterID = 123;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].usPn = 123;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].sBaudPort.eBaudRate = MT_BAUD_600;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].sBaudPort.ucPort = 12;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].eProto = MT_PROTO_DLT645_97;
    memcpy(pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].acMeterAddr, "123456789012", 12);
    memcpy(pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].acPwd, "123456", 6);
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].ucPayRateNum = 40;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].sDigit.ucInteger = 4;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].sDigit.ucDecimal = 1;
    memcpy(pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].acGathrAddr, "987654321012", 12);

    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].sUserClass.ucBig = 12;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[0].sUserClass.ucSmall = 12;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F10_TML_POWER_CFG;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    
    // 数据2
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].busMeterID = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].busPn = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].bsBaudPort = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].beProto = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].bacMeterAddr = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].bacPwd = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].bucPayRateNum = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].bsDigit = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].bacGathrAddr = TRUE;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].bsUserClass = TRUE;


    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].usMeterID = 123;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].usPn = 123;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].sBaudPort.eBaudRate = MT_BAUD_600;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].sBaudPort.ucPort = 12;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].eProto = MT_PROTO_DLT645_97;
    memcpy(pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].acMeterAddr, "123456789012", 12);
    memcpy(pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].acPwd, "123456", 6);
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].ucPayRateNum = 40;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].sDigit.ucInteger = 4;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].sDigit.ucDecimal = 1;
    memcpy(pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].acGathrAddr, "987654321012", 12);

    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].sUserClass.ucBig = 12;
    pscmPacket->sData[0].uApp.sTmlPowerCfg.sOne[1].sUserClass.ucSmall = 12;

    // 数据2
    /*
    pscmPacket->sData[1].eCmd  = CMD_AFN_4_F1_TML_UP_CFG;
    pscmPacket->sData[1].bApp  = TRUE;
    pscmPacket->sData[1].usPN  = 0;
    

    pscmPacket->sData[1].uApp.sTmlUpCfg.bAutoReportAsk_1 = TRUE;
    pscmPacket->sData[1].uApp.sTmlUpCfg.bAutoReportAsk_2 = TRUE;
    pscmPacket->sData[1].uApp.sTmlUpCfg.bAutoReportAsk_3 = TRUE;
    pscmPacket->sData[1].uApp.sTmlUpCfg.ucRTS = 1;
    pscmPacket->sData[1].uApp.sTmlUpCfg.ucRTM = 1;
    pscmPacket->sData[1].uApp.sTmlUpCfg.ucReSendTimes = 3;
    pscmPacket->sData[1].uApp.sTmlUpCfg.ucHeartBeat = 1;
    pscmPacket->sData[1].uApp.sTmlUpCfg.usTmWaitTimeoutS = 1;
*/

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

/*****************************************************************************
 函 数 名  : test_pack_afn04f10_s2m_auto
 功能描述  : 主动上报
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f10_s2m_auto()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;
    pscmPacket->bActive = TRUE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CURNT_PQUI;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 1;

    // app
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bsReadTime = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfP = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQ = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPf = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfI0 = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfS = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSc = TRUE;

    // 数据
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucYY = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucMM = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucDD = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucHH = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucmm = 12;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fP = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPa = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPb = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPc = 12.3456;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQ = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQa = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQb = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQc = 34.5678;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPf  = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfa = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfb = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfc = -123.4;


    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUa = 123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUb = 123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUc = 123.4;
    
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIa = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIb = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIc = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fI0 = -123.456;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fS  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSa  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSb  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSc  = -12.3456;
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

/*****************************************************************************
 函 数 名  : test_pack_afn0cf25_s2m_auto
 功能描述  : 主动上报
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn0cf25_s2m_auto()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;
    pscmPacket->bActive = TRUE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CURNT_PQUI;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 1;

    // app
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bsReadTime = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfP = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQ = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPf = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfI0 = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfS = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSc = TRUE;

    // 数据
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucYY = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucMM = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucDD = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucHH = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucmm = 12;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fP = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPa = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPb = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPc = 12.3456;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQ = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQa = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQb = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQc = 34.5678;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPf  = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfa = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfb = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfc = -123.4;


    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUa = 123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUb = 123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUc = 123.4;
    
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIa = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIb = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIc = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fI0 = -123.456;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fS  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSa  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSb  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSc  = -12.3456;
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}


/*****************************************************************************
 函 数 名  : test_pack_afn04f38_s2m
 功能描述  : 测试封装登录命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn0af38_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CFG_ASK_1_Q;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.ucBig = 1;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.ucM = 1;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].ucSmall = 1;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].ucFN = 2;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].eCmd[0] = CMD_CURNT_PQUI;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].eCmd[1] = CMD_TML_CLOCK;
  

    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

/*****************************************************************************
 函 数 名  : test_pack_afn0af39_s2m
 功能描述  : 测试封装登录命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn0af39_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CFG_ASK_1_Q;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.ucBig = 1;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.ucM = 1;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].ucSmall = 1;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].ucFN = 2;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].eCmd[0] = CMD_CURNT_PQUI;
    pscmPacket->sCmdData[0].uAppData.sAsk1Cfg.sOne[0].eCmd[1] = CMD_TML_CLOCK;
  

    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

/*****************************************************************************
 函 数 名  : test_pack_afn0af65_s2m
 功能描述  : 测试封装登录命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn0af65_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CFG_AUTO_1_Q;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 1;

    // app
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.eUnit = MT_RPT_mm;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.ucPeriod = 3;
    
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucYear = 13;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucMonth = 9;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucWeek = 1;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucDay = 16;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucHour = 11;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucMinute = 12;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucSecond = 13;

    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.ucR = 10;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.ucN = 2;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[0].eCmd = CMD_CURNT_PQUI;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[0].usPn = 10;

    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[1].eCmd = CMD_TML_CLOCK;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[1].usPn = 0;


    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

/*****************************************************************************
 函 数 名  : test_pack_afn0af66_s2m
 功能描述  : 测试封装登录命令
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn0af66_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CFG_AUTO_2_Q;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 1;

    // app
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.eUnit = MT_RPT_mm;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.ucPeriod = 3;
    
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucYear = 13;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucMonth = 9;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucWeek = 1;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucDay = 16;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucHour = 11;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucMinute = 12;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sTime.ucSecond = 13;

    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.ucR = 10;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.ucN = 2;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[0].eCmd = CMD_CURNT_PQUI;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[0].usPn = 10;

    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[1].eCmd = CMD_TML_CLOCK;
    pscmPacket->sCmdData[0].uAppData.sCfgAuto_1.sCmdPn[1].usPn = 0;


    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

/*****************************************************************************
 函 数 名  : test_pack_afn0af67_s2m
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn0af67_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_GOP_AUTO_1_Q;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 1;

    // app
    pscmPacket->sCmdData[0].uAppData.sGoAuto_1.eGo = MT_AUTO_GO;
   


    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}


/*****************************************************************************
 函 数 名  : test_pack_afn0af68_s2m
 功能描述  : 
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn0af68_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_GOP_AUTO_2_Q;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 1;

    // app
    pscmPacket->sCmdData[0].uAppData.sGoAuto_1.eGo = MT_AUTO_STOP;
   


    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}




/*****************************************************************************
 函 数 名  : test_pack_afn04f38_m2s
 功能描述  : F38：1类数据配置设置 （在终端支持的1类数据配置内）
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f38_m2s()
{
    eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F38_CFG_ASK_1;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sAsk1Cfg.ucBig = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg.ucM = 1;

    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].ucSmall = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].ucFN = 3;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[0] = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[1] = CMD_AFN_C_F2_TML_CLOCK;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[2] = CMD_AFN_C_F3_TML_PARA_STATE;

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

/*****************************************************************************
 函 数 名  : test_pack_afn04f39_m2s
 功能描述  : F38：1类数据配置设置 （在终端支持的1类数据配置内）
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f39_m2s()
{
    eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F39_CFG_ASK_2;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sAsk1Cfg.ucBig = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg.ucM = 1;

    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].ucSmall = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].ucFN = 3;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[0] = CMD_AFN_D_F1_FRTH_POWR_P1P4_D;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[1] = CMD_AFN_D_F39_LOAD_RATE_M;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[2] = CMD_AFN_D_F46_COPR_IRON_M;

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

/*****************************************************************************
 函 数 名  : test_pack_afn04f62_m2s
 功能描述  : F65：定时上报1类数据任务设置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f62_m2s()
{
    eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F39_CFG_ASK_2;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sAsk1Cfg.ucBig = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg.ucM = 1;

    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].ucSmall = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].ucFN = 3;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[0] = CMD_AFN_D_F1_FRTH_POWR_P1P4_D;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[1] = CMD_AFN_D_F39_LOAD_RATE_M;
    pscmPacket->sData[0].uApp.sAsk1Cfg.sOne[0].eCmd[2] = CMD_AFN_D_F46_COPR_IRON_M;

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

/*****************************************************************************
 函 数 名  : test_pack_afn04f65_m2s
 功能描述  : F65：定时上报1类数据任务设置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f65_m2s()
{
    eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F65_CFG_AUTO_1;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1; // 任务号

    pscmPacket->sData[0].uApp.sCfgAuto_1.eUnit = MT_RPT_mm;
    pscmPacket->sData[0].uApp.sCfgAuto_1.ucPeriod = 3;
    
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucYear = 13;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucMonth = 9;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucWeek = 1;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucDay = 16;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucHour = 11;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucMinute = 12;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucSecond = 13;


    
    pscmPacket->sData[0].uApp.sCfgAuto_1.ucR = 10;
    pscmPacket->sData[0].uApp.sCfgAuto_1.ucN = 2;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[0].eCmd = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[0].usPn = 10;

    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[1].eCmd = CMD_AFN_C_F2_TML_CLOCK;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[1].usPn = 0;

 
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

/*****************************************************************************
 函 数 名  : test_pack_afn04f66_m2s
 功能描述  : F66：定时上报2类数据任务设置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f66_m2s()
{
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F66_CFG_AUTO_2;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1; // 任务号

    pscmPacket->sData[0].uApp.sCfgAuto_1.eUnit = MT_RPT_mm;
    pscmPacket->sData[0].uApp.sCfgAuto_1.ucPeriod = 3;
    
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucYear = 13;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucMonth = 9;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucWeek = 1;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucDay = 16;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucHour = 11;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucMinute = 12;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sTime.ucSecond = 13;


    
    pscmPacket->sData[0].uApp.sCfgAuto_1.ucR = 10;
    pscmPacket->sData[0].uApp.sCfgAuto_1.ucN = 3;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[0].eCmd = CMD_AFN_D_F1_FRTH_POWR_P1P4_D;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[0].usPn = 1;

    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[1].eCmd = CMD_AFN_D_F43_FACT_TIME_D;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[1].usPn = 2;

    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[2].eCmd = CMD_AFN_D_F53_TML_FLOW_D;
    pscmPacket->sData[0].uApp.sCfgAuto_1.sCmdPn[2].usPn = 3;

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

/*****************************************************************************
 函 数 名  : test_pack_afn04f67_m2s
 功能描述  : F67：定时上报1类数据任务启动/停止设置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f67_m2s()
{
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F67_GOP_AUTO_1;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1; // 任务号

    pscmPacket->sData[0].uApp.sGopAuto_1.eGo = MT_AUTO_GO;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

/*****************************************************************************
 函 数 名  : test_pack_afn04f68_m2s
 功能描述  : F68：定时上报2类数据任务启动/停止设置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日 星期三
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
eMtErr test_pack_afn04f68_m2s()
{
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "0123456789ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_04_SETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_4_F68_GOP_AUTO_2;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1; // 任务号

    pscmPacket->sData[0].uApp.sGopAuto_1.eGo = MT_AUTO_STOP;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;

}

eMtErr test_pack_afn0cf25_S2M()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_CURNT_PQUI;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;
    
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bsReadTime = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfP = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQ = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfQc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPf = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfPfc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfUc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfIc = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfI0 = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfS = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSa = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSb = TRUE;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.bfSc = TRUE;

    // 数据
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucYY = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucMM = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucDD = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucHH = 12;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.sReadTime.ucmm = 12;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fP = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPa = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPb = 12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPc = 12.3456;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQ = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQa = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQb = 34.5678;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fQc = 34.5678;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPf  = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfa = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfb = -123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fPfc = -123.4;


    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUa = 123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUb = 123.4;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fUc = 123.4;
    
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIa = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIb = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fIc = -123.456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fI0 = -123.456;

    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fS  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSa  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSb  = -12.3456;
    pscmPacket->sCmdData[0].uAppData.sCurPQUI.fSc  = -12.3456;

    
    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn05f31()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_05_CTRL;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_5_F31_CHECK_TIME;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;
    pscmPacket->sData[0].uApp.sCheckTime.ucYear   = 13;
    pscmPacket->sData[0].uApp.sCheckTime.ucMonth  = 8;
    pscmPacket->sData[0].uApp.sCheckTime.ucWeek   = 3;
    pscmPacket->sData[0].uApp.sCheckTime.ucDay    = 21;
    pscmPacket->sData[0].uApp.sCheckTime.ucHour   = 15;
    pscmPacket->sData[0].uApp.sCheckTime.ucMinute = 15;
    pscmPacket->sData[0].uApp.sCheckTime.ucSecond = 15;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn05f27()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_05_CTRL;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_5_F27_PERMIT_TALK_GO;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;



    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn05f29()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_05_CTRL;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_5_F29_AUTO_SAY_GO;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;



    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn05f35()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_05_CTRL;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_5_F35_PERMIT_TALK_STOP;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;



    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}


eMtErr test_pack_afn09f1_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 2;
    pscmPacket->sCmdData[0].eCmd  = CMD_AFN_TML_VERSION;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    memcpy(pscmPacket->sCmdData[0].uAppData.sTmlVersion.ucFactoryID, "fact", 4);
    memcpy(pscmPacket->sCmdData[0].uAppData.sTmlVersion.ucDeviceID, "devciced", 8);
    memcpy(pscmPacket->sCmdData[0].uAppData.sTmlVersion.ucSoftwareID, "soft", 4);
    memcpy(pscmPacket->sCmdData[0].uAppData.sTmlVersion.ucInfoCodeCap, "12345678901", 11);
    memcpy(pscmPacket->sCmdData[0].uAppData.sTmlVersion.ucProtolID, "prot", 4);
    memcpy(pscmPacket->sCmdData[0].uAppData.sTmlVersion.ucHardWareID, "hard", 4);


    
    pscmPacket->sCmdData[0].uAppData.sTmlVersion.sDateSoftware.ucYY = 13;
    pscmPacket->sCmdData[0].uAppData.sTmlVersion.sDateSoftware.ucMM = 8;
    pscmPacket->sCmdData[0].uAppData.sTmlVersion.sDateSoftware.ucDD = 23;
    
    pscmPacket->sCmdData[0].uAppData.sTmlVersion.sDateHardware.ucYY = 13;
    pscmPacket->sCmdData[0].uAppData.sTmlVersion.sDateHardware.ucMM = 8;
    pscmPacket->sCmdData[0].uAppData.sTmlVersion.sDateHardware.ucDD = 23;


    #if 1
    pscmPacket->sCmdData[1].eCmd  = CMD_AFN_TML_INPUT_OUT;
    pscmPacket->sCmdData[1].bApp  = TRUE;
    pscmPacket->sCmdData[1].usPN  = 0;

    // app
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.ucPulseInputNum = 12;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.ucSwitchInputNum = 12;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.ucDcNum = 12;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.ucSwitchOutputNum = 12;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.usReadMeterNum = 1234;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.usTmlRecvBuf = 12345;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.usTmlSendBuf = 12345;

    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sMacAddr.ucMac[0] = 1;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sMacAddr.ucMac[1] = 2;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sMacAddr.ucMac[2] = 3;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sMacAddr.ucMac[3] = 4;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sMacAddr.ucMac[4] = 5;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sMacAddr.ucMac[5] = 6;


    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.ucPortNum = 1;

    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].ucPort = 12;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].eInter = MT_INTER_RS232;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].bStandard = TRUE;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].eUse = MT_PORR_USE_CNG;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].ulBaud = 19200;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].usDevNum  = 1234;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].usRecvBuf = 12345;
    pscmPacket->sCmdData[1].uAppData.sTmlInputOut.sPort[0].usSendBuf = 12345;
    #endif
    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn09f2_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_AFN_TML_INPUT_OUT;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.ucPulseInputNum = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.ucSwitchInputNum = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.ucDcNum = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.ucSwitchOutputNum = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.usReadMeterNum = 1234;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.usTmlRecvBuf = 12345;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.usTmlSendBuf = 12345;

    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sMacAddr.ucMac[0] = 1;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sMacAddr.ucMac[1] = 2;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sMacAddr.ucMac[2] = 3;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sMacAddr.ucMac[3] = 4;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sMacAddr.ucMac[4] = 5;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sMacAddr.ucMac[5] = 6;


    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.ucPortNum = 1;

    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].ucPort = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].eInter = MT_INTER_RS232;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].bStandard = TRUE;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].eUse = MT_PORR_USE_CNG;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].ulBaud = 19200;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].usDevNum  = 1234;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].usRecvBuf = 12345;
    pscmPacket->sCmdData[0].uAppData.sTmlInputOut.sPort[0].usSendBuf = 12345;


    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn09f3_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_TML_OTHER_CFG;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.usMp = 1234;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucGroup = 8;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucTask = 64;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucTeam = 8;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.eFrezMp = MT_FREZ_15;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.eFrezP = MT_FREZ_15;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.eFrezQ = MT_FREZ_15;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.eFrezPt = MT_FREZ_15;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.eFrezQt = MT_FREZ_15;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucDay = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucMonth = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucPCtrl = 3;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucHarm = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucCapa = 12;
    pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucVip = 12;

    int i = 0;
    for(i = 0; i < 16; i++)
    {
        pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.bBig[i] = TRUE;
    }

    for(i = 0; i < 16; i++)
    {
        pscmPacket->sCmdData[0].uAppData.sTmlOtherCfg.ucSmall[i] = i + 1;

    }

    
  


    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn09f4_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_SUPPORT_PARA_CFG;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlSuptParaCfg.ucNum = 2;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptParaCfg.eCfgCmd[0] = CMD_CFG_TML_UP;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptParaCfg.eCfgCmd[1] = CMD_CFG_TML_POWER;



    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn09f5_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_SUPPORT_CTRL_CFG;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.ucNum = 7;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[0] = CMD_CTRL_CHECK_TIME;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[1] = CMD_PERMIT_TALK_GO;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[2] = CMD_PERMIT_TALK_STOP;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[3] = CMD_PERMIT_LINK_ON;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[4] = CMD_PERMIT_LINK_OFF;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[5] = CMD_AUTO_REPORT_GO;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[6] = CMD_AUTO_REPORT_STOP;




    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn09f6_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_SUPPORT_ASK1_CFG;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlSuptAsk1.ucNum = 1;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptAsk1.eCfgCmd[0] = CMD_CURNT_PQUI;
  



    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}


eMtErr test_pack_afn09f7_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_SUPPORT_ASK2_CFG;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.ucNum = 7;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[0] = CMD_CTRL_CHECK_TIME;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[1] = CMD_PERMIT_TALK_GO;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[2] = CMD_PERMIT_TALK_STOP;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[3] = CMD_PERMIT_LINK_ON;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[4] = CMD_PERMIT_LINK_OFF;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[5] = CMD_AUTO_REPORT_GO;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptCtrlCfg.eCfgCmd[6] = CMD_AUTO_REPORT_STOP;




    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}


eMtErr test_pack_afn09f8_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_SUPPORT_EVNT_CFG;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.ucNum = 7;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.eErc[0] = MT_ERC_1_INIT;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.eErc[1] = MT_ERC_2_PARA_LOSS;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.eErc[2] = MT_ERC_3_PARA_MODIFY;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.eErc[3] = MT_ERC_4_STATE_CHANGE;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.eErc[4] = MT_ERC_5_REMOTE_SWITCH;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.eErc[5] = MT_ERC_6_PCTRL_SWITCH;
    pscmPacket->sCmdData[0].uAppData.sTmlSuptEvent.eErc[6] = MT_ERC_7_ECTRL_SWITCH;




    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn0cf02_s2m_auto()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;
    pscmPacket->bActive = TRUE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_TML_CLOCK;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sTmlClock.ucYear = 13;
    pscmPacket->sCmdData[0].uAppData.sTmlClock.ucMonth = 9;
    pscmPacket->sCmdData[0].uAppData.sTmlClock.ucWeek = 2;
    pscmPacket->sCmdData[0].uAppData.sTmlClock.ucDay = 24;
    pscmPacket->sCmdData[0].uAppData.sTmlClock.ucHour = 13;
    pscmPacket->sCmdData[0].uAppData.sTmlClock.ucMinute = 13;
    pscmPacket->sCmdData[0].uAppData.sTmlClock.ucSecond = 13;
   



    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn0cf02_s2m()
{
    /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F2_TML_CLOCK;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;
    
    pscmPacket->sData[0].uApp.sTmlClock.ucYear   = 13;
    pscmPacket->sData[0].uApp.sTmlClock.ucMonth  = 8;
    pscmPacket->sData[0].uApp.sTmlClock.ucDay    = 12;
    pscmPacket->sData[0].uApp.sTmlClock.ucWeek   = 1;
    pscmPacket->sData[0].uApp.sTmlClock.ucHour   = 13;
    pscmPacket->sData[0].uApp.sTmlClock.ucMinute = 45;
    pscmPacket->sData[0].uApp.sTmlClock.ucSecond = 12;
   
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf03_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    //UINT16  i;

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F3_TML_PARA_STATE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sTmlParaStat.bParaStatus[1] = 1;
    pscmPacket->sData[0].uApp.sTmlParaStat.bParaStatus[30] = 1;
    pscmPacket->sData[0].uApp.sTmlParaStat.bParaStatus[44] = 1;
    pscmPacket->sData[0].uApp.sTmlParaStat.bParaStatus[89] = 1;
    pscmPacket->sData[0].uApp.sTmlParaStat.bParaStatus[123] = 1;
    pscmPacket->sData[0].uApp.sTmlParaStat.bParaStatus[220] = 1;
    pscmPacket->sData[0].uApp.sTmlParaStat.bParaStatus[231] = 1;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf04_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   // UINT16  i;

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F4_TML_UPCOM_STATE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sTmlUpComStat.bCom = TRUE;
    pscmPacket->sData[0].uApp.sTmlUpComStat.bUp  = TRUE;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf05_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    INT32 i;
  
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F5_TML_CTRL_SET;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sTmlCtrlStat.bRemain  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.bDelete  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.bPayWarn = FALSE;
    for( i = 0; i < MT_AFN0CF05_MAXGROUPS; i++ )
    {
        pscmPacket->sData[0].uApp.sTmlCtrlStat.bGroup[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCtrlStat.bGroup[2] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.bGroup[7] = TRUE;

    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].ucScheme = 8;
    for( i = 0; i < MT_AFN0CF05_MAXGROUPS; i++ )
    {
        pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bPowerCtrl[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bPowerCtrl[4] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bPowerCtrl[5] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bTimeCtrl     = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bRestCtrl     = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bStopCtrl     = FALSE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bDownCtrl     = FALSE;

    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bMonthCtrl    = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bBuyCtrl      = TRUE;

    for( i = 0; i < MT_AFN0CF05_MAXGROUPS; i++ )
    {
        pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bPowerTimeSta[i] = TRUE;
    }
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bPowerTimeSta[3] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bPowerTimeSta[7] = FALSE;

    for( i = 0; i < MT_AFN0CF05_MAXGROUPS; i++ )
    {
        pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bElecTimeSta[i] = TRUE;
    }
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bElecTimeSta[2] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[2].bElecTimeSta[0] = FALSE;

    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].ucScheme = 9;
    for( i = 0; i < MT_AFN0CF05_MAXGROUPS; i++ )
    {
        pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bPowerCtrl[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bPowerCtrl[4] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bPowerCtrl[5] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bTimeCtrl     = FALSE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bRestCtrl     = FALSE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bStopCtrl     = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bDownCtrl     = TRUE;

    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bMonthCtrl    = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bBuyCtrl      = FALSE;

    for( i = 0; i < MT_AFN0CF05_MAXGROUPS; i++ )
    {
        pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bPowerTimeSta[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bPowerTimeSta[5] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bPowerTimeSta[6] = TRUE;

    for( i = 0; i < MT_AFN0CF05_MAXGROUPS; i++ )
    {
        pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bElecTimeSta[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bElecTimeSta[3] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCtrlStat.sGroup[7].bElecTimeSta[4] = TRUE;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf06_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    INT32 i;
  
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F6_TML_CTRL_STATE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    for ( i = 0; i < 8; i++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.bTrip[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.bTrip[2] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.bTrip[6] = TRUE;

    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.ucPayWarn = MT_AFN0CF06_PAYWARNING_ENABLE;

    for ( i = 0; i < 8;i ++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.bGroup[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.bGroup[5] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.bGroup[7] = TRUE;

    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].fCurPower           = 213.0;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].sPowerDrift.eDir    = MT_FLOAT_DIR_UP;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].sPowerDrift.ucValue = 6;

    for( i = 0; i < 8; i++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bPowerOutSta[i] = TRUE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bPowerOutSta[0] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bPowerOutSta[4] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bPowerOutSta[6] = FALSE;

    
    for( i = 0; i < 8; i++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bMonthElecOutSta[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bMonthElecOutSta[1] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bMonthElecOutSta[3] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bMonthElecOutSta[5] = TRUE;

    for( i = 0; i < 8; i++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bBuyElecOutSta[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bBuyElecOutSta[1] = TRUE;

    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bTimeCtrlSta = MT_AFN0CF06_TIME_CTRL_STAT;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bRestCtrlSta = MT_AFN0CF06_REST_CTRL_STAT;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bStopCtrlSta = 0;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bDownCtrlSta = 0;
    
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bMonthCtrlSta = MT_AFN0CF06_MONTH_CTRL_STAT;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[5].bBuyCtrlSta = 0;


    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].fCurPower           = 506.0;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].sPowerDrift.eDir    = MT_FLOAT_DIR_DOWN;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].sPowerDrift.ucValue = 38;

    for( i = 0; i < 8; i++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bPowerOutSta[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bPowerOutSta[2] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bPowerOutSta[3] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bPowerOutSta[7] = TRUE;

    
    for( i = 0; i < 8; i++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bMonthElecOutSta[i] = TRUE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bMonthElecOutSta[2] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bMonthElecOutSta[4] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bMonthElecOutSta[6] = FALSE;

    for( i = 0; i < 8; i++)
    {
        pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bBuyElecOutSta[i] = TRUE;
    }
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bBuyElecOutSta[5] = FALSE;

    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bTimeCtrlSta = 0;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bRestCtrlSta = 0;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bStopCtrlSta = MT_AFN0CF06_STOP_CTRL_STAT;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bDownCtrlSta = MT_AFN0CF06_DOWN_CTRL_STAT;
    
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bMonthCtrlSta = 0;
    pscmPacket->sData[0].uApp.sTmlCurCtrlStat.sGroup[7].bBuyCtrlSta = MT_AFN0CF06_BUY_CTRL_STAT;
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf07_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
  
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F7_TML_EC_VALUE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sTmlEcValue.ucEc1 = 22;
    pscmPacket->sData[0].uApp.sTmlEcValue.ucEc2 = 55;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf08_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    INT32 i;
  
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F8_TML_EVNT_FLAG;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    for(i = 0;i < MT_AFN0CF08_ERC_MAX;i++)
    {
        pscmPacket->sData[0].uApp.sTmlErcSta.bErc[i] = FALSE;
    }
    pscmPacket->sData[0].uApp.sTmlErcSta.bErc[2]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlErcSta.bErc[31] = TRUE;
    pscmPacket->sData[0].uApp.sTmlErcSta.bErc[8] = TRUE;
    pscmPacket->sData[0].uApp.sTmlErcSta.bErc[23] = TRUE;
    pscmPacket->sData[0].uApp.sTmlErcSta.bErc[19] = TRUE;
    pscmPacket->sData[0].uApp.sTmlErcSta.bErc[0]  = TRUE;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf17_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_NELC_TEMP;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sNelcTmper.fsXXX = 121.00;
   

    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn0cf18_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_NELC_HUMi;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 0;

    // app
    pscmPacket->sCmdData[0].uAppData.sNelcHumit.fsXXX = 999.00;
   

    
    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn0cf31_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F31_POWER_CUR;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sTime.ucMM = 9;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sTime.ucDD = 12;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sTime.ucHH = 13;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sTime.ucmm = 56;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 923456.1234;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 654321.1234;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 654321.34;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 234234.78;
    
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseB.dFrthHavePower = 111111.2222;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseB.dBackHavePower = 333333.4444;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseB.fComNonePower1 = 555555.66;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseB.fComNonePower2 = 777777.88;
    
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseC.dFrthHavePower = 999999.1111;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseC.dBackHavePower = 222222.3333;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseC.fComNonePower1 = 444444.55;
    pscmPacket->sData[0].uApp.sTmlPowerCur.sPhaseC.fComNonePower2 = 666666.77;
#if 0    
    pscmPacket->sData[1].eCmd  = CMD_AFN_C_F31_POWER_CUR;
    pscmPacket->sData[1].bApp  = TRUE;
    pscmPacket->sData[1].usPN  = 12;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sTime.ucYY = 13;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sTime.ucMM = 9;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sTime.ucDD = 11;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sTime.ucHH = 14;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sTime.ucmm = 11;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 888888.9999;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 111111.2222;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 444444.33;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 555555.66;
    
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 888888.7777;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 111111.9999;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 333333.22;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 444444.55;
    
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 777777.6666;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 888888.9999;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 444444.00;
    pscmPacket->sData[1].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 222222.33;

    pscmPacket->sData[2].eCmd  = CMD_AFN_C_F31_POWER_CUR;
    pscmPacket->sData[2].bApp  = TRUE;
    pscmPacket->sData[2].usPN  = 26;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sTime.ucYY = 13;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sTime.ucMM = 10;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sTime.ucDD = 23;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sTime.ucHH = 05;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sTime.ucmm = 55;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 123321.1234;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 321123.4321;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 456456.56;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 456654.65;
    
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 789789.7897;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 789987.9789;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 778899.22;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 998877.55;
    
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 224466.1133;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 113355.2244;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 335577.99;
    pscmPacket->sData[2].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 775533.33;

    pscmPacket->sData[3].eCmd  = CMD_AFN_C_F31_POWER_CUR;
    pscmPacket->sData[3].bApp  = TRUE;
    pscmPacket->sData[3].usPN  = 39;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sTime.ucYY = 13;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sTime.ucMM = 5;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sTime.ucDD = 26;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sTime.ucHH = 8;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sTime.ucmm = 55;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 115599.1591;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 995511.5915;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 775533.35;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 335577.53;
    
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 885522.8528;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 225588.2582;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 774411.11;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 996633.55;
    
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.dFrthHavePower = 886622.5544;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.dBackHavePower = 226688.7711;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.fComNonePower1 = 884422.66;
    pscmPacket->sData[3].uApp.sTmlPowerCur.sPhaseA.fComNonePower2 = 224488.33;
    
  #endif  
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf57_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F57_CURT_HARM_VALUE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.ucN = 4;

    // UA    
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUa.bfXXX_X[0] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUa.fXXX_X[0]  = 0.40f; 
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUa.bfXXX_X[1] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUa.fXXX_X[1]  = 0.03;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUa.bfXXX_X[2] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUa.fXXX_X[2]  = 567.8;
  
    // UB
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUb.bfXXX_X[0] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUb.fXXX_X[0]  = 753.9; 
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUb.bfXXX_X[1] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUb.fXXX_X[1]  = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUb.bfXXX_X[2] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUb.fXXX_X[2]  = 951.7;
 
    // UC
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUc.bfXXX_X[0] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUc.fXXX_X[0]  = 888.3; 
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUc.bfXXX_X[1] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUc.fXXX_X[1]  = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUc.bfXXX_X[2] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sUc.fXXX_X[2]  = 999.9;

    // IA
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIa.bfsXX_XX[0] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIa.fsXX_XX[0]  = 0.0; 
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIa.bfsXX_XX[1] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIa.fsXX_XX[1]  = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIa.bfsXX_XX[2] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIa.fsXX_XX[2]  = 0.0;
 
    // IB
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIb.bfsXX_XX[0] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIb.fsXX_XX[0]  = 0.0; 
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIb.bfsXX_XX[1] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIb.fsXX_XX[1]  = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIb.bfsXX_XX[2] = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIb.fsXX_XX[2]  = 0.0;

    // IC
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIc.bfsXX_XX[0] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIc.fsXX_XX[0]  = 79.99; 
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIc.bfsXX_XX[1] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIc.fsXX_XX[1]  = -55.63;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIc.bfsXX_XX[2] = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmValue.sIc.fsXX_XX[2]  = 66.65;


    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf58_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F58_CURT_HARM_RATE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.ucN = 4;
    // UA    
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.sTotal.bfsXXX_X = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.sTotal.fsXXX_X  = -79.4;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.bfsXXX_X[0]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.fsXXX_X[0]   = 23.7;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.bfsXXX_X[1]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.fsXXX_X[1]   = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.bfsXXX_X[2]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUa.fsXXX_X[2]   = 56.1;

    // UB
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.sTotal.bfsXXX_X = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.sTotal.fsXXX_X  = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.bfsXXX_X[0]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.fsXXX_X[0]   = 12.3;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.bfsXXX_X[1]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.fsXXX_X[1]   = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.bfsXXX_X[2]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUb.fsXXX_X[2]   = 0.0;
 
    // UC
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.sTotal.bfsXXX_X = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.sTotal.fsXXX_X  = 33.8;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.bfsXXX_X[0]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.fsXXX_X[0]   = 23.7;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.bfsXXX_X[1]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.fsXXX_X[1]   = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.bfsXXX_X[2]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sUc.fsXXX_X[2]   = 85.2;

    // IA
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.sTotal.bfsXXX_X = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.sTotal.fsXXX_X  = 50.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.bfsXXX_X[0]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.fsXXX_X[0]   = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.bfsXXX_X[1]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.fsXXX_X[1]   = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.bfsXXX_X[2]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIa.fsXXX_X[2]   = 0.0;
 
    // IB
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.sTotal.bfsXXX_X = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.sTotal.fsXXX_X  = 66.4;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.bfsXXX_X[0]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.fsXXX_X[0]   = 55.7;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.bfsXXX_X[1]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.fsXXX_X[1]   = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.bfsXXX_X[2]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIb.fsXXX_X[2]   = -0.1;

    // IC
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.sTotal.bfsXXX_X = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.sTotal.fsXXX_X  = 0.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.bfsXXX_X[0]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.fsXXX_X[0]   = 23.7;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.bfsXXX_X[1]  = FALSE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.fsXXX_X[1]   = 33.0;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.bfsXXX_X[2]  = TRUE;
    pscmPacket->sData[0].uApp.sTmlCurtHarmRate.sIc.fsXXX_X[2]   = 56.1;

    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf73_s2m()
{
   /* 1 定义变量 */ 
   eCmErr eRet;
   UINT16 usBuflen = 0;
   sCmPacket *pscmPacket = (sCmPacket*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    //sInit.eRole = MT_ROLE_MASTER;
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890ABCDEF";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;
    pscmPacket->bReSend = FALSE;

    pscmPacket->ucCmdNum = 1;
    pscmPacket->sCmdData[0].eCmd  = CMD_DC_RLTM_DATA;
    pscmPacket->sCmdData[0].bApp  = TRUE;
    pscmPacket->sCmdData[0].usPN  = 1;

    // app
    pscmPacket->sCmdData[0].uAppData.fDcRltmData = 0.0000;
   


    /* 4 调用函数 */
    eRet = ecm_3761_pack(pscmPacket, (UINT8*)g_ucOutBuf, &usBuflen);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);


    return MT_OK;
}

eMtErr test_pack_afn0cf129_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F129_FRTH_HAVE_POWR_C;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.ucM = 4;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.sTime.ucYY = 13;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.sTime.ucMM = 9;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.sTime.ucDD = 16;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.sTime.ucHH = 10;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.sTime.ucmm = 23;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.dTotalValue = 999999.8765;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.dValueItem[0] = 123456.7890;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.dValueItem[1] = 654321.9876;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.dValueItem[2] = 123321.1023;
    pscmPacket->sData[0].uApp.sTmlFrthHavePowrC.dValueItem[3] = 789987.1235;


    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf130_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F130_FRTH_NONE_POWR_C;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.ucM = 4;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.sTime.ucYY = 13;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.sTime.ucMM = 9;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.sTime.ucDD = 16;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.sTime.ucHH = 10;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.sTime.ucmm = 23;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.dTotalValue = 999999.87;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.dValueItem[0] = 123456.90;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.dValueItem[1] = 654321.96;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.dValueItem[2] = 123321.13;
    pscmPacket->sData[0].uApp.sTmlFrthNonePowrC.dValueItem[3] = 789987.15;


    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf166_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F166_METR_MODIFY_RECD;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sTime.ucYY = 13;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sTime.ucMM = 9;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sTime.ucDD = 16;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sTime.ucHH = 10;

    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOne.usTimes = 1234;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOne.sClock.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOne.sClock.ucMM = 12;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOne.sClock.ucDD = 12;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOne.sClock.ucHH = 12;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOne.sClock.ucmm = 12;

    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOther.usTimes = 6789;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOther.sClock.ucYY = 9;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOther.sClock.ucMM = 9;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOther.sClock.ucDD = 9;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOther.sClock.ucHH = 9;
    pscmPacket->sData[0].uApp.sTmlMetrModifyRecd.sOther.sClock.ucmm = 9;

    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0cf167_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F167_METR_BUY_USE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.sTime.ucYY = 13;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.sTime.ucMM = 9;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.sTime.ucDD = 16;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.sTime.ucHH = 10;

    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.usBuyTimes = 9999;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dCashLeft  = 789654.1235;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dCashTotal = 555555.3333;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dElecLeft  = 666666.22;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dElecOut   = 555555.11;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dElecTotal = 444444.99;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dElecLimit = 222222.88;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dElecWarn  = 111111.77;
    pscmPacket->sData[0].uApp.sTmlMetrBuyUse.dElecFault = 888888.66;
    
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df97_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F97_CURVE_FRTH_HAVE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlCurvePower.sTd_c.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlCurvePower.sTd_c.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlCurvePower.sTd_c.sT.ucDD = 19;
    pscmPacket->sData[0].uApp.sTmlCurvePower.sTd_c.sT.ucHH = 18;
    pscmPacket->sData[0].uApp.sTmlCurvePower.sTd_c.sT.ucmm = 46;
    pscmPacket->sData[0].uApp.sTmlCurvePower.sTd_c.ucM = 255;
    pscmPacket->sData[0].uApp.sTmlCurvePower.sTd_c.ucN = 6;

    pscmPacket->sData[0].uApp.sTmlCurvePower.dEnergy[0] = 1234.5678;
    pscmPacket->sData[0].uApp.sTmlCurvePower.dEnergy[1] = 9999.9999;
    pscmPacket->sData[0].uApp.sTmlCurvePower.dEnergy[2] = 0.0001;
    pscmPacket->sData[0].uApp.sTmlCurvePower.dEnergy[3] = 6666.5555;
    pscmPacket->sData[0].uApp.sTmlCurvePower.dEnergy[4] = 8523.9874;
    pscmPacket->sData[0].uApp.sTmlCurvePower.dEnergy[5] = 3000.0;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df101_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F101_CURVE_FRTH_HAVE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlCurveValue.sTd_c.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlCurveValue.sTd_c.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlCurveValue.sTd_c.sT.ucDD = 19;
    pscmPacket->sData[0].uApp.sTmlCurveValue.sTd_c.sT.ucHH = 18;
    pscmPacket->sData[0].uApp.sTmlCurveValue.sTd_c.sT.ucmm = 46;
    pscmPacket->sData[0].uApp.sTmlCurveValue.sTd_c.ucM = 255;
    pscmPacket->sData[0].uApp.sTmlCurveValue.sTd_c.ucN = 6;

    pscmPacket->sData[0].uApp.sTmlCurveValue.dValue[0] = 1234.56;
    pscmPacket->sData[0].uApp.sTmlCurveValue.dValue[1] = 9999.99;
    pscmPacket->sData[0].uApp.sTmlCurveValue.dValue[2] = 0.01;
    pscmPacket->sData[0].uApp.sTmlCurveValue.dValue[3] = 6666.55;
    pscmPacket->sData[0].uApp.sTmlCurveValue.dValue[4] = 8523.98;
    pscmPacket->sData[0].uApp.sTmlCurveValue.dValue[5] = 3000.0;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df105_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F105_CURVE_FACTOR_T;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlCurveFactor.sTd_c.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.sTd_c.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.sTd_c.sT.ucDD = 19;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.sTd_c.sT.ucHH = 18;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.sTd_c.sT.ucmm = 46;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.sTd_c.ucM = 255;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.sTd_c.ucN = 6;

    pscmPacket->sData[0].uApp.sTmlCurveFactor.fFactor[0] = 799.9;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.fFactor[1] = -799.9;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.fFactor[2] = 0.1;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.fFactor[3] = -0.1;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.fFactor[4] = 123.4;
    pscmPacket->sData[0].uApp.sTmlCurveFactor.fFactor[5] = 567.8;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df109_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F109_ANGLE_CURVE_VOLT;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlCurvePhase.sTd_c.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sTd_c.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sTd_c.sT.ucDD = 19;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sTd_c.sT.ucHH = 18;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sTd_c.sT.ucmm = 46;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sTd_c.ucM = 255;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sTd_c.ucN = 6;

    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[0].fPhaseA = 799.9;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[0].fPhaseB = 799.9;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[0].fPhaseC = 799.9;
    
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[1].fPhaseA = -799.9;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[1].fPhaseB = -799.9;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[1].fPhaseC = -799.9;
    
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[2].fPhaseA = 0.1;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[2].fPhaseB = 0.1;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[2].fPhaseC = 0.1;
    
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[3].fPhaseA = -0.1;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[3].fPhaseB = -0.1;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[3].fPhaseC = -0.1;
    
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[4].fPhaseA = 123.4;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[4].fPhaseB = 123.4;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[4].fPhaseC = 123.4;

    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[5].fPhaseA = 567.8;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[5].fPhaseB = 567.8;
    pscmPacket->sData[0].uApp.sTmlCurvePhase.sPhase[5].fPhaseC = 567.8;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df113_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F113_ELEC_HARM_TIME_A;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlIHarmTime.sTd_d.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sTd_d.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sTd_d.sT.ucDD = 19;

    memset(pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime,1,sizeof(sMtIHarmTime)* MT_AFN0DF113_COUNT);
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[0].fHarmI = 79.99;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[0].sTime.ucDD = 18;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[0].sTime.ucMM = 5;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[0].sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[0].sTime.ucmm = 59;

    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[17].fHarmI = 0.01;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[17].sTime.ucDD = 18;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[17].sTime.ucMM = 5;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[17].sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlIHarmTime.sHarmTime[17].sTime.ucmm = 59;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df116_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F116_VOLT_HARM_TIME_A;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlUHarmTime.sTd_d.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sTd_d.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sTd_d.sT.ucDD = 19;

    memset(pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime,1,sizeof(sMtUHarmTime)* MT_AFN0DF113_COUNT);
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[0].fHarmU = 799.9;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[0].sTime.ucDD = 18;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[0].sTime.ucMM = 5;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[0].sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[0].sTime.ucmm = 59;

    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[17].fHarmU= 0.1;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[17].sTime.ucDD = 18;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[17].sTime.ucMM = 5;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[17].sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlUHarmTime.sHarmTime[17].sTime.ucmm = 59;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df121_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F121_HARM_OVER_A;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlHarmOver.sTd_d.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlHarmOver.sTd_d.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlHarmOver.sTd_d.sT.ucDD = 19;

    pscmPacket->sData[0].uApp.sTmlHarmOver.ucN = 3;
    
    pscmPacket->sData[0].uApp.sTmlHarmOver.usUAbnormal = 99;
    pscmPacket->sData[0].uApp.sTmlHarmOver.usUHarm[0] = 1;
    pscmPacket->sData[0].uApp.sTmlHarmOver.usUHarm[1] = 2;
    pscmPacket->sData[0].uApp.sTmlHarmOver.usUHarm[2] = 3;

    pscmPacket->sData[0].uApp.sTmlHarmOver.usIAbnormal = 6;
    pscmPacket->sData[0].uApp.sTmlHarmOver.usIHarm[0] = 7;
    pscmPacket->sData[0].uApp.sTmlHarmOver.usIHarm[1] = 8;
    pscmPacket->sData[0].uApp.sTmlHarmOver.usIHarm[2] = 9;
    
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df169_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F169_HAVE_FRTH_R;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlValue.sTd_m.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlValue.sTd_m.sT.ucMM = 6;

    pscmPacket->sData[0].uApp.sTmlValue.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlValue.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlValue.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlValue.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlValue.sTime.ucmm = 59;

    pscmPacket->sData[0].uApp.sTmlValue.dValueA = 999999.9999;
    pscmPacket->sData[0].uApp.sTmlValue.dValueB = 123456.7890;
    pscmPacket->sData[0].uApp.sTmlValue.dValueC = 0.0001;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df170_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F170_NONE_FRTM_R;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTd_d.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTd_d.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTd_d.sT.ucDD = 23;

    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.sTime.ucmm = 59;
   
    pscmPacket->sData[0].uApp.sTmlReactiveValue.ucM = 3;

    pscmPacket->sData[0].uApp.sTmlReactiveValue.dValue[0] = 999999.99;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.dValue[1] = 123456.90;
    pscmPacket->sData[0].uApp.sTmlReactiveValue.dValue[2] = 0.01;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df171_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F171_HAVE_BACK_R;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlActiveValue.sTd_d.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlActiveValue.sTd_d.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlActiveValue.sTd_d.sT.ucDD = 23;

    pscmPacket->sData[0].uApp.sTmlActiveValue.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlActiveValue.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlActiveValue.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlActiveValue.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlActiveValue.sTime.ucmm = 59;
   
    pscmPacket->sData[0].uApp.sTmlActiveValue.ucM = 3;

    pscmPacket->sData[0].uApp.sTmlActiveValue.dValue[0] = 999999.9988;
    pscmPacket->sData[0].uApp.sTmlActiveValue.dValue[1] = 123456.9001;
    pscmPacket->sData[0].uApp.sTmlActiveValue.dValue[2] = 0.0001;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df177_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F177_HAVE_FRTH_M;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlActiveValueM.sTd_m.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlActiveValueM.sTd_m.sT.ucMM = 6;

    pscmPacket->sData[0].uApp.sTmlActiveValueM.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlActiveValueM.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlActiveValueM.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlActiveValueM.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlActiveValueM.sTime.ucmm = 59;
   
    pscmPacket->sData[0].uApp.sTmlActiveValueM.ucM = 3;

    pscmPacket->sData[0].uApp.sTmlActiveValueM.dValue[0] = 999999.9988;
    pscmPacket->sData[0].uApp.sTmlActiveValueM.dValue[1] = 123456.9001;
    pscmPacket->sData[0].uApp.sTmlActiveValueM.dValue[2] = 0.0001;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df178_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F178_NONE_FRTM_M;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlReactiveValueM.sTd_m.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.sTd_m.sT.ucMM = 6;

    pscmPacket->sData[0].uApp.sTmlReactiveValueM.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.sTime.ucmm = 59;
   
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.ucM = 3;

    pscmPacket->sData[0].uApp.sTmlReactiveValueM.dValue[0] = 999999.98;
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.dValue[1] = 123456.90;
    pscmPacket->sData[0].uApp.sTmlReactiveValueM.dValue[2] = 0.01;
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df185_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F185_HAVE_DMND_FRTH_D;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlMaxTime.sTd_d.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sTd_d.sT.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sTd_d.sT.ucDD = 23;

    pscmPacket->sData[0].uApp.sTmlMaxTime.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sTime.ucmm = 59;
   
    pscmPacket->sData[0].uApp.sTmlMaxTime.ucM = 2;

    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[0].fMax = 99.9999;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[0].sClock.ucMM = 8;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[0].sClock.ucDD = 16;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[0].sClock.ucHH = 9;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[0].sClock.ucmm = 30;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[1].fMax = 12.3456;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[1].sClock.ucMM = 9;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[1].sClock.ucDD = 17;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[1].sClock.ucHH = 10;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[1].sClock.ucmm = 31;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[2].fMax = 0.01;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[2].sClock.ucMM = 10;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[2].sClock.ucDD = 18;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[2].sClock.ucHH = 11;
    pscmPacket->sData[0].uApp.sTmlMaxTime.sMaxClock[2].sClock.ucmm = 32;    
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df215_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F215_METR_BUY_USR;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlMeterInfo.sTd_m.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.sTd_m.sT.ucMM = 6;

    pscmPacket->sData[0].uApp.sTmlMeterInfo.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.sTime.ucmm = 59;
   
    pscmPacket->sData[0].uApp.sTmlMeterInfo.usBuyTimes = 9999;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dCashLeft = 999999.9999;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dCashTotal = 0.0001;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dKilowattLeft = 999999.99;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dKilowattOver = 0.01;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dKilowattTotal = 123456.78;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dKilowattDebit = 0.0;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dKilowattWarn = 987654.32;
    pscmPacket->sData[0].uApp.sTmlMeterInfo.dKilowattFault = 111111.11;    
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn0df216_s2m()
{
    /* 1 定义变量 */ 
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
    
    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_CONTOR;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 123;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_S2M;
    pscmPacket->ePRM = MT_PRM_PASIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F216_METR_BALANCE;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;

    pscmPacket->sData[0].uApp.sTmlBalanceInfo.sTd_m.sT.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.sTd_m.sT.ucMM = 6;

    pscmPacket->sData[0].uApp.sTmlBalanceInfo.sTime.ucYY = 12;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.sTime.ucMM = 6;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.sTime.ucDD = 23;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.sTime.ucHH = 23;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.sTime.ucmm = 59;

    pscmPacket->sData[0].uApp.sTmlBalanceInfo.ucM = 3;

    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dTotalBalance = 111111.1111;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dBalance[0] = 222222.2222;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dBalance[1] = 333333.3333;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dBalance[2] = 444444.4444;
    
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dTotalNoBalance = 666666.6666;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dNoBalance[0] = 777777.7777;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dNoBalance[1] = 888888.8888;
    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dNoBalance[2] = 999999.9999;

    pscmPacket->sData[0].uApp.sTmlBalanceInfo.dFault = 0.0;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);
    return MT_OK;
}

eMtErr test_pack_afn05f37()
{
    eCmErr eRet;
    UINT16 usBuflen = 0;
    smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_05_CTRL;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_5_F37_AUTO_SAY_STOP;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;



    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}


eMtErr test_pack_afn05f38()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_05_CTRL;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_5_F38_TML_LINK_ON;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;



    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn09f1_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F1_TML_VERSION;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;



    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn09f2_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


#if 0
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F2_TML_INPUT_OUT;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;
#endif


    /*
      两个命令合在一起  

      
    */

#if 1
    pscmPacket->usDataNum = 2;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F1_TML_VERSION;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;
    
    pscmPacket->sData[1].eCmd  = CMD_AFN_9_F2_TML_INPUT_OUT;
    pscmPacket->sData[1].bApp  = FALSE;
    pscmPacket->sData[1].usPN  = 0;
#endif


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn09f3_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


#if 1
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F3_TML_OTHER_CFG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;
#endif


    /*
      两个命令合在一起  

      
    */

#if 0
    pscmPacket->usDataNum = 2;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F1_TML_VERSION;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;
    
    pscmPacket->sData[1].eCmd  = CMD_AFN_9_F2_TML_INPUT_OUT;
    pscmPacket->sData[1].bApp  = FALSE;
    pscmPacket->sData[1].usPN  = 0;
#endif


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn09f4_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F4_SUPPORT_PARA_CFG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;






    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn09f5_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F5_SUPPORT_CTRL_CFG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn09f6_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F6_SUPPORT_ASK1_CFG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn09f7_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F7_SUPPORT_ASK2_CFG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn09f8_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_09_CFIG;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_9_F8_SUPPORT_EVNT_CFG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0af10_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 ????? */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("?????\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 ???? */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0A_GETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_A_F10_TML_POWER_CFG;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;

    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usN = 10;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[0] = 1;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[1] = 2;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[2] = 3;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[3] = 4;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[4] = 5;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[5] = 6;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[6] = 7;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[7] = 8;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[8] = 9;
    pscmPacket->sData[0].uApp.sTmlPowerCfg_Q.usID[9] = 10;
    


    /* 4 ???? */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 ???? */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}
eMtErr test_pack_afn0af38_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0A_GETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_A_F38_CFG_ASK_1;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;


    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucBig = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucN = 16;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[0] = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[1] = 2;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[2] = 3;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[3] = 4;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[4] = 5;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[5] = 6;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[6] = 7;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[7] = 8;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[8] = 9;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[9] = 10;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[10] = 11;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[11] = 12;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[12] = 13;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[13] = 14;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[14] = 15;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[15] = 16;

   


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0af39_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0A_GETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_A_F39_CFG_ASK_2;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucBig = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucN = 16;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[0] = 1;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[1] = 2;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[2] = 3;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[3] = 4;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[4] = 5;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[5] = 6;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[6] = 7;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[7] = 8;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[8] = 9;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[9] = 10;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[10] = 11;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[11] = 12;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[12] = 13;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[13] = 14;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[14] = 15;
    pscmPacket->sData[0].uApp.sAsk1Cfg_Q.ucSmall[15] = 16;

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0af65_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0A_GETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_A_F65_CFG_AUTO_1;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 123;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0af66_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0A_GETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_A_F66_CFG_AUTO_2;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 123;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0af67_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0A_GETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_A_F67_GOP_AUTO_1;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 123;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0af68_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0A_GETP;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_A_F68_GOP_AUTO_2;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 123;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}
eMtErr test_pack_afn05f39()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_05_CTRL;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_5_F39_TML_LINK_OFF;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 0;



    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf02_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F2_TML_CLOCK;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf03_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F3_TML_PARA_STATE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf04_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F4_TML_UPCOM_STATE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf05_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F5_TML_CTRL_SET;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf06_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F6_TML_CTRL_STATE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf07_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F7_TML_EC_VALUE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf08_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F8_TML_EVNT_FLAG;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 0;


    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf25()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    char *str = "1234567890123456";
    bCmSetPw(str);
  
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;


    pscmPacket->usDataNum = 6;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
    
    pscmPacket->sData[1].eCmd  = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[1].bApp  = FALSE;
    pscmPacket->sData[1].usPN  = 2;
    
    pscmPacket->sData[2].eCmd  = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[2].bApp  = FALSE;
    pscmPacket->sData[2].usPN  = 3;
 
    pscmPacket->sData[3].eCmd  = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[3].bApp  = FALSE;
    pscmPacket->sData[3].usPN  = 56;
    
    pscmPacket->sData[4].eCmd  = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[4].bApp  = FALSE;
    pscmPacket->sData[4].usPN  = 57;    

    pscmPacket->sData[5].eCmd  = CMD_AFN_C_F25_POWER_RATE_CUR;
    pscmPacket->sData[5].bApp  = FALSE;
    pscmPacket->sData[5].usPN  = 58;
    

    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf31_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F31_POWER_CUR;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf57_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F57_CURT_HARM_VALUE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf58_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F58_CURT_HARM_RATE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf73_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F73_DC_RLTM_DATA;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}
eMtErr test_pack_afn0cf129_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F129_FRTH_HAVE_POWR_C;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf130_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F130_FRTH_NONE_POWR_C;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf166_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F166_METR_MODIFY_RECD;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0cf167_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0C_ASK1;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_C_F167_METR_BUY_USE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df28_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F28_UBLN_OVER_D;
    pscmPacket->sData[0].bApp  = TRUE;
    pscmPacket->sData[0].usPN  = 1;
    pscmPacket->sData[0].uApp.sTd_d.sT.ucYY = 13;
    pscmPacket->sData[0].uApp.sTd_d.sT.ucMM = 9;
    pscmPacket->sData[0].uApp.sTd_d.sT.ucDD = 22;
    
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df36_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F36_UBLN_OVER_M;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}
eMtErr test_pack_afn0df97_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F97_CURVE_FRTH_HAVE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df101_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F101_CURVE_FRTH_HAVE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df105_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F105_CURVE_FACTOR_T;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df109_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F109_ANGLE_CURVE_VOLT;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df113_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F113_ELEC_HARM_TIME_A;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df116_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F116_VOLT_HARM_TIME_A;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;

}

eMtErr test_pack_afn0df121_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F121_HARM_OVER_A;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df169_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F169_HAVE_FRTH_R;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df170_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F170_NONE_FRTM_R;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df171_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F171_HAVE_BACK_R;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df177_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F177_HAVE_FRTH_M;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df178_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F178_NONE_FRTM_M;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df185_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F185_HAVE_DMND_FRTH_D;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df215_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F215_METR_BUY_USR;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}

eMtErr test_pack_afn0df216_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F216_METR_BALANCE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}


// 请求一般事件 命令测试例

eMtErr test_pack_afn0ef1_m2s()
{
   eCmErr eRet;
   UINT16 usBuflen = 0;
   smtPack *pscmPacket = (smtPack*)g_ucPackMem;
   

    /* 2 环境初始化 */
    sCmInit  sInit;
    sInit.eRole = MT_ROLE_MASTER;
    //sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eRet = ecm_3761_init(&sInit);
    if(eRet != MT_OK)
    {
        printf("初始化失败\n");
        return eRet;
    }
    
    /* 3 封装参数 */
    memcpy(pscmPacket->sAddress.acRegionCode, "1100", 4);
    pscmPacket->sAddress.usTAddress = 1;
    pscmPacket->sAddress.bTeamAddr  = FALSE;
    pscmPacket->sAddress.ucMAddress = 1;

    pscmPacket->eAFN = AFN_0D_ASK2;
    pscmPacket->eDir = MT_DIR_M2S;
    pscmPacket->ePRM = MT_PRM_ACTIVE;
    
    pscmPacket->ePos = MT_POS_SIGLE;
    pscmPacket->ucSeq = 0;
    pscmPacket->bAcdFcb = TRUE;
    
    pscmPacket->usDataNum = 1;
    pscmPacket->sData[0].eCmd  = CMD_AFN_D_F216_METR_BALANCE;
    pscmPacket->sData[0].bApp  = FALSE;
    pscmPacket->sData[0].usPN  = 1;
     
    
    /* 4 调用函数 */
    eRet = emtPack(pscmPacket, &usBuflen, (UINT8*)g_ucOutBuf);
    if(eRet != MT_OK)
    {
        printf("ecm_3761_pack error %d\n", eRet);
        return eRet;
    }
    
    /* 5 输出结果 */ 
    printf_buffer_color((char*)g_ucOutBuf, usBuflen);

    return MT_OK;
}



typedef eMtErr(*ptest_pack)();

typedef struct
{
    int          nTestID;
    ptest_pack   pTest;

}sTestPack;


// 下行测试例表
sTestPack  g_test_pack_down[] = 
{   
    {CMD_AFN_0_F1_ALL_OK,          test_pack_0001_m2s},
    {CMD_AFN_0_F3_ONE_BY_ONE,      test_pack_afn00_f3},
    {CMD_AFN_2_F1_LOG_IN,          test_pack_afn02f1},
    {CMD_AFN_4_F1_TML_UP_CFG,      test_pack_afn04f1},
    {CMD_AFN_4_F10_TML_POWER_CFG,  test_pack_afn04f10},
    {CMD_AFN_4_F38_CFG_ASK_1,      test_pack_afn04f38_m2s},
    {CMD_AFN_4_F39_CFG_ASK_2,      test_pack_afn04f39_m2s},
    {CMD_AFN_4_F65_CFG_AUTO_1,     test_pack_afn04f65_m2s},
    {CMD_AFN_4_F66_CFG_AUTO_2,     test_pack_afn04f66_m2s},
    {CMD_AFN_4_F67_GOP_AUTO_1,     test_pack_afn04f67_m2s},
    {CMD_AFN_4_F68_GOP_AUTO_2,     test_pack_afn04f68_m2s},

    {CMD_AFN_5_F27_PERMIT_TALK_GO,     test_pack_afn05f27},
    {CMD_AFN_5_F29_AUTO_SAY_GO,        test_pack_afn05f29},
    {CMD_AFN_5_F35_PERMIT_TALK_STOP,    test_pack_afn05f35},
    {CMD_AFN_5_F37_AUTO_SAY_STOP,     test_pack_afn05f37},
    {CMD_AFN_5_F38_TML_LINK_ON,     test_pack_afn05f38},
    {CMD_AFN_5_F39_TML_LINK_OFF,     test_pack_afn05f39},

    {CMD_AFN_9_F1_TML_VERSION,     test_pack_afn09f1_m2s},
    {CMD_AFN_9_F2_TML_INPUT_OUT,   test_pack_afn09f2_m2s},
    {CMD_AFN_9_F3_TML_OTHER_CFG,   test_pack_afn09f3_m2s},
    {CMD_AFN_9_F4_SUPPORT_PARA_CFG,   test_pack_afn09f4_m2s},
    {CMD_AFN_9_F5_SUPPORT_CTRL_CFG,   test_pack_afn09f5_m2s},
    {CMD_AFN_9_F6_SUPPORT_ASK1_CFG,   test_pack_afn09f6_m2s},
    {CMD_AFN_9_F7_SUPPORT_ASK2_CFG,   test_pack_afn09f7_m2s},
    {CMD_AFN_9_F8_SUPPORT_EVNT_CFG,   test_pack_afn09f8_m2s},

    {CMD_AFN_A_F10_TML_POWER_CFG,         test_pack_afn0af10_m2s},

    {CMD_AFN_A_F38_CFG_ASK_1,         test_pack_afn0af38_m2s},
    {CMD_AFN_A_F39_CFG_ASK_2,         test_pack_afn0af39_m2s},
    {CMD_AFN_A_F65_CFG_AUTO_1,        test_pack_afn0af65_m2s},
    {CMD_AFN_A_F66_CFG_AUTO_2,        test_pack_afn0af66_m2s},
    {CMD_AFN_A_F67_GOP_AUTO_1,        test_pack_afn0af67_m2s},
    {CMD_AFN_A_F68_GOP_AUTO_2,        test_pack_afn0af68_m2s},
    

    {CMD_AFN_C_F2_TML_CLOCK,          test_pack_afn0cf02_m2s},
    {CMD_AFN_C_F3_TML_PARA_STATE,     test_pack_afn0cf03_m2s},
    {CMD_AFN_C_F4_TML_UPCOM_STATE,    test_pack_afn0cf04_m2s},
    {CMD_AFN_C_F5_TML_CTRL_SET,       test_pack_afn0cf05_m2s},
    {CMD_AFN_C_F6_TML_CTRL_STATE,     test_pack_afn0cf06_m2s},
    {CMD_AFN_C_F7_TML_EC_VALUE,       test_pack_afn0cf07_m2s},
    {CMD_AFN_C_F8_TML_EVNT_FLAG,      test_pack_afn0cf08_m2s},
    {CMD_AFN_C_F25_POWER_RATE_CUR,    test_pack_afn0cf25},
    {CMD_AFN_C_F31_POWER_CUR,         test_pack_afn0cf31_m2s},
    {CMD_AFN_C_F57_CURT_HARM_VALUE,   test_pack_afn0cf57_m2s},
    {CMD_AFN_C_F58_CURT_HARM_RATE,    test_pack_afn0cf58_m2s},
    {CMD_AFN_C_F73_DC_RLTM_DATA,    test_pack_afn0cf73_m2s},
        
    {CMD_AFN_C_F129_FRTH_HAVE_POWR_C, test_pack_afn0cf129_m2s},
    {CMD_AFN_C_F130_FRTH_NONE_POWR_C, test_pack_afn0cf130_m2s},
    {CMD_AFN_C_F166_METR_MODIFY_RECD, test_pack_afn0cf166_m2s},
    {CMD_AFN_C_F167_METR_BUY_USE,     test_pack_afn0cf167_m2s},

    {CMD_AFN_D_F97_CURVE_FRTH_HAVE,   test_pack_afn0df97_m2s},
    {CMD_AFN_D_F101_CURVE_FRTH_HAVE,  test_pack_afn0df101_m2s},
    {CMD_AFN_D_F105_CURVE_FACTOR_T,   test_pack_afn0df105_m2s},
    {CMD_AFN_D_F109_ANGLE_CURVE_VOLT, test_pack_afn0df109_m2s},
    {CMD_AFN_D_F113_ELEC_HARM_TIME_A, test_pack_afn0df113_m2s},
    {CMD_AFN_D_F116_VOLT_HARM_TIME_A, test_pack_afn0df116_m2s},
    {CMD_AFN_D_F121_HARM_OVER_A,      test_pack_afn0df121_m2s},
    {CMD_AFN_D_F169_HAVE_FRTH_R,      test_pack_afn0df169_m2s},
    {CMD_AFN_D_F170_NONE_FRTM_R,      test_pack_afn0df170_m2s},
    {CMD_AFN_D_F171_HAVE_BACK_R,      test_pack_afn0df171_m2s},
    {CMD_AFN_D_F177_HAVE_FRTH_M,      test_pack_afn0df177_m2s},
    {CMD_AFN_D_F178_NONE_FRTM_M,      test_pack_afn0df178_m2s},
    {CMD_AFN_D_F185_HAVE_DMND_FRTH_D, test_pack_afn0df185_m2s},
    {CMD_AFN_D_F215_METR_BUY_USR,     test_pack_afn0df215_m2s},
    {CMD_AFN_D_F216_METR_BALANCE,     test_pack_afn0df216_m2s},
    {CMD_AFN_D_F28_UBLN_OVER_D,       test_pack_afn0df28_m2s},
    {CMD_AFN_D_F36_UBLN_OVER_M,       test_pack_afn0df36_m2s},

    // 事件
    {CMD_AFN_E_F1_EVENT_1,            test_pack_afn0ef1_m2s},


    {0xFFFF, NULL}
    
};

// 上行测试例表
sTestPack  g_test_pack[] = 
{

    
    {CMD_AFN_0_F1_ALL_OK,          test_pack_0001},
    {CMD_AFN_0_F3_ONE_BY_ONE,      test_pack_afn00_f3},
    {CMD_AFN_2_F1_LOG_IN,          test_pack_afn02f1},
    {CMD_AFN_4_F1_TML_UP_CFG,      test_pack_afn04f1},

    
    {CMD_AFN_4_F10_TML_POWER_CFG,  test_pack_afn04f10_s2m_auto},

 
    {CMD_AFN_5_F31_CHECK_TIME,     test_pack_afn05f31},

    {CMD_AFN_5_F27_PERMIT_TALK_GO,    test_pack_afn05f27},
    {CMD_AFN_5_F29_AUTO_SAY_GO,       test_pack_afn05f29},
    {CMD_AFN_5_F31_CHECK_TIME,        test_pack_afn05f31},
    {CMD_AFN_5_F35_PERMIT_TALK_STOP,  test_pack_afn05f35},
    {CMD_AFN_5_F37_AUTO_SAY_STOP,     test_pack_afn05f37},
    {CMD_AFN_5_F38_TML_LINK_ON,       test_pack_afn05f38},
    {CMD_AFN_5_F39_TML_LINK_OFF,      test_pack_afn05f39},

    {CMD_AFN_9_F1_TML_VERSION,        test_pack_afn09f1_s2m},
    {CMD_AFN_9_F2_TML_INPUT_OUT,      test_pack_afn09f2_s2m},
    {CMD_AFN_9_F3_TML_OTHER_CFG,      test_pack_afn09f3_s2m},
    {CMD_AFN_9_F4_SUPPORT_PARA_CFG,   test_pack_afn09f4_s2m},
    {CMD_AFN_9_F5_SUPPORT_CTRL_CFG,   test_pack_afn09f5_s2m},
    {CMD_AFN_9_F6_SUPPORT_ASK1_CFG,   test_pack_afn09f6_s2m},
    {CMD_AFN_9_F7_SUPPORT_ASK2_CFG,   test_pack_afn09f7_s2m},
    {CMD_AFN_9_F8_SUPPORT_EVNT_CFG,   test_pack_afn09f8_s2m},

    {CMD_AFN_A_F38_CFG_ASK_1,         test_pack_afn0af38_s2m},
    {CMD_AFN_A_F39_CFG_ASK_2,         test_pack_afn0af39_s2m},
    {CMD_AFN_A_F65_CFG_AUTO_1,        test_pack_afn0af65_s2m},
    {CMD_AFN_A_F66_CFG_AUTO_2,        test_pack_afn0af66_s2m},
    
    {CMD_AFN_A_F67_GOP_AUTO_1,     test_pack_afn0af67_s2m},
    {CMD_AFN_A_F68_GOP_AUTO_2,     test_pack_afn0af68_s2m},
        

    
   // {CMD_AFN_C_F2_TML_CLOCK,          test_pack_afn0cf02_s2m},   // 被动应答
    {CMD_AFN_C_F2_TML_CLOCK,          test_pack_afn0cf02_s2m_auto}, // 主动上报
   
    {CMD_AFN_C_F3_TML_PARA_STATE,     test_pack_afn0cf03_s2m},
    {CMD_AFN_C_F4_TML_UPCOM_STATE,    test_pack_afn0cf04_s2m},
    {CMD_AFN_C_F5_TML_CTRL_SET,       test_pack_afn0cf05_s2m},
    {CMD_AFN_C_F6_TML_CTRL_STATE,     test_pack_afn0cf06_s2m},
    {CMD_AFN_C_F7_TML_EC_VALUE,       test_pack_afn0cf07_s2m},
    {CMD_AFN_C_F8_TML_EVNT_FLAG,      test_pack_afn0cf08_s2m},
    {CMD_AFN_C_F17_TOTAL_POWER_HAVE,  test_pack_afn0cf17_s2m},
    {CMD_AFN_C_F18_TOTAL_POWER_NONE,  test_pack_afn0cf18_s2m},
    //{CMD_AFN_C_F25_POWER_RATE_CUR,    test_pack_afn0cf25_s2m_auto},
    {CMD_AFN_C_F31_POWER_CUR,         test_pack_afn0cf31_s2m},
    {CMD_AFN_C_F57_CURT_HARM_VALUE,   test_pack_afn0cf57_s2m},
    {CMD_AFN_C_F58_CURT_HARM_RATE,    test_pack_afn0cf58_s2m},
    {CMD_AFN_C_F73_DC_RLTM_DATA ,     test_pack_afn0cf73_s2m},

    {CMD_AFN_C_F129_FRTH_HAVE_POWR_C, test_pack_afn0cf129_s2m},
    {CMD_AFN_C_F130_FRTH_NONE_POWR_C, test_pack_afn0cf130_s2m},
    {CMD_AFN_C_F166_METR_MODIFY_RECD, test_pack_afn0cf166_s2m},
    {CMD_AFN_C_F167_METR_BUY_USE,     test_pack_afn0cf167_s2m},

    {CMD_AFN_D_F97_CURVE_FRTH_HAVE,   test_pack_afn0df97_s2m},
    {CMD_AFN_D_F101_CURVE_FRTH_HAVE,  test_pack_afn0df101_s2m},
    {CMD_AFN_D_F105_CURVE_FACTOR_T,   test_pack_afn0df105_s2m},
    {CMD_AFN_D_F109_ANGLE_CURVE_VOLT, test_pack_afn0df109_s2m},
    {CMD_AFN_D_F113_ELEC_HARM_TIME_A, test_pack_afn0df113_s2m},
    {CMD_AFN_D_F116_VOLT_HARM_TIME_A, test_pack_afn0df116_s2m},
    {CMD_AFN_D_F121_HARM_OVER_A,      test_pack_afn0df121_s2m},
    {CMD_AFN_D_F169_HAVE_FRTH_R,      test_pack_afn0df169_s2m},
    {CMD_AFN_D_F170_NONE_FRTM_R,      test_pack_afn0df170_s2m},
    {CMD_AFN_D_F171_HAVE_BACK_R,      test_pack_afn0df171_s2m},
    {CMD_AFN_D_F177_HAVE_FRTH_M,      test_pack_afn0df177_s2m},
    {CMD_AFN_D_F178_NONE_FRTM_M,      test_pack_afn0df178_s2m},
    {CMD_AFN_D_F185_HAVE_DMND_FRTH_D, test_pack_afn0df185_s2m},
    {CMD_AFN_D_F215_METR_BUY_USR,     test_pack_afn0df215_s2m},
    {CMD_AFN_D_F216_METR_BALANCE,     test_pack_afn0df216_s2m},
 
    {CMD_AFN_C_F25_POWER_RATE_CUR, test_pack_afn0cf25_S2M},


    {0xFFFF, NULL}
    
};

/*****************************************************************************
 函 数 名  : test_pack
 功能描述  : 测试封装帧功能-测试例 入口 上行
 输入参数  : int i  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
void test_pack(int iTest)
{
    ptest_pack   pTest;
    int i = 0;
    int count = 0;
    eMtErr eRet = MT_OK;
    count = sizeof(g_test_pack)/sizeof(sTestPack);
    

    for(i = 0; i < count; i++)
    {
        if(g_test_pack[i].nTestID == iTest)
        {
            pTest = g_test_pack[i].pTest;
            
            if(pTest!= NULL)
            {
                eRet = pTest();
                if(eRet != MT_OK)
                {
                    show_mt_error(eRet);
                }
            }
            
            return;
        }
    }
   
  show_error("尚不支持的命令\n");
}

/*****************************************************************************
 函 数 名  : test_pack_down
 功能描述  : 测试封装帧功能-测试例 入口 下行
 输入参数  : int i  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月22日 星期一
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
void test_pack_down(int iTest)
{
    ptest_pack   pTest;
    int i = 0;
    int count = 0;
    eMtErr eRet = MT_OK;
    count = sizeof(g_test_pack_down)/sizeof(sTestPack);
    

    for(i = 0; i < count; i++)
    {
        if(g_test_pack_down[i].nTestID == iTest)
        {
            pTest = g_test_pack_down[i].pTest;
            
            if(pTest!= NULL)
            {
                eRet = pTest();
                if(eRet != MT_OK)
                {
                    show_mt_error(eRet);
                }
            }
            
            return;
        }
    }
   
    show_error("尚不支持的命令\n");
}

void print_td_d(sMtTd_d *psTd_d)
{
    if(psTd_d)
    {
        printf("Td_d = %02d/%02d/%02d", 
            psTd_d->sT.ucYY,
            psTd_d->sT.ucMM,
            psTd_d->sT.ucDD);
    }
}

void show_app_sub_data(eMtDir eDir,eMtCmd emtCmd, uMtApp *puAppData)
{
    int     i   = 0;
    int     j   = 0;
    eMtErr eRet = MT_OK;
    eMtErr eRet2 = MT_OK;

    sMtCmdInfor sCmdInfo;
    eMtCmd   eCmd = CMD_AFN_F_UNKOWN;
    char *pStr = NULL;
    
    switch(emtCmd)
    {
        case CMD_AFN_0_F3_ONE_BY_ONE:
        {
            sMtOnebyOne * psOnebyOne = (sMtOnebyOne*)puAppData;
            printf("afn = %d\n", psOnebyOne->eAFN);
            printf("Num = %d\n", psOnebyOne->ucNum);
            int i = 0;
            for(i = 0; i < psOnebyOne->ucNum; i++)
            {
                printf("%d:\n", i+1);
                printf("确认命令  = %04X ", psOnebyOne->sOne[i].eCmd);
                eRet = eMtGetCmdInfor(psOnebyOne->sOne[i].eCmd, MT_DIR_S2M, &sCmdInfo);
                
                
                if(MT_OK == eRet)
                {
                   pStr =  sCmdInfo.pName; 
                }
                else
                {
                    eRet2 = eMtGetCmdInfor(psOnebyOne->sOne[i].eCmd, MT_DIR_M2S, &sCmdInfo);
                    if(eRet2 == MT_OK)
                    {
                        pStr =  sCmdInfo.pName; 
                    }
                    else
                    {
                        pStr = "未知命令";
                    }
                }

                show_item_value(pStr);
                //printf("%s\n", pStr);

                printf("\n测量点号  = %d\n", psOnebyOne->sOne[i].usPn);
                printf("确认结果  = %s\n", psOnebyOne->sOne[i].bOk == TRUE ? "确认" : "否认");
            }
            
            
        }
        break;


        case CMD_AFN_4_F1_TML_UP_CFG:
        {
            sMtTmlUpCfg * psTmlUpCfg = (sMtTmlUpCfg*)puAppData;

            printf("ucRTS = %d\n", psTmlUpCfg->ucRTS);
            printf("ucRTM = %d\n", psTmlUpCfg->ucRTM);
            printf("ucReSendTimes = %d\n", psTmlUpCfg->ucReSendTimes);
            printf("ucHeartBeat = %d\n", psTmlUpCfg->ucHeartBeat);
            printf("usTmWaitTimeoutS = %d\n", psTmlUpCfg->usTmWaitTimeoutS);
            printf("bAutoReportAsk_1 = %s\n", psTmlUpCfg->bAutoReportAsk_1 == TRUE ? "true":"false");
            printf("bAutoReportAsk_2 = %s\n", psTmlUpCfg->bAutoReportAsk_2 == TRUE ? "true":"false");
            printf("bAutoReportAsk_3 = %s\n", psTmlUpCfg->bAutoReportAsk_3 == TRUE ? "true":"false");
        }
        break;


        case CMD_AFN_4_F10_TML_POWER_CFG:
        {
            sMtTmlPowerCfg * pData = (sMtTmlPowerCfg*)puAppData;
            char addr[13] = {0};
            char pwd[7] = {0};
            
            printf("usNum = %d\n", pData->usNum);
            for(i = 0; i < pData->usNum; i++)
            {
                printf("---------num %d:--------------\n", i+1);
                printf("busMeterID = %s\n", pData->sOne[i].busMeterID == TRUE ? "true":"false");
                printf("busPn = %s\n", pData->sOne[i].busPn == TRUE ? "true":"false");
                printf("bsBaudPort = %s\n", pData->sOne[i].bsBaudPort == TRUE ? "true":"false");
                printf("beProto = %s\n", pData->sOne[i].beProto == TRUE ? "true":"false");
                printf("bacMeterAddr = %s\n", pData->sOne[i].bacMeterAddr == TRUE ? "true":"false");
                printf("bacPwd = %s\n", pData->sOne[i].bacPwd == TRUE ? "true":"false");
                printf("bucPayRateNum = %s\n", pData->sOne[i].bucPayRateNum == TRUE ? "true":"false");
                printf("bsDigit = %s\n", pData->sOne[i].bsDigit == TRUE ? "true":"false");
                printf("bacGathrAddr = %s\n", pData->sOne[i].bacGathrAddr == TRUE ? "true":"false");
                printf("bsUserClass = %s\n", pData->sOne[i].bsUserClass == TRUE ? "true":"false");


                printf("Data:\n");
                printf("usMeterID = %d\n", pData->sOne[i].usMeterID);
                printf("usPn = %d\n", pData->sOne[i].usPn);
                printf("sBaudPort.eBaudRate = %d\n", pData->sOne[i].sBaudPort.eBaudRate);
                printf("sBaudPort.ucPort = %d\n", pData->sOne[i].sBaudPort.ucPort);
                printf("eProto = %d\n", pData->sOne[i].eProto);

                memcpy(addr, pData->sOne[i].acMeterAddr, 12);
                printf("acMeterAddr = %s\n",addr);
                memcpy(pwd,pData->sOne[i].acPwd, 6);
                printf("acPwd = %s\n",pwd);

                printf("ucPayRateNum = %d\n", pData->sOne[i].ucPayRateNum);
                
                printf("sDigit.ucInteger = %d\n", pData->sOne[i].sDigit.ucInteger);
                printf("sDigit.ucDecimal = %d\n", pData->sOne[i].sDigit.ucDecimal);
                memcpy(addr, pData->sOne[i].acGathrAddr, 12);
                printf("acMeterAddr = %s\n",addr);
                
                printf("sUserClass.ucBig = %d\n", pData->sOne[i].sUserClass.ucBig);
                printf("sUserClass.ucSmall = %d\n", pData->sOne[i].sUserClass.ucSmall);
              

                
            }

           
        }

        break;

        case CMD_AFN_A_F10_TML_POWER_CFG:
        {
            sMtTmlPowerCfg_Q* pData = (sMtTmlPowerCfg_Q*)puAppData;

            printf("usN = %d\n", pData->usN);
            for(i = 0; i < pData->usN; i++)
            {
               printf("usID[%d] = %d\n",i+1,  pData->usID[i]);
              
            }
        }
        break;
                
        case CMD_AFN_4_F38_CFG_ASK_1:
        {
            sMtAsk1Cfg * pData = (sMtAsk1Cfg*)puAppData;

            printf("ucBig = %d\n", pData->ucBig);
            printf("ucM = %d\n", pData->ucM);
            for(i = 0; i < pData->ucM; i++)
            {
               printf("sOne[%d] ucSmall = %d\n",i,  pData->sOne[i].ucSmall);
               printf("sOne[%d] ucFN = %d\n",i,  pData->sOne[i].ucFN);
               for(j = 0; j < pData->sOne[i].ucFN; j++)
               {
                  eCmd = pData->sOne[i].eCmd[j];
                  printf("sOne[%d] eCmd[%d] = 0x%04X\t",i, j, eCmd);
                  eRet = eMtGetCmdInfor(eCmd, MT_DIR_M2S, &sCmdInfo);
                  if(eRet == MT_OK)
                  {
                     printf("%s\n", sCmdInfo.pName);
                  }
                  else
                  {
                     printf("unknown\n");
                  }
               }
            }
        }
        break;

        case CMD_AFN_4_F39_CFG_ASK_2:
        {
            sMtAsk2Cfg * pData = (sMtAsk2Cfg*)puAppData;

            printf("ucBig = %d\n", pData->ucBig);
            printf("ucM = %d\n", pData->ucM);
            for(i = 0; i < pData->ucM; i++)
            {
               printf("sOne[%d] ucSmall = %d\n",i,  pData->sOne[i].ucSmall);
               printf("sOne[%d] ucFN = %d\n",i,  pData->sOne[i].ucFN);
               for(j = 0; j < pData->sOne[i].ucFN; j++)
               {
                  eCmd = pData->sOne[i].eCmd[j];
                  printf("sOne[%d] eCmd[%d] = 0x%04X\t",i, j, eCmd);
                  eRet = eMtGetCmdInfor(eCmd, MT_DIR_M2S, &sCmdInfo);
                  if(eRet == MT_OK)
                  {
                     printf("%s\n", sCmdInfo.pName);
                  }
                  else
                  {
                     printf("unknown\n");
                  }
               }
            }
        }
        break;
        
        case CMD_AFN_A_F38_CFG_ASK_1:
        case CMD_AFN_A_F39_CFG_ASK_2:
        {
            sMtAsk1Cfg_Q* pData = (sMtAsk1Cfg_Q*)puAppData;

            printf("ucBig = %d\n", pData->ucBig);
            printf("ucN = %d\n", pData->ucN);
            for(i = 0; i < pData->ucN; i++)
            {
               printf("ucSmall[%d] = %d\n",i,  pData->ucSmall[i]);
            
            }
        }
        break;
        
        case CMD_AFN_4_F65_CFG_AUTO_1:
        case CMD_AFN_4_F66_CFG_AUTO_2:
        case CMD_AFN_A_F65_CFG_AUTO_1:
        case CMD_AFN_A_F66_CFG_AUTO_2:
        {
            sMtCfgAuto_1 * pData = (sMtCfgAuto_1*)puAppData;
            switch(pData->eUnit)
            {
                case MT_RPT_mm:
                   pStr = "MT_RPT_mm 分钟";
                   break;
               case MT_RPT_HH:
                   pStr = "MT_RPT_HH 小时";
                    break;
                case MT_RPT_DD:
                   pStr = "MT_RPT_DD 天";
                   break;
               case MT_RPT_MM:
                   pStr = "MT_RPT_MM 月";
                    break;    
                default:
                    pStr = "MT_RPT_UNKOWN 不知";
                            break;
            }

            

            printf("eUnit    = %s\n", pStr);
            printf("ucPeriod = %d\n", pData->ucPeriod);
            printf("sTime    = %02d年%02d月%02d日 星期%02d %02d:%02d:%02d\n", 
                    pData->sTime.ucYear,
                    pData->sTime.ucMonth,
                    pData->sTime.ucDay, 
                    pData->sTime.ucWeek,
                    pData->sTime.ucHour,
                    pData->sTime.ucMinute,
                    pData->sTime.ucSecond);
            printf("ucR = %d\n", pData->ucR);
            printf("ucN = %d\n", pData->ucN);
            for(i = 0; i < pData->ucN; i++)
            {
                  printf("sOne[%d] usPn:%d  ",i, pData->sCmdPn[i].usPn);
                 
                  eCmd = pData->sCmdPn[i].eCmd;
                  printf("eCmd:0x%04X\t", eCmd);
                  eRet = eMtGetCmdInfor(eCmd, MT_DIR_M2S, &sCmdInfo);
                  if(eRet == MT_OK)
                  {
                     printf("%s\n", sCmdInfo.pName);
                  }
                  else
                  {
                     printf("unknown\n");
                  } 
            }           
        }
        break;
        
        case CMD_AFN_4_F67_GOP_AUTO_1:
        case CMD_AFN_4_F68_GOP_AUTO_2:    
        case CMD_AFN_A_F67_GOP_AUTO_1:
        case CMD_AFN_A_F68_GOP_AUTO_2:
        {
            sMtGopAuto_1 * pData = (sMtGopAuto_1*)puAppData;
            switch(pData->eGo)
            {
                case MT_AUTO_GO:
                   pStr = "启动";
                   break;
               case MT_AUTO_STOP:
                   pStr = "停止";
                    break;
             
                default:
                    pStr = " 不知";
                            break;
            }

            printf("eGo = %s\n", pStr);
                  
        }
        break;

        
        
        case CMD_AFN_C_F25_POWER_RATE_CUR:
        {
            sMtCurPQUI * pData = (sMtCurPQUI*)puAppData;
            //char addr[13] = {0};
           // char pwd[7] = {0};


            printf("bsReadTime = %s\n", pData->bsReadTime == TRUE ? "true":"false");
            printf("bfP = %s\n", pData->bfP == TRUE ? "true":"false");
            printf("bfPa = %s\n", pData->bfPa == TRUE ? "true":"false");
            printf("bfPb = %s\n", pData->bfPb == TRUE ? "true":"false");
            printf("bfPc = %s\n", pData->bfPc == TRUE ? "true":"false");
            
            printf("bfQ = %s\n", pData->bfQ == TRUE ? "true":"false");
            printf("bfQa = %s\n", pData->bfQa == TRUE ? "true":"false");
            printf("bfQb = %s\n", pData->bfQb == TRUE ? "true":"false");
            printf("bfQc = %s\n", pData->bfQc == TRUE ? "true":"false");
            
            printf("bfPf = %s\n", pData->bfPf == TRUE ? "true":"false");
            printf("bfPfa = %s\n", pData->bfPfa == TRUE ? "true":"false");
            printf("bfPfb = %s\n", pData->bfPfb == TRUE ? "true":"false");
            printf("bfPfc = %s\n", pData->bfPfc == TRUE ? "true":"false");
            
            printf("bfUa = %s\n", pData->bfUa == TRUE ? "true":"false");
            printf("bfUb = %s\n", pData->bfUb == TRUE ? "true":"false");
            printf("bfUc = %s\n", pData->bfUc == TRUE ? "true":"false");
            
            printf("bfIa = %s\n", pData->bfIa == TRUE ? "true":"false");
            printf("bfIb = %s\n", pData->bfIb == TRUE ? "true":"false");
            printf("bfIc = %s\n", pData->bfIc == TRUE ? "true":"false");
            printf("bfI0 = %s\n", pData->bfI0 == TRUE ? "true":"false");
     

            printf("bfS = %s\n", pData->bfS == TRUE ? "true":"false");
            printf("bfSa = %s\n", pData->bfSa == TRUE ? "true":"false");
            printf("bfSb = %s\n", pData->bfSb == TRUE ? "true":"false");
            printf("bfSc = %s\n", pData->bfSc == TRUE ? "true":"false");


            //data
            printf("bsReadTime\n");
            printf("YY= %d\n", pData->sReadTime.ucYY);
            printf("MM= %d\n", pData->sReadTime.ucMM);
            printf("DD= %d\n", pData->sReadTime.ucDD);
            printf("HH= %d\n", pData->sReadTime.ucHH);
            printf("mm= %d\n", pData->sReadTime.ucmm);

            //
            printf("fP = %f\n", pData->fP);
            printf("fPa = %f\n", pData->fPa);
            printf("fPb = %f\n", pData->fPb);
            printf("fPc = %f\n", pData->fPc);
            
            printf("fQ = %f\n", pData->fQ);
            printf("fQa = %f\n", pData->fQa);
            printf("fQb = %f\n", pData->fQb);
            printf("fQc = %f\n", pData->fQc);

            printf("fPf = %f\n", pData->fPf);
            printf("fPfa = %f\n", pData->fPfa);
            printf("fPfb = %f\n", pData->fPfb);
            printf("fPfc = %f\n", pData->fPfc);

            printf("fUa = %f\n", pData->fUa);
            printf("fUb = %f\n", pData->fUb);
            printf("fUc = %f\n", pData->fUc);

            printf("fIa = %f\n", pData->fIa);
            printf("fIb = %f\n", pData->fIb);
            printf("fIc = %f\n", pData->fIc);
            printf("fI0 = %f\n", pData->fI0);
          
            printf("fS = %f\n", pData->fS);
            printf("fSa = %f\n", pData->fSa);
            printf("fSb = %f\n", pData->fSb);
            printf("fSc = %f\n", pData->fSc);

        }
        break;

        case CMD_AFN_5_F31_CHECK_TIME:
        {
            sMtUserClock * pData = (sMtUserClock*)puAppData;
            printf("check time :\n");
            printf("ucYear : %d\n", pData->ucYear);
            printf("ucMonth : %d\n", pData->ucMonth);
            printf("ucWeek : %d\n", pData->ucWeek);
            printf("ucDay : %d\n", pData->ucDay);
            printf("ucHour : %d\n", pData->ucHour);
            printf("ucMinute : %d\n", pData->ucMinute);
            printf("ucSecond : %d\n", pData->ucSecond);

            
        }
        break;


        case CMD_AFN_9_F1_TML_VERSION:
        {   
            char buf[12] = {0};
            sMtTmlVersion * pData = (sMtTmlVersion*)puAppData;

            memcpy(buf, pData->ucFactoryID, 4);
            printf("ucFactoryID = %s\n", buf);
            
            memset(buf, 0x00, sizeof(buf));
            memcpy(buf, pData->ucDeviceID, 8);
            printf("ucDeviceID = %s\n", buf);

            memset(buf, 0x00, sizeof(buf));
            memcpy(buf, pData->ucSoftwareID, 4);
            printf("ucSoftwareID = %s\n", buf);

            memcpy(buf, pData->ucInfoCodeCap, 11);
            printf("ucInfoCodeCap = %s\n", buf);
            
            memset(buf, 0x00, sizeof(buf));
            memcpy(buf, pData->ucProtolID, 8);
            printf("ucProtolID = %s\n", buf);

            memset(buf, 0x00, sizeof(buf));
            memcpy(buf, pData->ucHardWareID, 4);
            printf("ucHardWareID = %s\n", buf);

            printf("sDateSoftware:\n");
            printf("%d年%d月%d日\n", pData->sDateSoftware.ucYY,
                                     pData->sDateSoftware.ucMM,
                                     pData->sDateSoftware.ucDD);

            printf("sDateHardware:\n");
            printf("%d年%d月%d日\n", pData->sDateHardware.ucYY,
                                      pData->sDateHardware.ucMM,
                                      pData->sDateHardware.ucDD);
            
        }
        break;
            


        case CMD_AFN_9_F2_TML_INPUT_OUT:
        {
            sMtTmlInputOut * pData = (sMtTmlInputOut*)puAppData;
            printf("ucPulseInputNum = %d\n",  pData->ucPulseInputNum);
            printf("ucSwitchInputNum = %d\n",  pData->ucSwitchInputNum);
            printf("ucDcNum = %d\n",  pData->ucDcNum);
            printf("ucSwitchOutputNum = %d\n",  pData->ucSwitchOutputNum);
            printf("usReadMeterNum = %d\n",  pData->usReadMeterNum);
            
            printf("usTmlRecvBuf = %d\n",  pData->usTmlRecvBuf);
            printf("usTmlSendBuf = %d\n",  pData->usTmlSendBuf);
            
            printf("sMacAddr = %d-%d-%d-%d-%d-%d\n",  pData->sMacAddr.ucMac[0],
                                                    pData->sMacAddr.ucMac[1],
                                                    pData->sMacAddr.ucMac[2],
                                                    pData->sMacAddr.ucMac[3],
                                                    pData->sMacAddr.ucMac[4],
                                                    pData->sMacAddr.ucMac[5]);
            printf("ucPortNum = %d\n",  pData->ucPortNum);

            for(i = 0; i < pData->ucPortNum; i++)
            {
                printf("port[%d]:\n", i + 1);
                printf("ucPort = %d\n",  pData->sPort[i].ucPort);


                printf("eInter = ");
                switch(pData->sPort[i].eInter)
                {
                    case MT_INTER_RS485:
                        printf("MT_INTER_RS485\n");
                        break;

                    case MT_INTER_RS232:
                        printf("MT_INTER_RS232\n");
                        break;
                        
                    case MT_INTER_SERIW:
                        printf("MT_INTER_SERIW\n");
                        break;

                    default:
                        printf("unkown eInterType\n");
                        break;

                }
                
                printf("bStandard = %s\n",  pData->sPort[i].bStandard == TRUE ? "true" : "false");

                 printf("eUse = ");
                switch(pData->sPort[i].eUse)
                {
                    case MT_PORR_USE_EXP:
                        printf("MT_PORR_USE_EXP\n");
                        break;

                    case MT_PORR_USE_CNG:
                        printf("MT_PORR_USE_CNG\n");
                        break;
                        
                    case MT_PORR_USE_GTH:
                        printf("MT_PORR_USE_GTH\n");
                        break;
                        
                    case MT_PORR_USE_SHR:
                        printf("MT_PORR_USE_SHR\n");
                        break;

                    default:
                        printf("unkown eUse\n");
                        break;

                }

                
                printf("ulBaud = %d\n",  pData->sPort[i].ulBaud);
                printf("usDevNum = %d\n",  pData->sPort[i].usDevNum);
                printf("usRecvBuf = %d\n",  pData->sPort[i].usRecvBuf);
                printf("usSendBuf = %d\n",  pData->sPort[i].usSendBuf);

            }

        }
        break;

        case CMD_AFN_9_F3_TML_OTHER_CFG:
            {
                sMtTmlOtherCfg * pData = (sMtTmlOtherCfg*)puAppData;
                printf("usMp = %d\n", pData->usMp);
                printf("ucGroup = %d\n", pData->ucGroup);
                printf("ucTask = %d\n", pData->ucTask);
                printf("ucTeam = %d\n", pData->ucTeam);
                printf("eFrezMp = %d\n", pData->eFrezMp);
                printf("eFrezP = %d\n", pData->eFrezP);
                printf("eFrezQ = %d\n", pData->eFrezQ);
                printf("eFrezPt = %d\n", pData->eFrezPt);
                printf("eFrezQt = %d\n", pData->eFrezQt);
                printf("ucDay = %d\n", pData->ucDay);
                printf("ucMonth = %d\n", pData->ucMonth);
                printf("ucPCtrl = %d\n", pData->ucPCtrl);
                printf("ucHarm = %d\n", pData->ucHarm);
                printf("ucCapa = %d\n", pData->ucCapa);
                printf("ucVip = %d\n", pData->ucVip);
                for(i = 0; i < 16; i++)
                {
                    printf("bBig[%d] = %s\n",i, pData->bBig[i] == TRUE ? "true" : "false");
                }
                
                for(i = 0; i < 16; i++)
                {
                    printf("ucSmall[%d] = %d\n",i, pData->ucSmall[i]);
                }


            }

            break;


            case CMD_AFN_9_F4_SUPPORT_PARA_CFG:
            {    
                sMtCmdInfor    sCmdInfo = {0};
                sMtSuptParaCfg * pData = (sMtSuptParaCfg*)puAppData;
                printf("support cfg num = %d\n", pData->ucNum);
                for(i = 0; i < pData->ucNum; i++)
                {
                    printf("%d: cmd = 0x%04X\t", i+1, pData->eCfgCmd[i]);
                    eRet = eMtGetCmdInfor(pData->eCfgCmd[i], MT_DIR_M2S, &sCmdInfo);  
                    if(MT_OK == eRet)
                    {
                        printf("%s\n", sCmdInfo.pName);
                    }
                    else
                    {
                       printf("错误的命令字\n");
                    }

                }

            }

            break;
            
            case CMD_AFN_9_F5_SUPPORT_CTRL_CFG:
            {    
                sMtCmdInfor    sCmdInfo = {0};
                sMtSuptCtrlCfg * pData = (sMtSuptCtrlCfg*)puAppData;
                printf("support cfg num = %d\n", pData->ucNum);
                for(i = 0; i < pData->ucNum; i++)
                {
                    printf("%d: cmd = 0x%04X\t", i+1, pData->eCfgCmd[i]);
                    eRet = eMtGetCmdInfor(pData->eCfgCmd[i], MT_DIR_M2S, &sCmdInfo);  
                    if(MT_OK == eRet)
                    {
                        printf("%s\n", sCmdInfo.pName);
                    }
                    else
                    {
                       printf("错误的命令字\n");
                    }
                }
            }

            break;
            
            case CMD_AFN_9_F6_SUPPORT_ASK1_CFG:
            {    
                sMtCmdInfor    sCmdInfo = {0};
                sMtSuptAsk1 * pData = (sMtSuptAsk1*)puAppData;
                printf("support cfg num = %d\n", pData->ucNum);
                for(i = 0; i < pData->ucNum; i++)
                {
                    printf("%d: cmd = 0x%04X\t", i+1, pData->eCfgCmd[i]);
                    eRet = eMtGetCmdInfor(pData->eCfgCmd[i], MT_DIR_M2S, &sCmdInfo);  
                    if(MT_OK == eRet)
                    {
                        printf("%s\n", sCmdInfo.pName);
                    }
                    else
                    {
                       printf("错误的命令字\n");
                    }
                }
            }

            break;

            case CMD_AFN_9_F7_SUPPORT_ASK2_CFG:
            {    
                sMtCmdInfor    sCmdInfo = {0};
                sMtSuptAsk2 * pData = (sMtSuptAsk2*)puAppData;
                printf("support cfg num = %d\n", pData->ucNum);
                for(i = 0; i < pData->ucNum; i++)
                {
                    printf("%d: cmd = 0x%04X\t", i+1, pData->eCfgCmd[i]);
                    eRet = eMtGetCmdInfor(pData->eCfgCmd[i], MT_DIR_M2S, &sCmdInfo);  
                    if(MT_OK == eRet)
                    {
                        printf("%s\n", sCmdInfo.pName);
                    }
                    else
                    {
                       printf("错误的命令字\n");
                    }
                }
            }

            break;

            case CMD_AFN_9_F8_SUPPORT_EVNT_CFG:
               {    
                    sMtSuptEvent * pData = (sMtSuptEvent*)puAppData;
                    printf("support event num = %d\n", pData->ucNum);
                    char *pStr = NULL;
                    for(i = 0; i < pData->ucNum; i++)
                    {
                        printf("%d: event =  %d", i+1, pData->eErc[i]);
                        switch(pData->eErc[i])
                        {
                            case MT_ERC_1_INIT:
                                pStr = "ERC1：数据初始化和版本变更记录";
                                break;

                            case MT_ERC_2_PARA_LOSS:
                                pStr = "ERC2：参数丢失记录";
                                break;


                            case MT_ERC_3_PARA_MODIFY:
                                pStr = "ERC3：参数变更记录";
                                break;

                            case MT_ERC_4_STATE_CHANGE:
                                pStr = "ERC4：状态量变位记录";
                                break;

                            case MT_ERC_5_REMOTE_SWITCH:
                                pStr = "ERC5：遥控跳闸记录";
                                break;

                            case MT_ERC_6_PCTRL_SWITCH:
                                pStr = "ERC6：功控跳闸记录";
                                break;
                                
                            case MT_ERC_7_ECTRL_SWITCH:
                                pStr = "ERC7：电控跳闸记录";
                                break;

                            case MT_ERC_8_METER_CHANGE:
                                pStr = "ERC8：电能表参数变更";
                                break;                    

                            case MT_ERC_9_ELEC_EXCP:
                                pStr = "ERC9：电流回路异常";
                                break;

                            case MT_ERC_10_VOLT_EXCP:
                                pStr = "ERC10：电压回路异常";
                                break;

                            case MT_ERC_11_PHASE_EXCP:
                                pStr = "ERC11：相序异常";
                                break;

                            case MT_ERC_12_METER_TIME:
                                pStr = "ERC12：电能表时间超差";
                                break;

                            case MT_ERC_13_METER_FAULT:
                                pStr = "ERC13：电表故障信息";
                                break;

                            case MT_ERC_14_TML_ON_OFF:
                                pStr = "ERC14：终端停/上电事件";
                                break;

                            case MT_ERC_15_HARM_OVER:
                                pStr = "ERC15：谐波越限告警";
                                break;

                            case MT_ERC_16_DC_OVER:
                                pStr = "ERC16：直流模拟量越限记录";
                                break;

                            case MT_ERC_17_UNBL_OVER:
                                pStr = "ERC17：电压/电流不平衡度越限记录";
                                break;

                            case MT_ERC_18_CAPA_LOCK:
                                pStr = "ERC18：电容器投切自锁记录";
                                break;

                            case MT_ERC_19_BUY_PARA:
                                pStr = "ERC19：购电参数设置记录";
                                break;

                            case MT_ERC_20_AUTH_ERROR:
                                pStr = "ERC20：消息认证错误记录";
                                break;
                                
                            case MT_ERC_21_TML_FAULT:
                                pStr = "ERC21：终端故障记录";
                                break;

                            case MT_ERC_22_HAVE_OVER:
                                pStr = "ERC22：有功总电能量差动越限事件记录";
                                break;

                            case MT_ERC_23_ECTRL_WARN:
                                pStr = "ERC23：电控告警事件记录";
                                break;

                            case MT_ERC_24_VOLT_OVER:
                                pStr = "ERC24：电压越限记录";
                                break;

                            case MT_ERC_25_ELEC_OVER:
                                pStr = "ERC25：电流越限记录";
                                break;

                            case MT_ERC_26_SP_OVER:
                                pStr = "ERC26：视在功率越限记录";
                                break;
                                
                            case MT_ERC_27_POWER_DOWN:
                                pStr = "ERC27：电能表示度下降记录";
                                break;

                            case MT_ERC_28_POWER_OVER:
                                pStr = "ERC28：电能量超差记录";
                                break;

                            case MT_ERC_29_METER_FLY:
                                pStr = "ERC29：电能表飞走记录";
                                break;

                            case MT_ERC_30_METER_STOP:
                                pStr = "ERC30：电能表停走记录";
                                break;

                            case MT_ERC_31_READ_FAULT:
                                pStr = "ERC31：终端485抄表失败事件记录";
                                break;
                                
                            case MT_ERC_32_FLOW_OVER:
                                pStr = "ERC32：终端与主站通信流量超门限事件记录";
                                break;      


                            case MT_ERC_33_METER_WORD:
                                pStr = "ERC33：电能表运行状态字变位事件记录";
                                break;

                            case MT_ERC_34_CT_EXCP:
                                pStr = "ERC34：CT异常事件记录";
                                break;
                                
                            case MT_ERC_35_UNKOWN:
                                pStr = "ERC35：发现未知电表事件记录";
                                break;      

                        default:
                            pStr = "错误的事件记录类型";
                            break;

                        }

                     printf("  %s\n", pStr);  
                    }
                }

                break;
        
            case CMD_AFN_C_F2_TML_CLOCK:
            {
                sMtUserClock * psTmlClock = (sMtUserClock*)puAppData;
                printf("%d年%d月%d日\n",psTmlClock->ucYear,psTmlClock->ucMonth,psTmlClock->ucDay);
                printf("星期 %d \n",psTmlClock->ucWeek);
                printf("%d时%d分%d秒\n",psTmlClock->ucHour,psTmlClock->ucMinute,psTmlClock->ucSecond);
            }
            break;
            
            case CMD_AFN_C_F3_TML_PARA_STATE:
            {
                sMtAfn0cF03 * psTmlClock = (sMtAfn0cF03*)puAppData;
                UINT16  i;
                for(i = 0; i < MT_TERMINAL_STATUS_USER_MAX;i++)
                {
                    printf("stat[%d]  = %d\n",i+1,psTmlClock->bParaStatus[i]);
                }
            }
            break;

            case CMD_AFN_C_F4_TML_UPCOM_STATE:
            {
                sMtAfn0cF04 * psUpComStat = (sMtAfn0cF04*)puAppData;
                printf("主动上报: %s\n",psUpComStat->bUp ? "允许" : "禁止");
                printf("通话: %s\n",psUpComStat->bCom ? "允许" : "禁止");
            }
            break;
            
            case CMD_AFN_C_F5_TML_CTRL_SET:
            {
                sMtAfn0cF05 * psCtrlSet = (sMtAfn0cF05*)puAppData;
                INT32   i,j;
                printf("保电投入状态: %s\n",psCtrlSet->bRemain ? "允许" : "禁止");
                printf("剔除投入状态: %s\n",psCtrlSet->bDelete ? "允许" : "禁止");
                printf("催费投入状态: %s\n",psCtrlSet->bPayWarn ? "允许" : "禁止");

                for ( i = 0 ;i < MT_AFN0CF05_MAXGROUPS;i++)
                {
                    printf("***********************************************************\n");
                    printf("总加组%d: %s\n",i+1,psCtrlSet->bGroup[i] ? "有效" : "无效");
                    if (psCtrlSet->bGroup[i])
                    {
                        printf("总加组%d: 功能定值方案号 %d\n\n",i+1,psCtrlSet->sGroup[i].ucScheme);
                        for (j = 0; j < 8;j++)
                        {
                            printf("总加组%d: 功能时段 %d %s\n",i+1,j+1,psCtrlSet->sGroup[i].bPowerCtrl[j] ? 
                            "有效":"无效");
                        }
                        printf("\n");
                        printf("总加组%d: 时段控 %s\n",i+1,psCtrlSet->sGroup[i].bTimeCtrl ? "有效":"无效");
                        printf("总加组%d: 厂休控 %s\n",i+1,psCtrlSet->sGroup[i].bRestCtrl ? "有效":"无效");
                        printf("总加组%d: 营业报停控 %s\n",i+1,psCtrlSet->sGroup[i].bStopCtrl ? "有效":"无效");
                        printf("总加组%d: 当前功率下浮控 %s\n\n",i+1,psCtrlSet->sGroup[i].bDownCtrl ? "有效":"无效");
                        printf("总加组%d: 月电控 %s\n",i+1,psCtrlSet->sGroup[i].bMonthCtrl ? "有效":"无效");
                        printf("总加组%d: 购电控 %s\n\n",i+1,psCtrlSet->sGroup[i].bBuyCtrl ? "有效":"无效");
                        for (j = 0; j < 8;j++)
                        {
                            printf("总加组%d: 功率轮次 %d %s\n",i+1,j+1,psCtrlSet->sGroup[i].bPowerTimeSta[j] ? 
                            "受控":"不受控");
                        }
                        printf("\n");
                        for (j = 0; j < 8;j++)
                        {
                            printf("总加组%d: 电控轮次 %d %s\n",i+1,j+1,psCtrlSet->sGroup[i].bElecTimeSta[j] ? 
                            "受控":"不受控");
                        }
                    }
                }
            }
            break;

            case CMD_AFN_C_F6_TML_CTRL_STATE:
            {
                sMtAfn0cF06 * psAfn0cF06 = (sMtAfn0cF06*)puAppData;
                INT32   i,j;

                for ( i = 8; i < 8; i++)
                {
                    printf("终端 %d: %s状态\n",i+1,psAfn0cF06->bTrip[i] ? "跳闸" : "合闸");
                }
                printf("\n");
                printf("当前终端 %s催费告警状态\n\n",(psAfn0cF06->ucPayWarn == MT_AFN0CF06_PAYWARNING_ENABLE) ? "处于" : "未处于");
                
                for ( i = 0 ;i < 8;i++)
                {
                    printf("***********************************************************\n");
                    printf("总加组%d: %s\n",i+1,psAfn0cF06->bGroup[i] ? "有效" : "无效");
                    if (psAfn0cF06->bGroup[i])
                    {
                        printf("当前功率定值: %0.1f\n\n",psAfn0cF06->sGroup[i].fCurPower);

                        printf("浮动方向:");
                        switch(psAfn0cF06->sGroup[i].sPowerDrift.eDir)
                        {
                        case MT_FLOAT_DIR_UP:
                            printf(" 上浮\n\n");
                            break;
                        case MT_FLOAT_DIR_DOWN:
                            printf(" 下浮\n\n");
                            break;
                        case MT_FLOAT_DIR_UNKOWN:
                            printf(" 未知浮动方向\n\n");
                            break;
                        default:
                            printf(" 数值错误\n\n"); 
                        }

                        printf("浮动系数:%d\n",psAfn0cF06->sGroup[i].sPowerDrift.ucValue);
                        printf("________________________________________________________\n");
                        for(j = 0; j < 8;j++ )
                        {
                            printf("终端 %d: %s功控跳闸状态\n\n",j+1,psAfn0cF06->sGroup[i].bPowerOutSta[j] ? "处于" : "未处于");
                        }
                        printf("________________________________________________________\n");
                        for(j = 0; j < 8;j++ )
                        {
                            printf("终端 %d: %s月电控跳闸状态\n\n",j+1,psAfn0cF06->sGroup[i].bMonthElecOutSta[j] ? "处于" : "未处于");
                        }
                        printf("________________________________________________________\n");
                        for(j = 0; j < 8;j++ )
                        {
                            printf("终端 %d: %s购电控跳闸状态\n\n",j+1,psAfn0cF06->sGroup[i].bBuyElecOutSta[j] ? "处于" : "未处于");
                        }
                        printf("________________________________________________________\n");
                        printf("终端%s时段控越限告警状态\n\n",psAfn0cF06->sGroup[i].bTimeCtrlSta ? "处于":"未处于");
                        printf("终端%s厂休控越限告警状态\n\n",psAfn0cF06->sGroup[i].bRestCtrlSta ? "处于":"未处于");
                        printf("终端%s营业报停控越限告警状态\n\n",psAfn0cF06->sGroup[i].bStopCtrlSta ? "处于":"未处于");
                        printf("终端%s当前功率下浮控越限告警状态\n\n",psAfn0cF06->sGroup[i].bDownCtrlSta ? "处于":"未处于");
                        printf("________________________________________________________\n");
                        printf("终端%s月电控越限告警状态\n\n",psAfn0cF06->sGroup[i].bMonthCtrlSta ? "处于":"未处于");
                        printf("终端%s购电控越限告警状态\n\n",psAfn0cF06->sGroup[i].bBuyCtrlSta ? "处于":"未处于");
                    }
                }
            }
            break;

            case CMD_AFN_C_F7_TML_EC_VALUE:
            {
                sMtAfn0cF07 * psAfn0cF07 = (sMtAfn0cF07*)puAppData;
                printf("重要事件计数器EC1: %d\n",psAfn0cF07->ucEc1);
                printf("一般事件计数器EC2: %d\n",psAfn0cF07->ucEc2);
            }
            break;

            case CMD_AFN_C_F8_TML_EVNT_FLAG:
            {
                sMtAfn0cF08 *psAfn0cF08 = (sMtAfn0cF08 *)puAppData;
                INT32 i;
                char *pStr;

                for ( i = 0;i < MT_AFN0CF08_ERC_MAX; i++)
                {
                    if(psAfn0cF08->bErc[i])
                    {
                        switch(i+1)
                        {
                            case MT_ERC_1_INIT:
                                pStr = "ERC1：数据初始化和版本变更记录";
                                break;

                            case MT_ERC_2_PARA_LOSS:
                                pStr = "ERC2：参数丢失记录";
                                break;


                            case MT_ERC_3_PARA_MODIFY:
                                pStr = "ERC3：参数变更记录";
                                break;

                            case MT_ERC_4_STATE_CHANGE:
                                pStr = "ERC4：状态量变位记录";
                                break;

                            case MT_ERC_5_REMOTE_SWITCH:
                                pStr = "ERC5：遥控跳闸记录";
                                break;

                            case MT_ERC_6_PCTRL_SWITCH:
                                pStr = "ERC6：功控跳闸记录";
                                break;
                                
                            case MT_ERC_7_ECTRL_SWITCH:
                                pStr = "ERC7：电控跳闸记录";
                                break;

                            case MT_ERC_8_METER_CHANGE:
                                pStr = "ERC8：电能表参数变更";
                                break;                    

                            case MT_ERC_9_ELEC_EXCP:
                                pStr = "ERC9：电流回路异常";
                                break;

                            case MT_ERC_10_VOLT_EXCP:
                                pStr = "ERC10：电压回路异常";
                                break;

                            case MT_ERC_11_PHASE_EXCP:
                                pStr = "ERC11：相序异常";
                                break;

                            case MT_ERC_12_METER_TIME:
                                pStr = "ERC12：电能表时间超差";
                                break;

                            case MT_ERC_13_METER_FAULT:
                                pStr = "ERC13：电表故障信息";
                                break;

                            case MT_ERC_14_TML_ON_OFF:
                                pStr = "ERC14：终端停/上电事件";
                                break;

                            case MT_ERC_15_HARM_OVER:
                                pStr = "ERC15：谐波越限告警";
                                break;

                            case MT_ERC_16_DC_OVER:
                                pStr = "ERC16：直流模拟量越限记录";
                                break;

                            case MT_ERC_17_UNBL_OVER:
                                pStr = "ERC17：电压/电流不平衡度越限记录";
                                break;

                            case MT_ERC_18_CAPA_LOCK:
                                pStr = "ERC18：电容器投切自锁记录";
                                break;

                            case MT_ERC_19_BUY_PARA:
                                pStr = "ERC19：购电参数设置记录";
                                break;

                            case MT_ERC_20_AUTH_ERROR:
                                pStr = "ERC20：消息认证错误记录";
                                break;
                                
                            case MT_ERC_21_TML_FAULT:
                                pStr = "ERC21：终端故障记录";
                                break;

                            case MT_ERC_22_HAVE_OVER:
                                pStr = "ERC22：有功总电能量差动越限事件记录";
                                break;

                            case MT_ERC_23_ECTRL_WARN:
                                pStr = "ERC23：电控告警事件记录";
                                break;

                            case MT_ERC_24_VOLT_OVER:
                                pStr = "ERC24：电压越限记录";
                                break;

                            case MT_ERC_25_ELEC_OVER:
                                pStr = "ERC25：电流越限记录";
                                break;

                            case MT_ERC_26_SP_OVER:
                                pStr = "ERC26：视在功率越限记录";
                                break;
                                
                            case MT_ERC_27_POWER_DOWN:
                                pStr = "ERC27：电能表示度下降记录";
                                break;

                            case MT_ERC_28_POWER_OVER:
                                pStr = "ERC28：电能量超差记录";
                                break;

                            case MT_ERC_29_METER_FLY:
                                pStr = "ERC29：电能表飞走记录";
                                break;

                            case MT_ERC_30_METER_STOP:
                                pStr = "ERC30：电能表停走记录";
                                break;

                            case MT_ERC_31_READ_FAULT:
                                pStr = "ERC31：终端485抄表失败事件记录";
                                break;
                                
                            case MT_ERC_32_FLOW_OVER:
                                pStr = "ERC32：终端与主站通信流量超门限事件记录";
                                break;      


                            case MT_ERC_33_METER_WORD:
                                pStr = "ERC33：电能表运行状态字变位事件记录";
                                break;

                            case MT_ERC_34_CT_EXCP:
                                pStr = "ERC34：CT异常事件记录";
                                break;
                                
                            case MT_ERC_35_UNKOWN:
                                pStr = "ERC35：发现未知电表事件记录";
                                break;      

                        default:
                            pStr = "错误的事件记录类型";
                            break;

                        }
                        printf("%s: 有事件\n",pStr);
                    }
                }
                
            }
            break;
            
            case CMD_AFN_C_F17_TOTAL_POWER_HAVE:
            {
                sMtTotalPowerHave * psU = (sMtTotalPowerHave*)puAppData;
                printf("当前总加有功功率 %f\n",psU->fsXXX);             
                printf("该命令被暂时当成的温度\n");  
            }
            break;

            case CMD_AFN_C_F18_TOTAL_POWER_NONE:
            {
                sMtTotalPowerNone * psU = (sMtTotalPowerNone*)puAppData;
                printf("当前总加无功功率 %f\n",psU->fsXXX);             
                printf("该命令被暂时当成的湿度\n");  
            }
            break;

            
            case CMD_AFN_C_F31_POWER_CUR:
            {
                sMtAfn0cf31 * psAfn0cF31 = (sMtAfn0cf31*)puAppData;
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psAfn0cF31->sTime.ucYY
                                                         ,psAfn0cF31->sTime.ucMM
                                                         ,psAfn0cF31->sTime.ucDD
                                                         ,psAfn0cF31->sTime.ucHH
                                                         ,psAfn0cF31->sTime.ucmm);

                printf("A相:\n\n");
                printf("正向有功电能示值: %.4f\n",psAfn0cF31->sPhaseA.dFrthHavePower);
                printf("反向有功电能示值: %.4f\n",psAfn0cF31->sPhaseA.dBackHavePower);
                printf("组合无功1电能示值: %.2f\n",psAfn0cF31->sPhaseA.fComNonePower1);
                printf("组合无功2电能示值: %.2f\n",psAfn0cF31->sPhaseA.fComNonePower2);

                printf("B相:\n\n");
                printf("正向有功电能示值: %.4f\n",psAfn0cF31->sPhaseB.dFrthHavePower);
                printf("反向有功电能示值: %.4f\n",psAfn0cF31->sPhaseB.dBackHavePower);
                printf("组合无功1电能示值: %.2f\n",psAfn0cF31->sPhaseB.fComNonePower1);
                printf("组合无功2电能示值: %.2f\n",psAfn0cF31->sPhaseB.fComNonePower2);


                printf("C相:\n\n");
                printf("正向有功电能示值: %.4f\n",psAfn0cF31->sPhaseC.dFrthHavePower);
                printf("反向有功电能示值: %.4f\n",psAfn0cF31->sPhaseC.dBackHavePower);
                printf("组合无功1电能示值: %.2f\n",psAfn0cF31->sPhaseC.fComNonePower1);
                printf("组合无功2电能示值: %.2f\n",psAfn0cF31->sPhaseC.fComNonePower2);  
            }
            break;

            case CMD_AFN_C_F57_CURT_HARM_VALUE:
            {
                sMtAfn0cF57 * psAfn0cF57 = (sMtAfn0cF57*)puAppData;
                UINT32  i;
                printf("谐波次数:  %d\n",psAfn0cF57->ucN);
                                                         
                printf("A相谐波电压:\n\n");
                for ( i = 0; i < psAfn0cF57->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF57->sUa.bfXXX_X[i] )
                    {
                        printf("%d次谐波电压有效值: %.1f\n",i+2,psAfn0cF57->sUa.fXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电压有效值: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");

                printf("\nB相谐波电压:\n\n");
                for ( i = 0; i < psAfn0cF57->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF57->sUb.bfXXX_X[i] )
                    {
                        printf("%d次谐波电压有效值: %.2f\n",i+2,psAfn0cF57->sUb.fXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电压有效值: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
                printf("\nC相谐波电压:\n\n");
                for ( i = 0; i < psAfn0cF57->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF57->sUc.bfXXX_X[i] )
                    {
                        printf("%d次谐波电压有效值: %.2f\n",i+2,psAfn0cF57->sUc.fXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电压有效值: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
                printf("\nA相谐波电流:\n\n");
                for ( i = 0; i < psAfn0cF57->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF57->sIa.bfsXX_XX[i] )
                    {
                        printf("%d次谐波电流有效值: %.2f\n",i+2,psAfn0cF57->sIa.fsXX_XX[i]);
                    }else
                    {
                        printf("%d次谐波电流: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
                printf("\nB相谐波电流:\n\n");
                for ( i = 0; i < psAfn0cF57->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF57->sIb.bfsXX_XX[i] )
                    {
                        printf("%d次谐波电流有效值: %.2f\n",i+2,psAfn0cF57->sIb.fsXX_XX[i]);
                    }else
                    {
                        printf("%d次谐波电流: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
                printf("\nC相谐波电流:\n\n");
                for ( i = 0; i < psAfn0cF57->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF57->sIc.bfsXX_XX[i] )
                    {
                        printf("%d次谐波电流有效值: %.2f\n",i+2,psAfn0cF57->sIc.fsXX_XX[i]);
                    }else
                    {
                        printf("%d次谐波电流: 无\n",i+2);
                    }
                }  
            }
            break;
            
            case CMD_AFN_C_F58_CURT_HARM_RATE:
            {
                sMtAfn0cF58 * psAfn0cF58 = (sMtAfn0cF58*)puAppData;
                UINT32  i;
                printf("谐波次数:  %d\n",psAfn0cF58->ucN);
                                                         
                printf("A相谐波电压:\n\n");
                if( TRUE == psAfn0cF58->sUa.sTotal.bfsXXX_X )
                {
                    printf("总谐波电压含有率: %.1f\n",psAfn0cF58->sUa.sTotal.fsXXX_X);
                }else
                {
                    printf("总谐波电压含有率: 无\n");
                }
                for ( i = 0; i < psAfn0cF58->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF58->sUa.bfsXXX_X[i] )
                    {
                        printf("%d次谐波电压含有率: %.1f\n",i+2,psAfn0cF58->sUa.fsXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电压含有率: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");

                printf("\nB相谐波电压:\n\n");
                if( TRUE == psAfn0cF58->sUb.sTotal.bfsXXX_X )
                {
                    printf("总谐波电压含有率: %.1f\n",psAfn0cF58->sUb.sTotal.fsXXX_X);
                }else
                {
                    printf("总谐波电压含有率: 无\n");
                }
                for ( i = 0; i < psAfn0cF58->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF58->sUb.bfsXXX_X[i] )
                    {
                        printf("%d次谐波电压含有率: %.2f\n",i+2,psAfn0cF58->sUb.fsXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电压含有率: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
                printf("\nC相谐波电压:\n\n");
                if( TRUE == psAfn0cF58->sUc.sTotal.bfsXXX_X )
                {
                    printf("总谐波电压含有率: %.1f\n",psAfn0cF58->sUc.sTotal.fsXXX_X);
                }else
                {
                    printf("总谐波电压含有率: 无\n");
                }                
                for ( i = 0; i < psAfn0cF58->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF58->sUc.bfsXXX_X[i] )
                    {
                        printf("%d次谐波电压含有率: %.2f\n",i+2,psAfn0cF58->sUc.fsXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电压含有率: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
                printf("\nA相谐波电流:\n\n");
                if( TRUE == psAfn0cF58->sIa.sTotal.bfsXXX_X )
                {
                    printf("总谐波电压含有率: %.1f\n",psAfn0cF58->sIa.sTotal.fsXXX_X);
                }else
                {
                    printf("总谐波电压含有率: 无\n");
                }                
                for ( i = 0; i < psAfn0cF58->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF58->sIa.bfsXXX_X[i] )
                    {
                        printf("%d次谐波电流含有率: %.2f\n",i+2,psAfn0cF58->sIa.fsXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电流含有率: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
                printf("\nB相谐波电流:\n\n");
                if( TRUE == psAfn0cF58->sIb.sTotal.bfsXXX_X )
                {
                    printf("总谐波电压含有率: %.1f\n",psAfn0cF58->sIb.sTotal.fsXXX_X);
                }else
                {
                    printf("总谐波电压含有率: 无\n");
                }                
                for ( i = 0; i < psAfn0cF58->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF58->sIb.bfsXXX_X[i] )
                    {
                        printf("%d次谐波电流含有率: %.2f\n",i+2,psAfn0cF58->sIb.fsXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电流含有率: 无\n",i+2);
                    }
                }
                printf("*****************************************************\n");
           
                printf("\nC相谐波电流:\n\n");
                if( TRUE == psAfn0cF58->sIc.sTotal.bfsXXX_X )
                {
                    printf("总谐波电压含有率: %.1f\n",psAfn0cF58->sIc.sTotal.fsXXX_X);
                }else
                {
                    printf("总谐波电压含有率: 无\n");
                }     
                for ( i = 0; i < psAfn0cF58->ucN - 1;i++)
                {
                    if( TRUE == psAfn0cF58->sIc.bfsXXX_X[i] )
                    {
                        printf("%d次谐波电流含有率: %.2f\n",i+2,psAfn0cF58->sIc.fsXXX_X[i]);
                    }else
                    {
                        printf("%d次谐波电流含有率: 无\n",i+2);
                    }
                }  
            }
            break;  

             case CMD_AFN_C_F73_DC_RLTM_DATA:
            {
                float *pData = (float*)puAppData;
                printf("Dc = %f\n", *pData);
             }
             break;
            case CMD_AFN_C_F129_FRTH_HAVE_POWR_C:
            {
                sMtAfn0cf129 * psAfn0cF129 = (sMtAfn0cf129*)puAppData;
                UINT8 i;
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psAfn0cF129->sTime.ucYY
                                                         ,psAfn0cF129->sTime.ucMM
                                                         ,psAfn0cF129->sTime.ucDD
                                                         ,psAfn0cF129->sTime.ucHH
                                                         ,psAfn0cF129->sTime.ucmm);

                printf("费率数: %d\n",psAfn0cF129->ucM);
                printf("正向有功总电能示值: %.4f\n",psAfn0cF129->dTotalValue);
                for (i = 0; i < psAfn0cF129->ucM;i++)
                {
                    printf("费率 %d 正向有功总电能示值: %.4f\n",i+1,psAfn0cF129->dValueItem[i]);
                }
            }
            break;

            case CMD_AFN_C_F130_FRTH_NONE_POWR_C:
            {
                sMtAfn0cf130 * psAfn0cF130 = (sMtAfn0cf130*)puAppData;
                UINT8 i;
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psAfn0cF130->sTime.ucYY
                                                         ,psAfn0cF130->sTime.ucMM
                                                         ,psAfn0cF130->sTime.ucDD
                                                         ,psAfn0cF130->sTime.ucHH
                                                         ,psAfn0cF130->sTime.ucmm);

                printf("费率数: %d\n",psAfn0cF130->ucM);
                printf("正向无功总电能示值: %.2f\n",psAfn0cF130->dTotalValue);
                for (i = 0; i < psAfn0cF130->ucM;i++)
                {
                    printf("费率 %d 正向无功总电能示值: %.2f\n",i+1,psAfn0cF130->dValueItem[i]);
                }
            }
            break;

            case CMD_AFN_C_F166_METR_MODIFY_RECD:
            {
                sMtAfn0cf166 * psAfn0cF166 = (sMtAfn0cf166*)puAppData;
               
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psAfn0cF166->sTime.ucYY
                                                         ,psAfn0cF166->sTime.ucMM
                                                         ,psAfn0cF166->sTime.ucDD
                                                         ,psAfn0cF166->sTime.ucHH
                                                         ,psAfn0cF166->sTime.ucmm);

               printf("电能表时钟修改次数 %d\n",psAfn0cF166->sOne.usTimes);
               printf("电能表最近一次时钟修改时间 %d年%d月%d日 %d时%d分\n",psAfn0cF166->sOne.sClock.ucYY
                                                                          ,psAfn0cF166->sOne.sClock.ucMM
                                                                          ,psAfn0cF166->sOne.sClock.ucDD
                                                                          ,psAfn0cF166->sOne.sClock.ucHH
                                                                          ,psAfn0cF166->sOne.sClock.ucmm);

               printf("电能表时段参数修改次数 %d\n",psAfn0cF166->sOther.usTimes);
               printf("电能表最近一次时段参数修改时间 %d年%d月%d日 %d时%d分\n",psAfn0cF166->sOther.sClock.ucYY
                                                                              ,psAfn0cF166->sOther.sClock.ucMM
                                                                              ,psAfn0cF166->sOther.sClock.ucDD
                                                                              ,psAfn0cF166->sOther.sClock.ucHH
                                                                              ,psAfn0cF166->sOther.sClock.ucmm);
            }
            break;

            case CMD_AFN_C_F167_METR_BUY_USE:
            {
                sMtAfn0cf167 * psAfn0cF167 = (sMtAfn0cf167*)puAppData;
     
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psAfn0cF167->sTime.ucYY
                                                         ,psAfn0cF167->sTime.ucMM
                                                         ,psAfn0cF167->sTime.ucDD
                                                         ,psAfn0cF167->sTime.ucHH
                                                         ,psAfn0cF167->sTime.ucmm);

               printf("购电次数 %d\n",psAfn0cF167->usBuyTimes);
               printf("剩余金额 %.04f\n",psAfn0cF167->dCashLeft);
               printf("累计购电金额 %.04f\n",psAfn0cF167->dCashTotal);
               printf("剩余电量 %.02f\n",psAfn0cF167->dElecLeft);
               printf("透支电量 %.02f\n",psAfn0cF167->dElecOut);
               printf("累计购电量 %.02f\n",psAfn0cF167->dElecTotal);
               printf("赊欠门限电量 %.02f\n",psAfn0cF167->dElecLimit);
               printf("报警电量 %.02f\n",psAfn0cF167->dElecWarn);
               printf("故障电量 %.02f\n",psAfn0cF167->dElecFault);
             }
            break;  

            case CMD_AFN_D_F97_CURVE_FRTH_HAVE:
            {
                sMtAfn0dF97 *psU = (sMtAfn0dF97*)puAppData;
                UINT8       i;
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTd_c.sT.ucYY
                                                         ,psU->sTd_c.sT.ucMM
                                                         ,psU->sTd_c.sT.ucDD
                                                         ,psU->sTd_c.sT.ucHH
                                                         ,psU->sTd_c.sT.ucmm);

                for (i = 0; i < psU->sTd_c.ucN;i++)
                {
                    printf("有功电能量%d  = %0.4f\n",i+1,psU->dEnergy[i]);
                }
            }
            break;

            case CMD_AFN_D_F101_CURVE_FRTH_HAVE:
            {
                sMtAfn0dF101 *psU = (sMtAfn0dF101*)puAppData;
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTd_c.sT.ucYY
                                                         ,psU->sTd_c.sT.ucMM
                                                         ,psU->sTd_c.sT.ucDD
                                                         ,psU->sTd_c.sT.ucHH
                                                         ,psU->sTd_c.sT.ucmm);

                for (i = 0; i < psU->sTd_c.ucN;i++)
                {
                    printf("有功电能量%d  = %0.2f\n",i+1,psU->dValue[i]);
                }
            }
            break;

            case CMD_AFN_D_F105_CURVE_FACTOR_T:
            {
                sMtAfn0dF105 *psU = (sMtAfn0dF105*)puAppData;
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTd_c.sT.ucYY
                                                         ,psU->sTd_c.sT.ucMM
                                                         ,psU->sTd_c.sT.ucDD
                                                         ,psU->sTd_c.sT.ucHH
                                                         ,psU->sTd_c.sT.ucmm);

                for (i = 0; i < psU->sTd_c.ucN;i++)
                {
                    printf("测试点功率因数%d  = %0.1f\n",i+1,psU->fFactor[i]);
                }
            }
            break;
            
            case CMD_AFN_D_F109_ANGLE_CURVE_VOLT:
            {
                sMtAfn0dF109 *psU = (sMtAfn0dF109*)puAppData;
                UINT8       i;
                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTd_c.sT.ucYY
                                                         ,psU->sTd_c.sT.ucMM
                                                         ,psU->sTd_c.sT.ucDD
                                                         ,psU->sTd_c.sT.ucHH
                                                         ,psU->sTd_c.sT.ucmm);

                for (i = 0; i < psU->sTd_c.ucN;i++)
                {
                    printf("相位角%d A = %0.1f\n",i+1,psU->sPhase[i].fPhaseA);
                    printf("相位角%d B = %0.1f\n",i+1,psU->sPhase[i].fPhaseA);
                    printf("相位角%d C = %0.1f\n",i+1,psU->sPhase[i].fPhaseA);
                }
            }
            break;

            case CMD_AFN_D_F113_ELEC_HARM_TIME_A:
            {
                sMtAfn0dF113 *psU = (sMtAfn0dF113*)puAppData;
                printf("抄表时间 %d年%d月%d日\n" ,psU->sTd_d.sT.ucYY
                                             ,psU->sTd_d.sT.ucMM
                                             ,psU->sTd_d.sT.ucDD);

                for (i = 0; i < MT_AFN0DF113_COUNT - 1;i++)
                {
                    printf("A相 %d次谐波电流日最大值 %.2f\n",i+2,psU->sHarmTime[i].fHarmI);
                    printf("A相 %d次谐波电流发生在 %d月%d日  %d时%d分\n",i+2
                                                                        ,psU->sHarmTime[i].sTime.ucMM
                                                                        ,psU->sHarmTime[i].sTime.ucDD
                                                                        ,psU->sHarmTime[i].sTime.ucHH
                                                                        ,psU->sHarmTime[i].sTime.ucmm);
                    
                }

                 printf("A相总畸变电流日最大值 %.2f\n",psU->sHarmTime[i].fHarmI);
                 printf("A相总畸变电流发生在 %d月%d日  %d时%d分\n" ,psU->sHarmTime[i].sTime.ucMM
                                                                   ,psU->sHarmTime[i].sTime.ucDD
                                                                   ,psU->sHarmTime[i].sTime.ucHH
                                                                   ,psU->sHarmTime[i].sTime.ucmm);
            }
            break;

            case CMD_AFN_D_F116_VOLT_HARM_TIME_A:
            {
                sMtAfn0dF116 *psU = (sMtAfn0dF116*)puAppData;
                printf("抄表时间 %d年%d月%d日\n" ,psU->sTd_d.sT.ucYY
                                             ,psU->sTd_d.sT.ucMM
                                             ,psU->sTd_d.sT.ucDD);

                for (i = 0; i < MT_AFN0DF113_COUNT - 1;i++)
                {
                    printf("A相 %d次谐波电压日最大值 %.1f\n",i+2,psU->sHarmTime[i].fHarmU);
                    printf("A相 %d次谐波电压发生在 %d月%d日  %d时%d分\n",i+2
                                                                        ,psU->sHarmTime[i].sTime.ucMM
                                                                        ,psU->sHarmTime[i].sTime.ucDD
                                                                        ,psU->sHarmTime[i].sTime.ucHH
                                                                        ,psU->sHarmTime[i].sTime.ucmm);
                    
                }

                 printf("A相总畸变电压日最大值 %.1f\n",psU->sHarmTime[i].fHarmU);
                 printf("A相总畸变电压发生在 %d月%d日  %d时%d分\n" ,psU->sHarmTime[i].sTime.ucMM
                                                                   ,psU->sHarmTime[i].sTime.ucDD
                                                                   ,psU->sHarmTime[i].sTime.ucHH
                                                                   ,psU->sHarmTime[i].sTime.ucmm);
            }
            break;

            case CMD_AFN_D_F121_HARM_OVER_A:
            {
                sMtAfn0dF121 *psU = (sMtAfn0dF121*)puAppData;
                printf("抄表时间 %d年%d月%d日\n" ,psU->sTd_d.sT.ucYY
                                             ,psU->sTd_d.sT.ucMM
                                             ,psU->sTd_d.sT.ucDD);
                                             

                printf("谐波次数 %d\n",psU->ucN);
                printf("++++++++++++++++++++++++++++++++++++++++++++++++++++++++++\n");        
                printf("总畸变电压含有率越限日累计时间 %d\n",psU->usUAbnormal);
                for(i = 0; i < psU->ucN;i++)
                {
                    printf("%d 次谐波电压含有率越限日累计时间 %d\n",i+2,psU->usUHarm[i]);
                }
                
                printf("\n总畸变电流含有率越限日累计时间 %d\n",psU->usIAbnormal);
                for(i = 0; i < psU->ucN;i++)
                {
                    printf("%d 次谐波电流含有率越限日累计时间 %d\n",i+2,psU->usIHarm[i]);
                }
            }
            break;

            case CMD_AFN_D_F169_HAVE_FRTH_R:
            {
                sMtAfn0dF169 *psU = (sMtAfn0dF169*)puAppData;
                printf("冻结时标 %d年%d月\n" ,psU->sTd_m.sT.ucYY
                                             ,psU->sTd_m.sT.ucMM);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                                                         
                printf("A相正向有功电能示值 %.4f\n",psU->dValueA);
                printf("B相正向有功电能示值 %.4f\n",psU->dValueB);
                printf("C相正向有功电能示值 %.4f\n",psU->dValueC);
                
            }
            break;

            case CMD_AFN_D_F170_NONE_FRTM_R:
            {
                sMtAfn0dF170 *psU = (sMtAfn0dF170*)puAppData;
                printf("冻结时标 %d年%d月%d日\n" ,psU->sTd_d.sT.ucYY
                                                 ,psU->sTd_d.sT.ucMM
                                                 ,psU->sTd_d.sT.ucDD);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                printf("费率数 %d\n",psU->ucM);
                for(i = 0; i < psU->ucM; i++)
                {
                    printf("费率 %d 正向无功(组合无功)电能示值 %.2f\n",i+1,psU->dValue[i]);
                }
            }
            break;

            case CMD_AFN_D_F171_HAVE_BACK_R:
            {
                sMtAfn0dF171 *psU = (sMtAfn0dF171*)puAppData;
                printf("冻结时标 %d年%d月%d日\n" ,psU->sTd_d.sT.ucYY
                                                 ,psU->sTd_d.sT.ucMM
                                                 ,psU->sTd_d.sT.ucDD);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                printf("费率数 %d\n",psU->ucM);
                for(i = 0; i < psU->ucM; i++)
                {
                    printf("费率 %d 有功电能示值 %.4f\n",i+1,psU->dValue[i]);
                }
            }
            break;

            case CMD_AFN_D_F177_HAVE_FRTH_M:
            {
                sMtAfn0dF177 *psU = (sMtAfn0dF177*)puAppData;
                printf("冻结时标 %d年%d月\n" ,psU->sTd_m.sT.ucYY
                                             ,psU->sTd_m.sT.ucMM);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                printf("费率数 %d\n",psU->ucM);
                for(i = 0; i < psU->ucM; i++)
                {
                    printf("费率 %d 有功电能示值 %.4f\n",i+1,psU->dValue[i]);
                }
            }
            break;

            case CMD_AFN_D_F178_NONE_FRTM_M:
            {
                sMtAfn0dF178 *psU = (sMtAfn0dF178*)puAppData;
                UINT8       i;
                printf("冻结时标 %d年%d月\n" ,psU->sTd_m.sT.ucYY
                                             ,psU->sTd_m.sT.ucMM);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                printf("费率数 %d\n",psU->ucM);
                for(i = 0; i < psU->ucM; i++)
                {
                    printf("费率 %d 无功(组合无功1)电能示值 %.2f\n",i+1,psU->dValue[i]);
                }
            }
            break;

            case CMD_AFN_D_F185_HAVE_DMND_FRTH_D:
            {
                sMtAfn0dF185 *psU = (sMtAfn0dF185*)puAppData;
                printf("冻结时标 %d年%d月%d日\n" ,psU->sTd_d.sT.ucYY
                                                 ,psU->sTd_d.sT.ucMM
                                                 ,psU->sTd_d.sT.ucDD);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                printf("费率数 %d\n",psU->ucM);
                for(i = 0; i < psU->ucM + 1; i++)
                {
                    printf("最大需量 %.4f\n",psU->sMaxClock[i].fMax);
                    printf("最大需量发生时间 %d月%d日%d时%d分\n",psU->sMaxClock[i].sClock.ucMM
                                                                ,psU->sMaxClock[i].sClock.ucDD
                                                                ,psU->sMaxClock[i].sClock.ucHH
                                                                ,psU->sMaxClock[i].sClock.ucmm);
                    
                }
            }
            break;

            case CMD_AFN_D_F215_METR_BUY_USR:
            {
                sMtAfn0dF215 *psU = (sMtAfn0dF215*)puAppData;
                printf("冻结时标 %d年%d月\n" ,psU->sTd_m.sT.ucYY
                                             ,psU->sTd_m.sT.ucMM);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                printf("购电次数 %d\n",psU->usBuyTimes);
                printf("剩余金额 %0.4f\n",psU->dCashLeft);
                printf("累计购电金额 %0.4f\n",psU->dCashTotal);
                printf("剩余电量 %0.2f\n",psU->dKilowattLeft);
                printf("透支电量 %0.2f\n",psU->dKilowattOver);
                printf("累计电量 %0.2f\n",psU->dKilowattTotal);
                printf("赊欠门电量 %0.2f\n",psU->dKilowattDebit);
                printf("告警电量 %0.2f\n",psU->dKilowattWarn);
                printf("故障电量 %0.2f\n",psU->dKilowattFault);
                
            }
            break;

            case CMD_AFN_D_F216_METR_BALANCE:
            {
                sMtAfn0dF216 *psU = (sMtAfn0dF216*)puAppData;
                printf("冻结时标 %d年%d月\n" ,psU->sTd_m.sT.ucYY
                                             ,psU->sTd_m.sT.ucMM);
                                             

                printf("抄表时间 %d年%d月%d日 %d时%d分\n",psU->sTime.ucYY
                                                         ,psU->sTime.ucMM
                                                         ,psU->sTime.ucDD
                                                         ,psU->sTime.ucHH
                                                         ,psU->sTime.ucmm);
                printf("已结有功功率总电能 %0.4f\n",psU->dTotalBalance);
                for(i = 0;i < psU->ucM;i++)
                {
                    printf("已结费率 %d有功功率总电能 %0.4f\n",i+1,psU->dBalance[i]); 
                }
                
                printf("\n未结有功功率总电能 %0.4f\n",psU->dTotalNoBalance);
                for(i = 0;i < psU->ucM;i++)
                {
                    printf("未结费率 %d有功功率总电能 %0.4f\n",i+1,psU->dNoBalance[i]); 
                }
                
                printf("\n故障电量 %0.4f\n",psU->dFault);
            }
            break;


            case CMD_AFN_D_F28_UBLN_OVER_D:
            {
                if(eDir == MT_DIR_S2M)
                {
                    sMtUblnOverD* psU = (sMtUblnOverD*)puAppData;
                    print_td_d(&psU->sTd_d);
                    printf("usCalElec = %d\n", psU->usCalElec);
                    printf("usCalVolt = %d\n", psU->usCalVolt);
                    printf("fMaxElec  = %f\n", psU->fMaxElec);
                    printf("sTimeMaxElec(DDHHmm) = %02d_%02d_%02d\n", 
                                               psU->sTimeMaxElec.ucDD,
                                               psU->sTimeMaxElec.ucHH,
                                               psU->sTimeMaxElec.ucmm);
                    printf("fMaxVolt  = %f\n", psU->fMaxVolt);
                    printf("sTimeMaxVolt(DDHHmm) = %02d_%02d_%02d\n", 
                                               psU->sTimeMaxVolt.ucDD,
                                               psU->sTimeMaxVolt.ucHH,
                                               psU->sTimeMaxVolt.ucmm);
                }
                else
                {
                   sMtTd_d * psU = (sMtTd_d*)puAppData;
                   print_td_d(psU);

                }
     
               
             }
            break;  

#if 0

            case CMD_AFN_4_FN_33_CFG_NOT_ELEC:
            {
                sMtCfgNotElec *psUser  =(sMtCfgNotElec*)puAppData;

                switch(psUser->eType)
                {
                    case MT_NELEC_TEMP:
                        pStr = "MT_NELEC_TEMP 温度";
                        break;
                    case MT_NELEC_FLOW:
                        pStr = "MT_NELEC_FLOW 流量";
                        break;
                    case MT_NELEC_FLOW_V:
                        pStr = "MT_NELEC_FLOW_V 流速";
                        break;
                    case MT_NELEC_PRESS:
                        pStr = "MT_NELEC_PRESS 压力";
                        break;
                    case MT_NELEC_HUMID:
                        pStr = "MT_NELEC_HUMID 湿度";
                        break;
                    case MT_NELEC_ILLUM:
                        pStr = "MT_NELEC_ILLUM 照度";
                        break;
                    case MT_NELEC_ROTATE:
                        pStr = "MT_NELEC_ROTATE 转速";
                        break;
                    default:
                        pStr = "未知类型";
                        break;
                }
    
                printf("eType =  %s\n", pStr);
                printf("dMin  =  %lf\n", psUser->dMin);
                printf("dMax  =  %lf\n", psUser->dMax);
            }
            break;

            case CMD_AFN_C_FN_90_NOT_ELEC:
                {
                    sMtNotElec *psUser  =(sMtNotElec*)puAppData;
                    switch(psUser->eType)
                    {
                        case MT_NELEC_TEMP:
                            pStr = "MT_NELEC_TEMP 温度";
                            break;
                        case MT_NELEC_FLOW:
                            pStr = "MT_NELEC_FLOW 流量";
                            break;
                        case MT_NELEC_FLOW_V:
                            pStr = "MT_NELEC_FLOW_V 流速";
                            break;
                        case MT_NELEC_PRESS:
                            pStr = "MT_NELEC_PRESS 压力";
                            break;
                        case MT_NELEC_HUMID:
                            pStr = "MT_NELEC_HUMID 湿度";
                            break;
                        case MT_NELEC_ILLUM:
                            pStr = "MT_NELEC_ILLUM 照度";
                            break;
                        case MT_NELEC_ROTATE:
                            pStr = "MT_NELEC_ROTATE 转速";
                            break;
                        default:
                            pStr = "未知类型";
                            break;
                    }
    
                    printf("Type  =  %s\n",  pStr);
                    printf("Value =  %lf\n", psUser->dValue);

                }
                break;

          #endif


    

            default:
            {
                show_error("没有数据域,或未实现显示\n");
            }

            break;





    }
}
/*****************************************************************************
 函 数 名  : show_app_data
 功能描述  : 显示应用层数据
 输入参数  : smtSub *pSub  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月27日 星期六
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
void show_app_data_adp(eMtDir eDir, sCmSub *pSub)
{
    if(!pSub)
    {
        return;
    }

    sMtCmdInfor    sCmdInfor   = {0};
    eMtErr         eRet        = MT_OK;    
   // pMtFunc        pFunc       = NULL;
    char *         pStr        = NULL;
    eMtCmd         emtCmd  ;

    pStr = NULL;
    
   emtCmd =  eGetMtCmd(pSub->eCmd);
   // sub cmd
   printf("sCmd  = %04X\t", emtCmd);
   show_sub_name("子命令ID\t");
   eRet = eMtGetCmdInfor(emtCmd, eDir, &sCmdInfor);
   if(eRet != MT_OK)
   {
        show_error("错误的命令类型!");
        return;
   }
   else
   {
        show_item_value(sCmdInfor.pName);
   }
   printf("\n");

   //pn 
   printf("usPN  = %d\t", pSub->usPN);
   show_sub_name("信息点PN\t");
   switch(sCmdInfor.ePn)
   {
       case MT_PN_P0:
        show_item_value("PN无关");
        break;
       case MT_PN_TM:
        show_item_value("终端号");
        break;
       case MT_PN_MP:
        show_item_value("测量点号");
        break;
       case MT_PN_GT:
        show_item_value("总加组号");
        break;
       case MT_PN_CT:
        show_item_value("控制轮次");
        break;
       case MT_PN_TK:
        show_item_value("任务号");
        break;
       case MT_PN_DP:
        show_item_value("直流模拟量端口号");
        break;
        
       default:
        show_error("未知PN 类型");
        break;
   }

    if( pSub->bApp != TRUE)
    {
        show_sub_name("\n无数据单元\t");
    }
  
    
    // 应用层数据
    if(sCmdInfor.pFunc == NULL)
    {
       show_item_value("无数据单元\n");
       return;

    }

    show_main_name("\n数据单元:\n");
    // 有应用层数据

    char buf[1024] = {0};
    uMtApp *pmtAppData = (uMtApp *)buf;
    emtTransAppData_cm_2_mt(emtCmd, &(pSub->uAppData), pmtAppData);
    show_app_sub_data(eDir, emtCmd, pmtAppData);
}





/*****************************************************************************
 函 数 名  : show_app_data
 功能描述  : 显示应用层数据
 输入参数  : smtSub *pSub  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月27日 星期六
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
void show_app_data(eMtDir eDir, sMtData *pSub)
{
   if(!pSub)
   {
      return;
   }


    sMtCmdInfor    sCmdInfor   = {0};
    eMtErr         eRet        = MT_OK;    
    // pMtFunc   pFunc       = NULL;
    //char *         pStr        = NULL;

   
   // sub cmd
   printf("sCmd  = %04X\t", pSub->eCmd);
   show_sub_name("子命令ID\t");
   eRet = eMtGetCmdInfor(pSub->eCmd, eDir, &sCmdInfor);
   if(eRet != MT_OK)
   {
        show_error("错误的命令类型!");
        return;
   }
   else
   {
        show_item_value(sCmdInfor.pName);
   }
   printf("\n");

   //pn 
   printf("usPN  = %d\t", pSub->usPN);
   show_sub_name("信息点PN\t");
   switch(sCmdInfor.ePn)
   {
       case MT_PN_P0:
        show_item_value("PN无关");
        break;
       case MT_PN_TM:
        show_item_value("终端号");
        break;
       case MT_PN_MP:
        show_item_value("测量点号");
        break;
       case MT_PN_GT:
        show_item_value("总加组号");
        break;
       case MT_PN_CT:
        show_item_value("控制轮次");
        break;
       case MT_PN_TK:
        show_item_value("任务号");
        break;
       case MT_PN_DP:
        show_item_value("直流模拟量端口号");
        break;
        
       default:
        show_error("未知PN 类型");
        break;
   }
   

      // 应用层数据
    if(sCmdInfor.pFunc == NULL)
    {
       show_item_value("无数据单元\n");
       return;

    }
    
    // 应用层数据
    if(sCmdInfor.pFunc == NULL)
    {
       show_item_value("无数据单元\n");
       return;

    }

    show_main_name("\n数据单元:\n");
    // 有应用层数据
    show_app_sub_data(eDir, pSub->eCmd, &(pSub->uApp));
    
}


void show_pack_adp(sCmPacket * psUnpack)
{
    if(!psUnpack)
    {
        printf("psUnpack is null\n");
        return;
    }

   
   // char *str = NULL;
    char str_reg[5] = {0};
    char strTmp[128] = {0};
    int i = 0;
    int nSubNum = 0;
    eMtDir eDir = MT_DIR_M2S;

    // 输出地址信息
    show_main_name("address:\n");
    memcpy(str_reg,psUnpack->sAddress.acRegionCode, 4);
    /*
    printf("reg_code\t= %c%c%c%c\t",
            psUnpack->sAddress.acRegionCode[0],
            psUnpack->sAddress.acRegionCode[1],
            psUnpack->sAddress.acRegionCode[2],
            psUnpack->sAddress.acRegionCode[3]);
            */
    printf("reg  = %s\t", str_reg);
    show_sub_name("区域码\n");

    printf("mst  = %d\t",    psUnpack->sAddress.ucMAddress);
    show_sub_name("主站地址\n");
    printf("tml  = %d\t",    psUnpack->sAddress.usTAddress);
    show_sub_name("终端地址\n");
    printf("team = %s\t",    psUnpack->sAddress.bTeamAddr == TRUE ? "true" : "flase");
    show_sub_name("是否为组地址\t");
    (psUnpack->sAddress.bTeamAddr == TRUE ) ? show_item_value("是") : show_item_value("否");

   
    

    
    // 应用层数据
    show_main_name("\n应用层数据:\n");
    nSubNum = psUnpack->ucCmdNum;
    printf("subNum = %d\t", nSubNum);
    show_sub_name("子命令个数\n");

    for(i = 0; i < nSubNum; i++)
    {
        //printf("app data %d\n", i+1);
        sprintf(strTmp, "\n-------------------------- %d -----------------------------\n", i+1);
        fm_print(strTmp, FR_GREEN|FM_BOLD);
        show_app_data_adp(eDir, &(psUnpack->sCmdData[i]));
        printf("\n\n");
        //fm_print(strTmp, FR_GREEN|FM_BOLD);
    }
    printf("\n");
}




void show_pack(smtPack *psUnpack)
{
    if(!psUnpack)
    {
        printf("psUnpack is null\n");
        return;
    }

   
    char *str = NULL;
    char str_reg[5] = {0};
    char strTmp[128] = {0};
    int i = 0;
    int nSubNum = 0;
    eMtDir eDir;

    // 输出地址信息
    show_main_name("address:\n");
    memcpy(str_reg,psUnpack->sAddress.acRegionCode, 4);
    /*
    printf("reg_code\t= %c%c%c%c\t",
            psUnpack->sAddress.acRegionCode[0],
            psUnpack->sAddress.acRegionCode[1],
            psUnpack->sAddress.acRegionCode[2],
            psUnpack->sAddress.acRegionCode[3]);
            */
    printf("reg  = %s\t", str_reg);
    show_sub_name("区域码\n");

    printf("mst  = %d\t",    psUnpack->sAddress.ucMAddress);
    show_sub_name("主站地址\n");
    printf("tml  = %d\t",    psUnpack->sAddress.usTAddress);
    show_sub_name("终端地址\n");
    printf("team = %s\t",    psUnpack->sAddress.bTeamAddr == TRUE ? "true" : "flase");
    show_sub_name("是否为组地址\t");
    (psUnpack->sAddress.bTeamAddr == TRUE ) ? show_item_value("是") : show_item_value("否");

    // 主功能码
    show_main_name("\nmain_code:\n");
    fm_print("AFN  = ",0);
    printf("%02X\t", psUnpack->eAFN);
    switch(psUnpack->eAFN)
    {
        case AFN_00_CONF:
            str = "确认否认";
            break;
        case AFN_01_RSET:
            str = "复位";
            break;
        case AFN_02_LINK:
            str = "接口检测";
            break;
        case AFN_03_RELY:
            str = "中继站命令";
            break;
        case AFN_04_SETP:
            str = "设置参数";
            break;
        case AFN_05_CTRL:
            str = "控制命令";
            break;
         case AFN_06_AUTH:
            str = "身份认证";
            break;
         case AFN_08_CASC:
            str = "请求被级联终端主动上报";
            break;
        case AFN_09_CFIG:
            str = "请求终端配置";
            break;
        case AFN_0A_GETP:
            str = "查询参数";
            break;
         case AFN_0B_ASKT:
            str = "请求任务数据";
            break;
         case AFN_0C_ASK1:
            str = "请求1类数据 实时数据";
            break;

        case AFN_0D_ASK2:
            str = "请求2类数据 历史数据";
            break;
        case AFN_0E_ASK3:
            str = "请求3类数据 事件数据";
            break;
         case AFN_0F_FILE:
            str = "文件传输";
            break;
         case AFN_10_DATA:
            str = "数据转发";
            break;

        default:
            str = "未知命令";
            break;

    }

    show_sub_name("主功能码\t");
    show_item_value(str);

    //控制域
    show_main_name("\nctrl_field:\n");
    printf("eDir = %d\t", psUnpack->eDir);
    eDir = psUnpack->eDir;
    switch(eDir)
    {
        case MT_DIR_UNKOWN:
            str = "未知报文方向";
            break;
        case MT_DIR_M2S:
            str = "从主到到集中器 下行";
            break;
        case MT_DIR_S2M:
            str = "从集中器到主站 上行";
            break;
        default:
            str = "未知报文方向";
            break;
    }
    
    show_sub_name("报文传输方向\t");
    show_item_value(str); 

    // ctrl erpm
    printf("\neRPM = %d\t", psUnpack->ePRM);
    switch(psUnpack->ePRM)
    {
        case MT_PRM_ACTIVE:
            str = "来自启动站";
            break;
        case MT_PRM_PASIVE:
            str = "来自从动站";
            break;
        default:
            str = "从主动站未知";
            break;
    }
    
    show_sub_name("报文来源\t");
    show_item_value(str); 
    printf("\n");

    // ucPFC 帧计数器
    show_main_name("ucSeq:\n");
    printf("ucSeq  = %d\t", psUnpack->ucSeq);
    show_sub_name("帧计数器\n"); 

    printf("ePos = %d\t", psUnpack->ePos);
    show_sub_name("位置类型\t"); 
    switch(psUnpack->ePos)
    {
        case MT_POS_SIGLE:
            show_item_value("单帧\n"); 
            break;
        case MT_POS_MUL_S:
            show_item_value("多帧起始帧\n"); 
            break;
        case MT_POS_MUL_M:
            show_item_value("多帧中间帧\n"); 
            break;
        case MT_POS_MUL_E:
            show_item_value("多帧最后帧\n"); 
            break;
    
        default:
            show_error("错误的帧类型\n"); 
            break;               
    }

    // 附加域
    show_main_name("aux_field:\n");

    // EC
    if(psUnpack->bEC == TRUE)
    {
        printf("bEC  = true\t");
        show_sub_name("事件计数器\t"); 
        show_item_value("有\n"); 
    }
    else
    {
        printf("bEC  = false\t");
        show_sub_name("事件计数器\t"); 
        show_item_value("无\n"); 
    }

    // pw
    if(psUnpack->bPW == TRUE)
    {
        printf("bPW  = true\t");
        show_sub_name("消息认证码\t"); 
        show_item_value("有\n"); 
    }
    else
    {
        printf("bPW  = false\t");
        show_sub_name("消息认证码\t"); 
        show_item_value("无\n"); 
    }

    //tp
    if(psUnpack->bTP == TRUE)
    {
        printf("bTP  = true\t");
        show_sub_name("时间标签\t"); 
        show_item_value("有\n"); 
    }
    else
    {
        printf("bTP  = false\t");
        show_sub_name("时间标签\t"); 
        show_item_value("无\n"); 
    }

    // show EC value
    if(psUnpack->bEC == TRUE)
    {
        show_main_name("EC:\n");
        printf("EC1  = %d\n",  psUnpack->sEC.ucEC1);
        printf("EC2  = %d\n",  psUnpack->sEC.ucEC2);
    }

    // show pw
    if(psUnpack->bPW == TRUE)
    {
        show_main_name("PW:\n");
        char apw[17] = {0};
        memcpy((void*)apw, psUnpack->acPW, 16);
        printf("aucPW = \t");
        for(i = 0; i < 16; i++)
        {
            printf("%02d ",apw[i]);
        }
        printf("\n");
        
    }

    // show tp
    if(psUnpack->bTP == TRUE)
    {
       show_main_name("TP:\n");
       printf("PFC  = %d\t", psUnpack->sTP.ucPFC);
       show_sub_name("启动帧帧序号计数器PFC\n"); 
       
       printf("PDM  = %d\t", psUnpack->sTP.ucPermitDelayMinutes);
       show_sub_name("允许发送传输延时时间\n");

       #if 1
       printf("ucDD = %d\n", psUnpack->sTP.sDDHHmmss.ucDD);
       printf("ucHH = %d\n", psUnpack->sTP.sDDHHmmss.ucHH);
       printf("ucmm = %d\n", psUnpack->sTP.sDDHHmmss.ucmm);
       printf("ucss = %d\t", psUnpack->sTP.sDDHHmmss.ucss);
  
       printf("%d", psUnpack->sTP.sDDHHmmss.ucDD);
       show_sub_name("日"); 
       printf("%d", psUnpack->sTP.sDDHHmmss.ucHH);
       show_sub_name("时"); 
       printf("%d", psUnpack->sTP.sDDHHmmss.ucmm);
       show_sub_name("分");
       printf("%d", psUnpack->sTP.sDDHHmmss.ucss);
       show_sub_name("秒\n"); 
        
       #endif
       
       
       #if 0
       printf("time  = \t");
       
       sprintf(str_reg,"%d", psUnpack->sTP.ucDD);
       show_item_value(str_reg);
       show_sub_name("日"); 
       
       sprintf(str_reg,"%d", psUnpack->sTP.ucHH);
       show_item_value(str_reg);
       show_sub_name("时"); 
       
       sprintf(str_reg, "%d", psUnpack->sTP.ucmm);
       show_item_value(str_reg);
       show_sub_name("分");
       
       sprintf(str_reg, "%d", psUnpack->sTP.ucss);
       show_item_value(str_reg);
       show_sub_name("秒\n"); 
       #endif
       
    }

    // 应用层数据
    show_main_name("app_data:\n");
    nSubNum = psUnpack->usDataNum;
    printf("subNum = %d\t", nSubNum);
    show_sub_name("子命令个数\n");

    for(i = 0; i < nSubNum; i++)
    {
        //printf("app data %d\n", i+1);
        sprintf(strTmp, "\n-------------------------- %d -----------------------------\n", i+1);
        fm_print(strTmp, FR_GREEN|FM_BOLD);
        show_app_data(eDir, &(psUnpack->sData[i]));
        printf("\n\n");
        //fm_print(strTmp, FR_GREEN|FM_BOLD);
    }
    printf("\n");
}

void test_unpack_adp(int nstart, int argc, char *argv[])
{
    INT32  i, j = 0;
    UINT16 usLen = argc - nstart  ;
    UINT8* pInBuf = NULL;
    sMtInit  sInit = {0};
   // printf("argc = %d , nstart = %d, usLen = %d------------",argc, nstart, usLen);
   printf("unpack len = %d\n", usLen);

    // 解决命令行输入的参数
    for(i = nstart; i < argc; i++)
    {
        g_ucInBuf[j++] = strtol(argv[i], NULL,16);
       // printf(" %02X", (unsigned int)strtol(argv[i], NULL,16));
    }

    pInBuf = (UINT8*)g_ucInBuf;

   // sCmPacket *psUnpack = (sCmPacket *)g_ucOutBuf;
   char OutBuf[1024] = {0};
    sCmPacket *psUnpack = (sCmPacket *)OutBuf;

    //printf_buffer_color((char*)pInBuf, usLen);

    // 协议初始化
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eMtInit(&sInit);
    
    //调用解析函数
    eMtErr eRet = ecm_3761_unpack(pInBuf, usLen, psUnpack);

    if(eRet != MT_OK)
    {
        printf("emtUnPack() error code = %d\n", eRet);
        show_mt_error(eRet);
    }
    else
    {
        fm_print("--------------------------unpack success-------------------------\n",FR_GREEN);
        // 显示解析结构
        show_pack_adp(psUnpack); 
        fm_print("--------------------------unpack success-------------------------\n",FR_GREEN);
    }
}



void unpack_analyse(int nstart, int argc, char *argv[])
{
    INT32  i, j = 0;
    UINT16 usLen = argc - nstart ;
    UINT8* pInBuf = NULL;
    eMtErr eRet = MT_OK;
    sMtInit  sInit = {0};
    sMtUnpackCommon sUnpackCommon;
    UINT8  ucTemp = 0;
    char buf[64] = {0};
    char *str = NULL;
    sMtCmdInfor    sCmdInfor   = {0};
    eMtDir eDir;

   
    for(i = nstart; i < argc; i++)
    {
        g_ucInBuf[j++] = strtol(argv[i], NULL,16);
    }

    pInBuf = (UINT8*)g_ucInBuf;

     // 协议初始化
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eMtInit(&sInit);

    // 
    eRet = emt_unpack_common(&sUnpackCommon,pInBuf, usLen);
    if(MT_OK != eRet)
    {
        printf("emt_unpack_common() error code = %d\n", eRet);
        return;
    }
    
    // 输出结果
    fm_print("--------------------------analyse success-------------------------\n",FR_GREEN);
    show_main_name("起始字符:\t");
    printf("    [%02X]\n", sUnpackCommon.sfComHead.f68);

    // L1   
    show_main_name("长 度 域:\t");
    show_sub_name(" L1:"); 
    printf("[%02X %02X]", (sUnpackCommon.sfComHead.p10 | (sUnpackCommon.sfComHead.L1 << 2)), sUnpackCommon.sfComHead.L2);
    printf("\t%d\t", sUnpackCommon.usLenUserField1);

    show_item_value("应用层");
    printf(" %d ", sUnpackCommon.usLenUserField1 - 6);
    show_item_value("控制域");
    printf(" 1 ");
    show_item_value("地址域");
    printf(" 5\n");
  
    // L2   
    show_sub_name("\t\t L2:");
    printf("[%02X %02X]\t", (sUnpackCommon.sfComHead.p10_2 | (sUnpackCommon.sfComHead.L1_2 << 2)), sUnpackCommon.sfComHead.L2_2);
    printf("%d\t", sUnpackCommon.usLenUserField2);
    show_item_value("应用层");
    printf(" %d ", sUnpackCommon.usLenUserField2 - 6);
    show_item_value("控制域");
    printf(" 1 ");
    show_item_value("地址域");
    printf(" 5\n");

    
    show_main_name("起始字符:\t");
    printf("    [%02X]\n", sUnpackCommon.sfComHead.s68);

    show_main_name("控 制 域:\t");
    show_sub_name("  C:");
    ucTemp = sUnpackCommon.sfComHead.C;
    printf("[%02X]\t", ucTemp);

    for(i = 7; i >= 0; i--)
    {
       printf("%d", (ucTemp >> i) & 0x01);
    }
    printf("\n");
    show_sub_name("\t\tDIR:\t\t");
    printf("%d\t",  ucTemp & 0x80 ? 1:0);
    BOOL bDown = ((ucTemp & 0x80) == 0 ? TRUE:FALSE);
    sprintf(buf, "%s\n", (ucTemp & 0x80) == 0 ? "下行":"上行");
    show_item_value(buf);

    // PRM
    show_sub_name("\t\tPRM:\t\t");
    printf("%d\t",  (ucTemp & 0x40) ? 1:0);
    BOOL bPRM = ((ucTemp & 0x40) ? TRUE:FALSE);
    sprintf(buf, "%s\n", (ucTemp & 0x40) == 0 ? "从动站":"启动站");
    show_item_value(buf);

    // FCB ACD
    if(bDown == TRUE)
    {   
        eDir = MT_DIR_M2S;
        // fcb
        show_sub_name("\t\tFCB:\t\t");
        printf("%d\t",  (ucTemp & 0x20) >> 5);
        show_item_value("服务的变化位\n");

        // fcv
        show_sub_name("\t\tFCV:\t\t");    
        printf("%d\t",  (ucTemp & 0x10) >> 4);
        if(((ucTemp & 0x10) >> 4) == 1)
        {
            show_item_value("FCB 位有效\n");
        }
        else
        {
            show_item_value("FCB 位无效\n");
        }
    }
    else
    {
        eDir = MT_DIR_S2M;

        show_sub_name("\t\tACD:\t");
        printf("%d\t",  (ucTemp & 0x20) >> 5);

        memset(buf, 0,64);
        if(((ucTemp & 0x20) >> 5) == 1)
        {
            sprintf(buf, "\t%s", "有事件\n");
        }
        else
        {
            sprintf(buf, "\t%s", "无事件\n");
        }

        show_item_value(buf);

    }

    // FNCODE
    show_sub_name("\t\tFNC:\t\t");
    ucTemp = ucTemp & 0xF;
    printf("%d\t",  ucTemp);
    memset(buf, 0,64);

    if(bPRM == FALSE)
    {
        switch(ucTemp)
        {
            case 0:
            sprintf(buf, "%s", "认可");
            break;     

            case 8:
            sprintf(buf, "%s", "用户数据");
            break; 

            case 9:
            sprintf(buf, "%s", "否认：无所召唤的数据");
            break; 

            case 11:
            sprintf(buf, "%s", "链路状态");
            break; 

            default:
            sprintf(buf, "%s", "未知功能码");

             break;
        }
        
    }
    else
    {
        switch(ucTemp)
        {
            case 1:
            sprintf(buf, "%s", "复位命令");
            break;     

            case 4:
            sprintf(buf, "%s", "用户数据");
            break; 

            case 9:
            sprintf(buf, "%s", "链路测试");
            break; 

            case 10:
            sprintf(buf, "%s", "请求1级数据");
            break; 

            case 11:
            sprintf(buf, "%s", "请求2级数据");
            break; 

            default:
            sprintf(buf, "%s", "未知功能码");
            break;
        }

    }

    show_item_value(buf);
    printf("\n");

    // address
    show_main_name("地 址 域:");
    show_sub_name("\t A1:");
    printf("[%02X %02X]\t\t", sUnpackCommon.sfComHead.A.acRegionCode[0], sUnpackCommon.sfComHead.A.acRegionCode[1]);
    show_item_value("行政区划码\n");

    show_sub_name("\t\t A2:");
    printf("[%02X %02X]\t", sUnpackCommon.sfComHead.A.acRegionCode[2], sUnpackCommon.sfComHead.A.acRegionCode[3]);

    printf("%d\t", sUnpackCommon.sComPack.sAddr.usTAddress);
    if(sUnpackCommon.sComPack.sAddr.usTAddress == 0)
    {
        show_item_value("集中器 无效地址");
    }
    else if(sUnpackCommon.sComPack.sAddr.usTAddress == 0xFFFF)
    {
        show_item_value("集中器 广播地址");
    }
    else
    {
        show_item_value("集中器地址");

    }
   
    show_sub_name("\n\t\t A3:");
    ucTemp = sUnpackCommon.sfComHead.A.acRegionCode[4];
    printf("[%02X]\t", ucTemp);
   
    printf("%d\t", sUnpackCommon.sComPack.sAddr.ucMAddress);
    show_item_value("主站地址\n");
    show_sub_name("\t\t bT:\t\t");
    ucTemp = ucTemp & 0x01;
    printf("%d\t", ucTemp);
    if(ucTemp == 1)
    {
        show_item_value("A2为组地址\n");
    }
    else
    {
        show_item_value("A2为单地址\n");
    }

    // AFN
    show_main_name("功能码域:");
    show_sub_name("\tAFN:");

    ucTemp = sUnpackCommon.sfComHead.A.acRegionCode[5];
    printf("[%02X]\t\t", ucTemp);

    eMtAFN eAfn = (eMtAFN)ucTemp;
    switch(ucTemp)
    {
        case AFN_00_CONF:
            str = "确认否认";
            break;
        case AFN_01_RSET:
            str = "复位";
            break;
        case AFN_02_LINK:
            str = "接口检测";
            break;
        case AFN_03_RELY:
            str = "中继站命令";
            break;
        case AFN_04_SETP:
            str = "设置参数";
            break;
        case AFN_05_CTRL:
            str = "控制命令";
            break;
         case AFN_06_AUTH:
            str = "身份认证";
            break;
         case AFN_08_CASC:
            str = "请求被级联终端主动上报";
            break;
        case AFN_09_CFIG:
            str = "请求终端配置";
            break;
        case AFN_0A_GETP:
            str = "查询参数";
            break;
         case AFN_0B_ASKT:
            str = "请求任务数据";
            break;
         case AFN_0C_ASK1:
            str = "请求1类数据 实时数据";
            break;

        case AFN_0D_ASK2:
            str = "请求2类数据 历史数据";
            break;
        case AFN_0E_ASK3:
            str = "请求3类数据 事件数据";
            break;
         case AFN_0F_FILE:
            str = "文件传输";
            break;
         case AFN_10_DATA:
            str = "数据转发";
            break;

        default:
            str = "未知命令";
            break;

    }
    show_item_value(str);

    // SEQ
    show_main_name("\n帧序列域:");
    show_sub_name("\tSEQ:");
    ucTemp = sUnpackCommon.sfComHead.A.acRegionCode[6];
    printf("[%02X]\t", ucTemp);

    for(i = 7; i >= 0; i--)
    {
       printf("%d", (ucTemp >> i) & 0x01);
    }
    printf("\n");
    show_sub_name("\t\tTpV:\t\t");
    printf("%d\t",  ucTemp & 0x80 ? 1:0);
    BOOL bTp = (ucTemp & 0x80) ? TRUE:FALSE;
    sprintf(buf, "%s\n", (bTp == TRUE) ? "有TP 6":"无TP 0");
    show_item_value(buf);

    // FIR
    UINT8 ucFIR = (ucTemp & 0x40) ? 1:0;
    show_sub_name("\t\tFIR:\t\t");
    printf("%d\n",  ucFIR);

    UINT8 ucFIN = (ucTemp & 0x40) ? 1:0;
    show_sub_name("\t\tFIN:\t\t");
    printf("%d\t", ucFIN);

    
    if(ucFIR == 0)
    {
        if(ucFIN == 0)
        {
            str = "多帧：中间帧";
        }
        else
        {
            str = "多帧：结束帧";
        }
    }
    else
    {
        if(ucFIN == 0)
        {
            str = "多帧：第1帧";
        }
        else
        {
            str = "单帧";
        }
        
    }
    
    show_item_value(str);

    // CON
    show_sub_name("\n\t\tCON:\t\t");
    BOOL bCon = (ucTemp & 0x10) ? TRUE : FALSE;
    printf("%d\t",  (ucTemp & 0x10) ? 1 : 0);
    if(bCon == TRUE)
    {
        show_item_value("需要确认");
    }
    else
    {
        show_item_value("不需要确认");
    }
    // CON
    show_sub_name("\n\t\tseq:\t\t");
    ucTemp = ucTemp & 0xF;
    printf("%d\t",  ucTemp);
 
    show_item_value("帧序号PSEQ/RSEQ\n");

    //第一个DaDt
    show_main_name("应用数据:");

    show_sub_name("\tCMD:");
    printf("[%02X %02X %02X %02X]\n", 
        sUnpackCommon.sComPack.pSeq2Cs[0],
        sUnpackCommon.sComPack.pSeq2Cs[1],
        sUnpackCommon.sComPack.pSeq2Cs[2],
        sUnpackCommon.sComPack.pSeq2Cs[3]);


    sMtDaDt sDaDt = {0};
    memcpy(&sDaDt, sUnpackCommon.sComPack.pSeq2Cs, sizeof(sMtDaDt));
    
    show_sub_name("\t\tDa1:\t\t");
    printf("%d\n",  sDaDt.ucDA1);
    show_sub_name("\t\tDa2:\t\t");
    printf("%d\n",  sDaDt.ucDA2);
    
    show_sub_name("\t\tDt1:\t\t");
    printf("%d\n",  sDaDt.ucDT1);
    show_sub_name("\t\tDt2:\t\t");
    printf("%d\n",  sDaDt.ucDT2);


    sMtPnFn sPnFn;
    eRet =  emt_dadt_to_pnfn(&sDaDt, &sPnFn);
    sMtCmdPn sCmdPn[64];
    UINT8    ucNumCmdPn = 0;
    if(eRet != MT_OK)
    {
        show_item_value("标识错误\n");
    }
    else
    {             
        eRet = emt_pnfn_to_cmdpn(eAfn, &sPnFn, 1,   (sMtCmdPn*)&sCmdPn, &ucNumCmdPn);

        for(i = 0; i < ucNumCmdPn; i++)
        {
             show_main_name("命令类型:");
            printf("\t%d:\n", i+1);
            show_sub_name("\t\tPn:\t\t");
            printf("%d\n",  sCmdPn[i].usPn);
            show_sub_name("\t\tcmd:\t\t");
            printf("%04X\t",  sCmdPn[i].eCmd);
            eRet = eMtGetCmdInfor(sCmdPn[i].eCmd, eDir, &sCmdInfor);
            if(MT_OK != eRet)
            {
               show_item_value("未知命令\n");
            }
            else
            {
               show_item_value(sCmdInfor.pName);
               printf("\n");
            }
        }
       
    }
    
   


    fm_print("--------------------------analyse success-------------------------\n",FR_GREEN);
}


void test_unpack(int nstart, int argc, char *argv[])
{
    INT32  i, j = 0;
    UINT16 usLen = argc - nstart ;
    UINT8* pInBuf = NULL;
    sMtInit  sInit = {0};

    // printf("\nargc = %d , nstart = %d, usLen = %d------------",argc, nstart, usLen);
    // 解决命令行输入的参数
    for(i = nstart; i < argc; i++)
    {
        g_ucInBuf[j++] = strtol(argv[i], NULL,16);
        //printf(" %02X", (unsigned int)strtol(argv[i], NULL,16));
    }


    pInBuf = (UINT8*)g_ucInBuf;


    
    //smtPack *psUnpack = (smtPack *)g_ucOutBuf;

    //smtPack *psUnpack = (smtPack *)malloc(BUF_LEN_MAX);
    smtPack *psUnpack = (smtPack *)malloc(5* 1024);
    //char buf[5*1024] = {0};
    
    //printf_buffer_color((char*)pInBuf, usLen);


    // 协议初始化
    sInit.eRole = MT_ROLE_CONTOR;
    sInit.ucPermitDelayMinutes = 255;
    eMtInit(&sInit);

    //调用解析函数
    eMtErr eRet = emtUnPack(psUnpack, pInBuf, usLen);

    if(eRet != MT_OK)
    {
        printf("emtUnPack() error code = %d\n", eRet);
        show_mt_error(eRet);
    }
    else
    {
        fm_print("--------------------------unpack success-------------------------\n",FR_GREEN);
        // 显示解析结构
        show_pack(psUnpack); 
        fm_print("--------------------------unpack success-------------------------\n",FR_GREEN);
       
    }
    
    free(psUnpack);
}


int main(int argc, char *argv[])
{
    int opt = 0;
    int longIndex = 0;
    char *strOpt = "uohcnmap:t:e:a:d:";
    int    iTest= 0;// 测试例ID
    struct option longOpts[] = 
    {
        {"address",     required_argument,    NULL,    'a'},
        {"pack",        required_argument,    NULL,    'p'},
        {"test",        required_argument,    NULL,    't'},
        {"down",        required_argument,    NULL,    'd'},
        {"error",       required_argument,    NULL,    'e'},
        {"adpack",      required_argument,    NULL,    'a'},
        {"help",        no_argument,          NULL,    'h'},
        {"unpack",      no_argument,          NULL,    'u'},
        {"oadp",        no_argument,          NULL,    'o'},
        {"modbus",      no_argument,          NULL,    'm'},
        {"nengxiao",    no_argument,          NULL,    'n'},
        {"all",         no_argument,          NULL,     0},
        {NULL,          no_argument,          NULL,     0}
    };
    
    while((opt = getopt_long(argc, argv,strOpt, longOpts, &longIndex)) != -1)
    {
        switch(opt)
        {
            case 'h':
                {
                   int len = sizeof(sMtAfn0dF218_f);
                   
                   printf("sMtAfn0dF218_f len = %d\n", len);
                   len = sizeof(sMtTd_c_f);
                   printf("sMtTd_c_f len = %d\n", len);

                }
                  break;

            case 'a':
                unpack_analyse(optind, argc, argv);
                break;

            case 'd':
                iTest = (int)strtol(optarg, NULL, 16);
                printf("test id = %04X\n", iTest);
                test_pack_down(iTest);
                break;

            case 't':
                iTest = (int)strtol(optarg, NULL, 16);
                printf("test id = %04X\n", iTest);
                test_pack(iTest);
                break;
            
            case 'e':
                {
                    int iErr = (int)strtol(optarg, NULL, 10);
                    show_mt_error(iErr);
                }
              break;

          
            case 'u':
                test_unpack(optind, argc, argv);
                break;
            case 'o':
                test_unpack_adp(optind, argc, argv);
                break;
               
              default:
                  printf("Your command line parameters are not right!\n");  
                return -1;
        }
    }
    return 0;
}





