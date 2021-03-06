PolarityFlip
=====

## The Story

    I found a client that had a specific need that could not be met at the
    local electronics store or Amazon.  He wanted a portable device that
    could take a variable DC input (5V-24V), and flip the polarity on the
    output to change the direction of a DC motor.  He wanted it timed, from
    1 second to 1000 seconds between polarity flips.  It was to keep doing
    this as long as it was on.

    So I designed the hardware first, I used an Arduino Nano, a rotary
    encoder, a small OLED display and a double pole double throw relay.  I
    set up the code to read the rotary encoder and change the time variable. 
    I setup a interupt driven function to change the state of the relay as
    needed by the time requested.  Then I designed a case in OpenSCAD and
    printed it out.

## Features

    Runs on a Arduino Nano and powers from the USB port.  The relay is a signal
    relay that can handle up to 2 Amps of current on the rely contacts.  I
    extended the timer from .5 seconds to 10,000 seconds.

    There is no reason that this HAS to run on a Arduino Nano, it could run
    on any 8 bit deivce with GPIO pins.

## Usage
    
    Plug in USB power to the port, you will see T: 500 on the display, turn
    the rotary encoder left or right and see the number change.  If you
    listen closely you will hear the relay click as it flips the polarity.

## The source code

    All Arduino C++, and two library.  The relay is on digital pin 7, the
    rotary encoder is in digital pins 2, 3 and 4, though only pins 2 and 3 are
    used. Pin 4 is the switch built into the rotary encoder and not used in
    this application.

    PolarityFlip.ino			by David Mandala - GPL v3 only
    README.md		    		This file
 
## Requires

    PolarityFlip.ino require both ssd1306 and EncoderStepCounter librarys. 
    In reality both could be bypassed and direcly handled but using the
    librarys made it days if not a week faster to get the code written. Time
    is money so....

    ssd1306 v 1.3.0 library		by Bill Greiman  - GPL v3 and later
    EncoderStepCounter v 1.1.0 library	by Manuel Reimer - LGPL v3.0

## License

    The GPLv3 license only, no later versions. See GPLv3-LICENSE.txt
