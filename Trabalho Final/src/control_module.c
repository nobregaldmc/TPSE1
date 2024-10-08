#include "control_module.h"
#include "soc_AM335x.h"
#include "pad.h"

void cmSetCtrlModule(CONTROL_MODULE offset, unsigned int value){
    unsigned int aux = SOC_CONTROL_REGS + offset;
    aux |= value;
}

unsigned int cmGetCtrlModule(CONTROL_MODULE offset){
    unsigned int aux = SOC_CONTROL_REGS + offset;
    return aux;
}