/*

   Polarity flip via DPDT relay
   Copyright David Mandala Developed by THEM, LLC
   GNU General Public License v3.0 ONLY

*/
#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiWire.h"
#include "EncoderStepCounter.h"

#define MAX_SECONDS 1000000L

#define MIN_SECONDS  5
#define TIMER_INTERVAL 1
#define TIMER_INTERVAL2 10

#define ENCODER_PIN1  2
#define ENCODER_PIN2  3
#define RELAY_PIN     7

// 0X3C+SA0 - 0x3C or 0x3D
#define I2C_ADDRESS 0x3C

SSD1306AsciiWire oled;
EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2);

void init_interrupt_timers(void){
    cli();                                //stop interrupts

    //set timer0 interrupt at 2kHz
    TCCR0A = 0;// set entire TCCR0A register to 0
    TCCR0B = 0;// same for TCCR0B
    TCNT0  = 0;//initialize counter value to 0
    // set compare match register for 2khz increments
    OCR0A = 124;// = (16*10^6) / (2000*64) - 1 (must be <256)
    // turn on CTC mode
    TCCR0A |= (1 << WGM01);
    // Set CS01 and CS00 bits for 64 prescaler
    TCCR0B |= (1 << CS01) | (1 << CS00);   
    // enable timer compare interrupt
    TIMSK0 |= (1 << OCIE0A);
    
    TCCR1A = 0;                           // set entire TCCR1A register to 0
    TCCR1B = 0;                           // same for TCCR1B
    TCNT1  = 0;                           //initialize counter value to 0
    TCCR1B |= (1 << WGM12);               // turn on CTC mode
    OCR1A = 19999;
    TCCR1B |= (0 << CS12) | (1 << CS11) | (0 << CS10);
    TIMSK1 |= (1 << OCIE1A);              // enable timer compare interrupt
    sei();                                //allow interrupts
    return;
}

void flip_relay(void){
    //  Flip relay, and flash onboard LED 
    digitalWrite( LED_BUILTIN,(digitalRead(LED_BUILTIN) ^ 1));
    digitalWrite( RELAY_PIN,(digitalRead(RELAY_PIN) ^ 1));
    return;
}

// This timer runs at 2kHz, on a hardware interupt.
// Reads the rotary encoder so we don't miss any rotations.
ISR(TIMER0_COMPA_vect){
    // Pump the rotary encoder library
    encoder.tick();
}

/* 
 * TimerVal used in the next 4 functions, it's a volatile var since it's used in the
 * interupt driven TIMER1 function but will be set in the main system loop and read  
 * in the display function.
 * 
 */

static volatile long TimerVal = 50000;

void display_timer_value(void) {
    float temp;
    temp = TimerVal * .01;
    // Place holder for internal display call
    Serial.print("Timer Val: ");
    Serial.println(temp);
    oled.clear();
    oled.set1X();
    oled.println();
    oled.set2X();
    oled.print("T: ");
    oled.println(temp);
    return;
}

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);

    Wire.begin();
    Wire.setClock(400000L);
    oled.begin(&Adafruit128x64, I2C_ADDRESS);
    oled.setFont(TimesNewRoman16_bold);
    oled.clear();
    oled.set1X();
    oled.println();
    oled.set2X();
    oled.print("T: ");
    oled.println(TimerVal * .01);
    
    Serial.begin (9600);
    init_interrupt_timers();
    encoder.begin();
    return;
}

static long Step_Increment = 100;

void loop(void) {
    signed char pos;
    pos = encoder.getPosition();
    if (pos != 0) {
        if (pos >0 && TimerVal < MAX_SECONDS) {
            if (TimerVal == 10){Step_Increment = TIMER_INTERVAL2;}
            else if (TimerVal == 100){ Step_Increment = 100;}
            TimerVal+=Step_Increment;
        }else if (pos < 0 && TimerVal > MIN_SECONDS) {
            if (TimerVal == 100){ Step_Increment = TIMER_INTERVAL2;}
            else if (TimerVal == 10){ Step_Increment = TIMER_INTERVAL;}
            TimerVal-=Step_Increment;
        }
        encoder.reset();
        display_timer_value();
    }
    return;
}

// This timer runs at 100Hz on a hardware interupt.
// Runs the relay to flip polarity on the output.
ISR(TIMER1_COMPA_vect){
    static long cnt;

    cnt += TIMER_INTERVAL;
    if (cnt >= TimerVal){
      cnt = 0;
      flip_relay();
    }
}
