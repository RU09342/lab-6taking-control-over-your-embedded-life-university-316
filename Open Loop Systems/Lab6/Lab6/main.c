#include <msp430.h>
#include "LCDDriver.h"

void Init_GPIO();

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    //temp toggle button
    P1REN = BIT1; //Resistor enabled for button P1.1
    P1OUT = BIT1; //Sets resistor to pull up for button 1 when not pressed

    //LCD Setup
        // Initialize LCD segments 0 - 21; 26 - 43
        LCDCPCTL0 = 0xFFFF;
        LCDCPCTL1 = 0xFC3F;
        LCDCPCTL2 = 0x0FFF;
        // Disable the GPIO power-on default high-impedance mode
        // to activate previously configured port settings
        PM5CTL0 &= ~LOCKLPM5;
        // Initialize LCD_C
        // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
        LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;
        // VLCD generated internally,
        // V2-V4 generated internally, v5 to ground
        // Set VLCD voltage to 2.60v
        // Enable charge pump and select internal reference for it
        LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;
        LCDCCPCTL = LCDCPCLKSYNC;               // Clock synchronization enabled
        LCDCMEMCTL = LCDCLRM;                   // Clear LCD memory
        //Turn LCD on
        LCDCCTL0 |= LCDON|LCDSSEL;

        TA1CCR0=64000;
        TA1CCTL2=OUTMOD_7;
        TA1CCR2=64000;
        TA1CTL = TASSEL_2|ID_3|MC_2;

        //Interupt Set up
        P1IE |= BIT1 + BIT2; //interupt enable for button P1.1
        P1IES |= BIT1 + BIT2; //sets interupt to trigger on falling edge (button press)
        P1IFG &= ~BIT1 + ~BIT2; //Clears interupt flag register

        //debouncing for button
        TB0CCTL1 = CCIE; // CCR0 interrupt enabled
        TB0CCR1 = 6000; //register that timer will compare itself too
        TB0CTL = TBSSEL_2 + ID_3 + MC_0 + TBCLR; // SMCLK, divide clock by 8, stop mode, clear TAR

        Init_GPIO();

          ADC12CTL0 = ADC12SHT0_2 | ADC12ON;      // Sampling time, S&H=16, ADC12 on
          ADC12CTL1 = ADC12SSEL_3 | ADC12SHP; ;                   // Use sampling timer
          ADC12CTL2 |= ADC12RES_3;                // 12-bit conversion results
          ADC12MCTL0 |= ADC12INCH_5;              // A1 ADC input select; Vref=AVCC
          ADC12IER0 |= ADC12IE0;                  // Enable ADC conv complete interrupt

          //Initialize timer
          TA0CCTL0 |= CCIE;// Timer B, capture control 0 interrupt enable
          TA0CCR0 = 62500;//Timer B, capture control 0 set to 50000 cycles
          TA0CTL=TASSEL__SMCLK|MC__UP|TACLR|(BIT6 + BIT7);

          // Startup clock system with DCO setting ~1MHz
          CSCTL0_H = CSKEY_H;                     // Unlock CS registers
          CSCTL1 = DCOFSEL_0 | DCORSEL;           // Set DCO to 8MHz
          CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
          CSCTL3 = DIVA__1 | DIVM__1;   // Set all dividers
          CSCTL0_H = 0;                           // Lock CS registers

          __bis_SR_register(LPM0_bits | GIE); //global interupt enable (enables all maskable interupts)
        return 0;
}
int temp=0;
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void){
    if (temp == 15){
        temp = 0;
    }
    switch (temp)
    {
    case 0: showChar(0x36,1);//6
            showChar(0x35,2);//5
            TA1CCR2=13440;
            break;
    case 1: showChar(0x36,1);//6
            showChar(0x34,2);//4
            TA1CCR2=15360;
            break;
    case 2: showChar(0x36,1);//6
            showChar(0x33,2);//3
            TA1CCR2=17280;
            break;
    case 3: showChar(0x36,1);//6
            showChar(0x32,2);//2
            TA1CCR2=19584;
            break;
    case 4: showChar(0x36,1);//6
            showChar(0x31,2);//1
            TA1CCR2=21760;
            break;
    case 5: showChar(0x36,1);//6
            showChar(0x30,2);//0
            TA1CCR2=24128;
            break;
    case 6: showChar(0x35,1);//5
            showChar(0x39,2);//9
            TA1CCR2=26624;
            break;
    case 7: showChar(0x35,1);//5
            showChar(0x38,2);//8
            TA1CCR2=29312;
            break;
    case 8: showChar(0x35,1);//5
            showChar(0x37,2);//7
            TA1CCR2=32128;
            break;
    case 9: showChar(0x35,1);//5
            showChar(0x36,2);//6
            TA1CCR2=35264;
            break;
    case 10: showChar(0x35,1);//5
            showChar(0x35,2);//5
            TA1CCR2=38720;
            break;
    case 11: showChar(0x35,1);//5
            showChar(0x34,2);//4
            TA1CCR2=42688;
            break;
    case 12: showChar(0x35,1);//5
            showChar(0x33,2);//3
            TA1CCR2=47488;
            break;
    case 13: showChar(0x35,1);//5
            showChar(0x32,2);//2
            TA1CCR2=54080;
            break;
    case 14: showChar(0x35,1);//5
            showChar(0x31,2);//1
            TA1CCR2=64000-1;
            break;
    }
    temp++;
    P1IFG = 0;
}

