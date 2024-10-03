#include "hw_types.h"
#include "soc_AM335x.h"

//Definindo a ativação do clock
#define CM_PER_GPIO1_CLKCTRL (0xAC) // Registrador que configura o modulo do GPIO1 (0xAC - offset do registrador)
#define CM_PER_GPIO1_CLKCTRL_MODULEMODE_ENABLE (0x2u) // Mascara ,Bits 1-0 são do module mode, setando em 2 para ativar
#define CM_PER_GPIO1_CLKCTRL_OPTFCLKEN_GPIO_1_GDBCLK (1<<18) // Mascara que configura o clock (0x00040000u(Bit 18) - valor da mascara)1<<18
#define CM_PER_GPIO1_CLKCTRL_M_O (0x00040002u)

//Definindo a configuração do mux
#define CM_conf_gpmc_a0 (0x0840) // Saída

//Definição da direção do pino
#define GPIO_OE (0x134) // Permissão para o pino ser saída
#define GPIO_SETDATAOUT (0X194) // Setar 1 no pino do GPIO
#define GPIO_CLEARDATAOUT (0x190) // Limpar o registrador do pino

int _main(void){
    unsigned char pisca=0;
    unsigned int valor;
    volatile unsigned int i; 

    //Definindo a ativação do clock
    //0x44E000AC |= (1<<18) | (1<<2);
    HWREG(SOC_PRCM_REGS + CM_PER_GPIO1_CLKCTRL) |= CM_PER_GPIO1_CLKCTRL_M_O;

    //Definindo a configuração do mux
    // Endereço base mais o offset do pino escolhido multiplexado para o modulo 7(GPIO1_16)
    HWREG(SOC_CONTROL_REGS + CM_conf_gpmc_a0) |= 7;

    //Definição da direção do pino
    valor = HWREG(SOC_GPIO_1_REGS+GPIO_OE);
    valor &= ~(1<<16);
    HWREG(SOC_GPIO_1_REGS+GPIO_OE) = valor;

    //delay
    void delay(){
        for(i = 0; i<10000000; i++);
    }
    

    while(1){
        pisca ^= (0x01u);
        if(pisca){
            HWREG(SOC_GPIO_1_REGS+GPIO_SETDATAOUT) = (1 << 16);
        }else{
            HWREG(SOC_GPIO_1_REGS+GPIO_CLEARDATAOUT) = (1 << 16);
        }
        delay();
    }

    return(0);
}



