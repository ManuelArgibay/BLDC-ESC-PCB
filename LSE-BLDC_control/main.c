
#include <stdint.h>

//System definitions
#define SYSCTL_RCGCGPIO 0x400FE608 //permite activar /desactivar los modulos GPIO del Run mode
#define SYSCTL_PRGPIO 0x400FEA08 //indica cuando los modulos estaran listos para ser accedidos por el SW
#define SYSCTL_RCC 0x400FE060    //este registro controla el clock cuando estamos en run mode
#define SYSCTL_RCGCPWM 0x400FE640 //registro para activar el clock control para los puertos que tendran PWM
#define SYSCLK_PRPWM 0x400FA40  //este registro indica cuando los puertos estaran listos para ser configurados como PWM
//Port B
#define GPIOB_DIR 0x40005400   //dirección pines (Puerto B)
#define GPIOB_DATA 0x40005000  //registro de datos (Puerto B)
#define GPIOB_AFSEL 0x40005420 //selección de función alternativa (Puerto B)
#define GPIOB_DEN   0x4000551C //registro de digital Enable (Puerto B)
#define GPIOB_AMSEL 0x40005528 //registro función analógica (Puerto B)
#define GPIOB_PCTL 0x4000552C  //registro para seleccionar función PWM (Puerto B)

//Port D
#define GPIOD_DIR 0x40007400   //dirección pines (Puerto D)
#define GPIOD_DATA 0x40007000  //registro de datos (Puerto D)
#define GPIOD_AFSEL 0x40007420 //selección de función alternativa (Puerto D)
#define GPIOD_DEN   0x4000751C //registro de digital Enable (Puerto D)
#define GPIOD_AMSEL 0x40007528 //registro función analógica (Puerto D)
#define GPIOD_PCTL 0x4000752C  //registro para seleccionar función PWM (Puerto D)

//Port E
#define GPIOE_DIR 0x40024400   //dirección pines (Puerto E)
#define GPIOE_DATA 0x40024000  //registro de datos (Puerto E)
#define GPIOE_AFSEL 0x40024420 //selección de función alternativa (Puerto E)
#define GPIOE_DEN   0x4002451C //registro de digital Enable (Puerto E)
#define GPIOE_AMSEL 0x40024528 //registro función analógica (Puerto E)
#define GPIOE_PCTL 0x4002452C  //registro para seleccionar función PWM (Puerto E)

//Port F
#define GPIOF_DIR 0x40025400   //dirección pines (Puerto F)
#define GPIOF_DATA 0x40025000  //registro de datos (Puerto F)
#define GPIOF_AFSEL 0x40025420 //selección de función alternativa (Puerto F)
#define GPIOF_DEN   0x4002551C //registro de digital Enable (Puerto F)
#define GPIOF_AMSEL 0x40025528 //registro función analógica (Puerto F)
#define GPIOF_PCTL 0x4002552C  //registro para seleccionar función PWM (Puerto F)

// definimos los pines que mas usaremos
#define PB5 0x20 //High Phase A
#define PB7 0x80 //High Phase C
#define PE4 0x10 //High Phase B	
#define PD0 0x01 //Low Phase A
#define PF2 0x04 //Low Phase C
#define PB3 0x10 //Low Phase B

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
	//Desactivamos las funciones alternativas, ya que queremos trabajar con PWM

	GPIOB_AFSEL &= ~(PB5 | PB7 | PB3);  //Desactivamos f.alternativa (PB5, PB7 y PB3)
	GPIOD_AFSEL &= ~(PD4);  //Desactivamos f.alternativa PD4
	GPIOE_AFSEL &= ~(PE4);  //Desactivamos f.alternativa PE4
	GPIOF_AFSEL &= ~(PF2);  //Desactivamos f.alternativa PF2

	//Desactivamos las funciones analogicas, ya que queremos trabajar con PWM

	GPIOB_AMSEL &= ~(PB5 | PB7 | PB3);  //Desactivamos f.analogica (PB5, PB7 y PB3)
	GPIOD_AMSEL &= ~(PD4);  //Desactivamos f.analogica PD4
	GPIOE_AMSEL &= ~(PE4);  //Desactivamos f.analogica PE4
	GPIOF_AMSEL &= ~(PF2);  //Desactivamos f.analogica PF2
	
	//En el Port Control (PCTL), hay que seleccionar la función GPIO. Lo hacemos
	//para los pines PB5, PB7, PE4, PD0, PF2 y PE4 que son las entradas High y Low del driver

	GPIOB_PCTL &= ~(PB5 | PB7 | PB3);  //GPIO Port B
	GPIOD_PCTL &= ~(PD4);  //GPIO Port D
	GPIOE_PCTL &= ~(PE4);  //GPIO Port E
	GPIOF_PCTL &= ~(PF2);  //GPIO Port F

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
	GPIOE_DIR |= (PE4);  			   //Definido como salida (PE4)
	GPIOF_DIR |= (PF2);             //Definido como salida (PF2)
}
/**
 * main.c
 */
int main(void)
{
	init_sys();
	init_GPIOs();

	
	return 0;
}
