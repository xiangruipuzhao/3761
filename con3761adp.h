/******************************************************************************

                  版权所有 (C), 2010-2015, 国网电力通信与用电技术分公司

 ******************************************************************************
  文 件 名   : con3761adp.h
  版 本 号   : 初稿
  作    者   : liming
  生成日期   : 2013年4月8日 星期一
  最近修改   :
  功能描述   : 集中器376.1协议解析适配层 头文件 
  函数列表   :
  修改历史   :
  简写说明   : CM 集中器到主站   MC 主站到集中器
  1.日    期   : 2013年4月8日 星期一
    作    者   : liming
    修改内容   : 创建文件

******************************************************************************/
#ifndef _CON_3761_ADP_H_
#define _CON_3761_ADP_H_
#include "qgdw_3761_api.h"

#pragma pack(push)  // 采用一字节对齐
#pragma pack(1)

typedef eMtErr eCmErr;

/* (1)定义主站与集中器通信协议命令类型, 每条命令对一条3761协议 
 * (2)该协议分十六大类,每大类,最多支持255个子命令
 * (3)有些大类只能逐个子命令发送,有些命令支持该大类下任意个子命令的组合,
 *    但组合的子命令标识需唯一
 * (4)适配层编写者需要注册实现,该命令与API命令之间直接相互转换
*/
typedef enum
{
    CM_CMD_UNKOWN,                  // 未知命令  

    /* 确认∕否认（AFN=00H） */
    CMD_ALL_OK,                     // 全部确认
    CMD_ALL_DENY,                   // 全部否认
    CMD_ONE_BY_ONE,                 // 按数据单元标识确认和否认


    /* 复位命令（AFN=01H） */ 
    CMD_HARD_INIT,                  // 硬件初始化
    CMD_DATA_INIT,                  // 数据区初始化
    CMD_FACTORY_RESET,              // 参数及全体数据区初始化（即恢复至出厂配置）
    CMD_PARA_INIT,                  // 参数（除与系统主站通信有关的）及全体数据区初始化


    /* 链路接口检测（AFN=02H） */
    CMD_LOG_IN,                     // 登录       无数据单元
    CMD_LOG_OUT,                    // 退出登录   无数据单元
    CMD_HEART_BEAT,                 // 心跳       无数据单元

    /* 设置参数（AFN == 04H） */ 
    //CMD_CFG_NOT_ELEC ,            // 非电气测量点数据参数配置
                                    // 1：温度，2：流量，3：流速，4：压力，
                                    // 5：湿度，6：照度，7：转速，其他：备用

    CMD_CFG_TML_UP,                 // 设置 终端上行通信口通信参数设置
    CMD_CFG_TML_POWER,              // 设置 终端电能表/交流采样装置配置参数
    CMD_CFG_ASK_1,                  // 1类数据配置设置(在终端支持的1类数据配置内) CMD_AFN_4_F38_CFG_ASK_1
    CMD_CFG_ASK_2,                  // 2类数据配置设置(在终端支持的2类数据配置内) CMD_AFN_4_F39_CFG_ASK_2
    CMD_CFG_AUTO_1,                 // 定时上报1类数据任务设置    CMD_AFN_4_F65_CFG_AUTO_1
    CMD_CFG_AUTO_2,                 // 定时上报2类数据任务设置    CMD_AFN_4_F66_CFG_AUTO_2
    CMD_GO_AUTO_1,                  // 定时上报1类数据任务启动/停止设置 CMD_AFN_4_F67_GOP_AUTO_1
    CMD_GO_AUTO_2,                  // 定时上报2类数据任务启动/停止设置 CMD_AFN_4_F68_GOP_AUTO_2

    /* 控制命令（AFN == 05H）*/
    CMD_CTRL_CHECK_TIME,            // 对时命令

    CMD_PERMIT_TALK_GO,             // 允许终端与主站通话  没有应用层数据 pn = p0
    CMD_PERMIT_TALK_STOP,           // 禁止终端与主站通话  没有应用层数据 pn = p0

    CMD_PERMIT_LINK_ON,             // 激活终端连接主站    没有应用层数据 pn = p0
    CMD_PERMIT_LINK_OFF,            // 命令终端断开连接    没有应用层数据 pn = p0

    CMD_AUTO_REPORT_GO,             // 允许终端主动上报   没有应用层数据 pn = p0
    CMD_AUTO_REPORT_STOP,           // 禁止终端主动上报   没有应用层数据 pn = p0


    /* 请求配置 (AFN == 09H） */ 
    CMD_AFN_TML_VERSION,            // 终端版本信息  pn = p0  对应数据结构 sMtTmlVersion
    CMD_AFN_TML_INPUT_OUT,          // 终端支持的输入、输出及通信端口配置  pn = p0  对应数据结构 sMtTmlInputOut
    CMD_TML_OTHER_CFG,              // 终端支持的其他配置  对应数据结构 sMtTmlOtherCfg
    CMD_SUPPORT_PARA_CFG,           // 终端支持的参数配置      CMD_AFN_9_F4_SUPPORT_PARA_CFG
    CMD_SUPPORT_CTRL_CFG,           // 终端支持的控制配置      CMD_AFN_9_F5_SUPPORT_CTRL_CFG
    CMD_SUPPORT_ASK1_CFG,           // 终端支持的1类数据配置   CMD_AFN_9_F6_SUPPORT_ASK1_CFG
    CMD_SUPPORT_ASK2_CFG,           // 终端支持的2类数据配置   CMD_AFN_9_F7_SUPPORT_ASK2_CFG
    CMD_SUPPORT_EVNT_CFG,           // 终端支持的事件记录配置  CMD_AFN_9_F8_SUPPORT_EVNT_CFG
    
    /* 查询参数（AFN == 0AH） */ 
    CMD_CFG_TML_UP_Q,               // 查询 终端上行通信口通信参数设置           CMD_AFN_A_F1_TML_UP_CFG
    CMD_CFG_TML_POWER_Q,            // 查询 终端电能表/交流采样装置配置参数   (命令有参数 sMtTmlPowerCfg_Q)   CMD_AFN_A_F10_TML_POWER_CFG
    CMD_CFG_ASK_1_Q,                // 1类数据配置设置(在终端支持的1类数据配置内) (命令有参数 sMtAsk1Cfg_Q) CMD_AFN_A_F38_CFG_ASK_1
    CMD_CFG_ASK_2_Q,                // 2类数据配置设置(在终端支持的2类数据配置内) (命令有参数 sMtAsk2Cfg_Q) CMD_AFN_A_F39_CFG_ASK_2
    CMD_CFG_AUTO_1_Q,               // 查询定时上报1类数据任务设置      CMD_AFN_A_F65_CFG_AUTO_1
    CMD_CFG_AUTO_2_Q,               // 查询定时上报2类数据任务设置      CMD_AFN_A_F66_CFG_AUTO_2
    CMD_GOP_AUTO_1_Q,               // 定时上报1类数据任务启动/停止设置 CMD_AFN_A_F67_GOP_AUTO_1
    CMD_GOP_AUTO_2_Q,               // 定时上报2类数据任务启动/停止设置 CMD_AFN_A_F68_GOP_AUTO_2

    /* 请求1类数据（AFN=0CH） */
    CMD_CURNT_PQUI,                 // 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
    CMD_TML_CLOCK,                  // 终端日历时钟  数据结构 sMtUserClock   CMD_AFN_C_F2_TML_CLOCK
    CMD_DC_RLTM_DATA,               // 直流模拟量实时数据 CMD_AFN_C_F73_DC_RLTM_DATA
    CMD_POWER_CUR,                  // 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值 对应3761命令 CMD_AFN_C_F31_POWER_CUR
    CMD_CURT_HARM_VALUE,            // 当前A、B、C三相电压、电流2～N次谐波有效值 CMD_AFN_C_F57_CURT_HARM_VALUE
    CMD_CURT_HARM_RATE,             // 当前A、B、C三相电压、电流2～N次谐波含有率 CMD_AFN_C_F58_CURT_HARM_RATE
    CMD_FRTH_HAVE_POWR_C,           // 当前正向有功电能示值（总、费率1～M）              CMD_AFN_C_F129_FRTH_HAVE_POWR_C
    CMD_FRTH_NONE_POWR_C,           // 当前正向无功（组合无功1）电能示值（总、费率1～M） CMD_AFN_C_F130_FRTH_NONE_POWR_C
    
    /* 请示2类数据 (AFN = 0DH) */
    CMD_UBLN_OVER_D,                // 日不平衡度越限累计时间 日冻结  数据结构 sMtUblnOverD 对应命令 CMD_AFN_D_F28_UBLN_OVER_D
    CMD_UBLN_OVER_M,                // 月不平衡度越限累计时间 月冻结  数据结构 sMtUblnOverM 对应命令 CMD_AFN_D_F36_UBLN_OVER_M


    // 读取温度与湿度以下两个命令为暂时替代性命令，后续可能会变
    CMD_NELC_TEMP,                  // 读取温度 CMD_AFN_C_F17_TOTAL_POWER_HAVE
    CMD_NELC_HUMi,                  // 读取湿度 CMD_AFN_C_F18_TOTAL_POWER_NONE
    
   
    CM_CMD_MAX,                     // 最大命令 
}eCmCmd;            // contrator to master cmd 集中器到主站的命令




