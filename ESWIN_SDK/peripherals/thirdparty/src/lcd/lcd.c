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

#include "lcd.h"
#include "stdlib.h"

/* Control the LCD parameters */
/* The default is portrait */
_lcd_dev lcddev;

/* Brush color, background color */
uint16_t POINT_COLOR = 0x0000, BACK_COLOR = 0xFFFF;
uint16_t DeviceCode;

/*****************************************************************************
 * @name       :void LCD_WR_REG(uint8_t data)
 * @function   :Write an 8-bit command to the LCD screen
 * @parameters :data:Command value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_REG(uint8_t data)
{
    LCD_RS_CLR;
    SPI_DRV_MasterTransferBlocking(SPI_NUM, (uint8_t *)&data, NULL, sizeof(data), 1000);
}

/*****************************************************************************
 * @name       :void LCD_WR_DATA(uint8_t data)
 * @function   :Write an 8-bit data to the LCD screen
 * @parameters :data:data value to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WR_DATA(uint8_t data)
{
    LCD_RS_SET;
    SPI_DRV_MasterTransferBlocking(SPI_NUM, (uint8_t *)&data, NULL, sizeof(data), 1000);
}

/*****************************************************************************
 * @name       :void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
 * @function   :Write data into registers
 * @parameters :LCD_Reg:Register address
                LCD_RegValue:Data to be written
 * @retvalue   :None
******************************************************************************/
void LCD_WriteReg(uint8_t LCD_Reg, uint16_t LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

/*****************************************************************************
 * @name       :void LCD_WriteRAM_Prepare(void)
 * @function   :Write GRAM
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_WriteRAM_Prepare(void)
{
    LCD_WR_REG(lcddev.wramcmd);
}

void LCD_WriteRAM(uint16_t RGB_Code)
{
    Lcd_WriteData_16Bit(RGB_Code);
}

/*****************************************************************************
 * @name       :void Lcd_WriteData_16Bit(uint16_t Data)
 * @function   :Write an 16-bit command to the LCD screen
 * @parameters :Data:Data to be written
 * @retvalue   :None
******************************************************************************/
void Lcd_WriteData_16Bit(uint16_t Data)
{
    LCD_RS_SET;
    uint8_t Data_a = Data >> 8;
    uint8_t Data_b = (Data << 8) >> 8;
    SPI_DRV_MasterTransferBlocking(SPI_NUM, (uint8_t *)&Data_a, NULL, sizeof(Data_a), 1000);
    SPI_DRV_MasterTransferBlocking(SPI_NUM, (uint8_t *)&Data_b, NULL, sizeof(Data_b), 1000);
}

/*****************************************************************************
 * @name       :void LCD_DrawPoint(uint16_t x,uint16_t y)
 * @function   :Write a pixel data at a specified location
 * @parameters :x:the x coordinate of the pixel
                y:the y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_DrawPoint(uint16_t x, uint16_t y)
{
    LCD_SetCursor(x, y); /* Set the cursor position */
    Lcd_WriteData_16Bit(POINT_COLOR);
}

