/******************************************************************************

                  版权所有 (C), 2001-2011, 国网电科院通信与用电技术分公司

 ******************************************************************************
  文 件 名   : fmprint.h
  版 本 号   : 初稿
  作    者   : liming
  生成日期   : 2013年4月26日 星期五
  最近修改   :
  功能描述   : 终端格式化输出头文件，实现终端样式输出
  函数列表   :
  修改历史   :
  1.日    期   : 2013年4月26日 星期五
    作    者   : liming
    修改内容   : 创建文件

******************************************************************************/

// 功能代码
#define FM_ALL_NORMAL    0x00000   // 重新设置属性到缺省设置
#define FM_BOLD          0x10000   // 粗体 
#define FM_HALF_BRIGHT   0x20000   // 设置一半亮度 
#define FM_UNDERLINE     0x40000
#define FM_FLASH         0x80000   // 闪烁
#define FM_RES_WHITE     0x100000  // 反白显示
#define FM_IN_VISIBLE    0x200000  // 不可见

// 前景色
#define FR_NORMAL  0x00 
#define FR_BLACK   0x01  
#define FR_RED     0x02
#define FR_GREEN   0x03
#define FR_YELLOW  0x04
#define FR_BLUE    0x05
#define FR_MAGENTA 0x06   // Magenta 紫红色
#define FR_CYAN    0x07   // 青蓝色
#define FR_WHITE   0x08  
#define FR_SET_UNDERLINE 0x09   // 默认前景色加下划线
#define FR_RMV_UNDERLINE 0x0A

// 背景色
#define BK_NORMAL  0x0000
#define BK_BLACK   0x0100
#define BK_RED     0x0200
#define BK_GREEN   0x0300
#define BK_YELLOW  0x0400
#define BK_BLUE    0x0500
#define BK_MAGENTA 0x0600  // Magenta 紫红色
#define BK_CYAN    0x0700  // 青蓝色
#define BK_WHITE   0x0800 
#define BK_DEFAULT 0x0900  // 默认背景色


void fmt_print(char* str,...);
void fm_print(char* str, unsigned int ulFormat);










#define FMT_ALL_NORMAL    "0"  // 重新设置属性到缺省设置
#define FMT_BOLD          "1"  // 粗体 
#define FMT_HALF_BRIGHT   "2"  // 设置一半亮度 
#define FMT_UNDERLINE     "4"
#define FMT_FLASH         "5"  // 闪烁
#define FMT_RES_WHITE     "7"  // 反白显示
#define FMT_IN_VISIBLE    "8"  // 不可见

// 前景色
#define FR_COLOR_BLACK   "30"  
#define FR_COLOR_RED     "31"
#define FR_COLOR_GREEN   "32"
#define FR_COLOR_YELLOW  "33"
#define FR_COLOR_BLUE    "34"
#define FR_COLOR_MAGENTA "35"   // Magenta 紫红色
#define FR_COLOR_CYAN    "36"   // 青蓝色
#define FR_COLOR_WHITE   "37"  
#define FR_COLOR_SET_UNDERLINE "38"   // 默认前景色加下划线
#define FR_COLOR_RMV_UNDERLINE "39"

// 背景色
#define BK_COLOR_BLACK   "40"
#define BK_COLOR_RED     "41"
#define BK_COLOR_GREEN   "42"
#define BK_COLOR_YELLOW  "43"
#define BK_COLOR_BLUE    "44"
#define BK_COLOR_MAGENTA "45"  // Magenta 紫红色
#define BK_COLOR_CYAN    "46"   // 青蓝色
#define BK_COLOR_WHITE   "47" 
#define BK_COLOR_DEFAULT "49"   // 默认背景色




#define FM_PRINT_BNG     "\033["
#define FM_PRINT_MID     "m"
#define FM_PRINT_END     "\033[0m"






/*
|---------------|---------------|---------------|---------------|
     前景色           背景色        特殊样式        光标控制
|---------------|---------------|---------------|---------------|




*/







typedef enum
{  
    E_BK_NORMAL = 0,  
    E_BK_BLACK,    // 1
    E_BK_RED,      //2// 2 
    E_BK_GREEN,    // 3
    E_BK_YELLOW,   // 4
    E_BK_BLUE,     // 5
    E_BK_MAGENTA,  // 6 
    E_BK_CYAN,     // 7 
    E_BK_WHITE,    // 8
    E_BK_DEFAULT,  // 9

}eBackColor;

typedef enum
{
    EFMT_ALL_NORMAL,   // 0  重新设置属性到缺省设置
    EFMT_BOLD,         // 1  粗体 
    EFMT_HALF_BRIGHT,  // 2  设置一半亮度 
    EFMT_UNDERLINE,    // 4
    EFMT_FLASH,        // 5  闪烁
    EFMT_RES_WHITE,    // 7  反白显示
    EFMT_IN_VISIBLE,   // 8  不可见
    
}eEffect;