/****************************************************
 *
 * 逐条确认与否认数据结构
 *
{*///
typedef struct
{
    eCmCmd eCmd;  // 命令
    UINT16 usPN;  // 测量点号
    BOOL   bOk; 
}sCmOne;

typedef struct
{
    UINT8  ucNum;     // 需要确认或否认的子命令个数
    sCmOne sOne[1];   // 每条确认与否认数据
}sCmOneByOne;
///*}


/****************************************************
 * 数据结构: 终端或集中器支持的参数配置
 * 对应数据: sMtSuptParaCfg
 *
{*///
typedef struct
{
    UINT8   ucNum;       // 支持的配置的参数的个数
    eCmCmd  eCfgCmd[1];  // 支持的配置 对应设置参数下的命令

}sCmSuptParaCfg;

///*}

/****************************************************
 * 数据结构: 终端支持的控制配置
 * 对应数据: sMtSuptCtrlCfg
 *
{*///
typedef struct
{
    UINT8   ucNum;       // 支持的控制命令的个数
    eCmCmd  eCfgCmd[1];  // 支持的控制命令的命令字  CMD_CTRL_CHECK_TIME 到 CMD_AUTO_REPORT_STOP

}sCmSuptCtrlCfg;
///*}


/****************************************************
 * 数据结构: 终端支持的1类数据配置
 * 对应数据: sMtSuptAsk1
 *
{*///
typedef struct
{
    UINT8   ucNum;       // 终端支持的1类数据配个数
    eCmCmd  eCfgCmd[1];  // ucNum 个命令    CMD_CURNT_PQUI 到 

}sCmSuptAsk1;
///*}


