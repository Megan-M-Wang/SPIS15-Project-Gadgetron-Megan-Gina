/** Name: Catapultinator
 * Authors: Gina Ratto and Megan Wang
 * Date: August 4, 2015
 * Description: Our amazing catapult!!!
 */

//The various libraries to control the parts
#include <MomentaryButton.h> // This library allows us to control buttons
#include <PinChangeInt.h>  //This library allows an almost constant awareness of the button's state
#include <Motor.h>  // This library allows us to move the wheels to move the robot
#include <LED.h>  // This library allows us to turn on and off LEDs
#include <Buzzer.h> //This library allows us to use the buzzer
#include <Song.h> 


//Put your const variables here

// Motor pins
int STBY = 13;
int PWMA = 10;
int AIN1 = 12;
int AIN2 = 11;
int PWMB = 9;
int BIN1 = A0;
int BIN2 = A1;

//Catapult Motor
int CATSTBY = 13;
int CATPWMA = 6;
int CATAIN1 = 4;
int CATAIN2 = 5;
int CATPWMB = 7;  //Dummy pin that doesn't exist
int CATBIN1 = 7;  //Dummy pin that doesn't exist
int CATBIN2 = 7;  //Dummy pin that doesn't exist

//Other pins
int PINLEDS = A5;
int PINBUTTON = A2;
int PINBUMPSENS = 3;
int PINBUZZ = 8;

//Have one or more state variables here (if needed)

const int IDLE = 0;
const int BRAKE = 1;
const int SONG = 2;
const int LIGHTON = 3;
const int LIGHTOFF = 4;
const int FORWARD = 5;
const int SPINRIGHT = 6;
const int ISPR = 7;
const int BACKWARD = 8;
int state = IDLE;
float savedTime = 0;

//These make the various objects and assign them names
Motor motor(STBY, PWMA, AIN1, AIN2, PWMB, BIN1, BIN2);  //Wheel motors
Motor catapult(CATSTBY, CATPWMA, CATAIN1, CATAIN2, CATPWMB, CATBIN1, CATBIN2);  //Catapult motor
LED led(PINLEDS);  //Group of all LEDs
MomentaryButton button(PINBUTTON);  //Button control
MomentaryButton bumpsensor(PINBUMPSENS);  //bump sensor control
Buzzer buzzer(PINBUZZ);  //Buzzer control
Song song(buzzer);

//Setup function
void setup() {
  motor.setup();
  catapult.setup();
  button.setup();
  bumpsensor.setup();
  
}

//Loop function
void loop() {
  // put your main code here, to run repeatedly:
  switch(state){
    case IDLE:
      idleState();
      break;
    case BRAKE:
      brakeState();
      break;
    case SONG:
      songState();
      break;
    case LIGHTON:
      lightonState();
      break;
    case LIGHTOFF:
      lightoffState();
      break;
    case FORWARD:
      forwardState();
      break;
    case BACKWARD:
      backwardState();
      break;
    case SPINRIGHT:
      spinrightState();
      break;
    case ISPR:
      interSPR();
      break;
       
  }
}

void idleState() {
 //IDLE State where pressing the button starts the music
 if (button.wasPressed()) {
   delay(250);
   state = SONG;
 }
}

void brakeState() {
  delay(250);
  motor.brake();
  led.turnOff();
  buzzer.turnOff();
  song.restartSong();
  savedTime = millis();
  delay(1000);
  state = IDLE;
}

void songState() {
  delay(250);
  //Starts to play the song
  song.playNextNote();
  state = FORWARD;
  if (song.getIndex() % 5 == 0) {
    state = LIGHTON;
  }
  if (song.getIndex() % 11 == 0) {
    state = LIGHTOFF;
  }
 if ((millis() - savedTime) > 5000) {
   state = ISPR;
 }
 if (song.getIndex() == song.getLength()) {
   state = BRAKE;
 }
 if (button.wasPressed()) {
   state = BRAKE;
 }
 }
 


void lightonState() {
  //Turns the leds on
  led.turnOn();
  delay(250);
  state = SONG;
  if (button.wasPressed()) {
   state = BRAKE;
 }
}

void lightoffState() {
  led.turnOff();
  delay(250);
  state = SONG;
  if (button.wasPressed()) {
   state = BRAKE;
 }
}



void forwardState() {
  //Makes the robot go forward 
  motor.forward(255);
  state = SONG;
  //Brake if button pressed
  if (bumpsensor.isPressed()) {
    state = BACKWARD;
    delay(250);
  }
  if (button.wasPressed()) {
   state = BRAKE;
 }  
}

void backwardState() {
  motor.backward(255);
  delay(400);
  state = ISPR;
  delay(250);
}

void spinrightState() {
  motor.spinRight();
  delay(250);
  song.playNextNote();
  if (millis() - savedTime > 575) {
    savedTime = millis();
    state = SONG; 
  }
  if (button.wasPressed()) {
    state = BRAKE;
  }
}
  


void interSPR() {
  delay (250);
  savedTime = millis();
  state = SPINRIGHT;
}
