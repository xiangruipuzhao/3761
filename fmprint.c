#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "fmprint.h"




/*****************************************************************************
 函 数 名  : fmt_print
 功能描述  : 格式化输出
 输入参数  : char* str  
             int fmt    
             ...        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月26日 星期五
    作    者   : liming
    修改内容   : 新生成函数

*****************************************************************************/
void fmt_print(char* str,...)
{
    if(!str)
    {
        return;
    }


    char outbuf[1024] = {0};
    char *pStr;
    int len = 0;

    va_list arg_ptr;

    len += sprintf(outbuf, "%s", FM_PRINT_BNG);
    va_start(arg_ptr, str);    
    do
    {
        pStr = va_arg(arg_ptr, char*);
        if(pStr != NULL)
        len += sprintf(outbuf+len,"%s;", pStr);
        
    }while(pStr != NULL);
    
    va_end(arg_ptr);
    sprintf(outbuf+len-1, "%s%s%s",FM_PRINT_MID, str, FM_PRINT_END);
    printf(outbuf);


    
    
}



/*****************************************************************************
 函 数 名  : fm_print
 功能描述  : | | 的方法来输入格式
 输入参数  : char* str              
             unsigned int ulFormat
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月26日 星期五
    作    者   : liming
    修改内容   : 新生成函数


使用帮助：前景色 | 背景色 | 多种功能码组合

// 功能代码
FM_ALL_NORMAL     重新设置属性到缺省设置
FM_BOLD           粗体 
FM_HALF_BRIGHT    设置一半亮度 
FM_UNDERLINE      下划线
FM_FLASH          闪烁
FM_RES_WHITE      反白显示
FM_IN_VISIBLE     不可见

// 前景色
FR_NORMAL  0x00 
FR_BLACK   0x01  
FR_RED     0x02
FR_GREEN   0x03
FR_YELLOW  0x04
FR_BLUE    0x05
FR_MAGENTA 0x06   Magenta 紫红色
FR_CYAN    0x07   青蓝色
FR_WHITE   0x08  
FR_SET_UNDERLINE   默认前景色加下划线
FR_RMV_UNDERLINE 

// 背景色
BK_NORMAL  
BK_BLACK   
BK_RED     
BK_GREEN   
BK_YELLOW  
BK_BLUE    
BK_MAGENTA  Magenta 紫红色
BK_CYAN     青蓝色
BK_WHITE    
BK_DEFAULT  默认背景色

*****************************************************************************/
void fm_print(char* str, unsigned int ulFormat)
{
    unsigned char ucFrontColor = 0;
    unsigned char ucBackColr   = 0;
    unsigned char ucEffect     = 0;
    unsigned char ucCtrl       = 0;
    unsigned int  ulFormatTmp  = 0;
    int len = 0;
    
    ucFrontColor = ulFormat & 0xFF;
    ucBackColr   = (ulFormat >> 8) & 0xFF;
    ucEffect     = (ulFormat >> 16) & 0xFF;
    ucCtrl       = (ulFormat >> 24) & 0xFF;

    char strFmt[64] = {0};

    switch(ucFrontColor)
    {

        case FR_NORMAL:
            break;
            
        case FR_BLACK:
            len = sprintf(strFmt,"%s",FR_COLOR_BLACK);
            break;
        case FR_RED:
            len = sprintf(strFmt,"%s",FR_COLOR_RED);
            break;
        case FR_GREEN:
            len =  sprintf(strFmt,"%s",FR_COLOR_GREEN);
            break;
        case FR_YELLOW:
            len = sprintf(strFmt,"%s",FR_COLOR_YELLOW);
            break;
        case FR_BLUE:
            len = sprintf(strFmt,"%s",FR_COLOR_BLUE);
            break;
        case FR_MAGENTA:
            len = sprintf(strFmt,"%s",FR_COLOR_MAGENTA);
            break;
        case FR_CYAN:
            len = sprintf(strFmt,"%s",FR_COLOR_CYAN);
            break;
       case FR_WHITE:
            len = sprintf(strFmt,"%s",FR_COLOR_WHITE);
            break;
        case FR_SET_UNDERLINE:
            len = sprintf(strFmt,"%s",FR_COLOR_SET_UNDERLINE);
            break;
        case FR_RMV_UNDERLINE:
            len = sprintf(strFmt,"%s",FR_COLOR_RMV_UNDERLINE);
            break;

        default:
        break;
    }


    if(ucBackColr != 0)
    {
        len += sprintf(strFmt+len,"%s",";");
    }
    
   switch(ucBackColr)
    {
        case E_BK_BLACK:
            len += sprintf(strFmt+len,"%s",BK_COLOR_BLACK);
            break;
        case E_BK_RED:
            len += sprintf(strFmt+len,"%s",BK_COLOR_RED);
            break;
        case E_BK_GREEN:
            len += sprintf(strFmt+len,"%s",BK_COLOR_GREEN);
            break;
        case E_BK_YELLOW:
            len += sprintf(strFmt+len,"%s",BK_COLOR_YELLOW);
            break;
        case E_BK_BLUE:
            len += sprintf(strFmt+len,"%s",BK_COLOR_BLUE);
            break;
        case E_BK_MAGENTA:
           len += sprintf(strFmt+len,"%s",BK_COLOR_MAGENTA);
            break;
        case E_BK_CYAN:
            len += sprintf(strFmt+len,"%s",BK_COLOR_CYAN);
            break;
       case E_BK_WHITE:
            len += sprintf(strFmt+len,"%s",BK_COLOR_WHITE);
            break;
        default:
            break;

    }

    if(ucEffect != 0)
    {
        len += sprintf(strFmt+len,"%s",";");
    }

    ulFormatTmp = ulFormat & 0xFFFF0000;
    if(ulFormatTmp & FM_BOLD)
    {
         len += sprintf(strFmt+len,"%s",FMT_BOLD);
         ulFormatTmp &= ~FM_BOLD;

         if(ulFormatTmp != 0)
         {
            len += sprintf(strFmt+len,"%s",";");  
         }
    }
    
    if(ulFormatTmp & FM_HALF_BRIGHT)
    {
         len += sprintf(strFmt+len,"%s",FMT_HALF_BRIGHT);
         ulFormatTmp &= ~FM_HALF_BRIGHT;
         if(ulFormatTmp != 0)
         {
            len += sprintf(strFmt+len,"%s",";");  
         }
    }


    if(ulFormatTmp & FM_UNDERLINE)
    {
         len += sprintf(strFmt+len,"%s",FMT_UNDERLINE);
         ulFormatTmp &= ~FM_UNDERLINE;
         if(ulFormatTmp != 0)
         {
            len += sprintf(strFmt+len,"%s",";");  
         }
    }

    if(ulFormatTmp & FM_FLASH)
    {
         len += sprintf(strFmt+len,"%s",FMT_FLASH);
         ulFormatTmp &= ~FM_FLASH;
         if(ulFormatTmp != 0)
         {
            len += sprintf(strFmt+len,"%s",";");  
         }
    }

    if(ulFormatTmp & FM_RES_WHITE)
    {
         len += sprintf(strFmt+len,"%s",FMT_RES_WHITE);
         ulFormatTmp &= ~FM_RES_WHITE;
         if(ulFormatTmp != 0)
         {
            len += sprintf(strFmt+len,"%s",";");  
         }
    }

    if(ulFormatTmp & FM_IN_VISIBLE)
    {
         len += sprintf(strFmt+len,"%s",FMT_IN_VISIBLE);
         ulFormatTmp &= ~FM_IN_VISIBLE;
         if(ulFormatTmp != 0)
         {
            len += sprintf(strFmt+len,"%s",";");  
         }
    }

    //CTRL
    printf("%s%s%s%s%s", FM_PRINT_BNG,strFmt,FM_PRINT_MID,str,FM_PRINT_END);


}



