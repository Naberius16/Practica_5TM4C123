#include "lib/include.h"

/* Para controlar 3 potenciometros se habilitan los 3 canales en el modulo 0  
   pag. 1056 para saber que pines corresponden a cada canal a una velocidad de
   conversion de 250ksps dandole la mayor con evento de procesador */

extern void Configura_Reg_ADC0(void)
{
    //Pag 396 para inicializar el modulo de reloj del adc RCGCADC, Se inicializa modulo 0
    SYSCTL->RCGCADC = (1<<0);
    //Pag 382 (RGCGPIO) Puertos base habilitación del reloj
    //                     F     E      D       C      B     A
    SYSCTL->RCGCGPIO |= (1<<5)|(1<<4)|(0<<3)|(0<<2)|(0<<1)|(1<<0);
    //Pag 760 (GPIODIR) Habilta los pines como I/O un cero para entrada y un uno para salida
    GPIOE_AHB->DIR = (0<<1) | (0<<2) | (0<<5); //PE1, PE2 y PE5
    //(GPIOAFSEL) pag.770 Enable alternate función para que el modulo analógico tenga control de esos pines
    GPIOE_AHB->AFSEL =  (1<<1) | (1<<2) | (1<<5); //PE1, PE2 y PE5
    //(GPIODEN) pag.781 desabilita el modo digital
    GPIOE_AHB->DEN = (0<<1) | (0<<2) | (0<<5); //PE1, PE2 y PE5
    //Pag 787 GPIOPCTL registro combinado con el GPIOAFSEL y la tabla pag 1808
    GPIOE_AHB->PCTL = GPIOE_AHB->PCTL & (0xFF0FF00F); //Poner 0 en los bits de los pines 1, 2, 5
    //(GPIOAMSEL) pag.786 habilitar analogico
    GPIOE_AHB->AMSEL = (1<<1) | (1<<2) | (1<<5); //PE1, PE2 y PE5
    //Pag 1159 El registro (ADCPC) establece la velocidad de conversión por segundo
    ADC0->PC = (0<<2)|(0<<1)|(1<<0);//250ksps: Se pone un 1 en binario para indicar que los 2 millos de muestrs por s se divide /8 para obtener 250ksps
    //Pag 1099 Este registro (ADCSSPRI) configura la prioridad de los secuenciadores
    ADC0->SSPRI = 0x3210;//se pone mayor prioridad en el sec. 0
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS  =   (0<<3) | (0<<2) | (0<<1) | (0<<0);
    //Pag 1091 Este registro (ADCEMUX) selecciona el evento que activa la conversión (trigger)
    ADC0->EMUX  = (0x0000);//se configura por procesador 
    //Pag 1129 Este registro (ADCSSMUX2) define las entradas analógicas con el canal y secuenciador seleccionado
    //ADC0->SSMUX2 = 0x0821; 
    //prueba:
    ADC0->SSMUX0 = (1<<0); //en el secuenciador 0 se asignar el canal 1
    ADC0->SSMUX1 = (2<<0); //en el secuenciador 1 se asignar el canal 2
    ADC0->SSMUX2 = (8<<0); //en el secuenciador 2 se asignar el canal 8
    //pag 1130 Este registro (ADCSSCTL2), configura el bit de control de muestreo y la interrupción
    ADC0->SSCTL0 = (1<<2) | (1<<1);
    ADC0->SSCTL1 = (1<<2) | (1<<1);
    ADC0->SSCTL2 = (1<<2) | (1<<1);
    /* Enable ADC Interrupt */
    ADC0->IM |= (1<<0) | (1<<1) | (1<<2); /* Unmask ADC0 sequence 0, 1 y 2 interrupt pag 1082*/
    //NVIC_PRI4_R = (NVIC_PRI4_R & 0xFFFFFF00) | 0x00000020;
    //NVIC_EN0_R = 0x00010000;
    //Pag 1077 (ADCACTSS) Este registro controla la activación de los secuenciadores
    ADC0->ACTSS = (0<<3) | (1<<2) | (1<<1) | (1<<0);
    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2);  //conf. por procesador
}
extern void ADC0_InSeq2(uint16_t *Result,uint16_t *duty)
{
    //se habilita el modo de configuración
    ADC0->PSSI |= (1<<0) | (1<<1) | (1<<2);  //por procesador
    // espera al convertidor
    while((ADC0->RIS&0x04)==0){};  
    //Secuenciador 0 - Canal 1 - PE2
    Result[0] = ADC0->SSFIFO0&0xFFF; // En el FIFO0 se almacenan las muestras del secuenciador 0, por lo que se lee, pag 860 
    duty[0] = (Result[0]*50000)/4096; //Cuentas para 20ms 
    //Secuenciador 1 - Canal 2 - PE1
    Result[1] = ADC0->SSFIFO1&0xFFF; // En el FIFO0 se almacenan las muestras del secuenciador 1
    duty[1] = (Result[1]*50000)/4096; //Cuentas para 20ms
    //Secuenciador 2 - Canal 8 - PE5
    Result[2] = ADC0->SSFIFO2&0xFFF;// En el FIFO0 se almacenan las muestras del secuenciador 2
    duty[2] = (Result[2]*50000)/4096;//Cuentas para 20ms
    ADC0->ISC = (1<<0) | (1<<1) | (1<<2);  //Conversion finalizada
}


