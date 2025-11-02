/**
  * This program is provided for learning purposes only. Unauthorized use for any other
  * purposes is prohibited.
  * Tested hardware: STM32F103ZET6 microcontroller, Point Atom ELITE STM32 development board,
  * operating at 72MHz with a crystal oscillator at 12MHz.
  * QDtech-TFT LCD driver for STM32 using IO simulation.
  * xiao feng@ShenZhen QDtech co.,LTD
  * Company website: www.qdtft.com
  * Taobao store: http://qdtech.taobao.com
  * Wiki technical website: http://www.lcdwiki.com
  * Technical support is provided by our company. Feel free to reach out for any technical issues.
  * Office (Fax): +86 0755-23594567
  * Mobile: 15989313508 (Mr. feng)
  * Email: lcdwiki01@gmail.com, support@lcdwiki.com, goodtft@163.com
  * Technical support QQ: 3002773612, 3002778157
  * Technical discussion QQ group: 324828016
  * Creation date: 2018/08/09
  * Version: V1.0
  * All rights reserved. Unauthorized reproduction is strictly prohibited.
  * Copyright(C) Shenzhen QDtech Co., Ltd. 2018-2028
  * All rights reserved
  */
/****************************************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**************************************************************************************************/

/**
 * Copyright Statement:
 * This software and related documentation (ESWIN SOFTWARE) are protected under relevant copyright laws.
 * The information contained herein is confidential and proprietary to
 * Beijing ESWIN Computing Technology Co., Ltd.(ESWIN)and/or its licensors.
 * Without the prior written permission of ESWIN and/or its licensors, any reproduction, modification,
 * use or disclosure Software, and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * Copyright ?[2023] [Beijing ESWIN Computing Technology Co., Ltd.]. All rights reserved.
 *
 * RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES THAT THE SOFTWARE
 * AND ITS DOCUMENTATIONS (ESWIN SOFTWARE) RECEIVED FROM ESWIN AND / OR ITS REPRESENTATIVES
 * ARE PROVIDED TO RECEIVER ON AN "AS-IS" BASIS ONLY. ESWIN EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE OR NON INFRINGEMENT.
 * NEITHER DOES ESWIN PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE SOFTWARE OF ANY THIRD PARTY
 * WHICH MAY BE USED BY,INCORPORATED IN, OR SUPPLIED WITH THE ESWIN SOFTWARE,
 * AND RECEIVER AGREES TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL ESWIN BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
 * OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Modification History :
 * Date:               Version:                    Author:
 * Changes:
 *
 */

#ifndef __LCD_H
#define __LCD_H
#include "stdlib.h"
#include "emps_platform.h"
#include "pins_driver.h"
#include <osal.h>
#include <stdint.h>
#include "spi_master_driver.h"

//LCD important parameter set
typedef struct
{
    uint16_t width;   // LCD width
    uint16_t height;  // LCD height
    uint16_t id;      // LCD ID
    uint8_t dir;      // Controls landscape or portrait: 0, portrait; 1. Landscape.
    uint16_t wramcmd; // Start writing the gram command
    uint16_t setxcmd; // Set the x-coordinate instruction
    uint16_t setycmd; // Set the Y coordinate instruction
} _lcd_dev;

/* Define the spi label */
#define SPI_NUM 1
//LCD parameters
extern _lcd_dev lcddev; //Manage the LCD important parameters
/////////////////////////////////////the user configuration area///////////////////////////////////
#define USE_HORIZONTAL 1 //The horizontal screen can rotate clockwise 0-0 degrees, 1-90 degrees, 2-180 degrees, and 3-270 degrees

//Define the size of the LCD
#define LCD_W 240
#define LCD_H 320

//function to be called outside of the TFTLCD section
extern uint16_t POINT_COLOR; //Default color of point is red
extern uint16_t BACK_COLOR;  //The default color of back is white

#define GPIO_TYPE_0 GPIOD
#define GPIO_TYPE   GPIOC
#define LED         2
#define LCD_RS      19
#define LCD_RST     21

//GPIO set（high�?
#define LCD_LED_ON  PINS_DRV_WritePin(GPIO_TYPE_0, LED, 1)
#define LCD_RS_SET  PINS_DRV_WritePin(GPIO_TYPE, LCD_RS, 1)
#define LCD_RST_SET PINS_DRV_WritePin(GPIO_TYPE, LCD_RST, 1)

//GPIO reset（low�?
#define LCD_LED_OFF PINS_DRV_WritePin(GPIO_TYPE_0, LED, 0)
#define LCD_RS_CLR  PINS_DRV_WritePin(GPIO_TYPE, LCD_RS, 0)
#define LCD_RST_CLR PINS_DRV_WritePin(GPIO_TYPE, LCD_RST, 0)

//The brush color
#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0XF81F
#define GRED    0XFFE0
#define GBLUE   0X07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0XBC40 //brown
#define BRRED   0XFC07 //brownish red
#define GRAY    0X8430 //gray
//GUI color

#define DARKBLUE  0X01CF //dark blue
#define LIGHTBLUE 0X7D7C //light blue
#define GRAYBLUE  0X5458 //gray blue
//The above three colors are the colors of the PANEL

#define LIGHTGREEN 0X841F //light green
#define LIGHTGRAY  0XEF5B //light gray(PANNEL)
#define LGRAY      0XC618 //Light gray (PANNEL), window background color

#define LGRAYBLUE 0XA651 //Light gray blue (middle layer color)
#define LBBLUE    0X2B12 //light brown blue (select the reverse color of the item)

uint16_t LCD_RD_DATA(void);

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(uint16_t Color);
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos);
void LCD_DrawPoint(uint16_t x, uint16_t y);     //draw point
uint16_t LCD_ReadPoint(uint16_t x, uint16_t y); //read point
void LCD_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);

uint16_t LCD_RD_DATA(void); //read LCD data
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue);
void LCD_WR_DATA(uint8_t data);
uint16_t LCD_ReadReg(uint8_t LCD_Reg);
void LCD_WriteRAM_Prepare(void);
void LCD_WriteRAM(uint16_t RGB_Code);
uint16_t LCD_ReadRAM(void);
uint16_t LCD_BGR2RGB(uint16_t c);
void LCD_SetParam(void);
void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_direction(uint8_t direction);
void LCD_WR_REG(uint8_t data);
void LCD_WR_DATA(uint8_t data);

#endif
