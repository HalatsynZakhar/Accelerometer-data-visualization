#ifndef INC_LIS3DSH_H_
#define INC_LIS3DSH_H_

/*Заголовні файли бібліотеки HAL та string*/
#include "stm32f4xx_hal.h"
#include <string.h>
//------------------------------------------------
/*Функція для знаходження модуля числа*/
#define Absolute(x)         (x > 0) ? x : -x
//------------------------------------------------
/*світлодіоди*/
#define LED_PORTD GPIOD /*порт D*/
#define LED3_ORANGE GPIO_PIN_13 //Помаранчевий
#define LED4_GREEN GPIO_PIN_12 //Зелений
#define LED5_RED GPIO_PIN_14 //Червоний
#define LED6_BLUE GPIO_PIN_15 //Синій
/*Увімкнення світлодіода*/
#define LED3_ORANGE_ON HAL_GPIO_WritePin(LED_PORTD, LED3_ORANGE, GPIO_PIN_SET) //Помаранчевий
#define LED4_GREEN_ON HAL_GPIO_WritePin(LED_PORTD, LED4_GREEN, GPIO_PIN_SET) //Зелений
#define LED5_RED_ON HAL_GPIO_WritePin(LED_PORTD, LED5_RED, GPIO_PIN_SET) //Червоний
#define LED6_BLUE_ON HAL_GPIO_WritePin(LED_PORTD, LED6_BLUE, GPIO_PIN_SET) //Синій
/*Вимкнення світлодіода*/
#define LED3_ORANGE_OFF HAL_GPIO_WritePin(LED_PORTD, LED3_ORANGE, GPIO_PIN_RESET) //Помаранчевий
#define LED4_GREEN_OFF HAL_GPIO_WritePin(LED_PORTD, LED4_GREEN, GPIO_PIN_RESET) //Зелений
#define LED5_RED_OFF HAL_GPIO_WritePin(LED_PORTD, LED5_RED, GPIO_PIN_RESET) //Червоний
#define LED6_BLUE_OFF HAL_GPIO_WritePin(LED_PORTD, LED6_BLUE, GPIO_PIN_RESET) //Синій
//------------------------------------------------
/*Chip Select*/
#define CHIP_SELECT_GPIO_PORT GPIOE
#define CHIP_SELECT_PIN3 GPIO_PIN_3/*Третій пін*/
#define CHIP_SELECT_ON HAL_GPIO_WritePin(CHIP_SELECT_GPIO_PORT, CHIP_SELECT_PIN3, GPIO_PIN_RESET)/*Ввімкнути*/
#define CHIP_SELECT_OFF HAL_GPIO_WritePin(CHIP_SELECT_GPIO_PORT, CHIP_SELECT_PIN3, GPIO_PIN_SET)/*Вимкнути*/
#define DUMMY ((uint8_t)0x00)/*заглушка байт*/
//------------------------------------------------
#define WHO_AM_I_CHECK 0x0F /*Перевірка роботи датчика*/
//------------------------------------------------
//Маскування адреси
#define RW_CMD ((uint8_t)0x80)/*Для зчитування*/
#define RW_MULTIPLEBYTE_CMD ((uint8_t)0x40)/*Для зчитування декількох байтів*/
//------------------------------------------------
/*Адреси ініціалізації регістрів */
#define LIS3DSH_ADDR_CONTROL_REG4           0x20
#define LIS3DSH_ADDR_CONTROL_REG5           0x24
//------------------------------------------------
/*Частота зчитування датчика*/
#define LIS3DSH_DR_POWERDOWN				((uint8_t)0x00)  /* Power Down Mode*/
#define LIS3DSH_DATA_RATE_3_125HZ			((uint8_t)0x10)  /* 3.125 Гц Нормальний режим*/
#define LIS3DSH_DATA_RATE_6_25HZ			((uint8_t)0x20)  /* 6.25  Гц Нормальний режим */
#define LIS3DSH_DATA_RATE_12_5HZ			((uint8_t)0x30)  /* 12.5  Гц Нормальний режим */
#define LIS3DSH_DATA_RATE_25HZ				((uint8_t)0x40)  /* 25    Гц Нормальний режим */
#define LIS3DSH_DATA_RATE_50HZ				((uint8_t)0x50)  /* 50    Гц Нормальний режим */
#define LIS3DSH_DATA_RATE_100HZ				((uint8_t)0x60)  /* 100   Гц Нормальний режим */
#define LIS3DSH_DATA_RATE_400HZ				((uint8_t)0x70)  /* 400   Гц Нормальний режим */
#define LIS3DSH_DATA_RATE_800HZ             ((uint8_t)0x80)  /* 800   Гц Нормальний режим */
#define LIS3DSH_DATA_RATE_1600HZ            ((uint8_t)0x90)  /* 1600  Гц Нормальний режим */
//------------------------------------------------
/*Ввімкнення для зчитування всіх осей*/
#define LIS3DSH_X_READ_ENABLE               ((uint8_t)0x01)
#define LIS3DSH_Y_READ_ENABLE               ((uint8_t)0x02)
#define LIS3DSH_Z_READ_ENABLE               ((uint8_t)0x04)
#define LIS3DSH_XYZ_READ_ENABLE             ((uint8_t)0x07)
//------------------------------------------------
/* Режим шини для чотирьох проводів */
#define LIS3DSH_SERIAL_INTERFACE_FOUR_WIRE  ((uint8_t)0x00)
//------------------------------------------------
/* Режим самотестування */
#define LIS3DSH_TEST_NORM                	((uint8_t)0x00)
#define LIS3DSH_TEST_P                   	((uint8_t)0x02)
#define LIS3DSH_TEST_M                   	((uint8_t)0x04)
//------------------------------------------------
/* Амплітуда */
#define LIS3DSH_FULLSCALE_2G                 ((uint8_t)0x00)  /* 2 g  */
#define LIS3DSH_FULLSCALE_4G                 ((uint8_t)0x08)  /* 4 g  */
#define LIS3DSH_FULLSCALE_6G                 ((uint8_t)0x10)  /* 6 g  */
#define LIS3DSH_FULLSCALE_8G                 ((uint8_t)0x18)  /* 8 g  */
#define LIS3DSH_FULLSCALE_16G                ((uint8_t)0x20)  /* 16 g */
#define LIS3DSH__FULLSCALE_SELECT            ((uint8_t)0x38)
//------------------------------------------------
 /* Фільтри для зглажування   */
