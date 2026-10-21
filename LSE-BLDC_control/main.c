
#include <stdint.h>
#include "tm4c123gh6pm"

//System definitions
#define SYSCTL_RCGCGPIO *((uint32_t *) 0x400FE608) //permite activar /desactivar los modulos GPIO del Run mode
#define SYSCTL_PRGPIO *((uint32_t *) 0x400FEA08) //indica cuando los modulos estaran listos para ser accedidos por el SW
#define SYSCTL_RCC *((uint32_t *) 0x400FE060) //este registro controla el clock cuando estamos en run mode
#define SYSCTL_RCGCPWM *((uint32_t *) 0x400FE640) //registro para activar el clock control para los puertos que tendran PWM
#define SYSCLK_PRPWM *((uint32_t *) 0x400FA40)  //este registro indica cuando los puertos estaran listos para ser configurados como PWM

//Port B
#define GPIOB_DIR *((uint32_t *) 0x40005400)   //dirección pines (Puerto B)
#define GPIOB_DATA *((uint32_t *) 0x40005000)  //registro de datos (Puerto B)
#define GPIOB_AFSEL *((uint32_t *) 0x40005420) //selección de función alternativa (Puerto B)
#define GPIOB_DEN  *((uint32_t *) 0x4000551C) //registro de digital Enable (Puerto B)
#define GPIOB_AMSEL *((uint32_t *) 0x40005528) //registro función analógica (Puerto B)
#define GPIOB_PCTL *((uint32_t *) 0x4000552C)  //registro para seleccionar función PWM (Puerto B)

//Port D
#define GPIOD_DIR *((uint32_t *) 0x40007400)   //dirección pines (Puerto D)
#define GPIOD_DATA *((uint32_t *) 0x40007000)  //registro de datos (Puerto D)
#define GPIOD_AFSEL *((uint32_t *) 0x40007420) //selección de función alternativa (Puerto D)
#define GPIOD_DEN   *((uint32_t *) 0x4000751C) //registro de digital Enable (Puerto D)
#define GPIOD_AMSEL *((uint32_t *) 0x40007528) //registro función analógica (Puerto D)
#define GPIOD_PCTL *((uint32_t *) 0x4000752C)  //registro para seleccionar función PWM (Puerto D)

//Port E
#define GPIOE_DIR *((uint32_t *) 0x40024400)   //dirección pines (Puerto E)
#define GPIOE_DATA *((uint32_t *) 0x40024000)  //registro de datos (Puerto E)
#define GPIOE_AFSEL *((uint32_t *) 0x40024420) //selección de función alternativa (Puerto E)
#define GPIOE_DEN   *((uint32_t *) 0x4002451C) //registro de digital Enable (Puerto E)
#define GPIOE_AMSEL *((uint32_t *) 0x40024528) //registro función analógica (Puerto E)
#define GPIOE_PCTL *((uint32_t *) 0x4002452C)  //registro para seleccionar función PWM (Puerto E)

//Port F
#define GPIOF_DIR *((uint32_t *) 0x40025400)   //dirección pines (Puerto F)
#define GPIOF_DATA *((uint32_t *) 0x40025000)  //registro de datos (Puerto F)
#define GPIOF_AFSEL *((uint32_t *) 0x40025420) //selección de función alternativa (Puerto F)
#define GPIOF_DEN   *((uint32_t *) 0x4002551C) //registro de digital Enable (Puerto F)
#define GPIOF_AMSEL *((uint32_t *) 0x40025528) //registro función analógica (Puerto F)
#define GPIOF_PCTL *((uint32_t *) 0x4002552C)  //registro para seleccionar función PWM (Puerto F)

// PWM
#define RCGC0 *((volatile uint32_t *) 0x400FE100)
#define PWM0CTL *((volatile uint32_t *) 0x40028040)
#define PWM0GENA *((volatile uint32_t*) 0x40028060)
#define PWM0GENB *((volatile uint32_t*) 0x40028064)
#define PWM0CMPA *((volatile uint32_t*) 0x40028058)
#define SRPWM *((volatile uint32_t*) 0x400FE540)
#define PWM0ENABLE *((volatile uint32_t *) 0x40028008)

