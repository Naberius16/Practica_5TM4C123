#include "lib/include.h"

volatile uint16_t Result[3];
uint16_t duty[3];

int main(void)
{
   
//Experimento 2
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 50Hz con tres potenciometros variar el ciclo de trabajo
 * para controlar la posicion de tres servos sg90 o otros.
 */
    Configurar_PLL(_20MHZ);  //Confiuracion de velocidad de reloj
    Configurar_UART0();
    Configura_Reg_ADC0();
    Configura_Reg_PWM1(8);
    while (1)
    {
        ADC0_InSeq2(Result,duty); //llamada a la conversion por procesador
        PWM0->_0_CMPB = duty[0];
        PWM0->_1_CMPA = duty[1];
        PWM0->_2_CMPA = duty[2];
    }
    
}