/****************************************************
 * 数据结构: 终端支持的2类数据配置
 * 对应数据: sMtSuptAsk2
 * 对应命令: CMD_SUPPORT_ASK2_CFG
 *
{*///
typedef struct
{
    UINT8   ucNum;       // 终端支持的1类数据配个数
    eCmCmd  eCfgCmd[1];  // ucNum 个命令    CMD_CURNT_PQUI 到 

}sCmSuptAsk2;

///*}

/****************************************************
 * 数据结构: 1类数据配置设置 
 * 对应数据: CMD_CFG_ASK_1  CMD_CFG_ASK_1_Q
 *
{*///
typedef struct
{    
    UINT8   ucSmall;            //  用户小类号            
    UINT8   ucFN;               //  支持的命令个数  0 ~ 248
    eCmCmd  eCmd[MT_FN_MAX];    //  支持的一类数据命令码 CMD_AFN_C_F2_TML_CLOCK 到 CMD_AFN_C_F170_READ_METER
}sCmAsk1CfgOne;

typedef struct
{    
    UINT8          ucBig;                //  用户小类号            
    UINT8          ucM;                  // 本次设置的组数m（m≤该用户大类号下支持的用户小类号个数）
    sCmAsk1CfgOne  sOne[1];   // M个信息
    
}sCmAsk1Cfg;
///*}

