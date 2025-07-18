#include <stdint.h>
#include "stm32l4xx_hal.h"
#define SPIhandler hspi2
extern SPI_HandleTypeDef SPIhandler;

#define ADXL345_CS_Pin 						GPIO_PIN_6
#define ADXL345_CS_GPIO_Port 			GPIOB
#define SPIhandler 							hspi2

#define POWER_CTL 								0x2D
#define DATA0											0x32
#define DEVID 										0x0
typedef struct {
	uint8_t SPIMode;
	uint8_t IntMode;
	uint8_t LPMode;
	uint8_t Rate;
	uint8_t Range;
	uint8_t Resolution;
	uint8_t Justify;
	uint8_t AutoSleep;
	uint8_t LinkMode;
}ADXL345_InitTypeDef;

typedef enum
{
	ADXL345_OK,
	ADXL345_ERROR
}ADXL345_status;

// ON/OFF switch
typedef enum
{
	ON,
	OFF
}ADXL345_switch;

ADXL345_status ADXL345_Init(ADXL345_InitTypeDef * adxl);
static void ADXL345_read_register(uint8_t address,uint8_t * value, uint8_t num_of_byte);
static void ADXL345_write_register(uint8_t address,uint8_t value);
void ADXL_Measure(ADXL345_switch s);
void ADXL345_get_acceleration(void *Data);