/*****************************************************************************
 * @name       :void LCD_Clear(uint16_t Color)
 * @function   :Full screen filled LCD screen
 * @parameters :color:Filled color
 * @retvalue   :None
******************************************************************************/
void LCD_Clear(uint16_t Color)
{
    unsigned int i, m;
    LCD_SetWindows(0, 0, lcddev.width - 1, lcddev.height - 1);
    LCD_RS_SET;
    for (i = 0; i < lcddev.height; i++) {
        for (m = 0; m < lcddev.width; m++) {
            Lcd_WriteData_16Bit(Color);
        }
    }
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @date       :2018-08-09
 * @function   :Reset LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_RESET(void)
{
    LCD_RST_CLR;
    OS_DelayMs(100);
    LCD_RST_SET;
    OS_DelayMs(50);
}

/*****************************************************************************
 * @name       :void LCD_RESET(void)
 * @function   :Initialization LCD screen
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void LCD_Init(void)
{
    LCD_RESET(); /* LCD reset */

    /**************2.2inch ILI9341 Initialize**********/
    LCD_WR_REG(0xCF);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xD9);
    LCD_WR_DATA(0X30);
    LCD_WR_REG(0xED);
    LCD_WR_DATA(0x64);
    LCD_WR_DATA(0x03);
    LCD_WR_DATA(0X12);
    LCD_WR_DATA(0X81);
    LCD_WR_REG(0xE8);
    LCD_WR_DATA(0x85);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x7A);
    LCD_WR_REG(0xCB);
    LCD_WR_DATA(0x39);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x02);
    LCD_WR_REG(0xF7);
    LCD_WR_DATA(0x20);
    LCD_WR_REG(0xEA); /*  Driver timing control B */
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0xC0);  /* Power control */
    LCD_WR_DATA(0x21); /* VRH[5:0]  1B */
    LCD_WR_REG(0xC1);  /* Power control */
    LCD_WR_DATA(0x12); /* SAP[2:0];BT[3:0]  01 */
    LCD_WR_REG(0xC5);  /* VCM control */
    LCD_WR_DATA(0x39); /* 3F */
    LCD_WR_DATA(0x37); /* 3C */
    LCD_WR_REG(0xC7);  /* VCM control2 */
    LCD_WR_DATA(0XAB); /* B0 */
    LCD_WR_REG(0x36);  /*  Memory Access Control */
    LCD_WR_DATA(0x48);
    LCD_WR_REG(0x3A); /*  Pixel Format Set */
    LCD_WR_DATA(0x55);
    LCD_WR_REG(0xB1); /* Frame Rate Control (In Normal Mode/Full Colors) */
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1B); /* 1A */
    LCD_WR_REG(0xB6);  /*  Display Function Control */
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0xA2); /* A2 */
    LCD_WR_REG(0xF2);  /*  3Gamma Function Disable */
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0x26); /* Gamma curve selected */
    LCD_WR_DATA(0x01);

    LCD_WR_REG(0xE0); /* Set Gamma */
    LCD_WR_DATA(0x0F);
    LCD_WR_DATA(0x23);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x0E);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x4B);
    LCD_WR_DATA(0XA8);
    LCD_WR_DATA(0x3B);
    LCD_WR_DATA(0x0A);
    LCD_WR_DATA(0x14);
    LCD_WR_DATA(0x06);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x00);
    LCD_WR_REG(0XE1); /* Set Gamma */
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x1C);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x10);
    LCD_WR_DATA(0x08);
    LCD_WR_DATA(0x34);
    LCD_WR_DATA(0x47);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x05);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x09);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x36);
    LCD_WR_DATA(0x0F);
    LCD_WR_REG(0x2B); /* Page Address Set */
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x01);
    LCD_WR_DATA(0x3f);
    LCD_WR_REG(0x2A); /* Column Address Set */
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0xef);
    LCD_WR_REG(0x11); /* Exit Sleep */
    OS_DelayMs(120);
    LCD_WR_REG(0x29); /* display on */

    LCD_direction(USE_HORIZONTAL); /* Set the LCD display direction */
    LCD_LED_ON;                    /* Light up the back light */
    LCD_Clear(DARKBLUE);           /* Clear the screen in dark blue */
}

/*****************************************************************************
 * @name       :void LCD_SetWindows(uint16_t xStar, uint16_t yStar,uint16_t xEnd,uint16_t yEnd)
 * @function   :Setting LCD display window
 * @parameters :xStar:the beginning x coordinate of the LCD display window
								yStar:the beginning y coordinate of the LCD display window
								xEnd:the ending x coordinate of the LCD display window
								yEnd:the ending y coordinate of the LCD display window
 * @retvalue   :None
******************************************************************************/
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    LCD_WR_REG(lcddev.setxcmd);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0x00FF & xEnd);

    LCD_WR_REG(lcddev.setycmd);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0x00FF & yEnd);

    LCD_WriteRAM_Prepare(); /* start write to GRAM */
}

/*****************************************************************************
 * @name       :void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
 * @function   :Set coordinate value
 * @parameters :Xpos:the  x coordinate of the pixel
								Ypos:the  y coordinate of the pixel
 * @retvalue   :None
******************************************************************************/
void LCD_SetCursor(uint16_t Xpos, uint16_t Ypos)
{
    LCD_SetWindows(Xpos, Ypos, Xpos, Ypos);
}

/*****************************************************************************
 * @name       :void LCD_direction(uint8_t direction)
 * @function   :Setting the display direction of LCD screen
 * @parameters :direction:0-0 degree
                          1-90 degree
													2-180 degree
													3-270 degree
 * @retvalue   :None
******************************************************************************/
void LCD_direction(uint8_t direction)
{
    lcddev.setxcmd = 0x2A;
    lcddev.setycmd = 0x2B;
    lcddev.wramcmd = 0x2C;
    switch (direction) {
    case 0:
        lcddev.width  = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, (1 << 3) | (0 << 6) | (0 << 7)); /* BGR==1,MY==0,MX==0,MV==0 */
        break;
    case 1:
        lcddev.width  = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (0 << 7) | (1 << 6) | (1 << 5)); /* BGR==1,MY==1,MX==0,MV==1 */
        break;
    case 2:
        lcddev.width  = LCD_W;
        lcddev.height = LCD_H;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 6) | (1 << 7)); /* BGR==1,MY==0,MX==0,MV==0 */
        break;
    case 3:
        lcddev.width  = LCD_H;
        lcddev.height = LCD_W;
        LCD_WriteReg(0x36, (1 << 3) | (1 << 7) | (1 << 5)); /* BGR==1,MY==1,MX==0,MV==1 */
        break;
    default:
        break;
    }
}
