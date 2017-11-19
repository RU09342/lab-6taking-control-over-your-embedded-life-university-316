#include <msp430.h>
//Chris Iapicco & Bryan Regn
//Created: November 1st, 2017
//Last Updated: November 1st, 2017
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;    // stop watchdog timer

    P6DIR|=BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6;
    P2DIR|=BIT7;

    TA0CTL = TASSEL_2 + MC_1 + TACLR + BIT6 + BIT7; // SMCLK, upmode, clear TAR
    TA0CCTL0 = CCIE;
    TA0CCR0 = 1000;
     __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0, enable interrupts
     while(1)
     {

     }
    return 0;
}
int count=0;
int over =0;
#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    if ((count<255) & (over == 0)){
   count=count+1;
   P6OUT= count & (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6);
   P2OUT=count & BIT7;
    }
    else
    {
        over = 1;
        if (count ==1)
                {
                    over = 0;
                }
        count=count-1;
        P6OUT= count & (BIT0+BIT1+BIT2+BIT3+BIT4+BIT5+BIT6);
        P2OUT=count & BIT7;
    }

}
