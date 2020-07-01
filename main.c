#include "mcc_generated_files/mcc.h"

/*
                         Main application
 */

uint32_t pwm1 = 0;
uint32_t pwm2 = 0;
uint32_t pwm3 = 0;
uint32_t a = 0;
unsigned char count = 0;

typedef enum {
    ButtonReleasedPhase = 0,
    ButtonIntermediatePhase = 1,
    ButtonPressedPhase = 2
} ButtonPhase;

ButtonPhase phaseRA3 = 0;
ButtonPhase phaseRA5 = 0;

void updateValues() {
    unsigned char c = count;
    if (1 & c) IO_RA0_SetHigh(); else IO_RA0_SetLow();
    
    c = c >>1;
    if (1 & c) IO_RA1_SetHigh(); else IO_RA1_SetLow();
    
    c = c >>1;
    if (1 & c) IO_RA2_SetHigh(); else IO_RA2_SetLow();
    
    c = c >>1;
    if (1 & c) {
        IO_RC4_SetHigh();
    } else {
        IO_RC4_SetLow();
    }
}

void RA3_pressed() {
    count = 0;
    updateValues();
}

void RA5_pressed() {
    count++;
    if (count > 9) {
        count = 0;
    }
    updateValues();
}

void handleInterruptTMR0() {
    unsigned RA3_new = IO_RA3_GetValue();
    unsigned RA5_new = IO_RA5_GetValue();
    
    if (RA3_new) {
        phaseRA3 = ButtonReleasedPhase;
    } else {
        // pin is 0
        if (phaseRA3 == ButtonReleasedPhase) {
            phaseRA3 = ButtonIntermediatePhase;
        } else if (phaseRA3 == ButtonIntermediatePhase) {
            phaseRA3 = ButtonPressedPhase;
            RA3_pressed();
        }
    }
    
    if (RA5_new) {
        phaseRA5 = ButtonReleasedPhase;
    } else {
        // pin is 0
        if (phaseRA5 == ButtonReleasedPhase) {
            phaseRA5 = ButtonIntermediatePhase;
        } else if (phaseRA5 == ButtonIntermediatePhase) {
            phaseRA5 = ButtonPressedPhase;
            RA5_pressed();
        }
    }
}

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    updateValues();
    
    TMR0_SetInterruptHandler(handleInterruptTMR0);
 
    // When using interrupts, you need to set the Global and Peripheral Interrupt Enable bits
    // Use the following macros to:

    // Enable the Global Interrupts
    INTERRUPT_GlobalInterruptEnable();

    // Enable the Peripheral Interrupts
    INTERRUPT_PeripheralInterruptEnable();

    // Disable the Global Interrupts
    //INTERRUPT_GlobalInterruptDisable();

    // Disable the Peripheral Interrupts
    //INTERRUPT_PeripheralInterruptDisable();

    // Test values
    //    PWM2_LoadDutyValue(0);
    //    PWM1_LoadDutyValue(80);
    //    PWM4_LoadDutyValue(30);
    
    while (1)
    {

        a = ADC_GetConversion(channel_AN3);
        a = a * 99 / 1023;
        
        if (a != pwm1) {
            pwm1 = a;
            PWM1_LoadDutyValue(a);
        }
    
        a = ADC_GetConversion(channel_AN4);
        a = a * 99 / 1023;
        
        if (a != pwm2) {
            pwm2 = a;
            PWM2_LoadDutyValue(a);
        }
    
        a = ADC_GetConversion(channel_AN6);
        a = a * 99 / 1023;
        
        if (a != pwm3) {
            pwm3 = a;
            PWM4_LoadDutyValue(a);
        }
    }
}


/**
 End of File
*/
