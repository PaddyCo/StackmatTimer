/******************************
This is an example for the Stackmat Timer library.

This example utilizes SoftwareSerial to make a software based
serial connection, which means it should work with basically
any digital pin on your board.

+====================+
| /!\ WARNING!!! /!\ |
+====================+
Stackmat timers communicate using the RS-232 protocol.
The valid range for a RS-232 signal is between +3V to +15V!
Signals above +5V will probably not end well for your Microcontroller, so you might want
to find a good way to guarantee your data signal is at an acceptable level.
(Maybe using a MAX232 or similar)

The timers I have tried seem to send the data at around 3V, 
which in my case simply works, but this does NOT mean it is the same
for all timers out there, and I do NOT guarantee the safety of your microcontroller.
+====================+
| /!\ WARNING!!! /!\ |
+====================+

Connect the mono audio from your Stackmat Timer of choice like so:
(Make sure to read the warning above)
+-----------------------+
| Audio Cable | Arduino |
+-----------------------+
|  Audio (-)  | GND     |
|  Audio (+)  | D7      |
+-----------------------+

D7 can be substituted with any digital pin on your board, just make 
sure to update the RX_PIN define in the example!

******************************/

#include <Arduino.h>
#include <SoftwareSerial.h>
#include <StackmatTimer.h>

#define RX_PIN D7 // The pin to use for data transmission, can be any digital pin

// Setup SoftwareSerial connection, parameters are:
// receivePin, transmitPin and inverse_logic
// Since we don't need transmitPin, we simply set it to 255
// inverse logic should be set to true
SoftwareSerial mySerial(RX_PIN, 255, true);

// Declare stackmat timer instance
StackmatTimer timer(&mySerial);
StackmatTimerState lastState;

void setup() {
    Serial.begin(19200);

    // Start serial, Baud rate is set to 1200
    mySerial.begin(STACKMAT_TIMER_BAUD_RATE);
}

void loop() {
    // Update the timer, this will update all the fields on the timer like State and Time.
    timer.Update();

    // If timer data has not been updated for an entire second,
    // assume timer is disconnected
    if (!timer.IsConnected()) {
      Serial.println("Timer is disconnected! Make sure it is connected and turned on.");
      delay(1000);
    }
    
    // Notify if the state changes
    if (timer.GetState() != lastState) {
        switch (timer.GetState())
        {
        case ST_Stopped:           
            Serial.printf("FINISH! Final time is %i:%02i.%03i!\n", timer.GetDisplayMinutes(), timer.GetDisplaySeconds(), timer.GetDisplayMilliseconds());
            break;
        case ST_Reset:
            Serial.println("Timer is reset!");
            break;
        case ST_Running:
            Serial.println("GO!");
            break;
        case ST_LeftHandOnTimer:
            Serial.println("Left hand on timer!");
            break;
        case ST_RightHandOnTimer:
            Serial.println("Right hand on timer!");
            break;
        case ST_BothHandsOnTimer:
            Serial.println("Both hands on timer!");
            break;
        case ST_ReadyToStart:
            Serial.println("Ready to start!");
            break;
        default:
            Serial.println("Unknown state!");
            Serial.println(timer.GetState());
            break;
        }
    }

    // If timer is running, print time every loop
    if (timer.GetState() == ST_Running) {
        Serial.printf("%i:%02i.%03i\n", timer.GetInterpolatedDisplayMinutes(), timer.GetInterpolatedDisplaySeconds(), timer.GetInterpolatedDisplayMilliseconds());
    }

    lastState = timer.GetState();

    delay(50);
}
