#include <stdint.h>

#include "adxl345.h"

float GAINX = 0.0039f;
float GAINY = 0.0039f;
float GAINZ = 0.0039f;

ADXL345_status ADXL345_Init(ADXL345_InitTypeDef * adxl)
{
	// Set CS pin high, as CS is active low
	HAL_GPIO_WritePin(ADXL345_CS_GPIO_Port,ADXL345_CS_Pin,GPIO_PIN_SET);
	
	HAL_Delay(10);
	
	uint8_t test_value = 0;
	
	// The Device Address register is constant which is 0xE5
	ADXL345_read_register(DEVID,&test_value,1);
	
	// Check if the received value is 0xE5
	if (test_value != 0xE5) 
	{
		return ADXL345_ERROR;
	}
			

	return ADXL345_OK;

}

static void ADXL345_write_register(uint8_t address,uint8_t value)
{
	// Set R/W = 0 for Write Mode
  address &= ~(1U<<7);
	
	// Set CS pin low to start the commuication
	HAL_GPIO_WritePin(ADXL345_CS_GPIO_Port,ADXL345_CS_Pin,GPIO_PIN_RESET);
	
	// Transmit the address 
	HAL_SPI_Transmit(&SPIhandler,&address,1,10);
	
	// Transmit the value
	HAL_SPI_Transmit(&SPIhandler,&value,1,10);
	
	// Reset CS ppin to stop the communication
	HAL_GPIO_WritePin(ADXL345_CS_GPIO_Port,ADXL345_CS_Pin,GPIO_PIN_SET);
	

}

static void ADXL345_read_register(uint8_t address,uint8_t * value, uint8_t num_of_byte)
{

		// Multiple Byte Read Settings
		if (num_of_byte > 1)
		{
		// Set the MB bit (6th bit)
		address |= (1U<<6);
		}
		else	
		{
			address &= ~(1U<<6);
		}
		
		// Set R/W bit = 1 for Read Mode
    address |= (1U<<7);		
		
		// Set CS pin low to start the commuication
		HAL_GPIO_WritePin(ADXL345_CS_GPIO_Port,ADXL345_CS_Pin,GPIO_PIN_RESET);
		
		// Transmit the address 
		HAL_SPI_Transmit(&SPIhandler,&address,1,10);
		
		// Receive the value and store it on the value variable
		HAL_SPI_Receive(&SPIhandler,value,num_of_byte,10);
		
		// Reset CS ppin to stop the communication
		HAL_GPIO_WritePin(ADXL345_CS_GPIO_Port,ADXL345_CS_Pin,GPIO_PIN_SET);
	
}

void ADXL_Measure(ADXL345_switch s)
		{
			uint8_t reg;
			ADXL345_read_register(POWER_CTL,&reg,1);
			switch (s) {
				case ON: 
					reg &= ~(1<<2);  // turn on normal mode instad of sleep mode
					reg |= (1<<3);   // turn on measure mode
					ADXL345_write_register(POWER_CTL,reg);
					break;
				
				case OFF:
					reg &= ~(1<<3); // turn on standby mode
					ADXL345_write_register(POWER_CTL,reg);
					break;				
				}
		}

void ADXL345_get_acceleration(void *Data)
	{
	uint8_t data[6]={0,0,0,0,0,0};	
	
	// DATA = 0x32, which 0x32 to 0x37 is address for All data
	//0x32, 0x33 DATAX0, DATAX1 (read only)
  //0x34, 0x35 DATAY0, DATAY1 (read only)
  //0x36, 0x37 DATAZ0, DATAZ1 (read only) 
	
	ADXL345_read_register(DATA0,data,6);

	float * fdata = Data;
	fdata[0] = ( (int16_t) ((data[1] << 8) | data[0])) * GAINX;
  fdata[1] = ( (int16_t) ((data[3] << 8) | data[2])) * GAINY;
  fdata[2] = ( (int16_t) ((data[5] << 8) | data[4])) * GAINZ;
	
	/* for instance, Combine Bytes: (data[1] << 8) | data[0]

		(0x01 << 8) results in 0x0100 (binary 0000000100000000)
		 0x0100 | 0x34 results in 0x0134 (binary 0000000100110100) */
	}
	