int timing=0;
#pragma vector = TIMER0_A0_VECTOR
__interrupt void turn_on_ADC(void)
{
    switch (timing)
    {
    case 0: timing++;
        break;
    case 1: timing = 0;
    ADC12CTL0|= 0x03;
        break;
    }
}
int Temp=0;
#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = ADC12_VECTOR
__interrupt void ADC12_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(ADC12_VECTOR))) ADC12_ISR (void)
#else
#error Compiler not supported!
#endif
{
    switch(__even_in_range(ADC12IV, ADC12IV_ADC12RDYIFG))
    {
        case ADC12IV_NONE:        break;    // Vector  0:  No interrupt
        case ADC12IV_ADC12OVIFG:  break;    // Vector  2:  ADC12MEMx Overflow
        case ADC12IV_ADC12TOVIFG: break;    // Vector  4:  Conversion time overflow
        case ADC12IV_ADC12HIIFG:  break;    // Vector  6:  ADC12BHI
        case ADC12IV_ADC12LOIFG:  break;    // Vector  8:  ADC12BLO
        case ADC12IV_ADC12INIFG:  break;    // Vector 10:  ADC12BIN
        case ADC12IV_ADC12IFG0:             // Vector 12:  ADC12MEM0 Interrupt
            Temp=ADC12MEM0/62.06;
                showChar((Temp/10)+0x30 , 5);
                showChar((Temp%10)+0x30, 6);

            break;
        case ADC12IV_ADC12IFG1:   break;    // Vector 14:  ADC12MEM1
        case ADC12IV_ADC12IFG2:   break;    // Vector 16:  ADC12MEM2
        case ADC12IV_ADC12IFG3:   break;    // Vector 18:  ADC12MEM3
        case ADC12IV_ADC12IFG4:   break;    // Vector 20:  ADC12MEM4
        case ADC12IV_ADC12IFG5:   break;    // Vector 22:  ADC12MEM5
        case ADC12IV_ADC12IFG6:   break;    // Vector 24:  ADC12MEM6
        case ADC12IV_ADC12IFG7:   break;    // Vector 26:  ADC12MEM7
        case ADC12IV_ADC12IFG8:   break;    // Vector 28:  ADC12MEM8
        case ADC12IV_ADC12IFG9:   break;    // Vector 30:  ADC12MEM9
        case ADC12IV_ADC12IFG10:  break;    // Vector 32:  ADC12MEM10
        case ADC12IV_ADC12IFG11:  break;    // Vector 34:  ADC12MEM11
        case ADC12IV_ADC12IFG12:  break;    // Vector 36:  ADC12MEM12
        case ADC12IV_ADC12IFG13:  break;    // Vector 38:  ADC12MEM13
        case ADC12IV_ADC12IFG14:  break;    // Vector 40:  ADC12MEM14
        case ADC12IV_ADC12IFG15:  break;    // Vector 42:  ADC12MEM15
        case ADC12IV_ADC12IFG16:  break;    // Vector 44:  ADC12MEM16
        case ADC12IV_ADC12IFG17:  break;    // Vector 46:  ADC12MEM17
        case ADC12IV_ADC12IFG18:  break;    // Vector 48:  ADC12MEM18
        case ADC12IV_ADC12IFG19:  break;    // Vector 50:  ADC12MEM19
        case ADC12IV_ADC12IFG20:  break;    // Vector 52:  ADC12MEM20
        case ADC12IV_ADC12IFG21:  break;    // Vector 54:  ADC12MEM21
        case ADC12IV_ADC12IFG22:  break;    // Vector 56:  ADC12MEM22
        case ADC12IV_ADC12IFG23:  break;    // Vector 58:  ADC12MEM23
        case ADC12IV_ADC12IFG24:  break;    // Vector 60:  ADC12MEM24
        case ADC12IV_ADC12IFG25:  break;    // Vector 62:  ADC12MEM25
        case ADC12IV_ADC12IFG26:  break;    // Vector 64:  ADC12MEM26
        case ADC12IV_ADC12IFG27:  break;    // Vector 66:  ADC12MEM27
        case ADC12IV_ADC12IFG28:  break;    // Vector 68:  ADC12MEM28
        case ADC12IV_ADC12IFG29:  break;    // Vector 70:  ADC12MEM29
        case ADC12IV_ADC12IFG30:  break;    // Vector 72:  ADC12MEM30
        case ADC12IV_ADC12IFG31:  break;    // Vector 74:  ADC12MEM31
        case ADC12IV_ADC12RDYIFG: break;    // Vector 76:  ADC12RDY
        default: break;
    }
}
void Init_GPIO() //P1.5=>A5     P3.4 txd     a5=>
{
    P3SEL0 |= BIT4;
    P8SEL0|=BIT6;
    P8SEL1|=BIT6;
    P1DIR|=BIT0;
    P1DIR|=BIT3;
    P1SEL0|=BIT3;
}

#pragma vector=TIMER1_B0_VECTOR
__interrupt void TIMER1_B0_ISR(void){
    //debouncing
    P1IE|=BIT1|BIT2;//Re-enable interupt
    TA1CTL&=~BIT4;//Stop timer
    TA1CTL|=BIT2;//Clear timer
    P1IFG&=~BIT1;//Clear P1.1 interrupt flag
    P1IFG&=~BIT2;//Clear P1.1 interrupt flag
}
