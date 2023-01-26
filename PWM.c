/*
 * ADC.c
 *
 *  Created on: 26/12/2022
 *      Author: Ashley Roxana Reséndiz Sánchez
 */
 #include "lib/include.h"

//Se trabaja con modulo 0 , generador 0, 1, 2 
//por lo tanto se puede trabajar con GEN 0: PB7-> PMW1, GEN 1: PB4-> PMW2 Y GEN 2: PE4-> PWM4 de acuerdo a la pág. 1233

extern void Configura_Reg_PWM1(uint16_t freq)
{
    //Se habilita modulo PWM0 pag 354
    SYSCTL->RCGCPWM |= (1<<0);
    
    //Habilita GPIO Puerto B y E porque se trabaja con gen. 1 mod. 0 pag 340 y pag. 1233 para saber que puertos/pines corresponden a cada modulo de PWM y cada generador/
    SYSCTL->RCGCGPIO |= (1<<1)|(1<<4);
    
    //Habialitar o desabilitar Divisor  Pag 254/                                                                  //  frec, tarjeta / frec PWM = cuentas 
    SYSCTL->RCC &=~(1<<20); //Se Deshabilita porque salen 40 mil cuentas que caben en los 16 bits del contador del PWM, 20 millinoes/ 5000 = 40 mil cuentas  
                                                    
    //Se habilita la función alternativa para PB4 Pag 672
    GPIOB->AFSEL |= (1<<4) | (1<<7); //PB4 PB7
    GPIOE->AFSEL |= (1<<4); //PE4
    
    //Combinado con la tabla Pag 1351 y el registro PCTL le digo que es pwm Pag 689, por eso se pone el valor de 4 en el pin PB4
    GPIOB->PCTL |= (GPIOB->PCTL&0x0FF0FFFF) | 0x40040000; // Para PB7 y PB4
    GPIOE->PCTL |= (GPIOE->PCTL&0xFFF0FFFF) | 0x00040000; //Para PE4

    //Para decirle si es digital o no Pag 682
    GPIOB->DEN |= (1<<4)|(1<<7); 
    GPIOE->DEN |= (1<<4); 

    //Indicar si son Entrada (0) o Salidas (1)
    GPIOB->DIR |= (1<<4)|(1<<7); 
    GPIOE->DIR |= (1<<4); 

    // Se desactiva generador acorde a tabla 1233 para saber con que gen. se trabaja y pag. 1266 para saber a que bit asignarle el valor
    //    Gen 
    PWM0->_0_CTL = (0<<0);
    PWM0->_1_CTL = (0<<0); //se le asigna 0 a 0 porque es el bit para indicar lo habilitado o desabilitado según pag. 1266
    PWM0->_2_CTL = (0<<0);

    // Modo de configuración - modo regresivo pag. 1285, pag. 1239 para saber que valor poner, y 1248 para saber con que comparador trabaja el PWM a usar 
    PWM0->_0_GENB = 0x0000080C; //Gen 0:  PWM 1 que trabaja con comparador B
    PWM0->_1_GENA = 0x0000008C; //se usa el comparador A que es con el que trabaja el PWM2 según pag. 1248
    PWM0->_2_GENA = 0x0000008C; //Gen 2:  PWM 4 trabaja con comparador A
                                                                        // frec tiva  /divisor = resultado  /frec PWM = cuentas
    // Configuración de cargas: cuentas=fclk/fpwm  para 20MHz,  cuentas = (20,000,000 / 0    = 20 000 000   /    400   = 50 000)
    PWM0->_0_LOAD = 50000; 
    PWM0->_1_LOAD = 50000;  
    PWM0->_2_LOAD = 50000; 
    PWM0->_0_CMPB = 30000; //Gen 0 trabaja con comparador B
    PWM0->_1_CMPA = 300; //%60 de las 50000 cuentas, Gen 1 trabaja con comparador A por pag. 1248
    PWM0->_2_CMPA = 2000; //Gen 2 trabaja con comparador B

    //Habilitar generadores a usar
    PWM0->_0_CTL = (1<<0);
    PWM0->_1_CTL = (1<<0);// Se activa el generador 1
    PWM0->_2_CTL = (1<<0);

    //Se habilita el PWM que se usa PWM1, PWM2 y PWM4 pag. 1247 y 1233
    PWM0->ENABLE = (1<<1)|(1<<2)|(1<<4); 
}

/*extern void PWM_Intensidad(uint16_t *Result)
{
    PWM0->_2_CMPA = 40000-((int)(color_azul*16000)/40); 
}*/