// ADC 
#define PE0  0x01 //ADC
#define SYSCTL_RCGCADC *(uint32_t*) 0x400FE638  // Enable/disable ADC modules from Run Mode
#define SYSCTL_PRADC *(uint32_t*)   0x400FEA38  // Indica cuando los modulos ADC estan ready para ser leidos por el software
#define ADC0_ACTSS   *(uint32_t*)   0x40038000
#define ADC0_EMUX    *(uint32_t*)    0x40038014
#define ADC0_SSMUX3  *(uint32_t*)    0x400380A0
#define ADC0_SSCTL3  *(uint32_t*)    0x400380A4
#define ADC0_ISC     *(uint32_t*)    0x4003800C


// definimos los pines que mas usaremos
#define PB5 0x20 //High Phase A
#define PE4 0x10 //High Phase B	
#define PB7 0x80 //High Phase C

#define PD0 0x01 //Low Phase A
#define PB3 0x10 //Low Phase B
#define PF2 0x04 //Low Phase C

#define PD2 0x04 //BEMF Phase C
#define PD3 0x08 //BEMF Phase C
#define PD6 0x40 //BEMF Phase C

#define PE0 0x01 //ADC

void init_sys(void)
{

	//Activamos el Run Mode para los puertos B, E, D, F(los que tendran el PWM que commutara)
	SYSCTL_RCGCGPIO |= 0x2B;

	//Para poder proseguir, hay que asegurarnos que los puertos estan armados (Ready)
	while ((SYSCLK_PRGPIO & 0x2B) != 0x2B);

	//Activamos el PWM para los puertos B y E
	SYSCTL_RCGCPWM |= 0x12;

	//Para poder proseguir, hay que asegurarnos que los puertos estan armados (Ready)
	while ((SYSCLK_PRPWM & 0x12) != 0x12);


}

void init_GPIOs(void)
{
	//Configuramos si el pin se comporta como GPIO o se controla por periférico
	GPIOD_AFSEL &= ~(PD0);  //Activamos GPIOs PF2
	GPIOB_AFSEL &= ~(PB3);  //Activamos GPIOs PF2
	GPIOF_AFSEL &= ~(PF2);  //Activamos GPIOs PF2
	GPIOD_AFSEL &= ~(PD2);  //Activamos GPIOs PB3
	GPIOD_AFSEL &= ~(PD3);  //Activamos GPIOs PD4
	GPIOD_AFSEL &= ~(PD6);  //Activamos GPIOs PF2
	GPIOB_AFSEL |= PB5;  //Activamos Perifericos PB5
	GPIOB_AFSEL |= PB7;  //Activamos Perifericos PB7
	GPIOE_AFSEL |= PE4;  //Activamos Perifericos PE4

	//Desactivamos las funciones analogicas, ya que queremos trabajar con PWM
	GPIOB_AMSEL &= ~(PB5 | PB7 | PB3);  //Desactivamos f.analogica (PB5, PB7 y PB3)
	GPIOD_AMSEL &= ~(PD4);  //Desactivamos f.analogica PD4
	GPIOE_AMSEL &= ~(PE4);  //Desactivamos f.analogica PE4
	GPIOF_AMSEL &= ~(PF2);  //Desactivamos f.analogica PF2
	
	//En el Port Control (PCTL), hay que seleccionar la función GPIO. Lo hacemos
	//para los pines PB5, PB7, PE4, PD0, PF2 y PE4 que son las entradas High y Low del driver
	GPIOB_PCTL = (GPIOB_PCTL & 0xFF0FFFFF) | 0x00400000; //Puerto B5 - PWM
	GPIOB_PCTL = (GPIOB_PCTL & 0x0FFFFFFFF) | 0x40000000; //Puerto B7 - PWM
	GPIOE_PCTL = (GPIOE_PCTL & 0xFFF0FFFF) | 0x00040000; //Puerto E4 - PWM

	// Habilitar función digital para todos los puertos para poder efectuar PWM
    GPIOB_DEN |= (PB5 | PB7 | PB3);
    GPIOD_DEN |= PD0;
    GPIOE_DEN |= PE4;
    GPIOF_DEN |= PF2;

	//Para iniciar en un estado conocido, establecemos el estado inicial para todos los GPIO en nivel bajo
	GPIOB_DATA &= ~(PB5 | PB7 | PB3);  //Estado inicial: nivel bajo (PB5, PB7 y PB3)
	GPIOD_DATA &= ~(PD4);              //Estado inicial: nivel bajo (PD4)
	GPIOE_DATA &= ~(PE4);  			   //Estado inicial: nivel bajo (PE4)
	GPIOF_DATA &= ~(PF2);             //Estado inicial: nivel bajo (PF2)

	//Definimos los pines como salida
	GPIOB_DIR |= (PB5 | PB7 | PB3);  //Definidos como salida (PB5, PB7 y PB3)
	GPIOD_DIR |= (PD4);              //Definido como salida (PD4)
	GPIOD_DIR &= (~PD2 & ~PD3 & ~PD6);              //Definido como salida (PD4)
	GPIOE_DIR |= (PE4);  			   //Definido como salida (PE4)
	GPIOF_DIR |= (PF2);             //Definido como salida (PF2)

}

