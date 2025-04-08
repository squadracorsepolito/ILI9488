/**
 * @file    ili9488.c
 * @author  Matteo Giuliani [matteo.giuliani.sc@gmail.com]
 * @date    2024-12-19 (date of creation)
 * @version v0.0.1
 * @prefix  ILI9488
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
static void ILI9488_swap_int(uint16_t *num1, uint16_t *num2);
static enum ILI9488_Status ILI9488_init_command_list(struct ILI9488_Handle *handle);
/*---------- Exported Variables ----------------------------------------------*/

/*---------- Exported Functions ----------------------------------------------*/

/*---------- Private Functions -----------------------------------------------*/

/*
 * Writes a data byte to the display. Pulls CS low as required.
 */
//void ILI9488_write_data(unsigned char data) {
enum ILI9488_Status ILI9488_write_data(struct ILI9488_Handle *handle, uint8_t data) {
    enum ILI9488_Status ret = Status_OK;
    handle->DC_RS_SetState(PinState_Set);
    handle->CS_SetState(PinState_Reset);
    ret = handle->SPI_Transmit_DMA(&data, 1);
    handle->CS_SetState(PinState_Set);
    return ret;
}

/*
 * Writes a command byte to the display
 */
enum ILI9488_Status ILI9488_write_command(struct ILI9488_Handle *handle, uint8_t cmd) {
    enum ILI9488_Status ret = Status_OK;
    handle->DC_RS_SetState(PinState_Reset);
    handle->CS_SetState(PinState_Reset);
    ret = handle->SPI_Transmit_DMA(&cmd, 1);
    handle->CS_SetState(PinState_Set);
    return ret;
}

/*
 * Swaps two 16-bit integers
 */
static void ILI9488_swap_int(uint16_t *num1, uint16_t *num2) {
    int temp = *num2;
    *num2    = *num1;
    *num1    = temp;
}

/*
 * Same as above, but initialises with an SPI port instead.
 */
#include <stddef.h>
enum ILI9488_Status ILI9488_init(struct ILI9488_Handle *handle) {
    if (handle == NULL)
        return Status_ERR;

    if (handle->CS_SetState == NULL || handle->DC_RS_SetState == NULL || handle->RST_SetState == NULL ||
        handle->SPI_Transmit_DMA == NULL || handle->Delay == NULL)
        return Status_ERR;

    // SET control pins for the LCD HIGH (they are active LOW)
    handle->RST_SetState(PinState_Set);    // RESET pin HIGH (Active LOW)
    handle->CS_SetState(PinState_Set);     // Chip Select Active LOW
    handle->DC_RS_SetState(PinState_Set);  // Data / Command select Active LOW
    handle->RST_SetState(PinState_Reset);
    handle->Delay(120);
    handle->RST_SetState(PinState_Set);
    handle->Delay(120);

    return ILI9488_init_command_list(handle);
}

/**
 * This is the magic initialisation routine.
 */
