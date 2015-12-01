/******************************************************************************

                  版权所有 (C), 2005-2015, 国家电网通信与用电技术分公司

 ******************************************************************************
  文 件 名   : qgdw_3761_api.h
  版 本 号   : 初稿
  作    者   : liming
  生成日期   : 2013年7月25日 星期四
  最近修改   :
  功能描述   : Q/GDW 376.1 2009 主站与采集终端通信协议解析 C语言 api
               该文件给出国家电网企业标准376.1协议相关的定义及协议解析的基本接口
               帧侧采用一字节对齐 其余采用默认字节序
               
  缩写说明   : 主站    master       -> m 
               集中器  concentrator -> c
               终端    terminal     -> t
  函数列表   : 
               (1) eMtInit()           协议模块初始化
               (2) emtFindValidPack()  一个buf内找到第一个有效帧
               (3) emtIsValidPack()    判断一个帧是否为有效的3761帧
               (4) emtPack()           封装3761协议
               (5) emtUnPack()         解析3761协议
               (6) vmtSetPw()          主站设置报文中的pw字段
               (7) vmtSetPwBuf()       主站设置报文中的pw字段
               (8) emtWhoAmI()         判断自是主站还是集中器
               (9) vmtSetEC()          集中器设置应答报文中的EC字段
               
  修改历史   :
  1.日    期   : 2013年7月25日 星期四
    作    者   : liming(vliming@126.com)
    修改内容   : 创建文件

******************************************************************************/
#ifndef _QGDW_3761_API_H_
#define _QGDW_3761_API_H_

// 类型定义
#ifndef _TYPE_DEF_LM_
#define _TYPE_DEF_LM_
#ifndef __RTL_H__
#define __RTL_H__
typedef char                BOOL;
#endif
#define TRUE                (1)
#define FALSE               (0)
typedef unsigned char       UCHAR;
typedef char                CHAR;
typedef unsigned short      USHORT;
typedef short               SHORT;
typedef unsigned long       ULONG;
typedef long                LONG;

#ifndef _TYPE_DEF_
#define _TYPE_DEF_
typedef char                INT8;       //c
typedef short               INT16;      //s
typedef int                 INT32;      //l
typedef unsigned char       UINT8;      //uc
typedef unsigned short      UINT16;     //us
typedef unsigned int        UINT32;     //ul

#ifdef MACHINE_64_BITS
typedef long                INT64;
typedef unsigned long       UINT64;
#else
typedef long long           INT64;
typedef unsigned long long  UINT64;
#endif

typedef char*               STRING;     //str
typedef char                ISBOOL;     //b
typedef unsigned int        IPADDR;     //ip
typedef double              DOUBLE;
typedef void*               FRWK_HANDLE;
#endif
#endif

/*******************************************************
 *  配置文件 编译宏
 *  用此API之前请配置好
 *
{*///

#define MT_CFG_SPT_MST         (1)         // 是否支持主站 配置文件 不支持将其改为 0 
#define MT_CFG_SPT_TML         (1)         // 是否支持主站 配置文件 不支持将其改为 0
#define MT_CFG_MEDIUM_NET      (1)         // 通信介质,将决定报文的最大长度 如果是以太置1 不是置0 
#define MT_CFG_ENCRYPT         (0)         // 是否需要加密 
///*}

/*******************************************************
 *  协议相关宏定义
 *  
{*///

#define METER_ADDR_LEN         (12)         // 电表地址长度                     
/****************************************
 *  地址域
 *  终端地址A2选址范围为1～65535。
 *  A2=0000H为无效地址，A2=FFFFH且A3的D0位为"1"时表示系统广播地址。
 *  
 *           
 *         
{*///
#define MT_TML_ADDR_MIN        (1)         // 终端或集中器地址最大值
#define MT_TML_ADDR_MAX        (65535)     // 主站地址最大值
#define MT_TML_ADDR_BRD        (0xFFFF)    // 主站地址最大值

#define MT_MST_ADDR_MIN        (0)         // 主站地址最大值
#define MT_MST_ADDR_MAX        (127)       // 主站地址最大值
///*}

/****************************************
 *  应用数据域 数据单元标识
 *  对于一个确定的DaDt结构,最大支持 8个Pn 同时各对应用于下边 8个Fn
 *  所以需要 8*8个数据单元
 *  
 *           
 *         
{*///
#define PN_INDEX_MAX       (8)          // 用于数据单元的组织
#define FN_INDEX_MAX       (8)          // 用于数据单元的组织
///*}

/****************************************
 *  解封、封装相关部分字节长
 *  
 *           
 *         
{*///
#define MT_PW_LEN           (16)         // 消息认证长度
#define MT_PN_MAX           (2040)       // 数据单元标识 DA Pn 最大值 
#define MT_PN_MIN           (0)          // 数据单元标识 Da Pn 最小值
#define MT_PN_NONE          (0xFFFF)     // 数据单元标识 Da Pn 无效值 即不用关心的值

#define MT_FN_MAX           (248)        // 数据单元标识 Dt Fn 最大值
#define MT_FN_MIN           (1)          // 数据单元标识 Dt Fn 最小值
#define MT_FN_NONE          (0xFF)       // 数据单元标识 Dt Fn 无效值 即不用关心的值

#define MT_UNIT_DATA_MAX    (128)        // 最大的数据单元字长

#define MT_FRM_MAX_WRLESS   (255)        // 采用专用无线数传信道,长度L1不大于255
#define MT_FRM_MAX_NET      (16384)      // 采用网络传输,长度L1不大于16383
#define MT_SEQ2CS_MAX       (2048)       // 每单帧允许SEQ与CS之间数据字节数最大值
#define MT_CA_LEN           (8)          // 控制域与地址域总字长 
#define MT_CANS_LEN         (8)          // 控制域C 地址域A AFN SEQ字节总长
#define MT_HCANS_LEN        (13)         // 控制域C 地址域A AFN SEQ 0x68 0x16 字节总长 
#define MT_UN_USER_LEN      (8)          // 报文中非数据域总长度  0x68 0x68 L L CS 0x16
#define MT_DADT_LEN         (4)          // 数据标识字长 DaDt  
#define MT_AFN_LEN          (1)          // AFN 字长
#define MT_DADT_ERR_LEN     (MT_DADT_LEN + 1)  // DaDt 与 ERR总字长

// 帧的最大字长
#if MT_CFG_MEDIUM_NET 
#define MT_FRM_LEN_MAX      MT_FRM_MAX_NET      
#else
#define MT_FRM_LEN_MAX      MT_FRM_MAX_WRLESS
#endif

#define MT_SEQ2CS_BYTES_MAX (MT_FRM_LEN_MAX - MT_HCANS_LEN)  // 每单帧允许SEQ与CS之间数据字节数最大值
#define MT_USER_MAX         (2*MT_SEQ2CS_BYTES_MAX)          // 用户侧等变长结构 smtPack smtBasePack 最大长度

// 帧的最小字长
#define MT_FRM_LEN_MIN      (20)                             // 心跳、登录的帧长最小
///*}
///*}

/*******************************************************
 *  应用层功能码 AFN
 *
{*///
typedef enum
{
    AFN_00_CONF  = 0x00,     // 确认否认
    AFN_01_RSET  = 0x01,     // 复位
    AFN_02_LINK  = 0x02,     // 链路接口检测
    AFN_03_RELY  = 0x03,     // 中继站命令
    AFN_04_SETP  = 0x04,     // 设置参数
    AFN_05_CTRL  = 0x05,     // 控制命令
    AFN_06_AUTH  = 0x06,     // 身份认证及密钥协商
    AFN_07_BACK  = 0x07,     // 备用
    AFN_08_CASC  = 0x08,     // 请求被级联终端主动上报
    AFN_09_CFIG  = 0x09,     // 请求终端配置
    AFN_0A_GETP  = 0x0A,     // 查询参数
    AFN_0B_ASKT  = 0x0B,     // 请求任务数据
    AFN_0C_ASK1  = 0x0C,     // 请求1类数据 (实时数据)
    AFN_0D_ASK2  = 0x0D,     // 请求2类数据 (历史数据)
    AFN_0E_ASK3  = 0x0E,     // 请求3类数据 (事件数据)
    AFN_0F_FILE  = 0x0F,     // 文件传输
    AFN_10_DATA  = 0x10,     // 数据转发
    AFN_11_MAX   = 0x11,     // 最大值
    AFN_NULL     = 0xFF      // 不存在的AFN

}eMtAFN;    
 ///*} 

/*******************************************************
 *  命令码定义
 *  命名规则 CMD_AFN_(xx)_F(xx)_NAME  
 *  xx 表示数字 
 *  F  表示信息类标识
 *  根据协议规则, 每组内的最多8个Fn可以组成一个数据标识
 *  上行: 终端或集中器到主站
 *  下行: 主站到终端或集中器
 *
{*///
typedef enum
{
    CMD_AFN_F_UNKOWN,                                // 未知命令

    /**********************************
     * 确认/否认
     * AFN : 00H
     *  PN : p0
     * 上行: 有
     * 下行: 有
    {*///
    CMD_AFN_0_F1_ALL_OK              = 0x0001,       // 全部确认：对收到报文中的全部数据单元标识进行确认
    CMD_AFN_0_F2_ALL_DENY            = 0x0002,       // 全部否认：对收到报文中的全部数据单元标识进行否认
    CMD_AFN_0_F3_ONE_BY_ONE          = 0x0003,       // 按数据单元标识确认和否认：对收到报文中的全部数据单元标识进行逐个确认/否认
    ///*}                                            // F4 - F248 备用

    /**********************************
     * 复位命令
     * AFN : 01
     *  PN : p0
     * 上行: AFN 00H 确认/否认
     * 下行: 有
    {*///
    CMD_AFN_1_F1_HARD_INIT           = 0x0101,       // 硬件初始化
    CMD_AFN_1_F2_DATA_INIT           = 0x0102,       // 数据区初始化
    CMD_AFN_1_F3_FACTORY_RESET       = 0x0103,       // 参数及全体数据区初始化（即恢复至出厂配置）
    CMD_AFN_1_F4_PARA_INIT           = 0x0104,       // 参数（除与系统主站通信有关的）及全体数据区初始化
    ///*}                                            // F5 - F248 备用
    
    /**********************************
     * 链路接口检测
     * AFN : 02
     *  PN : p0
     * 上行: 有
     * 下行: AFN 00H 确认/否认
    {*///
    CMD_AFN_2_F1_LOG_IN              = 0x0201,       // 登录
    CMD_AFN_2_F2_LOG_OUT             = 0x0202,       // 退出登录
    CMD_AFN_2_F3_HEART_BEAT          = 0x0203,       // 心跳
    ///*}                                            // F4 - F248 备用
    
    /**********************************
     * 中继站命令
     * AFN : 03
     *  PN : p0
     * 上行: 有
     * 下行: 有
    {*///
    CMD_AFN_3_F1_RELAY_CTRL          = 0x0301,       // 中继站工作状态控制
    CMD_AFN_3_F2_RELAY_STAT_QUERY    = 0x0302,       // 中继站工作状态查询
    CMD_AFN_3_F3_RELAY_RECD_QUERY    = 0x0303,       // 中继站工作状态切换记录查义
    CMD_AFN_3_F4_HEART_DATA_QUERY    = 0x0304,       // 中继站运行状态统计查询
    ///*}                                            // F4 - F248 备用
    
    /**********************************
     * 设置参数
     * AFN : 04
     *  PN : 分组定义
     * 上行: 有
     * 下行: 有
    {*///
    // 组1   pn:p0
    CMD_AFN_4_F1_TML_UP_CFG          = 0x0401,       // 终端上行通信口通信参数设置
    CMD_AFN_4_F2_TML_WIRELESS_CFG    = 0x0402,       // 终端上行通信口无线中继转发设置
    CMD_AFN_4_F3_MST_IP_PORT         = 0x0403,       // 主站IP地址和端口
    CMD_AFN_4_F4_MST_PHONE_SMS       = 0x0404,       // 主站电话号码和短信中心号码
    CMD_AFN_4_F5_TML_UP_AUTH         = 0x0405,       // 终端上行通信消息认证参数设置
    CMD_AFN_4_F6_TEAM_ADDR           = 0x0406,       // 集中器组地址设置
    CMD_AFN_4_F7_TML_IP_PORT         = 0x0407,       // 集中器IP地址和端口
    CMD_AFN_4_F8_TML_UP_WAY          = 0x0408,       // 集中器上行通信工作方式(以太专网或虚拟专网)

    // 组2   pn:p0
    CMD_AFN_4_F9_TML_EVENT_CFG       = 0x0409,       // 终端事件记录配置设置
    CMD_AFN_4_F10_TML_POWER_CFG      = 0x040A,       // 终端电能表/交流采样装置配置参数
    CMD_AFN_4_F11_TML_PULSE_CFG      = 0x040B,       // 终端脉冲配置参数
    CMD_AFN_4_F12_TML_STATE_INPUT    = 0x040C,       // 终端/集中器状态量输入参数
    CMD_AFN_4_F13_TML_SIMULA_CFG     = 0x040D,       // 终端电压/电流模拟量配置参数
    CMD_AFN_4_F14_TML_GRUP_TOTL      = 0x040E,       // 终端总加组配置参数
    CMD_AFN_4_F15_HAVE_DIFF_EVENT    = 0x040F,       // 有功总电能量差动超限事件参数设置
    CMD_AFN_4_F16_VPN_USER_PWD       = 0x0410,       // 虚拟专网用户名、密码

    // 组3   pn:p0
    CMD_AFN_4_F17_TML_SAFE_VALUE     = 0x0411,       // 终端保安定值
    CMD_AFN_4_F18_TML_PCTRL_PERD     = 0x0412,       // 终端功控时段
    CMD_AFN_4_F19_TML_PCTRL_FACTOR   = 0x0413,       // 终端时段功控定值浮动系数
    CMD_AFN_4_F20_TML_MONTH_FACTOR   = 0x0414,       // 终端月电能量控定值浮动系数
    CMD_AFN_4_F21_TML_POWER_FACTOR   = 0x0415,       // 终端电能量费率时段和费率数
    CMD_AFN_4_F22_TML_POWER_RATE     = 0x0416,       // 终端电能量费率
    CMD_AFN_4_F23_TML_WARNING_CFG    = 0x0417,       // 终端催告警参数
                                                     // F24 备用
    // 组4   pn:测量点号
    CMD_AFN_4_F25_MP_BASE_CFG        = 0x0419,       // 测量点基本参数
    CMD_AFN_4_F26_MP_LIMIT_CFG       = 0x041A,       // 测量点限值参数
    CMD_AFN_4_F27_MP_LOSS_CFG        = 0x041B,       // 测量点铜损、铁损参数
    CMD_AFN_4_F28_MP_PERIOD_FACTOR   = 0x041C,       // 测量点功率因数分段限值
    CMD_AFN_4_F29_TML_METER_ID       = 0x041D,       // 终端当地电能表显示号
    CMD_AFN_4_F30_TML_AUTO_READ      = 0x041E,       // 终端台区集中抄表停抄/投抄设置
    CMD_AFN_4_F31_SLAVE_ADDR         = 0x041F,       // 载波从节点附属节点地址
                                                     // F31~F32 备用
    // 组5   pn:p0
    CMD_AFN_4_F33_TML_READ_CFG       = 0x0421,       // 终端抄表运行参数设置
    CMD_AFN_4_F34_CON_DOWN_CFG       = 0x0422,       // 集中器下行通信模拟的参数设置
    CMD_AFN_4_F35_TML_READ_VIP       = 0x0423,       // 终端台区集中抄表重点户设置
    CMD_AFN_4_F36_TML_UP_LIMIT       = 0x0424,       // 终端上行通信流量门限设置
    CMD_AFN_4_F37_TML_CASC_CFG       = 0x0425,       // 终端级联通信参数
    CMD_AFN_4_F38_CFG_ASK_1          = 0x0426,       // 1类数据配置设置(在终端支持的1类数据配置内)
    CMD_AFN_4_F39_CFG_ASK_2          = 0x0427,       // 2类数据配置设置(在终端支持的2类数据配置内)
                                                     // F40 备用
    // 组6   pn:总加组号
    CMD_AFN_4_F41_PERIOD_VALUE       = 0x0429,       // 时段功控定值
    CMD_AFN_4_F42_FACTORY_HOLIDAY    = 0x042A,       // 厂休功控参数
    CMD_AFN_4_F43_SLIDE_TIME         = 0x042B,       // 功率控制的功率计算滑差时间
    CMD_AFN_4_F44_SHUTOUT_CFG        = 0x042C,       // 营业报停控参数
    CMD_AFN_4_F45_CTRL_TURN_CFG      = 0x042D,       // 功控轮次设定
    CMD_AFN_4_F46_MONTH_FIX_VALUE    = 0x042E,       // 月电量控定值
    CMD_AFN_4_F47_BUY_COUNT          = 0x042F,       // 购电量(费)控参数
    CMD_AFN_4_F48_ELEC_TURN_CFG      = 0x0430,       // 电控轮次设定
                                                   
    // 组7  pn:控制轮次
    CMD_AFN_4_F49_WARNING_TIME       = 0x0431,       // 功控告警时间
                                                     // F50~F56 备用
    // 组8  pn:总加组号
    CMD_AFN_4_F57_TML_WARN_SOUND     = 0x0439,       // 终端声音告警允许/禁止设置
    CMD_AFN_4_F58_TML_ATUO_PROTECT   = 0x043A,       // 终端自动保电设置
    CMD_AFN_4_F59_METER_LIMIT        = 0x043B,       // 电能表异常判别阈值设置
    CMD_AFN_4_F60_HUMOR_LIMIT        = 0x043C,       // 谐波限值
    CMD_AFN_4_F61_DC_SIMULA          = 0x043D,       // 直流模拟量接入参数
                                                     // F62~F64 备用
    // 组9  pn:任务号
    CMD_AFN_4_F65_CFG_AUTO_1         = 0x0441,       // 定时上报1类数据任务设置
    CMD_AFN_4_F66_CFG_AUTO_2         = 0x0442,       // 定时上报2类数据任务设置
    CMD_AFN_4_F67_GOP_AUTO_1         = 0x0443,       // 定时上报1类数据任务启动/停止设置 
    CMD_AFN_4_F68_GOP_AUTO_2         = 0x0444,       // 定时上报2类数据任务启动/停止设置 
                                                     // F69~F72 备用
    // 组10  pn:测量点号                                             
    CMD_AFN_4_F73_CAPA_CFG           = 0x0449,       // 电容器参数
    CMD_AFN_4_F74_CAPA_RUN_CFG       = 0x044A,       // 电容器投切运行参数
    CMD_AFN_4_F75_CAPA_PROT_PARA     = 0x044B,       // 电容器保护参数
    CMD_AFN_4_F76_CAPA_CTRL_WAY      = 0x044C,       // 电容器投切控制方式
                                                     // F77~F80 备用
    // 组11 pn:直流模拟量端口号                                                
    CMD_AFN_4_F81_CD_CHANGE_RATE     = 0x0451,       // 直流模拟量变比
    CMD_AFN_4_F82_CD_LIMIT           = 0x0452,       // 直流模拟量限值
    CMD_AFN_4_F83_CD_FREEZE_PARA     = 0x0453,       // 直流模拟量冻结参数
    ///*}                                            // F83~F248 备用

     /**********************************
     * 控制命令
     * AFN : 05
     *  PN : 分组定义
     * 上行: AFN 00H 确认/否认
     * 下行: 有
    {*///
    // 组1  pn:控制轮次                                               
    CMD_AFN_5_F1_REMOTE_TURN_OFF     = 0x0501,       // 遥控跳闸
    CMD_AFN_5_F2_PERMIT_TURN_ON      = 0x0502,       // 允许合闸
                                                     // F3~F8 备用
    // 组2  pn:总加组号                                                
    CMD_AFN_5_F9_PERIOD_GO           = 0x0509,       // 时段功控投入
    CMD_AFN_5_F10_HOLIDAY_GO         = 0x050A,       // 厂休功控投入
    CMD_AFN_5_F11_SHUTOUT_GO         = 0x050B,       // 营业报停功控投入
    CMD_AFN_5_F12_DOWN_GO            = 0x050C,       // 当前功率下浮控投入
                                                     // F13~F14 备用
    CMD_AFN_5_F15_MONTH_GO           = 0x050F,       // 月电控投入
    CMD_AFN_5_F16_BUY_GO             = 0x0510,       // 购电控投入                                              

    // 组3  pn:总加组号
    CMD_AFN_5_F17_PERIOD_STOP        = 0x0511,       // 时段功控解除
    CMD_AFN_5_F18_HOLIDAY_STOP       = 0x0512,       // 厂休功控解除
    CMD_AFN_5_F19_SHUTOUT_STOP       = 0x0513,       // 营业报停功控解除
    CMD_AFN_5_F20_DOWN_STOP          = 0x0514,       // 当前功率下浮控解除
                                                     // F21~F22 备用
    CMD_AFN_5_F23_MONTH_STOP         = 0x0517,       // 月电控解除
    CMD_AFN_5_F24_BUY_STOP           = 0x0518,       // 购电控解除  

    // 组4  pn:p0 
    CMD_AFN_5_F25_TML_PROTECT_GO     = 0x0519,       // 终端电保投入
    CMD_AFN_5_F26_WARN_PAY_GO        = 0x051A,       // 催费告警投入
    CMD_AFN_5_F27_PERMIT_TALK_GO     = 0x051B,       // 允许终端与主站通话
    CMD_AFN_5_F28_TAKE_OFF_TML_GO    = 0x051C,       // 终端剔除投入
    CMD_AFN_5_F29_AUTO_SAY_GO        = 0x051D,       // 允许终端主动上报
                                                     // F30 备用
    CMD_AFN_5_F31_CHECK_TIME         = 0x051F,       // 对时命令
    CMD_AFN_5_F32_CHINESE_INFO       = 0x0520,       // 中文信息

    // 组5 pn:p0 
    CMD_AFN_5_F33_TML_PROTECT_STOP   = 0x0521,       // 终端电保解除
    CMD_AFN_5_F34_WARN_PAY_STOP      = 0x0522,       // 催费告警解除
    CMD_AFN_5_F35_PERMIT_TALK_STOP   = 0x0523,       // 禁止终端与主站通话
    CMD_AFN_5_F36_TAKE_OFF_TML_STOP  = 0x0524,       // 终端剔除解除
    CMD_AFN_5_F37_AUTO_SAY_STOP      = 0x0525,       // 禁止终端主动上报
    CMD_AFN_5_F38_TML_LINK_ON        = 0x0526,       // 激活终端连接主站
    CMD_AFN_5_F39_TML_LINK_OFF       = 0x0527,       // 命令终端断开连接
                                                     // F40 备用
    // 组6 pn:测量点号 
    CMD_AFN_5_F41_CAPA_CTRL_GO       = 0x0529,       // 电容器控制投入
    CMD_AFN_5_F42_CAPA_CTRL_STOP     = 0x052A,       // 电容器控制解除
                                                     // F43~F48 备用
    // 组7 pn:p0  
    CMD_AFN_5_F49_READ_METER_STOP    = 0x0531,       // 命令指定通信端口暂停抄表
    CMD_AFN_5_F50_READ_METER_GO      = 0x0532,       // 命令指定通信端口恢复抄表
    CMD_AFN_5_F51_READ_METER_REDO    = 0x0533,       // 命令指定通信端口重新抄表
    CMD_AFN_5_F52_INIT_ROUTE         = 0x0534,       // 初始化指定通信端口下的全部中继路由信息
    CMD_AFN_5_F53_DELET_ALL_METER    = 0x0535,       // 删除指定通信端口下的全部电表
    ///*}                                            // F53~F248 备用
    
   /**********************************
     * 身份认证及密钥协商
     * AFN : 06
     *  PN : p0
     * 上行: 有
     * 下行: 有
    {*///
    CMD_AFN_6_F1_ID_AUTH_Q           = 0x0601,       // 身份认证请求
    CMD_AFN_6_F2_ID_AUTH_A           = 0x0602,       // 身份认证响应
    CMD_AFN_6_F3_RAND_Q              = 0x0603,       // 取随机数请求
    CMD_AFN_6_F4_RAND_A              = 0x0604,       // 取随机数响应
    ///*}                                            // F4 - F248 备用

    /**********************************
     * 请求被级联终端主动上报
     * AFN : 08
     *  PN : p0
     * 上行: 有主动上报数据时,
     *       无主动上报数据时, 应答否认报文 CMD_AFN_0_F2_ALL_DENY
     * 下行: 有
    {*///
    CMD_AFN_8_F1_CALL_AUTO_REPT      = 0x0801,       // 请求被级联终端主动上报
    ///*} 
    
   /**********************************
     * 请求终端配置及信息
     * AFN : 09
     *  PN : p0
     * 上行: 有
     * 下行: 有
    {*///
    CMD_AFN_9_F1_TML_VERSION         = 0x0901,       // 终端版本信息
    CMD_AFN_9_F2_TML_INPUT_OUT       = 0x0902,       // 终端支持的输入、输出及通信端口配置
    CMD_AFN_9_F3_TML_OTHER_CFG       = 0x0903,       // 终端支持的其他配置
    CMD_AFN_9_F4_SUPPORT_PARA_CFG    = 0x0904,       // 终端支持的参数配置
    CMD_AFN_9_F5_SUPPORT_CTRL_CFG    = 0x0905,       // 终端支持的控制配置
    CMD_AFN_9_F6_SUPPORT_ASK1_CFG    = 0x0906,       // 终端支持的1类数据配置
    CMD_AFN_9_F7_SUPPORT_ASK2_CFG    = 0x0907,       // 终端支持的2类数据配置
    CMD_AFN_9_F8_SUPPORT_EVNT_CFG    = 0x0908,       // 终端支持的事件记录配置
    ///*}                                            // F9 - F248 备用
  
   /**********************************
     * 查询参数
     * AFN : 0A
     *  PN : 分组定义
     * 上行: 有
     * 下行: 有
    {*///
    // 组1   pn:p0
    CMD_AFN_A_F1_TML_UP_CFG          = 0x0A01,       // 终端上行通信口通信参数设置
    CMD_AFN_A_F2_TML_WIRELESS_CFG    = 0x0A02,       // 终端上行通信口无线中继转发设置
    CMD_AFN_A_F3_MST_IP_PORT         = 0x0A03,       // 主站IP地址和端口
    CMD_AFN_A_F4_MST_PHONE_SMS       = 0x0A04,       // 主站电话号码和短信中心号码
    CMD_AFN_A_F5_TML_UP_AUTH         = 0x0A05,       // 终端上行通信消息认证参数设置
    CMD_AFN_A_F6_TEAM_ADDR           = 0x0A06,       // 集中器组地址设置
    CMD_AFN_A_F7_TML_IP_PORT         = 0x0A07,       // 集中器IP地址和端口
    CMD_AFN_A_F8_TML_UP_WAY          = 0x0A08,       // 集中器上行通信工作方式(以太专网或虚拟专网)

    // 组2   pn:p0
    CMD_AFN_A_F9_TML_EVENT_CFG       = 0x0A09,       // 终端事件记录配置设置
    CMD_AFN_A_F10_TML_POWER_CFG      = 0x0A0A,       // 终端电能表/交流采样装置配置参数
    CMD_AFN_A_F11_TML_PULSE_CFG      = 0x0A0B,       // 终端脉冲配置参数
    CMD_AFN_A_F12_TML_STATE_INPUT    = 0x0A0C,       // 终端/集中器状态量输入参数
    CMD_AFN_A_F13_TML_SIMULA_CFG     = 0x0A0D,       // 终端电压/电流模拟量配置参数
    CMD_AFN_A_F14_TML_GRUP_TOTL      = 0x0A0E,       // 终端总加组配置参数
    CMD_AFN_A_F15_HAVE_DIFF_EVENT    = 0x0A0F,       // 有功总电能量差动超限事件参数设置
    CMD_AFN_A_F16_VPN_USER_PWD       = 0x0A10,       // 虚拟专网用户名、密码

    // 组3   pn:p0
    CMD_AFN_A_F17_TML_SAFE_VALUE     = 0x0A11,       // 终端保安定值
    CMD_AFN_A_F18_TML_PCTRL_PERD     = 0x0A12,       // 终端功控时段
    CMD_AFN_A_F19_TML_PCTRL_FACTOR   = 0x0A13,       // 终端时段功控定值浮动系数
    CMD_AFN_A_F20_TML_MONTH_FACTOR   = 0x0A14,       // 终端月电能量控定值浮动系数
    CMD_AFN_A_F21_TML_POWER_FACTOR   = 0x0A15,       // 终端电能量费率时段和费率数
    CMD_AFN_A_F22_TML_POWER_RATE     = 0x0A16,       // 终端电能量费率
    CMD_AFN_A_F23_TML_WARNING_CFG    = 0x0A17,       // 终端催告警参数
                                                     // F24 备用
    // 组4   pn:测量点号
    CMD_AFN_A_F25_MP_BASE_CFG        = 0x0A19,       // 测量点基本参数
    CMD_AFN_A_F26_MP_LIMIT_CFG       = 0x0A1A,       // 测量点限值参数
    CMD_AFN_A_F27_MP_LOSS_CFG        = 0x0A1B,       // 测量点铜损、铁损参数
    CMD_AFN_A_F28_MP_PERIOD_FACTOR   = 0x0A1C,       // 测量点功率因数分段限值
    CMD_AFN_A_F29_TML_METER_ID       = 0x0A1D,       // 终端当地电能表显示号
    CMD_AFN_A_F30_TML_AUTO_READ      = 0x0A1E,       // 终端台区集中抄表停抄/投抄设置
    CMD_AFN_A_F31_SLAVE_ADDR         = 0x0A1F,       // 载波从节点附属节点地址
                                                     // F31~F32 备用
    // 组5   pn:p0
    CMD_AFN_A_F33_TML_READ_CFG       = 0x0A21,       // 终端抄表运行参数设置
    CMD_AFN_A_F34_CON_DOWN_CFG       = 0x0A22,       // 集中器下行通信模拟的参数设置
    CMD_AFN_A_F35_TML_READ_VIP       = 0x0A23,       // 终端台区集中抄表重点户设置
    CMD_AFN_A_F36_TML_UP_LIMIT       = 0x0A24,       // 终端上行通信流量门限设置
    CMD_AFN_A_F37_TML_CASC_CFG       = 0x0A25,       // 终端级联通信参数
    CMD_AFN_A_F38_CFG_ASK_1          = 0x0A26,       // 1类数据配置设置(在终端支持的1类数据配置内)
    CMD_AFN_A_F39_CFG_ASK_2          = 0x0A27,       // 2类数据配置设置(在终端支持的2类数据配置内)
                                                     // F40 备用
    // 组6   pn:总加组号
    CMD_AFN_A_F41_PERIOD_VALUE       = 0x0A29,       // 时段功控定值
    CMD_AFN_A_F42_FACTORY_HOLIDAY    = 0x0A2A,       // 厂休功控参数
    CMD_AFN_A_F43_SLIDE_TIME         = 0x0A2B,       // 功率控制的功率计算滑差时间
    CMD_AFN_A_F44_SHUTOUT_CFG        = 0x0A2C,       // 营业报停控参数
    CMD_AFN_A_F45_CTRL_TURN_CFG      = 0x0A2D,       // 功控轮次设定
    CMD_AFN_A_F46_MONTH_FIX_VALUE    = 0x0A2E,       // 月电量控定值
    CMD_AFN_A_F47_BUY_COUNT          = 0x0A2F,       // 购电量(费)控参数
    CMD_AFN_A_F48_ELEC_TURN_CFG      = 0x0A30,       // 电控轮次设定
                                                   
    // 组7  pn:控制轮次
    CMD_AFN_A_F49_WARNING_TIME       = 0x0A31,       // 功控告警时间
                                                     // F50~F56 备用
    // 组8  pn:总加组号 
    CMD_AFN_A_F57_TML_WARN_SOUND     = 0x0A39,       // 终端声音告警允许/禁止设置
    CMD_AFN_A_F58_TML_ATUO_PROTECT   = 0x0A3A,       // 终端自动保电设置
    CMD_AFN_A_F59_METER_LIMIT        = 0x0A3B,       // 电能表异常判别阈值设置
    CMD_AFN_A_F60_HUMOR_LIMIT        = 0x0A3C,       // 谐波限值
    CMD_AFN_A_F61_DC_SIMULA          = 0x0A3D,       // 直流模拟量接入参数
                                                     // F62~F64 备用
    // 组9  pn:任务号
    CMD_AFN_A_F65_CFG_AUTO_1         = 0x0A41,       // 定时上报1类数据任务设置
    CMD_AFN_A_F66_CFG_AUTO_2         = 0x0A42,       // 定时上报2类数据任务设置
    CMD_AFN_A_F67_GOP_AUTO_1         = 0x0A43,       // 定时上报1类数据任务启动/停止设置 
    CMD_AFN_A_F68_GOP_AUTO_2         = 0x0A44,       // 定时上报2类数据任务启动/停止设置 
                                                     // F69~F72 备用
    // 组10  pn:测量点号                                             
    CMD_AFN_A_F73_CAPA_CFG           = 0x0A49,       // 电容器参数
    CMD_AFN_A_F74_CAPA_RUN_CFG       = 0x0A4A,       // 电容器投切运行参数
    CMD_AFN_A_F75_CAPA_PROT_PARA     = 0x0A4B,       // 电容器保护参数
    CMD_AFN_A_F76_CAPA_CTRL_WAY      = 0x0A4C,       // 电容器投切控制方式
                                                     // F77~F80 备用
    // 组11 pn:直流模拟量端口号                                                
    CMD_AFN_A_F81_CD_CHANGE_RATE     = 0x0A51,       // 直流模拟量变比
    CMD_AFN_A_F82_CD_LIMIT           = 0x0A52,       // 直流模拟量限值
    CMD_AFN_A_F83_CD_FREEZE_PARA     = 0x0A53,       // 直流模拟量冻结参数
    ///*}                                            // F83~F248 备用

    /**********************************
     * 请求任务数据
     * AFN : 0B
     *  PN : 任务号
     * 上行: 根据不同类型,分别用请求1类数据和请求2类数据上行报文进行应答
     * 下行: 有
    {*///
    CMD_AFN_B_F1_TIMING_REPORT_1     = 0x0B01,       // 请求定时上报1类数据任务
    CMD_AFN_B_F2_TIMING_REPORT_2     = 0x0B02,       // 请求定时上报2类数据任务
    ///*} 

    /**********************************
     * 请求1类数据
     * AFN : 0C
     *  PN : 分组定义
     * 上行: 有
     * 下行: 有
    {*///
    // 组1   pn:p0                                   // F1 备用 
    CMD_AFN_C_F2_TML_CLOCK           = 0x0C02,       // 终端日历时钟
    CMD_AFN_C_F3_TML_PARA_STATE      = 0x0C03,       // 终端参数状态
    CMD_AFN_C_F4_TML_UPCOM_STATE     = 0x0C04,       // 终端上行通信状态
    CMD_AFN_C_F5_TML_CTRL_SET        = 0x0C05,       // 终端控制设置状态
    CMD_AFN_C_F6_TML_CTRL_STATE      = 0x0C06,       // 终端当前控制状态
    CMD_AFN_C_F7_TML_EC_VALUE        = 0x0C07,       // 终端事件计数器当前值
    CMD_AFN_C_F8_TML_EVNT_FLAG       = 0x0C08,       // 终端事件标志状态

    // 组2   pn:p0
    CMD_AFN_C_F9_TML_STATE_BIT       = 0x0C09,       // 终端状态量及变位标志
    CMD_AFN_C_F10_TML_MONTH_FLOW     = 0x0C0A,       // 终端与主站当日、月通信流量
    CMD_AFN_C_F11_TML_READ_METER     = 0x0C0B,       // 终端集中抄表状态信息
                                                     // F12~F16 备用
    // 组3   pn:总加组号
    CMD_AFN_C_F17_TOTAL_POWER_HAVE   = 0x0C11,       // 当前总加有功功率
    CMD_AFN_C_F18_TOTAL_POWER_NONE   = 0x0C12,       // 当前总加无功功率
    CMD_AFN_C_F19_DAY_POWER_HAVE     = 0x0C13,       // 当日总加有功电能量(总、费率 1~M)
    CMD_AFN_C_F20_DAY_POWER_NONE     = 0x0C14,       // 当日总加无功电能量(总、费率 1~M)
    CMD_AFN_C_F21_MONTH_POWER_HAVE   = 0x0C15,       // 当月总加有功电能量(总、费率 1~M)
    CMD_AFN_C_F22_MONTH_POWER_NONE   = 0x0C16,       // 当月总加无功电能量(总、费率 1~M)
    CMD_AFN_C_F23_TML_LEFT_POWER     = 0x0C17,       // 终端当前剩余电量(费)
    CMD_AFN_C_F24_DOWN_FREZ_VALUE    = 0x0C18,       // 当前功率下浮控控后总加有功功率冻结值
                                               
    // 组4   pn:测量点号
    CMD_AFN_C_F25_POWER_RATE_CUR     = 0x0C19,       // 当前三相及总有/无功功率、功率因数, 三相电压、电流、零序电流、视在功率
    CMD_AFN_C_F26_DROP_PHASE_LAST    = 0x0C1A,       // A、B、C三相断相统计数据及最近一次断相记录
    CMD_AFN_C_F27_METER_PROG_TIME    = 0x0C1B,       // 电能表日历时钟、编程次数及其最近一次操作时间
    CMD_AFN_C_F28_METER_WORD_CBIT    = 0x0C1C,       // 电表运行状态字及其变位标志
    CMD_AFN_C_F29_COPER_IRON_CUR     = 0x0C1D,       // 当前铜损、铁损有功总电能示值
    CMD_AFN_C_F30_COPER_IRON_LAST    = 0x0C1E,       // 上一结算日当前铜损、铁损有功总电能示值
    CMD_AFN_C_F31_POWER_CUR          = 0x0C1F,       // 当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
    CMD_AFN_C_F32_POWER_LAST         = 0x0C20,       // 上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值

    // 组5   pn:测量点号
    CMD_AFN_C_F33_FRTH_POWR_P1P4_C   = 0x0C21,       // 当前正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_C_F34_BACK_POWR_P2P3_C   = 0x0C22,       // 当前反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_C_F35_FRTH_DMND_M        = 0x0C23,       // 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_C_F36_BACK_DMND_M        = 0x0C24,       // 当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_C_F37_FRTH_POWR_P1P4_L   = 0x0C25,       // 上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_C_F38_BACK_POWR_P2P3_L   = 0x0C26,       // 上月（上一结算日）反向有/无功（组合无功2）电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_C_F39_FRTH_DMND_L        = 0x0C27,       // 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_C_F40_BACK_DMND_L        = 0x0C28,       // 上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）

    // 组6  pn:测量点号
    CMD_AFN_C_F41_FRTH_HAVE_POWR_D   = 0x0C29,       // 当日正向有功电能量（总、费率1～M）
    CMD_AFN_C_F42_FRTH_NONE_POWR_D   = 0x0C2A,       // 当日正向无功电能量（总、费率1～M）
    CMD_AFN_C_F43_BACK_HAVE_POWR_D   = 0x0C2B,       // 当日反向有功电能量（总、费率1～M）
    CMD_AFN_C_F44_BACK_NONE_POWR_D   = 0x0C2C,       // 当日反向无功电能量（总、费率1～M）
    CMD_AFN_C_F45_FRTH_HAVE_POWR_M   = 0x0C2D,       // 当月正向有功电能量（总、费率1～M）
    CMD_AFN_C_F46_FRTH_NONE_POWR_M   = 0x0C2E,       // 当月正向无功电能量（总、费率1～M）
    CMD_AFN_C_F47_BACK_HAVE_POWR_M   = 0x0C2F,       // 当月反向有功电能量（总、费率1～M）
    CMD_AFN_C_F48_BACK_NONE_POWR_M   = 0x0C30,       // 当月反向无功电能量（总、费率1～M）
                                                   
    // 组7  pn:测量点号
    CMD_AFN_C_F49_CURT_PHASE_ANGLE   = 0x0C31,       // 当前电压、电流相位角
                                                     // F50~F56 备用
    // 组8  pn:测量点号
    CMD_AFN_C_F57_CURT_HARM_VALUE    = 0x0C39,       // 当前A、B、C三相电压、电流2～N次谐波有效值
    CMD_AFN_C_F58_CURT_HARM_RATE     = 0x0C3A,       // 当前A、B、C三相电压、电流2～N次谐波含有率
                                                     // F59~F64 备用
    // 组9  pn:测量点号
    CMD_AFN_C_F65_CURT_CAPA_SWITCH   = 0x0C41,       // 当前电容器投切状态
    CMD_AFN_C_F66_CURT_CAPA_TIME     = 0x0C42,       // 当前电容器累计补偿投入时间和次数
    CMD_AFN_C_F67_CURT_CAPA_POWR     = 0x0C43,       // 当日、当月电容器累计补偿的无功电能量 
                                                     // F68~F72 备用
    // 组10  pn:直流模拟量端口号                                             
    CMD_AFN_C_F73_DC_RLTM_DATA       = 0x0C49,       // 直流模拟量实时数据
                                                     // F74~F80 备用
    // 组11 pn:总加组号                                                
    CMD_AFN_C_F81_HFRZ_GRUP_RATE_HAVE = 0x0C51,      // 小时冻结总加有功功率
    CMD_AFN_C_F82_HFRZ_GRUP_RATE_NONE = 0x0C52,      // 小时冻结总加无功功率
    CMD_AFN_C_F83_HFRZ_GRUP_POWR_HAVE = 0x0C53,      // 小时冻结总加有功总电能量
    CMD_AFN_C_F84_HFRZ_GRUP_POWR_NONE = 0x0C54,      // 小时冻结总加无功总电能量
                                                     // F85~F88 备用
    // 组12 pn:测量点号
    CMD_AFN_C_F89_HFRZ_RATE_HAVE     = 0x0C59,       // 小时冻结有功功率
    CMD_AFN_C_F90_HFRZ_RATE_HAVE_A   = 0x0C5A,       // 小时冻结A相有功功率
    CMD_AFN_C_F91_HFRZ_RATE_HAVE_B   = 0x0C5B,       // 小时冻结B相有功功率
    CMD_AFN_C_F92_HFRZ_RATE_HAVE_C   = 0x0C5C,       // 小时冻结C相有功功率
    CMD_AFN_C_F93_HFRZ_RATE_NONE     = 0x0C5D,       // 小时冻结无功功率
    CMD_AFN_C_F94_HFRZ_RATE_NONE_A   = 0x0C5E,       // 小时冻结A相无功功率
    CMD_AFN_C_F95_HFRZ_RATE_NONE_B   = 0x0C5F,       // 小时冻结B相无功功率
    CMD_AFN_C_F96_HFRZ_RATE_NONE_C   = 0x0C60,       // 小时冻结C相无功功率

    // 组13 pn:测量点号
    CMD_AFN_C_F97_HFRZ_VOLT_A        = 0x0C61,       // 小时冻结A相电压
    CMD_AFN_C_F98_HFRZ_VOLT_B        = 0x0C62,       // 小时冻结B相电压
    CMD_AFN_C_F99_HFRZ_VOLT_C        = 0x0C63,       // 小时冻结C相电压
    CMD_AFN_C_F100_HFRZ_ELEC_A       = 0x0C64,       // 小时冻结A相电流
    CMD_AFN_C_F101_HFRZ_ELEC_B       = 0x0C65,       // 小时冻结B相电流
    CMD_AFN_C_F102_HFRZ_ELEC_C       = 0x0C66,       // 小时冻结C相电流
    CMD_AFN_C_F103_HFRZ_ELEC_ZERO    = 0x0C67,       // 小时冻结零序电流
                                                     // F104 备用

    // 组14 pn:测量点号
    CMD_AFN_C_F105_HFRZ_FRTH_HAVE    = 0x0C69,       // 小时冻结正向有功总电能量
    CMD_AFN_C_F106_HFRZ_FRTH_NONE    = 0x0C6A,       // 小时冻结正向无功总电能量
    CMD_AFN_C_F107_HFRZ_BACK_HAVE    = 0x0C6B,       // 小时冻结反向有功总电能量
    CMD_AFN_C_F108_HFRZ_BACK_NONE    = 0x0C6C,       // 小时冻结反向无功总电能量
    CMD_AFN_C_F109_HFRZ_FRTH_HAVE_S  = 0x0C6D,       // 小时冻结正向有功总电能示值
    CMD_AFN_C_F110_HFRZ_FRTH_NONE_S  = 0x0C6E,       // 小时冻结正向无功总电能示值
    CMD_AFN_C_F111_HFRZ_BACK_HAVE_S  = 0x0C6F,       // 小时冻结反向有功总电能示值
    CMD_AFN_C_F112_HFRZ_BACK_NONE_S  = 0x0C70,       // 小时冻结反向无功总电能示值

    // 组15 pn:测量点号
    CMD_AFN_C_F113_HFRZ_FACT_TOTAL   = 0x0C71,       // 小时冻结总功率因数
    CMD_AFN_C_F114_HFRZ_FACT_A       = 0x0C72,       // 小时冻结A相功率因数
    CMD_AFN_C_F115_HFRZ_FACT_B       = 0x0C73,       // 小时冻结B相功率因数
    CMD_AFN_C_F116_HFRZ_FACT_C       = 0x0C74,       // 小时冻结C相功率因数
                                                     // F117~F120 备用    
    // 组16 pn:测量点号
    CMD_AFN_C_F121_HFRZ_DC_VALUE     = 0x0C79,       // 小时冻结直流模拟量
                                                     // F122~F128 备用
    // 组17 pn:测量点号
    CMD_AFN_C_F129_FRTH_HAVE_POWR_C  = 0x0C81,       // 当前正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F130_FRTH_NONE_POWR_C  = 0x0C82,       // 当前正向无功（组合无功1）电能示值（总、费率1～M）
    CMD_AFN_C_F131_BACK_HAVE_POWR_C  = 0x0C83,       // 当前反向有功电能示值（总、费率1～M）
    CMD_AFN_C_F132_BACK_NONE_POWR_C  = 0x0C84,       // 当前反向无功（组合无功1）电能示值（总、费率1～M）
    CMD_AFN_C_F133_NONE_POWR_P1_C    = 0x0C85,       // 当前一象限无功电能示值（总、费率1～M）
    CMD_AFN_C_F134_NONE_POWR_P2_C    = 0x0C86,       // 当前二象限无功电能示值（总、费率1～M）
    CMD_AFN_C_F135_NONE_POWR_P3_C    = 0x0C87,       // 当前三象限无功电能示值（总、费率1～M）
    CMD_AFN_C_F136_NONE_POWR_P4_C    = 0x0C88,       // 当前四象限无功电能示值（总、费率1～M）

    // 组18 pn:测量点号
    CMD_AFN_C_F137_FRTH_HAVE_POWR_L  = 0x0C89,       // 上月（上一结算日）正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F138_FRTH_NONE_POWR_L  = 0x0C8A,       // 上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
    CMD_AFN_C_F139_BACK_HAVE_POWR_L  = 0x0C8B,       // 上月（上一结算日）反向有功电能示值（总、费率1～M）
    CMD_AFN_C_F140_BACK_NONE_POWR_L  = 0x0C8C,       // 上月（上一结算日）反向无功（组合无功1）电能示值（总、费率1～M）
    CMD_AFN_C_F141_NONE_POWR_P1_L    = 0x0C8D,       // 上月（上一结算日）一象限无功电能示值（总、费率1～M）
    CMD_AFN_C_F142_NONE_POWR_P2_L    = 0x0C8E,       // 上月（上一结算日）二象限无功电能示值（总、费率1～M）
    CMD_AFN_C_F143_NONE_POWR_P3_L    = 0x0C8F,       // 上月（上一结算日）三象限无功电能示值（总、费率1～M）
    CMD_AFN_C_F144_NONE_POWR_P4_L    = 0x0C90,       // 上月（上一结算日）四象限无功电能示值（总、费率1～M）

    // 组19 pn:测量点号
    CMD_AFN_C_F145_FRTH_HAVE_DMND_C  = 0x0C91,       // 当月正向有功最大需量及发生时间（总、费率1～M）
    CMD_AFN_C_F146_FRTH_NONE_DMND_C  = 0x0C92,       // 当月正向无功最大需量及发生时间（总、费率1～M）
    CMD_AFN_C_F147_BACK_HAVE_DMND_C  = 0x0C93,       // 当月反向有功最大需量及发生时间（总、费率1～M）
    CMD_AFN_C_F148_BACK_NONE_DMND_C  = 0x0C94,       // 当月反向无功最大需量及发生时间（总、费率1～M）
    CMD_AFN_C_F149_FRTH_HAVE_DMND_L  = 0x0C95,       // 上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
    CMD_AFN_C_F150_FRTH_NONE_DMND_L  = 0x0C96,       // 上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
    CMD_AFN_C_F151_BACK_HAVE_DMND_L  = 0x0C97,       // 上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
    CMD_AFN_C_F152_BACK_NONE_DMND_L  = 0x0C98,       // 上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）

    // 组20 pn:测量点号
    CMD_AFN_C_F153_FREZ_ZONE_1       = 0x0C99,       // 第一时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F154_FREZ_ZONE_2       = 0x0C9A,       // 第二时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F155_FREZ_ZONE_3       = 0x0C9B,       // 第三时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F156_FREZ_ZONE_4       = 0x0C9C,       // 第四时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F157_FREZ_ZONE_5       = 0x0C9D,       // 第五时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F158_FREZ_ZONE_6       = 0x0C9E,       // 第六时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F159_FREZ_ZONE_7       = 0x0C9F,       // 第七时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_C_F160_FREZ_ZONE_8       = 0x0CA0,       // 第八时区冻结正向有功电能示值（总、费率1～M）

    // 组21 pn:测量点号
    CMD_AFN_C_F161_METR_REMOTE_CTRL  = 0x0CA1,       // 电能表远程控制通断电状态及记录
                                                     // F162~F164 备用
    CMD_AFN_C_F165_METR_SWITCH_RECD  = 0x0CA5,       // 电能表开关操作次数及时间
    CMD_AFN_C_F166_METR_MODIFY_RECD  = 0x0CA6,       // 电能表参数修改次数及时间
    CMD_AFN_C_F167_METR_BUY_USE      = 0x0CA7,       // 电能表购、用电信息
    CMD_AFN_C_F168_METR_BALANCE      = 0x0CA8,       // 电能表结算信息

    // 组22 pn:测量点号
    CMD_AFN_C_F169_READ_ROUTE        = 0x0CA9,       // 集中抄表中继路由信息
    CMD_AFN_C_F170_READ_METER        = 0x0CAA,       // 集中抄表电表抄读信息
    ///*}                                            // F171~F248 备用

     /**********************************
     * 请求2类数据
     * AFN : 0D
     *  PN : 分组定义
     * 上行: 有
     * 下行: 有
    {*///
    // 组1  电能示值、最大需量及电能量      pn:测量点号   (日冻结)                 
    CMD_AFN_D_F1_FRTH_POWR_P1P4_D    = 0x0D01,       // 正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F2_BACK_POWR_P2P3_D    = 0x0D02,       // 反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F3_FRTH_DMND_TIME_D    = 0x0D03,       // 正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F4_BACK_DMND_TIME_D    = 0x0D04,       // 反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F5_FRTH_HAVE_POWR_D    = 0x0D05,       // 正向有功电能量（总、费率1～M）
    CMD_AFN_D_F6_FRTH_NONE_POWR_D    = 0x0D06,       // 正向无功电能量（总、费率1～M）
    CMD_AFN_D_F7_BACK_HAVE_POWR_D    = 0x0D07,       // 反向有功电能量（总、费率1～M）
    CMD_AFN_D_F8_BACK_NONE_POWR_D    = 0x0D08,       // 反向无功电能量（总、费率1～M）

    // 组2  电能示值、最大需量 pn:测量点号 (抄表日冻结)
    CMD_AFN_D_F9_FRTH_POWR_P1P4_R    = 0x0D09,       // 正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F10_BACK_POWR_P2P3_R   = 0x0D0A,       // 反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F11_FRTH_DMND_TIME_R   = 0x0D0B,       // 正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F12_BACK_DMND_TIME_R   = 0x0D0C,       // 反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
                                                     // F13~F16 备用
                                                    
    // 组3  电能示值、最大需量及电能量 pn:测量点号 (月冻结)
    CMD_AFN_D_F17_FRTH_POWR_P1P4_M   = 0x0D11,       // 正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F18_BACK_POWR_P2P3_M   = 0x0D12,       // 反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F19_FRTH_DMND_TIME_M   = 0x0D13,       // 正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F20_BACK_DMND_TIME_M   = 0x0D14,       // 反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
    CMD_AFN_D_F21_FRTH_HAVE_POWR_M   = 0x0D15,       // 正向有功电能量（总、费率1～M）
    CMD_AFN_D_F22_FRTH_NONE_POWR_M   = 0x0D16,       // 正向无功电能量（总、费率1～M）
    CMD_AFN_D_F23_BACK_HAVE_POWR_M   = 0x0D17,       // 反向有功电能量（总、费率1～M）
    CMD_AFN_D_F24_BACK_NONE_POWR_M   = 0x0D18,       // 反向无功电能量（总、费率1～M）
                                               
    // 组4   pn:测量点号 日冻结
    CMD_AFN_D_F25_POWR_FRZE_D        = 0x0D19,       // 日总及分相最大有功功率及发生时间、有功功率为零时间
    CMD_AFN_D_F26_DMND_FRZE_D        = 0x0D1A,       // 日总及分相最大需量及发生时间
    CMD_AFN_D_F27_VOLT_FRZE_D        = 0x0D1B,       // 日电压统计数据
    CMD_AFN_D_F28_UBLN_OVER_D        = 0x0D1C,       // 日不平衡度越限累计时间
    CMD_AFN_D_F29_ELEC_OVER_D        = 0x0D1D,       // 日电流越限统计
    CMD_AFN_D_F30_POWR_RATE_D        = 0x0D1E,       // 日视在功率越限累计时间
    CMD_AFN_D_F31_LOAD_RATE_D        = 0x0D1F,       // 日负载率率统计
    CMD_AFN_D_F32_METR_DROP_D        = 0x0D20,       // 日电能表断相数据

    // 组5   pn:测量点号
    CMD_AFN_D_F33_POWR_FRZE_M        = 0x0D21,       // 月总及分相最大有功功率及发生时间、有功功率为零时间 月冻结
    CMD_AFN_D_F34_DMND_FRZE_M        = 0x0D22,       // 月总及分相有功最大需量及发生时间 月冻结
    CMD_AFN_D_F35_VOLT_FRZE_M        = 0x0D23,       // 月电压统计数据         月冻结
    CMD_AFN_D_F36_UBLN_OVER_M        = 0x0D24,       // 月不平衡度越限累计时间 月冻结
    CMD_AFN_D_F37_ELEC_OVER_M        = 0x0D25,       // 月电流越限统计         月冻结
    CMD_AFN_D_F38_POWR_RATE_M        = 0x0D26,       // 月视在功率越限累计时间 月冻结
    CMD_AFN_D_F39_LOAD_RATE_M        = 0x0D27,       // 月负载率率统计         月冻结
                                                     // F40 备用
    // 组6  pn:测量点号
    CMD_AFN_D_F41_CAPA_TIME_D        = 0x0D29,       // 电容器投入累计时间和次数 日冻结
    CMD_AFN_D_F42_CAPA_NONE_D        = 0x0D2A,       // 日、月电容器累计补偿的无功电能量 日冻结
    CMD_AFN_D_F43_FACT_TIME_D        = 0x0D2B,       // 日功率因数区段累计时间 日冻结
    CMD_AFN_D_F44_FACT_TIME_M        = 0x0D2C,       // 月功率因数区段累计时间 月冻结
    CMD_AFN_D_F45_COPR_IRON_D        = 0x0D2D,       // 铜损、铁损有功电能示值 日冻结
    CMD_AFN_D_F46_COPR_IRON_M        = 0x0D2E,       // 铜损、铁损有功电能示值 月冻结
                                                     // F47～F48 备用
    // 组7 终端统计数据 pn:测量点号
    CMD_AFN_D_F49_TML_ONOF_D         = 0x0D31,       // 终端日供电时间、日复位累计次数
    CMD_AFN_D_F50_TML_CTRL_D         = 0x0D32,       // 终端日控制统计数据
    CMD_AFN_D_F51_TML_ONOF_M         = 0x0D33,       // 终端月供电时间、月复位累计次数
    CMD_AFN_D_F52_TML_CTRL_M         = 0x0D34,       // 终端月控制统计数据
    CMD_AFN_D_F53_TML_FLOW_D         = 0x0D35,       // 终端与主站日通信流量   
    CMD_AFN_D_F54_TML_FLOW_M         = 0x0D36,       // 终端与主站月通信流量 
                                                     // F55～F56 备用 
    // 组8  总加组统计数据  pn:总加组号
    CMD_AFN_D_F57_GRUP_RATE_D        = 0x0D39,       // 总加组日最大、最小有功功率及其发生时间,有功功率为零日累计时间 日冻结
    CMD_AFN_D_F58_GRUP_HAVE_D        = 0x0D3A,       // 总加组日累计有功电能量（总、费率1～M） 日冻结
    CMD_AFN_D_F59_GRUP_NONE_D        = 0x0D3B,       // 总加组日累计无功电能量（总、费率1～M） 日冻结
    CMD_AFN_D_F60_GRUP_RATE_M        = 0x0D3C,       // 总加组月最大、最小有功功率及其发生时间,有功功率为零月累计时间 月冻结
    CMD_AFN_D_F61_GRUP_HAVE_M        = 0x0D3D,       // 总加组月累计有功电能量（总、费率1～M） 月冻结
    CMD_AFN_D_F62_GRUP_NONE_M        = 0x0D3E,       // 总加组月累计无功电能量（总、费率1～M） 月冻结
                                                     // F63～F64 备用
    // 组9  总加组越限统计数据 pn:总加组号
    CMD_AFN_D_F65_GRUP_RATE_OVER     = 0x0D41,       // 总加组超功率定值的月累计时间、月累计电能量  月冻结
    CMD_AFN_D_F66_GRUP_POWR_OVER     = 0x0D42,       // 总加组超月电能量定值的月累计时间、累计电能量 月冻结
                                                     // F67～F72 备用
    // 组10 总加组曲线         pn:总加组号                                             
    CMD_AFN_D_F73_CURVE_RATE_HAVE    = 0x0D49,       // 总加组有功功率曲线
    CMD_AFN_D_F74_CURVE_RATE_NONE    = 0x0D4A,       // 总加组无功功率曲线
    CMD_AFN_D_F75_CURVE_POWR_HAVE    = 0x0D4B,       // 总加组有功电能量曲线
    CMD_AFN_D_F76_CURVE_POWR_NONE    = 0x0D4C,       // 总加组无功电能量曲线
                                                     // F77～F80 备用
    // 组11  功率曲线 pn:测量点号                                                
    CMD_AFN_D_F81_CURVE_HAVE         = 0x0D51,       // 有功功率曲线
    CMD_AFN_D_F82_CURVE_HAVE_A       = 0x0D52,       // A相有功功率曲线
    CMD_AFN_D_F83_CURVE_HAVE_B       = 0x0D53,       // B相有功功率曲线
    CMD_AFN_D_F84_CURVE_HAVE_C       = 0x0D54,       // C相有功功率曲线
    CMD_AFN_D_F85_CURVE_NONE         = 0x0D55,       // 无功功率曲线
    CMD_AFN_D_F86_CURVE_NONE_A       = 0x0D56,       // A相无功功率曲线
    CMD_AFN_D_F87_CURVE_NONE_B       = 0x0D57,       // B相无功功率曲线
    CMD_AFN_D_F88_CURVE_NONE_C       = 0x0D58,       // C相无功功率曲线
         
    // 组12 电压电流曲线 pn:测量点号
    CMD_AFN_D_F89_CURVE_VOLT_A       = 0x0D59,       // A相电压曲线
    CMD_AFN_D_F90_CURVE_VOLT_B       = 0x0D5A,       // B相电压曲线
    CMD_AFN_D_F91_CURVE_VOLT_C       = 0x0D5B,       // C相电压曲线
    CMD_AFN_D_F92_CURVE_ELEC_A       = 0x0D5C,       // A相电流曲线
    CMD_AFN_D_F93_CURVE_ELEC_B       = 0x0D5D,       // B相电流曲线
    CMD_AFN_D_F94_CURVE_ELEC_C       = 0x0D5E,       // C相电流曲线
    CMD_AFN_D_F95_CURVE_ZERO_E       = 0x0D5F,       // 零序电流曲线
                                                     // F96 备用
    // 组13 总电能量、总电能示值曲线 pn:测量点号
    CMD_AFN_D_F97_CURVE_FRTH_HAVE    = 0x0D61,       // 正向有功总电能量 曲线
    CMD_AFN_D_F98_CURVE_FRTH_NONE    = 0x0D62,       // 正向无功总电能量 曲线
    CMD_AFN_D_F99_CURVE_BACK_HAVE    = 0x0D63,       // 反向有功总电能量 曲线
    CMD_AFN_D_F100_CURVE_BACK_HAVE   = 0x0D64,       // 反向无功总电能量 曲线
    CMD_AFN_D_F101_CURVE_FRTH_HAVE   = 0x0D65,       // 正向有功总电能示值 曲线
    CMD_AFN_D_F102_CURVE_FRTH_NONE   = 0x0D66,       // 正向无功总电能示值 曲线
    CMD_AFN_D_F103_CURVE_BACK_HAVE   = 0x0D67,       // 反向有功总电能示值 曲线
    CMD_AFN_D_F104_CURVE_BACK_NONE   = 0x0D68,       // 反向无功总电能示值 曲线

    // 组14 功率因数、电压相位角曲线、电流相位角曲线 pn:测量点号
    CMD_AFN_D_F105_CURVE_FACTOR_T    = 0x0D69,       // 总功率因数
    CMD_AFN_D_F106_CURVE_FACTOR_A    = 0x0D6A,       // A相功率因数
    CMD_AFN_D_F107_CURVE_FACTOR_B    = 0x0D6B,       // B相功率因数
    CMD_AFN_D_F108_CURVE_FACTOR_C    = 0x0D6C,       // C相功率因数
    CMD_AFN_D_F109_ANGLE_CURVE_VOLT  = 0x0D6D,       // 电压相位角曲线
    CMD_AFN_D_F110_ANGLE_CURVE_ELEC  = 0x0D6E,       // 电流相位角曲线
                                                     // F111～F112 备用

    // 组15 谐波监测统计数据 pn:测量点号
    CMD_AFN_D_F113_ELEC_HARM_TIME_A  = 0x0D71,       // A相2～19次谐波电流日最大值及发生时间 日冻结
    CMD_AFN_D_F114_ELEC_HARM_TIME_B  = 0x0D72,       // B相2～19次谐波电流日最大值及发生时间 日冻结
    CMD_AFN_D_F115_ELEC_HARM_TIME_C  = 0x0D73,       // C相2～19次谐波电流日最大值及发生时间 日冻结
    CMD_AFN_D_F116_VOLT_HARM_TIME_A  = 0x0D74,       // A相2～19次谐波电压含有率及总畸变率日最大值及发生时间 日冻结
    CMD_AFN_D_F117_VOLT_HARM_TIME_B  = 0x0D75,       // B相2～19次谐波电压含有率及总畸变率日最大值及发生时间 日冻结
    CMD_AFN_D_F118_VOLT_HARM_TIME_C  = 0x0D76,       // C相2～19次谐波电压含有率及总畸变率日最大值及发生时间 日冻结
                                                     // F119~F120 备用    
    // 组16 谐波越限统计数据 pn:测量点号
    CMD_AFN_D_F121_HARM_OVER_A       = 0x0D79,       // A相谐波越限日统计数据 日冻结
    CMD_AFN_D_F122_HARM_OVER_B       = 0x0D7A,       // B相谐波越限日统计数据 日冻结
    CMD_AFN_D_F123_HARM_OVER_C       = 0x0D7B,       // C相谐波越限日统计数据 日冻结
                                                     // F122~F128 备用
    // 组17 直流模拟量数据     pn:直流模拟量端口号
    CMD_AFN_D_F129_DC_D              = 0x0D81,       // 直流模拟量越限日累计时间、最大/最小值及发生时间 日冻结
    CMD_AFN_D_F130_DC_M              = 0x0D82,       // 直流模拟量越限月累计时间、最大/最小值及发生时间 月冻结
                                                     // F131～F136 备用
    // 组18 直流模拟量数据曲线 pn:测量点号
    CMD_AFN_D_F138_DC_CURVE          = 0x0D8A,       // 直流模拟量数据曲线

    // 组19 四个象限无功总电能示值曲线 pn:测量点号
    CMD_AFN_D_F145_CURVE_PHASE_1     = 0x0D91,       // 一象限无功总电能示值曲线
    CMD_AFN_D_F146_CURVE_PHASE_4     = 0x0D92,       // 四象限无功总电能示值曲线
    CMD_AFN_D_F147_CURVE_PHASE_2     = 0x0D93,       // 二象限无功总电能示值曲线
    CMD_AFN_D_F148_CURVE_PHASE_3     = 0x0D94,       // 三象限无功总电能示值曲线
                                                     // F149～F152 备用

    // 组20 分相电能示值 pn:测量点号
    CMD_AFN_D_F153_FRTH_HAVE_D       = 0x0D99,       // 日冻结分相正向有功电能示值 日冻结
    CMD_AFN_D_F154_FRTH_NONE_D       = 0x0D9A,       // 日冻结分相正向无功电能示值 日冻结
    CMD_AFN_D_F155_BACK_HAVE_D       = 0x0D9B,       // 日冻结分相反向有功电能示值 日冻结
    CMD_AFN_D_F156_BACK_NONE_D       = 0x0D9C,       // 日冻结分相反向无功电能示值 日冻结
    CMD_AFN_D_F157_FRTH_HAVE_M       = 0x0D9D,       // 月冻结分相正向有功电能示值 月冻结
    CMD_AFN_D_F158_FRTH_NONE_M       = 0x0D9E,       // 月冻结分相正向无功电能示值 月冻结
    CMD_AFN_D_F159_BACK_HAVE_M       = 0x0D9F,       // 月冻结分相反向有功电能示值 月冻结
    CMD_AFN_D_F160_BACK_NONE_M       = 0x0DA0,       // 月冻结分相反向无功电能示值 月冻结

    // 组21 pn:测量点号
    CMD_AFN_D_F161_HAVE_FRTH_D       = 0x0DA1,       // 正向有功电能示值（总、费率1～M）日冻结
    CMD_AFN_D_F162_NONE_FRTM_D       = 0x0DA2,       // 正向无功（组合无功1）电能示值（总、费率1～M）日冻结
    CMD_AFN_D_F163_HAVE_BACK_D       = 0x0DA3,       // 反向有功电能示值（总、费率1～M）日冻结
    CMD_AFN_D_F164_NONE_BACK_D       = 0x0DA4,       // 反向无功（组合无功1）电能示值（总、费率1～M）日冻结
    CMD_AFN_D_F165_NONE_D_PHASE_1    = 0x0DA5,       // 一象限无功电能示值（总、费率1～M）日冻结
    CMD_AFN_D_F166_NONE_D_PHASE_2    = 0x0DA6,       // 二象限无功电能示值（总、费率1～M）日冻结
    CMD_AFN_D_F167_NONE_D_PHASE_3    = 0x0DA7,       // 三象限无功电能示值（总、费率1～M）日冻结
    CMD_AFN_D_F168_NONE_D_PHASE_4    = 0x0DA8,       // 四象限无功电能示值（总、费率1～M）日冻结

    // 组22 pn:测量点号 抄表日冻结
    CMD_AFN_D_F169_HAVE_FRTH_R       = 0x0DA9,       // 正向有功电能示值（总、费率1～M）  抄表日冻结
    CMD_AFN_D_F170_NONE_FRTM_R       = 0x0DAA,       // 正向无功（组合无功1）电能示值（总、费率1～M） 抄表日冻结
    CMD_AFN_D_F171_HAVE_BACK_R       = 0x0DAB,       // 反向有功电能示值（总、费率1～M）  抄表日冻结
    CMD_AFN_D_F172_NONE_BACK_R       = 0x0DAC,       // 反向无功（组合无功1）电能示值（总、费率1～M）抄表日冻结
    CMD_AFN_D_F173_NONE_R_PHASE_1    = 0x0DAD,       // 一象限无功电能示值（总、费率1～M）抄表日冻结
    CMD_AFN_D_F174_NONE_R_PHASE_2    = 0x0DAE,       // 二象限无功电能示值（总、费率1～M）抄表日冻结
    CMD_AFN_D_F175_NONE_R_PHASE_3    = 0x0DAF,       // 三象限无功电能示值（总、费率1～M）抄表日冻结
    CMD_AFN_D_F176_NONE_R_PHASE_4    = 0x0DB0,       // 四象限无功电能示值（总、费率1～M）抄表日冻结

    // 组23 pn:测量点号
    CMD_AFN_D_F177_HAVE_FRTH_M       = 0x0DB1,       // 正向有功电能示值（总、费率1～M）   月冻结
    CMD_AFN_D_F178_NONE_FRTM_M       = 0x0DB2,       // 正向无功（组合无功1）电能示值（总、费率1～M）月冻结
    CMD_AFN_D_F179_HAVE_BACK_M       = 0x0DB3,       // 反向有功电能示值（总、费率1～M）   月冻结
    CMD_AFN_D_F180_NONE_BACK_M       = 0x0DB4,       // 反向无功（组合无功1）电能示值（总、费率1～M）月冻结
    CMD_AFN_D_F181_NONE_M_PHASE_1    = 0x0DB5,       // 一象限无功电能示值（总、费率1～M） 月冻结
    CMD_AFN_D_F182_NONE_M_PHASE_2    = 0x0DB6,       // 二象限无功电能示值（总、费率1～M） 月冻结
    CMD_AFN_D_F183_NONE_M_PHASE_3    = 0x0DB7,       // 三象限无功电能示值（总、费率1～M） 月冻结
    CMD_AFN_D_F184_NONE_M_PHASE_4    = 0x0DB8,       // 四象限无功电能示值（总、费率1～M） 月冻结

    // 组24 pn:测量点号
    CMD_AFN_D_F185_HAVE_DMND_FRTH_D  = 0x0DB9,       // 正向有功最大需量及发生时间（总、费率1～M）日冻结
    CMD_AFN_D_F186_NONE_DMND_FRTH_D  = 0x0DBA,       // 正向无功最大需量及发生时间（总、费率1～M）日冻结
    CMD_AFN_D_F187_HAVE_DMND_BACK_D  = 0x0DBB,       // 反向有功最大需量及发生时间（总、费率1～M）日冻结
    CMD_AFN_D_F188_NONE_DMND_BACK_D  = 0x0DBC,       // 反向无功最大需量及发生时间（总、费率1～M）日冻结
    CMD_AFN_D_F189_HAVE_DMND_FRTH_R  = 0x0DBD,       // 正向有功最大需量及发生时间（总、费率1～M）抄表日冻结
    CMD_AFN_D_F190_NONE_DMND_FRTH_R  = 0x0DBE,       // 正向无功最大需量及发生时间（总、费率1～M）抄表日冻结
    CMD_AFN_D_F191_HAVE_DMND_BACK_R  = 0x0DBF,       // 反向有功最大需量及发生时间（总、费率1～M）抄表日冻结
    CMD_AFN_D_F192_NONE_DMND_BACK_R  = 0x0DC0,       // 反向无功最大需量及发生时间（总、费率1～M）抄表日冻结

    // 组25 pn:测量点号
    CMD_AFN_D_F193_HAVE_DMND_FRTH_M  = 0x0DC1,       // 正向有功最大需量及发生时间（总、费率1～M） 月冻结
    CMD_AFN_D_F194_NONE_DMND_FRTH_M  = 0x0DC2,       // 正向无功最大需量及发生时间（总、费率1～M） 月冻结
    CMD_AFN_D_F195_HAVE_DMND_BACK_M  = 0x0DC3,       // 反向有功最大需量及发生时间（总、费率1～M） 月冻结
    CMD_AFN_D_F196_NONE_DMND_BACK_M  = 0x0DC4,       // 反向无功最大需量及发生时间（总、费率1～M） 月冻结
                                                     // F197～F200 备用
    // 组26 pn:测量点号
    CMD_AFN_D_F201_FREZ_ZONE_1       = 0x0DC9,       // 第一时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_D_F202_FREZ_ZONE_2       = 0x0DCA,       // 第二时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_D_F203_FREZ_ZONE_3       = 0x0DCB,       // 第三时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_D_F204_FREZ_ZONE_4       = 0x0DCC,       // 第四时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_D_F205_FREZ_ZONE_5       = 0x0DCD,       // 第五时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_D_F206_FREZ_ZONE_6       = 0x0DCE,       // 第六时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_D_F207_FREZ_ZONE_7       = 0x0DCF,       // 第七时区冻结正向有功电能示值（总、费率1～M）
    CMD_AFN_D_F208_FREZ_ZONE_8       = 0x0DD0,       // 第八时区冻结正向有功电能示值（总、费率1～M）
    
    // 组27 pn:测量点号
    CMD_AFN_D_F209_METR_REMOTE_INFO  = 0x0DD1,       // 电能表远程控制通断电状态及记录
                                                     // F210～F212 备用
    CMD_AFN_D_F213_METR_SWITCH_INFO  = 0x0DD5,       // 电能表开关操作次数及时间
    CMD_AFN_D_F214_METR_MODIFY_INFO  = 0x0DD6,       // 电能表参数修改次数及时间
    CMD_AFN_D_F215_METR_BUY_USR      = 0x0DD7,       // 电能表购、用电信息
    CMD_AFN_D_F216_METR_BALANCE      = 0x0DD8,       // 电能表结算信息

    // 组27 pn:测量点号
    CMD_AFN_D_F217_WHITE_YAWP_CURVE  = 0x0DD9,       // 台区集中抄表载波主节点白噪声曲线
    CMD_AFN_D_F218_COLOR_YAWP_CURVE  = 0x0DDA,       // 台区集中抄表载波主节点色噪声曲线
    ///*}                                            // F219～F248 备用
   
    /**********************************
     * 请求3类数据
     * AFN : 0E
     *  PN : p0
     * 上行: 有
     * 下行: 有
    {*///
    CMD_AFN_E_F1_EVENT_1             = 0x0E01,       // 请求重要事件
    CMD_AFN_E_F2_EVENT_2             = 0x0E02,       // 请求一般事件
    ///*}                                            // F3-F248 备用

    /**********************************
     * 文件传输
     * AFN : 0F
     *  PN : p0
     * 上行: 有
     * 下行: 有
    {*///
    CMD_AFN_F_F1_TRANS_WAY          = 0x0E01,        // 文件传输方式1
    ///*}                                            // F2～F248 备用  

    /**********************************
     * 数据转发
     * AFN : 10H
     *  PN : p0
     * 上行: 有
     * 下行: 有
    {*///
    // 组1 Pn:P0
    CMD_AFN_10_F1_TRANSMIT           = 0x1001,       // 透明转发
                                                     // F2～F8    备用    
    // 组2 Pn:P0
    CMD_AFN_10_F9_TRANS_READ         = 0x1009,       // 转发主站直接对终端的抄读数据命令
    CMD_AFN_10_F10_TRANS_SWITCH      = 0x100A,       // 转发主站直接对终端的遥控跳闸/允许合闸命令
    CMD_AFN_10_F11_TRANS_POWER       = 0x100B,       // 转发主站直接对终端的遥控送电命令
    ///*}                                            // F12～F248 备用    

    
    /* 在这之上添加扩展命令字 */
    CMD_AFN_FN_MAX  
}eMtCmd;
///*}

/*******************************************************
 *  错误码定义 eMtErr
 *
{*///
typedef enum
{  
    MT_OK,             // 无错误                    0 
    MT_ERR_NULL,       // 指针为空                  1 
    MT_ERR_OUTRNG,     // 参数越界                  2
    MT_ERR_NONE,       // 不存在,没有找到           3    
    MT_ERR_IO,         // IO错误                    4
    MT_ERR_RES,        // 资源不足                  5
    MT_ERR_INIT,       // 没有初始化                6
    MT_ERR_PARA,       // 参数错误                  7
    MT_ERR_FN,         // 指定Afn 中不支持的 Fn     8 
    MT_ERR_PN,         // 指定Afn 中不支持的 Pn     9 
    MT_ERR_ADDR,       // 无效地址                  10
    MT_ERR_CTRL,       // 控制域错误                11
    MT_ERR_SEQ,        // SEQ域错误                 12
    MT_ERR_SEQ2CS,     // SEQ2CS 错误错误           13
    MT_ERR_AFN,        // AFN 错误                  14
    MT_ERR_UNCOMP,     // 不完整的协议包            15
    MT_ERR_0x68,       // 错误的报文起始            16
    MT_ERR_PROTO,      // 错误的协议标识            17
    MT_ERR_CS,         // 错误的检测和              18
    MT_ERR_0x16,       // 错误的结尾符              19
    MT_ERR_LOGIC,      // 错误的处理逻辑            20
    MT_ERR_PACK,       // 不是一个有效的包          21
    MT_ERR_PFC,        // 序列号不对应              22
    MT_ERR_TIMEOUT,    // 报文超时                  23
    MT_ERR_PRM,        // 启动站标识错              24 
    MT_ERR_TEAM,       // Fn或Pn不在同一个组        25
    MT_ERR_ENCRYPT,    // 加密错误                  26
    MT_ERR_DECRYPT,    // 解密错误                  27
    
}eMtErr;    
///*} 

/*******************************************************
 *  事件类型定义 eMtErc
 *
{*///
typedef enum
{
    MT_ERC_UNKOWN,            // 未知事件记录类型
    MT_ERC_1_INIT,            // ERC1：数据初始化和版本变更记录
    MT_ERC_2_PARA_LOSS,       // ERC2：参数丢失记录
    MT_ERC_3_PARA_MODIFY,     // ERC3：参数变更记录
    MT_ERC_4_STATE_CHANGE,    // ERC4：状态量变位记录
    MT_ERC_5_REMOTE_SWITCH,   // ERC5：遥控跳闸记录
    MT_ERC_6_PCTRL_SWITCH,    // ERC6：功控跳闸记录
    MT_ERC_7_ECTRL_SWITCH,    // ERC7：电控跳闸记录
    MT_ERC_8_METER_CHANGE,    // ERC8：电能表参数变更
    MT_ERC_9_ELEC_EXCP,       // ERC9：电流回路异常
    MT_ERC_10_VOLT_EXCP,      // ERC10：电压回路异常
    MT_ERC_11_PHASE_EXCP,     // ERC11：相序异常
    MT_ERC_12_METER_TIME,     // ERC12：电能表时间超差
    MT_ERC_13_METER_FAULT,    // ERC13：电表故障信息
    MT_ERC_14_TML_ON_OFF,     // ERC14：终端停/上电事件
    MT_ERC_15_HARM_OVER,      // ERC15：谐波越限告警
    MT_ERC_16_DC_OVER,        // ERC16：直流模拟量越限记录
    MT_ERC_17_UNBL_OVER,      // ERC17：电压/电流不平衡度越限记录
    MT_ERC_18_CAPA_LOCK,      // ERC18：电容器投切自锁记录
    MT_ERC_19_BUY_PARA,       // ERC19：购电参数设置记录
    MT_ERC_20_AUTH_ERROR,     // ERC20：消息认证错误记录
    MT_ERC_21_TML_FAULT,      // ERC21：终端故障记录
    MT_ERC_22_HAVE_OVER,      // ERC22：有功总电能量差动越限事件记录
    MT_ERC_23_ECTRL_WARN,     // ERC23：电控告警事件记录
    MT_ERC_24_VOLT_OVER,      // ERC24：电压越限记录
    MT_ERC_25_ELEC_OVER,      // ERC25：电流越限记录
    MT_ERC_26_SP_OVER,        // ERC26：视在功率越限记录
    MT_ERC_27_POWER_DOWN,     // ERC27：电能表示度下降记录
    MT_ERC_28_POWER_OVER,     // ERC28：电能量超差记录
    MT_ERC_29_METER_FLY,      // ERC29：电能表飞走记录
    MT_ERC_30_METER_STOP,     // ERC30：电能表停走记录
    MT_ERC_31_READ_FAULT,     // ERC31：终端485抄表失败事件记录
    MT_ERC_32_FLOW_OVER,      // ERC32：终端与主站通信流量超门限事件记录
    MT_ERC_33_METER_WORD,     // ERC33：电能表运行状态字变位事件记录
    MT_ERC_34_CT_EXCP,        // ERC34：CT异常事件记录
    MT_ERC_35_UNKOWN,         // ERC35：发现未知电表事件记录
    
}eMtEventRecord, eMtErc; // 事件类型定义

/*******************************************************
 *  帧侧与用户侧 数据结构转换函数类型定义
 *
{*///

typedef enum
{
    MT_TRANS_UNKOWN,    // 未知方向
    MT_TRANS_U2F,       // 用户侧数据结构到帧数据结构
    MT_TRANS_F2U,       // 帧侧数据结构到用户侧数据结构 
}eMtTrans;              // 用户侧与帧侧数据结构转换方向

// 转换帧侧与用户侧数据结构的函数指针类型
// pusLen 为封装成帧侧的数据字节长
typedef eMtErr (*pMtFunc)(eMtTrans eTrans, void* pUser, void* pFrame, UINT16* pusLen);  
///*} 

/*******************************************************
 *  加密与解密函数
 *  关键数据的加密
 *  加密算法:对于应用层需要加密的关键数据,采用对称算法进行数据加解密
 *  加密部分:应用层功能码、数据单元标识及数据单元部分
 *  加密长度:通过密码机采用对称密钥算法将明文数据加密成密文,用户数据长度会相应改变
 *           终端在收到采用对称密钥算法加密的密文信息后,对数据进行解密
 *           解密成功返回原始的明文信息及明文信息的数据长度
 *  报文类型:需要加密的报文类型
 *           (1)复位命令的下行报文
 *           (2)设置参数命令的下行报文
 *           (3)控制命令的下行报文
 *
 *  我的疑问: AFN是否应该加密   如果加密将无法区分哪个类型是加密的报文
 *
{*///
#if MT_CFG_ENCRYPT
// 加密函数类型
typedef eMtErr (*peMtEncryptFunc)(UINT8* pInData, int nInLen, UINT8* pOutData, int *pOutLen);  

// 解密函数类型
typedef eMtErr (*peMtDecryptFunc)(UINT8* pInData, int nInLen, UINT8* pOutData, int *pOutLen);  
#endif
///*} 

/*******************************************************************
 *  实现附录 A.1 ~ A.28 数据格式的定义与转换函数
 *
{*///

/*********************************************
 *  数据格式01 对于表A.1
 *  格式: 
 *  字长: 6
 *  +-------+------+------+------+------+------+------+------+------+
 *  |              |                       字节格式                 |
 *  + 名 称 +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  |   秒  |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  |   分  |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  |   时  |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  |   日  |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  |星期-月| 星期BCD个位        |月BCD |        月BCD码个位        |
 *  |       |                    |码十位|                           |
 *  +-------+------+------+------+------+------+------+------+------+
 *  |   年  |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  星期定义如下：D5 ~ D7编码表示 0 ~ 7，0：无效，1-7依次表示星期一至星期日
{*///
// 用户侧
typedef struct 
{
    UINT8 ucYear;      //终端日历时钟年
    UINT8 ucMonth;     //终端日历时钟月
    UINT8 ucWeek;      //终端日历时钟星期
    UINT8 ucDay;       //终端日历日
    UINT8 ucHour;      //终端日历时钟小时
    UINT8 ucMinute;    //终端日历时钟分钟
    UINT8 ucSecond;    //终端日历时钟秒
    
}sMtUserClock, sMtFmt01, sMtYYWWMMDDhhmmss, sMtYWMDhms;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  bcd_sec_0:4;  //秒
    UINT8  bcd_sec_1:4;
    UINT8  bcd_min_0:4;  // 分
    UINT8  bcd_min_1:4;
    UINT8  bcd_hur_0:4;  // 时
    UINT8  bcd_hur_1:4;
    UINT8  bcd_day_0:4;  // 日
    UINT8  bcd_day_1:4;
    UINT8  bcd_mon_0:4;  // 月
    UINT8  bcd_mon_1:1;
    UINT8  bcd_wek_0:3;  // 星期
    UINT8  bcd_yea_0:4;  // 年
    UINT8  bcd_yea_1:4;
    
}sMtFrmClock, sMtFmt01_f, sMtYYWWMMDDhhmmss_f;
#pragma pack()

// 转换函数
eMtErr emt_trans_YYWWMMDDhhmmss(eMtTrans eTrans, sMtUserClock* psUser, sMtFrmClock* psFrame);
///*} 

/*********************************************
 *  数据格式02 对于表A.2
 *  格式: (+/-)XXX * 10(-3~4)   
 *  字长: 2
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名 称 +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |  G3  |  G2  |  G1  |  S   |         BCD码百位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  -- 
 *  S       : 符号位
 *  --
 *  G3 G2 G1:
 *  +-------+-------+-------+----------+
 *  |   G3  |  G2   |  G1   |   含义   |
 *  +-------+-------+-------+----------+
 *  |   0   |   0   |   0   |   10(4)  |
 *  +-------+-------+-------+----------+
 *  |   0   |   0   |   1   |   10(3)  |
 *  +-------+-------+-------+----------+
 *  |   0   |   1   |   0   |   10(2)  |
 *  +-------+-------+-------+----------+
 *  |   0   |   1   |   1   |   10(1)  |
 *  +-------+-------+-------+----------+
 *  |   1   |   0   |   0   |   10(0)  |
 *  +-------+-------+-------+----------+
 *  |   1   |   1   |   0   |   10(-1) |
 *  +-------+-------+-------+----------+
 *  |   0   |   0   |   0   |   10(-2) |
 *  +-------+-------+-------+----------+
 *  |   1   |   1   |   1   |   10(-3) |
 *  +-------+-------+-------+----------+
 *
{*///
// 用户侧
//float fsXXX;

// 帧侧
typedef struct
{
    UINT8 BCD_0  :4; // BCD 个位
    UINT8 BCD_1  :4; // BCD 十位
    UINT8 BCD_2  :4; // BCD 百位
    UINT8 S      :1; // 符号位
    UINT8 G1     :1; // 0
    UINT8 G2     :1; // 单位
    UINT8 G3     :1; // 0
    
}sMtFmt02_f,sMtsXXX;

// 转换函数
eMtErr emt_trans_sXXX(eMtTrans eTrans, float* psUser, sMtsXXX* psFrame);
///*} 

/*********************************************
 *  数据格式03 对于表A.4
 *  格式: (+/-)XXXXXXX
 *  字长: 2
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名 称 +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |         BCD码千位         |         BCD码百位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |        BCD码十万位        |         BCD码万位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE4 |  0   |   G  |  0   |  S   |         BCD码百万位       |
 *  +-------+------+------+------+------+------+------+------+------+
 *  G 的定义 G = 0, 表示单位为KWh、厘; G =1 表示单位为MWh、元
 *
{*///
// 用户侧
typedef enum
{
    MT_UNIT_UNKOWN,     // 未知单位
    MT_UINT_KWH_LI,     // KWh、厘
    MT_UINT_MWH_YUAN,   // MWh、元
}eMtUint;

#define MT_SX7_MIN     (-9999999)  
#define MT_SX7_MAX     (9999999)

typedef struct
{
    eMtUint eUnit;
    INT32   nData;   
}sMtWh,sMtMoney, sMtFmt03, sMtFmt_sX7;

// 帧侧
typedef struct
{
    UINT8 BCD_0  :4; // BCD 个位
    UINT8 BCD_1  :4; // BCD 十位
    UINT8 BCD_2  :4; // BCD 百位
    UINT8 BCD_3  :4; // BCD 千位
    UINT8 BCD_4  :4; // BCD 万位
    UINT8 BCD_5  :4; // BCD 十万位
    UINT8 BCD_6  :4; // BCD 百万位
    UINT8 S      :1; // 符号位
    UINT8 rev0   :1; // 0
    UINT8 G      :1; // 单位
    UINT8 rev1   :1; // 0
    
}sMtFmt03_f,sMtFmt_sX7_f;

// 转换函数
eMtErr emt_trans_sX7(eMtTrans eTrans, sMtFmt_sX7* psUser, sMtFmt_sX7_f* psFrame);
///*} 

/*********************************************
 *  数据格式04 对于表A.5
 *  格式: (上浮/下浮)XX (0 ~ 79)
 *  字长: 1
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名 称 +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |   S0 |  BCD码十位(0~7)    |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *
 *  S0定义：S0=0，表示上浮；S0=1，表示下浮。
 *
{*///
typedef enum
{
    MT_FLOAT_DIR_UNKOWN,  // 未知浮动方向
    MT_FLOAT_DIR_UP,      // 止浮
    MT_FLOAT_DIR_DOWN,    // 下浮

}eMtFloatDir;

#define MT_FLOAT_MIN    (0)
#define MT_FLOAT_MAX    (79)

// 用户侧
typedef struct
{
    eMtFloatDir eDir;     // 浮动方向
    UINT8       ucValue;  // 浮动数值
}sMtFloat, sMtsXX, sMtFmt04;

// 帧侧
typedef struct
{
    UINT8 BCD_0  :4; // BCD 个位
    UINT8 BCD_1  :3; // BCD 十位
    UINT8 S0     :1; // 符号位
}sMtFmt04_f,sMtsXX_f;

// 转换函数
eMtErr emt_trans_sXX(eMtTrans eTrans, sMtsXX* psUser, sMtsXX_f* psFrame);
///*}

/*********************************************
 *  数据格式05 对于表A.6
 *  格式: (+/-)XXX.X
 *  字长: 2
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名 称 +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码个分位        |        BCD码十分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |  S   |     BCD码百位      |         BCD码十位         |
 *  +-------+------+------+------+------+------+------+------+------+
{*///
// 用户侧
// float fsXXX.X

// 帧侧
#pragma pack(1)
typedef struct
{
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4; // BCD 个位
    UINT8 BCD_1  :4; // BCD 十位
    UINT8 BCD_2  :3; // BCD 百位
    UINT8 S      :1; // BCD 符号位
}sMtFmt05,sMtFmt05_f, sMtFmt_sXXX_X;
#pragma pack()

// 转换函数
eMtErr emt_trans_sXXX_X(eMtTrans eTrans, float* psUser, sMtFmt05* psFrame);
///*} 

/*********************************************
 *  数据格式06 对于表A.7
 *  格式: (+/-)XX.XX
 *  字长: 2
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名 称 +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码十分位        |        BCD码百分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |  S   |     BCD码十位      |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
{*///
// 用户侧
// float fXX.XX

// 帧侧
#pragma pack(1)
typedef struct
{
    UINT8 BCD_0_2:4;  // BCD 百分位
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :3;  // BCD 十位
    UINT8 S      :1;  // 符号位
}sMtFmt06, sMtFmt06_f, sMtFmt_sXX_XX;
#pragma pack()

// 转换函数
eMtErr emt_trans_sXX_XX(eMtTrans eTrans, float* psUser, sMtFmt06* psFrame);
///*} 

/*********************************************
 *  数据格式07 对于表A.8
 *  格式: (+)XXX.X
 *  字长: 2
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名 称 +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码个分位        |        BCD码十分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |         BCD码百位         |         BCD码十位         |
 *  +-------+------+------+------+------+------+------+------+------+
{*///

// 用户侧
// float fXXX.X

// 帧侧
#pragma pack(1)
typedef struct
{
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :4;  // BCD 十位
    UINT8 BCD_2  :4;  // BCD 百位
}sMtFmt07, sMtFmt07_f, sMtFmt_XXX_X;
#pragma pack()

// 转换函数
eMtErr emt_trans_XXX_X(eMtTrans eTrans, float* psUser, sMtFmt07* psFrame);
///*} 

/*********************************************
 *  数据格式08 对于表A.9
 *  格式: XXXX
 *  字长: 2
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码十位          |        BCD码个位          |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |        BCD码千位          |        BCD码百位          |
 *  +-------+------+------+------+------+------+------+------+------+
{*///

// 用户侧
// UINT16  xxxx

// 帧侧
typedef struct
{
    UINT8 BCD_4:4;  // BCD 个位
    UINT8 BCD_3:4;  // BCD 十位
    UINT8 BCD_2:4;  // BCD 百位
    UINT8 BCD_1:4;  // BCD 千位
}sMtFmt08, sMtFmt08_f, sMtFmt_XXXX;

// 转换函数
eMtErr emt_trans_XXXX(eMtTrans eTrans, UINT16* psUser, sMtFmt08* psFrame);
///*} 


/*********************************************
 *  数据格式09 对于表A.10
 *  格式: (+/-)XX.XXXX
 *  字长: 3
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码千分位        |        BCD码万分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |        BCD码十分位        |        BCD码百分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |  S   |     BCD码十位      |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
{*///

// 用户侧
// float fXX.XXXX

// 帧侧
typedef struct
{
    UINT8 BCD_0_4:4;  // BCD 万分位
    UINT8 BCD_0_3:4;  // BCD 千分位
    UINT8 BCD_0_2:4;  // BCD 百分位
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :3;  // BCD 十位
    UINT8 S      :1;  // 符号位
}sMtFmt09, sMtFmt_sXX_XXXX;

// 10 exp次方 exp < 10
int  nMtPow(UINT8 exp); 

// 转换函数
eMtErr emt_trans_sXX_XXXX(eMtTrans eTrans, float* psUser, sMtFmt09* psFrame);
///*} 

/*********************************************
 *  数据格式10 对于表A.11
 *  格式: XXXXXX
 *  字长: 3
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码十位          |        BCD码个位          |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |        BCD码千位          |        BCD码百位          |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |        BCD码十万位        |         BCD码万位         |
 *  +-------+------+------+------+------+------+------+------+------+
{*///

// 用户侧
// UINT32 ulXXXXXX

// 帧侧
typedef struct
{
  UINT8 BCD_6:4;  // BCD 个位
  UINT8 BCD_5:4;  // BCD 十位
  UINT8 BCD_4:4;  // BCD 百位
  UINT8 BCD_3:4;  // BCD 千位
  UINT8 BCD_2:4;  // BCD 万位
  UINT8 BCD_1:4;  // BCD 十万位
  
}sMtFmt_XXXXXX, sMtFmt10_f;

// 转换函数
eMtErr emt_trans_XXXXXX(eMtTrans eTrans, UINT32* psUser, sMtFmt_XXXXXX* psFrame);
///*} 

/*********************************************
 *  数据格式11 对于表A.12
 *  格式: (+)XXXXXX.XX
 *  字长: 4
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码十分位        |        BCD码百分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |         BCD码千位         |         BCD码百位         | 
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE4 |        BCD码十万位        |         BCD码万位         |
 *  +-------+------+------+------+------+------+------+------+------+
{*///

// 用户侧
// double dXXXXXX.XX

// 帧侧
typedef struct
{
    UINT8 BCD_0_2:4;  // BCD 百分位
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :4;  // BCD 十位
    UINT8 BCD_2  :4;  // BCD 百位
    UINT8 BCD_3  :4;  // BCD 千位
    UINT8 BCD_4  :4;  // BCD 万位
    UINT8 BCD_5  :4;  // BCD 十万位
    
}sMtFmt11, sMtFmt11_f, sMtFmt_XXXXXX_XX;

// 转换函数
eMtErr emt_trans_XXXXXX_XX(eMtTrans eTrans, double* psUser, sMtFmt_XXXXXX_XX* psFrame);
///*}

/*********************************************
 *  数据格式12 对于表A.13
 *  格式: (+)XXXXXXXXXXXX
 *  字长: 6
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |       BCD码十位           |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |       BCD码千分位         |         BCD码百位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |       BCD码十万位         |         BCD码万位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE4 |       BCD码千万位         |         BCD码百万位       | 
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE5 |       BCD码十亿位         |         BCD码亿位         | 
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE6 |       BCD码千亿位         |         BCD码百亿位       | 
 *  +-------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
// UINT8 ucData[12];

// 帧侧
typedef struct
{
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :4;  // BCD 十位
    UINT8 BCD_2  :4;  // BCD 百位
    UINT8 BCD_3  :4;  // BCD 千位
    UINT8 BCD_4  :4;  // BCD 万位
    UINT8 BCD_5  :4;  // BCD 十万位
    UINT8 BCD_6  :4;  // BCD 百万位
    UINT8 BCD_7  :4;  // BCD 千万位
    UINT8 BCD_8  :4;  // BCD 亿位
    UINT8 BCD_9  :4;  // BCD 十亿位
    UINT8 BCD_10 :4;  // BCD 百亿位
    UINT8 BCD_11 :4;  // BCD 千亿位
    
}sMtFmt12, sMtFmt_XX_6;

// 转换函数
eMtErr emt_trans_XX_6(eMtTrans eTrans, UINT8* psUser, sMtFmt_XX_6* psFrame);
///*}

/*********************************************
 *  数据格式13 对于表A.14
 *  格式: (+)XXXX.XXXX
 *  字长: 4
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码千分位        |        BCD码万分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |        BCD码十分位        |        BCD码百分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |         BCD码千位         |         BCD码百位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
// double dXXXX.XXXX

// 帧侧
typedef struct
{
    UINT8 BCD_0_4:4;  // BCD 万分位
    UINT8 BCD_0_3:4;  // BCD 千分位
    UINT8 BCD_0_2:4;  // BCD 百分位
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :4;  // BCD 十位
    UINT8 BCD_2  :4;  // BCD 百位
    UINT8 BCD_3  :4;  // BCD 千位
    
}sMtFmt13, sMtFmt_XXXX_XXXX, sMtFmt13_f;

// 转换函数
eMtErr emt_trans_XXXX_XXXX(eMtTrans eTrans, double* psUser, sMtFmt_XXXX_XXXX* psFrame);
///*}

/*********************************************
 *  数据格式14 对于表A.15
 *  格式: (+)XXXXXX.XXXX
 *  字长: 5
 *  格式:
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码千分位        |        BCD码万分位        | 
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |        BCD码十分位        |        BCD码百分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |         BCD码十位         |         BCD码个位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE4 |         BCD码千位         |         BCD码百位         | 
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE5 |        BCD码十万位        |          BCD码万位        |
 *  +-------+------+------+------+------+------+------+------+------+
{*///

// 用户侧
// double dXXXXXX.XXXX

// 帧侧
typedef struct
{
    UINT8 BCD_0_4:4;  // BCD 百分位
    UINT8 BCD_0_3:4;  // BCD 十分位
    UINT8 BCD_0_2:4;  // BCD 百分位
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :4;  // BCD 十位
    UINT8 BCD_2  :4;  // BCD 百位
    UINT8 BCD_3  :4;  // BCD 千位
    UINT8 BCD_4  :4;  // BCD 万位
    UINT8 BCD_5  :4;  // BCD 十万位
    
}sMtFmt14, sMtFmt14_f,sMtFmt_XXXXXX_XXXX;

// 转换函数
eMtErr emt_trans_XXXXXX_XXXX(eMtTrans eTrans, double* psUser, sMtFmt_XXXXXX_XXXX* psFrame);
///*}

/*********************************************
 *  数据格式15 对于表A.16
 *  单位: 年月日时分
 *  字长: 5
 *  格式:
 *  +------+------+------+------+------+------+------+------+------+
 *  |      |                       字节格式                        |
 *  + 名称 +------+------+------+------+------+------+------+------+
 *  |      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  分  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  时  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  日  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  月  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  年  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
typedef struct
{
    UINT8  ucYY;        // 年
    UINT8  ucMM;        // 月
    UINT8  ucDD;        // 日
    UINT8  ucHH;        // 时
    UINT8  ucmm;        // 分
 
}sMtFmt15, sMtYYMMDDhhmm;
 
// 帧侧
typedef struct
{
    UINT8  bcd_mm_0:4;  // 分
    UINT8  bcd_mm_1:4;
    UINT8  bcd_hh_0:4;  // 时
    UINT8  bcd_hh_1:4;
    UINT8  bcd_DD_0:4;  // 日
    UINT8  bcd_DD_1:4;
    UINT8  bcd_MM_0:4;  // 月
    UINT8  bcd_MM_1:4;
    UINT8  bcd_YY_0:4;  // 年
    UINT8  bcd_YY_1:4;
    
}sMtFmt15_f,sMtYYMMDDhhmm_f;

// 转换函数
eMtErr emt_trans_YYMMDDhhmm(eMtTrans eTrans, sMtYYMMDDhhmm* psUser, sMtYYMMDDhhmm_f* psFrame);
///*} 

/*********************************************
 *  数据格式16 对于表A.17
 *  单位: 月日时分
 *  字长: 4
 *  格式:
 *  +------+------+------+------+------+------+------+------+------+
 *  |      |                       字节格式                        |
 *  + 名称 +------+------+------+------+------+------+------+------+
 *  |      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  秒  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  分  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  时  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  日  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
typedef struct
{
    UINT8  ucDD;        // 日
    UINT8  ucHH;        // 时
    UINT8  ucmm;        // 分
    UINT8  ucss;        // 秒  
}sMtFmt16, sMtDDHHmmss;

// 帧侧
typedef struct
{
    UINT8  bcd_ss_0:4;  // 秒
    UINT8  bcd_ss_1:4;
    UINT8  bcd_mm_0:4;  // 分
    UINT8  bcd_mm_1:4;
    UINT8  bcd_HH_0:4;  // 时
    UINT8  bcd_HH_1:4;
    UINT8  bcd_DD_0:4;  // 日
    UINT8  bcd_DD_1:4;
    
}sMtFmt16_f, sMtDDHHmmss_f;

// 转换函数
eMtErr emt_trans_DDHHmmss(eMtTrans eTrans, sMtDDHHmmss* psDDHHmmss_u, sMtDDHHmmss_f* psDDHHmmss_f);
///*} 

/*********************************************
 *  数据格式17 对于表A.18
 *  单位: 月日时分
 *  字长: 4
 *  格式:
 *  +------+------+------+------+------+------+------+------+------+
 *  |      |                       字节格式                        |
 *  + 名称 +------+------+------+------+------+------+------+------+
 *  |      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  分  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  时  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  日  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  月  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
typedef struct
{
    UINT8  ucMM;        // 月
    UINT8  ucDD;        // 日
    UINT8  ucHH;        // 时
    UINT8  ucmm;        // 分
    
}sMtFmt17, sMtMMDDHHmm;

// 帧侧
typedef struct
{
    UINT8  bcd_mm_0:4;  // 分
    UINT8  bcd_mm_1:4;
    UINT8  bcd_HH_0:4;  // 时
    UINT8  bcd_HH_1:4;
    UINT8  bcd_DD_0:4;  // 日
    UINT8  bcd_DD_1:4;
    UINT8  bcd_MM_0:4;  // 月
    UINT8  bcd_MM_1:4;
    
}sMtFmt17_f, sMtMMDDHHmm_f;

// 转换函数
eMtErr emt_trans_MMDDHHmm(eMtTrans eTrans, sMtMMDDHHmm* psMMDDHHmm_u, sMtMMDDHHmm_f* psMMDDHHmm_f);
///*} 

/*********************************************
 *  数据格式18 对于表A.19
 *  单位: 月日时分
 *  字长: 3
 *  格式:
 *  +------+------+------+------+------+------+------+------+------+
 *  |      |                       字节格式                        |
 *  + 名称 +------+------+------+------+------+------+------+------+
 *  |      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  分  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  时  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  日  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
typedef struct
{
    UINT8  ucDD;        // 日
    UINT8  ucHH;        // 时
    UINT8  ucmm;        // 分
}sMtFmt18, sMtDDHHmm;

// 帧侧
typedef struct
{
    UINT8  bcd_mm_0:4;  // 分
    UINT8  bcd_mm_1:4;
    UINT8  bcd_HH_0:4;  // 时
    UINT8  bcd_HH_1:4;
    UINT8  bcd_DD_0:4;  // 日
    UINT8  bcd_DD_1:4;
}sMtFmt18_f, sMtDDHHmm_f;

// 转换函数
eMtErr emt_trans_DDHHmm(eMtTrans eTrans, sMtDDHHmm* psDDHHmm_u, sMtDDHHmm_f* psDDHHmm_f);
///*} 

/*********************************************
 *  数据格式19 对于表A.20
 *  单位: 月日时分
 *  字长: 2
 *  格式:
 *  +------+------+------+------+------+------+------+------+------+
 *  |      |                       字节格式                        |
 *  + 名称 +------+------+------+------+------+------+------+------+
 *  |      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  分  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  时  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
typedef struct
{
    UINT8  ucHH;        // 时
    UINT8  ucmm;        // 分
}sMtFmt19, sMtHHmm;

// 帧侧
typedef struct
{
    UINT8  bcd_mm_0:4;  // 分
    UINT8  bcd_mm_1:4;
    UINT8  bcd_HH_0:4;  // 时
    UINT8  bcd_HH_1:4;
}sMtFmt19_f, sMtHHmm_f;

// 转换函数
eMtErr emt_trans_HHmm(eMtTrans eTrans, sMtHHmm* psHHmm_u, sMtHHmm_f* psHHmm_f);
///*} 

/*********************************************
 *  数据格式20 对于表A.21
 *  单位: 年月日
 *  字长: 3
 *  格式:
 *  +------+------+------+------+------+------+------+------+------+
 *  |      |                       字节格式                        |
 *  + 名称 +------+------+------+------+------+------+------+------+
 *  |      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  日  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  月  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  年  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
typedef struct
{
    UINT8  ucYY;        // 年
    UINT8  ucMM;        // 月
    UINT8  ucDD;        // 日
}sMtFmt20, sMtYYMMDD;

// 帧侧
typedef struct
{
    UINT8  bcd_DD_0:4;  // 日
    UINT8  bcd_DD_1:4;
    UINT8  bcd_MM_0:4;  // 月
    UINT8  bcd_MM_1:4;
    UINT8  bcd_YY_0:4;  // 年
    UINT8  bcd_YY_1:4;
}sMtFmt20_f, sMtYYMMDD_f;

// 转换函数
eMtErr emt_trans_YYMMDD(eMtTrans eTrans, sMtYYMMDD* psUser, sMtYYMMDD_f* psFrame);
///*} 

/*********************************************
 *  数据格式21 对于表A.22
 *  单位: 年月
 *  字长: 2
 *  格式:
 *  +------+------+------+------+------+------+------+------+------+
 *  |      |                       字节格式                        |
 *  + 名称 +------+------+------+------+------+------+------+------+
 *  |      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  月  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
 *  |  年  |         BCD码十位         |         BCD码个位         |
 *  +------+------+------+------+------+------+------+------+------+
{*///

// 用户侧
typedef struct
{
    UINT8  ucYY;        // 年
    UINT8  ucMM;        // 月
    
}sMtFmt21, sMtYYMM;
 
// 帧侧
typedef struct
{
    UINT8  bcd_MM_0:4;  // 月
    UINT8  bcd_MM_1:4;
    UINT8  bcd_YY_0:4;  // 年
    UINT8  bcd_YY_1:4;
    
}sMtFmt21_f, sMtYYMM_f;

// 转换函数
eMtErr emt_trans_YYMM(eMtTrans eTrans, sMtYYMM* psUser, sMtYYMM_f* psFrame);
///*} 

/*********************************************
 *  数据格式22 对于表A.23
 *  格式: (+)X.X
 *  字长: 1
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |          BCD码个位        |        BCD码十分位        |
 *  +-------+------+------+------+------+------+------+------+------+

{*///

// 用户侧
// float fX_X;
 
// 帧侧
typedef struct
{
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
}sMtFmt22_f, sMtX_X_f;

// 转换函数
eMtErr emt_trans_X_X(eMtTrans eTrans, float* psUser, sMtFmt22_f* psFrame);
///*} 

/*********************************************
 *  数据格式23 对于表A.24
 *  格式: (+)X.X
 *  字长: 3
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码千分位        |        BCD码万分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |        BCD码十分位        |        BCD码百分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |          BCD码十位        |        BCD码个位          |
 *  +-------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
// float fXX_XXXX;
 
// 帧侧
typedef struct
{
    UINT8 BCD_0_4:4;  // BCD码万分位
    UINT8 BCD_0_3:4;  // BCD码十分位 
    UINT8 BCD_0_2:4;  // BCD码百分位 
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD码十位 
    UINT8 BCD_1  :4;  // BCD 个位
    
}sMtFmt23_f, sMtFmt23, sMtFmt_XX_XXXX;

// 转换函数
eMtErr emt_trans_XX_XXXX(eMtTrans eTrans, float* psUser, sMtFmt23_f* psFrame);
///*} 

/*********************************************
 *  数据格式25 对于表A.26
 *  格式: (+/-)XXX.XXX
 *  字长: 3
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码百分位        |        BCD码千分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |         BCD码个位         |        BCD码十分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |  S   |     BCD码百位      |         BCD码十位         |
 *  +-------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
// float sXXX.XXX    (+/-)XXX.XXX

// 帧侧
typedef struct
{
    UINT8 BCD_0_3:4;  // BCD 千分位
    UINT8 BCD_0_2:4;  // BCD 百分位
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    UINT8 BCD_1  :4;  // BCD 十位
    UINT8 BCD_2  :3;  // BCD 百位
    UINT8 S      :1;  // 符号位
    
}sMtFmt25, sMtFmt25_f, sMtFmt_sXXX_XXX;

// 转换函数
eMtErr emt_trans_sXXX_XXX(eMtTrans eTrans, float* psUser, sMtFmt25* psFrame);
///*} 


/*********************************************
 *  数据格式26 对于表A.27
 *  格式: (+)X.XXX
 *  字长: 3
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码百分位        |        BCD码千分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |         BCD码个位         |        BCD码十分位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
// float fX.XXX

// 帧侧
typedef struct
{
    UINT8 BCD_0_3:4;  // BCD 千分位
    UINT8 BCD_0_2:4;  // BCD 百分位
    UINT8 BCD_0_1:4;  // BCD 十分位
    UINT8 BCD_0  :4;  // BCD 个位
    
}sMtFmt26, sMtFmt_X_XXX;

// 转换函数
eMtErr emt_trans_X_XXX(eMtTrans eTrans, float* psUser, sMtFmt26* psFrame);
///*} 

/*********************************************
 *  数据格式27 对于表A.28
 *  格式: XXXXXXXX
 *  字长: 4
 *  +-------+------+------+------+------+------+------+------+------+
 *  |       |                       字节格式                        |
 *  + 名称  +------+------+------+------+------+------+------+------+
 *  |       |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE1 |        BCD码十位          |        BCD码个位          |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE2 |        BCD码千位          |        BCD码百位          |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE3 |        BCD码十万位        |        BCD码万位          |
 *  +-------+------+------+------+------+------+------+------+------+
 *  | BYTE4 |        BCD码千万位        |        BCD码百万位        |
 *  +-------+------+------+------+------+------+------+------+------+
 *
{*///

// 用户侧
// UINT32 ulXXXXXXXX

// 帧侧
typedef struct
{
    UINT8 BCD_8:4;  // BCD 个位
    UINT8 BCD_7:4;  // BCD 十位
    UINT8 BCD_6:4;  // BCD 百位
    UINT8 BCD_5:4;  // BCD 千位
    UINT8 BCD_4:4;  // BCD 万位
    UINT8 BCD_3:4;  // BCD 十万位
    UINT8 BCD_2:4;  // BCD 百万位
    UINT8 BCD_1:4;  // BCD 千万位
    
}sMtFmt_XXXXXXXX;

// 转换函数
eMtErr emt_trans_XXXXXXXX(eMtTrans eTrans, UINT32* psUser, sMtFmt_XXXXXXXX* psFrame);
///*} 

/******************************************************************************
 *  数据名称:数据冻结密度定义
 *  数据说明: 附录C
 * 
{*///

// 用户侧
typedef enum
{
    MT_FREZ_NULL  = 0,    // 不冻结
    MT_FREZ_15    = 1,    // 冻结间隔时间 15 分钟 (15分、30分、45分、0分)
    MT_FREZ_30    = 2,    // 冻结间隔时间 30 分钟 (30分、0分)
    MT_FREZ_60    = 3,    // 冻结间隔时间 60 分钟 (0分)
    MT_FREZ_5     = 254,  // 冻结间隔时间 5  分钟 (5分、10分、15分......、0分)
    MT_FREZ_1     = 255,  // 冻结间隔时间 1  分钟 (1分、2分、3分、......0分)
 
}eMtFmtFrez;

// 帧侧
// UINT8 ucFrez;

// 转换函数
eMtErr emt_trans_fmt_freeze(eMtTrans eTrans, eMtFmtFrez* psUser, UINT8* psFrame);
///*} 

/******************************************************************************
 *  数据名称:float类型的
 *  数据说明: 
 *  浮点数在机内用指数型式表示，分解为：数符，尾数，指数符，指数四部分。 
 *  无论是单精度还是双精度在存储中都分为三个部分：
 *    1.符号位(Sign)         : 0代表正，1代表为负 
 *    2.指数位（Exponent）   : 用于存储科学计数法中的指数数据，并且采用移位存储 
 *    3.尾数部分（Mantissa） : 尾数部分 
{*///
typedef union
{
    float v;
    struct
    {
        unsigned mantissa1 : 23;
        unsigned exponent  : 8;
        unsigned sign      : 1;  
    }s;

}uMtFloatFmt;

// 取得一个flaot的符号位  sign = -1 / +1
int    bMtGetFloatSign(float fVal);      

// 设置一个浮点数的符号位 sign = -1 / +1
void   vMtSetFloatSign(float *pfVal, int sign);  
///*}

/******************************************************************************
 * 数据名称: double 类型的合成与解合成
 * 数据说明: 
 * 浮点数在机内用指数型式表示，分解为：数符，尾数，指数符，指数四部分。 
 * 无论是单精度还是双精度在存储中都分为三个部分：
 *    1.符号位(Sign)         : 0代表正，1代表为负 
 *    2.指数位（Exponent）   : 用于存储科学计数法中的指数数据，并且采用移位存储 
 *    3.尾数部分（Mantissa） : 尾数部分 
 *
{*///

typedef union 
{
    double v;
    struct 
    {
        unsigned mantissa2 : 32;
        unsigned mantissa1 : 20;
        unsigned exponent  : 11;
        unsigned sign      :  1;
    } s;
 
}uMtDoubleFmt;

// 取得一个double的符号位 sign = -1 / +1
int    bMtGetDoubleSign(double dVal);       
 
// 设置一个浮点数的符号位 sign = -1 / +1
void   vMtSetDoubleSign(double *pdVal, int sign);  
///*}
///*} 

/************************************************************
 *  子命令信息结构
 *
{*///
typedef enum
{
    MT_ROLE_UNKOWN,    // 未知身份
    MT_ROLE_MASTER,    // 主站
    MT_ROLE_CONTOR,    // 集中器 或 终端
    
}eMtRole;              // 协议应用模块的身份
 
typedef enum
{
    MT_DIR_UNKOWN,     // 未知报文方向
    MT_DIR_M2S,        // 主站到集中器或终端  下行
    MT_DIR_S2M,        // 集中器或终端到主站  上行
    
}eMtDir;               // 报文的发送方向 

typedef enum
{
    MT_PN_UNKOWN,      // 未知PN类型
    MT_PN_P0,          // P0 不关心PN 集中器本身
    MT_PN_MP,          // 测量点号(measured point)
    MT_PN_GT,          // 总加组号(group total)
    MT_PN_CT,          // 控制轮次(control times)
    MT_PN_TK,          // 任务号  (task number)
    MT_PN_DP,          // 直流模拟量端口号 (direct current simulation port number)
    MT_PN_TM,          // 终端号
    
}eMtPn;

typedef struct
{    
    eMtCmd  eCmd;
    eMtDir  eDir;
    eMtPn   ePn;
    pMtFunc pFunc;
    char*   pName;    
}sMtCmdInfor;    
 ///*}

/************************************************************
 *  数据标识单元
 *  每个数据标识单元的信息是组的形式组织的
 *  每个数据标识单元包含最多(8)个Fn 及最多包含(8)个Pn
 *
 * -----
 * DADT 帧侧结构
 * 信息点 | DA | DA1 | D7 D6 D5 D4 D3 D2 D1 D0
 *        |    | DA2 | D7 D6 D5 D4 D3 D2 D1 D0
 * 信息类 | DT | DT1 | D7 D6 D5 D4 D3 D2 D1 D0
 *        |    | DT2 | D7 D6 D5 D4 D3 D2 D1 D0
 *
 * ---
 * 信息点DA由信息点元DA1和信息点组DA2两个字节构成
 * DA1 对位表示某一信息点组的1～8个信息点
 * DA2 采用二进制编码方式表示信息点组
 * DA1、DA2共同构成信息点标识pn（n = 1～2040）
 * 信息点标识pn对应于不同信息类标识Fn可以是以下各种含义
 * 测量点号、总加组号、控制轮次、直流模拟量端口号、任务号
 *
 * 信息点组DA2 | 信息点元DA1
 *       D7~D0 | D7    D6    D5    D4    D3    D2    D1    D0
 *           1 | p8    p7    p6    P5    p4    p3    p2    p1
 *           2 | p16   p15   p14   p13   p12   p11   p10   p9
 *           3 | p24   p23   p22   p21   p20   p19   p18   p17
 *          .. | ...   ...   ...   ...   ...   ...   ...   ...
 *          255| P2040 P2039 P2038 P2037 P2036 P2035 P2034 P2033
 *
 * ---
 * 信息类DT由信息类元DT1和信息类组DT2两个字节构成
 * DT1 对位表示某一信息类组的1～8种信息类型
 * DT2 采用二进制编码方式表示信息类组
 * DT1 DT2 共同构成信息类标识Fn（n = 1～248）
 *
 * 信息类组DT2 | 信息类元D T1
 *       D7~D0 | D7   D6   D5   D4   D3   D2   D1   D0
 *           0 | F8   F7   F6   F5   F4   F3   F2   F1
 *           1 | F16  F15  F14  F13  F12  F11  F10  F9
 *           2 | F24  F23  F22  F21  F20  F19  F18  F17
 *          .. | ...  ...  ...  ...  ...  ...  ...  ...
 *          30 | F248 F247 F246 F245 F244 F243 F242 F241
 *
{*///

// 用户侧结构
typedef struct
{
    
    UINT16  usPn[PN_INDEX_MAX];         // Pn (0 ~ 2040)
                                        // MT_PN_MAX  最大值
                                        // MT_PN_MIN  最小值
                                        // MT_PN_NONE 无效值
                                         
    UINT8   ucFn[FN_INDEX_MAX];         // Fn (1 ~ 248) 
                                        // MT_FN_MAX  最大值
                                        // MT_FN_MIN  最小值
                                        // MT_FN_NONE 无效值
}sMtPnFn;

// 帧侧结构
typedef struct
{
    UINT8       ucDA1;         // 信息点元
    UINT8       ucDA2;         // 信息点组
    UINT8       ucDT1;         // 信息类元
    UINT8       ucDT2;         // 信息类组
    
}sMtDaDt;

// 转换函数
eMtErr emt_pnfn_to_dadt(sMtPnFn* psPnFn, sMtDaDt* psDaDt);
eMtErr emt_dadt_to_pnfn(sMtDaDt* psDaDt, sMtPnFn* psPnFn);

typedef struct
{
    eMtCmd eCmd;
    UINT16 usPn; 
    
}sMtCmdPn;

// 转换函数
eMtErr emt_add_cmdpn(sMtCmdPn* psCmdPn,sMtCmdPn sNewCmdPn, UINT8 *pucNumCmdPn);
eMtErr emt_pnfn_to_cmdpn(eMtAFN eAfn, sMtPnFn* psPnFn, UINT8 ucNumPnFn,   sMtCmdPn* psCmdPn, UINT8 *pucNumCmdPn);
eMtErr emt_cmdpn_to_pnfn(eMtAFN eAfn, sMtPnFn* psPnFn, UINT8 *pucNumPnFn, sMtCmdPn* psCmdPn, UINT8  ucNumCmdPn);
eMtErr emt_dadt_to_cmdpn(eMtAFN eAfn, sMtDaDt* psDaDt, UINT8 ucNumDaDt,   sMtCmdPn* psCmdPn, UINT8 *pucNumCmdPn);
eMtErr emt_cmdpn_to_dadt(eMtAFN eAfn, sMtDaDt* psDaDt, UINT8 *pucNumDaDt, sMtCmdPn* psCmdPn, UINT8  ucNumCmdPn);
///*}

/************************************************************
 *  地址域结构
 *
{*///
// 用户侧结构
typedef struct
{
    UINT8  acRegionCode[4];    // 行政区划码A1 如北京 1100, 行政区划码按GB 2260-91的规定执行
    UINT16 usTAddress;         // 终端或集中器地址A2 范围(1 ~ 65535)
    BOOL   bTeamAddr;          // 为true 表示ulTAddress 为组地址, 为false 表示ulTAddress单一地址
    UINT8  ucMAddress;         // 主站地址和组地址标志A3, 范围(0~127)
    
}sMtAddress;

// 封帧侧结构
#pragma pack(1) 
typedef struct
{
    UINT8  acRegionCode[2];    // 行政区划码A1           (BCD)  低字节在前  高字节在后
    UINT16 usTAddress;         // 终端地址A2             (BIN)  范围(1 ~ 65535)
    //UINT8  ucTeamAddr:1;     // 为1表示 usTerminalAddress 为组地址,A2 == 0xFFFF即广播地址, 为 0 表示单地址
    UINT8  ucMAddress;         // 主站地址和组地址标志A3 (BIN)  最好不要用位域 
    
}sMtAddress_f; // 帧地址格式定义
#pragma pack()

// 转换函数
eMtErr emt_trans_address(eMtTrans eTrans, sMtAddress *psAddr_u, sMtAddress_f *psAddr_f);
///*}

/************************************************************
 * 帧序列域 SEQ
 * 作用: 用于描述帧之间的传输序列的变化规则
 * 长度: 帧侧为1字节
 * 结构: 
 *     +------+------+------+------+------+------+------+------+
 *     |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *     +------+------+------+------+------+------+------+------+
 *     |  TpV |  FIR |  FIN |  CON |         SEQ / RSEQ        |
 *     +------+------+------+------+------+------+------+------+
 *
 * -------------
 * TpV 帧时间标签有效位
 *     TpV=0：表示在附加信息域中无时间标签Tp
 *     TpV=1：表示在附加信息域中有时间标签Tp
 *
 * -------------
 * FIR 首帧标志 置"1",报文的第一帧
 * FIN 末帧标志 置"1",报文的最后一帧  
 *
 *     +-----+-----+------------------------+
 *     | FIR | FIN |        应用说明        |  
 *     +-----+-----+------------------------+
 *     |  0  |  0  |      多帧：中间帧      |  
 *     +-----+-----+------------------------+
 *     |  0  |  1  |      多帧：结束帧      |  
 *     +-----+-----+------------------------+
 *     |  1  |  0  |  多帧：第1帧 有后续帧  |  
 *     +-----+-----+------------------------+
 *     |  1  |  1  |          单帧          |  
 *     +-----+-----+------------------------+
 *
 * -------------
 * PFC 启动帧帧序号计数器
 *     每一对启动站和从动站之间均有1个独立的、由1字节构成的计数范围
 *     为0～255的启动帧帧序号计数器PFC,用于记录当前启动帧的序号
 *     启动站每发送1帧报文该计数器加1,从0～255循环加1递增,重发帧则不加1  
 * 
 * -------------
 *
 * PSEQ 启动帧序号
 *      PSEQ 取自1字节的启动帧计数器PFC的低4位计数值0～15
 *
 * RSEQ 响应帧序号
 *      响应帧序号RSEQ以启动报文中的PSEQ作为第一个响应帧序号
 *      后续响应帧序号在RSEQ的基础上循环加1递增,数值范围为0～15
 * 
 * 
{*///

typedef enum
{
    MT_POS_UNKOWN,    // 未知帧位置信息
    MT_POS_SIGLE,     // 单帧
    MT_POS_MUL_S,     // 多帧起始帧，有后续帧
    MT_POS_MUL_M,     // 多帧中间帧
    MT_POS_MUL_E,     // 多帧最后帧
    
}eMtPos;              // 帧的位置类型

// 用户侧
typedef struct
{
    BOOL     bTpv;     // 表示帧中有没有 时间标签 Tp
    BOOL     bCon;     // 是否需要确认
    eMtPos   ePos;     // 位置
    UINT8    ucSeq;    // pseq/rseq (0~15) 
    
}sMtSEQ;

// 帧侧
typedef struct
{
    UINT8   SEQ : 4;
    UINT8   CON : 1;
    UINT8   FIN : 1;
    UINT8   FIR : 1;
    UINT8   TPV : 1;
    
}sMtSEQ_f;

// 转换函数
eMtErr emt_trans_seq(eMtTrans eTrans, sMtSEQ *puSEQ, sMtSEQ_f *pfSEQ);
///*}

/************************************************************
 *  控制域结构及转换函数 
 *
 *  封帧侧为1字节
 *      +------+------+------+------+------+------+------+------+
 *      |  D7  |  D6  |  D5  |  D4  |  D3  |  D2  |  D1  |  D0  |
 *      +------+------+------+------+------+------+------+------+
 * 下行 |  DIR | PRM  |  FCB |  FCV |           FUNCODE         |
 *------+------+------+------+------+------+------+------+------+
 * 上行 |  DIR | PRM  |  ACD |      |           FUNCODE         |
 *      +------+------+------+------+------+------+------+------+
 *
 * DIR 传输方向位   DIR = 0 表示此帧报文是由主站发出的下行报文 
 *                  DIR = 1 表示此帧报文是由终端发出的上行报文
 *  
 * PRM 启动标志位   PRM = 1 表示此帧报文来自启动站
 *                  PRM = 0 表示此帧报文来自从动站
 *
 * FCB 帧计数位     (只存在于下行报文中) 
 *                  当帧计数有效位FCV=1时,FCB表示每个站连续的发送/确认
 *                  或者请求/响应服务的变化位, FCB位用来防止信息传输的丢失和重复
 *                  复位命令中的FCB=0，从动站接收复位命令后将FCB置"0"
 *
 * FCV 帧计数有效位 (只存在于下行报文中) 
 *                  FCV = 1 表示FCB位有效
 *                  FCV = 0 表示FCB位无效
 *
 * ACD 要求访问位   (只存在于上行报文中) 
 *                  ACD = 1 表示终端有重要事件等待访问
 *                  ACD = 0 表示终端无事件数据等待访问
 * 
 * FUNCODE 功能码 报文的启动站及从动站的功能码定义不同
 *
{*///

typedef enum
{
    MT_PRM_UNKWON,                   // 未知方向
    MT_PRM_ACTIVE,                   // 该帧来自于主动站
    MT_PRM_PASIVE,                   // 该帧来自于从动站
    
}eMtPRM;                             // 控制域中PRM字段描述 

typedef enum
{
    MT_FCD_RPM1_RESET     = 1,        // 复位命令
    MT_FCD_RPM1_USER      = 4,        // 用户数据
    MT_FCD_RPM1_LINK      = 9,        // 链路测试
    MT_FCD_RPM1_ASK_1     = 10,       // 请求1级数据
    MT_FCD_RPM1_ASK_2     = 11,       // 请求2级数据
    
}eMtFcodeRpm1;                        // 上行报文控制域功能码

typedef enum
{
    MT_FCD_RPM0_OK        = 0,        // 认可
    MT_FCD_RPM0_USER      = 8,        // 用户数据
    MT_FCD_RPM0_NONE      = 9,        // 否认:无所召唤的数据
    MT_FCD_RPM0_LINK      = 11,       // 链路状态
    
}eMtFcodeRpm0;                        // 下行报文控制域功能码

// 用户侧
typedef struct
{
    eMtDir     eDir;                   // 上下行
    eMtPRM     ePRM;                   // 标志该报文是来自启动站 还是从动站
    BOOL       bFcv;                   // 帧计数有效位FCV
    BOOL       bAcd_Fcb;               // ACD 要求访问位有效 (bFcv 为假时)
                                       // 自上次收到报文后发生新的重要事件，ACD位置"1"；
                                       // 收到主站请求事件报文并执行后，ACD位置"0"。
                                       // FCB 帧计数位  有效 (bFcv 为真时) 
                                       // 启动站向同一从动站传输新的发送/确认或请求/响应传输服务时，将FCB取相反值。
                                       // 启动站保存每一个从动站FCB值，若超时未收到从动站的报文，或接收出现差错，
                                       // 则启动站不改变FCB的状态，重复原来的发送/确认或者请求/响应服务。
                                       // 复位命令中的FCB=0，从动站接收复位命令后将FCB置"0"。
    union
    {
        eMtFcodeRpm0 eFcdPrm0;         // 从动站
        eMtFcodeRpm1 eFcdPrm1;         // 主动站
    }uFCode;

}sMtCtrl;

// 帧侧
// UINT8 ucMtCtrl;

// 转换函数
eMtErr emt_trans_ctrl(eMtTrans eTrans, sMtCtrl* puCtrl, UINT8* pfCtrl);

// 封装函数
eMtErr emt_get_ctrl(eMtAFN eAFN, eMtDir eDir, eMtPRM ePRM, BOOL bAcd_Fcb, sMtCtrl *psCtrl);
///*}

/************************************************************
 *  事件计数器EC
 *
{*///
typedef struct
{
    UINT8  ucEC1;                      // 重要事件计数器
    UINT8  ucEC2;                      // 一般事件计数器   
}sMtEC;                                // 事件计数器  
///*}

/************************************************************
 *  帧侧时间标签格式TP
 *
{*///
// 用户侧结构
typedef struct
{
    UINT8           ucPFC;                 // 启动帧帧序号计数器PFC
    UINT8           ucPermitDelayMinutes;  // 允许发送传输延时时间
    sMtDDHHmmss     sDDHHmmss;             // 启动帧发送时标
}sMtTP;                          

// 帧侧结构
#pragma pack(1) 
typedef struct
{
     UINT8          ucPFC;                 // 启动帧帧序号计数器PFC
     sMtDDHHmmss_f  sDDHHmmss;             // 启动帧发送时标
     UINT8          ucPermitDelayMinutes;  // 允许发送传输延时时间
}sMtTP_f;           
#pragma pack()

// 转换函数
eMtErr emt_trans_tp(eMtTrans eTrans, sMtTP* pTP_u, sMtTP_f* pTP_f);

// 获得当前用户侧Tp原始数据 
eMtErr emt_get_tp(UINT8 ucPFC, sMtTP *psuTp);   

// 以当前时间为基准判断一个tp是否超时
BOOL  bmt_tp_timeout(sMtTP *psTP);

// 获得当前报文类型对应的aux(tp ec pw)总字长
UINT16 usmt_get_aux_len(eMtAFN eAFN, eMtDir eDir, BOOL bEc, BOOL bTp); 
///*}

/************************************************************
 *  内部接口
 *
{*///
// 通过命令类型和报文方向获得该命令对应的相关信息
eMtErr eMtGetCmdInfor(eMtCmd eCmd, eMtDir eDir, sMtCmdInfor* psInfor);

/*******************************************
 *  数据项相关
 *  协议允许应答中每个实际的数据项为空,在帧侧需要将该数据内容域填充0xEE
{*///
BOOL   bmt_is_0xEE(UINT8* pData, UINT16 usLen);   // 判断帧侧某数据项是否是无效的
void   vmt_set_0xEE(UINT8* pData, UINT16 usLen);  // 将帧侧某数据项设置为无效

BOOL   bmt_is_none(UINT8* pData, UINT16 usLen);   // 数据项缺少
void   vmt_set_none(UINT8* pData, UINT16 usLen);  // 将数据项设置为缺少
///*}

/*******************************************
 *  附加域
 *
{*///
BOOL   bmt_have_pw(eMtAFN eAFN, eMtDir eDir);  // 此类报文中是否应该有pw字段
BOOL   bmt_have_ec(eMtAFN eAFN, eMtDir eDir);  // 此类报文中是否应该有EC字段
BOOL   bmt_have_tp(eMtAFN eAFN, eMtDir eDir);  // 此类报文中是否应该有tp字段
BOOL   bmt_need_con(eMtAFN eAFN, eMtDir eDir); // 此类报文是否需要确认
///*}

/*******************************************
 * 逐个确认与否认
 *
{*///
// 用户侧数据

typedef struct
{
    eMtCmd      eCmd;         // 
    UINT16      usPn;         // Pn  0 ~ 2040
    BOOL        bOk;
}sMtCmdErr;                   // 确认与否认 用户侧数据结构


typedef struct
{
    eMtAFN      eAFN;          // 需要确认的AFN
    UINT8       ucNum;         // 需要确认 或 否认的 Fn个数
    sMtCmdErr   sOne[1];       // ucNum 个sMtUFnPnErr  
}sMtOnebyOne;                  // 确认与否认 (用户侧数据结构)

// 帧侧数据
#pragma pack(1) 
typedef struct
{
    sMtPnFn     sPnFn;
    BOOL        bOk;
}sMtFnPnErr;                   // 确认与否认 用户侧数据结构

typedef struct
{
    sMtDaDt     sDaDt;         // 数据单元标识
    UINT8       ucErr;         // 错误 标识   (0 正确 1 错误)
}sMtOne_f;                     // 确认与否认  帧侧数据结构   

typedef struct
{
    UINT8       ucAFN;
    sMtOne_f    sOne[1];
}sMtOneByOne_f;
#pragma pack()
// 转换函数
eMtErr emtTrans_OneByOne(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 中继站工作状态控制
 * 对应AFN : AFN_03_RELY
 * 对应CMD : CMD_AFN_3_F1_RELAY_CTRL
 * PN 类型 : P0
{*///

// 用户侧
typedef struct
{
    // 未实现
    UINT8 ucData;
}sMtRelayStateCtrl, sMtAfn03F1;   // 中继站工作状态控制

// 帧侧
// UINT8 ucRelayCtrl


// 转换函数
eMtErr emtTrans_afn03f1(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F1：终端上行通信口通信参数设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F1_TML_UP_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F1_TML_UP_CFG 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8  ucRTS;             // 终端数传机延时时间RTS
    UINT8  ucRTM;             // 终端作为启动站允许发送传输延时时间 slave recieve time out minute
    UINT8  ucReSendTimes;     // 重发次数 (0~3)   0 表示不允许重发
    UINT8  ucHeartBeat;       // 心跳周期
    UINT16 usTmWaitTimeoutS;  // 集中器等待从动站响应的超时时间 秒数 0~4095
    BOOL   bAutoReportAsk_1;  // 允许自动上报1类数据，同时需要主站确认
    BOOL   bAutoReportAsk_2;  // 允许自动上报2类数据，同时需要主站确认
    BOOL   bAutoReportAsk_3;  // 允许自动上报3类数据，同时需要主站确认
    
}sMtTmlUpCfg, sMtAfn04F1;   

// 帧侧
typedef struct
{
    UINT8  ucRTS;             // 终端数传机延时时间RTS
    UINT8  ucRTM;             // 终端作为启动站允许发送传输延时时间 slave recieve time out minute
    UINT8  ucSWTS1;           // 终端等待从动站响应的超时时间和重发次数 wait ts
    UINT8  ucSWTS2;           // 终端等待从动站响应的超时时间和重发次数 wait ts
    UINT8  bs8Con;            // 需要主站确认的通信服务（CON=1）的标志
    UINT8  ucHeartBeat;       // 心跳周期

}sMtTmlUpCfg_f, sMtAfn04F1_f;   

// 转换函数
eMtErr emtTrans_afn04f1(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F2：终端上行通信口无线中继转发设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F2_TML_WIRELESS_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F2_TML_WIRELESS_CFG 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    BOOL   bPermit;           // 允许∕禁止：由第1字节的 D7表示允许或禁止终端转发，置"0"：禁止，置 "1"：允许。
    UINT8  ucAddrCount;       // 被转发的终端地址数n：由第1字节的D0～D6位表示，数值范围0～16。
    UINT16 usAddress[16];     // 被转发终端地址：每个地址为2字节，与终端地址A2定义相同

}sMtTmlConUpWireless, sMtAfn04F2;   

// 帧侧
typedef struct
{
    UINT8  ucPermit   :1;      // 允许∕禁止：由第1字节的 D7表示允许或禁止终端转发，置"0"：禁止，置 "1"：允许。
    UINT8  ucAddrCount:7;      // 被转发的终端地址数n：由第1字节的D0～D6位表示，数值范围0～16。
    UINT16 usAddress[1];       // 被转发终端地址：每个地址为2字节，与终端地址A2定义相同
    
}sMtTmlConUpWireless_f, sMtAfn04F2_f;   

// 转换函数
eMtErr emtTrans_afn04f2(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F3：主站IP地址和端口
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F3_MST_IP_PORT
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F3_MST_IP_PORT 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT32  ulMainIP;        // 主用地址
    UINT16  usMainPort;      // 主用端口
    UINT32  ulBackIP;        // 备用地址
    UINT16  usBackPort;      // 备用端口 
    UINT8   ucascAPN[16];    // APN

}sMtMasterIpPort, sMtAfn04F3;   

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucMainIP[4];     // 主用地址
    UINT16 usMainPort;      // 主用端口
    UINT8  ucBackIP[4];     // 备用地址
    UINT16 usBackPort;      // 备用端口 
    UINT8  ucascAPN[16];    // APN
    
}sMtMasterIpPort_f, sMtAfn04F3_f;   
#pragma pack()

// 转换函数
eMtErr emtTrans_afn04f3(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F4：主站电话号码和短信中心号码
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F4_MST_PHONE_SMS
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F4_MST_PHONE_SMS 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8 ucPhoneMaster[17]; // 主站电话号码         最长支持16位电话号码
    UINT8 ucPhoneSMS[17];    // 短信服务中心电话号码 最长支持16位电话号码
    
}sMtMasterPhoneSms, sMtAfn04F4;   

// 帧侧
typedef struct
{
    UINT8 bcdPhoneMaster[8];
    UINT8 bcdPhoneSMS[8]; 
    
}sMtMasterPhoneSms_f, sMtAfn04F4_f;   

// 转换函数
eMtErr emtTrans_afn04f4(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F5：终端上行通信消息认证参数设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F5_TML_UP_AUTH
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F5_TML_UP_AUTH 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlUpAuth, sMtAfn04F5;   

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlUpAuth_f, sMtAfn04F5_f;   
#pragma pack()

// 转换函数
eMtErr emtTrans_afn04f5(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F6：终端组地址设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F6_TEAM_ADDR
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F6_TEAM_ADDR 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlTeamAddr, sMtAfn04F6;   

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlTeamAddr_f, sMtAfn04F6_f;   
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f6(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F7：终端IP地址和端口
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F7_TML_IP_PORT
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F7_TML_IP_PORT 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlIpPort, sMtAfn04F7;   

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlIpPort_f, sMtAfn04F7_f;   
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f7(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F8：终端上行通信工作方式（以太专网或虚拟专网）
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F8_TML_UP_WAY
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F8_TML_UP_WAY 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlUpWay, sMtAfn04F8;   

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlUpWay_f, sMtAfn04F8_f;   
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f8(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F9：终端事件记录配置设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F9_TML_EVENT_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F9_TML_EVENT_CFG 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlEventCfg, sMtAfn04F9;   

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucSchemeNum; 
    UINT16 usPara;
    
}sMtTmlEventCfg_f, sMtAfn04F9_f;   
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f9(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F10：终端电能表/交流采样装置配置参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F10_TML_POWER_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F10_TML_POWER_CFG 对应的上行结构相同
{*///

#define  MT_PAYRATE_NUM_MIN     (1)  // 费率数最小值
#define  MT_PAYRATE_NUM_MAX     (48) // 费率数最大值

typedef enum
{
    MT_BAUD_DEFAULT,             // 0：表示无需设置或使用默认的
    MT_BAUD_600,                 // 波特率 600
    MT_BAUD_1200,                // 波特率 1200
    MT_BAUD_2400,                // 波特率 2400
    MT_BAUD_4800,                // 波特率 4800
    MT_BAUD_7200,                // 波特率 7200
    MT_BAUD_9600,                // 波特率 9600
    MT_BAUD_19200,               // 波特率 19200

}eMtBaudRate;                    // 波特率类型定义

#define MT_METER_ID_MIN          (1)
#define MT_METER_ID_MAX          (2040)

typedef enum
{
    MT_PROTO_DEFAULT,            // 表示终端无需对本序号的电能表/交流采样装置进行抄表
    MT_PROTO_DLT645_97,          // DL/T 645-1997
    MT_PROTO_DLT645_07,          // DL/T 645-2007
    MT_PROTO_AC,                 // 交流采样装置通信协议
    MT_PROTO_SERIAL,             // "串行接口连接窄带低压载波通信模块"接口协议
    MT_PROTO_UNKOWN,             // 其他：备用

}eMtMeterProto;                  // 通信协议类型


#define MT_DNUM_INTGR_MIN       (4) // 整数位数最小值
#define MT_DNUM_INTGR_MAX       (7) // 整数位数最大值

#define MT_DNUM_DECIM_MIN       (1) // 小数位数最小值
#define MT_DNUM_DECIM_MAX       (4) // 小数位数最大值

typedef struct
{
    UINT8       ucInteger;       // 通信接入的电能表的有功电能示值的整数位个数(4~7)
    UINT8       ucDecimal;       // 通信接入的电能表的有功电能示值的小数位个数(1~4)
}sMtDigitNum;                    // 整数与小数位数


#define MT_METER_PORT_MIN       (1)  // 通信端口号最小值
#define MT_METER_PORT_MAX       (31) // 通信端口号最大值

typedef struct
{
    eMtBaudRate    eBaudRate;        // 编码表示电能表、交流采样装置与终端的通信波特率
    UINT8          ucPort;           // 电能表、交流采样装置与终端连接所对应的终端通信端口号，数值范围1～31，其他值无效。
}sMtBaudPort;                        // 通信速率及端口号

#define MT_USER_CLASS_MIN        (0)  // 用户大类名最小值 
#define MT_USER_CLASS_MAX        (15) // 用户大类名最大值
#define MT_USER_CLASS_NUM        (16)
typedef struct
{
    UINT8        ucBig;          // 编码表示本电能表所属的用户大类号，数值范围0～15，
                                 // 依次表示16个用户大类号
    UINT8        ucSmall;        // 编码表示本电能表所属的用户小类号，数值范围0～15，
                                 // 依次表示16套1类和2类数据项的配置（应在终端支持的1类和2类数据配置范围内）。
}sMtUserClass;                   // 用户类号


typedef struct
{
    // 有无字段
    BOOL           busMeterID;          // 电能表/交流采样装置序号：数值范围1～2040
    BOOL           busPn;               // 所属测量点号：数值范围0～2040，如为"0"，
    BOOL           bsBaudPort;          // 通信速率及端口号
    BOOL           beProto;             // 通信协议类型
    BOOL           bacMeterAddr;        // 通信地址：数值范围0～999999999999。
    BOOL           bacPwd;              // 通信密码：终端与电能表通信的密码
    BOOL           bucPayRateNum;       // 电能费率个数 表示通信接入的测量点的电能费率个数，数值范围1～48
    BOOL           bsDigit;             // 有功电能示值整数位及小数位个数
    BOOL           bacGathrAddr;        // 所属采集器通信地址：数值范围0～999999999999，如为"0"，表示本电能表未通过采集器连接
    BOOL           bsUserClass;         // 用户大类号及用户小类号

    // 实际数据
    UINT16         usMeterID;           // 电能表/交流采样装置序号：数值范围1～2040 MT_METER_ID_MIN
    UINT16         usPn;                // 所属测量点号：数值范围0～2040，如为"0"，
    sMtBaudPort    sBaudPort;           // 通信速率及端口号
    eMtMeterProto  eProto;              // 通信协议类型
    UINT8          acMeterAddr[12];     // 通信地址：数值范围0～999999999999。
    UINT8          acPwd[6];            // 通信密码：终端与电能表通信的密码
    UINT8          ucPayRateNum;        // 电能费率个数 表示通信接入的测量点的电能费率个数，数值范围1～48
    sMtDigitNum    sDigit;              // 有功电能示值整数位及小数位个数
    UINT8          acGathrAddr[12];     // 所属采集器通信地址：数值范围0～999999999999，如为"0"，表示本电能表未通过采集器连接
    sMtUserClass   sUserClass;          // 用户大类号及用户小类号
    
}sMtTmlPowerCfgOne;                     // 终端电能表/交采装置配置数据单元格式


// 用户侧
typedef struct
{
    UINT16             usNum;        // 本次电能表/交流采样装置配置数量n：数值范围0～2040
    sMtTmlPowerCfgOne  sOne[1];      // usCfgNum 个配置信息
    
}sMtTmlPowerCfg, sMtAfn04F10;   

// 帧侧
#pragma pack(1) 
typedef struct
{    
    UINT8          ucPort    :5;    // 电能表、交流采样装置与终端连接所对应的终端通信端口号，数值范围1～31，其他值无效。
    UINT8          ucBaudRate:3;    // 编码表示电能表、交流采样装置与终端的通信波特率
    
}sMtBaudPort_f;                     // 通信速率及端口号

typedef struct
{   
    UINT8       ucDecimal:2;        // D1～D0：编码表示通信接入的电能表的有功电能示值的小数位个数，数值范围0～3依次表示1～4位小数。
    UINT8       ucInteger:2;        // D3～D2：编码表示通信接入的电能表的有功电能示值的整数位个数，数值范围0～3依次表示4～7位整数。
    UINT8       ucRev    :4;        //  D7～D6：备用。
}sMtDigitNum_f;  


typedef struct
{ 
    UINT8        ucSmall:4;       // 编码表示本电能表所属的用户小类号，数值范围0～15，
                                  // 依次表示16套1类和2类数据项的配置（应在终端支持的1类和2类数据配置范围内）。
    UINT8        ucBig  :4;       // 编码表示本电能表所属的用户大类号，数值范围0～15，
                                  // 依次表示16个用户大类号
}sMtUserClass_f;                  // 用户类号

typedef struct
{
    UINT16         usMeterID;           // 电能表/交流采样装置序号：数值范围1～2040
    UINT16         usPn;                // 所属测量点号：数值范围0～2040，如为"0"，
    sMtBaudPort_f  sBaudPort;           // 通信速率及端口号
    UINT8          ucProto;             // 通信协议类型
    sMtFmt_XX_6    acMeterAddr;         // 通信地址：数值范围0～999999999999。
    UINT8          acPwd[6];            // 通信密码：终端与电能表通信的密码
    UINT8          ucPayRateNum;        // 电能费率个数 表示通信接入的测量点的电能费率个数，数值范围1～48
    sMtDigitNum_f  sDigit;              // 有功电能示值整数位及小数位个数
    sMtFmt_XX_6    acGathrAddr;         // 所属采集器通信地址：数值范围0～999999999999，如为"0"，表示本电能表未通过采集器连接
    sMtUserClass_f sUserClass;          // 用户大类号及用户小类号

}sMtTmlPowerCfgOne_f; 

typedef struct
{
    UINT16              usNum;        // 本次电能表/交流采样装置配置数量n：数值范围0～2040
    sMtTmlPowerCfgOne_f sOne[1];      // usCfgNum 个配置信息
    
}sMtTmlPowerCfg_f, sMtAfn04F10_f;   
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f10(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F11：终端脉冲配置参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F11_TML_PULSE_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F11_TML_PULSE_CFG 对应的上行结构相同
{*///

// 用户侧
#define MT_PULSE_CFG_MIN    (0)
#define MT_PULSE_CFG_MAX    (64)

#define MT_PULSE_PORT_MIN   (1)   // 脉冲输入端口号 最小值
#define MT_PULSE_PORT_MAX   (64)  // 脉冲输入端口号 最大值

typedef enum
{
    MT_PULSE_FORTH_HAVE,    // 正向有功
    MT_PULSE_FORTH_NONE,    // 正向无功
    MT_PULSE_BACK_HAVE,     // 反向有功
    MT_PULSE_BACK_NONE,     // 反向无功
    
}sMtPulse;                  // 脉冲类型

typedef struct
{
    UINT8     ucPort;           // 脉冲输入端口号：脉冲接入终端的脉冲输入端口号，数值范围1～64。
    UINT8     ucPn;             // 所属测量点号：数值范围0～64，如为"0"，表示本端口的脉冲未指向任何测量点，即被"删除"。
    sMtPulse  ePulse;           // 脉冲类型
    UINT16    usK;              // 电表常数k
    
}sMtTmlPulseCfgOne;

typedef struct
{
    UINT8             ucNum;            // 本次脉冲配置路数n：数值范围0～64  MT_PULSE_CFG_MAX
    sMtTmlPulseCfgOne aTmlPulseCfg[1];  // ucNum个数所

}sMtTmlPulseCfg,sMtAfn04F11;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucPort;           // 脉冲输入端口号：脉冲接入终端的脉冲输入端口号，数值范围1～64。
    UINT8  ucPn;             // 所属测量点号：数值范围0～64，如为"0"，表示本端口的脉冲未指向任何测量点，即被"删除"。
    UINT8  ucPulse;          // 脉冲类型
    UINT16 usK;              // 电表常数k

}sMtTmlPulseCfgOne_f;

typedef struct
{
    UINT8              ucNum;             // 本次脉冲配置路数n：数值范围0～64。
    sMtTmlPulseCfgOne_f aTmlPulseCfg[1];  // ucNum个数所

}sMtTmlPulseCfg_f,sMtAfn04F11_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f11(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F12：终端状态量输入参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F12_TML_STATE_INPUT
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F12_TML_STATE_INPUT 对应的上行结构相同
{*///

typedef enum
{
    MT_INPUT_UNKOWN,   // 未知
    MT_INPUT_ON,       // 接入
    MT_INPUT_OFF       // 未接入
    
}eMtStateInput;        // 状态量接入标志

typedef enum
{
    MT_ATTR_UNKOWN,    // 未知
    MT_ATTR_ON,        // 常开触点
    MT_ATTR_OFF,       // 常闭触点
    
}eMtStateAttr;         // 状态量属性标志位

// 用户侧
typedef struct
{
    eMtStateInput aeStateInput[8];  // 1～8路状态量输入，置"1"：接入    置"0"：未接入
    eMtStateAttr  aeStateAttr[8];   // 1～8路状态量输入，置"1"常开触点  置"0"：常闭触点

}sMtTmlStateInput, sMtAfn04F12;

// 帧侧
typedef struct
{
    UINT8 ucStateInput;
    UINT8 ucStateAttr;

}sMtTmlStateInput_f, sMtAfn04F12_f;

// 转换函数
eMtErr emtTrans_afn04f12(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F13：终端电压/电流模拟量配置参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F13_TML_SIMULA_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F13_TML_SIMULA_CFG 对应的上行结构相同
{*///

typedef enum
{
    MT_SIMU_UNKOWN,     // 未知
    MT_SIMA_UA,         // A相电压
    MT_SIMA_UB,         // B相电压
    MT_SIMA_UC,         // C相电压
    MT_SIMA_IA,         // A相电流
    MT_SIMA_IB,         // B相电流
    MT_SIMA_IC,         // C相电流
    
}eMtSimulaType;         // 电压/电流模拟量属性

typedef struct
{
    UINT8         ucPort;       // 电压/电流模拟量输入端口号：电压/电流模拟量接入终端的模拟量输入端口号，数值范围1～64
    UINT8         ucPn;         // 所属测量点号：数值范围0～64，如为"0"，表示本端口的电压/电流模拟量未指向任何测量点，即被"删除"   
    eMtSimulaType eType;        // 电压/电流模拟量属性
    
}sMtTmlSimulaCfgOne;

// 用户侧
typedef struct
{
    UINT8               ucNum;       // 本次电压/电流模拟量配置路数n：数值范围0～64。
    sMtTmlSimulaCfgOne  sOne[1];     // ucNum 个配置信息
    
}sMtTmlSimulaCfg, sMtAfn04F13;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8         ucPort;       // 电压/电流模拟量输入端口号：电压/电流模拟量接入终端的模拟量输入端口号，数值范围1～64
    UINT8         ucPn;         // 所属测量点号：数值范围0～64，如为"0"，表示本端口的电压/电流模拟量未指向任何测量点，即被"删除"   
    UINT8         ucType;       // 电压/电流模拟量属性
}sMtTmlSimulaCfgOne_f;

typedef struct
{
    UINT8                 ucNum;       // 本次电压/电流模拟量配置路数n：数值范围0～64。
    sMtTmlSimulaCfgOne_f  sOne[1];     // ucNum 个配置信息
}sMtTmlSimulaCfg_f, sMtAfn04F13_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f13(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F14：终端总加组配置参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F14_TML_GRUP_TOTL
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F14_TML_GRUP_TOTL 对应的上行结构相同
{*///

typedef enum
{
    MT_MPDIR_UNKOWN,    // 未知
    MT_MPDIR_FORTH,     // 正向
    MT_MPDIR_BACK,      // 反向
}eMtMpDir;              // 表示测量点的正向还是反向有功/无功功率、有功/无功电能量参与总加的标志

typedef enum
{
    MT_MTOPER_PLUS,     // 加运算
    MT_MTOPER_SUBS,     // 减运算
}eMtMpOper;             // 参与总加运算的运算符标志

typedef struct
{
    UINT8         ucPort;       // 表示0～63，顺序对应第1～第64个测量点号。
    eMtMpDir      eMpDir;       // 表示测量点的正向还是反向有功/无功功率、有功/无功电能量参与总加的标志
    eMtMpOper     eMpOper;      // 表示参与总加运算的运算符标志；置"0"："加"运算；置"1"："减"运算。
}sMtMpDir;

typedef struct
{
    UINT8  ucGroupID;           // 总加组序号：数值范围1～8
    UINT8  ucM;                 // 总加组的测量点数量m：数值范围0～64，如为"0"，表示本总加组没有要参与总加的测量点，即被"删除"。
    sMtMpDir sOne[64];          // 第1 ~ M 个测量点号及总加标志
}sMtTmlGrupTotlOne;

// 用户侧
typedef struct
{
    UINT8               ucN;         // 本次总加组配置数量n：数值范围1～8。
    sMtTmlGrupTotlOne   sOne[1];     // ucN个配置信息
}sMtTmlGrupTotl, sMtAfn04F14;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucGroupID;           // 总加组序号：数值范围1～8
    UINT8  ucM;                 // 总加组的测量点数量m：数值范围0～64，如为"0"，表示本总加组没有要参与总加的测量点，即被"删除"。
    UINT8  ucOne[1];            // 第1 ~ M 个测量点号及总加标志
}sMtTmlGrupTotlOne_f;

typedef struct
{
    UINT8                 ucN;       // 本次电压/电流模拟量配置路数n：数值范围0～64。
    sMtTmlGrupTotlOne_f   sOne[1];   // ucN 个配置信息
}sMtTmlGrupTotl_f, sMtAfn04F14_f;
#pragma pack() 
// 转换函数
eMtErr emtTrans_afn04f14(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F15：有功总电能量差动越限事件参数设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F15_HAVE_DIFF_EVENT
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F15_HAVE_DIFF_EVENT 对应的上行结构相同
{*///

typedef enum
{
    MT_DIFF_TIME_UNKOWN, 
    MT_DIFF_TIME_15,     // 15分钟电量
    MT_DIFF_TIME_30,     // 30分钟电量
    MT_DIFF_TIME_60,     // 60分钟电量
    
}eMtDiffTime;            // 电能量的时间跨度

typedef enum
{
    MT_DIFF_COMPR_REL,     // 相对对比
    MT_DIFF_COMPR_ABS,     // 绝对对比
}eMtDiffCompr;             // 对比方法标志

typedef struct
{
    eMtDiffTime  eTime;   // 电能量的时间跨度
    eMtDiffCompr eCompr;  // 对比方法标志
}sMtDiffCompr;

typedef struct
{
    UINT8        ucDiffID;            // 有功总电能量差动组序号：数值范围1～8
    UINT8        ucComprID;           // 对比的总加组序号 数值范围0～8，如这两个中的任一个为"0"，表示本差动组没有要参与对比的总加组，即被"删除"。
    UINT8        ucReferID;           // 参照的总加组序号 数值范围0～8，如这两个中的任一个为"0"，表示本差动组没有要参与对比的总加组，即被"删除"。
    sMtDiffCompr sDiff;               // 参与差动的电能量的时间区间及对比方法标志
    UINT8        ucValueRel;          // 差动越限相对偏差值(％)
    sMtFmt_sX7   sValueAbs;           // 差动越限绝对偏差值(kWh)
}sMtHaveDiffEventOne;

// 用户侧
typedef struct
{
    UINT8                 ucN;          // 本次有功总电能量差动组配置数量n：数值范围1～8。
    sMtHaveDiffEventOne   sOne[1];      // ucN个配置信息

}sMtHaveDiffEvent, sMtAfn04F15;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8        ucDiffID;            // 有功总电能量差动组序号：数值范围1～8
    UINT8        ucComprID;           // 对比的总加组序号 数值范围0～8，如这两个中的任一个为"0"，表示本差动组没有要参与对比的总加组，即被"删除"。
    UINT8        ucReferID;           // 参照的总加组序号 数值范围0～8，如这两个中的任一个为"0"，表示本差动组没有要参与对比的总加组，即被"删除"。
    UINT8        ucDiff;               // 参与差动的电能量的时间区间及对比方法标志
    UINT8        ucValueRel;          // 差动越限相对偏差值(％)
    sMtFmt_sX7_f sValueAbs;           // 差动越限绝对偏差值(kWh)

}sMtHaveDiffEventOne_f;

typedef struct
{
    UINT8                   ucN;       // 本次有功总电能量差动组配置数量n：数值范围1～8。
    sMtHaveDiffEventOne_f   sOne[1];   // ucN个配置信息

}sMtHaveDiffEvent_f, sMtAfn04F15_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f15(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F16：虚拟专网用户名、密码
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F16_VPN_USER_PWD
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F16_VPN_USER_PWD 对应的上行结构相同
{*///

#define MT_VPN_PWD_LEN         (32)
#define MT_VPN_USR_LEN         (32)

// 用户侧
typedef struct
{
    UINT8  ucUser[MT_VPN_USR_LEN]; // 虚拟专网用户名
    UINT8  ucPwd[MT_VPN_PWD_LEN];  // 虚拟专网密码

}sMtVpnUserPwd, sMtVpnUserPwd_f,sMtAfn04F16_f;

// 帧侧
// sMtVpnUserPwd_f,sMtAfn04F16_f


// 转换函数
eMtErr emtTrans_afn04f16(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F17：终端保安定值
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F17_TML_SAFE_VALUE
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F17_TML_SAFE_VALUE 对应的上行结构相同
{*///

// 用户侧
// float fTmlSafeValue;

// 帧侧 
//sMtFmt02_f sXXX;

// 转换函数
eMtErr emtTrans_afn04f17(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F18：终端功控时段
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F18_TML_PCTRL_PERD
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F18_TML_PCTRL_PERD 对应的上行结构相同
{*///
typedef enum
{
    MT_PCTRL_UNKOWN, // 未知控制方式
    MT_PCTRL_NONE,   // 不控制
    MT_PCTRL_1,      // 控制1
    MT_PCTRL_2,      // 控制1

}eMtPCtrl;

typedef struct
{
   eMtPCtrl eFrntHalf;   // 前半小时
   eMtPCtrl eBackHalf;   // 后半小时
  
}sMtPCtrlHour;

// 用户侧
typedef struct
{
   sMtPCtrlHour sPCtrlHour[24];  

}sMtPCtrl;            // 终端功控时段数据单元格式

// 帧侧 
typedef struct
{
    UINT8 uc2Hour[12];
    
}sMtPCtrl_f;

// 转换函数
eMtErr emtTrans_afn04f18(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F19：终端时段功控定值浮动系数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F19_TML_PCTRL_FACTOR
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F19_TML_PCTRL_FACTOR 对应的上行结构相同
{*///

// 用户侧
// sMtFloat sTmlPCtrlFactor;

// 帧侧 
//sMtFmt04_f sXXX;

// 转换函数
eMtErr emtTrans_afn04f19(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F20：终端月电能量控定值浮动系数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F20_TML_MONTH_FACTOR
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F20_TML_MONTH_FACTOR 对应的上行结构相同
{*///

// 用户侧
// sMtFloat sTmlMonthFactor;

// 帧侧 
//sMtFmt04_f sXXX;

// 转换函数
eMtErr emtTrans_afn04f20(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F21：终端电能量费率时段和费率数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F21_TML_POWER_FACTOR
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F21_TML_POWER_FACTOR 对应的上行结构相同
{*///

// #define MT_PAYRATE_NUM_MAX
// #define MT_PAYRATE_NUM_MIN

#define MT_PAYRATE_ID_MAX   MT_PAYRATE_NUM_MAX
#define MT_PAYRATE_ID_MIN   MT_PAYRATE_NUM_MIN

// 用户侧
typedef struct
{
    UINT8 ucPayRateID[48];  // 费率号   依次表示 (0：00～0：30) .... (23：30～0：00) 时段费率号
    UINT8 ucPayRateNum;     // 费率数

}sMtTmlPowerFactor, sMtTmlPowerFactor_f;

// 帧侧 
//sMtTmlPowerFactor_f;

// 转换函数
eMtErr emtTrans_afn04f21(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F22：终端电能量费率
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F22_TML_POWER_RATE
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F22_TML_POWER_RATE 对应的上行结构相同
{*///

// #define MT_PAYRATE_NUM_MAX
// #define MT_PAYRATE_NUM_MIN

// 用户侧
typedef struct
{
    UINT8     ucM;             // 费率数M
    sMtMoney  sPayRate[1];     // 第1 ~ M 费率

}sMtTmlPowerRate, sMtAfn04f22;

// 帧侧 
#pragma pack(1) 
typedef struct
{
    UINT8     ucM;               // 费率数M
    sMtFmt03_f  sPayRate[1];     // 第1 ~ M 费率

}sMtTmlPowerRate_f, sMtAfn04f22_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f22(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F23：终端催费告警参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F23_TML_WARNING_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F23_TML_WARNING_CFG 对应的上行结构相同
{*///

typedef enum
{
   MT_WARN_UNKOWN,   // 未知
   MT_WARN_GO,       // 告警
   MT_WARN_STOP,     // 不告警
   
}eMtWarning;

// 用户侧
typedef struct
{
   eMtWarning eHour[24];         // 表示从 0：00～1：00 每个小时时段是否告警

}sMtTmlWarningCfg, sMtAfn04f23;  // 终端催费告警参数数据单元格式

// 帧侧 
typedef struct
{
    UINT8 ucHour[3];            // D0～D23按顺序对位表示0～23点，每位对应表示1个小时，置"1"：告警，置"0"：不告警，

}sMtTmlWarningCfg_f, sMtAfn04f23_f;

// 转换函数
eMtErr emtTrans_afn04f23(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F25：测量点基本参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F25_MP_BASE_CFG
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F25_MP_BASE_CFG 对应的上行结构相同
{*///
typedef enum
{   
    MT_CONCT_UNKOWN, // 未知
    MT_CONCT_P3_L3,  // 三相三线
    MT_CONCT_P3_L4,  // 三相四线
    MT_CONCT_P1,     // 单相表
    
}eMtConnect;

typedef enum
{
    MT_PHASE_UNKOWN, // 未知
    MT_PHASE_A,      // A 相
    MT_PHASE_B,      // B 相
    MT_PHASE_C,      // C 相
    
}eMtPhase;

typedef struct
{
    eMtConnect eConnect;
    eMtPhase   ePhase;
    
}sMtConnectWay; // 电源接线方式

// 用户侧
typedef struct
{ 
    UINT16         usTimesU;            // 电压互感器倍率
    UINT16         usTimesI;            // 电流互感器倍率
    float          fU;                  // 额定电压   (+)XXX.X  (V)
    float          fI;                  // 额定电流
    float          fP;                  // 额定负荷
    
}sMtMpBaseCfg, sMtAfn04f25;             // 测量点基本参数数据单元格式

// 帧侧 
#pragma pack(1) 
typedef struct
{
    UINT16         usTimesU;            // 电压互感器倍率
    UINT16         usTimesI;            // 电流互感器倍率
    sMtFmt_XXX_X   fU;                  // 额定电压   (+)XXX.X  (V)
    sMtX_X_f       fI;                  // 额定电流
    sMtFmt_XX_XXXX fP;                  // 额定负荷
    
}sMtMpBaseCfg_f, sMtAfn04f25_f;         // 测量点基本参数数据单元格式
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f25(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F26：测量点限值参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F26_MP_LIMIT_CFG
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F26_MP_LIMIT_CFG 对应的上行结构相同
{*///

typedef struct
{
    float    fMax;        // 电压合格上限   XXX.X  (V)
    float    fMin;        // 电压合格下限   XXX.X  (V)
    float    fDrop;       // 电压断相门限   XXX.X  (V)

}sMtStandRateU;           // 电压合格率判别参数

typedef struct
{
    float    fMax;        // 电压上上限（过压门限）XXX.X  (V)
    UINT8    ucMins;      // 越限持续时间  (min)
    float    fRenew;      // 越限恢复系数  XXX.X (%)
}sMtOverU;                // 过压判别参数 

typedef struct
{
    float    fMin;        // 电压下下限（欠压门限）XXX.X  (V)
    UINT8    ucMins;      // 越限持续时间  (min)
    float    fRenew;      // 越限恢复系数  XXX.X (%)

}sMtLossU;                // 欠压判别参数

typedef struct
{
    float    fMax;        // 相电流上上限（过流门限）(+/-)XXX.XXX (A)
    UINT8    ucMins;      // 越限持续时间  (min)
    float    fRenew;      // 越限恢复系数  XXX.X (%)
}sMtOverI;                // 过流判别参数 

typedef struct
{
    float    fMax;        // 相电流上限（额定电流门限）(+/-)XXX.XXX (A)
    UINT8    ucMins;      // 越限持续时间  (min)
    float    fRenew;      // 越限恢复系数  XXX.X (%)
}sMtExcesI;               // 超额定电流判别参数 

typedef struct
{
    float    fMax;        // 相电流上限（额定电流门限）(+/-)XXX.XXX (A)
    UINT8    ucMins;      // 越限持续时间  (min)
    float    fRenew;      // 越限恢复系数  XXX.X (%)
}sMtOverI0;               // 零序电流超限判别参数
 
typedef struct
{
    float    fMax;        // 相电流上限（额定电流门限）(+/-)XX_XXXX (A)
    UINT8    ucMins;      // 越限持续时间  (min)
    float    fRenew;      // 越限恢复系数  XXX.X (%)
}sMtUpLimitS;             // 视在功率超上上限判别参数

typedef struct
{
    float    fMax;        // 相电流上限（额定电流门限）(+/-)XX_XXXX (A)
    UINT8    ucMins;      // 越限持续时间  (min)
    float    fRenew;      // 越限恢复系数  XXX.X (%)
}sMtLimitS;               // 视在功率超上限判别参数

typedef struct
{
    float    fLimit;       // 三相电压不平衡限值 XXX.X (%)
    UINT8    ucMins;       // 越限持续时间  (min)
    float    fRenew;       // 越限恢复系数  XXX.X (%)
}sMtUbU;                   // 三相电压不平衡超限判别参数

typedef struct
{
    float    fLimit;        // 三相电流不平衡限值 XXX.X (%)
    UINT8    ucMins;       // 越限持续时间  (min)
    float    fRenew;       // 越限恢复系数  XXX.X (%)
}sMtUbI;                   // 三相电流不平衡超限判别参数


// 用户侧
typedef struct
{
    sMtStandRateU  sU;       // 电压合格率判别参数   
    sMtOverU       sOverU;   // 过压判别参数 
    sMtLossU       sLossU;   // 欠压判别参数
    sMtOverI       sOverI;   // 过流判别参数
    sMtExcesI      sExcesI;  // 超额定电流判别参数
    sMtOverI0      sOverI0;  // 零序电流超限判别参数
    sMtUpLimitS    sUpS;     // 视在功率超上上限判别参数
    sMtLimitS      sLiS;     // 视在功率超上限判别参数
    sMtUbU         sUbU;     // 三相电压不平衡超限判别参数
    sMtUbI         sUbI;     // 三相电流不平衡超限判别参数
    UINT8          ucMLossU; // 连续失压时间限值
}sMtMpLimitCfg;

// 帧侧 
#pragma pack(1) 
typedef struct
{
    sMtFmt07   fMax;        // 电压合格上限   XXX.X  (V)
    sMtFmt07   fMin;        // 电压合格下限   XXX.X  (V)
    sMtFmt07   fDrop;       // 电压断相门限   XXX.X  (V)

}sMtStandRateU_f;           // 电压合格率判别参数

typedef struct
{
    sMtFmt07   fMax;        // 电压上上限（过压门限）XXX.X  (V)
    UINT8      ucMins;      // 越限持续时间  (min)
    sMtFmt05   fRenew;      // 越限恢复系数  XXX.X (%)
    
}sMtOverU_f;                // 过压判别参数 

typedef struct
{
    sMtFmt07   fMin;        // 电压下下限（欠压门限）XXX.X  (V)
    UINT8      ucMins;      // 越限持续时间  (min)
    sMtFmt05   fRenew;      // 越限恢复系数  XXX.X (%)

}sMtLossU_f;                // 欠压判别参数

typedef struct
{
    sMtFmt25  fMax;        // 相电流上上限（过流门限）(+/-)XXX.XXX (A)
    UINT8     ucMins;      // 越限持续时间  (min)
    sMtFmt05  fRenew;      // 越限恢复系数  XXX.X (%)
    
}sMtOverI_f;               // 过流判别参数 

typedef struct
{
    sMtFmt25  fMax;        // 相电流上限（额定电流门限）(+/-)XXX.XXX (A)
    UINT8     ucMins;      // 越限持续时间  (min)
    sMtFmt05  fRenew;      // 越限恢复系数  XXX.X (%)
    
}sMtExcesI_f;              // 超额定电流判别参数 

typedef struct
{
    sMtFmt25  fMax;        // 相电流上限（额定电流门限）(+/-)XXX.XXX (A)
    UINT8     ucMins;      // 越限持续时间  (min)
    sMtFmt05  fRenew;      // 越限恢复系数  XXX.X (%)
}sMtOverI0_f;              // 零序电流超限判别参数
 
typedef struct
{
    sMtFmt23  fMax;        // 相电流上限（额定电流门限）(+/-)XX_XXXX (A)
    UINT8     ucMins;      // 越限持续时间  (min)
    sMtFmt05  fRenew;      // 越限恢复系数  XXX.X (%)
}sMtUpLimitS_f;            // 视在功率超上上限判别参数

typedef struct
{
    sMtFmt23  fMax;        // 相电流上限（额定电流门限）(+/-)XX_XXXX (A)
    UINT8     ucMins;      // 越限持续时间  (min)
    sMtFmt05  Renew;       // 越限恢复系数  XXX.X (%)
}sMtLimitS_f;              // 视在功率超上限判别参数

typedef struct
{
    sMtFmt05  fLimit;       // 三相电压不平衡限值 XXX.X (%)
    UINT8     ucMins;       // 越限持续时间  (min)
    sMtFmt05  fRenew;       // 越限恢复系数  XXX.X (%)
}sMtUbU_f;                  // 三相电压不平衡超限判别参数

typedef struct
{
    sMtFmt05  fLimit;        // 三相电流不平衡限值 XXX.X (%)
    UINT8     ucMins;        // 越限持续时间  (min)
    sMtFmt05  fRenew;        // 越限恢复系数  XXX.X (%)
}sMtUbI_f;                   // 三相电流不平衡超限判别参数

typedef struct
{
    sMtStandRateU_f  sU;       // 电压合格率判别参数   
    sMtOverU_f       sOverU;   // 过压判别参数 
    sMtLossU_f       sLossU;   // 欠压判别参数
    sMtOverI_f       sOverI;   // 过流判别参数
    sMtExcesI_f      sExcesI;  // 超额定电流判别参数
    sMtOverI0_f      sOverI0;  // 零序电流超限判别参数
    sMtUpLimitS_f    sUpS;     // 视在功率超上上限判别参数
    sMtLimitS_f      sLiS;     // 视在功率超上限判别参数
    sMtUbU_f         sUbU;     // 三相电压不平衡超限判别参数
    sMtUbI_f         sUbI;     // 三相电流不平衡超限判别参数
    UINT8            ucMLossU; // 连续失压时间限值 (分钟)

}sMtMpLimitCfg_f,sMtAfn04f26_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f26(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F27：测量点铜损、铁损参数 
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F27_MP_LOSS_CFG
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F27_MP_LOSS_CFG 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    float    fRa;                // A相电阻RA   (Ω)
    float    fXa;                // A相电抗XA   (Ω)
    float    fGa;                // A相电导GA (S)
    float    fBa;                // A相电纳BA (S)
    
    float    fRb;                // B相电阻RB (Ω)
    float    fXb;                // B相电抗XB (Ω)
    float    fGb;                // B相电导GB (S)
    float    fBb;                // B相电纳BB (S)

    float    fRc;                 // C相电阻RC (Ω)
    float    fXc;                 // C相电抗XC (Ω)
    float    fGc;                 // C相电导GC (S)
    float    fBc;                 // C相电纳BC (S)

}sMtMpLossCfg, sMtAfn04f27;

// 帧侧
#pragma pack(1) 
typedef struct
{
    sMtFmt26    fRa;               // A相电阻RA (Ω)
    sMtFmt26    fXa;               // A相电抗XA (Ω)
    sMtFmt26    fGa;               // A相电导GA (S)
    sMtFmt26    fBa;               // A相电纳BA (S)
    
    sMtFmt26    fRb;               // B相电阻RB (Ω)
    sMtFmt26    fXb;               // B相电抗XB (Ω)
    sMtFmt26    fGb;               // B相电导GB (S)
    sMtFmt26    fBb;               // B相电纳BB (S)

    sMtFmt26    fRc;               // C相电阻RC (Ω)
    sMtFmt26    fXc;               // C相电抗XC (Ω)
    sMtFmt26    fGc;               // C相电导GC (S)
    sMtFmt26    fBc;               // C相电纳BC (S)

}sMtMpLossCfg_f, sMtAfn04f27_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn04f27(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F28：测量点功率因数分段限值
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F28_MP_PERIOD_FACTOR
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F28_MP_PERIOD_FACTOR 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    float  fLimit_1;     // 功率因数分段限值1 (+/-)XXX.X   ％
    float  fLimit_2;     // 功率因数分段限值2 (+/-)XXX.X   ％

}sMtMpPeriodFactor, sMtAfn04f28;

// 帧侧
#pragma pack(1) 
typedef struct
{
    sMtFmt05  fLimit_1;     // 功率因数分段限值1 (+/-)XXX.X   ％
    sMtFmt05  fLimit_2;     // 功率因数分段限值2 (+/-)XXX.X   ％

}sMtMpPeriodFactor_f, sMtAfn04f28_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f28(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F29：终端当地电能表显示号 
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F29_TML_METER_ID
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F29_TML_METER_ID 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
   UINT8  ucMeterID[12];       // 终端当地电能表显示号

}sMtTmlMeterID, sMtAfn04f29,sMtTmlMeterID_f, sMtAfn04f29_f;
// 帧侧


// 转换函数
eMtErr emtTrans_afn04f29(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F30：台区集中抄表停抄/投抄设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F30_TML_AUTO_READ
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F30_TML_AUTO_READ 对应的上行结构相同
{*///

typedef enum
{
    MT_AUTO_READ_UNKOWN,       // 未知
    MT_AUTO_READ_GO,           // 投抄
    MT_AUTO_READ_STOP,         // 停抄 
}eMtAutoRead;

// 用户侧
typedef struct
{
   eMtAutoRead  bRead;       // 台区集中抄表停抄/投抄设置

}sMtTmlAutoRead, sMtAfn04f30;

// 帧侧
#pragma pack(1) 
typedef struct
{
   UINT8 bRead;             // 台区集中抄表停抄/投抄设置

}sMtTmlAutoRead_f, sMtAfn04f30_f;
#pragma pack()



// 转换函数
eMtErr emtTrans_afn04f30(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F31：载波从节点附属节点地址
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F31_SLAVE_ADDR
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F31_SLAVE_ADDR 对应的上行结构相同
{*///

typedef struct
{
    UINT8 ucAddr[12];
    
}sMtSlaveAddr;

// 用户侧
typedef struct
{
    UINT8         ucN;               // 本次设置的载波从节点附属节点地址个数 n（n≤20）
    sMtSlaveAddr  sAddr[1];          // 0 ~ N个地址信息
    
}sMtSlaveAddrCfg, sMtAfn04f31;
// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8        ucN;                // 本次设置的载波从节点附属节点地址个数 n（n≤20）
    sMtFmt12     sAddr[1];           // 0 ~ N个地址信息

}sMtSlaveAddrCfg_f, sMtAfn04f31_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn04f31(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F33：终端抄表运行参数设置 
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F33_TML_READ_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F33_TML_READ_CFG 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    sMtHHmm sBegin;        // 允许抄表时段开始时间
    sMtHHmm sEnd;          // 允许抄表时段开始时间

}sMtBeginEnd;

typedef struct
{
    UINT8          ucPort;                    // 终端通信端口号：数据范围1～31
    UINT16         usWord;                    // 台区集中抄表运行控制字
    UINT32         ulReadDate;                // 抄表日包括日期
    sMtHHmm        sHHmm;                     // 抄表日-时间
    UINT8          ucPeriod;                  // 终端抄表间隔为终端实时抄表的时间间隔，取值范围为1～60分钟。
    sMtDDHHmm      sDDHHmm;                   // 对电表广播校时定时时间
    UINT8          ucM;                       // 允许抄表时段数m（0≤m≤24）
    sMtBeginEnd    sTime[1];                  // 0 ~ m个
}sMtTmlReadCfgOne;

typedef struct
{
    UINT8              ucN;                // 本次设置的参数块个数n：数值范围1～31
    sMtTmlReadCfgOne   sOne[1];            // 0 ~ N

}sMtTmlReadCfg, sMtAfn04f33;

// 帧侧
#pragma pack(1) 
typedef struct
{
    sMtFmt19_f sBegin;        // 允许抄表时段开始时间
    sMtFmt19_f sEnd;          // 允许抄表时段开始时间

}sMtBeginEnd_f;

typedef struct
{
    UINT8          ucPort;                    // 终端通信端口号：数据范围1～31
    UINT16         usWord;                    // 台区集中抄表运行控制字
    UINT32         ulReadDate;                // 抄表日包括日期
    sMtFmt19_f     sHHmm;                     // 抄表日-时间
    UINT8          ucPeriod;                  // 终端抄表间隔为终端实时抄表的时间间隔，取值范围为1～60分钟。
    sMtFmt18_f     sDDHHmm;                   // 对电表广播校时定时时间
    UINT8          ucM;                       // 允许抄表时段数m（0≤m≤24）
    sMtBeginEnd_f  sTime[1];                  // 0 ~ m个
}sMtTmlReadCfgOne_f;

typedef struct
{
    UINT8                ucN;                // 本次设置的参数块个数n：数值范围1～31
    sMtTmlReadCfgOne_f   sOne[1];            // 0 ~ N

}sMtTmlReadCfg_f, sMtAfn04f33_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f33(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F34：集中器下行通信模块的参数设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F34_CON_DOWN_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F34_CON_DOWN_CFG 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT8  ucPort;       // 终端通信端口号
    UINT8  ucWord;       // 与终端接口端的通信控制字
    UINT8  ucBaud;       // 与终端接口端的通信控制字
}sMtConDownCfgOne;

typedef struct
{
    UINT8                ucN;           // 本次设置的参数块个数n
    sMtConDownCfgOne     sOne[1];       // 1~N个信息

}sMtConDownCfg, sMtAfn04f34;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucPort;       // 终端通信端口号
    UINT8  ucWord;       // 与终端接口端的通信控制字
    UINT8  ucBaud;       // 与终端接口端的通信控制字
}sMtConDownCfgOne_f;

typedef struct
{
    UINT8                ucN;           // 本次设置的参数块个数n
    sMtConDownCfgOne_f   sOne[1];       // 1~N个信息

}sMtConDownCfg_f, sMtAfn04f34_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn04f34(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F35：台区集中抄表重点户设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F35_TML_READ_VIP
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F35_TML_READ_VIP 对应的上行结构相同
{*///

// 用户侧
typedef struct
{
    UINT16 usMeterID;
   
}sMtTmlReadVipOne;

typedef struct
{
    UINT8                ucN;           // 本次设置的参数块个数n
    sMtTmlReadVipOne     sOne[1];       // 1~N个信息

}sMtTmlReadVip, sMtAfn04f35;

// 帧侧
#pragma pack(1) 
typedef struct
{
   UINT16 usMeterID;
   
}sMtTmlReadVipOne_f;

typedef struct
{
    UINT8                  ucN;           // 本次设置的参数块个数n
    sMtTmlReadVipOne_f     sOne[1];       // 1~N个信息

}sMtTmlReadVip_f, sMtAfn04f35_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f35(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F36：终端上行通信流量门限设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F36_TML_UP_LIMIT
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F36_TML_UP_LIMIT 对应的上行结构相同
{*///

// 用户侧
typedef struct
{    
    UINT32 ulLimit;         // 月通信流量门限为0，表示系统不需要终端进行流量控制。

}sMtTmlUpLimit, sMtAfn04f36;

// 帧侧
typedef struct
{
     UINT32 ulLimit;       // 月通信流量门限为0，表示系统不需要终端进行流量控制。
}sMtTmlUpLimit_f, sMtAfn04f36_f;

// 转换函数
eMtErr emtTrans_afn04f36(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F37：终端级联通信参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F37_TML_CASC_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F37_TML_CASC_CFG 对应的上行结构相同
{*///

// 用户侧
typedef struct
{    
    UINT8 ucN;               // 级联/被级联标志、及其对应的被级联/级联的终端个数n

}sMtTmlCascCfg, sMtAfn04f37;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucBCD[2];
    UINT16 usAddr;

}sMtCascOne_f;

typedef struct
{
    UINT8 ucPort;            // 终端级联通信端口号
    UINT8 ucWord;            // 终端级联通信控制字
    UINT8 ucGramTimeOut;     // 接收等待报文超时时间
    UINT8 ucByteTimeOut;     // 接收等待字节超时时间
    UINT8 ucReTryTimes;      // 级联方（主动站）接收失败重发次数
    UINT8 ucTime;            // 级联巡测周期
    UINT8 ucN;               // 级联/被级联标志、及其对应的被级联/级联的终端个数n
    sMtCascOne_f sOne[1];    // 

}sMtTmlCascCfg_f, sMtAfn04f37_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f37(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F38：1类数据配置设置 （在终端支持的1类数据配置内）
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F38_CFG_ASK_1
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F38_CFG_ASK_1 对应的上行结构相同
{*///

#define MT_ASK_M_MIN   (1)   // 本次设置的组数m 最小值
#define MT_ASK_M_MAX   (16)  // 本次设置的组数m 最大值

// 用户侧
typedef struct
{    
    UINT8   ucSmall;                    //  用户小类号 (0 ~ 15)           
    UINT8   ucFN;                       //  支持的命令个数  0 ~ 248
    eMtCmd  eCmd[MT_FN_MAX];            //  支持的一类数据命令码 CMD_AFN_C_F2_TML_CLOCK 到 CMD_AFN_C_F170_READ_METER
}sMtAsk1CfgOne;

typedef struct
{    
    UINT8          ucBig;                //  用户大类号  (0 ~ 15)          
    UINT8          ucM;                  // 本次设置的组数m（m≤该用户大类号下支持的用户小类号个数）
    sMtAsk1CfgOne  sOne[1];              // M个信息
    
}sMtAsk1Cfg, sMtAfn04f38;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8 ucSmall;            // 用户小类号            
    UINT8 ucN;                // 信息类组数n：数值范围0～31，为0表示都不支持。        
    UINT8 ucF8n[1];           // 第1~n组信息类组所对应的信息类元标志位
    
}sMtAsk1CfgOne_f;

typedef struct
{ 
    UINT8            ucBig;    // 本次设置所对应的用户大类号
    UINT8            ucM;      // 本次设置的组数m（m≤该用户大类号下支持的用户小类号个数）
    //sMtAsk1CfgOne_f  sOne[1];  // 
    UINT8            sOne[1];  // 上面结构的  sMtAsk1CfgOne_f

}sMtAsk1Cfg_f, sMtAfn04f38_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn04f38(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F39：2类数据配置设置（在终端支持的2类数据配置内）
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F39_CFG_ASK_2
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F39_CFG_ASK_2 对应的上行结构相同
{*///

// 用户侧
typedef struct
{    
    UINT8 ucSmall;              //  用户小类号            
    UINT8   ucFN;               //  支持的命令个数  0 ~ 248
    eMtCmd  eCmd[MT_FN_MAX];    //  支持的一类数据命令码 CMD_AFN_D_F1_FRTH_POWR_P1P4_D 到 CMD_AFN_D_F218_COLOR_YAWP_CURVE

}sMtAsk2CfgOne;

typedef struct
{    
    UINT8          ucBig;           //  用户小类号            
    UINT8          ucM;             // 本次设置的组数m（m≤该用户大类号下支持的用户小类号个数）
    sMtAsk2CfgOne  sOne[1];         // M个信息
    
}sMtAsk2Cfg, sMtAfn04f39;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8 ucSmall;            // 用户小类号            
    UINT8 ucN;                // 信息类组数n：数值范围0～31，为0表示都不支持。        
    UINT8 ucF8n[1];           // 第1~n组信息类组所对应的信息类元标志位
}sMtAsk2CfgOne_f;

typedef struct
{ 
    UINT8            ucBig;    // 本次设置所对应的用户大类号
    UINT8            ucM;      // 本次设置的组数m（m≤该用户大类号下支持的用户小类号个数）
    //sMtAsk1CfgOne_f  sOne[1];  // 
    UINT8            sOne[1];  // 上面结构的  sMtAsk1CfgOne_f

}sMtAsk2Cfg_f, sMtAfn04f39_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f39(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F41：时段功控定值
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F41_PERIOD_VALUE
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F41_PERIOD_VALUE 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f41(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F42：厂休功控参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F42_FACTORY_HOLIDAY
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F42_FACTORY_HOLIDAY 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f42(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F43：功率控制的功率计算滑差时间
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F43_SLIDE_TIME
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F43_SLIDE_TIME 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f43(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F44：营业报停控参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F44_SHUTOUT_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F44_SHUTOUT_CFG 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f44(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F45：功控轮次设定
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F45_CTRL_TURN_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F45_CTRL_TURN_CFG 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f45(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F46：月电量控定值
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F46_MONTH_FIX_VALUE
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F46_MONTH_FIX_VALUE 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f46(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F47：购电量（费）控参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F47_BUY_COUNT
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F47_BUY_COUNT 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f47(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F48：电控轮次设定
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F48_ELEC_TURN_CFG
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F48_ELEC_TURN_CFG 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f48(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F49：功控告警时间
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F49_WARNING_TIME
 * PN 类型 : P0
 * 结构说明: 与 CMD_AFN_A_F49_WARNING_TIME 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f49(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F57：终端声音告警允许∕禁止设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F57_TML_WARN_SOUND
 * PN 类型 : 总加组号
 * 结构说明: 与 CMD_AFN_A_F57_TML_WARN_SOUND 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f57(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F58：终端自动保电参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F58_TML_ATUO_PROTECT
 * PN 类型 : 总加组号
 * 结构说明: 与 CMD_AFN_A_F58_TML_ATUO_PROTECT 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f58(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F59：电能表异常判别阈值设定
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F59_METER_LIMIT
 * PN 类型 : 总加组号
 * 结构说明: 与 CMD_AFN_A_F59_METER_LIMIT 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f59(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F60：谐波限值
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F60_HUMOR_LIMIT
 * PN 类型 : 总加组号
 * 结构说明: 与 CMD_AFN_A_F60_HUMOR_LIMIT 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f60(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F61：直流模拟量接入参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F61_DC_SIMULA
 * PN 类型 : 总加组号
 * 结构说明: 与 CMD_AFN_A_F61_DC_SIMULA 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f61(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F65：定时上报1类数据任务设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F65_CFG_AUTO_1
 * PN 类型 : 任务号(1 ~ 61)
 * 结构说明: 与 CMD_AFN_A_F65_CFG_AUTO_1 对应的上行结构相同
{*///

#define MT_R_MIN     (1)     // 曲线数据抽取倍率R 最小值
#define MT_R_MAX     (96)    // 曲线数据抽取倍率R 最大值

#define MT_PRD_MIN   (1)     // 定时上报周期最小值
#define MT_PRD_MAX   (63)    // 定时上报周期最大值

// 用户侧
typedef enum
{
    MT_RPT_UNKOWN,         // 未知
    MT_RPT_mm,             // 分
    MT_RPT_HH,             // 时
    MT_RPT_DD,             // 日
    MT_RPT_MM              // 月
    
}eMtRptUnit;               // 定时上报周期单位

typedef struct
{
    eMtRptUnit    eUnit;       // 定时上报周期单位(分、时、日、月)
    UINT8         ucPeriod;    // 定时上报周期   范围(MT_PRD_MIN ~ MT_PRD_MAX)
    sMtYWMDhms    sTime;       // 上报基准时间：秒分时日月年
    UINT8         ucR;         // 曲线数据抽取倍率R (1～96)
    UINT8         ucN;         // 数据单元标识个数n
    sMtCmdPn      sCmdPn[1];   // N个数据单元标识  

}sMtCfgAuto_1, sMtAfn04f65;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8         ucPeriod:6;  // 定时上报周期单位(分、时、日、月)
    UINT8         ucUnit:2;    // 定时上报周期
    sMtFmt01_f    sTime;       // 上报基准时间：秒分时日月年
    UINT8         ucR;         // 曲线数据抽取倍率R (1～96)
    UINT8         ucN;         // 数据单元标识个数n
    sMtDaDt       sDaDt[1];    // N个数据单元标识  
    
}sMtCfgAuto_1_f, sMtAfn04f65_f;
#pragma pack() 

// 转换函数

eMtErr emtTrans_afn04f65(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F66：定时上报2类数据任务设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F66_CFG_AUTO_2
 * PN 类型 : 任务号
 * 结构说明: 与 CMD_AFN_A_F66_CFG_AUTO_2 对应的上行结构相同
{*///
// 用户侧
typedef struct
{
    eMtRptUnit    eUnit;    // 定时上报周期单位(分、时、日、月)
    UINT8         ucPeriod; // 定时上报周期
    sMtYWMDhms    sTime;    // 上报基准时间：秒分时日月年
    UINT8         ucR;      // 曲线数据抽取倍率R (1～96)
    UINT8         ucN;      // 数据单元标识个数n
    sMtCmdPn      sCmdPn[1];   // N个数据单元标识  

}sMtCfgAuto_2, sMtAfn04f66;

// 帧侧
#pragma pack(1) 

typedef struct
{
    UINT8        ucPeriod:6;  // 定时上报周期单位(分、时、日、月)
    UINT8        ucUnit:2;    // 定时上报周期
    sMtFmt01_f   sTime;       // 上报基准时间：秒分时日月年
    UINT8        ucR;         // 曲线数据抽取倍率R (1～96)
    UINT8        ucN;         // 数据单元标识个数n
    sMtDaDt      sDaDt[1];    // N个数据单元标识  
}sMtCfgAuto_2_f, sMtAfn04f66_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f66(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F67：定时上报1类数据任务启动/停止设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F67_GOP_AUTO_1
 * PN 类型 : 任务号
 * 结构说明: 与 CMD_AFN_A_F67_GOP_AUTO_1 对应的上行结构相同
{*///
// 用户侧

typedef enum
{
    MT_AUTO_UNKOWN,  // 无效值
    MT_AUTO_GO,      // 继续
    MT_AUTO_STOP,    // 停止
}eMtAutoGo;

typedef struct
{
    eMtAutoGo eGo;       
    
}sMtGopAuto_1, sMtAfn04f67;


// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8 ucGo;
    
}sMtGopAuto_1_f, sMtAfn04f67_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn04f67(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F68：定时上报2类数据任务启动/停止设置
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_4_F68_GOP_AUTO_2
 * PN 类型 : 任务号
 * 结构说明: 与 CMD_AFN_A_F68_GOP_AUTO_2 对应的上行结构相同
{*///
// 用户侧
typedef struct
{
    eMtAutoGo eGo;       
    
}sMtGopAuto_2, sMtAfn04f68;


// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8 ucGo;
    
}sMtGopAuto_2_f, sMtAfn04f68_f;
#pragma pack() 
// 转换函数
eMtErr emtTrans_afn04f68(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F73：电容器参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_A_F73_CAPA_CFG
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F73_CAPA_CFG 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f73(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F74：电容器投切运行参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_A_F74_CAPA_RUN_CFG
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F74_CAPA_RUN_CFG 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f74(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F75：电容器保护参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_A_F75_CAPA_PROT_PARA
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F75_CAPA_PROT_PARA 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f75(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F76：电容器投切控制方式
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_A_F76_CAPA_CTRL_WAY
 * PN 类型 : 测量点号
 * 结构说明: 与 CMD_AFN_A_F76_CAPA_CTRL_WAY 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f76(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F81：直流模拟量输入变比
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_A_F81_CD_CHANGE_RATE
 * PN 类型 : 直流模拟量端口号
 * 结构说明: 与 CMD_AFN_A_F81_CD_CHANGE_RATE 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f81(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F82：直流模拟量限值
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_A_F82_CD_LIMIT
 * PN 类型 : 直流模拟量端口号
 * 结构说明: 与 CMD_AFN_A_F82_CD_LIMIT 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f82(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F83：直流模拟量冻结参数
 * 对应AFN : AFN_04_SETP
 * 对应CMD : CMD_AFN_A_F83_CD_FREEZE_PARA
 * PN 类型 : 直流模拟量端口号
 * 结构说明: 与 CMD_AFN_A_F83_CD_FREEZE_PARA 对应的上行结构相同
{*///

// 转换函数
eMtErr emtTrans_afn04f83(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F1：遥控跳闸
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F1_REMOTE_TURN_OFF
 * PN 类型 : 控制轮次
{*///

// 用户侧
typedef struct
{
    UINT8 ucResTime;   // 限电时间      0～15 (单位: 半小时)  为0时，表示紧急限电，即长时间限电，不自动解除限电状态；不为0时，按设置的限电时间进行限电。
    UINT8 ucWarnTime;  // 告警延时时间  0～15，单位：min      为0时，表示立即跳闸；不为0时，按设置的告警时间进行告警。

}sMtRemoteTurnOff,sMtRemoteTurnOff_f;

// 帧侧

// 转换函数
eMtErr emtTrans_afn05f1(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F9：时段功控投入
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F9_PERIOD_GO
 * PN 类型 : 控制轮次
{*///

// 用户侧
typedef struct
{
    BOOL  bPeriod[8];  // 表示第1～第8时段，置"1"：有效，置"0"：无效
    UINT8 ucWayID;     // 数值范围：0～2依次表示第1～第3套方案，其他值无效。
}sMtPeriodGo, sMtAfn05F9; 

// 帧侧
typedef struct
{
    UINT8 ucPeriod;    // 表示第1～第8时段，置"1"：有效，置"0"：无效
    UINT8 ucWayID;     // 数值范围：0～2依次表示第1～第3套方案，其他值无效。
}sMtPeriodGo_f, sMtAfn05F9_f; 

// 转换函数
eMtErr emtTrans_afn05f9(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F12：当前功率下浮控投入
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F12_DOWN_GO
 * PN 类型 : 控制轮次
{*///

// 用户侧
typedef struct
{
    
}sMtDownGo, sMtAfn05F12; 

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8      ucSlideTime;       // 当前功率下浮控定值滑差时间
    sMtFmt04_f sFactor;           // 当前功率下浮控定值浮动系数
    UINT8      ucDelayTime;       // 控后总加有功功率冻结延时时间
    UINT8      ucCtrlTime;        // 当前功率下浮控的控制时间
    UINT8      ucWarnTime1;       // 当前功率下浮控第1轮告警时间
    UINT8      ucWarnTime2;       // 当前功率下浮控第2轮告警时间
    UINT8      ucWarnTime3;       // 当前功率下浮控第3轮告警时间
    UINT8      ucWarnTime4;       // 当前功率下浮控第4轮告警时间

}sMtDownGo_f, sMtAfn05F12_f; 
#pragma pack()

// 转换函数
eMtErr emtTrans_afn05f12(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F25：终端保电投入
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F25_TML_PROTECT_GO
 * PN 类型 : 控制轮次
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f25(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F31：对时命令
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F31_CHECK_TIME
 * PN 类型 : p0 
{*///

// 用户侧
//sMtYYWWMMDDhhmmss  sTime; 

// 帧侧
//sMtYYWWMMDDhhmmss_f sTime;

// 转换函数
eMtErr emtTrans_afn05f31(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F32：中文信息
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F32_CHINESE_INFO
 * PN 类型 : p0 
{*///

#define  MT_CHINESE_INFO_MAX   (200) // 中文信息最大长度

// 用户侧
typedef enum
{
    MT_CHIN_INFO_NORMAL, // 普通信息
    MT_CHIN_INFO_IMPORT, // 重要信息
}eMtChInInfo;

typedef struct
{
    eMtChInInfo eType;     // 表示信息内容的分类，为"0"表示普通信息，为"1"表示重要信息，其他数值备用。
    UINT8       ucSerID;   // 某类中文信息的本条信息的编号，其值取决于终端所能存储的信息总容量。
    UINT8       ucLen;     // 长度Lcn（≤200）
    UINT8       ucInfo[MT_CHINESE_INFO_MAX]; // Lcn长的汉字信息
}sMtChineseInfo, sMtAfn05F32;


// 帧侧
typedef struct
{
    UINT8  ucTypeID;       // 表示信息内容的分类，为"0"表示普通信息，为"1"表示重要信息，其他数值备用。
    UINT8  ucLen;          // 某类中文信息的本条信息的编号，其值取决于终端所能存储的信息总容量。
    UINT8  ucInfo[1];      // Lcn长的汉字信息
    
}sMtChineseInfo_f, sMtAfn05F32_f;

// 转换函数
eMtErr emtTrans_afn05f32(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F41：电容器控制投入
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F41_CAPA_CTRL_GO
 * PN 类型 : 测量点号
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f41(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F42：电容器控制切除
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F42_CAPA_CTRL_STOP
 * PN 类型 : 测量点号
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f42(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F49：命令指定通信端口暂停抄表
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F49_READ_METER_STOP
 * PN 类型 : P0
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f49(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F50：命令指定通信端口恢复抄表
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F50_READ_METER_GO
 * PN 类型 : P0
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f50(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F51：命令指定通信端口重新抄表
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F51_READ_METER_REDO
 * PN 类型 : P0
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f51(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F52：初始化指定通信端口下的全部中继路由信息
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F52_INIT_ROUTE
 * PN 类型 : P0
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f52(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F53：删除指定通信端口下的全部电表
 * 对应AFN : AFN_05_CTRL
 * 对应CMD : CMD_AFN_5_F53_DELET_ALL_METER
 * PN 类型 : 测量点号
{*///

// 用户侧

// 帧侧


// 转换函数
eMtErr emtTrans_afn05f53(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F1：身份认证请求
 * 对应AFN : AFN_06_AUTH
 * 对应CMD : CMD_AFN_6_F1_ID_AUTH_Q
 * PN 类型 : p0
{*///

// 用户侧
typedef struct
{
    UINT8 ucQuerry[16];
}sMtIdAuthQ,sMtIdAuthQ_f;

// 帧侧

// 转换函数
eMtErr emtTrans_afn06f1(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F2：身份认证响应
 * 对应AFN : AFN_06_AUTH
 * 对应CMD : CMD_AFN_6_F2_ID_AUTH_A
 * PN 类型 : p0
{*///

// 用户侧
typedef struct
{
    UINT8 ucAnswer[16];
}sMtIdAuthA,sMtIdAuthA_f;

// 帧侧

// 转换函数
eMtErr emtTrans_afn06f2(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F3：取随机数
 * 对应AFN : AFN_06_AUTH
 * 对应CMD : CMD_AFN_6_F3_RAND_Q
 * PN 类型 : p0
{*///

// 用户侧
typedef struct
{
    UINT8 ucQuerry[16];
}sMtRandQ,sMtRandQ_f;

// 帧侧

// 转换函数
eMtErr emtTrans_afn06f3(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F4：取随机数响应
 * 对应AFN : AFN_06_AUTH
 * 对应CMD : CMD_AFN_6_F4_RAND_A
 * PN 类型 : p0
{*///

// 用户侧
typedef struct
{
    UINT8 ucAnswer[16];
}sMtRandA,sMtRandA_f;

// 帧侧

// 转换函数
eMtErr emtTrans_afn06f4(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F1：终端版本信息
 * 对应AFN : AFN_09_CFIG
 * 对应CMD : CMD_AFN_9_F1_TML_VERSION
 * PN 类型 : p0
{*///

// 用户侧
typedef struct
{
    UINT8      ucFactoryID[4];           // 厂商代号 (ASCII)
    UINT8      ucDeviceID[8];            // 设备编号 (ASCII)
    UINT8      ucSoftwareID[4];          // 终端软件版本号
    sMtYYMMDD  sDateSoftware;            // 终端软件发布日期：日月年
    UINT8      ucInfoCodeCap[11];        // 终端配置容量信息码  (ASCII)
    UINT8      ucProtolID[4];            // 终端通信协议.版本号 (ASCII)
    UINT8      ucHardWareID[4];          // 终端通信协议.版本号 (ASCII)
    sMtYYMMDD  sDateHardware;            // 终端硬件发布日期：日月年
    
}sMtTmlVersion, sMtAfn09F1;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8       ucFactoryID[4];           // 厂商代号 (ASCII)
    UINT8       ucDeviceID[8];            // 设备编号 (ASCII)
    UINT8       ucSoftwareID[4];          // 终端软件版本号
    sMtFmt20_f  sDateSoftware;            // 终端软件发布日期：日月年
    UINT8       ucInfoCodeCap[11];        // 终端配置容量信息码  (ASCII)
    UINT8       ucProtolID[4];            // 终端通信协议.版本号 (ASCII)
    UINT8       ucHardWareID[4];          // 终端通信协议.版本号 (ASCII)
    sMtFmt20_f  sDateHardware;            // 终端硬件发布日期：日月年
  
}sMtTmlVersion_f, sMtAfn09F1_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn09f1(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F2：终端支持的输入、输出及通信端口配置
 * 对应AFN : AFN_09_CFIG
 * 对应CMD : CMD_AFN_9_F2_TML_INPUT_OUT
 * PN 类型 : p0
{*///

typedef struct
{
    UINT8 ucMac[6];

}sMtMac;

#define MT_CIRCLE_MAX              (64)  // 路数最大值
#define MT_CIRCLE_MIN              (0)   // 路数最小值

typedef enum
{
    MT_INTER_RS485,                      // 直接RS485接口
    MT_INTER_RS232,                      // 直接RS232接口
    MT_INTER_SERIW,                      // 串行接口连接窄带低压载波通信模块
    
}eMtInterface;                           // 通道类型
 
typedef enum
{
    MT_PORR_USE_EXP,                     // 专变、公变抄表
    MT_PORR_USE_CNG,                     // 变电站抄表
    MT_PORR_USE_GTH,                     // 台区低压集抄
    MT_PORR_USE_SHR,                     // 当地用户侧数据共享

}eMtPortUse;                             // 用途，数值范围：0～3依次表示专变、公变抄表，变电站抄表，台区低压集抄，当地用户侧数据共享


#define MT_PORT_ID_MIN           (1)     //  端口最小值
#define MT_PORT_ID_MAX           (31)    //  端口最大值

typedef struct
{
    UINT8        ucPort;                 // 通信端口的端口号 (数值范围1～31)
    eMtInterface eInter;                 // 通道类型
    BOOL         bStandard;              // 是否为 标准异步串行口
    eMtPortUse   eUse;                   // 用途
    UINT32       ulBaud;                 // 通信端口支持的最高波特率（bps）
    UINT16       usDevNum;               // 端口上支持的设备个数 (0～2040)
    UINT16       usRecvBuf;              // 端口支持的最大接收缓存区字节数 (256~16383)
    UINT16       usSendBuf;              // 端口支持的最大发送缓存区字节数 (256~16383)
    
}sMtPortOne;

#define MT_READ_METER_MAX           (2040)
#define MT_READ_METER_MIN           (0)

#define MT_SEND_REC_MAX             (16383)
#define MT_SEND_REC_MIN             (256)

// 用户侧
typedef struct
{
    UINT8  ucPulseInputNum;               // 脉冲量输入路数 (0~64)
    UINT8  ucSwitchInputNum;              // 开关量输入路数 (0~64)
    UINT8  ucDcNum;                       // 直流模拟量输入路数 (0~64)
    UINT8  ucSwitchOutputNum;             // 脉冲量输入路数 (0~64)
    UINT16 usReadMeterNum;                // 支持的抄电能表/交流采样装置最多个数 (0~2040)  MT_READ_METER_MAX
    UINT16 usTmlRecvBuf;                  // 支持的终端上行通信最大接收缓存区字节数 (256~16383) MT_SEND_REC_MAX
    UINT16 usTmlSendBuf;                  // 支持的终端上行通信最大发送缓存区字节数 (256~16383) MT_SEND_REC_MAX
    sMtMac sMacAddr;                      // MAC地址
    UINT8  ucPortNum;                     // 通信端口数量n  0～31
    sMtPortOne sPort[1];                  // ucPortNum端口配置

}sMtTmlInputOut, sMtAfn09F2;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8   ucPort     :5;               // 编码表示本通信口的端口号，数值范围1～31。
    UINT8   eInter     :2;               // 编码表示本通信端口的接口及通道类型，数值范围：0～3
    UINT8   bStandard  :1;               // D7=0表示本通信口为标准异步串行口，D7=1表示为非标准异步串行口。
    UINT8   ucRes      :5;               // 备用
    UINT8   eUse       :3;               // 0～3依次表示专变、公变抄表，变电站抄表，台区低压集抄，当地用户侧数据共享

}sMtPortWord_f;

typedef struct
{
    sMtPortWord_f sWord;                  // 通信端口的端口号及信息字
    UINT32        ulBaud;                 // 通信端口支持的最高波特率（bps）
    UINT16        usDevNum;               // 端口上支持的设备个数 (0～2040)
    UINT16        usRecvBuf;              // 端口支持的最大接收缓存区字节数 (256~16383) MT_SEND_REC_MAX
    UINT16        usSendBuf;              // 端口支持的最大发送缓存区字节数 (256~16383) MT_SEND_REC_MAX
    
}sMtPortOne_f;

typedef struct
{
    UINT8        ucPulseInputNum;               // 脉冲量输入路数 (0~64)
    UINT8        ucSwitchInputNum;              // 开关量输入路数 (0~64)
    UINT8        ucDcNum;                       // 直流模拟量输入路数 (0~64)
    UINT8        ucSwitchOutputNum;             // 脉冲量输入路数 (0~64)
    UINT16       usReadMeterNum;                // 支持的抄电能表/交流采样装置最多个数 (0~2040)
    UINT16       usTmlRecvBuf;                  // 支持的抄电能表/交流采样装置最多个数 (256~16383)
    UINT16       usTmlSendBuf;                  // 支持的抄电能表/交流采样装置最多个数 (256~16383)
    sMtMac       sMacAddr;                      // MAC地址
    UINT8        ucPortNum;                     // 通信端口数量n  0～31
    sMtPortOne_f sPort[1];                      // ucPortNum端口配置

}sMtTmlInputOut_f, sMtAfn09F2_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn09f2(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F3：终端支持的其他配置
 * 对应AFN : AFN_09_CFIG
 * 对应CMD : CMD_AFN_9_F3_TML_OTHER_CFG
 * PN 类型 : p0
{*///

#define MT_TASK_MIN     (0)     // 支持的任务最多个数 最小值
#define MT_TASK_MAX     (64)    // 支持的任务最多个数 最大值

#define MT_GROUP_MIN    (0)     // 支持的总加组最多组数最小值
#define MT_GROUP_MAX    (8)     // 支持的总加组最多组数最大值

#define MT_DAY_MIN      (0)     // 日期最小值
#define MT_DAY_MAX      (31)    // 日期最大值

#define MT_MONTH_MIN    (0)     // 月份最小值
#define MT_MONTH_MAX    (12)    // 月份最大值

#define MT_PCTRL_MIN    (0)
#define MT_PCTRL_MAX    (3)

#define MT_HARM_MIN     (0)     // 谐波次数最小值
#define MT_HARM_MAX     (19)    // 谐波次数最大值

#define MT_VIP_MIN      (0)     // 支持的台区集中抄表重点户最多户数 最小值
#define MT_VIP_MAX      (20)    // 支持的台区集中抄表重点户最多户数 最大值

#define MT_CAPA_MIN     (0)     // 支持的无功补偿电容器组最多组数 最小值
#define MT_CAPA_MAX     (16)    // 支持的无功补偿电容器组最多组数 最大值

// 用户侧
typedef struct
{
    UINT16         usMp;        // 支持的测量点最多点数 (0 ~ 2040)
    UINT8          ucGroup;     // 支持的总加组最多组数 (0 ~ 8)
    UINT8          ucTask;      // 支持的任务最多个数   (0 ~ 64)
    UINT8          ucTeam;      // 支持的有功总电能量差动组最多组数 (0～8)
    eMtFmtFrez     eFrezMp;     // 支持的测量点数据最大冻结密度
    eMtFmtFrez     eFrezP;      // 支持的总加组有功功率数据最大冻结密度
    eMtFmtFrez     eFrezQ;      // 支持的总加组无功功率数据最大冻结密度
    eMtFmtFrez     eFrezPt;     // 支持的总加组有功电能量数据最大冻结密度
    eMtFmtFrez     eFrezQt;     // 支持的总加组无功电能量数据最大冻结密度
    UINT8          ucDay;       // 支持的日数据最多存放天数 (0 ~ 31)
    UINT8          ucMonth;     // 支持的月数据最多存放月数 (0 ~ 12)
    UINT8          ucPCtrl;     // 支持的时段功控定值方案最多个数 (0 ~ 3)
    UINT8          ucHarm;      // 支持的谐波检测最高谐波次数 (0 ~ 19)
    UINT8          ucCapa;      // 支持的无功补偿电容器组最多组数 (0 ~ 16)
    UINT8          ucVip;       // 支持的台区集中抄表重点户最多户数 (0 ~ 20)
    BOOL           bBig[MT_USER_CLASS_NUM];    // 支持的用户大类号标志
                                               // 依次分别对应第0～15号用户大类，置"1"表示支持，置"0"表示不支持，
                                               // 且对应的"支持用户小类号个数"应为0。
    UINT8          ucSmall[MT_USER_CLASS_NUM]; // 支持0 ~ 15号用户大类下的用户小类号个数

}sMtTmlOtherCfg, sMtAfn09F3;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT16         usMp;        // 支持的测量点最多点数 (0 ~ 2040)
    UINT8          ucGroup;     // 支持的总加组最多组数 (0 ~ 8)
    UINT8          ucTask;      // 支持的任务最多个数   (0 ~ 64)
    UINT8          ucTeam;      // 支持的有功总电能量差动组最多组数 (0～8)
    UINT8          eFrezMp;     // 支持的测量点数据最大冻结密度
    UINT8          eFrezP;      // 支持的总加组有功功率数据最大冻结密度
    UINT8          eFrezQ;      // 支持的总加组无功功率数据最大冻结密度
    UINT8          eFrezPt;     // 支持的总加组有功电能量数据最大冻结密度
    UINT8          eFrezQt;     // 支持的总加组无功电能量数据最大冻结密度
    UINT8          ucDay;       // 支持的日数据最多存放天数 (0 ~ 31)
    UINT8          ucMonth;     // 支持的月数据最多存放月数 (0 ~ 12)
    UINT8          ucPCtrl;     // 支持的时段功控定值方案最多个数 (0 ~ 3)
    UINT8          ucHarm;      // 支持的谐波检测最高谐波次数 (0 ~ 19)
    UINT8          ucCapa;      // 支持的无功补偿电容器组最多组数 (0 ~ 16)
    UINT8          ucVip;       // 支持的台区集中抄表重点户最多户数 (0 ~ 20)
    UINT16         bBig;        // 支持的用户大类号标志
                                // 依次分别对应第0～15号用户大类，置"1"表示支持，置"0"表示不支持，
                                // 且对应的"支持用户小类号个数"应为0。
    UINT8          ucSmall[MT_USER_CLASS_NUM]; // 支持0 ~ 15号用户大类下的用户小类号个数

}sMtTmlOtherCfg_f, sMtAfn09F3_f;
#pragma pack() 
 
// 转换函数
eMtErr emtTrans_afn09f3(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F4：终端支持的参数配置
* 对应AFN : AFN_09_CFIG
* 对应CMD : CMD_AFN_9_F4_SUPPORT_PARA_CFG
* PN 类型 : P0
* 数据说明: 对可设置的配置 命令字进行说明  设置参数（AFN=04H）
            CMD_AFN_4_F1_TML_UP_CFG 到 CMD_AFN_4_F83_CD_FREEZE_PARA
{*///

// 用户侧
typedef struct
{
    UINT8   ucNum;       // 支持的配置的参数的个数
    eMtCmd  eCfgCmd[1];  // 支持的配置  CMD_AFN_4_F1_TML_UP_CFG 到 CMD_AFN_4_F83_CD_FREEZE_PARA

}sMtSuptParaCfg, sMtAfn09F4;

// 帧侧
typedef struct
{
    UINT8  ucTeam;        // 支持的信息类组数n
    UINT8  ucFlag[1];     // 支持的第1~n组信息类组所对应的信息类元标志位

}sMtSuptParaCfg_f, sMtAfn09F4_f;

// 辅助函数
eMtErr emtTrans_afn09f4_ast(eMtTrans eTrans,eMtCmd eCmd, UINT8 *pArray, UINT8 *pucTeam);

// 转换函数
eMtErr emtTrans_afn09f4(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F5：终端支持的控制配置
* 对应AFN : AFN_09_CFIG
* 对应CMD : CMD_AFN_9_F5_SUPPORT_CTRL_CFG
* PN 类型 : P0
* 数据说明: 对可设置的配置 命令字进行说明  设置参数（AFN=04H）
            CMD_AFN_5_F1_REMOTE_TURN_OFF 到 CMD_AFN_5_F53_DELET_ALL_METER
{*///

// 用户侧
typedef struct
{
    UINT8   ucNum;       // 支持的控制命令的个数
    eMtCmd  eCfgCmd[1];  // 支持的控制命令的命令字  CMD_AFN_5_F1_REMOTE_TURN_OFF 到 CMD_AFN_5_F53_DELET_ALL_METER

}sMtSuptCtrlCfg, sMtAfn09F5;

// 帧侧
typedef struct
{
    UINT8  ucTeam;        // 支持的信息类组数n
    UINT8  ucFlag[1];     // 支持的第1~n组信息类组所对应的信息类元标志位

}sMtSuptCtrlCfg_f, sMtAfn09F5_f;

// 辅助函数
eMtErr emtTrans_afn09f5_ast(eMtTrans eTrans,eMtCmd eCmd, UINT8 *pArray, UINT8 *pucTeam);

// 转换函数
eMtErr emtTrans_afn09f5(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F6：终端支持的1类数据配置
* 对应AFN : AFN_09_CFIG
* 对应CMD : CMD_AFN_9_F6_SUPPORT_ASK1_CFG
* PN 类型 : P0
* 数据说明: 对可设置的配置 命令字进行说明  设置参数（AFN=04H）
            CMD_AFN_C_F2_TML_CLOCK 到 CMD_AFN_C_F170_READ_METER
{*///

// 用户侧
typedef struct
{
    UINT8   ucNum;       // 终端支持的1类数据配个数
    eMtCmd  eCfgCmd[1];  // ucNum 个命令    CMD_AFN_C_F2_TML_CLOCK 到 CMD_AFN_C_F170_READ_METER

}sMtSuptAsk1, sMtAfn09F6;

// 帧侧
typedef struct
{
    UINT8  ucBigClass;     // D0～D15依次分别对应第0～15号用户大类，置"1"表示支持，置"0"表示不支持。
    UINT8  ucTeam;        // 支持的信息类组数n
    UINT8  ucFlag[1];     // 支持的第1~n组信息类组所对应的信息类元标志位

}sMtSuptAsk1_f, sMtAfn09F6_f;

// 辅助函数
eMtErr emtTrans_afn09f6_ast(eMtTrans eTrans,eMtCmd eCmd, UINT8 *pArray, UINT8 *pucTeam);

// 转换函数
eMtErr emtTrans_afn09f6(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F7：终端支持的2类数据配置
* 对应AFN : AFN_09_CFIG
* 对应CMD : CMD_AFN_9_F7_SUPPORT_ASK2_CFG
* PN 类型 : P0
* 数据说明: 对可设置的配置 命令字进行说明  设置参数（AFN=04H）
            CMD_AFN_D_F1_FRTH_POWR_P1P4_D 到 CMD_AFN_D_F218_COLOR_YAWP_CURVE
{*///

// 用户侧
typedef struct
{
    UINT8   ucNum;     // 支持的控制命令的FN个数
    eMtCmd  eCfgCmd[1];  // 支持的控制命令的命令字  CMD_AFN_D_F1_FRTH_POWR_P1P4_D 到 CMD_AFN_D_F218_COLOR_YAWP_CURVE

}sMtSuptAsk2, sMtAfn09F7;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucBigClass;    // D0～D15依次分别对应第0～15号用户大类，置"1"表示支持，置"0"表示不支持。
    UINT8  ucTeam;        // 支持的信息类组数n
    UINT8  ucFlag[1];     // 支持的第1~n组信息类组所对应的信息类元标志位

}sMtSuptAsk2_f, sMtAfn09F7_f;
#pragma pack() 

// 辅助函数
eMtErr emtTrans_afn09f7_ast(eMtTrans eTrans,eMtCmd eCmd, UINT8 *pArray, UINT8 *pucTeam);

// 转换函数
eMtErr emtTrans_afn09f7(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F8：终端支持的事件记录配置
* 对应AFN : AFN_09_CFIG
* 对应CMD : CMD_AFN_9_F8_SUPPORT_EVNT_CFG
* PN 类型 : P0
* 数据说明: MT_ERC_1_INIT 到 MT_ERC_35_UNKOWN
            
{*///

// 用户侧
typedef struct
{
    UINT8   ucNum;       // 终端支持的事件个数
    eMtErc  eErc[1];     // MT_ERC_1_INIT 到 MT_ERC_35_UNKOWN

}sMtSuptEvent, sMtAfn09F8;

// 帧侧
typedef struct
{
    UINT8  ucFlag[8];     // 支持的事件记录标志位：
                          // D0～D63按顺序对位表示事件代码ERC1～ERC64所定义的事件，位值为"0"表示不支持，为"1"表示支持。

}sMtSuptEvent_f, sMtAfn09F8_f;


// 转换函数
eMtErr emtTrans_afn09f8(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
* 数据结构: F10：终端电能表/交流采样装置配置参数   (查询命令参数)
* 对应AFN : AFN_0A_GETP
* 对应CMD : CMD_AFN_A_F10_TML_POWER_CFG
* PN 类型 : P0
* 数据说明: 查询的命令 CMD_AFN_4_F10_TML_POWER_CFG  参数  
            应答用结构 sMtTmlPowerCfg 
           
{*///

// 用户侧
typedef struct
{
    UINT16  usN;          // 本次查询数量n (1 ~ 2040) 
    UINT16  usID[1];      // 本次查询的第(1 ~ N)个对象序号

}sMtTmlPowerCfg_Q, sMtAfn0AF10;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT16  usN;          // 本次查询数量n (1 ~ 2040) 
    UINT16  usID[1];      // 本次查询的第(1 ~ N)个对象序号

}sMtTmlPowerCfg_Q_f, sMtAfn0AF10_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0af10(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F38：1类数据配置设置 （在终端支持的1类数据配置内）(查询命令参数)
* 对应AFN : AFN_0A_GETP
* 对应CMD : CMD_AFN_A_F38_CFG_ASK_1
* PN 类型 : P0
* 数据说明: 查询的命令 CMD_AFN_4_F38_CFG_ASK_1 参数  
            应答用结构 sMtAsk1Cfg
           
{*///

// 用户侧
typedef struct
{
    UINT8  ucBig;         // 本次查询的用户大类号
    UINT8  ucN;           // 本次查询数量n
    UINT8  ucSmall[1];    // 本次查询的第(1 ~ N)个用户小类号

}sMtAsk1Cfg_Q, sMtAfn0AF38;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucBig;         // 本次查询的用户大类号
    UINT8  ucN;           // 本次查询数量n
    UINT8  ucSmall[1];    // 本次查询的第(1 ~ N)个用户小类号
    
}sMtAsk1Cfg_Q_f, sMtAfn0AF38_f;
#pragma pack(0) 

// 转换函数
eMtErr emtTrans_afn0af38(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
* 数据结构: F38：1类数据配置设置 （在终端支持的1类数据配置内）(查询命令参数)
* 对应AFN : AFN_0A_GETP
* 对应CMD : CMD_AFN_A_F38_CFG_ASK_1
* PN 类型 : P0
* 数据说明: 查询的命令 CMD_AFN_4_F38_CFG_ASK_1 参数  
            应答用结构 sMtAsk1Cfg
           
{*///

// 用户侧
typedef struct
{
    UINT8  ucBig;         // 本次查询的用户大类号
    UINT8  ucN;           // 本次查询数量n
    UINT8  ucSmall[1];    // 本次查询的第(1 ~ N)个用户小类号

}sMtAsk2Cfg_Q, sMtAfn0AF39;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8  ucBig;         // 本次查询的用户大类号
    UINT8  ucN;           // 本次查询数量n
    UINT8  ucSmall[1];    // 本次查询的第(1 ~ N)个用户小类号
    
}sMtAsk2Cfg_Q_f, sMtAfn0AF39_f;
#pragma pack(0) 

// 转换函数
eMtErr emtTrans_afn0af39(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}




/*******************************************
* 数据结构: F2: 终端日历时钟
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F2_TML_CLOCK
* PN 类型: P0
{*///

//用户侧
typedef sMtUserClock sMtAfn0cF02;

// 帧侧
typedef sMtFrmClock  sMtAfn0cF02_f;

eMtErr emtTrans_afn0cf02(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F3: 终端参数状态
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F3_TML_PARA_STATE
* PN 类型: P0
{*///

#define MT_TERMINAL_STATUS_USER_MAX (248)
#define MT_TERMINAL_STATUS_FRM_MAX (31)
//用户侧
typedef struct 
{
    BOOL bParaStatus[1];  // 终端参数状态  248个数组
    
}sMtParaSta,sMtAfn0cF03;

// 帧侧
typedef struct
{
    UINT8  bParaStatus[MT_TERMINAL_STATUS_FRM_MAX];
    
}sMtAfn0cF03_f;

eMtErr emtTrans_afn0cf03(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F4: 终端上行通信状态
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F4_TML_UPCOM_STATE
* PN 类型: P0
{*///

#define MT_AFN0CF04_ENABLE  1
#define MT_AFN0CF04_DISABLE 0

//用户侧
typedef struct 
{
    BOOL    bCom;            // 允许/ 禁止通话
    BOOL    bUp;             // 允许/ 禁止上报
    
}sMtTmlUpComState, sMtAfn0cF04;

// 帧侧
typedef struct
{
    UINT8   ucUpStat:2;     // 终端主动上报状态
    UINT8   ucComStat:2;    // 终端通话状态
    UINT8   ucReserve:4;    // 保留备用
}sMtTmlUpComState_f, sMtAfn0cF04_f;

eMtErr emtTrans_afn0cf04(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
* 数据结构: F5: 终端通信状态
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F5_TML_CTRL_SET
* PN 类型: P0
{*///

// 帧侧
#pragma pack(1) 
typedef struct 
{
    UINT8 ucPowerNum;          // 功控定值方案号
    UINT8 ucPowerFlag;         // 功控时段有效标志位
    UINT8 ucPowerSta;          // 功控状态
    UINT8 ucElecSta;           // 电控状态
    UINT8 ucPowerTimeSta;      // 功控轮次状态
    UINT8 ucElecTimeSta;       // 电控轮次状态
    
}sMtComGroupSta_f;

typedef struct 
{
    UINT8             ucWarnSta;         // 保电、剔除和催费告警投入状态
    UINT8             ucGroupFlag;       // 总家组有效标志位
    sMtComGroupSta_f  sGroup[1];  // 总加组 
    
}sMtAfn0cF05_f;
#pragma pack()

// 用户侧
#define MT_AFN0CF05_MAXGROUPS   8

#define MT_AFN0CF05_REMAIN_ENABLE   (UINT8)(0X01)   // 保电状态使能
#define MT_AFN0CF05_DELETE_ENABLE   (UINT8)(0X02)   // 剔除状态使能
#define MT_AFN0CF05_PAYWAR_ENABLE   (UINT8)(0X04)   // 催费告警状态使能

typedef enum
{
    MT_AFN0CF05_SCHM_0,     
    MT_AFN0CF05_SCHM_1,
    MT_AFN0CF05_SCHM_2,
    MT_AFN0CF05_SCHM_3,
    
}eMtAfn0cf05_Schm;      //  功控定值方案号

#define MT_AFN0CF05_TIME_CTRL_ENABLE   (UINT8)(0X01)   // 时段控使能
#define MT_AFN0CF05_REST_CTRL_ENABLE   (UINT8)(0X02)   // 厂休控使能
#define MT_AFN0CF05_STOP_CTRL_ENABLE   (UINT8)(0X04)   // 营业报停使能
#define MT_AFN0CF05_DOWN_CTRL_ENABLE   (UINT8)(0X08)   // 当前功率下浮控使能

#define MT_AFN0CF05_MONTH_CTRL_ENABLE   (UINT8)(0X01)   // 月控使能
#define MT_AFN0CF05_BUY_CTRL_ENABLE     (UINT8)(0X02)   // 购电控使能

typedef struct 
{
    UINT8   ucScheme;   // 功控定值方案号
    BOOL    bPowerCtrl[8];  // 按顺序对位表示1 ~ 8时段控投入胡的有效时段
    BOOL    bTimeCtrl;  // 时段控投入状态
    BOOL    bRestCtrl;  // 厂休控投入状态
    BOOL    bStopCtrl;  // 营业报停控投入状态
    BOOL    bDownCtrl;  // 当前功率下浮控投入状态
    BOOL    bMonthCtrl; // 月电控投入状态
    BOOL    bBuyCtrl;   // 购电控投入状态
    BOOL    bPowerTimeSta[8];   // 按顺序对位表示1 ~ 8轮次开关功控受控状态
    BOOL    bElecTimeSta[8];    // 按顺序对位表示1 ~ 8轮次开关电控受控状态
}sMtComGroupSta;

typedef struct
{
    BOOL    bRemain;            // 保电投入状态
    BOOL    bDelete;            // 剔除投入状态
    BOOL    bPayWarn;           // 催费告警投入状态
    UINT8   ucGroupNum;         // 总加组个数
    BOOL    bGroup[MT_AFN0CF05_MAXGROUPS];        // 终端总加组对应1 ~ 8 总加组
    sMtComGroupSta sGroup[MT_AFN0CF05_MAXGROUPS]; // 总加组状态对应1 ~ 8 总加组
}sMtAfn0cF05;

eMtErr emtTrans_afn0cf05(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F6: 终端控制状态
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F6_TML_CTRL_STATE
* PN 类型: P0
{*///

//  帧侧
typedef struct 
{
    sMtFmt02_f sCurPower;       // 当前功率定值
    sMtFmt04_f sPowerDrift;      // 当前功率下浮控浮浮动系数
    UINT8      ucPowerOutSta;     // 功控跳闸输出状态
    UINT8      ucMonthElecOutSta; // 月电控跳闸输出状态
    UINT8      ucBuyElecOutSta;   // 购电控跳闸输出状态
    UINT8      ucPowerWarnSta;    // 功控越限告警状态
    UINT8      ucElecWarnSta;     // 功控越限告警状态 
}sMtCtrlGroupSta_f;

typedef struct 
{
    UINT8    ucTripOutSta;      // 遥控跳闸输出状态
    UINT8    ucPayWarnSta;      // 当前催费告警状态
    UINT8    ucGroupFlag;       // 总家组有效标志位
    sMtCtrlGroupSta_f sGroup[1];  // 总加组 
}sMtAfn0cF06_f;

#define MT_AFN0CF06_PAYWARNING_ENABLE   (UINT8)(0X01)   // 处于催费告警状态 
#define MT_AFN0CF06_PAYWARNING_DISABLE  (UINT8)(0X00)   // 不处于催费告警状态 

// 用户侧

#define MT_AFN0CF06_TIME_CTRL_STAT   (UINT8)(0X01)   // 时段控状态
#define MT_AFN0CF06_REST_CTRL_STAT   (UINT8)(0X02)   // 厂休控状态
#define MT_AFN0CF06_STOP_CTRL_STAT   (UINT8)(0X04)   // 营业报停状态
#define MT_AFN0CF06_DOWN_CTRL_STAT   (UINT8)(0X08)   // 当前功率下浮控状态

#define MT_AFN0CF06_MONTH_CTRL_STAT   (UINT8)(0X01)   // 月控状态
#define MT_AFN0CF06_BUY_CTRL_STAT     (UINT8)(0X02)   // 购电控状态

typedef struct 
{
    float       fCurPower;              // 当前功率定值
    sMtFloat    sPowerDrift;            // 当前功率下浮控浮浮动系数
    BOOL        bPowerOutSta[8];        // 按顺序对位表示1 ~ 8 轮次功控跳闸输出状态
    BOOL        bMonthElecOutSta[8];    // 按顺序对位表示1 ~ 8 轮次月电控跳闸输出状态
    BOOL        bBuyElecOutSta[8];      // 按顺序对位表示1 ~ 8 轮次购电控跳闸输出状态
    BOOL        bTimeCtrlSta;           // 时段控状态
    BOOL        bRestCtrlSta;           // 厂休控状态
    BOOL        bStopCtrlSta;           // 营业报停状态
    BOOL        bDownCtrlSta;           // 当前功率下浮控状态
    BOOL        bMonthCtrlSta;          // 月电控状态
    BOOL        bBuyCtrlSta;            // 购电控状态
}sMtCtrlGroupSta;

typedef struct
{
    BOOL    bTrip[8];        // 按顺序对位表示终端1 ~ 8 轮次遥控跳闸输出状态
    UINT8   ucPayWarn;       // 终端是否处于催费警告状态 
    BOOL    bGroup[8];       // 终端总加组对应1 ~ 8 总加组
    sMtCtrlGroupSta sGroup[8];  // 总加组当前控制状态
}sMtAfn0cF06;

eMtErr emtTrans_afn0cf06(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
* 数据结构: F7: 终端事件计数器当前值
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F7_TML_EC_VALUE
* PN 类型: P0
{*///

//  帧侧
typedef struct 
{
    UINT8   ucEc1;  // 重要事件计数器EC1
    UINT8   ucEc2;  // 重要事件计数器EC2
}sMtAfn0cF07_f;

// 用户侧
typedef sMtAfn0cF07_f sMtAfn0cF07;

eMtErr emtTrans_afn0cf07(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F8: 终端事件标志状态
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F8_TML_EVNT_FLAG
* PN 类型: P0
{*///

//  帧侧
typedef struct 
{
    UINT32  ucErcLow;   //按位表示ERC1 ~ ERC32 
    UINT32  ucErcHigh;  // 按位表示ERC33 ~ ERC64
}sMtAfn0cF08_f;

// 用户侧
#define MT_AFN0CF08_ERC_MAX   (64)   // ERC终端时间最大数
typedef struct
{
    BOOL bErc[MT_AFN0CF08_ERC_MAX];  // 保留，仅用于计算长度
}sMtAfn0cF08;

eMtErr emtTrans_afn0cf08(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F9: 终端状态量及变位标志
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F9_TML_STATE_BIT
* PN 类型: P0
{*///

//  帧侧
typedef struct 
{
    UINT8 ucReserve[2]; // 保留，仅用于计算长度 
    
}sMtAfn0cF09_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF09;

eMtErr emtTrans_afn0cf09(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F10: 终端与主站当日、月通信流量
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F10_TML_MONTH_FLOW
* PN 类型: P0
{*///

//  帧侧
typedef struct 
{
    UINT8 ucReserve[8]; // 保留，仅用于计算长度 
    
}sMtAfn0cF10_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF10;

eMtErr emtTrans_afn0cf10(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F11: 终端集中抄表状态信息
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F11_TML_READ_METER
* PN 类型: P0
{*///

//  帧侧
#pragma pack(1) 
typedef struct 
{
    UINT8 ucTmlComPort;       // 终端通信端口号
    UINT16 usMeterNum;        // 要抄电表总数
    UINT8 ucWorkState;        // 当前抄表工作状态标志
    UINT16 ucReadOkNum;       // 抄表成功块数
    UINT8 ucKeyMeterOkNum;    // 抄重点表成功块数
    sMtFrmClock sStartTime;   // 开始时间
    sMtFrmClock sEndTime;     // 结束时间
    
}sMtDataBlock;

typedef struct 
{
    UINT8   ucBlockNum;
    sMtDataBlock sDataBlock[1];
    
}sMtAfn0cF11_f;
#pragma pack()

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF11;

eMtErr emtTrans_afn0cf11(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F17: 当前总加有功功率
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F17_TOTAL_POWER_HAVE
* PN 类型: 总加组号
{*///

//  帧侧
typedef sMtsXXX sMtAfn0cF17_f;

// 用户侧
typedef struct
{
    float fsXXX;  // 三位有效数字(10[-3] ~ 10[4])
            
}sMtAfn0cF17, sMtTotalPowerHave;

eMtErr emtTrans_afn0cf17(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F18: 当前总加无功功率
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F18_TOTAL_POWER_NONE
* PN 类型: 总加组号
{*///

//  帧侧
typedef sMtsXXX sMtAfn0cF18_f;

// 用户侧
typedef struct
{
    float fsXXX;  // 三位有效数字(10[-3] ~ 10[4])
    
}sMtAfn0cF18, sMtTotalPowerNone;

eMtErr emtTrans_afn0cf18(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F19: 当日总加有功电能量(总、费率 1~M)
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F19_DAY_POWER_HAVE
* PN 类型: 总加组号
{*///

//  帧侧
#pragma pack(1) 
typedef struct
{
    UINT8   ucRateNum;               // 费率数M
    sMtFmt_sX7_f sTotalPowerHave;    // 当日总加有功总电量
    sMtFmt_sX7_f sDayPowerHave[1];   // 费率n当日总加有功电能量
    
}sMtAfn0cF19_f;
#pragma pack() 

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF19;

eMtErr emtTrans_afn0cf19(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F20: 当日总加无功电能量(总、费率 1~M)
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F20_DAY_POWER_NONE
* PN 类型: 总加组号
{*///

//  帧侧
#pragma pack(1) 
typedef struct
{
    UINT8   ucRateNum;               // 费率数M
    sMtFmt_sX7_f sTotalPowerNone;    // 当日总加有功总电量
    sMtFmt_sX7_f sDayPowerNone[1];   // 费率n当日总加有功电能量
    
}sMtAfn0cF20_f;
#pragma pack()

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
 
}sMtAfn0cF20;

eMtErr emtTrans_afn0cf20(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F21: 当月总加有功电能量(总、费率 1~M)
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F21_MONTH_POWER_HAVE
* PN 类型: 总加组号
{*///

//  帧侧
typedef  sMtAfn0cF19_f sMtAfn0cF21_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF21;

eMtErr emtTrans_afn0cf21(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F22: 当月总加无功电能量(总、费率 1~M)
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F22_MONTH_POWER_NONE
* PN 类型: 总加组号
{*///

//  帧侧
typedef sMtAfn0cF20_f sMtAfn0cF22_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF22;

eMtErr emtTrans_afn0cf22(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F23: 终端当前剩余电量(费)
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F23_TML_LEFT_POWER
* PN 类型: 总加组号
{*///

//  帧侧
typedef sMtFmt_sX7_f sMtAfn0cF23_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF23;

eMtErr emtTrans_afn0cf23(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F24: 当前功率下浮控控后总加有功功率冻结值
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F24_DOWN_FREZ_VALUE
* PN 类型: 总加组号
{*///

//  帧侧
typedef sMtsXXX sMtAfn0cF24_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;  // 保留，仅用于计算长度
    
}sMtAfn0cF24;

eMtErr emtTrans_afn0cf24(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F25_POWER_RATE_CUR
 * PN 类型 : 测量点号
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL            bsReadTime;     // 终端抄表时间    (分时日月年)
    BOOL            bfP;            // 当前总有功功率  (KW) (+/-)XX.XXXX 
    BOOL            bfPa;           // 当前A相有功功率 (KW) (+/-)XX.XXXX 
    BOOL            bfPb;           // 当前B相有功功率 (KW) (+/-)XX.XXXX 
    BOOL            bfPc;           // 当前C相有功功率 (KW) (+/-)XX.XXXX 

    BOOL            bfQ;            // 当前总无功功率  (KW) (+/-)XX.XXXX 
    BOOL            bfQa;           // 当前A相无功功率 (KW) (+/-)XX.XXXX 
    BOOL            bfQb;           // 当前B相无功功率 (KW) (+/-)XX.XXXX 
    BOOL            bfQc;           // 当前C相无功功率 (KW) (+/-)XX.XXXX 

    BOOL            bfPf;           // 当前总功率因数  (%)  (+/-)XXX.X
    BOOL            bfPfa;          // 当前A相功率因数 (%)  (+/-)XXX.X
    BOOL            bfPfb;          // 当前B相功率因数 (%)  (+/-)XXX.X
    BOOL            bfPfc;          // 当前C相功率因数 (%)  (+/-)XXX.X

    BOOL            bfUa;           // 当前A相电压     (V)       XXX.X
    BOOL            bfUb;           // 当前B相电压     (V)       XXX.X
    BOOL            bfUc;           // 当前C相电压     (V)       XXX.X

    BOOL            bfIa;           // 当前A相电流     (A)  (+/-)XXX.XXX     
    BOOL            bfIb;           // 当前B相电流     (A)  (+/-)XXX.XXX     
    BOOL            bfIc;           // 当前C相电流     (A)  (+/-)XXX.XXX     
    BOOL            bfI0;           // 当前零序电流    (A)  (+/-)XXX.XXX   
    
    BOOL            bfS;            // 当前总视在功率  (KW) (+/-)XX.XXXX 
    BOOL            bfSa;           // 当前A相视在功率 (KW) (+/-)XX.XXXX 
    BOOL            bfSb;           // 当前B相视在功率 (KW) (+/-)XX.XXXX 
    BOOL            bfSc;           // 当前C相视在功率 (KW) (+/-)XX.XXXX            

    // 实际数据
    sMtYYMMDDhhmm   sReadTime;      // 终端抄表时间    (分时日月年)
    float           fP;             // 当前总有功功率  (KW) (+/-)XX.XXXX 
    float           fPa;            // 当前A相有功功率 (KW) (+/-)XX.XXXX 
    float           fPb;            // 当前B相有功功率 (KW) (+/-)XX.XXXX 
    float           fPc;            // 当前C相有功功率 (KW) (+/-)XX.XXXX 

    float           fQ;             // 当前总无功功率  (KW) (+/-)XX.XXXX 
    float           fQa;            // 当前A相无功功率 (KW) (+/-)XX.XXXX 
    float           fQb;            // 当前B相无功功率 (KW) (+/-)XX.XXXX 
    float           fQc;            // 当前C相无功功率 (KW) (+/-)XX.XXXX 

    float           fPf;            // 当前总功率因数  (%)  (+/-)XXX.X
    float           fPfa;           // 当前A相功率因数 (%)  (+/-)XXX.X
    float           fPfb;           // 当前B相功率因数 (%)  (+/-)XXX.X
    float           fPfc;           // 当前C相功率因数 (%)  (+/-)XXX.X

    float           fUa;            // 当前A相电压     (V)       XXX.X
    float           fUb;            // 当前B相电压     (V)       XXX.X
    float           fUc;            // 当前C相电压     (V)       XXX.X

    float           fIa;            // 当前A相电流     (A)  (+/-)XXX.XXX     
    float           fIb;            // 当前B相电流     (A)  (+/-)XXX.XXX     
    float           fIc;            // 当前C相电流     (A)  (+/-)XXX.XXX     
    float           fI0;            // 当前零序电流    (A)  (+/-)XXX.XXX   
    
    float           fS;             // 当前总视在功率  (KW) (+/-)XX.XXXX 
    float           fSa;            // 当前A相视在功率 (KW) (+/-)XX.XXXX 
    float           fSb;            // 当前B相视在功率 (KW) (+/-)XX.XXXX 
    float           fSc;            // 当前C相视在功率 (KW) (+/-)XX.XXXX 
    
}sMtCurPQUI, sMtAfn0cF25;

// 帧侧
#pragma pack(1) 
typedef struct
{
    sMtYYMMDDhhmm_f           sReadTime;      // 终端抄表时间    (分时日月年)
    sMtFmt_sXX_XXXX           fP;             // 当前总有功功率  (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fPa;            // 当前A相有功功率 (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fPb;            // 当前B相有功功率 (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fPc;            // 当前C相有功功率 (KW) (+/-)XX.XXXX 

    sMtFmt_sXX_XXXX           fQ;             // 当前总无功功率  (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fQa;            // 当前A相无功功率 (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fQb;            // 当前B相无功功率 (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fQc;            // 当前C相无功功率 (KW) (+/-)XX.XXXX 

    sMtFmt_sXXX_X             fPf;            // 当前总功率因数  (%)  (+/-)XXX.X
    sMtFmt_sXXX_X             fPfa;           // 当前A相功率因数 (%)  (+/-)XXX.X
    sMtFmt_sXXX_X             fPfb;           // 当前B相功率因数 (%)  (+/-)XXX.X
    sMtFmt_sXXX_X             fPfc;           // 当前C相功率因数 (%)  (+/-)XXX.X

    sMtFmt_XXX_X              fUa;            // 当前A相电压     (V)       XXX.X
    sMtFmt_XXX_X              fUb;            // 当前B相电压     (V)       XXX.X
    sMtFmt_XXX_X              fUc;            // 当前C相电压     (V)       XXX.X

    sMtFmt_sXXX_XXX           fIa;            // 当前A相电流     (A)  (+/-)XXX.XXX     
    sMtFmt_sXXX_XXX           fIb;            // 当前B相电流     (A)  (+/-)XXX.XXX     
    sMtFmt_sXXX_XXX           fIc;            // 当前C相电流     (A)  (+/-)XXX.XXX     
    sMtFmt_sXXX_XXX           fI0;            // 当前零序电流    (A)  (+/-)XXX.XXX   
    
    sMtFmt_sXX_XXXX           fS;             // 当前总视在功率  (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fSa;            // 当前A相视在功率 (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fSb;            // 当前B相视在功率 (KW) (+/-)XX.XXXX 
    sMtFmt_sXX_XXXX           fSc;            // 当前C相视在功率 (KW) (+/-)XX.XXXX 
}sMtCurPUIQ_f, sMtAfn0cF25_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn0cf25(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);

///*}

/*******************************************
* 数据结构: F26:   A、B、C三相断相统计数据及最近一次断相记录
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F26_DROP_PHASE_LAST
* PN 类型: 测量点号
{*///

//  帧侧
#pragma pack(1) 
typedef struct 
{
    sMtYYMMDDhhmm_f sTime;              // 终端抄表时间
    sMtFmt_XXXX   sTotalPhaseBreakNum;  // 终端总断相次数
    sMtFmt_XXXX   sAPhaseBreakNum;      // A 相断相次数
    sMtFmt_XXXX   sBPhaseBreakNum;      // B 相断相次数
    sMtFmt_XXXX   sCPhaseBreakNum;      // C 相断相次数
    sMtFmt_XXXXXX sTotalBreakTime;      // 断相时间累计值
    sMtFmt_XXXXXX sABreakTime;          // A 相断相时间累计值
    sMtFmt_XXXXXX sBBreakTime;          // B 相断相时间累计值
    sMtFmt_XXXXXX sCBreakTime;          // B 相断相时间累计值
    sMtMMDDHHmm_f sStartClkLast;        // 最后一次断相起始时刻
    sMtMMDDHHmm_f sAStartClkLast;       // A相最近断相起始时刻
    sMtMMDDHHmm_f sBStartClkLast;       // B相最近断相起始时刻
    sMtMMDDHHmm_f sCStartClkLast;       // C相最近断相起始时刻
    sMtMMDDHHmm_f sEndClkLast;          // 最后一次断相结束时刻
    sMtMMDDHHmm_f sAEndClkLast;         // A相最近断相起始时刻
    sMtMMDDHHmm_f sBEndClkLast;         // B相最近断相起始时刻
    sMtMMDDHHmm_f sCEndClkLast;         // C相最近断相起始时刻
    
}sMtAfn0cF26_f;
#pragma pack()

// 用户侧
typedef struct
{
    UINT8 ucReserve;    // 保留，仅用于计算长度
    
}sMtAfn0cF26;

eMtErr emtTrans_afn0cf26(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F27:   电能表日历时钟、编程次数及其最近一次操作时间
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F27_METER_PROG_TIME
* PN 类型: 测量点号
{*///

//  帧侧
#pragma pack(1) 
typedef struct 
{
    sMtYYMMDDhhmm_f sTime;            // 终端抄表时间
    sMtFrmClock   sClock;             // 终端日历时钟
    sMtFmt_XXXXXXXX sBatteryTime;     // 电池工作时间
    sMtFmt_XXXXXX sProgramTimes;      // 编程总次数
    sMtFrmClock   sProgramClkLast;    // 最近一次编程发生时刻
    sMtFmt_XXXXXX sMtZeroTimes;       // 电表清零总次数
    sMtFrmClock   sMtZeroClkLast;     // 最近一次清零发生时刻
    sMtFmt_XXXXXX sNeedsZeroTimes;    // 需量清零总次数
    sMtFrmClock   sNeedsZeroClkLast;  // 最近一次需量清零发生时刻
    sMtFmt_XXXXXX sEventZeroTimes;    // 事件清零总次数
    sMtFrmClock   sEventZeroClkLast;  // 最近一次事件清零发生时刻
    sMtFmt_XXXXXX sTimingTimes;       // 校时总次数
    sMtFrmClock   sTimingClkLast;     // 最近一次校时发生时刻
    
}sMtAfn0cF27_f;
#pragma pack()

// 用户侧
typedef struct
{
    UINT8 ucReserve;    // 保留，仅用于计算长度
    
}sMtAfn0cF27;

eMtErr emtTrans_afn0cf27(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F28:   电表运行状态字及其变位标志
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F28_METER_WORD_CBIT
* PN 类型: 测量点号
{*///

//  帧侧
typedef struct 
{
    sMtYYMMDDhhmm_f sTime;               // 终端抄表时间
    UINT16          usMtStateFlag[7];    // 电表运行状态字变位标志
    UINT16          usMtState[7];        // 电表运行状态字
    
}sMtAfn0cF28_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;    // 保留，仅用于计算长度
    
}sMtAfn0cF28;

eMtErr emtTrans_afn0cf28(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F29:   当前铜损、铁损有功总电能示值
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F29_COPER_IRON_CUR
* PN 类型: 测量点号
{*///

//  帧侧
#pragma pack(1) 
typedef struct 
{
  sMtYYMMDDhhmm_f   sTime;          // 终端抄表时间
  union
    {
        sMtFmt_XXXXXX_XXXX sCopperValueCur;     // 当前铜损有功总电能示值
        sMtFmt_XXXXXX_XXXX sCopperValueLast;    // 上一结算铜损有功电能示值
        
    }uCopperValue;

  union
  {
    sMtFmt_XXXXXX_XXXX sIronValueCur;          // 当前铁损有功总电能示值
    sMtFmt_XXXXXX_XXXX sIronValueLast;         // 上一结算铁损有功电能示值
    
  }uIronValue;        
  
}sMtAfn0cF29_f;
#pragma pack()

// 用户侧
typedef struct
{
    UINT8 ucReserve;        // 保留，仅用于计算长度
    
}sMtAfn0cF29;

eMtErr emtTrans_afn0cf29(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
* 数据结构: F30:   上一结算日铜损、铁损有功总电能示值
* 对应AFN: AFN_0C_ASK1
* 对应CMD: CMD_AFN_C_F30_COPER_IRON_LAST
* PN 类型: 测量点号
{*///

//  帧侧
typedef sMtAfn0cF29_f sMtAfn0cF30_f;

// 用户侧
typedef struct
{
    UINT8 ucReserve;        // 保留，仅用于计算长度
    
}sMtAfn0cF30;

eMtErr emtTrans_afn0cf30(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F31：当前A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F31_POWER_CUR
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{   
    double dFrthHavePower;      // 正向有功电能示值  XXXXXX_XXXX (kWh)
    double dBackHavePower;      // 反向有功电能示值  XXXXXX_XXXX (kWh)
    double  fComNonePower1;     // 组合无功1电能示值 XXXXXX_XX   (Kvarh)
    double  fComNonePower2;     // 组合无功2电能示值 XXXXXX_XX   (Kvarh)
    
}sMtPowerPhase;                 // 分相 电能示值

typedef struct
{
    sMtYYMMDDhhmm  sTime;    // 终端抄表时间
    sMtPowerPhase  sPhaseA;  // 当前A相
    sMtPowerPhase  sPhaseB;  // 当前B相
    sMtPowerPhase  sPhaseC;  // 当前C相
    
}sMtCurPower, sMtAfn0cf31;

// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtFmt14_f  dFrthHavePower;    // 正向有功电能示值  XXXXXX_XXXX (kWh)
    sMtFmt14_f  dBackHavePower;    // 反向有功电能示值  XXXXXX_XXXX (kWh)
    sMtFmt11_f  fComNonePower1;    // 组合无功1电能示值 XXXXXX_XX   (Kvarh)
    sMtFmt11_f  fComNonePower2;    // 组合无功2电能示值 XXXXXX_XX   (Kvarh)
    
}sMtPowerPhase_f;                  // 分相 电能示值

typedef struct
{
    sMtFmt15_f       sTime;    // 终端抄表时间
    sMtPowerPhase_f  sPhaseA;  // 当前A相
    sMtPowerPhase_f  sPhaseB;  // 当前B相
    sMtPowerPhase_f  sPhaseC;  // 当前C相
    
}sMtPower_f, sMtAfn0cf31_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn0cf31(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F32：上一结算日A、B、C三相正/反向有功电能示值、组合无功1/2电能示值
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F32_POWER_LAST
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef sMtAfn0cf31 sMtAfn0cf32;

// 帧侧
typedef sMtAfn0cf31_f sMtAfn0cf32_f;

// 转换函数
eMtErr emtTrans_afn0cf32(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F33：当前正向有/ 无功电能示值、一/ 四象无功电能示值 ( 总、费率1  ~ M, 1 <= M  <= 12)
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F33_FRTH_POWR_P1P4_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef struct 
{
    sMtFmt14_f  sTotalPower;
    sMtFmt14_f  sPower[1];
}sMtHaveValue;        // 有功电能示值

typedef struct 
{
    sMtFmt11_f  sTotalPower;
    sMtFmt11_f  sPower[1];
}sMtNoneValue;       // 无功电能示值 

typedef sMtHaveValue sMtCurFrthHaveValue;        // 当前正向有功电能示值

typedef sMtNoneValue sMtCurFrthNoneValue;        // 当前正向无功电能示值

typedef sMtNoneValue sMtCurP1NoneValue;           // 当前一象限无功电能示值

typedef sMtNoneValue sMtCurP4NoneValue;           // 当前四象限无功电能示值       

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
    
}sMtAfn0cf33_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn0cf33(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F34：当前反向有/ 无功电能示值、二/ 三象无功电能示值 ( 总、费率1  ~ M, 1 <= M  <= 12)
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F34_BACK_POWR_P2P3_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtHaveValue sMtCurBackHaveValue;        // 当前反向有功电能示值

typedef sMtNoneValue sMtCurBackNoneValue;        // 当前反向无功电能示值

typedef sMtNoneValue sMtP3NoneValue;               // 当前三象限无功电能示值

typedef sMtNoneValue sMtP2NoneValue;               // 当前二象限无功电能示值

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
}sMtAfn0cf34_f;

// 转换函数
eMtErr emtTrans_afn0cf34(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F35： 当月正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F35_FRTH_DMND_M
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef struct 
{
    sMtFmt23_f  sMaxDmnd;       
    sMtFmt23_f  sDmnd[1];    
}sMtDmnd;                   // 最大需求量

typedef struct 
{
    sMtFmt17_f  sDmndTime;
    sMtFmt17_f  sTime[1];
    
}sMtDmndTime;            // 最大需求量发生时间


typedef sMtDmnd sMtCurFrthHaveDmnd;     // 当月正向有功最大需求量

typedef sMtDmnd sMtCurFrthNoneDmnd;     // 当月正向无功最大需求量

typedef sMtDmndTime sMtCurFrthHaveTime; // 当月正向有功最大需求量发生时间

typedef sMtDmndTime sMtCurFrthNoneTime; // 当月正向无功最大需求量发生时间

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
}sMtAfn0cf35_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf35(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F36：当月反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F36_BACK_DMND_M
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef sMtDmnd sMtCurBackHaveDmnd;     // 当月反向有功最大需求量

typedef sMtDmnd sMtCurBackNoneDmnd;     // 当月正向无功最大需求量

typedef sMtDmndTime sMtCurBackHaveTime; // 当月反向有功最大需求量发生时间

typedef sMtDmndTime sMtCurBackNoneTime; // 当月反向无功最大需求量发生时间

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
}sMtAfn0cf36_f;
#pragma pack()

// 转换函数
eMtErr emtTrans_afn0cf36(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F37：上月（上一结算日）正向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F37_FRTH_POWR_P1P4_L
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef sMtHaveValue sMtLastFrthHaveValue;      // 上月( 上一结算日) 正向有功电能示值

typedef sMtNoneValue sMtLastFrthNoneValue;      //  上月(上一结算日) 正向无功电能示值

typedef sMtNoneValue sMtLastP1NoneValue;         //  上月(上一结算日) 一象限无功电能示值

typedef sMtNoneValue sMtLastP4NoneValue;         //  上月(上一结算日) 四象限无功电能示值

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
}sMtAfn0cf37_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf37(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F38：上月（上一结算日）反向有/无功（组合无功1）电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F38_BACK_POWR_P2P3_L
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef sMtHaveValue sMtLastBackHaveValue;      // 上月( 上一结算日) 反向有功电能示值

typedef sMtNoneValue sMtLastBackNoneValue;      //  上月(上一结算日) 反向无功电能示值

typedef sMtNoneValue sMtLastP2NoneValue;         //  上月(上一结算日) 二象限无功电能示值

typedef sMtNoneValue sMtLastP3NoneValue;         //  上月(上一结算日) 三象限无功电能示值

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
}sMtAfn0cf38_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf38(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F39： 上月（上一结算日）正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F39_FRTH_DMND_L
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef sMtDmnd sMtLastFrthHaveDmnd;     // 上月（上一结算日）正向有功最大需求量

typedef sMtDmnd sMtLastFrthNoneDmnd;     // 上月（上一结算日）正向无功最大需求量

typedef sMtDmndTime sMtLastFrthHaveTime; // 上月（上一结算日）正向有功最大需求量发生时间

typedef sMtDmndTime sMtLastFrthNoneTime; // 上月（上一结算日）正向无功最大需求量发生时间

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
}sMtAfn0cf39_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf39(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F40：上月（上一结算日）反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F40_BACK_DMND_L
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack() 
typedef sMtDmnd sMtLastBackHaveDmnd;     // 上月（上一结算日）反向有功最大需求量

typedef sMtDmnd sMtLastBackNoneDmnd;     //上月（上一结算日）正向无功最大需求量

typedef sMtDmndTime sMtLastBackHaveTime; // 上月（上一结算日）反向有功最大需求量发生时间

typedef sMtDmndTime sMtLastBackNoneTime; // 上月（上一结算日）反向无功最大需求量发生时间

typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucRateM;        // 费率数
}sMtAfn0cf40_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf40(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当日正向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F41_FRTH_HAVE_POWR_D
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

#define      MT_PAY_RATE_MAX    (64)
// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdForthHavePowerD;                   // 当日正向有功总电能量
    BOOL                bdForthHavePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)正向有功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dForthHavePowerD;                   // 当日正向有功总电能量
    double              dForthHavePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)正向有功电能量  
    
}sMtForthHavePowerD, sMtAfn0cF41;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dForthHavePowerD;                   // 当日正向有功总电能量
    sMtFmt_XXXX_XXXX    dForthHavePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)正向有功电能量  
    
}sMtForthHavePowerD_f, sMtAfn0cF41_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf41(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当日正向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F42_FRTH_NONE_POWR_D
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdForthNonePowerD;                   // 当日正向无功总电能量
    BOOL                bdForthNonePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)正向无功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dForthNonePowerD;                   // 当日正向无功总电能量
    double              dForthNonePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)正向无功电能量  
    
}sMtForthNonePowerD, sMtAfn0cF42;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dForthNonePowerD;                   // 当日正向无功总电能量
    sMtFmt_XXXX_XXXX    dForthNonePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)正向无功电能量  
    
}sMtForthNonePowerD_f, sMtAfn0cF42_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf42(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当日反向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F43_BACK_HAVE_POWR_D
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdBackHavePowerD;                   // 当日反向有功总电能量
    BOOL                bdBackHavePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)反向有功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dBackHavePowerD;                   // 当日反向有功总电能量
    double              dBackHavePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)正向有功电能量  
    
}sMtBackHavePowerDay, sMtAfn0cF43;

// 帧侧
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dBackHavePowerD;                   // 当日反向有功总电能量
    sMtFmt_XXXX_XXXX    dBackHavePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)反向有功电能量  
    
}sMtBackHavePowerDay_f, sMtAfn0cF43_f;

// 转换函数
eMtErr emtTrans_afn0cf43(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当日反向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F44_BACK_NONE_POWR_D
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdBackNonePowerD;                   // 当日反向无功电能量
    BOOL                bdBackNonePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)反向无功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dBackNonePowerD;                   // 当日反向无功总电能量
    double              dBackNonePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)反向无功电能量  
    
}sMtBackNonePowerDay, sMtAfn0cF44;

// 帧侧
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dBackNonePowerD;                   // 当日反向无功电能量
    sMtFmt_XXXX_XXXX    dBackNonePowerMD[MT_PAY_RATE_MAX]; // 当日费率(1 ~ M)反向无功电能量  
    
}sMtBackNonePowerDay_f, sMtAfn0cF44_f;

// 转换函数
eMtErr emtTrans_afn0cf44(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当月正向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F45_FRTH_HAVE_POWR_M
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdForthHavePowerM;                   // 当月正向有功总电能量
    BOOL                bdForthHavePowerMM[MT_PAY_RATE_MAX]; // 当月费率(1 ~ M)正向有功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dForthHavePowerM;                   // 当月正向有功总电能量
    double              dForthHavePowerMM[MT_PAY_RATE_MAX]; // 当月费率(1 ~ M)正向有功电能量  
    
}sMtForthHavePowerM, sMtAfn0cF45;

// 帧侧
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dForthHavePowerM;                   // 当月正向有功总电能量
    sMtFmt_XXXX_XXXX    dForthHavePowerMM[MT_PAY_RATE_MAX]; // 当月费率(1 ~ M)正向有功电能量  
    
}sMtForthHavePowerM_f, sMtAfn0cF45_f;

// 转换函数
eMtErr emtTrans_afn0cf45(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当月正向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F46_FRTH_NONE_POWR_M
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdForthNonePowerM;                   // 当月正向无功总电能量
    BOOL                bdForthNonePowerMM[MT_PAY_RATE_MAX]; // 当月费率(1 ~ M)正向无功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dForthNonePowerM;                   // 当月正向无功总电能量
    double              dForthNonePowerMM[MT_PAY_RATE_MAX]; // 当月费率(1 ~ M)正向无功电能量  
    
}sMtForthNonePowerM, sMtAfn0cF46;

// 帧侧
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dForthNonePowerM;                   // 当月正向无功总电能量
    sMtFmt_XXXX_XXXX    dForthNonePowerMM[MT_PAY_RATE_MAX]; // 当月费率(1 ~ M)正向无功电能量  
    
}sMtForthNonePowerM_f, sMtAfn0cF46_f;

// 转换函数
eMtErr emtTrans_afn0cf46(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当月反向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F47_BACK_HAVE_POWR_M
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdBackHavePowerM;                   // 当月反向有功总电能量
    BOOL                bdBackHavePowerMM[MT_PAY_RATE_MAX]; // 当月费率(1 ~ M)反向有功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dBackHavePowerM;                    // 当月反向有功总电能量
    double              dBackHavePowerMM[MT_PAY_RATE_MAX];  // 当月费率(1 ~ M)正向有功电能量  
    
}sMtBackHavePowerMc, sMtAfn0cF47;

// 帧侧
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dBackHavePowerM;                    // 当月反向有功总电能量
    sMtFmt_XXXX_XXXX    dBackHavePowerMM[MT_PAY_RATE_MAX];  // 当月费率(1 ~ M)反向有功电能量  
    
}sMtBackHavePowerMc_f, sMtAfn0cF47_f;

// 转换函数
eMtErr emtTrans_afn0cf47(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 当月反向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F48_BACK_NONE_POWR_M
 * PN 类型 : 测量点号
 * 费率范围:  M (1 ~ 14) (DLT645 1997)
 * 费率范围:  M (1 ~ 63) (DLT645 2007)
{*///

// 用户侧
typedef struct
{
    // 有无数据 分别对应实际数据 为真则表示有效
    BOOL                bucM;                                // 费率数M
    BOOL                bdBackNonePowerM;                    // 当月反向无功电能量
    BOOL                bdBackNonePowerMM[MT_PAY_RATE_MAX];  // 当月费率(1 ~ M)反向无功电能量 
    
    // 实际数据
    UINT8               ucM;                                // 费率数M
    double              dBackNonePowerM;                    // 当月反向无功总电能量
    double              dBackNonePowerMM[MT_PAY_RATE_MAX];  // 当月费率(1 ~ M)反向无功电能量  
    
}sMtBackNonePowerMc, sMtAfn0cF48;

// 帧侧
typedef struct
{
    UINT8               ucM;                                // 费率数M
    sMtFmt_XXXX_XXXX    dBackNonePowerM;                    // 当月反向无功电能量
    sMtFmt_XXXX_XXXX    dBackNonePowerMM[MT_PAY_RATE_MAX];  // 当月费率(1 ~ M)反向无功电能量  
    
}sMtBackNonePowerMc_f, sMtAfn0cF48_f;

// 转换函数
eMtErr emtTrans_afn0cf48(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F49：当前电压、电流相位角
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F49_CURT_PHASE_ANGLE
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct 
{
    sMtFmt05    sUab;            // Uab/Ua相位角
    sMtFmt05    sUb;             // Ub相位角
    sMtFmt05    sUcb;            // Ucb/Uc相位角
    sMtFmt05    sIa;             // Ia相位角
    sMtFmt05    sIb;             // Ib相位角
    sMtFmt05    sIc;             // Ic相位角
}sMtAfn0cf49_f;

// 转换函数
eMtErr emtTrans_afn0cf49(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F57：当前A、B、C三相电压、电流2～N次谐波有效值
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F57_CURT_HARM_VALUE
 * PN 类型 : 测量点号
{*///

#define MT_HARM_TIMES_MIN          (2)    // 谐波次数最小值
#define MT_HARM_TIMES_MAX          (19)   // 谐波次数最大值

typedef struct
{
    BOOL  bfXXX_X[MT_HARM_TIMES_MAX];     // 有无对应的数据
    float fXXX_X[MT_HARM_TIMES_MAX];      // 电压谐波 格式 XXX.X (V)

}sMtHarmU;

typedef struct
{
    BOOL  bfsXX_XX[MT_HARM_TIMES_MAX];    // 有无对应的数据
    float fsXX_XX[MT_HARM_TIMES_MAX];     // 电流谐波 格式 (+/-) XX.XX (A)

}sMtHarmI;

// 用户侧
typedef struct
{
    UINT8        ucN;                     // 谐波次数N（2 ≤N ≤19）
    sMtHarmU     sUa;                     // A相2次~N次谐波电压
    sMtHarmU     sUb;                     // B相2次~N次谐波电压
    sMtHarmU     sUc;                     // C相2次~N次谐波电压
    sMtHarmI     sIa;                     // A相2次~N次谐波电流
    sMtHarmI     sIb;                     // B相2次~N次谐波电流
    sMtHarmI     sIc;                     // C相2次~N次谐波电流
    
}sMtCurtHarmValue, sMtAfn0cF57;

// 帧侧
#pragma pack(1) 
typedef union
{       
    sMtFmt_XXX_X   XXX_X;
    sMtFmt_sXX_XX  sXX_XX;
    
}uMtHarm;

typedef struct
{
     UINT8      ucN;                     // 谐波次数N（2 ≤N ≤19）
     uMtHarm    uHarm[1];                // 6 * (N-1)个
    
}sMtCurtHarmValue_f, sMtAfn0cF57_f;
#pragma pack() 
// 转换函数
eMtErr emtTrans_afn0cf57(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F58：当前A、B、C三相电压、电流2～N次谐波含有率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F58_CURT_HARM_RATE
 * PN 类型 : 测量点号
{*///

typedef struct
{
    BOOL  bfsXXX_X;                       // 有无对应的数据
    float fsXXX_X;                        // 谐波电压含有率 格式  (+/-) XXX.X (%)

}sMtHarmRateUt;

typedef struct
{
    sMtHarmRateUt sTotal;                  // 总谐波电压含有率
    BOOL  bfsXXX_X[MT_HARM_TIMES_MAX];     // 有无对应的数据
    float fsXXX_X[MT_HARM_TIMES_MAX];      // 谐波电压含有率 格式  (+/-) XXX.X (%)

}sMtHarmRateU;

typedef struct
{
    BOOL  bfsXXX_X;                        // 有无对应的数据
    float fsXXX_X;                         // 谐波电流含有率 格式 (+/-) XXX.X (%)

}sMtHarmRateIt;

typedef struct
{
    sMtHarmRateIt   sTotal;               // 总谐波电流含有率
    BOOL  bfsXXX_X[MT_HARM_TIMES_MAX];    // 有无对应的数据
    float fsXXX_X[MT_HARM_TIMES_MAX];     // 谐波电流含有率 格式 (+/-) XXX.X (%)

}sMtHarmRateI;

// 用户侧
typedef struct
{
    UINT8            ucN;                  // 谐波次数N（2 ≤N ≤19）

    sMtHarmRateU     sUa;                  // A相2次~N次谐波电压含有率
    sMtHarmRateU     sUb;                  // B相2次~N次谐波电压含有率
    sMtHarmRateU     sUc;                  // C相2次~N次谐波电压含有率

    sMtHarmRateI     sIa;                  // A相2次~N次谐波电流含有率
    sMtHarmRateI     sIb;                  // B相2次~N次谐波电流含有率
    sMtHarmRateI     sIc;                  // C相2次~N次谐波电流含有率
    
}sMtCurtHarmRate, sMtAfn0cF58;

// 帧侧
#pragma pack(1) 
typedef struct
{
     UINT8            ucN;                 // 谐波次数N（2 ≤N ≤19）
     sMtFmt_sXXX_X    sXXX_X[1];           // 6 * (N-1)个
    
}sMtCurtHarmRate_f, sMtAfn0cF58_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf58(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F65：当前电容器投切状态
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F65_CURT_CAPA_SWITCH
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧

typedef struct 
{
   UINT8    ucRunMode;      // 运行方式
   UINT16   usCapaSwtich;   // 电容器的投切状态
}sMtAfn0cf65_f;

// 转换函数
eMtErr emtTrans_afn0cf65(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F66：当前电容器累计补偿投入时间和次数
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F66_CURT_CAPA_TIME
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧

typedef struct 
{
   UINT32   uiSeconds[9];   // 当前n组电容器累计投入时间(秒)
   UINT16   uiTimes[9];     // 当前n组电容器累计投入次数
}sMtAfn0cf66_f;

// 转换函数
eMtErr emtTrans_afn0cf66(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F67：当日、当月电容器累计补偿的无功电能量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F67_CURT_CAPA_POWR
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧

typedef struct 
{
   sMtFmt13  sCurDayNonePower;      // 当前日补偿的无功电能量
   sMtFmt13  sCurMonthNonePower;    // 当前月补偿的无功电能量
}sMtAfn0cf67_f;

// 转换函数
eMtErr emtTrans_afn0cf67(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F73：直流模拟量当前数据
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F73_DC_RLTM_DATA
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
//float fsXXX         (+/-)XXX  (10(4) ~ 10(-3))

// 帧侧

typedef sMtFmt02_f sMtAfn0cf73_f;   // 直流模拟量当前数据

// 转换函数
eMtErr emtTrans_afn0cf73(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F81：小时冻结总加有功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F81_HFRZ_GRUP_RATE_HAVE
 * PN 类型 : 总加组号
 *
{*///

// 用户侧


// 帧侧

typedef struct 
{
    UINT8   ucReserve:2;        // 保留备用
    UINT8   ucHourDec:2;        // 冻结时间十位( 0 ~ 23)
    UINT8   ucHour:4;           // 冻结时间个位
    UINT8   ucFrzFreq;          // 冻结密度
}sMtTimeStamp;

enum
{
    MT_FRZ_NONE,            // 不冻结
    MT_FRZ_15,              // 15分钟冻结一次
    MT_FRZ_30,              // 30分钟冻结一次
    MT_FRZ_60,              // 60分钟冻结一次
    MT_FRZ_5 = 254,        // 5分钟冻结一次
    MT_FRZ_1 = 255,        // 1分钟冻结一次
    MT_FRZ_UNKNOWN,        // 其他备用
};

typedef struct 
{
    sMtTimeStamp    sTimeStamp;     // 小时冻结类数据时标
    sMtFmt02_f      sPower[1];      // 功率
}sMtFrzGrupPower;

typedef sMtFrzGrupPower sMtAfn0cf81_f;

// 转换函数
eMtErr emtTrans_afn0cf81(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F82：小时冻结总加无功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F82_HFRZ_GRUP_RATE_NONE
 * PN 类型 :  总加组号
 *
{*///

// 用户侧


// 帧侧

typedef sMtFrzGrupPower sMtAfn0cf82_f;

// 转换函数
eMtErr emtTrans_afn0cf82(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F83：小时冻结总加有功总电能量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F83_HFRZ_GRUP_POWR_HAVE
 * PN 类型 : 总加组号
 *
{*///

// 用户侧


// 帧侧

typedef struct 
{
    sMtTimeStamp    sTimeStamp;     // 小时冻结类数据时标
    sMtFmt03_f      sValue[1];      // 电量
}sMtFrzGrupValue;           // 冻结电能量

typedef sMtFrzGrupValue sMtAfn0cf83_f;

// 转换函数
eMtErr emtTrans_afn0cf83(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F84：小时冻结总加无功总电能量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F84_HFRZ_GRUP_POWR_NONE
 * PN 类型 : 总加组号
 *
{*///

// 用户侧


// 帧侧

typedef sMtFrzGrupValue sMtAfn0cf84_f;

// 转换函数
eMtErr emtTrans_afn0cf84(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F89：小时冻结有功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F89_HFRZ_RATE_HAVE
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct 
{ 
    sMtTimeStamp    sTimeStamp;
    sMtFmt09        sPower[1];
}sMtFrzPower;

typedef sMtFrzPower sMtAfn0cf89_f;

// 转换函数
eMtErr emtTrans_afn0cf89(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F90：小时冻结A相有功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F90_HFRZ_RATE_HAVE_A
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzPower sMtAfn0cf90_f;

// 转换函数
eMtErr emtTrans_afn0cf90(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F91：小时冻结B 相有功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F91_HFRZ_RATE_HAVE_B
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzPower sMtAfn0cf91_f;

// 转换函数
eMtErr emtTrans_afn0cf91(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F92：小时冻结C 相有功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F92_HFRZ_RATE_HAVE_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzPower sMtAfn0cf92_f;

// 转换函数
eMtErr emtTrans_afn0cf92(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F93：小时冻结无功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F93_HFRZ_RATE_NONE
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzPower sMtAfn0cf93_f;

// 转换函数
eMtErr emtTrans_afn0cf93(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F94：小时冻结A  相无功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F94_HFRZ_RATE_NONE_A
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzPower sMtAfn0cf94_f;

// 转换函数
eMtErr emtTrans_afn0cf94(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F95：小时冻结B  相无功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F95_HFRZ_RATE_NONE_B
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzPower sMtAfn0cf95_f;

// 转换函数
eMtErr emtTrans_afn0cf95(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F96：小时冻结C  相无功功率
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F96_HFRZ_RATE_NONE_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzPower sMtAfn0cf96_f;

// 转换函数
eMtErr emtTrans_afn0cf96(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F97：小时冻结A相电压
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F97_HFRZ_VOLT_A
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct 
{ 
    sMtTimeStamp    sTimeStamp;
    sMtFmt07        sVolt[1];
}sMtFrzVolt;

typedef sMtFrzVolt sMtAfn0cf97_f;

// 转换函数
eMtErr emtTrans_afn0cf97(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F98：小时冻结B相电压
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F97_HFRZ_VOLT_B
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzVolt sMtAfn0cf98_f;

// 转换函数
eMtErr emtTrans_afn0cf98(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F99：小时冻结C相电压
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F97_HFRZ_VOLT_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzVolt sMtAfn0cf99_f;

// 转换函数
eMtErr emtTrans_afn0cf99(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F100：小时冻结A相电流
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F100_HFRZ_ELEC_A
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct 
{ 
    sMtTimeStamp    sTimeStamp;
    sMtFmt25        sElec[1];
}sMtFrzElec;

typedef sMtFrzElec sMtAfn0cf100_f;

// 转换函数
eMtErr emtTrans_afn0cf100(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F101：小时冻结B相电流
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F101_HFRZ_ELEC_B
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzElec sMtAfn0cf101_f;

// 转换函数
eMtErr emtTrans_afn0cf101(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F102：小时冻结C相电流
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F102_HFRZ_ELEC_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzElec sMtAfn0cf102_f;

// 转换函数
eMtErr emtTrans_afn0cf102(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F103：小时冻结零序电流
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F103_HFRZ_ELEC_ZERO
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzElec sMtAfn0cf103_f;

// 转换函数
eMtErr emtTrans_afn0cf103(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F105：小时冻结正向有功总电能量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F105_HFRZ_FRTH_HAVE
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct 
{ 
    sMtTimeStamp    sTimeStamp;
    sMtFmt13        sErgy[1];
}sMtFrzErgy;                

typedef sMtFrzErgy sMtAfn0cf105_f;

// 转换函数
eMtErr emtTrans_afn0cf105(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F106：小时冻结正向无功总电能量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F106_HFRZ_FRTH_NONE
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzErgy sMtAfn0cf106_f;

// 转换函数
eMtErr emtTrans_afn0cf106(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F107：小时冻结反向有功总电能量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F107_HFRZ_BACK_HAVE
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzErgy sMtAfn0cf107_f;

// 转换函数
eMtErr emtTrans_afn0cf107(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F108：小时冻结反向无功总电能量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F108_HFRZ_BACK_NONE
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzElec sMtAfn0cf108_f;

// 转换函数
eMtErr emtTrans_afn0cf108(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F109：小时冻结正向有功总电能示值
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F109_HFRZ_FRTH_HAVE_S
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct 
{ 
    sMtTimeStamp    sTimeStamp;
    sMtFmt11        sValue[1];
}sMtFrzValue;                

typedef sMtFrzValue sMtAfn0cf109_f;

// 转换函数
eMtErr emtTrans_afn0cf109(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F110：小时冻结正向无功总电能示值
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F110_HFRZ_FRTH_NONE_S
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzValue sMtAfn0cf110_f;

// 转换函数
eMtErr emtTrans_afn0cf110(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F111：小时冻结反向有功总电能示值
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F111_HFRZ_BACK_HAVE_S
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzValue sMtAfn0cf111_f;

// 转换函数
eMtErr emtTrans_afn0cf111(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F112： 小时冻结反向无功总电能示值
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F112_HFRZ_BACK_NONE_S
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtFrzValue sMtAfn0cf112_f;

// 转换函数
eMtErr emtTrans_afn0cf112(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F113：小时冻结总功率因数
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F113_HFRZ_FACT_TOTAL
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct 
{ 
    sMtTimeStamp    sTimeStamp;
    sMtFmt05        sFact[1];
}sMtFrzFact;                

typedef sMtFrzFact sMtAfn0cf113_f;

// 转换函数
eMtErr emtTrans_afn0cf113(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F114：小时冻结A相功率因数
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F114_HFRZ_FACT_A
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
      
typedef sMtFrzFact sMtAfn0cf114_f;

// 转换函数
eMtErr emtTrans_afn0cf114(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F115：小时冻结B相功率因数
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F115_HFRZ_FACT_B
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
      
typedef sMtFrzFact sMtAfn0cf115_f;

// 转换函数
eMtErr emtTrans_afn0cf115(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F116：小时冻结C相功率因数
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F116_HFRZ_FACT_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧


// 帧侧
      
typedef sMtFrzFact sMtAfn0cf116_f;

// 转换函数
eMtErr emtTrans_afn0cf116(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F121：小时冻结直流模拟量
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F121_HFRZ_DC_VALUE
 * PN 类型 :  直流模拟量端口号
 *
{*///

// 用户侧


// 帧侧

typedef sMtFrzGrupPower sMtAfn0cf121_f;

// 转换函数
eMtErr emtTrans_afn0cf121(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F129：当前正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F129_FRTH_HAVE_POWR_C
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct 
{
    sMtFmt15    sTime;          // 终端抄表时间
    UINT8       ucM;            // 费率数M >= 1
    double      dTotalValue;   // 正向有功电能示值 dXXXXXX.XXXX
    double      dValueItem[1]; // 费率m 正向有功电能示值 dXXXXXX.XXXX
}sMtAfn0cf129, sMtFrthHavePower;


// 帧侧
typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucM;            // 费率数M >= 1
    sMtFmt14_f  sTotalValue;   // 正向有功电能示值
    sMtFmt14_f  sValueItem[1]; // 费率m 正向有功电能示值
}sMtAfn0cf129_f, sMtFrthHavePower_f;

// 转换函数
eMtErr emtTrans_afn0cf129(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F130：当前正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F130_FRTH_NONE_POWR_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧
typedef struct 
{
    sMtFmt15 sTime;          // 终端抄表时间
    UINT8    ucM;            // 费率数M
    DOUBLE   dTotalValue;   // 正向有功电能示值 dXXXXXX.XX
    DOUBLE   dValueItem[1]; // 费率m 正向有功电能示值 dXXXXXX.XX
}sMtAfn0cf130, sMtFrthNonePower;

// 帧侧
#pragma pack(1) 
typedef struct 
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucM;        // 费率数M
    sMtFmt11_f  sTotalValue;   // 正向有功电能示值
    sMtFmt11_f  sValueItem[1]; // 费率m 正向有功电能示值
}sMtAfn0cf130_f, sMtFrthNonePower_f;
#pragma pack() 
// 转换函数
eMtErr emtTrans_afn0cf130(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F131：当前反向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F131_BACK_HAVE_POWR_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf129_f sMtAfn0cf131_f;

// 转换函数
eMtErr emtTrans_afn0cf131(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F132：当前反向无功（组合无功1）电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F132_BACK_NONE_POWR_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf132_f;

// 转换函数
eMtErr emtTrans_afn0cf132(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F133：当前一象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F133_NONE_POWR_P1_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf133_f;

// 转换函数
eMtErr emtTrans_afn0cf133(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F134：当前二象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F134_NONE_POWR_P2_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf134_f;

// 转换函数
eMtErr emtTrans_afn0cf134(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F135：当前三象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F135_NONE_POWR_P3_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf135_f;

// 转换函数
eMtErr emtTrans_afn0cf135(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F136：当前四象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F136_NONE_POWR_P4_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf136_f;

// 转换函数
eMtErr emtTrans_afn0cf136(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F137：上月（上一结算日）正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F137_FRTH_HAVE_POWR_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf129_f sMtAfn0cf137_f;

// 转换函数
eMtErr emtTrans_afn0cf137(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F138：上月（上一结算日）正向无功（组合无功1）电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F138_FRTH_NONE_POWR_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf138_f;

// 转换函数
eMtErr emtTrans_afn0cf138(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F139：上月（上一结算日）反向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F139_BACK_HAVE_POWR_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf129_f sMtAfn0cf139_f;

// 转换函数
eMtErr emtTrans_afn0cf139(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F140：上月（上一结算日）反向无功（组合无功1）电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F140_BACK_NONE_POWR_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf140_f;

// 转换函数
eMtErr emtTrans_afn0cf140(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F141： 上月（上一结算日）一象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F141_NONE_POWR_P1_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf141_f;

// 转换函数
eMtErr emtTrans_afn0cf141(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F142： 上月（上一结算日）二象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F142_NONE_POWR_P2_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf142_f;

// 转换函数
eMtErr emtTrans_afn0cf142(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F143： 上月（上一结算日）三象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F143_NONE_POWR_P3_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf143_f;

// 转换函数
eMtErr emtTrans_afn0cf143(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F144： 上月（上一结算日）四象限无功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F144_NONE_POWR_P4_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef sMtAfn0cf130_f sMtAfn0cf144_f;

// 转换函数
eMtErr emtTrans_afn0cf144(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F145： 当月正向有功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F145_FRTH_HAVE_DMND_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef struct 
{
    sMtFmt23_f  sDmnd;          // 最大需量
    sMtFmt17_f  sClock;         // 最大需量发生时间
}sMtDmndClock;

typedef struct
{
    sMtFmt15_f      sTime;          // 终端抄表时间
    UINT8           ucRateM;        // 费率数M
    sMtDmndClock    sDmndClock[1]; // 终端最大需量及发生时间
}sMtAfn0cf145_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf145(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F146：当月正向无功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F146_FRTH_NONE_DMND_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf145_f sMtAfn0cf146_f;

// 转换函数
eMtErr emtTrans_afn0cf146(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F147：当月反向有功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F147_BACK_HAVE_DMND_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf145_f sMtAfn0cf147_f;

// 转换函数
eMtErr emtTrans_afn0cf147(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F148：当月反向无功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F148_BACK_NONE_DMND_C
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf145_f sMtAfn0cf148_f;

// 转换函数
eMtErr emtTrans_afn0cf148(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F149：上月（上一结算日）正向有功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F149_FRTH_HAVE_DMND_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf145_f sMtAfn0cf149_f;

// 转换函数
eMtErr emtTrans_afn0cf149(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F150：上月（上一结算日）正向无功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F150_FRTH_NONE_DMND_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf145_f sMtAfn0cf150_f;

// 转换函数
eMtErr emtTrans_afn0cf150(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F151：上月（上一结算日）反向有功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F151_BACK_HAVE_DMND_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf145_f sMtAfn0cf151_f;

// 转换函数
eMtErr emtTrans_afn0cf151(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F152：上月（上一结算日）反向无功最大需量及发生时间（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F152_BACK_NONE_DMND_L
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf145_f sMtAfn0cf152_f;

// 转换函数
eMtErr emtTrans_afn0cf152(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F153： 第一时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F153_FREZ_ZONE_1
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf153_f;

// 转换函数
eMtErr emtTrans_afn0cf153(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F154： 第二时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F154_FREZ_ZONE_2
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf154_f;

// 转换函数
eMtErr emtTrans_afn0cf154(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F155： 第三时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F155_FREZ_ZONE_3
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf155_f;

// 转换函数
eMtErr emtTrans_afn0cf155(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F156： 第四时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F156_FREZ_ZONE_4
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf156_f;

// 转换函数
eMtErr emtTrans_afn0cf156(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F157： 第五时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F157_FREZ_ZONE_5
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf157_f;

// 转换函数
eMtErr emtTrans_afn0cf157(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F158：第六时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F158_FREZ_ZONE_6
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf158_f;

// 转换函数
eMtErr emtTrans_afn0cf158(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F159：第七时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F159_FREZ_ZONE_7
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf159_f;

// 转换函数
eMtErr emtTrans_afn0cf159(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F160：第八时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F160_FREZ_ZONE_8
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧

typedef sMtAfn0cf129_f sMtAfn0cf160_f;

// 转换函数
eMtErr emtTrans_afn0cf160(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F161：电能表远程控制通断电状态及记录
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F161_METR_REMOTE_CTRL
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
enum 
{   
    MT_ON,              // 0 表示断电
    MT_OFF = 17,       // 11H 表示通电
};

typedef struct
{
    sMtFmt15_f  sTime;          // 终端抄表时间
    UINT8       ucMtOnOff;      // 终端通电 状态( 0 - 断电，11H - 通电)
    sMtFmt15_f  sOnTimeLast;    // 最后一次电能表远程控制通电时间
    sMtFmt15_f  sOffTimeLast;   // 最后一次电能表远程控制断电时间
}sMtAfn0cf161_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf161(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F165：电能表开关操作次数及时间
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F165_METR_SWITCH_RECD
 * PN 类型 :  测量点号
 *
{*///

// 用户侧
typedef struct 
{   
    UINT16      usTimes;    // 发生次数
    sMtFmt15    sClock;     // 最后一次发生时间
}sMtOptLog;

typedef struct
{
    sMtFmt15      sTime;           // 终端抄表时间
    sMtOptLog     sOne;            // 对某项操作的次数和最后一次发生时间
    sMtOptLog     sOther;          // 对另一项操作的次数和最后一次发生时间   
    
}sMtAfn0cf165;

// 帧侧
#pragma pack(1) 
typedef struct 
{
    sMtFmt08    sTimes;     // 次数
    sMtFmt15_f  sClock;     // 发生时刻
}sMtOptLog_f;

typedef struct
{
    sMtFmt15_f      sTime;           // 终端抄表时间
    sMtOptLog_f     sOne;            // 对某项操作的次数和最后一次发生时间
    sMtOptLog_f     sOther;          // 对另一项操作的次数和最后一次发生时间   
    
}sMtAfn0cf165_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf165(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F166：电能表参数修改次数及时间
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F166_METR_MODIFY_RECD
 * PN 类型 :  测量点号
 *
{*///

// 用户侧

typedef sMtAfn0cf165 sMtAfn0cf166;
// 帧侧
typedef sMtAfn0cf165_f sMtAfn0cf166_f;

// 转换函数
eMtErr emtTrans_afn0cf166(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F167：电能表购、用电信息
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F167_METR_BUY_USE
 * PN 类型 :  测量点号
 *
{*///

// 用户侧

typedef struct 
{
    sMtFmt15    sTime;      // 终端抄表时间
    UINT16      usBuyTimes;  //  购电次数
    DOUBLE      dCashLeft;  // 剩余金额 dXXXXXX.XXXX
    DOUBLE      dCashTotal; // 累计购电金额 dXXXXXX.XXXX
    DOUBLE      dElecLeft;  // 剩余电量double dXXXXXX.XX
    DOUBLE      dElecOut;   // 透支电量double dXXXXXX.XX
    DOUBLE      dElecTotal; // 累计购电量double dXXXXXX.XX
    DOUBLE      dElecLimit; // 赊欠门限电量double dXXXXXX.XX
    DOUBLE      dElecWarn;  // 报警电量double dXXXXXX.XX
    DOUBLE      dElecFault; // 故障电量double dXXXXXX.XX  
    
}sMtAfn0cf167;

// 帧侧
#pragma pack(1) 
typedef struct
{
    sMtFmt15_f  sTime;      // 终端抄表时间
    sMtFmt08    sBuyTimes;  //  购电次数
    sMtFmt14_f  sCashLeft;  // 剩余金额
    sMtFmt14_f  sCashTotal; // 累计购电金额
    sMtFmt11_f  sElecLeft;  // 剩余电量
    sMtFmt11_f  sElecOut;   // 透支电量
    sMtFmt11_f  sElecTotal; // 累计购电量
    sMtFmt11_f  sElecLimit; // 赊欠门限电量
    sMtFmt11_f  sElecWarn;  // 报警电量
    sMtFmt11_f  sElecFault; // 故障电量
    
}sMtAfn0cf167_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf167(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F168：电能表结算信息
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F168_METR_BALANCE
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct
{
    sMtFmt15_f  sTime;      // 终端抄表时间
    UINT16      usRateM;    // 费率数M
    UINT8       ucErgy[1];  // 电能 
}sMtAfn0cf168_f;

// 转换函数
eMtErr emtTrans_afn0cf168(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F169：集中抄表中继路由信息
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F169_READ_ROUTE
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
#pragma pack(1) 
typedef struct 
{
    UINT8       m;             // 第n 个中继路由级数
    sMtFmt12    sAddress[1];   // 第n 个中继路由的第Mn 个地址
}sMtRoute;

typedef struct
{
    UINT8       ucPort;       // 所属终端通信端口号
    UINT8       ucRoutN;      // 中继路由个数N 
    sMtRoute    sRoute[1];    // 中继路由级数和地址
}sMtAfn0cf169_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0cf169(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F170： 集中抄表电表抄读信息
 * 对应AFN : AFN_0C_ASK1
 * 对应CMD : CMD_AFN_C_F170_READ_METER
 * PN 类型 :  测量点号
 *
{*///

// 用户侧


// 帧侧
typedef struct
{
    UINT8       ucPort;             // 所属终端通信端口号
    UINT8       ucRoutN;            // 中继路由个数N 
    UINT8       ucSignalPhase;      // 载波超痛通信相位
    UINT8       ucSignalQuality;    // 载波信号品质
    UINT8       ucReadFlagLast;     // 最近一次抄表成功/ 失败标志
    sMtFmt01_f  sTimeOkLast;        // 最近一次抄表成功时间
    sMtFmt01_f  sTimeFailLast;      // 最后一次抄表失败时间
    UINT8       ucTimesFailTotal;   // 最近连续失败累计次数
}sMtAfn0cf170_f;

// 转换函数
eMtErr emtTrans_afn0cf170(eMtTrans eTrans,void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 类型定义: 请求2类数据各类型定义
 * 类型说明: 请求2类数据命令下行报文数据单元内容仅有数据时标
 *           数据时标用来定义请求数据的起始时间ts、数据密度m和数据点数n
 * 时标类型: Td_h、Td_c、Td_d、Td_m
 * 
{*///

/*******************************************
 * 数据结构: 小时冻结类数据时标数据格式
 * 对应类型: Td_h
 * 数据说明: 表示上一整点的小时时间，数值范围0～23
{*///
#define MT_TD_H_MIN       (0)   // 小时时标最小值
#define MT_TD_H_MAX       (23)  // 小时时标最大值

// 用户侧
typedef struct
{
    UINT8           ucHH;        // 整点小时时间 (0 ~ 23)
    
}sMtTd_h;
    
// 帧侧
typedef struct
{
     UINT8          ucBCD_0 : 4;  // 小时（个位）
     UINT8          ucBCD_1 : 2;  // 小时（十位）
     UINT8          ucRes   : 2;  // 备用
    
}sMtTd_h_f;

// 转换函数
eMtErr emtTrans_td_h(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 曲线类数据时标数据格式
 * 对应类型: Td_c
 *
{*///

// 用户侧
#define MT_MAX_FREEZE_POINTS        256
typedef struct
{
    sMtYYMMDDhhmm     sT;     // 起始时间ts：分时日月年
    UINT8             ucM;    // 数据冻结密度m
    UINT8             ucN;    // 数据点数n
    
}sMtTd_c;
    
// 帧侧
typedef struct
{
    sMtFmt15_f       sT;      // 起始时间ts：分时日月年
    UINT8            ucM;     // 数据冻结密度m
    UINT8            ucN;     // 数据点数n
    
}sMtTd_c_f;

// 转换函数
eMtErr emtTrans_td_c(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 日冻结类数据时标Td_d
 * 对应类型: Td_d
 * 数据说明: 日冻结数据是指终端在每日日末24点时刻所冻结的数据
 *           其中由抄表得到的数据是日末24点时刻所抄回冻结的电表数据
 *           抄表日冻结数据是指终端按主站设置的终端抄表日日末24点时刻所抄回冻结的电表数据
 *
{*///

// 用户侧
typedef struct
{
    sMtYYMMDD      sT;     // 日月年
    
}sMtTd_d;
    
// 帧侧
typedef struct
{
    sMtFmt20_f     sT;    // 日月年
 
}sMtTd_d_f;

// 转换函数
eMtErr emtTrans_td_d(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: 月冻结类数据时标Td_m
 * 对应类型: Td_m
 * 数据说明: 月冻结数据是指终端在每月月末24点时刻所冻结的数据
 *           其中由抄表得到的数据是月末24点时刻所抄回冻结的电表数据
 *
{*///

// 用户侧
typedef struct
{
    sMtYYMM      sT;     // 月年
    
}sMtTd_m;
    
// 帧侧
typedef struct
{
    sMtYYMM_f    sT;    // 月年
 
}sMtTd_m_f;

// 转换函数
eMtErr emtTrans_td_m(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F1：日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F1_FRTH_POWR_P1P4_D
 * PN 类型 : 测量点号
 *
{*///

 #define MT_M_MIN      (1)                        // 费率数最小值
 #define MT_M_MAX      (12)                       // 费率数最大值

// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                           // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                           // 终端抄表时间
    UINT8         ucM;                             // 费率数M（1≤M≤12）
    double        dFrthHavePowerT;                 // 正向有功总电能示值              (+)XXXXXX.XXXX
    double        dFrthHavePower[MT_M_MAX];        // 费率(1 ~ ucM)正向有功电能示值   (+)XXXXXX.XXXX
    double        dFrthNonePowerT;                 // 正向无功总电能示值              (+)XXXXXX.XX
    double        dFrthNonePower[MT_M_MAX];        // 费率(1 ~ ucM)正向无功电能示值   (+)XXXXXX.XX
    double        dNonePowerP1T;                   // 一象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP1[MT_M_MAX];          // 费率(1 ~ ucM)一象限无功电能示值 (+)XXXXXX.XX
    double        dNonePowerP4T;                   // 四象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP4[MT_M_MAX];          // 费率(1 ~ ucM)四象限无功电能示值 (+)XXXXXX.XX
   
}sMtFrthPowerP1P2D, sMtAfn0dF1;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                           // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                           // 终端抄表时间
    UINT8        ucM;                             // 费率数M（1≤M≤12）
    UINT8        ucPower[1];                      // 
    //sMtFmt14_f   dFrthHavePowerT;                 // 正向有功总电能示值              (+)XXXXXX.XXXX
    //sMtFmt14_f   dFrthHavePower[MT_M_MAX];        // 费率(1 ~ ucM)正向有功电能示值   (+)XXXXXX.XXXX
    //sMtFmt11_f   dFrthNonePowerT;                 // 正向无功总电能示值              (+)XXXXXX.XX
    //sMtFmt11_f   dFrthNonePower[MT_M_MAX];        // 费率(1 ~ ucM)正向无功电能示值   (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP1T;                   // 一象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP1[MT_M_MAX];          // 费率(1 ~ ucM)一象限无功电能示值 (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP4T;                   // 四象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP4[MT_M_MAX];          // 费率(1 ~ ucM)四象限无功电能示值 (+)XXXXXX.XX
  
}sMtFrthPowerP1P2D_f, sMtAfn0dF1_f;

// 转换函数
eMtErr emtTrans_afn0df1(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F2：日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F2_BACK_POWR_P2P3_D
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                           // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                           // 终端抄表时间
    UINT8         ucM;                             // 费率数M（1≤M≤12）
    double        dBackHavePowerT;                 // 正向有功总电能示值              (+)XXXXXX.XXXX
    double        dBackHavePower[MT_M_MAX];        // 费率(1 ~ ucM)反向有功电能示值   (+)XXXXXX.XXXX
    double        dBackNonePowerT;                 // 反向无功总电能示值              (+)XXXXXX.XX
    double        dBackNonePower[MT_M_MAX];        // 费率(1 ~ ucM)反向无功电能示值   (+)XXXXXX.XX
    double        dNonePowerP2T;                   // 二象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP2[MT_M_MAX];          // 费率(1 ~ ucM)二象限无功电能示值 (+)XXXXXX.XX
    double        dNonePowerP3T;                   // 三象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP3[MT_M_MAX];          // 费率(1 ~ ucM)三象限无功电能示值 (+)XXXXXX.XX
   
}sMtFrthPowerP2P3D, sMtAfn0dF2;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                           // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                           // 终端抄表时间
    UINT8        ucM;                             // 费率数M（1≤M≤12）
    UINT8        ucPower[1];                      // 
    //sMtFmt14_f   dBackHavePowerT;                 // 反向有功总电能示值             (+)XXXXXX.XXXX
    //sMtFmt14_f   dBackHavePower[MT_M_MAX];        // 费率(1 ~ ucM)反向有功电能示值  (+)XXXXXX.XXXX
    //sMtFmt11_f   dBackNonePowerT;                 // 反向无功总电能示值             (+)XXXXXX.XX
    //sMtFmt11_f   dBackNonePower[MT_M_MAX];        // 费率(1 ~ ucM)反向无功电能示值  (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP2T;                   // 二象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP2[MT_M_MAX];          // 费率(1 ~ ucM)二象限无功电能示值 (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP3T;                   // 三象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP3[MT_M_MAX];          // 费率(1 ~ ucM)三象限无功电能示值 (+)XXXXXX.XX
  
}sMtFrthPowerP2P3D_f, sMtAfn0dF2_f;

// 转换函数
eMtErr emtTrans_afn0df2(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F3：日冻结正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F3_FRTH_DMND_TIME_D
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                     // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）

    float         fFrthHaveT;                // 正向有功总最大需量                     (+)XX.XXXX
    float         fFrthHave[MT_M_MAX];       // 费率(1 ~ ucM)正向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeFrthHaveT;            // 正向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeFrthHave[MT_M_MAX];   // 费率(1 ~ ucM)正向有功最大需量发生时间  分时日月

    float         fFrthNoneT;                // 反向有功总最大需量                     (+)XX.XXXX
    float         fFrthNone[MT_M_MAX];       // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeFrthNoneT;            // 反向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeFrthNone[MT_M_MAX];   // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月
    
}sMtFrthDemand, sMtAfn0dF3;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                      // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    UINT8        ucDemand[1];                // 为了获得地址
    
    #if 0
    // 大致结构如下, 但是M值不同而变长
    sMtFmt23_f   fFrthHaveT;                 // 正向有功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fFrthHave[MT_M_MAX];        // 费率(1 ~ ucM)正向有功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeFrthHaveT;             // 正向有功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeFrthHave[MT_M_MAX];    // 费率(1 ~ ucM)正向有功最大需量发生时间  分时日月

    sMtFmt23_f   fFrthNoneT;                 // 正向有功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fFrthNone[MT_M_MAX];        // 费率(1 ~ ucM)正向有功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeFrthNoneT;             // 正向有功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeFrthNone[MT_M_MAX];    // 费率(1 ~ ucM)正向有功最大需量发生时间  分时日月
    #endif
    
}sMtFrthDemand_f, sMtAfn0dF3_f;

// 转换函数
eMtErr emtTrans_afn0df3(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F4：日冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F4_BACK_DMND_TIME_D
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                     // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）

    float         fBackHaveT;                // 反向有功总最大需量                     (+)XX.XXXX
    float         fBackHave[MT_M_MAX];       // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeBackHaveT;            // 反向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeBackHave[MT_M_MAX];   // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月

    float         fBackNoneT;                // 反向有功总最大需量                     (+)XX.XXXX
    float         fBackNone[MT_M_MAX];       // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeBackNoneT;            // 反向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeBackNone[MT_M_MAX];   // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月
    
}sMtBackDemand, sMtBackDemandD, sMtAfn0dF4;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                      // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    UINT8        ucDemand[1];                // 为了获得地址
    
    #if 0
    // 大致结构如下, 但是M值不同而变长
    sMtFmt23_f   fBackHaveT;                 // 反向有功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fBackHave[MT_M_MAX];        // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeBackHaveT;             // 反向有功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeBackHave[MT_M_MAX];    // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月

    sMtFmt23_f   fBackNoneT;                 // 反向无功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fBackNone[MT_M_MAX];        // 费率(1 ~ ucM)反向无功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeBackNoneT;             // 反向无功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeBackNone[MT_M_MAX];    // 费率(1 ~ ucM)反向无功最大需量发生时间  分时日月
    #endif
    
}sMtBackDemand_f, sMtBackDemandD_f, sMtAfn0dF4_f;

// 转换函数
eMtErr emtTrans_afn0df4(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F5：日冻结正向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F5_FRTH_HAVE_POWR_D
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                     // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dFrthHaveT;                // 日正向有功总电能量            (+)XXXX.XXXX
    double        dFrthHave[1];              // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthHavePowerD, sMtAfn0dF5;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                      // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dFrthHaveT;                 // 日正向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dFrthHave[1];               // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthHavePowerD_f, sMtAfn0dF5_f;

// 转换函数
eMtErr emtTrans_afn0df5(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F6：日冻结正向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F6_FRTH_NONE_POWR_D
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                     // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dFrthNoneT;                // 日正向有功总电能量            (+)XXXX.XXXX
    double        dFrthNone[1];              // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthNonePowerD, sMtAfn0dF6;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                      // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dFrthNoneT;                 // 日正向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dFrthNone[1];               // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthNonePowerD_f, sMtAfn0dF6_f;

// 转换函数
eMtErr emtTrans_afn0df6(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F7：日冻结反向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F7_BACK_HAVE_POWR_D
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                     // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dBackHaveT;                // 日反向有功总电能量            (+)XXXX.XXXX
    double        dBackHave[1];              // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackHavePowerD, sMtAfn0dF7;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                      // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dBackHaveT;                 // 日反向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dBackHave[1];               // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackHavePowerD_f, sMtAfn0dF7_f;

// 转换函数
eMtErr emtTrans_afn0df7(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F8：日冻结反向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F8_BACK_NONE_POWR_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                     // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dBackNoneT;                // 日反向有功总电能量            (+)XXXX.XXXX
    double        dBackNone[1];              // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackNonePowerD, sMtAfn0dF8;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;                      // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dBackNoneT;                 // 日反向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dBackNone[1];               // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackNonePowerD_f, sMtAfn0dF8_f;

// 转换函数
eMtErr emtTrans_afn0df8(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F9：抄表日冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F9_FRTH_POWR_P1P4_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtFrthPowerP1P2D    sMtFrthPowerP1P2R, sMtAfn0dF9;

// 帧侧
typedef sMtFrthPowerP1P2D_f sMtFrthPowerP1P2R_f, sMtAfn0dF9_f;

// 转换函数
eMtErr emtTrans_afn0df9(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F10：抄表日冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F10_BACK_POWR_P2P3_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtFrthPowerP2P3D    sMtFrthPowerP2P3R, sMtAfn0dF10;

// 帧侧
typedef sMtFrthPowerP2P3D_f    sMtFrthPowerP2P3R_f, sMtAfn0dF10_f;

// 转换函数
eMtErr emtTrans_afn0df10(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F11：抄表日冻结电能表正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F11_FRTH_DMND_TIME_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtFrthDemand    sMtFrthDemandR, sMtAfn0dF11;

// 帧侧
typedef sMtFrthDemand_f  sMtFrthDemandR_f, sMtAfn0dF11_f;

// 转换函数
eMtErr emtTrans_afn0df11(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F12：抄表日冻结电能表反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F12_BACK_DMND_TIME_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtBackDemand    sMtBackDemandR, sMtAfn0dF12;

// 帧侧
typedef sMtBackDemand_f  sMtBackDemandR_f, sMtAfn0dF12_f;

// 转换函数
eMtErr emtTrans_afn0df12(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F17：月冻结正向有/无功电能示值、一/四象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F17_FRTH_POWR_P1P4_M
 * PN 类型 : 测量点号
 *
{*///

// #define MT_M_MIN      (1)                        // 费率数最小值
// #define MT_M_MAX      (12)                       // 费率数最大值

// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                           // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                           // 终端抄表时间
    UINT8         ucM;                             // 费率数M（1≤M≤12）
    double        dFrthHavePowerT;                 // 正向有功总电能示值              (+)XXXXXX.XXXX
    double        dFrthHavePower[MT_M_MAX];        // 费率(1 ~ ucM)正向有功电能示值   (+)XXXXXX.XXXX
    double        dFrthNonePowerT;                 // 正向无功总电能示值              (+)XXXXXX.XX
    double        dFrthNonePower[MT_M_MAX];        // 费率(1 ~ ucM)正向无功电能示值   (+)XXXXXX.XX
    double        dNonePowerP1T;                   // 一象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP1[MT_M_MAX];          // 费率(1 ~ ucM)一象限无功电能示值 (+)XXXXXX.XX
    double        dNonePowerP4T;                   // 四象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP4[MT_M_MAX];          // 费率(1 ~ ucM)四象限无功电能示值 (+)XXXXXX.XX
   
}sMtFrthPowerP1P2M, sMtAfn0dF17;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                           // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                           // 终端抄表时间
    UINT8        ucM;                             // 费率数M（1≤M≤12）
    UINT8        ucPower[1];                      // 点位,用于取地址
    //sMtFmt14_f   dFrthHavePowerT;               // 正向有功总电能示值              (+)XXXXXX.XXXX
    //sMtFmt14_f   dFrthHavePower[MT_M_MAX];      // 费率(1 ~ ucM)正向有功电能示值   (+)XXXXXX.XXXX
    //sMtFmt11_f   dFrthNonePowerT;               // 正向无功总电能示值              (+)XXXXXX.XX
    //sMtFmt11_f   dFrthNonePower[MT_M_MAX];      // 费率(1 ~ ucM)正向无功电能示值   (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP1T;                 // 一象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP1[MT_M_MAX];        // 费率(1 ~ ucM)一象限无功电能示值 (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP4T;                 // 四象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP4[MT_M_MAX];        // 费率(1 ~ ucM)四象限无功电能示值 (+)XXXXXX.XX
  
}sMtFrthPowerP1P2M_f, sMtAfn0dF17_f;

// 转换函数
eMtErr emtTrans_afn0df17(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: FF18：月冻结反向有/无功电能示值、二/三象限无功电能示值（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F18_BACK_POWR_P2P3_M
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                           // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                           // 终端抄表时间
    UINT8         ucM;                             // 费率数M（1≤M≤12）
    double        dBackHavePowerT;                 // 正向有功总电能示值              (+)XXXXXX.XXXX
    double        dBackHavePower[MT_M_MAX];        // 费率(1 ~ ucM)反向有功电能示值   (+)XXXXXX.XXXX
    double        dBackNonePowerT;                 // 正向无功总电能示值              (+)XXXXXX.XX
    double        dBackNonePower[MT_M_MAX];        // 费率(1 ~ ucM)反向无功电能示值   (+)XXXXXX.XX
    double        dNonePowerP2T;                   // 二象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP2[MT_M_MAX];          // 费率(1 ~ ucM)二象限无功电能示值 (+)XXXXXX.XX
    double        dNonePowerP3T;                   // 三象限无功总电能示值            (+)XXXXXX.XX
    double        dNonePowerP3[MT_M_MAX];          // 费率(1 ~ ucM)三象限无功电能示值 (+)XXXXXX.XX
   
}sMtFrthPowerP2P3M, sMtAfn0dF18;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                           // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                           // 终端抄表时间
    UINT8        ucM;                             // 费率数M（1≤M≤12）
    UINT8        ucPower[1];                      // 
    //sMtFmt14_f   dBackHavePowerT;                 // 正向有功总电能示值             (+)XXXXXX.XXXX
    //sMtFmt14_f   dBackHavePower[MT_M_MAX];        // 费率(1 ~ ucM)正向有功电能示值  (+)XXXXXX.XXXX
    //sMtFmt11_f   dBackNonePowerT;                 // 正向无功总电能示值             (+)XXXXXX.XX
    //sMtFmt11_f   dBackNonePower[MT_M_MAX];        // 费率(1 ~ ucM)正向无功电能示值  (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP2T;                   // 二象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP2[MT_M_MAX];          // 费率(1 ~ ucM)二象限无功电能示值 (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP3T;                   // 三象限无功总电能示值            (+)XXXXXX.XX
    //sMtFmt11_f   dNonePowerP3[MT_M_MAX];          // 费率(1 ~ ucM)三象限无功电能示值 (+)XXXXXX.XX
  
}sMtFrthPowerP2P3M_f, sMtAfn0dF18_f;

// 转换函数
eMtErr emtTrans_afn0df18(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F19：正向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F19_FRTH_DMND_TIME_M
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                     // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）

    float         fFrthHaveT;                // 正向有功总最大需量                     (+)XX.XXXX
    float         fFrthHave[MT_M_MAX];       // 费率(1 ~ ucM)正向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeFrthHaveT;            // 正向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeFrthHave[MT_M_MAX];   // 费率(1 ~ ucM)正向有功最大需量发生时间  分时日月

    float         fFrthNoneT;                // 反向有功总最大需量                     (+)XX.XXXX
    float         fFrthNone[MT_M_MAX];       // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeFrthNoneT;            // 反向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeFrthNone[MT_M_MAX];   // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月
    
}sMtFrthDemandM, sMtAfn0dF19;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    UINT8        ucDemand[1];                // 为了获得地址
    
    #if 0
    // 大致结构如下, 但是因M值不同而变长
    sMtFmt23_f   fFrthHaveT;                 // 正向有功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fFrthHave[MT_M_MAX];        // 费率(1 ~ ucM)正向有功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeFrthHaveT;             // 正向有功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeFrthHave[MT_M_MAX];    // 费率(1 ~ ucM)正向有功最大需量发生时间  分时日月

    sMtFmt23_f   fFrthNoneT;                 // 正向有功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fFrthNone[MT_M_MAX];        // 费率(1 ~ ucM)正向有功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeFrthNoneT;             // 正向有功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeFrthNone[MT_M_MAX];    // 费率(1 ~ ucM)正向有功最大需量发生时间  分时日月
    #endif
    
}sMtFrthDemandM_f, sMtAfn0dF19_f;

// 转换函数
eMtErr emtTrans_afn0df19(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F20：月冻结反向有/无功最大需量及发生时间（总、费率1～M，1≤M≤12）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F20_BACK_DMND_TIME_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                     // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）

    float         fBackHaveT;                // 反向有功总最大需量                     (+)XX.XXXX
    float         fBackHave[MT_M_MAX];       // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeBackHaveT;            // 反向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeBackHave[MT_M_MAX];   // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月

    float         fBackNoneT;                // 反向有功总最大需量                     (+)XX.XXXX
    float         fBackNone[MT_M_MAX];       // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtMMDDHHmm   sTimeBackNoneT;            // 反向有功总最大需量发生时间             分时日月
    sMtMMDDHHmm   sTimeBackNone[MT_M_MAX];   // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月
    
}sMtBackDemandM, sMtAfn0dF20;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    UINT8        ucDemand[1];                // 为了获得地址
    
    #if 0
    // 大致结构如下, 但是M值不同而变长
    sMtFmt23_f   fBackHaveT;                 // 反向有功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fBackHave[MT_M_MAX];        // 费率(1 ~ ucM)反向有功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeBackHaveT;             // 反向有功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeBackHave[MT_M_MAX];    // 费率(1 ~ ucM)反向有功最大需量发生时间  分时日月

    sMtFmt23_f   fBackNoneT;                 // 反向无功总最大需量                     (+)XX.XXXX
    sMtFmt23_f   fBackNone[MT_M_MAX];        // 费率(1 ~ ucM)反向无功最大需量          (+)XX.XXXX
    sMtFmt17_f   sTimeBackNoneT;             // 反向无功总最大需量发生时间             分时日月
    sMtFmt17_f   sTimeBackNone[MT_M_MAX];    // 费率(1 ~ ucM)反向无功最大需量发生时间  分时日月
    #endif
    
}sMtBackDemandM_f, sMtAfn0dF20_f;

// 转换函数
eMtErr emtTrans_afn0df20(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F21：月冻结正向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F22_FRTH_NONE_POWR_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                     // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dFrthHaveT;                // 日正向有功总电能量            (+)XXXX.XXXX
    double        dFrthHave[1];              // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthHavePowerM, sMtAfn0dF21;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dFrthHaveT;                 // 日正向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dFrthHave[1];               // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthHavePowerM_f, sMtAfn0dF21_f;

// 转换函数
eMtErr emtTrans_afn0df21(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F22：月冻结正向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F6_FRTH_NONE_POWR_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                     // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dFrthNoneT;                // 日正向有功总电能量            (+)XXXX.XXXX
    double        dFrthNone[1];              // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthNonePowerM, sMtAfn0dF22;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dFrthNoneT;                 // 日正向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dFrthNone[1];               // 日费率(1 ~ ucM)正向有功电能量 (+)XXXX.XXXX
 
}sMtFrthNonePowerM_f, sMtAfn0dF22_f;

// 转换函数
eMtErr emtTrans_afn0df22(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F23：月冻结反向有功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F23_BACK_HAVE_POWR_M
 * PN 类型 : 测量点号
 *
{*///

// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                     // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dBackHaveT;                // 日反向有功总电能量            (+)XXXX.XXXX
    double        dBackHave[1];              // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackHavePowerM, sMtAfn0dF23;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dBackHaveT;                 // 日反向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dBackHave[1];               // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackHavePowerM_f, sMtAfn0dF23_f;

// 转换函数
eMtErr emtTrans_afn0df23(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F24：月冻结反向无功电能量（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F24_BACK_NONE_POWR_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m       sTd_m;                     // 月冻结类数据时标Td_m
    sMtYYMMDDhhmm sTime;                     // 终端抄表时间
    UINT8         ucM;                       // 费率数M（1≤M≤12）
    double        dBackNoneT;                // 日反向有功总电能量            (+)XXXX.XXXX
    double        dBackNone[1];              // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackNonePowerM, sMtAfn0dF24;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt15_f   sTime;                      // 终端抄表时间
    UINT8        ucM;                        // 费率数M（1≤M≤12）
    sMtFmt13_f   dBackNoneT;                 // 日反向有功总电能量            (+)XXXX.XXXX
    sMtFmt13_f   dBackNone[1];               // 日费率(1 ~ ucM)反向有功电能量 (+)XXXX.XXXX
 
}sMtBackNonePowerM_f, sMtAfn0dF24_f;

// 转换函数
eMtErr emtTrans_afn0df24(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F25：日冻结日总及分相最大有功功率及发生时间、有功功率为零时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F25_POWR_FRZE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d     sTd_m;                       // 日冻结类数据时标Td_d
    float       fPowerRateT;                 // 三相总最大有功功率         (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateT;             // 三相总最大有功功率发生时间 (分时日)
    float       fPowerRateA;                 // A相最大有功功率            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateA;             // A相最大有功功率发生时间    (分时日)
    float       fPowerRateB;                 // B相最大有功功率            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateB;             // B相最大有功功率发生时间    (分时日)
    float       fPowerRateC;                 // C相最大有功功率            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateC;             // C相最大有功功率发生时间    (分时日)
    UINT16      usMinutesZeroT;              // 三相总有功功率为零时间     (分钟)
    UINT16      usMinutesZeroA;              // A相有功功率为零时间        (分钟)
    UINT16      usMinutesZeroB;              // B相有功功率为零时间        (分钟)
    UINT16      usMinutesZeroC;              // C相有功功率为零时间        (分钟)
  
}sMtPowerFreezeD, sMtAfn0dF25;
    
// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt23_f   fPowerRateT;                // 三相总最大有功功率 (kW) (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateT;            // 三相总最大有功功率发生时间 (分时日)
    sMtFmt23_f   fPowerRateA;                // A相最大有功功率            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateA;            // A相最大有功功率发生时间    (分时日)
    sMtFmt23_f   fPowerRateB;                // B相最大有功功率            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateB;            // B相最大有功功率发生时间    (分时日)
    sMtFmt23_f   fPowerRateC;                // C相最大有功功率            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateC;            // C相最大有功功率发生时间    (分时日)
    UINT16       usMinutesZeroT;             // 三相总有功功率为零时间     (分钟)
    UINT16       usMinutesZeroA;             // A相有功功率为零时间        (分钟)
    UINT16       usMinutesZeroB;             // B相有功功率为零时间        (分钟)
    UINT16       usMinutesZeroC;             // C相有功功率为零时间        (分钟)
   
}sMtPowerFreezeD_f, sMtAfn0dF25_f;

// 转换函数
eMtErr emtTrans_afn0df25(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F26：日冻结日总及分相有功最大需量及发生时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F26_DMND_FRZE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d     sTd_d;                    // 日冻结类数据时标Td_d
    float       fDemandT;                 // 三相有功总最大需量         (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandT;             // 三相有功总最大需量发生时间 (分时日)
    float       fDemandA;                 // A相有功最大需量            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandA;             // A相有功最大需量发生时间    (分时日)
    float       fDemandB;                 // B相有功最大需量            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandB;             // B相有功最大需量发生时间    (分时日)
    float       fDemandC;                 // C相有功最大需量            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandC;             // C相有功最大需量发生时间    (分时日)
    
}sMtDemandFreezeD, sMtAfn0dF26;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                   // 日冻结类数据时标Td_d
    sMtFmt23_f   fDemandT;                // 三相有功总最大需量 (kW) (+)XX.XXXX
    sMtFmt18_f   sTimeDemandT;            // 三相有功总最大需量发生时间 (分时日)
    sMtFmt23_f   fDemandA;                // A相有功最大需量            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimeDemandA;            // A相有功最大需量发生时间    (分时日)
    sMtFmt23_f   fDemandB;                // B相有功最大需量            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimeDemandB;            // B相有功最大需量发生时间    (分时日)
    sMtFmt23_f   fDemandC;                // C相有功最大需量            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimeDemandC;            // C相有功最大需量发生时间    (分时日)
 
}sMtDemandFreezeD_f, sMtAfn0dF26_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df26(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F27：日冻结日电压统计数据
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F27_VOLT_FRZE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    UINT16 usOverUpUp;                  // 电压越上上限日累计时间 (分钟)
    UINT16 usOverDownDown;              // 电压越下下限日累计时间 (分钟)
    UINT16 usOverUp;                    // 电压越上限日累计时间   (分钟)
    UINT16 usOverDown;                  // 电压越下限日累计时间   (分钟)
    UINT16 usOk;                        // 电压合格日累计时间     (分钟)

}sMtVoltFrzeTime;

typedef struct
{
    float       fMin;                   // 电压最小值           (+)XXX.X  (V)   
    sMtDDHHmm   sTimeMin;               // 电压最小值发生时间   (分时日)
    float       fMax;                   // 电压最大值           (+)XXX.X  (V)
    sMtDDHHmm   sTimeMax;               // 电压最大值发生时间   (分时日)

}sMtVoltLimit;

typedef struct
{
    sMtTd_d          sTd_d;              // 日冻结类数据时标Td_d
    sMtVoltFrzeTime  sTimeA;             // A相
    sMtVoltFrzeTime  sTimeB;             // B相
    sMtVoltFrzeTime  sTimeC;             // C相
    sMtVoltLimit     sVoltA;             // A相
    sMtVoltLimit     sVoltB;             // B相
    sMtVoltLimit     sVoltC;             // C相
    float            fAveA;              // A相平均电压 (+)XXX.X  (V)
    float            fAveB;              // B相平均电压 (+)XXX.X  (V)
    float            fAveC;              // C相平均电压 (+)XXX.X  (V)

}sMtVoltFreezeD, sMtAfn0dF27;
    
// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT16 usOverUpUp;                  // 电压越上上限日累计时间 (分钟)
    UINT16 usOverDownDown;              // 电压越下下限日累计时间 (分钟)
    UINT16 usOverUp;                    // 电压越上限日累计时间   (分钟)
    UINT16 usOverDown;                  // 电压越下限日累计时间   (分钟)
    UINT16 usOk;                        // 电压合格日累计时间     (分钟)

}sMtVoltFrzeTime_f;

typedef struct
{
    sMtFmt07_f    fMin;                   // 电压最小值         (+)XXX.X  (V)
    sMtFmt18_f    sTimeMin;               // 电压最小值发生时间 (分时日)
    sMtFmt07_f    fMax;                   // 电压最大值         (+)XXX.X  (V)
    sMtFmt18_f    sTimeMax;               // 电压最大值发生时间 (分时日)
    
}sMtVoltLimit_f;

typedef struct
{   
    sMtTd_d_f          sTd_d;              // 日冻结类数据时标Td_d
    sMtVoltFrzeTime_f  sTimeA;             // A相
    sMtVoltFrzeTime_f  sTimeB;             // B相
    sMtVoltFrzeTime_f  sTimeC;             // C相
    sMtVoltLimit_f     sVoltA;             // A相
    sMtVoltLimit_f     sVoltB;             // B相
    sMtVoltLimit_f     sVoltC;             // C相
    sMtFmt07_f         fAveA;              // A相平均电压      (+)XXX.X  (V)
    sMtFmt07_f         fAveB;              // B相平均电压      (+)XXX.X  (V)
    sMtFmt07_f         fAveC;              // C相平均电压      (+)XXX.X  (V)
    
}sMtVoltFreezeD_f, sMtAfn0dF27_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df27(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F28：日冻结日不平衡度越限累计时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F28_UBLN_OVER_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d     sTd_d;                    // 日冻结类数据时标Td_d
    UINT16      usCalElec;                // 电流不平衡度越限日累计时间 (min)
    UINT16      usCalVolt;                // 电压不平衡度越限日累计时间 (min)
    float       fMaxElec;                 // 电流不平衡最大值           (+)XXX.X (%)
    sMtDDHHmm   sTimeMaxElec;             // 电流不平衡最大值发生时间   (分时日)
    float       fMaxVolt;                 // 电压不平衡最大值           (+)XXX.X (%)
    sMtDDHHmm   sTimeMaxVolt;             // 电压不平衡最大值发生时间   (分时日)
 
}sMtUblnOverD, sMtAfn0dF28;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                    // 日冻结类数据时标Td_d
    UINT16       usCalElec;                // 电流不平衡度越限日累计时间 (min)
    UINT16       usCalVolt;                // 电压不平衡度越限日累计时间 (min)
    sMtFmt05_f   fMaxElec;                 // 电流不平衡最大值
    sMtFmt18_f   sTimeMaxElec;             // 电流不平衡最大值发生时间
    sMtFmt05_f   fMaxVolt;                 // 电压不平衡最大值
    sMtFmt18_f   sTimeMaxVolt;             // 电压不平衡最大值发生时间
}sMtUblnOverD_f, sMtAfn0dF28_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df28(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F29：日冻结日电流越限数据
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F29_ELEC_OVER_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d      sTd_d;                    // 日冻结类数据时标Td_d
    UINT16       usOverUpUpA;              // A相电流越上上限累计时间
    UINT16       usOverUpA;                // A相电流越上限累计时间
    UINT16       usOverUpUpB;              // B相电流越上上限累计时间
    UINT16       usOverUpB;                // B相电流越上限累计时间
    UINT16       usOverUpUpC;              // C相电流越上上限累计时间
    UINT16       usOverUpC;                // C相电流越上限累计时间
    UINT16       usOverUpZ;                // 零序电流越上限累计时间
    float        fMaxA;                    // A相电流最大值         (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxA;               // A相电流最大值发生时间
    float        fMaxB;                    // B相电流最大值         (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxB;               // B相电流最大值发生时间
    float        fMaxC;                    // C相电流最大值         (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxC;               // C相电流最大值发生时间
    float        fMaxZ;                    // 零序电流最大值        (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxZ;               // 零序电流最大值发生时间
    
}sMtElecOverD, sMtAfn0dF29;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                    // 日冻结类数据时标Td_d
    UINT16       usCalElec;                // 电流不平衡度越限日累计时间 (min)
    UINT16       usOverUpUpA;              // A相电流越上上限累计时间
    UINT16       usOverUpA;                // A相电流越上限累计时间
    UINT16       usOverUpUpB;              // B相电流越上上限累计时间
    UINT16       usOverUpB;                // B相电流越上限累计时间
    UINT16       usOverUpUpC;              // C相电流越上上限累计时间
    UINT16       usOverUpC;                // C相电流越上限累计时间
    UINT16       usOverUpZ;                // 零序电流越上限累计时间
    sMtFmt25_f   fMaxA;                    // A相电流最大值         (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxA;               // A相电流最大值发生时间
    sMtFmt25_f   fMaxB;                    // B相电流最大值         (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxB;               // B相电流最大值发生时间
    sMtFmt25_f   fMaxC;                    // C相电流最大值         (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxC;               // C相电流最大值发生时间
    sMtFmt25_f   fMaxZ;                    // 零序电流最大值        (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxZ;               // 零序电流最大值发生时间
    
}sMtElecOverD_f, sMtAfn0dF29_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df29(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F30：日冻结日视在功率越限累计时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F30_POWR_RATE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d      sTd_d;                   // 日冻结类数据时标Td_d
    UINT16       usOverUpUp;              // 视在功率越上上限累计时间
    UINT16       usOverUp;                // 视在功率越上限累计时间
   
    
}sMtSPowerRateD, sMtAfn0dF30;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                   // 日冻结类数据时标Td_d
    UINT16       usOverUpUp;              // 视在功率越上上限累计时间
    UINT16       usOverUp;                // 视在功率越上限累计时间
  
}sMtSPowerRateD_f, sMtAfn0dF30_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df30(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F31：日负载率统计
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F31_LOAD_RATE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d      sTd_d;                   // 日冻结类数据时标Td_d
    float        fMax;                    // 负载率最大值         (+/-)XXX.X (%) 
    sMtDDHHmm    sTimeMax;                // 负载率最大值发生时间 (分时日)   
    float        fMin;                    // 负载率最大值         (+/-)XXX.X (%) 
    sMtDDHHmm    sTimeMin;                // 负载率最小值发生时间 (分时日)
    
}sMtLoadRateD, sMtAfn0dF31;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                   // 日冻结类数据时标Td_d
    sMtFmt05_f   fMax;                    // 负载率最大值         (%) 
    sMtFmt18_f   sTimeMax;                // 负载率最大值发生时间 (分时日)   
    sMtFmt05_f   fMin;                    // 负载率最大值         (%)
    sMtFmt18_f   sTimeMin;                // 负载率最小值发生时间 (分时日)
  
}sMtLoadRateD_f, sMtAfn0dF31_f;

#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df31(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F32：日冻结电能表断相数据
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F32_METR_DROP_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d       sTd_d;                   // 日冻结类数据时标Td_d
    sMtYYMMDDhhmm sTime;                   // 终端抄表时间
    UINT16        usDropT;                 // 总断相次数    (+)XXXX
    UINT16        usDropA;                 // A相断相次数   (+)XXXX
    UINT16        usDropB;                 // B相断相次数   (+)XXXX
    UINT16        usDropC;                 // C相断相次数   (+)XXXX
    UINT32        ulMinutesT;              // 断相累计时间  (+)XXXXXX (分钟)
    UINT32        ulMinutesA;              // A断相累计时间 (+)XXXXXX (分钟)
    UINT32        ulMinutesB;              // B断相累计时间 (+)XXXXXX (分钟)
    UINT32        ulMinutesC;              // C断相累计时间 (+)XXXXXX (分钟)
    sMtMMDDHHmm   sTimeStart;              // 最近一次断相起始时刻 (分时日月)
    sMtMMDDHHmm   sTimeStartA;             // A相最近断相起始时刻  (分时日月)
    sMtMMDDHHmm   sTimeStartB;             // B相最近断相起始时刻  (分时日月)
    sMtMMDDHHmm   sTimeStartC;             // C相最近断相起始时刻  (分时日月)
    sMtMMDDHHmm   sTimeEnd;                // 最近一次断相结束时刻 (分时日月)
    sMtMMDDHHmm   sTimeEndA;               // A相最近断相结束时刻  (分时日月)
    sMtMMDDHHmm   sTimeEndB;               // B相最近断相结束时刻  (分时日月)
    sMtMMDDHHmm   sTimeEndC;               // C相最近断相结束时刻  (分时日月)
    
}sMtMeterDropD, sMtAfn0dF32;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                   // 日冻结类数据时标Td_d
    sMtFmt15_f   sTime;                   // 终端抄表时间
    sMtFmt08_f   usDropT;                 // 总断相次数    (+)XXXX
    sMtFmt08_f   usDropA;                 // A相断相次数   (+)XXXX
    sMtFmt08_f   usDropB;                 // B相断相次数   (+)XXXX
    sMtFmt08_f   usDropC;                 // C相断相次数   (+)XXXX
    sMtFmt10_f   ulMinutesT;              // 断相累计时间  (+)XXXXXX (分钟)
    sMtFmt10_f   ulMinutesA;              // A断相累计时间 (+)XXXXXX (分钟)
    sMtFmt10_f   ulMinutesB;              // B断相累计时间 (+)XXXXXX (分钟)
    sMtFmt10_f   ulMinutesC;              // C断相累计时间 (+)XXXXXX (分钟)
    sMtFmt17_f   sTimeStart;              // 最近一次断相起始时刻 (分时日月)
    sMtFmt17_f   sTimeStartA;             // A相最近断相起始时刻  (分时日月)
    sMtFmt17_f   sTimeStartB;             // B相最近断相起始时刻  (分时日月)
    sMtFmt17_f   sTimeStartC;             // C相最近断相起始时刻  (分时日月)
    sMtFmt17_f   sTimeEnd;                // 最近一次断相结束时刻 (分时日月)
    sMtFmt17_f   sTimeEndA;               // A相最近断相结束时刻  (分时日月)
    sMtFmt17_f   sTimeEndB;               // B相最近断相结束时刻  (分时日月)
    sMtFmt17_f   sTimeEndC;               // C相最近断相结束时刻  (分时日月)

}sMtMeterDropD_f, sMtAfn0dF32_f;

#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df32(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F33：月冻结月总及分相最大有功功率及发生时间、有功功率为零时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F33_POWR_FRZE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m     sTd_m;                       // 月冻结类数据时标Td_m
    float       fPowerRateT;                 // 三相总最大有功功率         (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateT;             // 三相总最大有功功率发生时间 (分时日)
    float       fPowerRateA;                 // A相最大有功功率            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateA;             // A相最大有功功率发生时间    (分时日)
    float       fPowerRateB;                 // B相最大有功功率            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateB;             // B相最大有功功率发生时间    (分时日)
    float       fPowerRateC;                 // C相最大有功功率            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimePowerRateC;             // C相最大有功功率发生时间    (分时日)
    UINT16      usMinutesZeroT;              // 三相总有功功率为零时间     (分钟)
    UINT16      usMinutesZeroA;              // A相有功功率为零时间        (分钟)
    UINT16      usMinutesZeroB;              // B相有功功率为零时间        (分钟)
    UINT16      usMinutesZeroC;              // C相有功功率为零时间        (分钟)
  
}sMtPowerFreezeM, sMtAfn0dF33;
    
// 帧侧
typedef struct
{   
    sMtTd_m_f    sTd_m;                      // 月冻结类数据时标Td_m
    sMtFmt23_f   fPowerRateT;                // 三相总最大有功功率 (kW) (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateT;            // 三相总最大有功功率发生时间 (分时日)
    sMtFmt23_f   fPowerRateA;                // A相最大有功功率            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateA;            // A相最大有功功率发生时间    (分时日)
    sMtFmt23_f   fPowerRateB;                // B相最大有功功率            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateB;            // B相最大有功功率发生时间    (分时日)
    sMtFmt23_f   fPowerRateC;                // C相最大有功功率            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimePowerRateC;            // C相最大有功功率发生时间    (分时日)
    UINT16       usMinutesZeroT;             // 三相总有功功率为零时间     (分钟)
    UINT16       usMinutesZeroA;             // A相有功功率为零时间        (分钟)
    UINT16       usMinutesZeroB;             // B相有功功率为零时间        (分钟)
    UINT16       usMinutesZeroC;             // C相有功功率为零时间        (分钟)
   
}sMtPowerFreezeM_f, sMtAfn0dF33_f;

// 转换函数
eMtErr emtTrans_afn0df33(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F34：月冻结月总及分相有功最大需量及发生时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F34_DMND_FRZE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m     sTd_m;                    // 月冻结类数据时标Td_m
    float       fDemandT;                 // 三相有功总最大需量         (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandT;             // 三相有功总最大需量发生时间 (分时日)
    float       fDemandA;                 // A相有功最大需量            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandA;             // A相有功最大需量发生时间    (分时日)
    float       fDemandB;                 // B相有功最大需量            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandB;             // B相有功最大需量发生时间    (分时日)
    float       fDemandC;                 // C相有功最大需量            (kW)      (+)XX.XXXX
    sMtDDHHmm   sTimeDemandC;             // C相有功最大需量发生时间    (分时日)
    
}sMtDemandFreezeM, sMtAfn0dF34;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_m_f    sTd_m;                   // 月冻结类数据时标Td_m
    sMtFmt23_f   fDemandT;                // 三相有功总最大需量 (kW) (+)XX.XXXX
    sMtFmt18_f   sTimeDemandT;            // 三相有功总最大需量发生时间 (分时日)
    sMtFmt23_f   fDemandA;                // A相有功最大需量            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimeDemandA;            // A相有功最大需量发生时间    (分时日)
    sMtFmt23_f   fDemandB;                // B相有功最大需量            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimeDemandB;            // B相有功最大需量发生时间    (分时日)
    sMtFmt23_f   fDemandC;                // C相有功最大需量            (kW)      (+)XX.XXXX
    sMtFmt18_f   sTimeDemandC;            // C相有功最大需量发生时间    (分时日)
 
}sMtDemandFreezeM_f, sMtAfn0dF34_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df34(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F35：月冻结月电压统计数据
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F35_VOLT_FRZE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
/*
typedef struct
{
    UINT16 usOverUpUp;                  // 电压越上上限日累计时间 (分钟)
    UINT16 usOverDownDown;              // 电压越下下限日累计时间 (分钟)
    UINT16 usOverUp;                    // 电压越上限日累计时间   (分钟)
    UINT16 usOverDown;                  // 电压越下限日累计时间   (分钟)
    UINT16 usOk;                        // 电压合格日累计时间     (分钟)

}sMtVoltFrzeTime;

typedef struct
{
    float       fMin;                   // 电压最小值           (+)XXX.X  (V)   
    sMtDDHHmm   sTimeMin;               // 电压最小值发生时间   (分时日)
    float       fMax;                   // 电压最大值           (+)XXX.X  (V)
    sMtDDHHmm   sTimeMax;               // 电压最大值发生时间   (分时日)

}sMtVoltLimit;
*/
typedef struct
{
    sMtTd_m          sTd_m;              // 月冻结类数据时标Td_m
    sMtVoltFrzeTime  sTimeA;             // A相
    sMtVoltFrzeTime  sTimeB;             // B相
    sMtVoltFrzeTime  sTimeC;             // C相
    sMtVoltLimit     sVoltA;             // A相
    sMtVoltLimit     sVoltB;             // B相
    sMtVoltLimit     sVoltC;             // C相
    float            fAveA;              // A相平均电压 (+)XXX.X  (V)
    float            fAveB;              // B相平均电压 (+)XXX.X  (V)
    float            fAveC;              // C相平均电压 (+)XXX.X  (V)

}sMtVoltFreezeM, sMtAfn0dF35;
    
// 帧侧
#pragma pack(1) 
/*
typedef struct
{
    UINT16 usOverUpUp;                  // 电压越上上限日累计时间 (分钟)
    UINT16 usOverDownDown;              // 电压越下下限日累计时间 (分钟)
    UINT16 usOverUp;                    // 电压越上限日累计时间   (分钟)
    UINT16 usOverDown;                  // 电压越下限日累计时间   (分钟)
    UINT16 usOk;                        // 电压合格日累计时间     (分钟)

}sMtVoltFrzeTime_f;

typedef struct
{
    sMtFmt07_f    fMin;                   // 电压最小值         (+)XXX.X  (V)
    sMtFmt18_f    sTimeMin;               // 电压最小值发生时间 (分时日)
    sMtFmt07_f    fMax;                   // 电压最大值         (+)XXX.X  (V)
    sMtFmt18_f    sTimeMax;               // 电压最大值发生时间 (分时日)
    
}sMtVoltLimit_f;

*/
typedef struct
{   
    sMtTd_m_f          sTd_m;              // 月冻结类数据时标Td_m
    sMtVoltFrzeTime_f  sTimeA;             // A相
    sMtVoltFrzeTime_f  sTimeB;             // B相
    sMtVoltFrzeTime_f  sTimeC;             // C相
    sMtVoltLimit_f     sVoltA;             // A相
    sMtVoltLimit_f     sVoltB;             // B相
    sMtVoltLimit_f     sVoltC;             // C相
    sMtFmt07_f         fAveA;              // A相平均电压      (+)XXX.X  (V)
    sMtFmt07_f         fAveB;              // B相平均电压      (+)XXX.X  (V)
    sMtFmt07_f         fAveC;              // C相平均电压      (+)XXX.X  (V)
    
}sMtVoltFreezeM_f, sMtAfn0dF35_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df35(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F36：月冻结月不平衡度越限累计时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F36_UBLN_OVER_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m     sTd_m;                    // 月冻结类数据时标Td_m
    UINT16      usCalElec;                // 电流不平衡度越限日累计时间 (min)
    UINT16      usCalVolt;                // 电压不平衡度越限日累计时间 (min)
    float       fMaxElec;                 // 电流不平衡最大值           (+)XXX.X (%)
    sMtMMDDHHmm sTimeMaxElec;             // 电流不平衡最大值发生时间   (分时日月)
    float       fMaxVolt;                 // 电压不平衡最大值           (+)XXX.X (%)
    sMtMMDDHHmm sTimeMaxVolt;             // 电压不平衡最大值发生时间   (分时日月)
 
}sMtUblnOverM, sMtAfn0dF36;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_m_f    sTd_m;                    // 月冻结类数据时标Td_m
    UINT16       usCalElec;                // 电流不平衡度越限日累计时间 (min)
    UINT16       usCalVolt;                // 电压不平衡度越限日累计时间 (min)
    sMtFmt05_f   fMaxElec;                 // 电流不平衡最大值
    sMtFmt17_f   sTimeMaxElec;             // 电流不平衡最大值发生时间
    sMtFmt05_f   fMaxVolt;                 // 电压不平衡最大值
    sMtFmt17_f   sTimeMaxVolt;             // 电压不平衡最大值发生时间
    
}sMtUblnOverM_f, sMtAfn0dF36_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df36(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F37：月冻结月电流越限数据
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F37_ELEC_OVER_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m      sTd_m;                    // 月冻结类数据时标Td_m
    UINT16       usOverUpUpA;              // A相电流越上上限累计时间
    UINT16       usOverUpA;                // A相电流越上限累计时间
    UINT16       usOverUpUpB;              // B相电流越上上限累计时间
    UINT16       usOverUpB;                // B相电流越上限累计时间
    UINT16       usOverUpUpC;              // C相电流越上上限累计时间
    UINT16       usOverUpC;                // C相电流越上限累计时间
    UINT16       usOverUpZ;                // 零序电流越上限累计时间
    float        fMaxA;                    // A相电流最大值         (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxA;               // A相电流最大值发生时间
    float        fMaxB;                    // B相电流最大值         (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxB;               // B相电流最大值发生时间
    float        fMaxC;                    // C相电流最大值         (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxC;               // C相电流最大值发生时间
    float        fMaxZ;                    // 零序电流最大值        (+)XXX.XXX (A)
    sMtDDHHmm    sTimefMaxZ;               // 零序电流最大值发生时间
    
}sMtElecOverM, sMtAfn0dF37;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_m_f    sTd_m;                    // 月冻结类数据时标Td_m
    UINT16       usCalElec;                // 电流不平衡度越限日累计时间 (min)
    UINT16       usOverUpUpA;              // A相电流越上上限累计时间
    UINT16       usOverUpA;                // A相电流越上限累计时间
    UINT16       usOverUpUpB;              // B相电流越上上限累计时间
    UINT16       usOverUpB;                // B相电流越上限累计时间
    UINT16       usOverUpUpC;              // C相电流越上上限累计时间
    UINT16       usOverUpC;                // C相电流越上限累计时间
    UINT16       usOverUpZ;                // 零序电流越上限累计时间
    sMtFmt25_f   fMaxA;                    // A相电流最大值         (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxA;               // A相电流最大值发生时间
    sMtFmt25_f   fMaxB;                    // B相电流最大值         (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxB;               // B相电流最大值发生时间
    sMtFmt25_f   fMaxC;                    // C相电流最大值         (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxC;               // C相电流最大值发生时间
    sMtFmt25_f   fMaxZ;                    // 零序电流最大值        (+)XXX.XXX (A)
    sMtFmt18_f   sTimefMaxZ;               // 零序电流最大值发生时间
    
}sMtElecOverM_f, sMtAfn0dF37_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df37(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F38：月冻结月视在功率越限累计时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F38_POWR_RATE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m      sTd_m;                   // 月冻结类数据时标Td_m
    UINT16       usOverUpUp;              // 视在功率越上上限累计时间
    UINT16       usOverUp;                // 视在功率越上限累计时间
    
}sMtSPowerRateM, sMtAfn0dF38;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_m_f    sTd_m;                   // 月冻结类数据时标Td_m
    UINT16       usOverUpUp;              // 视在功率越上上限累计时间
    UINT16       usOverUp;                // 视在功率越上限累计时间
  
}sMtSPowerRateM_f, sMtAfn0dF38_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df38(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F39：月负载率统计
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F39_LOAD_RATE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m      sTd_m;                   // 月冻结类数据时标Td_m
    float        fMax;                    // 负载率最大值         (+/-)XXX.X (%) 
    sMtDDHHmm    sTimeMax;                // 负载率最大值发生时间 (分时日)   
    float        fMin;                    // 负载率最大值         (+/-)XXX.X (%) 
    sMtDDHHmm    sTimeMin;                // 负载率最小值发生时间 (分时日)
    
}sMtLoadRateM, sMtAfn0dF39;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_m_f    sTd_m;                   // 月冻结类数据时标Td_m
    sMtFmt05_f   fMax;                    // 负载率最大值         (%) 
    sMtFmt18_f   sTimeMax;                // 负载率最大值发生时间 (分时日)   
    sMtFmt05_f   fMin;                    // 负载率最大值         (%)
    sMtFmt18_f   sTimeMin;                // 负载率最小值发生时间 (分时日)
  
}sMtLoadRateM_f, sMtAfn0dF39_f;

#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df39(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F41：日冻结电容器累计投入时间和次数
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F41_CAPA_TIME_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
#define MT_CAPA_TEAM       (9)            // 投入电容器组数
typedef struct
{
    sMtTd_d      sTd_d;                   // 日冻结类数据时标Td_d
    UINT32       ulSeconds[MT_CAPA_TEAM]; // 第(1~9)组电容器累计投入时间 (秒)
    UINT32       ulTimes[MT_CAPA_TEAM];   // 第(1~9)组电容器累计投入次数 (次)
    
}sMtCapaTimeD, sMtAfn0dF41;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                   // 日冻结类数据时标Td_d
    UINT32       ulSeconds[MT_CAPA_TEAM]; // 第(1~9)组电容器累计投入时间 (秒)
    UINT32       ulTimes[MT_CAPA_TEAM];   // 第(1~9)组电容器累计投入次数 (次)
    
}sMtCapaTimeD_f, sMtAfn0dF41_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df41(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F42：日冻结日、月电容器累计补偿的无功电能量
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F42_CAPA_NONE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d      sTd_d;                   // 日冻结类数据时标Td_d
    double       dPowerD;                 // 日补偿的无功电能量   (+)XXXX.XXXX (kvarh)
    double       dPowerM;                 // 月补偿的无功电能量   (+)XXXX.XXXX (kvarh)
    
}sMtCapaNoneD, sMtAfn0dF42;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                   // 日冻结类数据时标Td_d
    sMtFmt13_f   dPowerD;                 // 日补偿的无功电能量
    sMtFmt13_f   dPowerM;                 // 月补偿的无功电能量
 
}sMtCapaNoneD_f, sMtAfn0dF42_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df42(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F43：日冻结日功率因数区段累计时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F43_FACT_TIME_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_d      sTd_d;                   // 日冻结类数据时标Td_d
    UINT16       usTime_1;                // 区段1累计时间（功率因数＜定值1）(min)
    UINT16       usTime_2;                // 区段2累计时间（功率因数＜定值1）(min)
    UINT16       usTime_3;                // 区段3累计时间（功率因数＜定值1）(min)
    
}sMtFactTimeD, sMtAfn0dF43;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_d_f    sTd_d;                   // 日冻结类数据时标Td_d
    UINT16       usTime_1;                // 区段1累计时间（功率因数＜定值1）(min)
    UINT16       usTime_2;                // 区段2累计时间（功率因数＜定值1）(min)
    UINT16       usTime_3;                // 区段3累计时间（功率因数＜定值1）(min)
 
}sMtFactTimeD_f, sMtAfn0dF43_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df43(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F44：月冻结月功率因数区段累计时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F44_FACT_TIME_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m      sTd_m;                   // 月冻结类数据时标Td_m
    UINT16       usTime_1;                // 区段1累计时间（功率因数＜定值1）(min)
    UINT16       usTime_2;                // 区段2累计时间（功率因数＜定值1）(min)
    UINT16       usTime_3;                // 区段3累计时间（功率因数＜定值1）(min)
    
}sMtFactTimeM, sMtAfn0dF44;
    
// 帧侧
#pragma pack(1) 
typedef struct
{   
    sMtTd_m_f    sTd_m;                   // 月冻结类数据时标Td_m
    UINT16       usTime_1;                // 区段1累计时间（功率因数＜定值1）(min)
    UINT16       usTime_2;                // 区段2累计时间（功率因数＜定值1）(min)
    UINT16       usTime_3;                // 区段3累计时间（功率因数＜定值1）(min)
 
}sMtFactTimeM_f, sMtAfn0dF44_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0df44(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F97： 正向有功总电能量 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F97_CURVE_FRTH_HAVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_c     sTd_c;          // 曲线类数据时标Td_c
    DOUBLE      dEnergy[MT_MAX_FREEZE_POINTS];   // 有功电能量(XXXX.XXXX)
}sMtAfn0dF97;
    
// 帧侧
typedef struct
{   
    sMtTd_c_f    sTd_c;         // 曲线类数据时标Td_c
    sMtFmt13_f   sEnergy[0];   // 电能量
}sMtAfn0dF97_f;

// 转换函数
eMtErr emtTrans_afn0df97(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F98： 正向无功总电能量 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F98_CURVE_FRTH_NONE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF97 sMtAfn0dF98;
    
// 帧侧
typedef sMtAfn0dF97_f sMtAfn0dF98_f ;

// 转换函数
eMtErr emtTrans_afn0df98(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F99： 反向有功总电能量 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F99_CURVE_BACK_HAVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF97 sMtAfn0dF99;
    
// 帧侧
typedef sMtAfn0dF97_f sMtAfn0dF99_f ;

// 转换函数
eMtErr emtTrans_afn0df99(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F100：反向无功总电能量 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F100_CURVE_BACK_HAVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF97 sMtAfn0dF100;
    
// 帧侧
typedef sMtAfn0dF97_f sMtAfn0dF100_f ;

// 转换函数
eMtErr emtTrans_afn0df100(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F101： 正向有功总电能示值 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F101_CURVE_FRTH_HAVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_c     sTd_c;          // 曲线类数据时标Td_c
    DOUBLE      dValue[MT_MAX_FREEZE_POINTS];      // 电能示值 dXXXXXX.XX
}sMtAfn0dF101;
    
// 帧侧
typedef struct
{   
    sMtTd_c_f    sTd_c;         // 曲线类数据时标Td_c
    sMtFmt11_f   sValue[0];    // 电能示值
}sMtAfn0dF101_f;

// 转换函数
eMtErr emtTrans_afn0df101(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F102：正向无功总电能示值 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F102_CURVE_FRTH_NONE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF101 sMtAfn0dF102;
    
// 帧侧
typedef sMtAfn0dF101_f sMtAfn0dF102_f ;

// 转换函数
eMtErr emtTrans_afn0df102(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F103：反向有功总电能示值 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F103_CURVE_BACK_HAVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF101 sMtAfn0dF103;
    
// 帧侧
typedef sMtAfn0dF101_f sMtAfn0dF103_f ;

// 转换函数
eMtErr emtTrans_afn0df103(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F104： 反向无功总电能示值 曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F104_CURVE_BACK_NONE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF101 sMtAfn0dF104;
    
// 帧侧
typedef sMtAfn0dF101_f sMtAfn0dF104_f ;

// 转换函数
eMtErr emtTrans_afn0df104(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F105： 总功率因数
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F105_CURVE_FACTOR_T
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_c     sTd_c;                              // 曲线类数据时标Td_c
    float       fFactor[MT_MAX_FREEZE_POINTS];      // sXXX.X
}sMtAfn0dF105;
    
// 帧侧
typedef struct
{   
    sMtTd_c_f    sTd_c;         // 曲线类数据时标Td_c
    sMtFmt05     sFactor[0];    // 电能示值
}sMtAfn0dF105_f;

// 转换函数
eMtErr emtTrans_afn0df105(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F106： A相功率因数
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F106_CURVE_FACTOR_A
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF105 sMtAfn0dF106;
    
// 帧侧
typedef sMtAfn0dF105_f sMtAfn0dF106_f ;

// 转换函数
eMtErr emtTrans_afn0df106(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F107：B相功率因数
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F107_CURVE_FACTOR_B
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF105 sMtAfn0dF107;
    
// 帧侧
typedef sMtAfn0dF105_f sMtAfn0dF107_f ;

// 转换函数
eMtErr emtTrans_afn0df107(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F108：C相功率因数
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F108_CURVE_FACTOR_C
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF105 sMtAfn0dF108;
    
// 帧侧
typedef sMtAfn0dF105_f sMtAfn0dF108_f ;

// 转换函数
eMtErr emtTrans_afn0df108(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F109： 电压相位角曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F109_ANGLE_CURVE_VOLT
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct 
{
    float    fPhaseA;     // 相位角A sXXX.X
    float    fPhaseB;     // 相位角B sXXX.X
    float    fPhaseC;     // 相位角C sXXX.X
}sMtPhase;

typedef struct
{
    sMtTd_c     sTd_c;          // 曲线类数据时标Td_c
    sMtPhase    sPhase[0];      // 计算时重新考虑数组大小
}sMtAfn0dF109;
    
// 帧侧
typedef struct 
{
    sMtFmt05    sPhaseA;     // 相位角A
    sMtFmt05    sPhaseB;     // 相位角B
    sMtFmt05    sPhaseC;     // 相位角C
}sMtPhase_f;

typedef struct
{   
    sMtTd_c_f    sTd_c;         // 曲线类数据时标Td_c
    sMtPhase_f   sPhase[0];     // 相位角
}sMtAfn0dF109_f;

// 转换函数
eMtErr emtTrans_afn0df109(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F110： 电流相位角曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F110_ANGLE_CURVE_ELEC
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_c     sTd_c;          // 曲线类数据时标Td_c
    DOUBLE      dValue[0];      // 计算时重新考虑数组大小
}sMtAfn0dF110;
    
// 帧侧
typedef struct 
{
    sMtFmt05    sIa;     // Ia 相位角
    sMtFmt05    sIb;     // Ib相位角
    sMtFmt05    sIc;     // Ic相位角
}sMtPhaseI_f;

typedef struct
{   
    sMtTd_c_f    sTd_c;         // 曲线类数据时标Td_c
    sMtPhaseI_f  sPhase[0];     // 相位角
}sMtAfn0dF110_f;

// 转换函数
eMtErr emtTrans_afn0df110(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F113： A相2～19次谐波电流日最大值及发生时间 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F113_ELEC_HARM_TIME_A
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
#define MT_AFN0DF113_COUNT  (19 - 2 + 1)  // 2 ~ 19 次谐波加上总畸变
typedef struct 
{
    float       fHarmI;     // n 次谐波电流日最大值fXX.XX
    sMtFmt17    sTime;      // 最大值发生时间
}sMtIHarmTime;

typedef struct
{
    sMtTd_d         sTd_d;          // 曲线类数据时标Td_d
    sMtIHarmTime    sHarmTime[MT_AFN0DF113_COUNT];    // 谐波与时间
}sMtAfn0dF113;

// 帧侧
typedef struct 
{
    sMtFmt06    sHarmI;     // n 次谐波电流日最大值
    sMtFmt17_f  sTime;      // 最大值发生时间
}sMtIHarmTime_f;

typedef struct
{   
    sMtTd_d_f       sTd_d;         // 曲线类数据时标Td_d
    sMtIHarmTime_f  sHarmTime[MT_AFN0DF113_COUNT];    // 谐波与时间
}sMtAfn0dF113_f;

// 转换函数
eMtErr emtTrans_afn0df113(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F114：  B相2～19次谐波电流日最大值及发生时间 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F114_ELEC_HARM_TIME_B
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF113 sMtAfn0dF114;

// 帧侧
typedef sMtAfn0dF113_f sMtAfn0dF114_f;

// 转换函数
eMtErr emtTrans_afn0df114(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F115：  C相2～19次谐波电流日最大值及发生时间 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F115_ELEC_HARM_TIME_C
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF113 sMtAfn0dF115;

// 帧侧
typedef sMtAfn0dF113_f sMtAfn0dF115_f;

// 转换函数
eMtErr emtTrans_afn0df115(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F116： A相2～19次谐波电压含有率及总畸变率日最大值及发生时间 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F116_VOLT_HARM_TIME_A
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
#define MT_AFN0DF116_COUNT  (19 - 2 + 1)  // 2 ~ 19 次谐波加上总畸变

typedef struct 
{
    float       fHarmU;     // n 次谐波电压含有率日最大值fsXXX.X
    sMtFmt17    sTime;      // 最大值发生时间
}sMtUHarmTime;

typedef struct
{
    sMtTd_d         sTd_d;          // 曲线类数据时标Td_d
    sMtUHarmTime    sHarmTime[MT_AFN0DF116_COUNT]; // 谐波与时间
}sMtAfn0dF116;

// 帧侧

typedef struct 
{
    sMtFmt05    sHarmU;     // n 次谐波电压含有率日最大值
    sMtFmt17_f  sTime;      // 最大值发生时间
}sMtUHarmTime_f;

typedef struct
{   
    sMtTd_d_f       sTd_d;        // 曲线类数据时标Td_d
    sMtUHarmTime_f  sHarmTime[MT_AFN0DF116_COUNT];    // 谐波与时间
}sMtAfn0dF116_f;

// 转换函数
eMtErr emtTrans_afn0df116(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F117：  B相2～19次谐波电压含有率及总畸变率日最大值及发生时间 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F117_VOLT_HARM_TIME_B
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF116 sMtAfn0dF117;

// 帧侧
typedef sMtAfn0dF116_f sMtAfn0dF117_f;

// 转换函数
eMtErr emtTrans_afn0df117(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F118：  C相2～19次谐波电压含有率及总畸变率日最大值及发生时间 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F118_VOLT_HARM_TIME_C
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF116 sMtAfn0dF118;

// 帧侧
typedef sMtAfn0dF116_f sMtAfn0dF118_f;

// 转换函数
eMtErr emtTrans_afn0df118(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F121： A相谐波越限日统计数据 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F121_HARM_OVER_A
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
#define MT_AFN0DF121_COUNT  (19 - 2)  // 2 ~ 19 次谐波

typedef struct
{
    sMtTd_d     sTd_d;          // 曲线类数据时标Td_d
    UINT8       ucN;            // 谐波次数N
    UINT16      usUAbnormal;   // 总畸变电压含有率越限日累计时间
    UINT16      usUHarm[MT_AFN0DF121_COUNT];   // 总畸变电压含有率越限日累计时间
    UINT16      usIAbnormal;   // 总畸变电压含有率越限日累计时间
    UINT16      usIHarm[MT_AFN0DF121_COUNT];   // 总畸变电压含有率越限日累计时间
}sMtAfn0dF121;

// 帧侧
typedef struct
{   
    sMtTd_d_f    sTd_d;        // 曲线类数据时标Td_d
    UINT8        ucN;          // 谐波次数N
    UINT16       usTime[0];   // 越限累计时间
}sMtAfn0dF121_f;

// 转换函数
eMtErr emtTrans_afn0df121(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F122：   B相谐波越限日统计数据 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F122_HARM_OVER_B
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF121 sMtAfn0dF122;

// 帧侧
typedef sMtAfn0dF121_f sMtAfn0dF122_f;

// 转换函数
eMtErr emtTrans_afn0df122(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F123：   C相谐波越限日统计数据 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F123_HARM_OVER_C
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF121 sMtAfn0dF123;

// 帧侧
typedef sMtAfn0dF121_f sMtAfn0dF123_f;

// 转换函数
eMtErr emtTrans_afn0df123(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F129：  直流模拟量越限日累计时间、最大/最小值及发生时间 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F129_DC_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
//typedef sMtAfn0dF121 sMtAfn0dF123;

// 帧侧
typedef struct 
{
    sMtTd_d     sTd_d;             // 曲线类数据时标Td_d
    UINT16      usOverUpLimit;    // 直流模拟量越上限月累计时间
    UINT16      usOverDownLimit;  // 直流模拟量越下限月累计时间
    sMtFmt02_f  sMax;              // 直流模拟量最大值
    sMtFmt18_f  sMaxTime;         // 直流模拟量最大值发生时间
    sMtFmt02_f  sMin;             // 直流模拟量最小值  
    sMtFmt18_f  sMinTime;        // 直流模拟量最小值发生时间   
}sMtAfn0dF129_f;

// 转换函数
eMtErr emtTrans_afn0df129(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F130：   直流模拟量越限月累计时间、最大/最小值及发生时间 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F130_DC_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF129_f sMtAfn0dF130_f;

// 转换函数
eMtErr emtTrans_afn0df130(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F138：  直流模拟量数据曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F138_DC_CURVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct 
{
    sMtTd_c sTd_c;          // 曲线类数据时标Td_c   
}sMtAfn0dF138;

// 帧侧
typedef struct
{
    sMtTd_c_f   sTd_c;      // 曲线类数据时标Td_c
    sMtFmt02_f  sData[0];   // 直流模拟量冻结数据
}sMtAfn0dF138_f;

// 转换函数
eMtErr emtTrans_afn0df138(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F145： 一象限无功总电能示值曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F145_CURVE_PHASE_1
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct 
{
    sMtTd_c sTd_c;          // 曲线类数据时标Td_c   
}sMtAfn0dF145;

// 帧侧
typedef struct
{
    sMtTd_c_f   sTd_c;      // 曲线类数据时标Td_c
    sMtFmt11_f  sData[0];   // 电能示值
}sMtAfn0dF145_f;

// 转换函数
eMtErr emtTrans_afn0df145(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F146：四象限无功总电能示值曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F146_CURVE_PHASE_4
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF145 sMtAfn0dF146;

// 帧侧
typedef sMtAfn0dF145_f sMtAfn0dF146_f;

// 转换函数
eMtErr emtTrans_afn0df146(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F147：二象限无功总电能示值曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F147_CURVE_PHASE_2
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF145 sMtAfn0dF147;

// 帧侧
typedef sMtAfn0dF145_f sMtAfn0dF147_f;

// 转换函数
eMtErr emtTrans_afn0df147(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F148：三象限无功总电能示值曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F148_CURVE_PHASE_3
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF145 sMtAfn0dF148;

// 帧侧
typedef sMtAfn0dF145_f sMtAfn0dF148_f;

// 转换函数
eMtErr emtTrans_afn0df148(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F153：日冻结分相正向有功电能示值 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F153_FRTH_HAVE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef struct
{
    sMtTd_d_f   sTd_d;      // 日冻结类数据时标Td_d
    sMtFmt15_f  sTime;      // 终端抄表时间
    sMtFmt14_f  sValueA;    // A相有功电能示值
    sMtFmt14_f  sValueB;    // B相有功电能示值
    sMtFmt14_f  sValueC;    // C相有功电能示值
}sMtAfn0dF153_f;

// 转换函数
eMtErr emtTrans_afn0df153(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F154：日冻结分相正向无功电能示值 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F154_FRTH_NONE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef struct
{
    sMtTd_d_f   sTd_d;      // 日冻结类数据时标Td_d
    sMtFmt15_f  sTime;      // 终端抄表时间
    sMtFmt11_f  sValueA;    // A相无功电能示值
    sMtFmt11_f  sValueB;    // B相无功电能示值
    sMtFmt11_f  sValueC;    // C相无功电能示值
}sMtAfn0dF154_f;

// 转换函数
eMtErr emtTrans_afn0df154(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F155：日冻结分相反向有功电能示值 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F155_BACK_HAVE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF153_f sMtAfn0dF155_f;

// 转换函数
eMtErr emtTrans_afn0df155(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F156：日冻结分相反向无功电能示值 日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F156_BACK_NONE_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF154_f sMtAfn0dF156_f;

// 转换函数
eMtErr emtTrans_afn0df156(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F157：月冻结分相正向有功电能示值 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F157_FRTH_HAVE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct {
    sMtTd_m     sTd_m;      // 日冻结类数据时标Td_m
    sMtFmt15    sTime;      // 终端抄表时间
    DOUBLE      dValueA;    // A相有功电能示值dXXXXXX.XXXX
    DOUBLE      dValueB;    // B相有功电能示值
    DOUBLE      dValueC;    // C相有功电能示值
}sMtAfn0dF157;

// 帧侧
typedef struct {
    sMtTd_m_f   sTd_m;      // 日冻结类数据时标Td_m
    sMtFmt15_f  sTime;      // 终端抄表时间
    sMtFmt14_f  sValueA;    // A相有功电能示值
    sMtFmt14_f  sValueB;    // B相有功电能示值
    sMtFmt14_f  sValueC;    // C相有功电能示值
}sMtAfn0dF157_f;

// 转换函数
eMtErr emtTrans_afn0df157(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F158：月冻结分相正向无功电能示值 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F158_FRTH_NONE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef struct
{
    sMtTd_d_f   sTd_m;      // 日冻结类数据时标Td_m
    sMtFmt15_f  sTime;      // 终端抄表时间
    sMtFmt11_f  sValueA;    // A相无功电能示值
    sMtFmt11_f  sValueB;    // B相无功电能示值
    sMtFmt11_f  sValueC;    // C相无功电能示值
}sMtAfn0dF158_f;

// 转换函数
eMtErr emtTrans_afn0df158(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F159：月冻结分相反向有功电能示值 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F159_BACK_HAVE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF157_f sMtAfn0dF159_f;

// 转换函数
eMtErr emtTrans_afn0df159(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F160：月冻结分相反向无功电能示值 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F160_BACK_NONE_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF158_f sMtAfn0dF160_f;

// 转换函数
eMtErr emtTrans_afn0df160(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F161：正向有功电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F161_HAVE_FRTH_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct 
{
    sMtTd_d         sTd_d;          // 冻结类数据时标Td_d  
    sMtFmt15        sTime;          // 终端抄表时间
    UINT8           ucM;            // 费率M
    DOUBLE          dValue[MT_M_MAX];     //  有功电能示值 XXXXXX.XXXX
}sMtAfn0dF161;
// 帧侧
typedef struct
{
    sMtTd_d_f       sTd_d;          // 冻结类数据时标Td_d
    sMtFmt15_f      sTime;          // 终端抄表时间
    UINT8           ucM;            // 费率M
    sMtFmt14_f      sValue[0];     //  有功电能示值         
}sMtAfn0dF161_f;

// 转换函数
eMtErr emtTrans_afn0df161(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F162：正向无功（组合无功1）电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F162_NONE_FRTM_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct 
{
    sMtTd_d         sTd_d;          // 冻结类数据时标Td_d  
    sMtFmt15        sTime;          // 终端抄表时间
    UINT8           ucM;            // 费率M
    DOUBLE          dValue[MT_M_MAX];  //dXXXXXX.XX
}sMtAfn0dF162;
// 帧侧
typedef struct
{
    sMtTd_d_f       sTd_d;          // 冻结类数据时标Td_d
    sMtFmt15_f      sTime;          // 终端抄表时间
    UINT8           ucM;            // 费率M
    sMtFmt11_f      sValue[0];     //  有功电能示值         
}sMtAfn0dF162_f;

// 转换函数
eMtErr emtTrans_afn0df162(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F163：反向有功电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F163_HAVE_BACK_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF161 sMtAfn0dF163;
// 帧侧
typedef sMtAfn0dF161_f sMtAfn0dF163_f;

// 转换函数
eMtErr emtTrans_afn0df163(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F164：反向无功（组合无功1）电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F164_NONE_BACK_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF162 sMtAfn0dF164;
// 帧侧
typedef sMtAfn0dF162_f sMtAfn0dF164_f;

// 转换函数
eMtErr emtTrans_afn0df164(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F165： 一象限无功电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F165_NONE_D_PHASE_1
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF162 sMtAfn0dF165;
// 帧侧
typedef sMtAfn0dF162_f sMtAfn0dF165_f;

// 转换函数
eMtErr emtTrans_afn0df165(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: F166：  二象限无功电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F166_NONE_D_PHASE_2
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF162 sMtAfn0dF166;
// 帧侧
typedef sMtAfn0dF162_f sMtAfn0dF166_f;

// 转换函数
eMtErr emtTrans_afn0df166(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F167：  三象限无功电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F167_NONE_D_PHASE_3
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF162 sMtAfn0dF167;
// 帧侧
typedef sMtAfn0dF162_f sMtAfn0dF167_f;

// 转换函数
eMtErr emtTrans_afn0df167(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F168：  四象限无功电能示值（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F168_NONE_D_PHASE_4
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF162 sMtAfn0dF168;
// 帧侧
typedef sMtAfn0dF162_f sMtAfn0dF168_f;

// 转换函数
eMtErr emtTrans_afn0df168(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F169：  正向有功电能示值（总、费率1～M）  抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F169_HAVE_FRTH_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF157 sMtAfn0dF169;

// 帧侧
typedef sMtAfn0dF157_f sMtAfn0dF169_f;

// 转换函数
eMtErr emtTrans_afn0df169(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F170：  正向有功电能示值（总、费率1～M）  抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F170_NONE_FRTM_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF162 sMtAfn0dF170;
// 帧侧
typedef sMtAfn0dF162_f sMtAfn0dF170_f;

// 转换函数
eMtErr emtTrans_afn0df170(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F171：  反向有功电能示值（总、费率1～M）  抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F171_HAVE_BACK_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF163 sMtAfn0dF171;

// 帧侧
typedef sMtAfn0dF163_f sMtAfn0dF171_f;

// 转换函数
eMtErr emtTrans_afn0df171(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F172： 反向无功（组合无功1）电能示值（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F172_NONE_BACK_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF164_f sMtAfn0dF172_f;

// 转换函数
eMtErr emtTrans_afn0df172(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F173： 一象限无功电能示值（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F173_NONE_R_PHASE_1 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF165_f sMtAfn0dF173_f;

// 转换函数
eMtErr emtTrans_afn0df173(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F174： 二象限无功电能示值（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F174_NONE_R_PHASE_2 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF166_f sMtAfn0dF174_f;

// 转换函数
eMtErr emtTrans_afn0df174(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F175： 三象限无功电能示值（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F175_NONE_R_PHASE_3 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF167_f sMtAfn0dF175_f;

// 转换函数
eMtErr emtTrans_afn0df175(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F176： 四象限无功电能示值（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F176_NONE_R_PHASE_4 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧

// 帧侧
typedef sMtAfn0dF168_f sMtAfn0dF176_f;

// 转换函数
eMtErr emtTrans_afn0df176(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F177： 正向有功电能示值（总、费率1～M）   月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F177_HAVE_FRTH_M 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m         sTd_m;       // 冻结类数据时标
    sMtFmt15        sTime;       // 终端抄表时间
    UINT8           ucM;         // 费率M
    DOUBLE          dValue[MT_M_MAX];// 有功电能示值
 }sMtAfn0dF177;

// 帧侧
typedef struct
{
    sMtTd_m_f       sTd_m;       // 冻结类数据时标
    sMtFmt15_f      sTime;       // 终端抄表时间
    UINT8           ucM;         // 费率M
    sMtFmt14_f      sValue[0];  // 有功电能示值dXXXXXX.XXXX
}sMtAfn0dF177_f;

// 转换函数
eMtErr emtTrans_afn0df177(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F178： 正向无功（组合无功1）电能示值（总、费率1～M）月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F178_NONE_FRTM_M 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m         sTd_m;       // 冻结类数据时标
    sMtFmt15        sTime;       // 终端抄表时间
    UINT8           ucM;         // 费率M
    DOUBLE          dValue[MT_M_MAX];  // 无功电能示值dXXXXXX.XX
 }sMtAfn0dF178;

// 帧侧
typedef struct
{
    sMtTd_m_f       sTd_m;       // 冻结类数据时标
    sMtFmt15_f      sTime;       // 终端抄表时间
    UINT8           ucM;         // 费率M
    sMtFmt11_f      sValue[0];  // 无功电能示值
}sMtAfn0dF178_f;

// 转换函数
eMtErr emtTrans_afn0df178(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F179： 反向有功电能示值（总、费率1～M）   月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F179_HAVE_BACK_M 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF177 sMtAfn0dF179;

// 帧侧
typedef sMtAfn0dF177_f sMtAfn0dF179_f;

// 转换函数
eMtErr emtTrans_afn0df179(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F180：反向无功（组合无功1）电能示值（总、费率1～M）月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F180_NONE_BACK_M 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF178 sMtAfn0dF180;

// 帧侧
typedef sMtAfn0dF178_f sMtAfn0dF180_f;

// 转换函数
eMtErr emtTrans_afn0df180(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F181：一象限无功电能示值（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F181_NONE_M_PHASE_1 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF178 sMtAfn0dF181;

// 帧侧
typedef sMtAfn0dF178_f sMtAfn0dF181_f;

// 转换函数
eMtErr emtTrans_afn0df181(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F182：二象限无功电能示值（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F182_NONE_M_PHASE_2 
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF178 sMtAfn0dF182;

// 帧侧
typedef sMtAfn0dF178_f sMtAfn0dF182_f;

// 转换函数
eMtErr emtTrans_afn0df182(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F183：三象限无功电能示值（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F183_NONE_M_PHASE_3
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF178 sMtAfn0dF183;

// 帧侧
typedef sMtAfn0dF178_f sMtAfn0dF183_f;

// 转换函数
eMtErr emtTrans_afn0df183(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F183：四象限无功电能示值（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F184_NONE_M_PHASE_4
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF178 sMtAfn0dF184;

// 帧侧
typedef sMtAfn0dF178_f sMtAfn0dF184_f;

// 转换函数
eMtErr emtTrans_afn0df184(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F185：正向有功最大需量及发生时间（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F185_HAVE_DMND_FRTH_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct 
{
    float           fMax;       // 最大需量XX_XXXX
    sMtFmt17        sClock;     // 最大需量发生时间
}sMtMaxTime;

typedef struct
{
    sMtTd_d             sTd_d;      // 冻结类数据时标
    sMtFmt15            sTime;      // 终端抄表时间
    UINT8               ucM;        // 费率M    
    sMtMaxTime          sMaxClock[MT_M_MAX + 1];    // 最大需量和发生时间
}sMtAfn0dF185;

// 帧侧
typedef struct 
{
    sMtFmt23_f      sMax;       // 最大需量
    sMtFmt17_f      sClock;     // 最大需量发生时间
}sMtMaxTime_f;

typedef struct
{
    sMtTd_d_f           sTd_d;      // 冻结类数据时标
    sMtFmt15_f          sTime;      // 终端抄表时间
    UINT8               ucM;        // 费率M
    sMtMaxTime_f        sMaxClock[1]; // 最大需量和发生时间
}sMtAfn0dF185_f;

// 转换函数
eMtErr emtTrans_afn0df185(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F186：正向无功最大需量及发生时间（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F186_NONE_DMND_FRTH_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF185 sMtAfn0dF186;

// 帧侧

typedef sMtAfn0dF185_f sMtAfn0dF186_f;

// 转换函数
eMtErr emtTrans_afn0df186(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F187：反向有功最大需量及发生时间（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F187_HAVE_DMND_BACK_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF185 sMtAfn0dF187;

// 帧侧

typedef sMtAfn0dF185_f sMtAfn0dF187_f;

// 转换函数
eMtErr emtTrans_afn0df187(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F188：反向无功最大需量及发生时间（总、费率1～M）日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F188_NONE_DMND_BACK_D
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF185 sMtAfn0dF188;

// 帧侧

typedef sMtAfn0dF185_f sMtAfn0dF188_f;

// 转换函数
eMtErr emtTrans_afn0df188(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F189：正向有功最大需量及发生时间（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F189_HAVE_DMND_FRTH_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF185 sMtAfn0dF189;

// 帧侧

typedef sMtAfn0dF185_f sMtAfn0dF189_f;

// 转换函数
eMtErr emtTrans_afn0df189(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F190：正向无功最大需量及发生时间（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F190_NONE_DMND_FRTH_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF186 sMtAfn0dF190;

// 帧侧

typedef sMtAfn0dF186_f sMtAfn0dF190_f;

// 转换函数
eMtErr emtTrans_afn0df190(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F191：反向有功最大需量及发生时间（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F191_HAVE_DMND_BACK_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF187 sMtAfn0dF191;

// 帧侧

typedef sMtAfn0dF187_f sMtAfn0dF191_f;

// 转换函数
eMtErr emtTrans_afn0df191(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F192：反向无功最大需量及发生时间（总、费率1～M）抄表日冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F192_NONE_DMND_BACK_R
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF188 sMtAfn0dF192;

// 帧侧

typedef sMtAfn0dF188_f sMtAfn0dF192_f;

// 转换函数
eMtErr emtTrans_afn0df192(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F193：正向有功最大需量及发生时间（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F193_HAVE_DMND_FRTH_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m             sTd_m;      // 冻结类数据时标td_m
    sMtFmt15            sTime;      // 终端抄表时间
    UINT8               ucM;        // 费率M    
}sMtAfn0dF193;

// 帧侧

typedef struct
{
    sMtTd_d_f           sTd_m;      // 冻结类数据时标td_m
    sMtFmt15_f          sTime;      // 终端抄表时间
    UINT8               ucM;        // 费率M
    sMtMaxTime          sMaxClock[1]; // 最大需量和发生时间
}sMtAfn0dF193_f;

// 转换函数
eMtErr emtTrans_afn0df193(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F194：正向无功最大需量及发生时间（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F194_NONE_DMND_FRTH_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF193 sMtAfn0dF194;

// 帧侧

typedef sMtAfn0dF193_f sMtAfn0dF194_f;

// 转换函数
eMtErr emtTrans_afn0df194(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F195：反向有功最大需量及发生时间（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F195_HAVE_DMND_BACK_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF193 sMtAfn0dF195;

// 帧侧

typedef sMtAfn0dF193_f sMtAfn0dF195_f;

// 转换函数
eMtErr emtTrans_afn0df195(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F196：反向无功最大需量及发生时间（总、费率1～M） 月冻结
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F196_NONE_DMND_BACK_M
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF193 sMtAfn0dF196;

// 帧侧

typedef sMtAfn0dF193_f sMtAfn0dF196_f;

// 转换函数
eMtErr emtTrans_afn0df196(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F201：第一时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F201_FREZ_ZONE_1
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF177 sMtAfn0dF201;

// 帧侧

typedef sMtAfn0dF177_f sMtAfn0dF201_f;

// 转换函数
eMtErr emtTrans_afn0df201(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F202：第二时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F202_FREZ_ZONE_2
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF201 sMtAfn0dF202;

// 帧侧

typedef sMtAfn0dF201_f sMtAfn0dF202_f;

// 转换函数
eMtErr emtTrans_afn0df202(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F203：第三时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F203_FREZ_ZONE_3
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF201 sMtAfn0dF203;

// 帧侧

typedef sMtAfn0dF201_f sMtAfn0dF203_f;

// 转换函数
eMtErr emtTrans_afn0df203(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F204：第四时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F204_FREZ_ZONE_4
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF201 sMtAfn0dF204;

// 帧侧

typedef sMtAfn0dF201_f sMtAfn0dF204_f;

// 转换函数
eMtErr emtTrans_afn0df204(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F205：第五时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F205_FREZ_ZONE_5
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF201 sMtAfn0dF205;

// 帧侧

typedef sMtAfn0dF201_f sMtAfn0dF205_f;

// 转换函数
eMtErr emtTrans_afn0df205(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F206：第六时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F206_FREZ_ZONE_6
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF201 sMtAfn0dF206;

// 帧侧

typedef sMtAfn0dF201_f sMtAfn0dF206_f;

// 转换函数
eMtErr emtTrans_afn0df206(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F207：第七时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F207_FREZ_ZONE_7
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF201 sMtAfn0dF207;

// 帧侧

typedef sMtAfn0dF201_f sMtAfn0dF207_f;

// 转换函数
eMtErr emtTrans_afn0df207(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F208：第八时区冻结正向有功电能示值（总、费率1～M）
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F208_FREZ_ZONE_8
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef sMtAfn0dF201 sMtAfn0dF208;

// 帧侧

typedef sMtAfn0dF201_f sMtAfn0dF208_f;

// 转换函数
eMtErr emtTrans_afn0df208(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F209：电能表远程控制通断电状态及记录
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F209_METR_REMOTE_INFO
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{

}sMtAfn0dF209;

// 帧侧
#define MT_AFN0DF209_METER_ON       0       //电能表断电
#define MT_AFN0DF209_METER_OFF      0x11   // 电能表通电

typedef struct
{
    sMtTd_d_f       sTd_d;          // 冻结类数据时标td_d
    sMtFmt15_f      sTime;          //终端抄表时间
    UINT8           ucOnOff;        // 电能表通断电状态
    sMtFmt15_f      sTimeLastOn;    // 最后一次电能表远程控制通电时间
    sMtFmt15_f      sTimeLastOff;   // 最后一次电能表远程控制断电时间  
}sMtAfn0dF209_f;

// 转换函数
eMtErr emtTrans_afn0df209(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F213：电能表开关操作次数及时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F213_METR_SWITCH_INFO
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{

}sMtAfn0dF213;

// 帧侧
typedef struct
{
    sMtTd_m_f       sTd_m;          // 冻结类数据时标td_m
    sMtFmt15_f      sTime;          //终端抄表时间
    sMtFmt08_f      sSwitchTimes;  // 编程开关操作次数
    sMtFmt15_f      sSwitchClock;   // 最后一次编程开关操作时间
    sMtFmt08_f      sTailTimes;     // 电能表尾盖打开次数
    sMtFmt15_f      sTailClock;     // 最后一次电能表尾盖打开时间 
}sMtAfn0dF213_f;

// 转换函数
eMtErr emtTrans_afn0df213(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F214：电能表参数修改次数及时间
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F214_METR_MODIFY_INFO
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{

}sMtAfn0dF214;

// 帧侧
typedef struct
{
    sMtTd_m_f       sTd_m;          // 冻结类数据时标td_m
    sMtFmt15_f      sTime;          //终端抄表时间
    sMtFmt08_f      sModifyTimes;  //电能表时钟修改次数
    sMtFmt15_f      sModifyClock;   // 最后一次时钟修改时间
    sMtFmt08_f      sParaTimes;     // 电能表时段参数修改次数
    sMtFmt15_f      sParaClock;     // 最后一次电能表时段参数修改时间
}sMtAfn0dF214_f;

// 转换函数
eMtErr emtTrans_afn0df214(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F215：电能表购、用电信息
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F215_METR_BUY_USR
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m         sTd_m;          // 冻结类数据时标td_m
    sMtFmt15        sTime;          //终端抄表时间
    UINT16          usBuyTimes;      //购电次数
    DOUBLE          dCashLeft;      // 剩余金额dXXXXXX.XXXX
    DOUBLE          dCashTotal;     // 累计购电金额dXXXXXX.XXXX
    DOUBLE          dKilowattLeft; // 剩余电量dXXXXXX.XX
    DOUBLE          dKilowattOver; // 透支电量dXXXXXX.XX
    DOUBLE          dKilowattTotal; // 累计购电量dXXXXXX.XX  
    DOUBLE          dKilowattDebit; // 赊欠门购电量dXXXXXX.XX
    DOUBLE          dKilowattWarn; // 报警购电量dXXXXXX.XX
    DOUBLE          dKilowattFault;  // 故障电量dXXXXXX.XX
}sMtAfn0dF215;

// 帧侧
typedef struct
{
    sMtTd_m_f       sTd_m;          // 冻结类数据时标td_m
    sMtFmt15_f      sTime;          //终端抄表时间
    sMtFmt08_f      sBuyTimes;      //购电次数
    sMtFmt14_f      sCashLeft;      // 剩余金额
    sMtFmt14_f      sCashTotal;     // 累计购电金额
    sMtFmt11_f      sKilowattLeft; // 剩余电量
    sMtFmt11_f      sKilowattOver; // 透支电量
    sMtFmt11_f      sKilowattTotal; // 累计购电量  
    sMtFmt11_f      sKilowattDebit; // 赊欠门购电量
    sMtFmt11_f      sKilowattWarn; // 报警购电量
    sMtFmt11_f      sKilowattFault;  // 故障电量
}sMtAfn0dF215_f;

// 转换函数
eMtErr emtTrans_afn0df215(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F216：电能表结算信息
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F216_METR_BALANCE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_m         sTd_m;          // 冻结类数据时标td_m
    sMtFmt15        sTime;          //终端抄表时间
    UINT8           ucM;            // 费率M
    DOUBLE          dTotalBalance;  //已结有功总电能dXXXXXX.XXXX
    DOUBLE          dBalance[MT_M_MAX]; // 费率M 已结正向有功总电能dXXXXXX.XXXX
    DOUBLE          dTotalNoBalance; // 未结有功总电能dXXXXXX.XXXX
    DOUBLE          dNoBalance[MT_M_MAX]; // 未结费率M 有功总电能dXXXXXX.XXXX
    DOUBLE          dFault;         // 故障电量dXXXXXX.XXXX
}sMtAfn0dF216;

// 帧侧
typedef struct
{
    sMtTd_m_f       sTd_m;          // 冻结类数据时标td_m
    sMtFmt15_f      sTime;          //终端抄表时间
    UINT8           ucM;            // 费率M
    sMtFmt14_f      sValue[0];      // 有功电能
}sMtAfn0dF216_f;

// 转换函数
eMtErr emtTrans_afn0df216(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F217：台区集中抄表载波主节点白噪声曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F217_WHITE_YAWP_CURVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_c         sTd_c;          // 冻结类数据时标td_c
    UINT8           ucNoiseRate[256];   // 噪声比值    
}sMtAfn0dF217;

// 帧侧
typedef struct
{
    sMtTd_c_f       sTd_c;          // 冻结类数据时标td_c
    UINT8           ucNoiseRate[0];            // 噪声比值
}sMtAfn0dF217_f;

// 转换函数
eMtErr emtTrans_afn0df217(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: F218：台区集中抄表载波主节点色噪声曲线
 * 对应AFN : AFN_0D_ASK2
 * 对应CMD : CMD_AFN_D_F218_COLOR_YAWP_CURVE
 * PN 类型 : 测量点号
 *
{*///
// 用户侧
typedef struct
{
    sMtTd_c         sTd_c;          // 冻结类数据时标td_c  
}sMtAfn0dF218;

// 帧侧
typedef struct 
{
    UINT8       ucNoiseRate;        // 噪声比
    sMtFmt12    sAddress;           //主节点地址
}sMtCurvePoint_f;

typedef struct
{
    sMtTd_c_f           sTd_c;          // 冻结类数据时标td_c
    sMtCurvePoint_f     sPoint[0];      // 噪声比值
}sMtAfn0dF218_f;

// 转换函数
eMtErr emtTrans_afn0df218(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}



///*}



/*******************************************
 * 类型定义: 各事件记录的数据格式
 * 
{*///

/*******************************************
 * 数据结构: ERC1：数据初始化和版本变更记录
 * 对应类型: MT_ERC_1_INIT
 *
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_1_INIT
    sMtYYMMDDhhmm  sTime;          // 初始化/版本变更时间：分时日月年
    BOOL           bInit;          // 置"真"：终端进行参数及数据区初始化，记录初始化时间；置"假"：无初始化发生
    BOOL           bModify;        // 置"真"：终端版本变更，记录变更时间和变更前后版本号；置"假"：版本未变更
    UINT8          ucPreVerson[4]; // 变更前软件版本号
    UINT8          ucNewVerson[4]; // 变更后软件版本号
}sMtRec1, sMtRecInit;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 1
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 初始化/版本变更时间：分时日月年
    UINT8          ucFlag;         // 事件标志
    UINT8          ucPreVerson[4]; // 变更前软件版本号
    UINT8          ucNewVerson[4]; // 变更后软件版本号

}sMtRec1_f, sMtRecInit_f;

// 转换函数
eMtErr emtTrans_rec_1(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC2：参数丢失记录
 * 对应类型: MT_ERC_2_PARA_LOSS
 *
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_2_PARA_LOSS
    sMtYYMMDDhhmm  sTime;          // 初始化/版本变更时间：分时日月年
    BOOL           bTmlParaLoss;   // 置"1"：终端参数丢失，置"0"：参数未丢失；
    BOOL           bMpParaLoss;    // 置"1"：测量点参数丢失，置"0"：参数未丢失。
   
}sMtRec2, sMtRecParaLoss;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 2
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 初始化/版本变更时间：分时日月年
    UINT8          ucFlag;         // 事件标志

}sMtRec2_f, sMtRecParaLoss_f;

// 转换函数
eMtErr emtTrans_rec_2(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);

///*}

/*******************************************
 * 数据结构: ERC3：参数变更记录
 * 对应类型: MT_ERC_3_PARA_MODIFY
 * 
{*///
typedef struct
{
    UINT8  ucFn;                  // 有疑问: 是不是只是设置参数的那些CMD 还是支持所有命令? 
    UINT16 ucPn;                  // 发现有问题: 信息不完备 返向解释时不能得到该Fn 对应的AFN

}sMtRec2One;

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_3_PARA_MODIFY
    sMtYYMMDDhhmm  sTime;          // 初始化/版本变更时间：分时日月年
    UINT8          ucMstAddr;      // 启动站地址
    UINT8          ucNum;          // 参数变更的数据单元标识个数
    sMtRec2One     sDIndex[1];     // 变更参数数据单元标识1 ~ i (ucNum 个)
   
}sMtRec3, sMtRecParaModify;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 3
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 初始化/版本变更时间：分时日月年
    UINT8          ucMstAddr;      // 启动站地址
    sMtDaDt        sDaDt[1];       // 变更参数数据单元标识1 ~ i (ucLe 来计算)
}sMtRec3_f, sMtRecParaModify_f;

// 转换函数
eMtErr emtTrans_rec_3(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);

///*}

/*******************************************
 * 数据结构: ERC4：状态量变位记录
 * 对应类型: MT_ERC_4_STATE_CHANGE
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_4_STATE_CHANGE
    sMtYYMMDDhhmm  sTime;          // 初始化/版本变更时间：分时日月年
    BOOL           bIsChange[8];   // 状态变位 置"1"：发生变位，置"0"：无变位发生。
    BOOL           bChange[8];     // 变位后状态
    
}sMtRec4, sMtRecStateChange;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 4
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 初始化/版本变更时间：分时日月年
    UINT8          bIsChange;      // 状态变位 置"1"：发生变位，置"0"：无变位发生。
    UINT8          bChange;        // 变位后状态
}sMtRec4_f, sMtRecStateChange_f;

// 转换函数
eMtErr emtTrans_rec_4(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC5：遥控跳闸记录
 * 对应类型: MT_ERC_5_REMOTE_SWITCH
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_5_REMOTE_SWITCH
    sMtYYMMDDhhmm  sTime;          // 跳闸时间：分时日月年
    BOOL           bIsOff[8];      // 跳闸轮次：0～7按顺序对位表示1～8个控制轮次，置"真"：该轮次跳闸，置"假"：该轮次未跳闸。
    float          fPower;         // 跳闸时功率（总加功率）
    float          fPower2m;       // 跳闸后2分钟的功率（总加功率）
}sMtRec5, sMtRecRemoteSwitch;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 5
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 跳闸时间：分时日月年
    UINT8          bIsOff;         // 跳闸轮次
    sMtFmt02_f     fPower;         // 跳闸时功率（总加功率）
    sMtFmt02_f     fPower2m;       // 跳闸后2分钟的功率（总加功率）
    
}sMtRec5_f, sMtRecRemoteSwitch_f;

// 转换函数
eMtErr emtTrans_rec_5(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC6：功控跳闸记录
 * 对应类型: MT_ERC_6_PCTRL_SWITCH
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_6_PCTRL_SWITCH
    sMtYYMMDDhhmm  sTime;          // 跳闸时间：分时日月年
    UINT8          ucPn;           //  pn（总加组号）
    BOOL           bIsOff[8];      // 跳闸轮次：0～7按顺序对位表示1～8个控制轮次，置"真"：该轮次跳闸，置"假"：该轮次未跳闸。
    BOOL           bPeriod;        // 时段控      是否有效
    BOOL           bFactoryHolidy; // 厂休控      是否有效
    BOOL           bShutOut;       // 营业报停控  是否有效
    BOOL           bDownGo;        // 当前功率下浮控
    float          fPower;         // 跳闸时功率（总加功率）
    float          fPower2m;       // 跳闸后2分钟的功率（总加功率）
    float          fPowerFix;      // 跳闸时功率定值
    
}sMtRec6, sMtRecPCtrlSwitch;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 6
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 跳闸时间：分时日月年
    UINT8          ucPn;           // pn（总加组号）
    UINT8          bIsOff;         // 跳闸轮次
    UINT8          ucType;         // 功控类别
    sMtFmt02_f     fPower;         // 跳闸时功率（总加功率）
    sMtFmt02_f     fPower2m;       // 跳闸后2分钟的功率（总加功率）
    sMtFmt02_f     fPowerFix;      // 跳闸时功率定值
    
}sMtRec6_f, sMtRecPCtrlSwitch_f;

// 转换函数
eMtErr emtTrans_rec_6(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC7：电控跳闸记录
 * 对应类型: MT_ERC_7_ECTRL_SWITCH
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_7_ECTRL_SWITCH
    sMtYYMMDDhhmm  sTime;          // 跳闸时间：分时日月年
    UINT8          ucPn;           //  pn（总加组号）
    BOOL           bIsOff[8];      // 跳闸轮次：0～7按顺序对位表示1～8个控制轮次，置"真"：该轮次跳闸，置"假"：该轮次未跳闸。
    BOOL           bMonth;         // 月电控      是否有效
    BOOL           bBuy;           // 购电控      是否有效
    sMtFmt_sX7     sPower;         // 月电能量(月电控)    剩余电能量/费(购电控)
    sMtFmt_sX7     sPowerFix;      // 月电控定值(月电控)  购电控跳闸门限(购电控)
    
}sMtRec7, sMtRecECtrlSwitch;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 7
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 跳闸时间：分时日月年
    UINT8          ucPn;           //  pn（总加组号）
    UINT8          bIsOff;         //  跳闸轮次
    UINT8          ucType;         //  功控类别  
    sMtFmt03_f     sPower;         // 月电能量(月电控)    剩余电能量/费(购电控)
    sMtFmt03_f     sPowerFix;      // 月电控定值(月电控)  购电控跳闸门限(购电控)
    
}sMtRec7_f, sMtRecECtrlSwitch_f;

// 转换函数
eMtErr emtTrans_rec_7(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC8：电能表参数变更
 * 对应类型: MT_ERC_8_METER_CHANGE
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_8_METER_CHANGE
    sMtYYMMDDhhmm  sTime;          // 跳闸时间：分时日月年
    UINT16         usPn;           // pn（测量点号）(1~2048)
    BOOL           bPayRate;       // 置"1"：电能表费率时段变化，置"0"：电表费率时段无变更；
    BOOL           bPramTime;      // 置"1"：电能表编程时间更改，置"0"：编程时间未更改；
    BOOL           bReadMeter;     // 置"1"：电能表抄表日更改，  置"0"：抄表日未更改；
    BOOL           bPulse;         // 置"1"：电能表脉冲常数更改，置"0"：脉冲常数未更改；
    BOOL           bInduct;        // 置"1"：电能表的互感器倍率更改，置"0"：互感器倍率未更改；
    BOOL           bDemand;        // 置"1"：电能表最大需量清零，置"0"：最大需量未被清零；
}sMtRec8, sMtRecMeterChange;

// 帧侧
typedef struct
{
    UINT8          ucRec;          // ERC = 8
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // D11～D0：pn（测量点号1～2048）
    UINT8          ucFlag;         // 变更标志
    
}sMtRec8_f, sMtRecMeterChange_f;

// 转换函数
eMtErr emtTrans_rec_8(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC9：电流回路异常
 * 对应类型: MT_ERC_9_ELEC_EXCP
 * 
{*///
typedef enum
{
    MT_ELEC_EXCP_UNKOWN,     // 未知 备用
    MT_ELEC_EXCP_SHORT,      // 短路
    MT_ELEC_EXCP_CARVE,      // 开路
    MT_ELEC_EXCP_BACK,       // 反向
        
}eMtElecExcp;

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_9_ELEC_EXCP
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    eMtElecExcp    eExcp;          // 异常类型
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bPhaseA;        // A 相是否发生异常
    BOOL           bPhaseB;        // B 相是否发生异常
    BOOL           bPhaseC;        // C 相是否发生异常
    float          fUa;            // 发生时的Ua/Uab    (+)XXX.X
    float          fUb;            // 发生时的Ub        (+)XXX.X
    float          fUc;            // 发生时的Uc/Ucb    (+)XXX.X
    float          fIa;            // 发生时的Ia        (+/-)XXX.XXX
    float          fIb;            // 发生时的Ib        (+/-)XXX.XXX
    float          fIc;            // 发生时的Ic        (+/-)XXX.XXX
    double         dPower;         // 发生时电能表正向有功总电能示值     (+)XXXXXX.XXXX
    
}sMtRec9, sMtRecElecExcp;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 9
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 异常标志 
    sMtFmt07_f     fUa;
    sMtFmt07_f     fUb;
    sMtFmt07_f     fUc;
    sMtFmt25_f     fIa;
    sMtFmt25_f     fIb;
    sMtFmt25_f     fIc;
    sMtFmt14_f     dPower;
    
}sMtRec9_f, sMtRecElecExcp_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_9(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC10：电压回路异常
 * 对应类型: MT_ERC_10_VOLT_EXCP
 * 
{*///
// 用户侧
typedef enum
{
    MT_VOLT_EXCP_UNKOWN,     // 未知 备用
    MT_VOLT_EXCP_DROP,       // 断相
    MT_VOLT_EXCP_LOSS,       // 失压
        
}eMtVoltExcp;

typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_10_VOLT_EXCP
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    eMtVoltExcp    eExcp;          // 异常类型
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bPhaseA;        // A 相是否发生异常
    BOOL           bPhaseB;        // B 相是否发生异常
    BOOL           bPhaseC;        // C 相是否发生异常
    float          fUa;            // 发生时的Ua/Uab    (+)XXX.X
    float          fUb;            // 发生时的Ub        (+)XXX.X
    float          fUc;            // 发生时的Uc/Ucb    (+)XXX.X
    float          fIa;            // 发生时的Ia        (+/-)XXX.XXX
    float          fIb;            // 发生时的Ib        (+/-)XXX.XXX
    float          fIc;            // 发生时的Ic        (+/-)XXX.XXX
    double         dPower;         // 发生时电能表正向有功总电能示值     (+)XXXXXX.XXXX
    
}sMtRec10, sMtRecVoltExcp;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 10
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 异常标志 
    sMtFmt07_f     fUa;
    sMtFmt07_f     fUb;
    sMtFmt07_f     fUc;
    sMtFmt25_f     fIa;
    sMtFmt25_f     fIb;
    sMtFmt25_f     fIc;
    sMtFmt14_f     dPower;
    
}sMtRec10_f, sMtRecVoltExcp_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_10(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC11：相序异常
 * 对应类型: MT_ERC_11_PHASE_EXCP
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_9_ELEC_EXCP
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    float          fUa;            // ∠Ua/Uab （单位：度）   (+/-)XXX.X
    float          fUb;            // ∠Ub     （单位：度）   (+/-)XXX.X
    float          fUc;            // ∠Uc/Ucb （单位：度）   (+/-)XXX.X
    float          fIa;            // ∠Ia （单位：度）       (+/-)XXX.X
    float          fIb;            // ∠Ib （单位：度）       (+/-)XXX.X
    float          fIc;            // ∠Ic （单位：度）       (+/-)XXX.X
    double         dPower;         // 发生时电能表正向有功总电能示值     (+)XXXXXX.XXXX

}sMtRec11, sMtRecPhaseExcp;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 9
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    sMtFmt05_f     fUa;            // ∠Ua/Uab （单位：度）   (+/-)XXX.X
    sMtFmt05_f     fUb;            // ∠Ub     （单位：度）   (+/-)XXX.X
    sMtFmt05_f     fUc;            // ∠Uc/Ucb （单位：度）   (+/-)XXX.X
    sMtFmt05_f     fIa;            // ∠Ia （单位：度）       (+/-)XXX.X
    sMtFmt05_f     fIb;            // ∠Ib （单位：度）       (+/-)XXX.X
    sMtFmt05_f     fIc;            // ∠Ic （单位：度）       (+/-)XXX.X
    sMtFmt14_f     dPower;
 
}sMtRec11_f, sMtRecPhaseExcp_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_11(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC12：电能表时间超差
 * 对应类型: MT_ERC_12_METER_TIME
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_9_ELEC_EXCP
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    
}sMtRec12, sMtRecMeterTime;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 9
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    
}sMtRec12_f, sMtRecMeterTime_f;
#pragma pack(0) 

// 转换函数
eMtErr emtTrans_rec_12(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC13：电表故障信息
 * 对应类型: MT_ERC_13_METER_FAULT
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_13_METER_FAULT
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bDemand;        // 是否 电能表编程次数或最大需量清零次数发生变化 
    BOOL           bDrop;          // 是否 电能表断相次数变化 
    BOOL           bLoss;          // 是否 电能表失压次数变化 
    BOOL           bOff;           // 是否 电能表停电次数变化 
    BOOL           bLess;          // 是否 电能表电池欠压

}sMtRec13, sMtRecMeterFault;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 13
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 异常标志 
    
}sMtRec13_f, sMtRecMeterFault_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_13(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC14：终端停/上电事件
 * 对应类型: MT_ERC_14_TML_ON_OFF
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_14_TML_ON_OFF
    sMtYYMMDDhhmm  sTime;          // 停电发生时间：分时日月年
    sMtYYMMDDhhmm  sTimeGo;        // 上电时间：分时日月年
    
}sMtRec14, sMtRecTmlOnOff;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 14
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 停电发生时间：分时日月年
    sMtFmt15_f     sTimeGo;        // 上电时间：分时日月年
    
}sMtRec14_f, sMtRecTmlOnOff_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_14(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC15：谐波越限告警
 * 对应类型: MT_ERC_15_HARM_OVER
 * 
{*///
typedef enum
{
    MT_OVER_LIMIT_V,   // 谐波电压越限事件
    MT_OVER_LIMIT_E,   // 谐波电流越限事件
    
}eMtOverLimit;

typedef struct
{
    BOOL    bOver;       // 是否越限
    float   fValue;      // 越限时该相(2 ~ 19)次谐波电压含有率（％）(+/-)XXX.X  或  电流有效值(A) (+/-)XX.XX
}sMtOver;

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_15_HARM_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    eMtOverLimit   eOver;          // 谐波越限事件类型
    BOOL           bA;             // 表示Uab/Ua 或 Iab/Ia 发生异常
    BOOL           bB;             // 表示Ub     或 Ib     发生异常
    BOOL           bC;             // 表示Ucb/Uc 或 Icb/Ic 发生异常
    sMtOver        sTotal;         // 越限时总畸变      电压含有率（％）/电流有效值
    sMtOver        sHarm2;         // 越限时该相2 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm3;         // 越限时该相3 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm4;         // 越限时该相4 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm5;         // 越限时该相5 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm6;         // 越限时该相6 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm7;         // 越限时该相7 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm8;         // 越限时该相8 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm9;         // 越限时该相9 次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm10;        // 越限时该相10次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm11;        // 越限时该相11次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm12;        // 越限时该相12次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm13;        // 越限时该相13次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm14;        // 越限时该相14次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm15;        // 越限时该相15次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm16;        // 越限时该相16次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm17;        // 越限时该相17次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm18;        // 越限时该相18次谐波电压含有率（％）/电流有效值
    sMtOver        sHarm19;        // 越限时该相19次谐波电压含有率（％）/电流有效值
    
}sMtRec15, sMtRecHarmOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 15
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 异常标志
    UINT8          ucOver[3];      // 谐波越限标志
    UINT16         usHarm[20];     // 越限时(总 ~ 19次)畸变电压含有率（％）/电流有效值
    
}sMtRec15_f, sMtRecHarmOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_15(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC16：直流模拟量越限记录
 * 对应类型: MT_ERC_16_DC_OVER
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_16_DC_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bOverMax;       // 上限 是否 发生或恢复越限
    BOOL           bOverMin;       // 下限 是否 发生或恢复越限
    float          fDcValue;       // 越限时直流模拟量数据   (+/-)XXX  10(-3) ~ 10(4) 
}sMtRec16, sMtRecDcOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 16
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 越限标志
    sMtFmt02_f     fDcValue;       // 越限时直流模拟量数据 
}sMtRec16_f, sMtRecDcOver_F;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_16(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC17：电压/电流不平衡度越限记录
 * 对应类型: MT_ERC_17_UNBL_OVER
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_17_UNBL_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bOverU;         // 是否 电压不平衡度越限
    BOOL           bOverI;         // 是否 电流不平衡度越限
    float          fUnbU;          // 发生时的电压不平衡度（％）  (+/-)XXX.X
    float          fUnbI;          // 发生时的电流不平衡度（％）  (+/-)XXX.X
    float          fUa;            // 发生时的Ua/Uab
    float          fUb;            // 发生时的Ub
    float          fUc;            // 发生时的Ub
    float          fIa;            // 发生时的Ia
    float          fIb;            // 发生时的Ib
    float          fIc;            // 发生时的Ic
   
}sMtRec17, sMtRecUnblOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 17
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 越限标志
    sMtFmt05_f     fUnbU;
    sMtFmt05_f     fUnbI;
    sMtFmt07_f     fUa;
    sMtFmt07_f     fUb;
    sMtFmt07_f     fUc;
    sMtFmt25_f     fIa;
    sMtFmt25_f     fIb;
    sMtFmt25_f     fIc;
    
}sMtRec17_f, sMtRecUnblOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_17(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC18：电容器投切自锁记录
 * 对应类型: MT_ERC_18_CAPA_LOCK
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_18_CAPA_LOCK
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bVoltOver;      // 自锁原因 - 过压
    BOOL           bDevFault;      // 自锁原因 - 装置故障
    BOOL           bCirFault;      // 自锁原因 - 执行回路故障
    BOOL           bCacpClock[16]; // 表示第1～16组电容器组，置"1"：该电容器组自锁。置"0"：该电容器组未自锁。
    float          fFacter;        //  越限发生时功率因数 (+/-)XXX.X
    float          fPowerNone;     // 越限发生时无功功率  (+)XX.XXXX
    float          fVolt;          // 越限发生时电压      (+)XXX.X  (V)
}sMtRec18, sMtRecCapaLock;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 18
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 越限标志
    UINT16         usCacp;         // 电容器组标志
    sMtFmt05_f     fFacter;        // 越限发生时功率因数
    sMtFmt23_f     fPowerNone;     // 越限发生时无功功率
    sMtFmt07_f     fVolt;          // 越限发生时电压
    
}sMtRec18_f, sMtRecCapaLock_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_18(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC19：购电参数设置记录
 * 对应类型: MT_ERC_19_BUY_PARA
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_19_BUY_PARA
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT8          ucPn;           // pn（总加组号）
    UINT32         ulID;           // 购电单号
    BOOL           bAppend;        // 真:追加 假:刷新
    sMtMoney       sBuyLimit;      // 购电量值
    sMtMoney       sWarning;       // 报警门限
    sMtMoney       sTurnOff;       // 跳闸门限
    sMtMoney       sBuyBefore;     // 本次购电前剩余电能量（费）
    sMtMoney       sButAfter;      // 本次购电后剩余电能量（费）
    
}sMtRec19, sMtRecBuyPara;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 19
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT8          ucPn;           // pn（总加组号）
    UINT32         ulID;           // 购电单号
    UINT8          bAppend;        // 55H:追加 AAH:刷新
    sMtFmt03_f     sBuyLimit;      // 购电量值
    sMtFmt03_f     sWarning;       // 报警门限
    sMtFmt03_f     sTurnOff;       // 跳闸门限
    sMtFmt03_f     sBuyBefore;     // 本次购电前剩余电能量（费）
    sMtFmt03_f     sButAfter;      // 本次购电后剩余电能量（费）
    
}sMtRec19_f, sMtRecBuyPara_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_19(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC20：消息认证错误记录
 * 对应类型: MT_ERC_20_AUTH_ERROR
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_20_AUTH_ERROR
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT8          acPW[16];       // 消息认证码PW
    UINT8          ucMSA;          // 启动站地址MSA
}sMtRec20, sMtRecAuthError;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 20
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT8          acPW[16];       // 消息认证码PW
    UINT8          ucMSA;          // 启动站地址MSA
    
}sMtRec20_f, sMtRecAuthError_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_20(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC21：终端故障记录
 * 对应类型: MT_ERC_21_TML_FAULT
 * 
{*///

typedef enum
{
    MT_TMFT_UNKOWN,          // 终端未知故障
    MT_TMFT_MEMORY,          // 终端主板内存故障
    MT_TMFT_CLOCK,           // 时钟故障
    MT_TMFT_COMM,            // 主板通讯故障
    MT_TMFT_485,             // 485抄表故障
    MT_TMFT_DISPLAY,         // 显示板故障
    MT_TMFT_CARRIER,         // 载波通道异常
 
}eMtTmlFault;                // 终端故障类型


// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_21_TML_FAULT
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    eMtTmlFault    eFault;         // 终端故障编码
    
}sMtRec21, sMtRecTmlFault;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 21
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT8          eFault;         // 终端故障编码
    
}sMtRec21_f, sMtRecTmlFault_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_21(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC22：有功总电能量差动越限事件记录
 * 对应类型: MT_ERC_22_HAVE_OVER
 * 
{*///

typedef struct
{
   double dCompr;            // 越限时对比总加组第(1~64)测量点有功总电能示值 XXXXXX.XXXX
   double dRefer;            // 越限时参照总加组第(1~64)测量点有功总电能示值 XXXXXX.XXXX
   
}sMtPowerCompare;            // 测量点有功总电能示值 (对比 及 参照)


// 用户侧
typedef struct
{ 
    eMtEventRecord   eRec;           // = MT_ERC_22_HAVE_OVER
    sMtYYMMDDhhmm    sTime;          // 发生时间：分时日月年
    UINT8            ucTeam;         // 电能量差动组号(0~63)
    BOOL             bStart;         // 起止标志
    sMtFmt_sX7       sTotalCompr;    // 越限时对比总加组有功总电能量
    sMtFmt_sX7       sTotalRefer;    // 越限时参照总加组有功总电能量
    UINT8            ucDifRelative;  // 越限时差动越限相对偏差值
    sMtFmt_sX7       sDifAbsolut;    // 越限时差动越限绝对偏差值
    UINT8            ucN;            // 对比的总加组测量点数量n（1≤n≤64）
    sMtPowerCompare  sPower[1];      // ucN 个
    
}sMtRec22, sMtRecHaveOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 22
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT8          ucTeamStart;    // 
    sMtFmt03_f     sTotalCompr;    // 越限时对比总加组有功总电能量
    sMtFmt03_f     sTotalRefer;    // 越限时参照总加组有功总电能量
    UINT8          ucDifRelative;  // 越限时差动越限相对偏差值
    sMtFmt03_f     sDifAbsolut;    // 越限时差动越限绝对偏差值

    UINT8          ucN;            // 对比的总加组测量点数量n（1≤n≤64）
    UINT8          ucTmp;          // 

    /*
    UINT8          ucN;            // 对比的总加组测量点数量n（1≤n≤64）
    sMtFmt15_f     dPowerCompr;    // 越限时对比总加组第1~N测量点有功总电能示值
    UINT8          ucM;            // 参照的总加组测量点数量m（1≤n≤64）
    sMtFmt15_f     dPowerRefer;    // 越限时参照总加组第1~M测量点有功总电能示值
    
    */
    
}sMtRec22_f, sMtRecHaveOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_22(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC23：电控告警事件记录
 * 对应类型: MT_ERC_23_ECTRL_WARN
 * 
{*///

typedef enum
{
    NT_ECTRL_UNKOWN,    // 未知电控
    MT_ECTRL_MONTH,     // 月电控
    MT_ECTRL_BUY,       // 购电控

}eMtElceCtrl;           // 电控类别


// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_23_ECTRL_WARN
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT8          ucPN;           // pn（总加组号） (0 ~ 63)
    BOOL           bCtrl[8];       // 按位对应表示第1～第8个控制输出轮次，置"1"：表示该轮次投入，置"0"：表示该轮次未投入
    eMtElceCtrl    eType;          // 电控类别
    sMtFmt_sX7     sPowerWarning;  // 告警时电能量
    sMtFmt_sX7     sCtrlFixed;     // 告警时电控定值
    
}sMtRec23, sMtRecECtrlWarn;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 23
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT8          ucPN;           // pn（总加组号） (0 ~ 63)
    UINT8          bCtrl;          // 按位对应表示第1～第8个控制输出轮次，置"1"：表示该轮次投入，置"0"：表示该轮次未投入
    UINT8          eType;          // 电控类别
    sMtFmt03_f     sPowerWarning;  // 告警时电能量
    sMtFmt03_f     sCtrlFixed;     // 告警时电控定值
 
}sMtRec23_f, sMtRecECtrlWarn_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_23(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC24：电压越限记录
 * 对应类型: MT_ERC_24_VOLT_OVER
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_24_VOLT_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bPhaseA;        // A相发生越限
    BOOL           bPhaseB;        // B相发生越限
    BOOL           bPhaseC;        // C相发生越限
    BOOL           bOverUpUp;      // 真:为越上上限 假:为越下下限
    float          fUa;            // 发生时的Ua/Uab  (+)XXX.X
    float          fUb;            // 发生时的Ub      (+)XXX.X
    float          fUc;            // 发生时的Ub      (+)XXX.X
}sMtRec24, sMtRecVoltOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 24
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 越限标志
    sMtFmt07_f     fUa;
    sMtFmt07_f     fUb;
    sMtFmt07_f     fUc;
}sMtRec24_f, sMtRecVoltOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_24(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC25：电流越限记录
 * 对应类型: MT_ERC_25_ELEC_OVER
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_25_ELEC_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bPhaseA;        // A相发生越限
    BOOL           bPhaseB;        // B相发生越限
    BOOL           bPhaseC;        // C相发生越限
    BOOL           bOverUpUp;      // 真:为越上上限 假:为越上限
    float          fIa;            // 发生时的Ia  (+/-)XXX.XXX
    float          fIb;            // 发生时的Ib  (+/-)XXX.XXX
    float          fIc;            // 发生时的Ic  (+/-)XXX.XXX
   
}sMtRec25, sMtRecElecOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 25
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 越限标志
    sMtFmt25_f     fIa;
    sMtFmt25_f     fIb;
    sMtFmt25_f     fIc;
    
}sMtRec25_f, sMtRecElecOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_25(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC26：视在功率越限记录
 * 对应类型: MT_ERC_26_SP_OVER
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_26_SP_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bOverUpUp;      // 真:为越视在功率上上限 假:为越视在功率上限
    float          fSPower;        // 发生时的视在功率         (+)XX.XXXX
    float          fSPwrLimit;     // 发生时的视在功率限值     (+)XX.XXXX

}sMtRec26, sMtRecSpOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 26
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 越限标志
    sMtFmt23_f     fSPower;        // 发生时的视在功率
    sMtFmt23_f     fSPwrLimit;     // 发生时的视在功率限值

}sMtRec26_f, sMtRecSpOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_26(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC27：电能表示度下降记录
 * 对应类型: MT_ERC_27_POWER_DOWN
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_27_POWER_DOWN
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    double         dPowerBefor;    // 下降前电能表正向有功总电能示值 (+)XXXXXX.XXXX
    double         dPowerAfter;    // 下降后电能表正向有功总电能示值 (+)XXXXXX.XXXX
    
}sMtRec27, sMtRecPowerDown;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 27
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    sMtFmt14_f     dPowerBefor;    // 下降前电能表正向有功总电能示值
    sMtFmt14_f     dPowerAfter;    // 下降后电能表正向有功总电能示值
    
}sMtRec27_f, sMtRecPowerDown_F;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_27(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC28：电能量超差记录
 * 对应类型: MT_ERC_28_POWER_OVER
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_28_POWER_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    double         dPowerCmpr;     // 电能量超差发生时对应正向有功总电能示值 (+)XXXXXX.XXXX
    double         dPower;         // 电能量超差发生时正向有功总电能示值     (+)XXXXXX.XXXX
    float          fLimit;         // 电能量超差阈值                         (+)X_X
    
}sMtRec28, sMtRecPowerOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 28
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    sMtFmt14_f     dPowerCmpr;     // 电能量超差发生时对应正向有功总电能示值
    sMtFmt14_f     dPower;         // 电能量超差发生时正向有功总电能示值
    sMtFmt22_f     fLimit;         // 电能量超差阈值
    
}sMtRec28_f, sMtRecPowerOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_28(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC29：电能表飞走记录
 * 对应类型: MT_ERC_29_METER_FLY
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_29_METER_FLY
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    double         dPowerBefor;    // 电能表飞走发生前正向有功总电能示值  (+)XXXXXX.XXXX
    double         dPowerAfter;    // 电能表飞走发生后正向有功总电能示值  (+)XXXXXX.XXXX 
    float          fLimit;         // 电能表飞走阈值                      (+)X_X
    
}sMtRec29, sMtRecMeterFly;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 29
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    sMtFmt14_f     dPowerBefor;    // 电能表飞走发生前正向有功总电能示值
    sMtFmt14_f     dPowerAfter;    // 电能表飞走发生后正向有功总电能示值
    sMtFmt22_f     fLimit;         // 电能表飞走阈值

}sMtRec29_f, sMtRecMeterFly_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_29(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC30：电能表停走记录
 * 对应类型: MT_ERC_30_METER_STOP
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_30_METER_STOP
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    double         dPower;         // 电能表停走发生时正向有功总电能示值  (+)XXXXXX.XXXX 
    UINT8          ucLimit;        // 电能表停走阈值
    
}sMtRec30, sMtRecMeterStop;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 30
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    sMtFmt14_f     dPower;         // 电能表停走发生时正向有功总电能示值
    UINT8          ucLimit;        // 电能表停走阈值
    
}sMtRec30_f, sMtRecMeterStop_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_30(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC31：终端485抄表失败事件记录
 * 对应类型: MT_ERC_31_READ_FAULT
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_31_READ_FAULT
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    double         dPower;         // 电能表停走发生时正向有功总电能示值  (+)XXXXXX.XXXX 
    sMtYYMMDDhhmm  sReadTime;      // 最近一次抄表成功时间：分时日月年   
    double         dHavePower;     // 最近一次抄表成功正向有功总电能示值
    double         dNonePower;     // 最近一次抄表成功正向无功总电能示值 (+)XXXXXX.XX

}sMtRec31, sMtRecReadFault;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 31
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    sMtFmt15_f     sReadTime;      // 最近一次抄表成功时间：分时日月年   
    sMtFmt14_f     dHavePower;     // 最近一次抄表成功正向有功总电能示值
    sMtFmt11_f     dNonePower;     // 最近一次抄表成功正向无功总电能示值 (+)XXXXXX.XX
  
}sMtRec31_f, sMtRecReadFault_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_31(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}


/*******************************************
 * 数据结构: ERC32：终端与主站通信流量超门限事件记录
 * 对应类型: MT_ERC_32_FLOW_OVER
 * 
{*///
// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_32_FLOW_OVER
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT32         ulMonth;        // 当月已发生的通信流量
    UINT32         ulLimit;        // 月通信流量门限
    
}sMtRec32, sMtRecFlowOver;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 32
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT32         ulMonth;        // 当月已发生的通信流量
    UINT32         ulLimit;        // 月通信流量门限
    
}sMtRec32_f, sMtRecFlowOver_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_32(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC33：电能表运行状态字变位事件记录
 * 对应类型: MT_ERC_33_METER_WORD
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_9_ELEC_EXCP
    sMtYYMMDDhhmm  sTime;          // MT_ERC_33_METER_WORD
    UINT16         usPn;           // pn（测量点号1～2048）

}sMtRec33, sMtRecMeterWord;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 33
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT16         usFlag[7];      // 电表运行状态字变位标志1 ~ 7
    UINT16         uState[7];      // 电表运行状态字1~7
}sMtRec33_f, sMtRecMeterWord_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_33(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC34：CT异常事件记录
 * 对应类型: MT_ERC_34_CT_EXCP
 * 
{*///

typedef enum
{
    MT_CLFT_UNKOWN,
    MT_CLFT_SHORT_1,
    MT_CLFT_SHORT_2,
    MT_CLFT_OPEN,

}eMtCircleFault;

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_34_CT_EXCP
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    UINT16         usPn;           // pn（测量点号1～2048）
    BOOL           bStart;         // 起/止标志：置"1"：发生，置"0"：恢复。
    BOOL           bPhaseA;        // A相是否有发生该事件
    BOOL           bPhaseB;        // B相是否有发生该事件
    BOOL           bPhaseC;        // C相是否有发生该事件
    eMtCircleFault eFault;         // 异常类型，取值范围1～3，依次表示一次侧短路、二次侧短路、二次侧开路。
    
}sMtRec34, sMtRecCtExcp;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 34
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
    UINT16         usPnFlag;       // Pn 及标志
    UINT8          ucExcp;         // 异常标志

}sMtRec34_f, sMtRecCtExcp_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_34(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}

/*******************************************
 * 数据结构: ERC35：发现未知电表事件记录
 * 对应类型: MT_ERC_35_UNKOWN
 * 
{*///

// 用户侧
typedef struct
{ 
    eMtEventRecord eRec;           // = MT_ERC_35_UNKOWN
    sMtYYMMDDhhmm  sTime;          // 发生时间：分时日月年
    
}sMtRec35, sMtRecUnkown;

// 帧侧
#pragma pack(1) 
typedef struct
{
    UINT8          ucRec;          // ERC = 35
    UINT8          ucLe;           // 长度Le
    sMtFmt15_f     sTime;          // 发生时间：分时日月年
   
}sMtRec35_f, sMtRecUnkown_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_rec_35(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
///*}
///*}

/*******************************************
 * 数据结构: F1：请求重要事件
 * 对应AFN : AFN_0E_ASK3
 * 对应CMD : CMD_AFN_E_F1_EVENT_1
 * PN 类型 : P0
 * 规则说明:
 *     所请求的事件记录个数Y满足下列计算公式：
 *     (1)当Pm＜Pn时，Y= Pn-Pm	
 *     (2)当Pm＞Pn时，Y= 256＋Pn-Pm
 *   
{*///

// 命令参数
// 用户侧
typedef struct
{
    UINT8 ucPm;   // 请求事件记录起始指针Pm
    UINT8 ucPn;   // 请求事件记录结束指针Pn

}sMtAskEvent1, sMtAskEvent1_f;

// 应答结构

typedef union
{
    sMtRec1    sRecInit;          // ERC1 ：数据初始化和版本变更记录
    sMtRec2    sRecParaLoss;      // ERC2 ：参数丢失记录
    sMtRec3    sRecParaModify;    // ERC3 ：参数变更记录
    sMtRec4    sRecStateChange;   // ERC4 ：状态量变位记录
    sMtRec5    sRecRemoteSwitch;  // ERC5 ：遥控跳闸记录
    sMtRec6    sRecPCtrlSwitch;   // ERC6 ：功控跳闸记录
    sMtRec7    sRecECtrlSwitch;   // ERC7 ：电控跳闸记录
    sMtRec8    sRecMeterChange;   // ERC8 ：电能表参数变更
    sMtRec9    sRecElecExcp;      // ERC9 ：电流回路异常
    sMtRec10   sRecVoltExcp;      // ERC10：电压回路异常
    sMtRec11   sRecPhaseExcp;     // ERC11：相序异常
    sMtRec12   sRecMeterTime;     // ERC12：电能表时间超差
    sMtRec13   sRecMeterFault;    // ERC13：电表故障信息
    sMtRec14   sRecTmlOnOff;      // ERC14：终端停/上电事件
    sMtRec15   sRecHarmOver;      // ERC15：谐波越限告警
    sMtRec16   sRecDcOver;        // ERC16：直流模拟量越限记录
    sMtRec17   sRecUnblOver;      // ERC17：电压/电流不平衡度越限记录 
    sMtRec18   sRecCapaLock;      // ERC18：电容器投切自锁记录
    sMtRec19   sRecBuyPara;       // ERC19：购电参数设置记录
    sMtRec20   sRecAuthError;     // ERC20：消息认证错误记录
    sMtRec21   sRecTmlFault;      // ERC21：终端故障记录
    sMtRec22   sRecHaveOver;      // ERC22：有功总电能量差动越限事件记录
    sMtRec23   sRecECtrlWarn;     // ERC23：电控告警事件记录
    sMtRec24   sRecVoltOver;      // ERC24：电压越限记录
    sMtRec25   sRecElecOver;      // ERC25：电流越限记录
    sMtRec26   sRecSpOver;        // ERC26：视在功率越限记录
    sMtRec27   sRecPowerDown;     // ERC27：电能表示度下降记录
    sMtRec28   sRecPowerOver;     // ERC28：电能量超差记录
    sMtRec29   sRecMeterFly;      // ERC29：电能表飞走记录
    sMtRec30   sRecMeterStop;     // ERC30：电能表停走记录
    sMtRec31   sRecReadFault;     // ERC31：终端485抄表失败事件记录
    sMtRec32   sRecFlowOver;      // ERC32：终端与主站通信流量超门限事件记录
    sMtRec33   sRecMeterWord;     // ERC33：电能表运行状态字变位事件记录
    sMtRec34   sRecCtExcp;        // ERC34：CT异常事件记录
    sMtRec35   sRecUnkown;        // ERC35：发现未知电表事件记录

}uMtRec;    // 事件记录数据格式

// 用户侧
typedef struct
{
    UINT8  ucEC1;    // 当前重要事件计数器EC1
    UINT8  ucEC2;    // 当前一般事件计数器EC2
    UINT8  ucPm;     // 本帧报文传送的事件记录起始指针Pm
    UINT8  ucPn;     // 本帧报文传送的事件记录结束指针Pn
    uMtRec uRec[1];  // Y 个事件记录信息
                     // * (1)当Pm＜Pn时，Y= Pn-Pm	
                     // * (2)当Pm＞Pn时，Y= 256＋Pn-Pm
}sMtResEvent1;


// 帧侧数据结构
#pragma pack(1) 
typedef struct
{
    UINT8  ucEC1;    // 当前重要事件计数器EC1
    UINT8  ucEC2;    // 当前一般事件计数器EC2
    UINT8  ucPm;     // 本帧报文传送的事件记录起始指针Pm
    UINT8  ucPn;     // 本帧报文传送的事件记录结束指针Pn
    UINT8  uRec[1];  // Y 个事件记录信息
                     // * (1)当Pm＜Pn时，Y= Pn-Pm	
                     // * (2)当Pm＞Pn时，Y= 256＋Pn-Pm
}sMtResEvent1_f;
#pragma pack() 

// 转换函数
eMtErr emtTrans_afn0ef1_m2s(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
eMtErr emtTrans_afn0ef1_s2m(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);

// 辅助函数
pMtFunc pMtGetRecTransFunc(UINT8 ucRecID);
///*}

/*******************************************
 * 数据结构: F2：请求一般事件
 * 对应AFN : AFN_0E_ASK3
 * 对应CMD : CMD_AFN_E_F2_EVENT_2
 * PN 类型 : P0
 * 规则说明:
 *     所请求的事件记录个数Y满足下列计算公式：
 *     (1)当Pm＜Pn时，Y= Pn-Pm	
 *     (2)当Pm＞Pn时，Y= 256＋Pn-Pm
 *   
{*///

// 命令参数
// 用户侧
typedef sMtAskEvent1 sMtAskEvent2;

// 帧侧
typedef sMtAskEvent1_f sMtAskEvent2_f;

// 应答结构
// 用户侧
typedef sMtResEvent1 sMtResEvent2;

// 帧侧数据结构
typedef sMtResEvent1_f sMtResEvent2_f;

// 转换函数
eMtErr emtTrans_afn0ef2_m2s(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);
eMtErr emtTrans_afn0ef2_s2m(eMtTrans eTrans, void* psUser, void* psFrame, UINT16* pusfLen);

///*}


/*********************************************
 *  BCD 转换函数
 *  
 *  
{*///
UINT8 ucmt_get_bcd_0(UINT8 ucData);        // 获得该数的BCD低位
UINT8 ucmt_get_bcd_1(UINT8 ucData);        // 获得该数的BCD高位
///*} 

/***************************************************************************
 * BCD 与 字符串的转换函数
 * len 必须为偶数
 *
{*///
eMtErr emt_str_to_bcd(const UINT8* pstr, UINT8* pbcd, INT32 len); 
eMtErr emt_bcd_to_str(const UINT8* pbcd, UINT8* pstr, INT32 len);
///*}

/***************************************************************************
 * 字节序转换函数
 * 低字节在前，高字节在后
 *
{*///
// 判断主机序是否为大字节序
BOOL   bmt_big_endian();

// 将一个UINT16数值字小字节序写入地址pWrite
eMtErr emt_write_uint16_small_endian(UINT16 usData, UINT8* pWrite);  

// 将一个UINT32数值字小字节序写入地址pWrite
eMtErr emt_write_uint32_small_endian(UINT32 ulData, UINT8* pWrite); 

// 从指定地址位置以小字节序的方式读入一个整数
eMtErr emt_read_uint16_small_endian(UINT8* pRead, UINT16* pUint16);

// 从指定地址位置以小字节序的方式读入一个长整数
eMtErr emt_read_uint32_small_endian(UINT8* pRead, UINT32* pUint32);  
///*}

/***************************************************************************
 *  数据单元 应用层数据结构定义
 *  上行与下行同类型的数据结构有时会一不样
 *  
 *  终端在响应主站对终端的参数或数据请求时,
 *    如终端没有所需的某个数据项,则将应答报文中DT的对应标志位清除；
 *    如终端仅是没有某个数据项中的部分内容，则应将该数据项中的所缺部分
 *    内容的每个字节填写“EEH”。
 *    注意:在扩展应用层数据单元时,需要在每个定义中为每个数据项加上 
 *         有/无的标识 可以用一个BOOL 来表示 
 *  
{*///
typedef union
{
  /***************************************
     *  上下行通用
     *  主站到从站 和 从站到主站
     *  双向同样的数据结构
    {*///
    sMtOnebyOne      sOneByOne;        // 逐个确认或否认                  CMD_AFN_0_F3_ONE_BY_ONE
    sMtTmlUpCfg      sTmlUpCfg;        // 终端上行通信口通信参数设置      CMD_AFN_4_F1_TML_UP_CFG          CMD_AFN_A_F1_TML_UP_CFG
    sMtTmlPowerCfg   sTmlPowerCfg;     // 终端电能表/交流采样装置配置参数 CMD_AFN_A_F10_TML_POWER_CFG
    float            fTmlSafeValue;    // 终端保安定值                    CMD_AFN_A_F17_TML_SAFE_VALUE
    sMtFloat         sTmlPCtrlFactor;  // 终端时段功控定值浮动系数        CMD_AFN_4_F19_TML_PCTRL_FACTOR   CMD_AFN_A_F19_TML_PCTRL_FACTOR
    sMtFloat         sTmlMonthFactor;  // 终端月电能量控定值浮动系数      CMD_AFN_4_F20_TML_MONTH_FACTOR   CMD_AFN_A_F20_TML_MONTH_FACTOR
    sMtAsk1Cfg       sAsk1Cfg;         // 1类数据配置设置                 CMD_AFN_4_F38_CFG_ASK_1          CMD_AFN_A_F38_CFG_ASK_1
    sMtAsk2Cfg       sAsk2Cfg;         // 2类数据配置设置                 CMD_AFN_4_F39_CFG_ASK_2          CMD_AFN_A_F39_CFG_ASK_2
    sMtCfgAuto_1     sCfgAuto_1;       // 定时上报1类数据任务设置         CMD_AFN_4_F65_CFG_AUTO_1         CMD_AFN_A_F65_CFG_AUTO_1
    sMtCfgAuto_2     sCfgAuto_2;       // 定时上报2类数据任务设置         CMD_AFN_4_F66_CFG_AUTO_2         CMD_AFN_A_F66_CFG_AUTO_2
    sMtGopAuto_1     sGopAuto_1;       // 定时上报1类数据任务启动/停止设置CMD_AFN_4_F67_GOP_AUTO_1         CMD_AFN_A_F67_GOP_AUTO_1
    sMtGopAuto_2     sGopAuto_2;       // 定时上报2类数据任务启动/停止设置CMD_AFN_4_F68_GOP_AUTO_2         CMD_AFN_A_F68_GOP_AUTO_2
    ///*}



    /***************************************
     *  上行
     *  从站到主站
     *  通常是应答 响应
    {*///
    sMtCurPQUI       sCurPQUI;           // 当前三相及总有/无功功率、功率因数，三相电压、电流、零序电流、视在功率 CMD_AFN_C_F25_POWER_RATE_CUR
    sMtTmlVersion    sTmlVersion;        // 终端版本信息  CMD_AFN_9_F1_TML_VERSION
    sMtTmlInputOut   sTmlInputOut;       // 终端支持的输入、输出及通信端口配置 CMD_AFN_9_F2_TML_INPUT_OUT
    sMtTmlOtherCfg   sTmlOtherCfg;       // 终端支持的其他配置      CMD_AFN_9_F3_TML_OTHER_CFG
    sMtSuptParaCfg   sTmlSuptParaCfg;    // 终端支持的参数配置      CMD_AFN_9_F4_SUPPORT_PARA_CFG
    sMtSuptCtrlCfg   sTmlSuptCtrlCfg;    // 终端支持的控制配置      CMD_AFN_9_F5_SUPPORT_CTRL_CFG
    sMtSuptAsk1      sTmlSuptAsk1;       // 终端支持的1类数据配置   CMD_AFN_9_F6_SUPPORT_ASK1_CFG
    sMtSuptAsk2      sTmlSuptAsk2;       // 终端支持的2类数据配置   CMD_AFN_9_F7_SUPPORT_ASK2_CFG
    sMtSuptEvent     sTmlSuptEvent;      // 终端支持的事件记录配置  CMD_AFN_9_F8_SUPPORT_EVNT_CFG
    sMtUserClock     sTmlClock;          // 终端日历时钟            CMD_AFN_C_F2_TML_CLOCK
    sMtParaSta       sTmlParaStat;       // 终端参数状态            CMD_AFN_C_F3_TML_PARA_STATE 
    sMtTmlUpComState sTmlUpComStat;      // 终端上行通信状态        CMD_AFN_C_F4_TML_UPCOM_STATE 
    sMtAfn0cF05      sTmlCtrlStat;       // 终端控制设置状态        CMD_AFN_C_F5_TML_CTRL_SET
    sMtAfn0cF06      sTmlCurCtrlStat;    // 终端控制设置状态        CMD_AFN_C_F6_TML_CTRL_STATE
    sMtAfn0cF07      sTmlEcValue;        // 终端控制设置状态        CMD_AFN_C_F7_TML_EC_VALUE
    sMtAfn0cF08      sTmlErcSta;         // 终端时间标志状态         CMD_AFN_C_F8_TML_EVNT_FLAG
    sMtAfn0cf31      sTmlPowerCur;       // 当前A 、B 、C三相正/ 反有功电能示值、组合无功1/2 电能示值 CMD_AFN_C_F31_POWER_CUR
    sMtCurtHarmValue sTmlCurtHarmValue;  // 当前A、B、C三相电压、电流2～N次谐波有效值CMD_AFN_C_F57_CURT_HARM_VALUE
    sMtCurtHarmRate  sTmlCurtHarmRate;   // 当前A、B、C三相电压、电流2～N次谐波含有率CMD_AFN_C_F58_CURT_HARM_RATE
    sMtFrthHavePower sTmlFrthHavePowrC;  // 当前正向有功电能示值（总、费率1～M）CMD_AFN_C_F129_FRTH_HAVE_POWR_C
    sMtFrthNonePower sTmlFrthNonePowrC;  // 当前正向无功（组合无功1）电能示值（总、费率1～M）CMD_AFN_C_F130_FRTH_NONE_POWR_C
    sMtAfn0cf166     sTmlMetrModifyRecd; // 电能表参数修改次数及时间 CMD_AFN_C_F166_METR_MODIFY_RECD
    sMtAfn0cf167     sTmlMetrBuyUse;     // 电能表购、用电信息       CMD_AFN_C_F167_METR_BUY_USE
    float            fDcRltmData;        // 直流模拟量实时数据 (仅3位有效数据)   CMD_AFN_C_F73_DC_RLTM_DATA
    sMtUblnOverD     sUblnOverD;         // 日冻结日不平衡度越限累计时间   CMD_AFN_D_F28_UBLN_OVER_D
    sMtUblnOverM     sUblnOverM;         // 月冻结月不平衡度越限累计时间   CMD_AFN_D_F36_UBLN_OVER_M
    
    sMtAfn0dF97      sTmlCurvePower;     // 电能量 曲线
    sMtAfn0dF101     sTmlCurveValue;     // 电能示值 曲线
    sMtAfn0dF105     sTmlCurveFactor;    // 功率因数曲线
    sMtAfn0dF109     sTmlCurvePhase;     // 相位角曲线
    sMtAfn0dF113     sTmlIHarmTime;      // 谐波电流最大值及发生时间
    sMtAfn0dF116     sTmlUHarmTime;      // 谐波电压最大值及发生时间
    sMtAfn0dF121     sTmlHarmOver;       // 日冻结测量点谐波越限日统计   
    sMtAfn0dF169     sTmlValue;          // 正向有功电能示值    
    sMtAfn0dF170     sTmlReactiveValue;  // 无组合无功电能示值
    sMtAfn0dF171     sTmlActiveValue;    // 有功电能示值
    sMtAfn0dF177     sTmlActiveValueM;   // 有功电能示值(月冻结)
    sMtAfn0dF178     sTmlReactiveValueM; // 无组合无功电能示值(月冻结)
    sMtAfn0dF185     sTmlMaxTime;        // 最大需量及发生时间
    sMtAfn0dF215     sTmlMeterInfo;      // 电能表购、用电信息
    sMtAfn0dF216     sTmlBalanceInfo;    // 月冻结电能结算信息   
    ///*}

    /***************************************
     *  下行
     *  主站到从站
     *  通常是命令 请求
    {*///
    sMtUserClock     sCheckTime;         // 对时命令  CMD_AFN_5_F31_CHECK_TIME
    sMtTmlPowerCfg_Q sTmlPowerCfg_Q;     // 终端电能表/交流采样装置配置参数 命令参数 CMD_AFN_A_F10_TML_POWER_CFG
    sMtAsk1Cfg_Q     sAsk1Cfg_Q;         // 1类数据配置设置 （在终端支持的1类数据配置内）(查询命令参数) CMD_AFN_A_F38_CFG_ASK_1
    sMtAsk2Cfg_Q     sAsk2Cfg_Q;         // 1类数据配置设置 （在终端支持的2类数据配置内）(查询命令参数) CMD_AFN_A_F39_CFG_ASK_2
    sMtTd_c          sTd_c;              // 曲线类数据时标  命令参数
    sMtTd_h          sTd_h;              // 小时冻结 命令参数
    sMtTd_d          sTd_d;              // 日冻结   命令参数
    sMtTd_m          sTd_m;              // 月冻结   命令参数  
    ///*}

    // 仅用于占位 无应用意义
    UINT8            ucRev[MT_UNIT_DATA_MAX];
    
}uMtApp;
///*}

/***************************************
 *  帧内容数据结构 用户侧  (低级抽象)
 *  用于封装与解析
 *  变长结构
{*///

typedef struct
{
    sMtPnFn    sPnFn;
    uMtApp     *puApp[PN_INDEX_MAX][FN_INDEX_MAX];
}sMtLiteData;

typedef struct
{
    sMtAddress    sAddress;      // 地址域
    sMtCtrl       sCtrl;         // 控制域

    // 附加域
    UINT8         acPW[16];      // pw字段  消息认证码字段PW用于重要下行报文中，
                                 // 由16字节组成,PW是由主站按系统约定的认证算法产生
                                 // 并在主站发送的报文中下发给终端
                                 // 由终端进行校验认证,通过则响应主站命令,反之则否认
                                 // 终端在收到带有PW的报文,必须在认证通过后,才能响应命令。

    sMtEC         sEC;           // 事件计数器
    sMtTP         sTP;           // 时间标签
 
    // 应用层数据域  
    eMtAFN        eAFN;          // 功能码
    sMtSEQ        sSEQ;          // 帧序列域
    
    // 数据单元域  变长域
    UINT16        usDataNum;     // 数据单元组的个数
    sMtLiteData   sData[1];      // 数据单元组
}smtLitePack;

// 封帧函数
eMtErr emtLitePack(smtLitePack* psPack, UINT16* pusLen, UINT8* pOutBuf);   

// 解帧函数
eMtErr emtLiteUnPack(smtLitePack *psUnpack, UINT8* pInBuf, UINT16 usLen);
///*}

/***************************************
 *  数据单元标识与对应的数据单元 (低级抽象)
 *  
 * 数据单元为按数据单元标识所组织的数据,包括参数、命令等。
 * 数据组织的顺序规则：先按pn从小到大、再按Fn从小到大的次序,
 * 即：完成一个信息点pi的所有信息类Fn的处理后，再进行下一个pi+1的处理
 * 
{*///

typedef struct
{
    sMtPnFn    sPnFn;
    uMtApp     uApp[PN_INDEX_MAX][FN_INDEX_MAX];
}sMtBaseData;

// 根据总加组有效标志位获取总加组个数
UINT8 emtGetGroupNum(UINT8 ucGroupFlag);

// 判断最8个Pn是否在同一个信息点组,同时取得所属的信息点组
BOOL bmt_same_team_pn(UINT16 *pUsPn8, UINT8* pucDa2);

// 判断一组Pn是否都为0
BOOL bmt_is_p0(UINT16 *pUsPn8);

// 获得pn对应的信息点组
UINT8 ucmt_get_pn_team(UINT16 usPn); 

// 获得一组pn对应的信息点组
UINT8 ucmt_get_pn8_team(UINT16 *pusPn); 

// 判断某一个pn是否在一组8个pn中
BOOL bmt_in_pn8(UINT16 usPn, UINT16 *pusPn8);

// 判断最8个Fn是否在同一个信息类组,同时取得所属的信息类组
BOOL bmt_same_team_fn(UINT8  *pUcFn8, UINT8* pucDt2);

// 获得Fn对应的信息点组
UINT8 ucmt_get_fn_team(UINT8 ucFn); 

// 获得Fn对应的信息点在某组的bit
UINT8 ucmt_get_fn_bit(UINT8 ucFn);

// 获得一组Fn对应的信息点组
UINT8 ucmt_get_fn8_team(UINT8 *pucFn); 

// 判断某一个fn是否在一组8个fn中
BOOL bmt_in_fn8(UINT8 ucFn, UINT8 *pucFn8);

///*}

/***************************************
 *  帧内容数据结构 用户侧  (低级抽象)
 *  用于封装与解析
 *  变长结构
{*///
typedef struct
{
    sMtAddress    sAddress;      // 地址域
    sMtCtrl       sCtrl;         // 控制域

    // 附加域
    UINT8         acPW[16];      // pw字段  消息认证码字段PW用于重要下行报文中，
                                 // 由16字节组成,PW是由主站按系统约定的认证算法产生
                                 // 并在主站发送的报文中下发给终端
                                 // 由终端进行校验认证,通过则响应主站命令,反之则否认
                                 // 终端在收到带有PW的报文,必须在认证通过后,才能响应命令。

    sMtEC         sEC;           // 事件计数器
    sMtTP         sTP;           // 时间标签
 
    // 应用层数据域  
    eMtAFN        eAFN;          // 功能码
    sMtSEQ        sSEQ;          // 帧序列域
    
    // 数据单元域  变长域
    UINT16        usDataNum;     // 数据单元组的个数
    sMtBaseData   sData[1];      // 数据单元组
}smtBasePack;

// 封帧函数
eMtErr emtBasePack(smtBasePack* psPack, UINT16* pusLen, UINT8* pOutBuf);   

// 解帧函数
eMtErr emtBaseUnPack(smtBasePack *psUnpack, UINT8* pInBuf, UINT16 usLen);
///*}

/***************************************
 *  通用部分封装函数  (低级抽象)
 *  在该函数内实现加密与解密过程
 *  
{*///

// 用户侧数据
typedef struct
{
    sMtAddress  sAddr;            // 地址信息
    sMtCtrl     sCtrl;
    sMtSEQ      sSEQ;         

    UINT16      usSeq2CsLen;     // 从SEQ 到 CS 之间的数据的长度
    UINT8       *pSeq2Cs;        // 从SEQ 到 CS 之间的数据
}sMtComPack;

// 抽象帧头
typedef struct
{
    UINT8         f68;          // 0x68
    
    UINT8         p10:2;        // 本协议标识  10
    UINT8         L1 :6;        // 
    UINT8         L2;           // L 第一次
    UINT8         p10_2:2;      //   10
    UINT8         L1_2 :6;      // 本协议标识
    UINT8         L2_2;         // L 第二次
    
    UINT8         s68;          // 0x68
    UINT8         C;            // 控制域
    sMtAddress_f  A;            // 地址域
    UINT8         AFN;          // 主功能码
    UINT8         SEQ;          // 帧序列
    
}sMtfComHead;                   // 帧侧公共的头结构

// 计算检测和函数
UINT8 ucmt_get_check_sum(UINT8 *pStartPos, UINT16 usLen);

// 封装每个帧的公共部分 
eMtErr emt_pack_common(eMtAFN eAFN, sMtComPack *psCommon,  UINT16 *pusLen, UINT8  *pOutBuf); 

// 解析公共部分
typedef struct
{
    sMtfComHead sfComHead;
    sMtComPack  sComPack;
    UINT8       u8CS;
    UINT16      usLenUserField1; 
    UINT16      usLenUserField2; 

}sMtUnpackCommon;

eMtErr emt_unpack_common(sMtUnpackCommon *psUnpack, UINT8* pInBuf, UINT16 usLen);

///*}

/*******************************************************
 *  外部接口
 *
{*///

/***************************************
 *  协议模块初始化
 *  使该协议API需要先进行初始化
 *  
{*///
typedef struct
{
    eMtRole eRole;                  // 身份，主站或是从站
    UINT8   ucPermitDelayMinutes;   // 允许时延
    UINT8   aucPw[MT_PW_LEN];       // 密码

#if MT_CFG_ENCRYPT
    peMtEncryptFunc   EncryptFunc;  // 加密接口
    peMtDecryptFunc   DecryptFunc;  // 解密接口
#endif
    
}sMtInit;                           // 协议初始化数据结构

// 初始化函数
eMtErr eMtInit(sMtInit* psInit);
///*}

// 获得该类型的报文的主动性
eMtPRM emtGetPrm(eMtDir eDir, eMtAFN eAfn, BOOL bAuto);

// 判断一个报文是否是有一个有效的376.1帧
eMtErr emtIsValidPack(const UINT8* pOutBuf, UINT16 usLen);

// 从帧缓冲区中找到第一个有效的帧的位置及长度
eMtErr emtFindValidPack(UINT8* pinBuf, UINT16 usLen, UINT16* pusFirstOff, UINT16* pusFirstLen);

// 设置事件计数器
void  vmtSetEC(UINT8 ucEC1, UINT8 ucEC2);

// 以字符串方式设置密码
void  vmtSetPw(char *pPw);

// 以buffer 方式设置登录密码
void vmtSetPwBuf(UINT8* buf);

// 获得我的身份
eMtRole emtWhoAmI();

// 获得命令对应的AFN
eMtAFN eGetCmdAfn(eMtCmd eCmd);

// 获得命令对应的Fn
UINT8  ucGetCmdFn(eMtCmd eCmd);
/***************************************
 *  数据单元标识与对应的数据单元 (高级抽象)
 *  
 * 数据单元为按数据单元标识所组织的数据,包括参数、命令等。
 * 数据组织的顺序规则：离散方式
 * 即：命令标识 数据单元 一一对应的方式
 * 
{*///
typedef struct
{
    eMtCmd       eCmd;        // 功能码
    UINT16       usPN;        // Pn (0 <= pn <= 2040 )
    BOOL         bApp;        // 是否有数据单元
    uMtApp       uApp;        // 对应不同的命令类型, 及上下行类型, 应用层数据时有时无
    
}sMtData;
///*}

/***************************************
 *  帧内容数据结构 用户侧  (高级抽象)
 *  用于封装
 *  变长结构
{*///
typedef struct
{
    sMtAddress    sAddress;        // 地址域
    eMtAFN        eAFN;            // 主功能码  同帧中必须是同一个主功能码下的不同子命令的组合
    eMtDir        eDir;            // 上下行
    eMtPRM        ePRM;            // 标志该报文是来自启动站 还是从动站
    eMtPos        ePos;            // 位置
    UINT8         ucSeq;           // pseq/rseq (0~15) 
 
    BOOL          bAcdFcb;         // FCB 帧计数位 在下行报文中有效
                                   /*
                                      启动站向同一从动站传输新的发送/确认或请求/响应传输服务时，将FCB取相反值。
                                      启动站保存每一个从动站FCB值，若超时未收到从动站的报文，或接收出现差错，
                                      则启动站不改变FCB的状态，重复原来的发送/确认或者请求/响应服务。
                                      复位命令中的FCB=0，从动站接收复位命令后将FCB置"0"
                                   */
                           
                                   // ACD 事件请求访问位, 如果有重要事件需要主站访问, 置真
                                   // 只在上行报文中 有效
    BOOL          bCon;            // 是否需要确认

    // 附加域
    BOOL          bPW;             // 有没有Pw字段
    BOOL          bEC;             // 有没有EC 字段
    BOOL          bTP;             // 是否有Tp字段

    
    UINT8         acPW[MT_PW_LEN]; // pw字段  消息认证码字段PW用于重要下行报文中，
                                   // 由16字节组成,PW是由主站按系统约定的认证算法产生
                                   // 并在主站发送的报文中下发给终端
                                   // 由终端进行校验认证,通过则响应主站命令,反之则否认
                                   // 终端在收到带有PW的报文,必须在认证通过后,才能响应命令。
    sMtEC         sEC;             // 事件计数器
    sMtTP         sTP;             // 时间标签
 

    // 数据单元域  变长域
    UINT16        usDataNum;      // 数据单元组的个数
    sMtData       sData[1];       // 数据单元组 变长 可能没有 可能多个
    
}smtPack;

// 封装函数
eMtErr emtPackBase(smtPack* psPack, UINT16* pusLen, UINT8* pOutBuf);   

// 解帧函数
eMtErr emtUnPackBase(smtPack *psUnpack, UINT8* pInBuf, UINT16 usLen);

// 封装函数
eMtErr emtPackLite(smtPack* psPack, UINT16* pusLen, UINT8* pOutBuf);   

// 解帧函数
eMtErr emtUnPackLite(smtPack *psUnpack, UINT8* pInBuf, UINT16 usLen);

// 高级接口
// 封装函数
eMtErr emtPack(smtPack* psPack, UINT16* pusLen, UINT8* pOutBuf);   

// 解帧函数
eMtErr emtUnPack(smtPack *psUnpack, UINT8* pInBuf, UINT16 usLen);
///*}
///*}
 
#endif // _QGDW_3761_API_H_
