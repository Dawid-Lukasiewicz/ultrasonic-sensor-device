#include <Servo.h>
#include <math.h>
#define trigPin 6
#define echoPin 5
#define PI 3.14

uint16_t DisCM()
{
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    return (pulseIn(echoPin, HIGH) / 58);
}

Servo Serw;
int16_t Position = 0, Change = 5, DelayedTimeMs = 50;
float Radians = 0;
uint8_t flag = 0;
int cm_value = 0;
float Xl, Yl;

void setup()
{
    // Initialazing serial port baud rate
    Serial.begin(9600);

    // Initialazing servo motor
    Serw.attach(3);
    Serw.write(Position);

    // Initialazing ultrasonic sensor HC-SR04
    pinMode(trigPin, OUTPUT); // Trig pin - input of sensor
    pinMode(echoPin, INPUT);  // Echo pin - output of sensor
    delay(2000);
}

void loop()
{
    if (Serial.available() > 0)
    {
        Serial.read();
        while (Position <= 180 && Position >= 0)
        {

            // Measure distance
            cm_value = DisCM();
            if(cm_value > 50)
              cm_value = 50;
            // Calculate coordinates
            Radians = Position*PI/180;
            Xl = (double)cm_value * cos(Radians);
            Yl = (double)cm_value * sin(Radians);

            // send result in data frame ":L X Y\n" from proximity sensor
            Serial.print(":");      //Start of frame
            Serial.print(cm_value); //L value
            Serial.print(" ");      //Gap between values
            // send X coordinates
            Serial.print(Xl, 2);    //X value
            Serial.print(" ");      //Gap between values
            // send Y coordinates
            Serial.print(Yl, 2);    //Y value
            Serial.println();

            // Change servo motor position
            switch (flag)
            {
            case 0:
                Position += Change;
                Serw.write(Position);
                delay(DelayedTimeMs);
                break;
            case 1:
                Position -= Change;
                Serw.write(Position);
                delay(DelayedTimeMs);
                break;
            default:
                break;
            }
        }
        // Change direction of servo motor flag
        if (Position >= 180)
        {
            flag = 1;
            Position = 180;
            Serw.write(Position);
        }
        else if (Position <= 0)
        {
            flag = 0;
            Position = 0;
            Serw.write(Position);
        }
    }
}
