# 3.5" (320x420) TFT LCD (ILI9488) with SPI interface Driver
This library is a "driver" for the 3.5" (320x420) TFT LCD (ILI9488) with SPI interface.  

The library is platform agnostic and works in polling mode.   

The exposed functions don't cover all the ili9488 functionalities but the bare minimum to make it usable with LVGL.

## How to use this library

- Include the header: `#include "ili9488.h`
- Define the wrapper functions (below an example using STM32 HAL):
```C
void ILI9488_CS_SetState(enum ILI9488_PinState state){
    HAL_GPIO_WritePin(CS_PIN_GPIO_Port, CS_PIN_Pin, (GPIO_PinState)state);
}
void ILI9488_DC_RS_SetState(enum ILI9488_PinState state){
    HAL_GPIO_WritePin(DC_RS_PIN_GPIO_Port, DC_RS_PIN_Pin, (GPIO_PinState)state);
}
void ILI9488_RST_SetState(enum ILI9488_PinState state){
    HAL_GPIO_WritePin(RST_PIN_GPIO_Port, RST_PIN_Pin, (GPIO_PinState)state);
}

enum ILI9488_Status ILI9488_SPI_Transmit(uint8_t *data, uint16_t size){
    // ex using SPI1 peripheral and 10ms timout
    return (enum ILI9488_Status)HAL_SPI_Transmit(&hspi1, data, size, 10ms);
}
void ILI9488_Delay(uint32_t delay){
   HAL_Delay(delay);
}
```
- Define and initialize the `ILI9488_Handle` and call `ILI9488_init()`:
```c
struct ILI9488_Handle hili9488 = {
    .CS_SetState = & ILI9488_CS_SetState,
    .DC_RS_SetState = & ILI9488_DC_RS_SetState,
    .RST_SetState = & ILI9488_RST_SetState,
    .SPI_Transmit = & ILI9488_SPI_Transmit,
    .Delay = & ILI9488_Delay
};
enum ILI9488_Status status = ILI9488_init(&hili9488);
// Check for status == Status_OK
```
