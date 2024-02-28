This is a very simple project to realize DIY R.A. axis tracking on Equatorial mount of a telescope. 
The goal was to minimize soldering and wiring and solve things with modular approach. So for initial version i've used a totally overkill RAMPS 1.4 on Arduino Mega.  

Software is provided AS IS. I'm not responsible for any destruction you may cause by incorrect wiring etc.

# How to use
## Configuration
If you'll use the same mount and gear ratio that I use, you'll need to adjust your Sidereal speed. You can do it on LCD screen experimentally, but it's better to calculate a correct amount and set it in `sidereal_speed.h`. It should be in steps per second. Regular NEMA17 has 200 steps, which you multiply by 16 because we use 1/16 microsteps. Not go to your mount and determine how many full turns of R.A. handle is required to turn R.A. axis for a full 360 degrees. Now calculate how many steps you'll need assuming your target speed is 361° per 24 hours on R.A. axis.

## Common hardware
* NEMA17 stepper motor
* [3D Printed motor mount and drive](https://www.thingiverse.com/thing:6508420)
* TMC2208 stepper driver or similar. Expensive driver is highly recommended. A4988 will vibrate on low sidereal speed, which is very noticable on high magnification.

## Controls
* Encoder rotate: slow R.A. adjustment
* Encoder click and rotate: coarse R.A. adjustment
* Encoder 2 clicks and rotate: change R.A. adjustment base speed
* Encoder 3 clicks and rotate: fine speed adjustment
* Encoder 4 clicks and rotate: coarse speed adjustments

LCD is used to display debug info on current speed and positions.

### w/ optional remote encoder
* Encoder rotate: slow R.A. adjustment
* Encoder click and rotate: faster R.A. adjustment
* Encoder 2 clicks and rotate: even faster R.A. adjustment


## Zero soldering/wiring version
Hardware:
* Arduino Mega
* [RAMPS 1.4](https://reprap.org/wiki/RAMPS_1.4)
* [RepRapDiscount Full Graphic Smart Controller](https://reprap.org/wiki/RepRapDiscount_Full_Graphic_Smart_Controller)
* You may want to print a case for all those boards later. For example [this bulky but very clever design](https://www.thingiverse.com/thing:3383359).

Install RAMPS on Arduino.
Install jupmers for 1/16 steps on E0 and plug your stepper driver into E0. Plug your NEMA17 motor into E0 motor output. Connect 12V to RAMPS low amp input. Do not exceed 12V, it is an upper limit of what Arduino can take.
Connect LCD board via provided cables. 
[Official wiki](https://reprap.org/wiki/RAMPS_1.4) explains it in great detail.

Flash `RAMPS14_LCD` env to arduino. Assemble and power from a 12V source. Have fun

### Optional remote
Connect an extra encoder with loooong wires to AUX2 pins on RAMPS according to `pins.h`. Print some case for it. Flash `RAMPS14_LCD_REMOTE` env. Now you can use a lightweight remote to control this overkill monstrosity.

## Arduino nano self-serivce version
Hardware:
* Arduino Nano
* Random cheap encoder board
* Buck converter or two
* You may want to print a case for all those boards later.

I've included `nano_small` env. Pinout is in `pins.h`, adjust to your preferences. Maybe install a cap on motors 12v input. Wire, solder, assemble and have fun.

# Future
I don't like Arduino projects. I rely on those things only if any other solutions for my problem are astronomically (pun intended) expensive or nonexistent. I'm good with soldering, but I absolutely hate designing and debugging circuits, so don't expect much.

I created this for myself and i am using it. Current revision is "good enough" for me. Maybe I'll either improve the code to display actually useful info on the screen or go for compact version based on Nano.

# Contribution
Position adjustment via encoders is clunky, if you know how to make the input more responsive and connected to encoder movement, feel free to submit PRs. My intention is that once you start rotating the encoder, the axis motion mimics encoder movement at some preset ratio.


# Support
ETH/Polygon: `0x7c888B61286Ea00E01e2b6979A2265588f7dfDaA`
TRX: `TFMuwm3zz5a5uDFUMei9tCUeUYCQNEuQTt`


[![CC BY-NC-SA 4.0][cc-by-nc-sa-shield]][cc-by-nc-sa]

This work is licensed under a
[Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License][cc-by-nc-sa].

[![CC BY-NC-SA 4.0][cc-by-nc-sa-image]][cc-by-nc-sa]

[cc-by-nc-sa]: http://creativecommons.org/licenses/by-nc-sa/4.0/
[cc-by-nc-sa-image]: https://licensebuttons.net/l/by-nc-sa/4.0/88x31.png
[cc-by-nc-sa-shield]: https://img.shields.io/badge/License-CC%20BY--NC--SA%204.0-lightgrey.svg