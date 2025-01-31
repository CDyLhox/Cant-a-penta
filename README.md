**Cant-a-penta**
hello world;

we are the Midas and the Dylan and we made the cant-a-penta, 
a machine which tracks pitch incoming pitch information and activates triggers 
based on the incoming signal. more on this later.. 

The code has a lot of comments which hopefully explains a little bit of it's 
inner workings, this doesn't need to be true though, cause the code is a little 
bit of a *if it works it works* type situations. some functions are a little nested
and could work better. no bother though.

**CODE EPLANATION**
when provided an array of output pins for the relais it initialises these as outputs,
afterwards the teensy audio library tracks the pitches the audio shield recieves
and stores these pitches as midi in another array called the **midiArray**. the 
midi values in the midiArray are then compared and when a semitone is detected 
this version of the code then activates 2 random outputs 2 times. you could turn 
this around but for this demo we chose to leave it this way for proof of concept.

**HARDWARE EXPLANATION**
Solder pins on the teensy 4.1 and the audio shield, connect the two by placing them on top of each other, for the audio shield to work, the usb port of the teensy and the mini jack of the audio shield need to be on the same side. We are going to work with two circuits divided by the relay boards, one circuit that is connected to the Teensy and the other circuit is the 12v circuit that will power the solanoids. The circuits need to be seperate because the Teensy can't deliver or receive 12 Volts, DO NOT CONNECT THE 12V TO THE TEENSY, YOU WILL FRY IT! The relay boards are very easily connected to the Teensy, connect the pins as follows: VCC to the 5V on the Teensy and GND to the ground of the Teensy, this will power the little circuit board that the relay is connected to. 

Connecting the relay board:
The IN pin is to be connected to the output pins of the teensy, in this case pin 13, 14 and 15. One relay each. The 3.3V that the Teensy will send out with these pins will be amplified on the circuit board and switch the relay. 

Connecting the solanoids:
One wire of the solanoid is to be connected to ground (of the 12V) and the other wire is to be connected to the NO output of the relay board, the 12 Volt is to be connected to the COM output of the relay board. It is best to keep this circuit completely seperate from the Teensy, you could have a shared ground from the 12v and the Teensy but to prevent harming the Teensy by mistake you can leave it be. When the switch in the relay is flipped the circuit of the solanoids is completed and the solanoid will move.

Connecting the guitar:
On the top of the audio shield you wil find a four line in pinholes, connect the L (left) and G (ground) pinholes to the tip of a jack and the ground of a jack and stick it in the guitar. The mini jack out on the audio shiel will function as a jackout.