/********************** ETUDE 3 CART 360 2019 *******************************
 * WELCOME! 
 * THE PURPOSE OF THIS EXERCISE IS TO DESIGN A VERY SIMPLE KEYBOARD (5 KEYS)
 * WHICH ALLOWS YOU TO PLAY LIVE, RECORD, AND PLAYBACK SINGLE NOTES (NO CHORDS). 
 * THERE WILL BE A BUTTON WHICH WHEN PRESSED WILL TAKE THE USER TO THE NEXT MODE:
 * THERE ARE 5 POSSIBLE MODES
 * 0 ==  reset    ==   led off == also resets
 * 1 ==  live     ==   led BLUE
 * 2 ==  record   ==   led RED
 * 3 ==  play     ==   led GREEN
 * 4 ==  loopMODE ==   led PURPLE
 * 
 * ADDITIONALLY - WHEN THE USER CHANGES MODE, 
 * THE RGB LED WILL CHANGE COLOR (ACCORDING TO THE SPECIFICATIONS)

 * PLEASE FOLLOW THE INSTRUCTIONS IN THE COMMENTS:
 * DO NOT ADD ANY MORE FUNCTION DEFINITIONS 
 * ONLY IMPLEMENT THE FUNCTION DEFINITIONS SUPPLIED
 * THERE IS NO NEED TO ADD ANY NEW VARIABLES OR CONSTANTS
 * PROVIDE COMMENTS FOR ANY OF THE CODE IMPLEMENTED
 * GOOD LUCK!
 */
/**** CONSTANTS ********************************************************/

#define BUTTON_MODE_PIN 2 // Button to change the mode

// constants for RGB LED
#define LED_PIN_R 9 // R PIN
#define LED_PIN_G 6 // G PIN
#define LED_PIN_B 5 // B PIN

// constant for note in (button-resistor ladder on breadboard)
# define NOTE_IN_PIN A0

//constant for max number of notes in array
#define MAX_NOTES 16

// a constant for duration
const int duration = 200;

// constant for pin to output for buzzer 
#define BUZZER_PIN 3 // PWM

/******** VARIABLES *****************************************************/
// counter for how many notes we have
int countNotes =0;
// array to hold the notes played (for record/play mode)
int notes [MAX_NOTES]; 
int mode =0; // start at off
// int value to store the note value read
int freq =0;

/*************************************************************************/


/**********************SETUP() DO NOT CHANGE*******************************/
// Declare pin mode for the single digital input
void setup()
{
  pinMode(BUTTON_MODE_PIN, INPUT);
//  Debug Serial Code
//  Serial.begin(9600);
}

/**********************LOOP() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change the loop - it establishes the flow of the program
 * We call here 3 functions repeatedly:
 * 1: chooseMode(): this function will determine the mode that your program is in 
 * based on if the button (linked to the BUTTON_MODE_PIN) was pressed
 * 2: setRGB(): will set the color of the RGB LED based on the value of the mode variable
 * 3: selectMode(): will determine which function to call based on the value of the mode variable

**************************************************************************/
void loop()
{
  chooseMode();
  setRGB();
  selectMode();
}
/******************CHOOSEMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Read the value from the Button (linked to the BUTTON_MODE_PIN) and 
 * if is being pressed then change the mode variable.
 * REMEMBER:
 * mode == 0 is reset 
 * mode == 1 is live
 * mode == 2 is record
 * mode == 3 is play
 * mode == 4 is loopMode
 * Every time the user presses the button, the program will go to the next mode,
 * once it reaches 4, it should go back to mode == 0. 
 * (i.e. if mode ==2 and we press, then mode ==3) ...
**************************************************************************/
void chooseMode(){
  // int modeVal is the shorthand I use to check if BUTTON_MODE_PIN is HIGH 
  int modeVAL = digitalRead(BUTTON_MODE_PIN);
  // if mode is zero (which it is at the start)
    if (mode == 0){
      // and if you hit the mode button
      if (modeVAL == 1){
        // set mode to 1
      mode = 1;
        // in lieu of break in order to not skip through to step 4 all of a sudden helps provide controle 
      delay(duration);
      }
      // else if mode is x & mode button is hit then set mode to x+1 (basically rinse and repeate for the rest of chooseMode)
    } else if (mode == 1){
      if (modeVAL == 1){
      mode = 2;
      delay(duration);
      }
    } else if (mode == 2){
      if (modeVAL == 1){
      mode = 3;
      delay(duration);
      }
    } else if (mode == 3){
      if (modeVAL == 1){
      mode = 4;
      delay(duration);
      }
    } else if (mode == 4){
      if (modeVAL == 1){
      mode = 0;
      delay(duration);
    }
  }
}

