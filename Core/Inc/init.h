#ifndef INIT_H

#include "stm32f4xx.h"
#include "stm32f429xx.h"
#include "stdio.h"

// Светодиоды на PB0, PB7, PB14
// Кнопки на PC6, PC7

#define RCC_TACT *(uint32_t*)(0x40023800UL + 0x30)

#define GPIO_B_RCC_ON 0x02UL // Включение GPIOB

// Включение GPIOB

#define MODERB_ *(uint32_t*)(0x40020400UL + 0x00)

#define MODERB7_ON 0x4000UL
#define MODERB0_ON 0x1UL
#define MODERB14_ON 0x10000000UL

#define MODERC6_ON_INPUT 0x00
#define MODERC7_ON_INPUT 0x00

// Настройка режимов выхода

#define OTYPER_ *(uint32_t*)(0x40020400UL + 0x04)
#define OTYPER_ON 0x0000000000000000UL // Режим пуш-пулл

// Настрока режим работы порта 

#define OSPEEDR_ *(uint32_t*)(0x40020400 + 0x08)
#define OSPEEDRB_0_ON 0x01UL
#define OSPEEDRB_7_ON 0x4000UL
#define OSPEEDRB_14_ON 0x10000000UL

// Настройка скорости работы

#define PUPDR_ *(uint32_t*)(0x40020400 + 0x0C)
#define PUPDRB_0_ON 0x00UL
#define PUPDRB_7_ON 0x00UL
#define PUPDRB_14_ON 0x00UL // но пулл-ап и но пулл-аут

// Настройка подтяжки 

#define BSRR_ *(uint32_t*)(0x40020400UL + 0x18)

#define BSRRB_0_ON 0x01UL
#define BSRRB_0_OFF 0x010000UL

#define BSRRB_7_ON 0x080UL
#define BSRRB_7_OFF 0x0800000UL

#define BSRRB_14_ON 0x04000UL
#define BSRRB_14_OFF 0x040000000UL

// 

// Кнопка на РС6, РС7

//

#define GPIO_C_RCC_ON 0x04UL // Включение GPIOC

#define MODERC_ *(uint32_t*)(0x40020800UL + 0x00)

#define MODERC6_ON 0x00000000
#define MODERC7_ON 0x00000000 // Настроили пины на вход

#define OSPEEDRC_ *(uint32_t*)(0x40020800UL + 0x08)

#define OSPEEDRC6_ON 0x1000
#define OSPEEDRC7_ON 0x4000 // Поставил для кнопок среднюю скорость

#define PUPDRC_ *(uint32_t*)(0x40020800UL + 0x0C)

#define PUPDRC6_ON 0x1000
#define PUPDRC7_ON 0x4000 // Настроил братков на пулл-ап

#define IDRC_ *(uint32_t*)(0x40020800UL + 0x10)

#define IDRC6_ 0x40
#define IDRC7_ 0x80

#define SETBIT(REG, ACT) ((REG) |= (ACT))

// Маски

void macros_init(void);
void cmsis_init(void);
void simple_init(void);

// Прототипы функций для определения регистров

uint8_t pressed_button(uint32_t mask);

uint32_t blick_LED_mode(uint32_t mode);

void glow_LED(uint8_t LED1, uint8_t LED2,uint8_t LED3, uint8_t mode);

#endif