/**
 * @file    ili9488.h
 * @author  Matteo Giuliani [matteo.giuliani.sc@gmail.com || glnmatteo0@gmail.com]
 * @date    2024-12-19 (date of creation)
 * @version v0.0.1
 * @prefix  ILI9488
 *
 * @brief   Header file of the ILI9488 driver
 *
 * @license Licensed under "THE BEER-WARE LICENSE", Revision 69 
 *          see LICENSE file in the root directory of this software component
 */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _ILI9488_H_
#define _ILI9488_H_

/* ---------- Includes -------------------------------------------------------*/

#include "gpio.h"
#include "spi.h"
#include "stdint.h"

/* ---------- Exported types -------------------------------------------------*/
struct Gpio_Tuple {
    GPIO_TypeDef *GPIO_Port;
    uint16_t GPIO_Pin;
};

enum ILI9488_PinState { PinState_Reset = 0, PinState_Set };
enum ILI9488_Status { Status_OK = 0, Status_ERR, Status_Busy, Status_Timeout };

struct ILI9488_GPIO_Map {
    struct Gpio_Tuple CS;   
    struct Gpio_Tuple DC;   
    struct Gpio_Tuple RST;  
};

struct ILI9488_Handle {
    void (*CS_SetState)(enum ILI9488_PinState state);
    void (*DC_RS_SetState)(enum ILI9488_PinState state);
    void (*RST_SetState)(enum ILI9488_PinState state);
    enum ILI9488_Status (*SPI_Transmit_DMA)(uint8_t *data, uint16_t size);
    void (*Delay)(uint32_t delay);  // Delay in ms
};

/* ---------- Exported constants ---------------------------------------------*/
// Parameter settings
#define HORIZONTAL_RES 480
#define VERTICAL_RES   320
#define LANDSCAPE      1

// Registers (DO NOT CHANGE)
#define ILI9488_NOP     0x00
#define ILI9488_SWRESET 0x01
#define ILI9488_RDDID   0x04
#define ILI9488_RDDST   0x09

#define ILI9488_SLPIN  0x10
#define ILI9488_SLPOUT 0x11
#define ILI9488_PTLON  0x12
#define ILI9488_NORON  0x13

#define ILI9488_RDMODE     0x0A
#define ILI9488_RDMADCTL   0x0B
#define ILI9488_RDPIXFMT   0x0C
#define ILI9488_RDIMGFMT   0x0D
#define ILI9488_RDSELFDIAG 0x0F

#define ILI9488_INVOFF   0x20
#define ILI9488_INVON    0x21
#define ILI9488_GAMMASET 0x26
#define ILI9488_DISPOFF  0x28
#define ILI9488_DISPON   0x29

#define ILI9488_CASET 0x2A
#define ILI9488_PASET 0x2B
#define ILI9488_RAMWR 0x2C
#define ILI9488_RAMRD 0x2E

#define ILI9488_PTLAR  0x30
#define ILI9488_MADCTL 0x36
#define ILI9488_PIXFMT 0x3A

#define ILI9488_FRMCTR1 0xB1
#define ILI9488_FRMCTR2 0xB2
#define ILI9488_FRMCTR3 0xB3
#define ILI9488_INVCTR  0xB4
#define ILI9488_DFUNCTR 0xB6

#define ILI9488_PWCTR1 0xC0
#define ILI9488_PWCTR2 0xC1
#define ILI9488_PWCTR3 0xC2
#define ILI9488_PWCTR4 0xC3
#define ILI9488_PWCTR5 0xC4
#define ILI9488_VMCTR1 0xC5
#define ILI9488_VMCTR2 0xC7

#define ILI9488_RDID1 0xDA
#define ILI9488_RDID2 0xDB
#define ILI9488_RDID3 0xDC
#define ILI9488_RDID4 0xDD

#define ILI9488_GMCTRP1 0xE0
#define ILI9488_GMCTRN1 0xE1

/* ---------- Exported variables ---------------------------------------------*/
/* ---------- Exported macros ------------------------------------------------*/
/* ---------- Exported functions ---------------------------------------------*/
enum ILI9488_Status ILI9488_write_data(struct ILI9488_Handle *handle, uint8_t data);
enum ILI9488_Status ILI9488_write_command(struct ILI9488_Handle *handle, uint8_t cmd);
enum ILI9488_Status ILI9488_init(struct ILI9488_Handle *handle, struct ILI9488_GPIO_Map *map);
enum ILI9488_Status ILI9488_set_draw_window(struct ILI9488_Handle *handle,
                                            uint16_t x1,
                                            uint16_t y1,
                                            uint16_t x2,
                                            uint16_t y2);
enum ILI9488_Status ILI9488_SPI_Send_DMA(struct ILI9488_Handle *handle, uint8_t *data, uint16_t size);

void ILI9488_CS_Pin_SetState(enum ILI9488_PinState state);
void ILI9488_DC_Pin_SetState(enum ILI9488_PinState state);
void ILI9488_RST_Pin_SetState(enum ILI9488_PinState state);
enum ILI9488_Status ILI9488_SPI_Transmit_DMA(uint8_t *data, uint16_t size);
void ILI9488_Delay(uint32_t delay_ms);
/* ---------- Private types --------------------------------------------------*/
/* ---------- Private variables ----------------------------------------------*/
/* ---------- Private constants ----------------------------------------------*/
/* ---------- Private Macros -------------------------------------------------*/

#endif