/****************************************************
 * 数据结构: 2类数据配置设置 
 * 对应数据: CMD_CFG_ASK_2  CMD_CFG_ASK_2_Q
 *
{*///
typedef struct
{    
    UINT8   ucSmall;            //  用户小类号            
    UINT8   ucFN;               //  支持的命令个数  0 ~ 248
    eCmCmd  eCmd[MT_FN_MAX];    //  支持的一类数据命令码 CMD_AFN_D_F1_FRTH_POWR_P1P4_D 到 CMD_AFN_D_F218_COLOR_YAWP_CURVE
}sCmAsk2CfgOne;

typedef struct
{    
    UINT8          ucBig;                //  用户小类号            
    UINT8          ucM;                  // 本次设置的组数m（m≤该用户大类号下支持的用户小类号个数）
    sCmAsk2CfgOne  sOne[1];   // M个信息
    
}sCmAsk2Cfg;
///*}

/****************************************************
 * 数据结构: 定时上报1 2类数据任务 设置、查询应答
 * 对应数据: sMtCfgAuto_1 sMtCfgAuto_2
 * 对应命令: sCmCfgAuto_1 : CMD_CFG_AUTO_1 和 CMD_CFG_AUTO_1_Q
 * 对应命令: sCmCfgAuto_2 : CMD_CFG_AUTO_2 和 CMD_CFG_AUTO_2_Q
 *
{*///
typedef struct
{
    eCmCmd eCmd;            
    UINT16 usPn;
    
}sCmCmdPn;

typedef struct
{
    eMtRptUnit    eUnit;               // 定时上报周期单位(分、时、日、月)
    UINT8         ucPeriod;            // 定时上报周期
    sMtYWMDhms    sTime;               // 上报基准时间：秒分时日月年
    UINT8         ucR;                 // 曲线数据抽取倍率R (1～96)
    UINT8         ucN;                 // 数据单元标识个数n
    sCmCmdPn      sCmdPn[1];   // N个数据单元标识  

}sCmCfgAuto_1, sCmCfgAuto_2;
///*}

/****************************************************
 * 数据结构: 暂时替代命令
 * 对应数据: sMtTotalPowerHave
 * 对应命令: CMD_NELC_TEMP  CMD_NELC_HUMi
 * 对应命令: CMD_AFN_C_F17_TOTAL_POWER_HAVE  CMD_AFN_C_F18_TOTAL_POWER_NONE
 *
{*///
typedef sMtTotalPowerHave sMcNelcTmper;  // 温度
typedef sMtTotalPowerNone sMcNelcHumit;  // 湿度
///*}

