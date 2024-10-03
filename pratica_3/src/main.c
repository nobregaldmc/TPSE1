#include "hw_types.h"
#include "soc_AM335x.h"

//Definindo a ativação do clock
#define CM_PER_GPIO1_CLKCTRL (0xAC) // Registrador que configura o modulo do GPIO1 (0xAC - offset do registrador)
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE (0x2u) // Mascara ,Bits 1-0 são do module mode, setando em 2 para ativar
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK (1<<18) // Mascara que configura o clock (0x00040000u(Bit 18) - valor da mascara)1<<18
#define CM_PER_GPIO1_CLKCTRL_M_O (0x00040002u)

//Definindo a configuração do mux
#define CM_conf_gpmc_a0 (0x0840) // Saída
#define CM_conf_gpmc_a1 (0x0844) // Botão

//Definição da direção do pino
#define GPIO_OE (0x134) // Entrada ou Saída 
#define GPIO_SETDATAOUT (0X194) // Setar 1 no pino do GPIO  
#define GPIO_CLEARDATAOUT (0x190) // Limpar o registrador do pino
#define GPIO_DATAIN (0x138) // Entrada

//Defines do watchdog
#define WDT1 													0x44E35000
#define WDT_WSPR 												0x48
#define WDT_WWPS												0x34
#define W_PEND_WSPR												(1<<0x4u)


void watchdog(){
	HWREG(WDT1+WDT_WSPR) = 0xAAAA;
	while((HWREG(WDT1+WDT_WWPS) & W_PEND_WSPR) != 0x0){};
	HWREG(WDT1+WDT_WSPR) = 0x5555;
	while((HWREG(WDT1+WDT_WWPS) & W_PEND_WSPR) != 0x0){};
}

int _main(void){
    unsigned int valor;
    volatile unsigned int i; 

    //Definindo a ativação do clock
    //0x44E000AC |= (1<<18) | (1<<2);
        HWREG(SOC_PRCM_REGS + CM_PER_GPIO1_CLKCTRL) |= CM_PER_GPIO1_CLKCTRL_M_O;



    //Configurando GPIO1_16 como saída - led
    //Configurando GPIO1_17 como entrada - botão

    //Definindo a configuração do mux
    HWREG(SOC_CM_PER_REGS+CM_PER_GPIO1_CLKCTRL) |= CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK | CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE;
    HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a0) |= 7;  // setar o pino como GPIO
    HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a1) |= 7;  // setar o pino como GPIO

    //Definição da direção do pino
    valor = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
    valor &= ~(1<<16);
    HWREG(SOC_GPIO_1_REGS+GPIO_OE) = valor;

    unsigned int valor1;
    valor1 = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
    valor1 |= (1<<17);
    HWREG(SOC_GPIO_1_REGS+GPIO_OE) = valor1; 



    
    //HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) |= (1 << 16);

    while(1){
        if(HWREG(SOC_GPIO_1_REGS+GPIO_DATAIN) & (1<<17)){                             // Se o registrador de entrada do pino 17 for diferente de 0
            HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = (1 << 16);

        }else{
            HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = (1 << 16);
        }
    }

    return(0);
}



