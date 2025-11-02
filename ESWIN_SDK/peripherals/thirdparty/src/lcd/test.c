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
#include "gui.h"
#include "test.h"
#include "pic.h"

//========================variable==========================//
uint16_t ColorTab[5] = {RED, GREEN, BLUE, YELLOW, BRED}; /* Define color array */
//=====================end of variable======================//

/*****************************************************************************
 * @name       :void DrawTestPage(uint8_t *str)
 * @date       :2018-08-09
 * @function   :Drawing test interface
 * @parameters :str:the start address of the Chinese and English strings
 * @retvalue   :None
******************************************************************************/

void DrawTestPage(uint8_t *str)
{
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    POINT_COLOR = WHITE;
    /* Center alignment display */
    Gui_StrCenter(0, 120, BLUE, WHITE, str, 16, 1);
}

/*****************************************************************************
 * @name       :void main_test(void)
 * @date       :2018-08-09
 * @function   :Drawing the main Interface of the Comprehensive Test Program
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void main_test(void)
{
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    //  DrawTestPage((uint8_t *)"ﬁ»ÀπŒ∞º∆À„◊€∫œ≤‚ ‘≥Ã–Ú");
    /* Center alignment display */
    Gui_StrCenter(0, 30, RED, BLUE, (uint8_t *)"ﬁ»ÀπŒ∞º∆À„", 16, 1);
    /* Center alignment display */
    Gui_StrCenter(0, 60, RED, BLUE, (uint8_t *)"◊€∫œ≤‚ ‘≥Ã–Ú", 16, 1);
    /* Center alignment display */
    Gui_StrCenter(0, 90, GREEN, BLUE, (uint8_t *)"2.2\" ILI9341 240X320", 16, 1);
    OS_DelayMs(1500);
}