typedef union
{
    // 上下行通用
    sCmOneByOne      scmOneByOne;       // 逐条确认与否认             CMD_ONE_BY_ONE
    sMtTmlUpCfg      sTmlUpCfg;         // 终端上行通信口通信参数设置 CMD_CFG_TML_UP
    sMtTmlPowerCfg   sTmlPowerCfg;      // 终端电能表/交流采样装置配置参数 CMD_CFG_TML_POWER
    sCmAsk1Cfg       sAsk1Cfg;          // 设置参数 CMD_CFG_ASK_1 和 查询参数 CMD_CFG_ASK_1_Q
    sCmAsk2Cfg       sAsk2Cfg;          // 设置参数 CMD_CFG_ASK_2 和 查询参数 CMD_CFG_ASK_2_Q
    sCmCfgAuto_1     sCfgAuto_1;        // 定时上报1类数据任务 设置、查询应答 CMD_CFG_AUTO_1   CMD_CFG_AUTO_1_Q
    sCmCfgAuto_2     sCfgAuto_2;        // 定时上报2类数据任务 设置、查询应答 CMD_CFG_AUTO_2   CMD_CFG_AUTO_2_Q
    sMtGopAuto_1     sGoAuto_1;         // 定时上报1类数据任务启动/停止设置   CMD_GO_AUTO_1    CMD_GOP_AUTO_1_Q
    sMtGopAuto_2     sGoAuto_2;         // 定时上报1类数据任务启动/停止设置   CMD_GO_AUTO_2    CMD_GOP_AUTO_2_Q

    // 上行特有 集中器到主站
    sMtTmlVersion    sTmlVersion;       // 终端版本信息    CMD_AFN_TML_VERSION
    sMtTmlInputOut   sTmlInputOut;      // 终端支持的输入、输出及通信端口配置 CMD_AFN_TML_INPUT_OUT
    sMtTmlOtherCfg   sTmlOtherCfg;      // 终端支持的其他配置     CMD_TML_OTHER_CFG
    sCmSuptParaCfg   sTmlSuptParaCfg;   // 终端支持的参数配置     CMD_SUPPORT_PARA_CFG
    sCmSuptCtrlCfg   sTmlSuptCtrlCfg;   // 终端支持的控制配置     CMD_SUPPORT_CTRL_CFG
    sCmSuptAsk1      sTmlSuptAsk1;      // 终端支持的1类数据配置  CMD_SUPPORT_ASK1_CFG
    sCmSuptAsk2      sTmlSuptAsk2;      // 终端支持的2类数据配置  CMD_SUPPORT_ASK2_CFG
    sMtSuptEvent     sTmlSuptEvent;     // 终端支持的事件记录配置 CMD_SUPPORT_EVNT_CFG
    sMtUserClock     sTmlClock;         // 终端日历时钟           CMD_TML_CLOCK 
    sMtCurPQUI       sCurPQUI;          // 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率 CMD_CURNT_PQUI 

    sMtUblnOverD     sUblnOverD;        // 日冻结 日不平衡度越限累计时间  CMD_UBLN_OVER_D 
    sMtUblnOverM     sUblnOverM;        // 月冻结 月不平衡度越限累计时间  CMD_UBLN_OVER_M 
    float            fDcRltmData;       // 直流模拟量实时数据 (仅3位有效数据)  CMD_DC_RLTM_DATA CMD_AFN_C_F73_DC_RLTM_DATA
    sMtCurPower      sCurPower;         // 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值 CMD_POWER_CUR
    sMtCurtHarmValue sTmlCurtHarmValue; // 当前A、B、C三相电压、电流2～N次谐波有效值 CMD_CURT_HARM_VALUE CMD_AFN_C_F57_CURT_HARM_VALUE
    sMtCurtHarmRate  sTmlCurtHarmRate;  // 当前A、B、C三相电压、电流2～N次谐波含有率 CMD_CURT_HARM_RATE  CMD_AFN_C_F58_CURT_HARM_RATE
    sMtFrthHavePower sTmlFrthHavePowrC; // 当前正向有功电能示值（总、费率1～M）           CMD_FRTH_HAVE_POWR_C CMD_AFN_C_F129_FRTH_HAVE_POWR_C
    sMtFrthNonePower sTmlFrthNonePowrC; // 当前正向无功（组合无功1）电能示值（总、费率1～M）CMD_FRTH_NONE_POWR_C CMD_AFN_C_F130_FRTH_NONE_POWR_C
    sMcNelcTmper     sNelcTmper;        // 温度 CMD_NELC_TEMP
    sMcNelcHumit     sNelcHumit;        // 温度 CMD_NELC_HUMi
    

    // 下行特有 主站到集中器
    sMtUserClock     sCheckTime;        // 对时命令      CMD_CTRL_CHECK_TIME
    sMtTmlPowerCfg_Q sTmlPowerCfg_Q;    // 终端电能表/交流采样装置配置参数 命令参数 CMD_CFG_TML_POWER_Q
    sMtAsk1Cfg_Q     sAsk1Cfg_Q;        // 1类数据配置设置 （在终端支持的1类数据配置内）(查询命令参数) CMD_CFG_ASK_1_Q
    sMtAsk2Cfg_Q     sAsk2Cfg_Q;        // 1类数据配置设置 （在终端支持的2类数据配置内）(查询命令参数) CMD_CFG_ASK_2_Q
    sMtTd_c          sTd_c;             // 曲线类数据时标  命令参数   
    sMtTd_h          sTd_h;             // 小时冻结 命令参数
    sMtTd_d          sTd_d;             // 日冻结   命令参数
    sMtTd_m          sTd_m;             // 月冻结   命令参数  
    

}uCmApp;  // 用户侧应用层数据    与  uMtApp 一一对应