static enum ILI9488_Status ILI9488_init_command_list(struct ILI9488_Handle *handle) {
    uint8_t tmp = 0;
    //********Start Initial Sequence*******//
    tmp += (uint8_t)ILI9488_write_command(handle, 0xE0);  // P-Gamma
    tmp += (uint8_t)ILI9488_write_data(handle, 0x00);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x13);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x18);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x04);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x0F);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x06);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x3A);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x56);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x4D);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x03);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x0A);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x06);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x30);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x3E);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x0F);
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0XE1);  // N-Gamma
    tmp += (uint8_t)ILI9488_write_data(handle, 0x00);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x13);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x18);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x01);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x11);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x06);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x38);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x34);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x4D);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x06);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x0D);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x0B);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x31);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x37);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x0F);
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0xC0);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x18);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x16);
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0xC1);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x45);
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0xC5);  // VCOM
    tmp += (uint8_t)ILI9488_write_data(handle, 0x00);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x63);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x01);
    if (tmp == 1)
        return Status_ERR;

    tmp += (uint8_t)ILI9488_write_command(handle, 0x36);  // RAM address mode
    // 0xF8 and 0x3C are landscape mode. 0x5C and 0x9C for portrait mode.
    if (ILI9488_LANDSCAPE)
        tmp += (uint8_t)ILI9488_write_data(handle, 0xF8);
    else
        tmp += (uint8_t)ILI9488_write_data(handle, 0x5C);
    if (tmp == 1)
        return Status_ERR;

    tmp += (uint8_t)ILI9488_write_command(handle, 0x3A);  // Interface Mode Control
    tmp += (uint8_t)ILI9488_write_data(handle, 0x66);     // 16-bit serial mode
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0xB0);  // Interface Mode Control
    tmp += (uint8_t)ILI9488_write_data(handle, 0x80);     // SDO not in use
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0xB1);  // Frame rate 70HZ
    tmp += (uint8_t)ILI9488_write_data(handle, 0x00);     //
    tmp += (uint8_t)ILI9488_write_data(handle, 0x10);
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0xB4);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x02);
    if (tmp == 1)
        return Status_ERR;

    tmp += (uint8_t)ILI9488_write_command(handle, 0xB6);  // RGB/MCU Interface Control
    tmp += (uint8_t)ILI9488_write_data(handle, 0x02);
    // tmp += (uint8_t)ILI9488_write_data(handle,0x22);
    if (tmp == 1)
        return Status_ERR;

    tmp += (uint8_t)ILI9488_write_command(handle, 0xE9);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x00);
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0xF7);
    tmp += (uint8_t)ILI9488_write_data(handle, 0xA9);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x51);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x2C);
    tmp += (uint8_t)ILI9488_write_data(handle, 0x82);
    if (tmp == 1)
        return Status_ERR;
    tmp += (uint8_t)ILI9488_write_command(handle, 0x11);
    if (tmp == 1)
        return Status_ERR;
    handle->Delay(120);
    tmp += (uint8_t)ILI9488_write_command(handle, 0x20);  // change it to 0x21 if you want to invert colors
    if (tmp == 1)
        return Status_ERR;

    handle->Delay(120);
    tmp += (uint8_t)ILI9488_write_command(handle, 0x29);
    if (tmp == 1)
        return Status_ERR;
    return Status_OK;
}

/*
 * Sets the X,Y position for following commands on the display.
 * Should only be called within a function that draws something
 * to the display.
 */
enum ILI9488_Status ILI9488_set_draw_window(struct ILI9488_Handle *handle,
                                            uint16_t x1,
                                            uint16_t y1,
                                            uint16_t x2,
                                            uint16_t y2) {
    uint8_t tmp;
    // Check that the values are in order
    if (x2 < x1)
        ILI9488_swap_int(&x2, &x1);
    if (y2 < y1)
        ILI9488_swap_int(&y2, &y1);

    tmp += (uint8_t)ILI9488_write_command(handle, ILI9488_CASET);
    tmp += (uint8_t)ILI9488_write_data(handle, x1 >> 8);
    tmp += (uint8_t)ILI9488_write_data(handle, x1 & 0xFF);

    tmp += (uint8_t)ILI9488_write_data(handle, x2 >> 8);
    tmp += (uint8_t)ILI9488_write_data(handle, x2 & 0xFF);
    if (tmp == 1)
        return Status_ERR;

    tmp += (uint8_t)ILI9488_write_command(handle, ILI9488_PASET);
    tmp += (uint8_t)ILI9488_write_data(handle, y1 >> 8);
    tmp += (uint8_t)ILI9488_write_data(handle, y1 & 0xFF);

    tmp += (uint8_t)ILI9488_write_data(handle, y2 >> 8);
    tmp += (uint8_t)ILI9488_write_data(handle, y2 & 0xFF);
    if (tmp == 1)
        return Status_ERR;

    tmp += (uint8_t)ILI9488_write_command(handle, ILI9488_RAMWR);
    if (tmp == 1)
        return Status_ERR;

    return Status_OK;
}

//TODO add a comment that explain that is called after ILI9488_set_draw_window
enum ILI9488_Status ILI9488_draw(struct ILI9488_Handle *handle, uint8_t *data, uint16_t size) {
    handle->DC_RS_SetState(PinState_Set);  // RESET pin HIGH (Active LOW)
    handle->CS_SetState(PinState_Reset);   // RESET pin HIGH (Active LOW)

    enum ILI9488_Status status = handle->SPI_Transmit_DMA(data, size);
    return status;
}