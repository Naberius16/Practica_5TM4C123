#include "lib/include.h"

extern void Configura_Reg_PWM1(uint16_t freq)
{
    /*Se habilita modulo PWM0 pag 354*/
    SYSCTL->RCGCPWM |= (1<<0);
    /*Habialitar o desabilitar Divisor  Pag 254*/                                                                 //  frec, tarjeta / frec PWM = cuentas 
    SYSCTL->RCC &= ~(1<<20); //Se desabilita porque salen 5 mil cuentas que caben en los 16 bits del contador del PWM, 50 millinoes/ 10 mil = 5 mil cuentas  
    /*Se habilita la función alternativa para PB4 Pag 672*/
    GPIOB->AFSEL |= (1<<4);
    //Se indica como salida el PB4
    GPIOB->DIR = (1<<4); 
    /*Combinado con la tabla Pag 1351 y el registro PCTL le digo que es pwm Pag 689, por eso se pone el valor de 4 en el pin PB4*/
    GPIOB->PCTL |= (GPIOB->PCTL&0xFFF0FFFF) | 0x00040000; 
    /* para decirle si es digital o no Pag 682*/
    GPIOB->DEN |= (1<<4); 
    // Se desactiva generador acorde a tabla pag 1233 para saber con que gen. se trabaja y pag. 1266 para saber a que bit asignarle el valor
    PWM0->_1_CTL = (0<<0); //se le asigna 0 a 0 pag. 1266
    // Modo de configuración - modo regresivo pag. 1285, pag. 1239 para saber que valor poner, y 1248 para saber con que comparador trabaja el PWM
    PWM0->_1_GENA = 0x0000008C; //se usa el comparador A que es con el que trabaja el PWM2 según pag. 1248
    // Configuración de cargas
    PWM0->_1_LOAD = 5000;  
    PWM0->_1_CMPA = 3750; 
    //Habilitar generadores a usar
    PWM0->_1_CTL = (1<<0);// Activa el generador 1
    //Se habilita el PWM que se usa PWM2 pag. 1247 y 1233
    PWM0->ENABLE = (1<<2); 
}
