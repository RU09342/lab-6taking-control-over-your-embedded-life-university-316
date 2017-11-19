# Open-loop fan Control

## Hardware

A circuit was created to simulate how a cooling fan may need to be implemented to cool off a voltage regulator. This circuit can be seen below. 

![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-university-316/blob/master/Pictures/Open%20loop%20system.png)

A relay was chosen to power the fan with a low-side MOSFET switch on the coil of the relay to enable the MSP430 to control the power. With the fan located and fixed in a certain location relative to the circuit data was collected regarding the duty cycle of the power to the fan, and the temperature that the fan reached. At full power the fan was able to bring the temperature of the regulator down to 51C, and with the fan off the regulator reached 65C. A graph of the relation between duty cycle and temperature can be seen below.

![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-university-316/blob/master/Pictures/Temperature%20chart.png)

The MSP430FR6989 was chosen for its on-board LCD screen. This LCD screen was used to both display the read temperature in real time, but also as a way to toggle through which temperature you would like to be achieved. The ADC input to the MSP430 is through P8.6. The PWM output to the fan is through P1.3.

The temperature sensor used was the LM35. The heatsink on this chip was tied directly onto the heatsink of the voltage regulator for maximum heat transfer. For an easier, more accurate temperature reading the output of the LM35 was put through a gain of 5. This created an output which was 50mV/C. 

## Software

The software is extremely user friendly. The read temperature will be read on the right side of the MSP430FR6989 LCD screen. The left side is a selectable range of temperatures which the fan will settle on (51C-65C). Using the right button on the development the temperature can be toggled, and the fan will be set to a specific power setting to settle on that specific temperature. 

The temperature reading software was configured using the 12-bit ADC at an interval of 1 reading per second. Its timing was controlled by a timer. In the ADC interrupt the value of the ADC conversion was divided by 62.06, which was determined to be the amount of ADC12MEM0 bits per degree celcius at 50mV/C. The temperature is the displayed on the LCD screen by using basic ASCII logic. 

The fan control is hard coded. Every time the button is pressed, and a new temperature is selected, the CCR value for the output leading to the fan is automatically changed according to the graph that was shown previously.

Using the software is as simple as hooking up the correct ports to on the circuit, and selecting the desired temperature on the LCD screen using the button. 