void init_PWM(void)
{
	RCGC0 |= 0x00100000;
	RCGC2 |= 0x00000012;
	PWM0CTL &= 0xFFFFFFFD;
	PWM0GENA |= 0x0000000E;
	PWM0GENB &= 0xFFFFF000;
	PWM0LOAD = (PWM0LOAD & 0xFFFF0000) | 0x000009C3; // Definido un LOAD de 2499 para un PWM de 32kHz -> 80MHz / 32kHz - 1 = 2499
	PWM0CTL |= 0x00000001;  // Comienza a funcionar el PWM

}

void PWM(uint32_t compA, uint8_t pwm)
{
	PWM0CMPA = (PWM0CMPA & 0xFFFF0000) | compA;
	PWM0ENABLE |= pwm;
}

void init_ADC(void)
{
	SYSCTL_RCGCADC	|=	 0x01; //habilitamos el reloj del puerto E, donde esta el pin PE0 (Run mode)
	SYSCTL_RCGCGPIO |=   0x10; //habilitamos el reloj del ADC (Run mode)

    while ((SYSCTL_PRGPIO & 0x01) == 0);  //Esperamos a que el reloj del ADC este ready
    while ((SYSCTL_PRADC  & 0x10) == 0);  //Esperamos a que el reloj del puerto E este ready

	//Inicializamos GPIO para el ADC
	GPIOE_DIR   &= ~PE0;
    GPIOE_AFSEL |=  PE0;
    GPIOE_DEN   &= ~PE0;
    GPIOE_AMSEL |=  PE0;

	// Configurar el sample sequencer (SS3)

    ADC0_ACTSS &= ~0x08;       // Desactivamos el SS3. Para programar el ADC, debe estar desactivado. Esto previene ejecucion erronea durante configuracion
    ADC0_EMUX  &= ~0xF000;     // trigger event para el SS3
    ADC0_SSMUX3 = 0x03;        // configuramos el input de nuestro ADC (en nuestro caso, es el PE0, que corresponde al AIN3)
    ADC0_SSCTL3 = 0x06;        // configuramos los bits the nibble correspondientes a la muesta
    ADC0_ISC = 0x08;            // Limpiamos las flags que pueden haber pendientes
    ADC0_ACTSS |= 0x08;        // Una vez configurado, podemos habilitar SS3
	
}

int main(void)
{
	init_sys();
	init_GPIOs();
	init_PWM();

	
	return 0;
}
