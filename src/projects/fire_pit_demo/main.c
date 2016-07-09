/******************** (C) COPYRIGHT 2011 野火嵌入式开发工作室 ********************
 * 文件名       ：main.c
 * 描述         ：PIT定时中断实验
 *          
 * 实验平台     ：野火kinetis开发板
 * 库版本       ：
 * 嵌入系统     ：
 *
 * 作者         ：野火嵌入式开发工作室 
 * 淘宝店       ：http://firestm32.taobao.com
 * 技术支持论坛 ：http://www.ourdev.cn/bbs/bbs_list.jsp?bbs_id=1008   
**********************************************************************************/	


#include "common.h"
#include "include.h"


/*************************************************************************
*                             野火嵌入式开发工作室
*                               PIT定时中断测试
*
*  实验说明：野火 PIT 定时中断实验，在中断函数了用 LED0 显示进入了中断函数。
*            
*  实验操作：无
*
*  实验效果：LED0间隔1s闪烁
*
*  实验目的：测试 PIT 是否定时产生中断
*
*  修改时间：2012-2-29     已测试
*
*  备    注：野火Kinetis开发板的 LED0~3 ，分别接PTD15~PTD12 ，低电平点亮
*            
*************************************************************************/
void main()
{
    
    DisableInterrupts;                                //禁止总中断
    
    LED_INIT();                                       //初始化LED，PIT0中断用到LED
    pit_init_ms(PIT0,1000);                           //初始化PIT0，定时时间为： 1000ms
    
    EnableInterrupts;			              //开总中断
    while(1)
    {
    }
}