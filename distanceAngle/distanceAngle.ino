#include <SoftwareSerial.h>

#include <Servo.h>

Servo myservo;

const int Lin = 10, Rin = 12, Lout = 11, Rout = 13, serv = 9; //setting sensor pins and servo pin

// establish variables for duration
// and the distance result in inches
long Rduration, Lduration, Rinches, Linches;

int threshold = 50; //Sensor threshold in inches

int angle = 10; //Initial angle

boolean debug = true; //Serial communication for debuging. Set to true for serial communication.

void setup() {
  // initialize serial communication:
  Serial.begin(9600);
  myservo.attach(9); //attach servo to pin 9
}

void loop()
{

  //Most of the sensor code has been taken from David Mellis's PING sensor code
  //I modified it for a 4 pin sensor as oppsed to the 3 pin sensor
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(Rout, OUTPUT);
  digitalWrite(Rout, LOW);
  delayMicroseconds(2);
  digitalWrite(Rout, HIGH);
  delayMicroseconds(5);
  digitalWrite(Rout, LOW);

  Rduration = pulseIn(Rin, HIGH);

  pinMode(Lout, OUTPUT);
  digitalWrite(Lout, LOW);
  delayMicroseconds(2);
  digitalWrite(Lout, HIGH);
  delayMicroseconds(5);
  digitalWrite(Lout, LOW);

  Lduration = pulseIn(Lin, HIGH);

  // convert the time into a distance
  Rinches = microsecondsToInches(Rduration);
  Linches = microsecondsToInches(Lduration);

  
  follow();
  Serial.write(Rinches);
  Serial.write(Linches);
  Serial.write(angle);
  delay(50);
}

long microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

void follow()
{
  boolean switchs = true;
  if (Linches <= threshold || Rinches <= threshold)
  {
    if (Linches + 2 < Rinches)
    {
      angle = angle - 2;
    }
    if (Rinches + 2 < Linches)
    {
      angle = angle + 2;
    }

  } else {
    while (Linches > threshold && Rinches > threshold) {

      if (switchs) {
        angle = angle + 2;
      } else {
        angle = angle - 2;
      }
      if (angle > 160)
      {
        angle = 160;
        switchs = false;
      }
      if (angle < 0)
      {
        angle = 0;
        switchs = true;
      }
      myservo.write(angle);
      
      pinMode(Rout, OUTPUT);
      digitalWrite(Rout, LOW);
      delayMicroseconds(2);
      digitalWrite(Rout, HIGH);
      delayMicroseconds(5);
      digitalWrite(Rout, LOW);

      Rduration = pulseIn(Rin, HIGH);

      pinMode(Lout, OUTPUT);
      digitalWrite(Lout, LOW);
      delayMicroseconds(2);
      digitalWrite(Lout, HIGH);
      delayMicroseconds(5);
      digitalWrite(Lout, LOW);

      Lduration = pulseIn(Lin, HIGH);

      // convert the time into a distance
      Rinches = microsecondsToInches(Rduration);
      Linches = microsecondsToInches(Lduration);
    }
  

  }

  if (angle > 160)
  {
    angle = 160;
  }
  if (angle < 0)
  {
    angle = 0;
  }
  myservo.write(angle);


}

