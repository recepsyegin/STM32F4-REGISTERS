#include "stm32f407xx.h"



void SysClockConfig(void)
{
	
	#define PLL_M 4
	#define PLL_N 180
	#define PLL_P 0   // PLLP = 2
	
	
	// STEPS FOLLOWED
	
	// 1. Enable HSE and wait for the HSE to become READY
	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY));
	
	//2.Set the POWER ENABLE CLOCK and VOLTAGE REGULATOR
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	
	PWR->CR |= PWR_CR_VOS;
	
	//3. Configure the FLASH PREFETCH and the LATENCY Related Settings
	FLASH->ACR |= FLASH_ACR_ICEN | FLASH_ACR_DCEN | FLASH_ACR_PRFTEN | FLASH_ACR_LATENCY_5WS;
	
	
	//4. Configure the PRESCALARS HCLK,PCLK1,PCLK2
	
	//AHB PR
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;
	
	//APB1 PR
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;
	
	//APB2 PR
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV2;
	
	//5.Configure the MAIN PLL
	RCC->PLLCFGR = (PLL_M << 0) | (PLL_N << 6) | (PLL_P) << 16 | (RCC_PLLCFGR_PLLSRC_HSE);
	
	//6.Enable the PLL and wait for it to become reaedy
	RCC->CR |= RCC_CR_PLLON;
	while(!(RCC->CR & RCC_CR_PLLRDY));
	
	//7. Select the Clock Source and wait for it to be set
	RCC->CFGR |= RCC_CFGR_SW_PLL;
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_PLL);
}


void GPIO_Config()
{
	/*****************STEPS FOLLOWED*****************/
	
	// 1.ENABLE THE GPIO CLOCK
	// 2.SET THE PIN AS OUTPUT
	// 3.CONFIGURE THE OUTPUT MODE
	
	/************************************************/
	
	// 1-ENABLE the GPIO Clock
	RCC->AHB1ENR |= (1<<3);
	
	// 2-SET THE PIN AS OUTPUT - > "GPIO_D : 12,13,14,15" (STM32F407 IÇIN GPIOD 12,13,14,15 -> Dahili LED'ler)
	GPIOD->MODER |= (1<<24) | (1<<26) | (1<<28) | (1<<30);		
	
	
	// 3-CONFIGURE THE OUTPUT MODE
	GPIOD->OTYPER = 0U;
	GPIOD->OSPEEDR = 0U;
}



void Delay(uint32_t time)
{
	while(time--);
}

int main(void)
{
	
	SysClockConfig();
	GPIO_Config();


	
	while(1)
	{
		GPIOD->BSRR |= (1<<12) | (1<<13) | (1<<14) | (1<<15);
		Delay(10000000);
		GPIOD->BSRR |= (1<<28) | (1<<29) | (1<<30) | (1<<31);
		Delay(10000000);
	}
	
	
	
	
} //  "Main sonu"