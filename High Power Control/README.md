# Relays 

Before hooking up the microcontroller to the relay input, some tests were performed to test the current draw of the relay coil. This was done by putting voltage from a power supply directly across the coil and slowly raising the voltage until the relay switched. It seemed that it always took about 70 mA at the minimum to trigger the relay, and always took at least 4-5V. For this reason the MSP430 was determined to be insufficient for switching the relay and this was never attempted to prevent destroying the chip.

The switching speed of the relay was tested experimented with by running various square waves through the coil coming from the wave generator. There was always a small amount of delay between the input wave and the output wave. This is just due to the mechanical switching of the relay.

![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-university-316/commit/3ba7f6c9b4877e8ada4868de48b8222bd3fbdbfe)

It was determined that the absolute failure point for the relay came somewhere around 100-200 Hz. It was difficult to identify a clear point where it failed. As the relay began to switch at speeds approaching 100 Hz there was many sources of error. for starters, there was a significant delay between current entering the coil, and the coil switching. Second, at high rates there was a large amount of bouncing that occured at the output of the relay. Both of these effects can be seen below.

![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-university-316/commit/3ba7f6c9b4877e8ada4868de48b8222bd3fbdbfe)

For these reasons a failure frequency was difficult to determine but the reliable frequencies for a relay of this kind is most likely around 100 Hz or less. This leads to the conlusion that relays are effective for lower frequency, higher power implementations.

# Mosfet Switching

MOSFET switches typically work by preventing current to flow when you dont want it. By preventing current to flow you can control when you are actually giving power to a load.

The MOSFET switching was done with two different implementations. The high side implementation required two MOSFETS, an NMOS and a PMOS. The low side switch only required an NMOS. The high side switch was slightly more tricky. This was because interfacing the 3.3v coming out of the microcontroller, with the 12v source voltage on the PMOS proved to require an NMOS inverter. The high side switch circuit is shown below. For this switch, a 0V output from the MSP430 correlated to a 0V output across the load. When the MSP430 was outputting 3.3V, the voltage across the load was about 12V.

![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-university-316/commit/9f91814661921cec0b9592cdc54231eff5d98b3d)

The low side switching circuit proved to be much more simple. All this circuit required was one NMOS in between the resistor and ground. This was easier to interface with because the 3.3V coming from the microcontroller could be control the gate to soource voltage effectively. That circuit is shown below. The voltage levels for the low side switch are the same where a 0V output will create 0V across the load, and 3.3v from the MSP430 will create about 12V across the load.

![Alt Text](https://github.com/RU09342/lab-6taking-control-over-your-embedded-life-university-316/commit/9f91814661921cec0b9592cdc54231eff5d98b3d)

Both of these circuits had nearly the same performance. There was no appreciable current coming out of the microcontroller. Additionally, putting a square wave up to 15MHz couldnt cause the switches to fail. However, the greatest flaw of these switches was their current carrying capability. These MOSFETs being used were only rated at 70 mA, which is far less than the relay. It seems that the MOSFET switch is best suited for low power, high frequncy applications.

# Safe High Power Switching

The MOSFET switch is great for its near-zero current and its fast switching speeds, but it is low power. The relay is great for high power, but cannot be turned on using the MSP430. Therefore, the best high power switch is a combination of these useful devices. A relay which had a low side MOSFET switch across the coil worked very effectively. The circuit is seen below.

![Alt Text]()

This circuit had near zero current draw from the MSP430 but had the high power performance of the relay. Effectively this circuit was the best of both worlds.

# Software

The software created for this section of the lab was extremely simple. The code was just there for testing purposes and generated a square wave. It was generated using the toggle setting on a PWM output on the MSP430G2553. Every few timer cycles the CCR value for the PWM changes, making the frequency go up. This was tested until the relay stopped chaning effectively. Other testing was done using the wave generator in the labs.