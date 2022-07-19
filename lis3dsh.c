#include "lis3dsh.h"
#include "usbd_cdc_if.h"
#include "stdio.h"

extern SPI_HandleTypeDef hspi1;
char line[22]={0};

static void Error (void)
{
	LED5_RED_ON;
}

uint8_t WriteRead_SPI(uint8_t Input_byte)
{
	uint8_t res_byte = 0;
	if(HAL_SPI_TransmitReceive(&hspi1,(uint8_t*) &Input_byte,(uint8_t*) &res_byte,1,0x1000)!=HAL_OK)
	{
		Error();
	}
	return res_byte;
}


void Read_lis3dsh_IO(uint8_t *pBuffer, uint8_t Addr_read, uint16_t NumByteRead)
{
	if(NumByteRead>0x01)
	{
		Addr_read |= (uint8_t) (RW_CMD | RW_MULTIPLEBYTE_CMD);
	}
	else
	{
		Addr_read |= (uint8_t)RW_CMD;
	}
	CHIP_SELECT_ON;
	WriteRead_SPI(Addr_read);
	while(NumByteRead>0x00)
	{
		*pBuffer=WriteRead_SPI(DUMMY);
		NumByteRead--;
		pBuffer++;
	}
	CHIP_SELECT_OFF;
}

void Write_lis3dsh_IO(uint8_t *pBuffer, uint8_t Addr_write, uint16_t NumByteRead)
{
	CHIP_SELECT_OFF;
	if(NumByteRead>0x01)
	{
		Addr_write |= (uint8_t) RW_MULTIPLEBYTE_CMD;
	}
	CHIP_SELECT_ON;
	WriteRead_SPI(Addr_write);
	while(NumByteRead>=0x01)
	{
		WriteRead_SPI(*pBuffer);
		NumByteRead--;
		pBuffer++;
	}
	CHIP_SELECT_OFF;
}

uint8_t Read_ID_lis3dsh(void)
{
  uint8_t control = 0;
	Read_lis3dsh_IO(&control,WHO_AM_I_CHECK,1);
	return control;
}

void Init_lis3dsh(uint16_t InitStruct)
{
  uint8_t control = 0;
	control=(uint8_t)(InitStruct);
	Write_lis3dsh_IO(&control, LIS3DSH_ADDR_CONTROL_REG4,1);
	control=(uint8_t)(InitStruct>>8);
	Write_lis3dsh_IO(&control, LIS3DSH_ADDR_CONTROL_REG5,1);
}

void GetXYZ_lis3dsh(int16_t* pData)
{
	int8_t buff[6];
	uint8_t control,i = 0x00;
	//float sens = LIS3DSH_SENSITIVITY_0_00006G;
	float value = 0;
	Read_lis3dsh_IO(&control, LIS3DSH_ADDR_CONTROL_REG5,1);
	Read_lis3dsh_IO((uint8_t*)&buff[0], LIS3DSH_OUT_X_LOW_ADDR,1);
	Read_lis3dsh_IO((uint8_t*)&buff[1], LIS3DSH_OUT_X_HIGH_ADDR,1);
	Read_lis3dsh_IO((uint8_t*)&buff[2], LIS3DSH_OUT_Y_LOW_ADDR,1);
	Read_lis3dsh_IO((uint8_t*)&buff[3], LIS3DSH_OUT_Y_HIGH_ADDR,1);
	Read_lis3dsh_IO((uint8_t*)&buff[4], LIS3DSH_OUT_Z_LOW_ADDR,1);
	Read_lis3dsh_IO((uint8_t*)&buff[5], LIS3DSH_OUT_Z_HIGH_ADDR,1);
	for(i=0;i<3;i++)
	{
		//value = ((buff[2*i+1] << 8) + buff[2*i])*sens;
		value = ((buff[2*i+1] << 8) + buff[2*i]);
		pData[i]=(int16_t)value;
	}
}

void LIS3DSH_ReadAcc(void){
	int16_t buff[3]={0};
	int16_t valueX, valueY,valueZ = 0x0000;
	GetXYZ_lis3dsh(buff);
	valueX=buff[0];
	valueY=buff[1];
	valueZ=buff[2];

	printf("X:%06d Y:%06d Z:%06d\n", valueX, valueY, valueZ);

    sprintf(line,"%06d;%06d;%06d\n", valueX, valueY, valueZ);
    CDC_Transmit_FS((uint8_t*)line,strlen(line));


	if((Absolute(valueX))>(Absolute(valueY))){
		if(valueX>1000)
		{
			LED5_RED_ON;
		}
		else if(valueX<-1000){
			LED4_GREEN_ON;
		}
	}
	else{
		if(valueY>1000){
			LED3_ORANGE_ON;
		}
		else if(valueY<-1000){
			LED6_BLUE_ON;
		}
	}
	HAL_Delay(25);
	LED3_ORANGE_OFF;
	LED4_GREEN_OFF;
	LED5_RED_OFF;
	LED6_BLUE_OFF;
}

void LIS3DSH_initialization(void)
{
	uint16_t control = 0x0000;
	HAL_Delay(1000);
	if(Read_ID_lis3dsh()==0x3F) LED4_GREEN_ON;
	else Error();
	control = (uint16_t) (LIS3DSH_DATA_RATE_100HZ | LIS3DSH_XYZ_READ_ENABLE);
	control |= ((uint16_t) (LIS3DSH_SERIAL_INTERFACE_FOUR_WIRE|\
						LIS3DSH_TEST_NORM|\
   						 LIS3DSH_FULLSCALE_2G|\
						 LIS3DSH_FILTER_BANDWIDTH_800HZ))<<8;
	Init_lis3dsh(control);
}

