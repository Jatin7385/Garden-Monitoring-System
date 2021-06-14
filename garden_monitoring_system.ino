#include <Servo.h>
int celsius;
int red,blue,green = 0;
int shadow = 0;
int dist = 0;
const int pingPin = 7;
const int servoPin = 6;
int begin = 0;
Servo servo;
void setup()
{
  servo.attach(servoPin);
  pinMode(A0, INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(8,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  Serial.begin(9600);
}

void loop()
{
  celsius = map(((analogRead(A0) - 20) * 3.04), 0, 1023, -40, 125);
  if(celsius<30)
  {
    red = 0;green = 255;blue = 0;
  }
  else if(celsius>30 && celsius<40)
  {
    red = 255;green = 255;blue = 0;
  }
  else
  {
    red = 255;green = 0;blue = 0;
  }
  dist = analogRead(A2);
  shadow = analogRead(A1);
  Serial.println(dist);
  if(shadow<300){digitalWrite(8,HIGH);delay(200);digitalWrite(8,LOW);}
  
  // establish variables for duration of the ping, and the distance result
  // in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  if(cm<=20)
  {
    if(begin == 0){begin = 180;}
    else{begin = 0;}
    servo.write(begin);
  }

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  
  Serial.println(celsius);
  analogWrite(11,red);
  analogWrite(10,blue);
  analogWrite(9,green);
  delay(1000);
}
long microsecondsToInches(long microseconds) {
  // According to Parallax's datasheet for the PING))), there are 73.746
  // microseconds per inch (i.e. sound travels at 1130 feet per second).
  // This gives the distance travelled by the ping, outbound and return,
  // so we divide by 2 to get the distance of the obstacle.
  // See: https://www.parallax.com/package/ping-ultrasonic-distance-sensor-downloads/
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the object we
  // take half of the distance travelled.
  return microseconds / 29 / 2;
}