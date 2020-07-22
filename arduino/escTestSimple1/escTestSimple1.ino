#include <Servo.h>
Servo esc_signal;

void setup()
{
  esc_signal.attach(6);  //Specify here the pin number on which the signal pin of ESC is connected.
  esc_signal.write(0);   //ESC arm command. ESCs won't start unless input speed is less during initialization.
  delay(6000);            //ESC initialization delay.
}

void loop()
{
esc_signal.write(180);    //Vary this between 40-130 to change the speed of motor. Higher value, higher speed.
//delay(15);
}