#define LIS3DSH_FILTER_BANDWIDTH_800HZ       ((uint8_t)0x00)  /* 800 Гц */
#define LIS3DSH_FILTER_BANDWIDTH_400HZ       ((uint8_t)0x08)  /* 400 Гц  */
#define LIS3DSH_FILTER_BANDWIDTH_200HZ       ((uint8_t)0x10)  /* 200 Гц */
#define LIS3DSH_FILTER_BANDWIDTH_50HZ        ((uint8_t)0x18)  /* 50 Гц  */
//------------------------------------------------
/*Чутливість датчика*/
#define LIS3DSH_SENSITIVITY_0_00006G          0.06  /* 0.06 mg/цифру*/
#define LIS3DSH_SENSITIVITY_0_000012G         0.12  /* 0.12 mg/цифру*/
#define LIS3DSH_SENSITIVITY_0_000018G         0.18  /* 0.18 mg/цифру*/
#define LIS3DSH_SENSITIVITY_0_000024G         0.24  /* 0.24 mg/цифру*/
#define LIS3DSH_SENSITIVITY_0_000073G         0.73  /* 0.73 mg/цифру*/
//------------------------------------------------
/*Адреси зчитування даних*/
#define LIS3DSH_OUT_X_LOW_ADDR                0x28
#define LIS3DSH_OUT_X_HIGH_ADDR               0x29
#define LIS3DSH_OUT_Y_LOW_ADDR                0x2A
#define LIS3DSH_OUT_Y_HIGH_ADDR               0x2B
#define LIS3DSH_OUT_Z_LOW_ADDR                0x2C
#define LIS3DSH_OUT_Z_HIGH_ADDR               0x2D
//------------------------------------------------
/*Функція для ініціалізації акселерометра*/
void LIS3DSH_initialization(void);
/*Функція для зчитування даних з акселерометра*/
void LIS3DSH_ReadAcc(void);

#endif /* INC_LIS3DSH_H_ */