/******************SETRGB(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * Depending on the value of the mode variable:
 * if the mode is 0 - RGB LED IS OFF
 * if the mode is 1 - RGB LED IS BLUE
 * if mode is 2 - RGB LED IS RED
 * if mode is 3 - RGB LED IS GREEN
 * if mode is 4 - RGB LED iS PURPLE
 * YOU MUST USE A SWITCH CASE CONSTRUCT (NOT A SERIES OF IF / ELSE STATEMENTS
**************************************************************************/
void setRGB()
{
  //the switch case here is check the mode and depending which mode its corresponding case is called 
  switch(mode) {
    //if mode 0, analogWrite(LED_PIN_(BRG), 0) essentially keep the pins off analog
    case 0 : analogWrite(LED_PIN_B, 0);
             analogWrite(LED_PIN_R, 0);
             analogWrite(LED_PIN_G, 0);
             //take a break let the program collect itself
             break;
    case 1 : analogWrite(LED_PIN_B, 255);
             analogWrite(LED_PIN_R, 0);
             analogWrite(LED_PIN_G, 0);
             break;
    case 2 : analogWrite(LED_PIN_B, 0);
             analogWrite(LED_PIN_R, 255);
             analogWrite(LED_PIN_G, 0);
             break;
    case 3 : analogWrite(LED_PIN_B, 0);
             analogWrite(LED_PIN_R, 0);
             analogWrite(LED_PIN_G, 255);
             break;
    case 4 : analogWrite(LED_PIN_B, 127);
             analogWrite(LED_PIN_R, 127);
             analogWrite(LED_PIN_G, 0);
             break;
  }
}
/**********************SELECTMODE() DO NOT CHANGE *******************************
 * INSTRUCTIONS: 
 * There is NO NEED to change this function - it selects WHICH function should run based on the mode variable
 * There are 4 possibilities
 * 1: reset(): this function will reset any arrays etc, and will do nothing else
 * 2: live(): this function will play the corresponding notes 
 * to the user pressing the respective buttons. 
 * NOTE:: the notes played are NOT stored
 * 3: record(): this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * 4: play(): this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * 5: loopMode(): this function will playback any notes stored in the array that were recorded, 
 * BUT unlike the previous mode, you can choose in which sequence the notes are played.
 * REQUIRED: only play notes from the array (no live stuff)

******************************************************************************/
void selectMode()
{
  if(mode == 0) { 
    reset();
  }
  else if(mode ==1) {
    live();
  }
  else if(mode ==2) {
    record();
  }
  
  else if(mode ==3) {
    play();
  }
   
   else if(mode ==4) {
    loopMode();
  }
}
/******************RESET(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function should ensure that any notes recorded are no longer available
**************************************************************************/
void reset()
{
  //Reset the countNotes to zero erasing all previous data
  countNotes = 0;
  
}
/******************LIVE(): IMPLEMENT **************************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * NOTE:: the notes played are NOT stored
 * SO: you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * THEN - output the note to the buzzer using the tone() function
**************************************************************************/
void live()
{
  //to discuss resistors in a ladder look at this 
  Serial.println(analogRead(NOTE_IN_PIN));

    //freq is the stored value of the sound from the pin
    freq = analogRead(NOTE_IN_PIN);
    //pass freq through tone into the speaker(BUZZER_PIN) 
    tone(BUZZER_PIN, freq);
//    }
}
/******************RECORD(): IMPLEMENT **********************************
 * INSTRUCTIONS:
 * this function will play the corresponding notes 
 * to the user pressing the respective buttons
 * AND will STORE up to 16 consecutive notes in an array.
 * SO:you need read in the input from the analog input (linked to the button-resistor ladder combo)
 * AND - output the note to the buzzer using the tone() function
 * THEN store that note in the array  - BE CAREFUL - you can only allow for up to MAX_NOTES to be stored
**************************************************************************/
void record()
{
  //if we are playing a Note
  if (analogRead(NOTE_IN_PIN) > 16){
    // and the number of notes played is less than 16
     if (countNotes < MAX_NOTES){
      //get the frequency thats playing
      int freq = analogRead(NOTE_IN_PIN);
      //play that frequency through the buzza *bzzt* *Bzeeepb*
      tone(BUZZER_PIN, freq);
      //push the current frequency into the notes array 
      notes[countNotes] = freq;
      //+1 countNotes goes up by one 
      countNotes++;
      //delay for 200mili
      delay(duration);
    }
    //if a note is not being played, NO SOUND
    noTone(BUZZER_PIN);
  }
  Serial.println(countNotes);
}
/******************PLAY(): IMPLEMENT ************************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ each value IN ORDER
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void play()
{
  for (int i = 0; i < countNotes; i++){
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration);
    if (digitalRead(BUTTON_MODE_PIN) == HIGH){
      noTone(BUZZER_PIN);
      break;
    }
  }
}
/******************LOOPMODE(): IMPLEMENT *********************************
 * INSTRUCTIONS:
 * this function will playback any notes stored in the array that were recorded
 * in the previous mode
 * SO: you need to go through the array of values (be careful - the user may not have put in MAX_NOTES)
 * READ values IN ANY ORDERING (You MUST use the array - but you can determine your own sequence)
 * AND output each note to the buzzer using the tone() function
 * ALSO: as long as we are in this mode, the notes are played over and over again
 * BE CAREFUL: make sure you allow for the user to get to another mode from the mode button...
**************************************************************************/
void loopMode()
{
  //basically the same code as the play mode above
  for (int i = 0; i < countNotes; i++){
    Serial.println(i);
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration);
      if (digitalRead(BUTTON_MODE_PIN) == HIGH){
      noTone(BUZZER_PIN);
      break;
     }
  }
  //including a second for loop to go backwards so that once it finishes its first normal cycle it then regresses back ect
   for (int i = countNotes; i >= 0; i--){
    Serial.println(i);
    tone(BUZZER_PIN, notes[i], duration);
    delay(duration);
    // must include this to escape
     if (digitalRead(BUTTON_MODE_PIN) == HIGH){
      noTone(BUZZER_PIN);
      break;
     }
  }
}

/**************************************************************************/
/*ANALYSIS
 *To break it down simply, more resistors means lower voltage passing through to the speaker
 *since sound is simply the vibrations in the air its important to remeber that a speaker is basically 
 *a tiny trampoline vibrating based on the electrical signal it receives to decide which strength & 
 *what kind of sound it'll make, so lower voltage or more resistors means a weaker sound or a lower pitch.
 */
