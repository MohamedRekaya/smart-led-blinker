#include "stm32f4xx.h"



int main(){

	RCC -> AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	GPIOD -> MODER |= 0X55000000U;

	while(1){

		GPIOD -> ODR ^= 0Xf000U;
		int delay = 1000000;
		while ( --delay ){

		}
	}



}
