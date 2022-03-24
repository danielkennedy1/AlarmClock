#include <Servo.h>

Servo myservo;  // create servo object to control a servo

int pos = 0;    // variable to store the servo position

void setup() {
    myservo.attach(10);  // defines digital pin 10 as the one controlling the servo
}
void loop(){



  if(digitalRead(2) == HIGH){              //waits for signal
    for (pos = 0; pos <= 180; pos += 1) { 
    myservo.write(pos);              //moves servo to press button on kettle
    delay(15);                       
  }
  for (pos = 180; pos >= 0; pos -= 1) { 
    myservo.write(pos);              //moves back away from the button
    delay(15);                      
  }
  }
}