/*****************************************************************************
 * @name       :void Test_Color(void)
 * @date       :2018-08-09
 * @function   :Color fill test(white,black,red,green,blue)
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Color(void)
{
    //  DrawTestPage((uint8_t *)"≤‚ ‘1:¥ø…´ÃÓ≥‰≤‚ ‘");
    LCD_Fill(0, 0, lcddev.width, lcddev.height, WHITE);
    Show_Str(20, 30, BLUE, YELLOW, (uint8_t *)"WHITE", 16, 1);
    OS_DelayMs(800);
    LCD_Fill(0, 0, lcddev.width, lcddev.height, RED);
    Show_Str(20, 30, BLUE, YELLOW, (uint8_t *)"RED ", 16, 1);
    OS_DelayMs(800);
    LCD_Fill(0, 0, lcddev.width, lcddev.height, GREEN);
    Show_Str(20, 30, BLUE, YELLOW, (uint8_t *)"GREEN ", 16, 1);
    OS_DelayMs(800);
    LCD_Fill(0, 0, lcddev.width, lcddev.height, BLUE);
    Show_Str(20, 30, RED, YELLOW, (uint8_t *)"BLUE ", 16, 1);
    OS_DelayMs(800);
}

/*****************************************************************************
 * @name       :void Test_FillRec(void)
 * @date       :2018-08-09
 * @function   :Rectangular display and fill test
                Display red,green,blue,yellow,pink rectangular boxes in turn,
                1500 milliseconds later,
                Fill the rectangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_FillRec(void)
{
    uint8_t i = 0;
    //  DrawTestPage((uint8_t *)"≤‚ ‘2:GUIæÿ–ŒÃÓ≥‰≤‚ ‘");
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++) {
        POINT_COLOR = ColorTab[i];
        LCD_DrawRectangle(lcddev.width / 2 - 80 + (i * 15),
                        lcddev.height / 2 - 80 + (i * 15),
                        lcddev.width / 2 - 80 + (i * 15) + 60,
                        lcddev.height / 2 - 80 + (i * 15) + 60);
    }
    OS_DelayMs(1500);
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++) {
        POINT_COLOR = ColorTab[i];
        LCD_DrawFillRectangle(lcddev.width / 2 - 80 + (i * 15),
                            lcddev.height / 2 - 80 + (i * 15),
                            lcddev.width / 2 - 80 + (i * 15) + 60,
                            lcddev.height / 2 - 80 + (i * 15) + 60);
    }
    OS_DelayMs(1500);
}

/*****************************************************************************
 * @name       :void Test_Circle(void)
 * @date       :2018-08-09
 * @function   :circular display and fill test
                Display red,green,blue,yellow,pink circular boxes in turn,
                1500 milliseconds later,
                Fill the circular in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Circle(void)
{
    uint8_t i = 0;
    //  DrawTestPage((uint8_t *)"≤‚ ‘3:GUIª≠‘≤ÃÓ≥‰≤‚ ‘");
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
        gui_circle(lcddev.width / 2 - 80 + (i * 25),
                  lcddev.height / 2 - 50 + (i * 25), ColorTab[i], 30, 0);
    OS_DelayMs(1500);
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++)
        gui_circle(lcddev.width / 2 - 80 + (i * 25),
                  lcddev.height / 2 - 50 + (i * 25), ColorTab[i], 30, 1);
    OS_DelayMs(1500);
}

/*****************************************************************************
 * @name       :void English_Font_test(void)
 * @date       :2018-08-09
 * @function   :English display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void English_Font_test(void)
{
    //  DrawTestPage((uint8_t *)"≤‚ ‘5:”¢Œƒœ‘ æ≤‚ ‘");
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    Show_Str(10, 30, BLUE, YELLOW, (uint8_t *)"6X12:abcdefghijklmnopqrstuvwxyz0123456789", 12, 0);
    Show_Str(10, 45, BLUE, YELLOW, (uint8_t *)"6X12:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 12, 1);
    Show_Str(10, 60, BLUE, YELLOW, (uint8_t *)"6X12:~!@#$%^&*()_+{}:<>?/|-+.", 12, 0);
    Show_Str(10, 80, BLUE, YELLOW, (uint8_t *)"8X16:abcdefghijklmnopqrstuvwxyz0123456789", 16, 0);
    Show_Str(10, 100, BLUE, YELLOW, (uint8_t *)"8X16:ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789", 16, 1);
    Show_Str(10, 120, BLUE, YELLOW, (uint8_t *)"8X16:~!@#$%^&*()_+{}:<>?/|-+.", 16, 0);
    OS_DelayMs(1200);
}

/*****************************************************************************
 * @name       :void Test_Triangle(void)
 * @date       :2018-08-09
 * @function   :triangle display and fill test
                Display red,green,blue,yellow,pink triangle boxes in turn,
                1500 milliseconds later,
                Fill the triangle in red,green,blue,yellow and pink in turn
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Test_Triangle(void)
{
    uint8_t i = 0;
    //  DrawTestPage((uint8_t *)"≤‚ ‘4:GUI TriangleÃÓ≥‰≤‚ ‘");
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++) {
        POINT_COLOR = ColorTab[i];
        Draw_Triangel(lcddev.width / 2 - 80 + (i * 20),
                    lcddev.height / 2 - 20 + (i * 15),
                    lcddev.width / 2 - 50 - 1 + (i * 20),
                    lcddev.height / 2 - 20 - 52 - 1 + (i * 15),
                    lcddev.width / 2 - 20 - 1 + (i * 20),
                    lcddev.height / 2 - 20 + (i * 15));
    }
    OS_DelayMs(1500);
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 5; i++) {
        POINT_COLOR = ColorTab[i];
        Fill_Triangel(lcddev.width / 2 - 80 + (i * 20),
                      lcddev.height / 2 - 20 + (i * 15),
                      lcddev.width / 2 - 50 - 1 + (i * 20),
                      lcddev.height / 2 - 20 - 52 - 1 + (i * 15),
                      lcddev.width / 2 - 20 - 1 + (i * 20),
                      lcddev.height / 2 - 20 + (i * 15));
    }
    OS_DelayMs(1500);
}

/*****************************************************************************
 * @name       :void Chinese_Font_test(void)
 * @date       :2018-08-09
 * @function   :chinese display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Chinese_Font_test(void)
{
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    Show_Str(10, 30, BLUE, YELLOW, (uint8_t *)"16X16:ﬁ»ÀπŒ∞º∆À„ºº ıπ…∑›”–œﬁπ´Àæª∂”≠ƒ˙", 16, 0);
    Show_Str(10, 50, BLUE, YELLOW, (uint8_t *)"16X16:Welcome ﬁ»ÀπŒ∞", 16, 0);
    Show_Str(10, 70, BLUE, YELLOW, (uint8_t *)"24X24:÷–Œƒ≤‚ ‘", 24, 1);
    Show_Str(10, 100, BLUE, YELLOW, (uint8_t *)"32X32:◊÷ÃÂ≤‚ ‘", 32, 1);
    OS_DelayMs(1200);
}

/*****************************************************************************
 * @name       :void Pic_test(void)
 * @date       :2018-08-09
 * @function   :picture display test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Pic_test(void)
{
    //  DrawTestPage((uint8_t *)"≤‚ ‘7:Õº∆¨œ‘ æ≤‚ ‘");
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    Gui_Drawbmp16(30, 30, gImage_qq);
    Show_Str(30 + 12, 75, BLUE, YELLOW, (uint8_t *)"QQ", 16, 1);
    Gui_Drawbmp16(90, 30, gImage_qq);
    Show_Str(90 + 12, 75, BLUE, YELLOW, (uint8_t *)"QQ", 16, 1);
    Gui_Drawbmp16(150, 30, gImage_qq);
    Show_Str(150 + 12, 75, BLUE, YELLOW, (uint8_t *)"QQ", 16, 1);
    OS_DelayMs(1200);
}

/*****************************************************************************
 * @name       :void Rotate_Test(void)
 * @date       :2018-08-09
 * @function   :rotate test
 * @parameters :None
 * @retvalue   :None
******************************************************************************/
void Rotate_Test(void)
{
    uint8_t i             = 0;
    uint8_t *Direction[4] = {(uint8_t *)"Rotation:0", (uint8_t *)"Rotation:90",
                          (uint8_t *)"Rotation:180", (uint8_t *)"Rotation:270"};
    LCD_Fill(0, 20, lcddev.width, lcddev.height - 20, WHITE);
    for (i = 0; i < 4; i++) {
        LCD_direction(i);
        //    DrawTestPage((uint8_t *)"≤‚ ‘8:∆¡ƒª–˝◊™≤‚ ‘");
        Show_Str(20, 30, BLUE, YELLOW, Direction[i], 16, 1);
        Gui_Drawbmp16(30, 50, gImage_qq);
        OS_DelayMs(1000);
        OS_DelayMs(1000);
    }
    LCD_direction(USE_HORIZONTAL);
}
