#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    P1DIR |= BIT6;//P1.6 set to output TA0.1


    TA0CCR0 =1000 ;// PWM Period-> 10Hz
    TA0CCTL0 |= CCIE;
   // TA0CCTL1 = OUTMOD_4;// CCR1 set/reset
   // TA0CCR1 = 31250; //100% duty cycle
    TA0CTL |= (BIT6 + BIT7);//Divide timer clk by 8
    TA0CTL = TASSEL_2 + MC_1;// SMCLK, up mode

    __bis_SR_register(LPM0_bits + GIE);// Enter LPM0
	return 0;
}
int timing=0;
#pragma vector=TIMER0_A0_VECTOR
__interrupt void timer(void)
{
    if (timing <20)
    {
        timing++;
    }
    else
    {
        P1OUT^=BIT6;
        timing=0;
        TA0CCR0=TA0CCR0-10;
    }
}
