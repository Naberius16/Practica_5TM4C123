#include "lib/include.h"

int main(void)
{
    //Experimento 1
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 50,000,000 Hz
 * junto con el generador 1  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 10KHz  */

    Configurar_PLL(_50MHZ);  //Confiuracion de velocidad de reloj
    Configura_Reg_PWM1(8);  //Función para configurar el PWM 

    while (1)
    {
        
    }
    
}

