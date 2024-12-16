/**
 * @file    ili9488.c
 * @author  Matteo Giuliani [matteo.giuliani.sc@gmail.com]
 * @date    2024-11-07 (date of creation)
 * @updated 202x-xx-xx (date of last update)
 * @version v0.0.1
 * @prefix  TMP
 *
 * @brief   Source code for the ILI9488 driver
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 
 *          see LICENSE file in the root directory of this software component
 */

/*---------- Includes --------------------------------------------------------*/

#include "ili9488.h"

/*---------- Private define --------------------------------------------------*/

/*---------- Private macro ---------------------------------------------------*/

/*---------- Private variables -----------------------------------------------*/

/*---------- Private function prototypes -------------------------------------*/

/*---------- Exported Variables ----------------------------------------------*/

/*---------- Exported Functions ----------------------------------------------*/

/*---------- Private Functions -----------------------------------------------*/

/*
 * Writes a byte to SPI without changing chip select (CS) state.
 * Called by the write_command() and write_data() functions which
 * control these pins as required.
 */
void spi_write(unsigned char data) {
    HAL_SPI_Transmit(&hspi3, &data, 1, 10);
    while (HAL_SPI_GetState(&hspi3) != HAL_SPI_STATE_READY)
        ;
}

/*
 * Writes a data byte to the display. Pulls CS low as required.
 */
void lcd_write_data(unsigned char data) {
    HAL_GPIO_WritePin(LCD_TFT_DC_GPIO_Port, LCD_TFT_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_TFT_CS_GPIO_Port, LCD_TFT_CS_Pin, GPIO_PIN_RESET);
    if (SPI_MODE)
        spi_write(data);
    //HAL_GPIO_WritePin(LCD_TFT_DC_GPIO_Port, LCD_TFT_DC_Pin, GPIO_PIN_SET);
    HAL_GPIO_WritePin(LCD_TFT_CS_GPIO_Port, LCD_TFT_CS_Pin, GPIO_PIN_SET);
}

/*
 * Writes a command byte to the display
 */
void lcd_write_command(unsigned char data) {
    HAL_GPIO_WritePin(LCD_TFT_DC_GPIO_Port, LCD_TFT_DC_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_TFT_CS_GPIO_Port, LCD_TFT_CS_Pin, GPIO_PIN_RESET);
    if (SPI_MODE)
        spi_write(data);
    HAL_GPIO_WritePin(LCD_TFT_CS_GPIO_Port, LCD_TFT_CS_Pin, GPIO_PIN_SET);
}

/*
 * Swaps two 16-bit integers
 */
void swap_int(unsigned int *num1, unsigned int *num2) {
    int temp = *num2;
    *num2    = *num1;
    *num1    = temp;
}

/*
 * Same as above, but initialises with an SPI port instead.
 */
void lcd_init_spi() {
    //SET control pins for the LCD HIGH (they are active LOW)
    HAL_GPIO_WritePin(LCD_TFT_RST_GPIO_Port, LCD_TFT_RST_Pin, GPIO_PIN_SET);  //RESET pin HIGH (Active LOW)
    HAL_GPIO_WritePin(LCD_TFT_CS_GPIO_Port, LCD_TFT_CS_Pin, GPIO_PIN_SET);    //Chip Select Active LOW
    HAL_GPIO_WritePin(LCD_TFT_DC_GPIO_Port, LCD_TFT_DC_Pin, GPIO_PIN_SET);    //Data / Command select Active LOW
    HAL_GPIO_WritePin(LCD_TFT_RST_GPIO_Port, LCD_TFT_RST_Pin, GPIO_PIN_RESET);
    HAL_Delay(120);
    HAL_GPIO_WritePin(LCD_TFT_RST_GPIO_Port, LCD_TFT_RST_Pin, GPIO_PIN_SET);
    HAL_Delay(120);

    lcd_init_command_list();
}

/**
 * This is the magic initialisation routine.
 */
