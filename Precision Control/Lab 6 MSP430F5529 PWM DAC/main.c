#include <msp430.h>
//Chris Iapicco
//Created: October 1st, 2017
//Last Updated: October 1st, 2017

void Port_Mapping(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT

  P1DIR |=BIT2;
  P1DIR |= BIT2;

  TA0CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA0CCR0 = 60000;//Set CCR0 of TA
  TA0CCR1 = 0;
  TA0CCTL1 = OUTMOD_7;
  TA0CTL = TASSEL_2 + MC_1 + TACLR;                  // SMCLK, stopped
  TA0CTL|=(BIT6+BIT7);//Divide clock by 8

  __bis_SR_register(LPM0_bits+GIE);             // Enter LPM0
}

//Timer1 A0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void debounce(void)
{
    TA0CCR1 = TA0CCR1 + 1000;
}
