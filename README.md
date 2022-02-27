# sample_turner
A better stepper motor speed knob sketch for arduino nano. 

* Uses hardware timer/counter1 to generate step pulses of fairly accurate frequency, high stability and high maximum step rate (tested 60 kHz, likely can go substantially higher).
* Direct register manipulation means it is only compatible with avr, maybe even only with atmega328p. 
* Exponential speed characteristic for wide range of speeds.
* position mode for tuning of driver current
