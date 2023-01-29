#include "lib/include.h"

/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
cuya frecuencia sea de 50Hz*/

extern void Configura_Reg_PWM0(uint16_t freq)
{
    // Se habilita PWM0 pag 398 
    SYSCTL->RCGCPWM |= (1<<0); 
    //Se habilita GPIO puerto G y F */
    SYSCTL->RCGCGPIO |= (1<<6) | (1<<5);
    //Se habilita la función alternativa pag. 770
    GPIOG_AHB->AFSEL |= (1<<0); //PG0 
    GPIOF_AHB->AFSEL |= (1<<1) | (1<<2); //PF1 y PF2
    //Se habilita como salida los pines del pwm  1: salida, 0:entrada 
    GPIOG_AHB->DIR = (1<<0);
    GPIOF_AHB->DIR = (1<<1) | (1<<2);
    /*Indicar func. alternativa*/
    GPIOG_AHB->PCTL |= (GPIOG_AHB->PCTL&0xFFFFFFF0) | 0x00000006; //PG0
    GPIOF_AHB->PCTL |= (GPIOF_AHB->PCTL&0xFFFFF00F) | 0x00000660; //PF1 y PF2
    //Para indicar si es 1: digital o  0: analogico 
    GPIOG_AHB->DEN |= (1<<0); //PG0
    GPIOF_AHB->DEN |= (1<<1) | (1<<2); //PF1 PF2
    //Habialitar o desabilitar Divisor e indicar cuál es el valor del divisor pag 1747
    //Se coloca 1  al bit 8 para activar divisor y en el bit 0,1,2 se pone "2" en binario para dividir /8
    PWM0->CC = (1<<8) | (0<<2) | (1<<1) | (0<<0);
    //    Gen
    PWM0->_0_CTL = (0<<0);//se le asigna 0 a 0 porque es el bit para indicar lo habilitado o desabilitado según pag. 1708
    PWM0->_1_CTL = (0<<0);
    PWM0->_2_CTL = (0<<0);
   // Modo de configuración - modo regresivo pag. 1727, pag. 1678 para saber que valor poner, y 1687 para saber con que comparador trabaja el PWM a usar 
   PWM0->_0_GENB |= (0X2<<2)|(0X3<<10)|(0x0<<0);   
   PWM0->_1_GENA |= (0X2<<2)|(0X3<<6)|(0x0<<0);  
   PWM0->_2_GENA |= (0X2<<2)|(0X3<<6)|(0x0<<0); 
                                                                         // frec tiva  /divisor = resultado /frec PWM = cuentas
    // Configuración de cargas: cuentas=fclk/fpwm  para 20MHz,  cuentas = (20,000,000 / 8     = 2500000   /    50   = 50000) 
    PWM0->_0_LOAD = 50000;
    PWM0->_1_LOAD = 50000;
    PWM0->_2_LOAD = 50000;
    // Establecer el valor del ciclo de trabajo
    PWM0->_0_CMPB = 5000;//Cuentas para 2 ms
    PWM0->_1_CMPA = 5000;           
    PWM0->_2_CMPA = 5000;
    //Habilitar generadores a usar
    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);
    PWM0->_2_CTL = (1<<0);
    //Se habilita el PWM que se usa PWM1, PWM2 y PWM4 pag. 1247 y 1233
    PWM0->ENABLE = (1<<1) | (1<<2) | (1<<4);          
}