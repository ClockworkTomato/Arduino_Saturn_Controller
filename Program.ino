#include <Joystick.h>

// WIP - This program maps the buttons of a Sega Saturn controller to DirectInput using an Arduino Micro

Joystick_ Joystick(0x03, JOYSTICK_TYPE_GAMEPAD, 
                  9, //number of buttons
                  1, //number of dpads
                  false, false, false, false, false, false, false, false, false, false, false); //no other components

const short int numButtons = 8; //number of single buttons -1
const short int numDpad = 3; //number of arrows -1
const short int numTot = numButtons + numDpad +1; //total number of buttons -1

short int buttonState[numTot +1]; //store button states
short int OldButtonState[numTot +1]; //store previous ones


void setup(){
  
  short int i;

  for(i=0; i <= numTot; i++){

    //set buttons to zero (unpressed)
    buttonState[i]=0;
    OldButtonState[i]=0;
  }

  
  for(i=0; i <= numButtons; i++){
    
    //setup Arduino pins for buttons, 2 through 10
    pinMode(i+2,INPUT_PULLUP);
  }

  //the Arduino clone I'm using doesn't have enough digital pins, so the dpad will use the analog ones
  pinMode(A0,INPUT_PULLUP);
  pinMode(A1,INPUT_PULLUP);
  pinMode(A2,INPUT_PULLUP);
  pinMode(A3,INPUT_PULLUP);


  //initializes the joystick; "false" only updates its state when "sendState" is called for
  Joystick.begin(false); 
  
}

void loop(){

  short int i;

  //check if buttons are pressed
  for(i=0; i<=numButtons; i++){ 

    buttonState[i] = !digitalRead(i+2); //buttons are grounded when pressed
    
    if( buttonState[i] != OldButtonState[i] ){

      Joystick.setButton(i, buttonState[i]); //buttons are numbered, from 0 to numButtons-1
      OldButtonState[i] = buttonState[i];
    }
  }


  //check if the dpad is pressed
  buttonState[numButtons +1] = !digitalRead(A0); //left
  buttonState[numButtons +2] = !digitalRead(A1); //up
  buttonState[numButtons +3] = !digitalRead(A2); //right
  buttonState[numButtons +4] = !digitalRead(A3); //down

  //if any arrow has been pressed or unpressed it detects which one
  if( buttonState[numButtons +1] != OldButtonState[numButtons +1]
      || buttonState[numButtons +2] != OldButtonState[numButtons +2]
      || buttonState[numButtons +3] != OldButtonState[numButtons +3]
      || buttonState[numButtons +4] != OldButtonState[numButtons +4] ) {

    //if no arrow is pressed, resets the dpad number 0 (the only dpad available) to its default position (-1) i.e. with no arrow pressed
    if( buttonState[numButtons +1] == 0 && buttonState[numButtons +2] == 0 && buttonState[numButtons +3] == 0 && buttonState[numButtons +4] == 0 )
      Joystick.setHatSwitch(0,-1);

    //otherwhise, it checks which arrow or combination thereof is actually pressed; it doesn't check physically impossible ones, like "up-left" + "right+down"
    //the dpad direction is set by specifying its angle, starting from "up" as 0 degrees and moving clockwise
    else if( buttonState[numButtons +1] == 1){
      if( buttonState[numButtons +2] == 1 )  Joystick.setHatSwitch(0, 315);
      else if ( buttonState[numButtons +4] == 1 )  Joystick.setHatSwitch(0, 225);
      else Joystick.setHatSwitch(0, 270);
    }

    else if( buttonState[numButtons +2] == 1){
      if( buttonState[numButtons +3] == 1 )   Joystick.setHatSwitch(0, 45);
      else Joystick.setHatSwitch(0, 0);
    }

    else if( buttonState[numButtons +3] == 1){
      if( buttonState[numButtons +4] == 1 )   Joystick.setHatSwitch(0, 135);
      else Joystick.setHatSwitch(0, 90);
    }

    else Joystick.setHatSwitch(0, 180);

    OldButtonState[numButtons +1] = buttonState[numButtons +1];
    OldButtonState[numButtons +2] = buttonState[numButtons +2];
    OldButtonState[numButtons +3] = buttonState[numButtons +3];
    OldButtonState[numButtons +4] = buttonState[numButtons +4];
  }
  

  //update the Joystick state
  Joystick.sendState(); 

  delay(10); //maybe that's too much input lag. Should I even be using this function for a controller?
}