void lcd_init_command_list(void) {
    //********Start Initial Sequence*******//
    lcd_write_command(0xE0);  //P-Gamma
    lcd_write_data(0x00);
    lcd_write_data(0x13);
    lcd_write_data(0x18);
    lcd_write_data(0x04);
    lcd_write_data(0x0F);
    lcd_write_data(0x06);
    lcd_write_data(0x3A);
    lcd_write_data(0x56);
    lcd_write_data(0x4D);
    lcd_write_data(0x03);
    lcd_write_data(0x0A);
    lcd_write_data(0x06);
    lcd_write_data(0x30);
    lcd_write_data(0x3E);
    lcd_write_data(0x0F);
    lcd_write_command(0XE1);  //N-Gamma
    lcd_write_data(0x00);
    lcd_write_data(0x13);
    lcd_write_data(0x18);
    lcd_write_data(0x01);
    lcd_write_data(0x11);
    lcd_write_data(0x06);
    lcd_write_data(0x38);
    lcd_write_data(0x34);
    lcd_write_data(0x4D);
    lcd_write_data(0x06);
    lcd_write_data(0x0D);
    lcd_write_data(0x0B);
    lcd_write_data(0x31);
    lcd_write_data(0x37);
    lcd_write_data(0x0F);
    lcd_write_command(0xC0);
    lcd_write_data(0x18);
    lcd_write_data(0x16);
    lcd_write_command(0xC1);
    lcd_write_data(0x45);
    lcd_write_command(0xC5);  //VCOM
    lcd_write_data(0x00);
    lcd_write_data(0x63);
    lcd_write_data(0x01);

    lcd_write_command(0x36);  //RAM address mode
    //0xF8 and 0x3C are landscape mode. 0x5C and 0x9C for portrait mode.
    if (LANDSCAPE)
        lcd_write_data(0xF8);
    else
        lcd_write_data(0x5C);

    lcd_write_command(0x3A);  //Interface Mode Control
    lcd_write_data(0x66);     //16-bit serial mode
    lcd_write_command(0xB0);  //Interface Mode Control
    lcd_write_data(0x80);     //SDO not in use
    lcd_write_command(0xB1);  //Frame rate 70HZ
    lcd_write_data(0x00);     //
    lcd_write_data(0x10);
    lcd_write_command(0xB4);
    lcd_write_data(0x02);

    lcd_write_command(0xB6);  //RGB/MCU Interface Control
    lcd_write_data(0x02);
    //lcd_write_data(0x22);

    lcd_write_command(0xE9);
    lcd_write_data(0x00);
    lcd_write_command(0xF7);
    lcd_write_data(0xA9);
    lcd_write_data(0x51);
    lcd_write_data(0x2C);
    lcd_write_data(0x82);
    lcd_write_command(0x11);
    HAL_Delay(120);
    lcd_write_command(0x20);  // change it to 0x21 if you want to invert colors

    HAL_Delay(120);
    lcd_write_command(0x29);
}

/*
 * Sets the X,Y position for following commands on the display.
 * Should only be called within a function that draws something
 * to the display.
 */
void set_draw_window(unsigned int x1, unsigned int y1, unsigned int x2, unsigned int y2) {
    //Check that the values are in order
    if (x2 < x1)
        swap_int(&x2, &x1);
    if (y2 < y1)
        swap_int(&y2, &y1);

    lcd_write_command(ILI9488_CASET);
    lcd_write_data(x1 >> 8);
    lcd_write_data(x1 & 0xFF);

    lcd_write_data(x2 >> 8);
    lcd_write_data(x2 & 0xFF);

    lcd_write_command(ILI9488_PASET);
    lcd_write_data(y1 >> 8);
    lcd_write_data(y1 & 0xFF);

    lcd_write_data(y2 >> 8);
    lcd_write_data(y2 & 0xFF);

    lcd_write_command(ILI9488_RAMWR);
}