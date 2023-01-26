#include "lib/include.h"

//Experimento 3
/*Usando el modulo 0 de PWM con una frecuencia de reloj del sistema de 20,000,000 Hz
 * junto con el generador 0,1,2  habilitar alguno de los pwm's asociados y obtener un PWM
 * cuya frecuencia sea de 500Hz , utilizando el uart se enviara dato desde interfaz de simulink ****
 * para controlar la intensidad luminosa usando un led RGB externa 
 */
 

    //Declaración de variables
    //Variables para colores
    volatile uint8_t red = 114; //tipo volatil para ir modificando su valor, 82 por su valor en ASCII de R 
    volatile uint8_t green = 103; //Valor en ascii de G
    volatile uint8_t blue = 98; //Valor en ascii de B
    volatile uint16_t color;

int main(void)
{
    Configurar_PLL(_20MHZ);  //configuracion de velocidad de reloj
    Configurar_UART0();
    Configura_Reg_PWM1(8);

    while (1)
    {
        color = readChar();
        
        switch (color)
        {
            //COLOR SELECCIONADO AZUL
            case 'b':
                while (blue == 98)
                {
                    //Se lee el valor mandado por el UART para cambiar el ciclo de trabajo -> intensidad del led
                    blue = (int)readChar();
                }

                //Se hace la conversión para modificar el ciclo de trabajo del PWM -> intensidad del led
                //       PE4  40000:total de cuentas,  int_rojo: valor enviado por UART, las 16,000 cuentas equivalen al 40% de las cuentas
                PWM0->_2_CMPA = 50000-((int)(blue*30000)/60); 
                blue = 98;
            break;

            //COLOR SELECCIONADO VERDE
            case 'g':
                while (green == 103)
                {
                    green = (int)readChar();
                }

                //Se hace la conversión para modificar el ciclo de trabajo del PWM -> intensidad del led
                //       PB7  40000:total de cuentas,  int_rojo: valor enviado por UART, las 16,000 cuentas equivalen al 40% de las cuentas
                PWM0->_0_CMPB = 50000-((int)(green*30000)/60); //PB7 (G)
                green = 103;
            break;

            //COLOR SELECCIONADO ROJO
            case 'r':
                while (red == 114)
                {
                    red = (int)readChar(); //recibe el % del uart para calcular el dc%
                }

                //Se hace la conversión para modificar el ciclo de trabajo del PWM -> intensidad del led
                //       PB4  40000:total de cuentas,  int_rojo: valor enviado por UART, las 16,000 cuentas equivalen al 40% de las cuentas 
                PWM0->_1_CMPA = 50000-((int)(red*30000)/60); //PB4 (R)
                red = 114;
            break;

        }   
    }
}