typedef struct
{
    eCmCmd   eCmd;        // 子命令字
    UINT16   usPN;        // 子命令字一级参数
    BOOL     bApp;        // 应用层数据时有时无
    uCmApp   uAppData;    // 应用层数据 联合
}sCmSub;                  // 子命令数据结构


/*
 * 定义解析与封装共用数据结构, 集中器调用
 * 说明:
 * (1)每一个有效帧中，只包含一种主cmd类型的报文
 * (2)每一个有效帧中，可以包含多个对应于主cmd类型的子cmd类型的数据域
 *    个数最少一个，可以多个，且多个时候可以任意顺序组合
 * (3)每个subcmd类型下可能有数据，可以无应用层数据域
 */
typedef struct
{
    sMtAddress   sAddress;       // 地址信息
    BOOL         bReSend;        // 是否为重发  TRUE 重发的帧计数不会自动增加1 FALSE 会自增1
    BOOL         bActive;        // 是否为主动上报 (仅登录、心跳、退出登录、1类2类数据主动上报时为真)
    UINT8        ucCmdNum;       // 该包中含有子命令的个数 最少有一个子命令 
    sCmSub       sCmdData[1];    // 每条子命令的内容
    
}sCmPacket;

typedef struct
{
    eMtRole      eRole;                    // 身份    
    UINT8        ucPermitDelayMinutes;     // 允许时延

}sCmInit; // 3761协议层初始化数据结构

eMtAFN ecmGetCmdAfn(eCmCmd eCmd);

// 上层调用本模块前,保确实进行初始化
eCmErr ecm_3761_init(sCmInit * psCmInit); 
// 上层接口封装
INT32 ncm_3761_pack(sCmPacket* psCmPacket, UINT8 * pBuf);
INT32 ncm_3761_unpack(UINT8 * pBuf, UINT16 usBufLen, sCmPacket* psCmPacket);
INT32 ncm_3761_checkAndGetPacket(UINT8  *pinBuf, UINT16 usLen, UINT16 *pusFirstOff, UINT16 *pusFirstLen);


// 封装函数
eCmErr ecm_3761_pack(sCmPacket* psCmPacket, UINT8 * pBuf, UINT16 *pusBufLen);

// 解封装函数
eCmErr ecm_3761_unpack(UINT8 * pBuf, UINT16 usBufLen, sCmPacket* psCmPacket);

// 判断是否为一个有效帧
BOOL bcm_valid_3761_packet(UINT8  *pinBuf, UINT16 usLen);


// 找到第一个有效帧的位置
eCmErr ecm_find_frist_valid_pack(UINT8  *pinBuf, UINT16 usLen, UINT16 *pusFirstOff, UINT16 *pusFirstLen);

// 设计事件计数器
void vCmSetEC(UINT8 ucEC1, UINT8 ucEC2);

// 设置密码
BOOL bCmSetPw(char *pw); 

// 内部接口
eMtCmd emtTransAppData_cm_2_mt(eMtCmd emtCmd, uCmApp *pcmAppData, uMtApp *pmtAppData);
eMtCmd emtTransAppData_mt_2_cm(eMtCmd emtCmd, uMtApp *pmtAppData, uCmApp *pcmAppData);

eMtCmd eGetMtCmd(eCmCmd cmCMD);   
eCmCmd eGetCmCmd(eMtCmd mtCMD);
eMtPRM ecmGetPrm(eCmCmd cmCMD);  // 获得该命令的主动性

typedef struct
{
    eCmCmd ecmCMD;
    eMtCmd emtCMD;
    eMtPRM ePrm;   // 集中器封装该命令时的主动性
 
}sCmdMap; // 命令映射

#pragma pack(pop)  // 一字节对齐
#endif /* _CON_3761_ADP_H_ */
