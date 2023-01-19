# BATTERY CALCULATIONS

4.2 [V] = 2.52 [V]		100%
3.0 [V] = 1.80 [V]		0%
-------------------------------------------
2520 [mV]	FULL
1800 [mV]	EMPTY
2100 [mV]	41,6 %
-------------------------------------------
720  [mV]	Range
-------------------------------------------
charge =	2520 - (x)
charge =	(x-2520)	+ 720		=	0
charge =	(x-2520)	+ 720		=	720

RAW RANGE
-------------------------------------------
charge =	(1800-2520)	+ 720		=	0
charge =	(2520-2520)	+ 720		=	720
charge =	(2100-2520)	+ 720		=	300
-------------------------------------------

Percentage
-------------------------------------------
charge =	((1800-2520)	+ 720)	/ 720		=	0
charge =	((2520-2520)	+ 720)	/ 720		=	720
charge =	((2100-2520)	+ 720) /  720		=	41,6%
-------------------------------------------

Percentage Simplified
-------------------------------------------
charge =	((x-1800))/720
-------------------------------------------


# LEDC Controller

The LED PWM controller can generate independent digital waveform on six channels.

The LED PWM controller:

• can generate digital waveform with configurable periods and duty cycle. 
  The accuracy of duty cycle can beup to 18 bits.
• has multiple clock sources, including APB clock and external main crystal clock.
• can operate when the CPU is in Light-sleep mode.
• supports gradual increase or decrease of duty cycle, which is useful for the LED RGB color-gradient generator.

For GPIOs assigned to LED PWM, please refer to Table 10.
For more information, please refer to Chapter LED PWM Controller (LEDC) in ESP32-C3 Technical Reference

