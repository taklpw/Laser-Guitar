/**
 * @author Kelly Lynch
 * @date 2016-11-30
 * @brief Software for a laser guitar synth which shines lasers onto phototransistors and uses 
 * the fact as to whether they're blocked or not to determine what sound to produce.
 * 
 * @bugs None known
 * @todo Add attack/decay functionality, Add pitch slider as either membrane variable resistor or ultrasonic sensor, make interrupt driven
 */

/* Global Variables */
int audioOutPin = A0;

/**
 * @name setup()
 * @brief Initialises inputs and outputs
 * @param void
 * @return void
 */
void setup() {
  /* Set pints 2 to 7 as outputs */
  /* Laser diodes */
  for(int i=2; i<=7; i++){
    pinMode(i, OUTPUT);
  }
  startupSequence(50, 2);
  
  /* Set pints 8 to 13 as inputs */
  /* Phototransistors */
  for(int i=8; i<=13; i++){
    pinMode(i, INPUT);
  }
}

/**
 * @name loop()
 * @brief runs infinitely
 * @param void
 * @return void
 */
void loop() {
  playTone(getTone());
}

/**
 * @name startupSequence()
 * @brief produces a sequence of inputs to create a simple animation upon startup
 * @param int delayTime: Time factor which determines speed of animation, int cycles: determines how many times the animations run
 * @return void
 */
void startupSequence(int delayTime, int cycles){
    /* Back and Forth */
    int previ = 0;
    for(int j=0; j<cycles; j++){
      /* Back */
      for(int i=2; i<=7; i++){
        digitalWrite(i, HIGH);
        if(i > 2){
          digitalWrite(previ, LOW);
        } 
        delay(delayTime);
        previ = i;
      }
      /* Forth */
      for(int i=8; i>=2; i--){
        digitalWrite(i, HIGH);
        if(i < 8){
          digitalWrite(previ, LOW);
        } 
        delay(delayTime);
        previ = i;
      }
    }
    
    /* Flash */
    for(int j=0; j<cycles; j++){
      for(int i=2; i<=7;i++){
        digitalWrite(i, HIGH);  
      }
      delay(delayTime*10);
      for(int i=2; i<=7;i++){
        digitalWrite(i, LOW);  
      }
      delay(delayTime*10);
      for(int i=2; i<=7;i++){
        digitalWrite(i, HIGH);  
      }
    }
}

/**
 * @name getTone()
 * @brief  Checks the states of all the phototransistors and pitch shifting variable 
 * reistors and outputs a corrosponding tone level
 * @param void
 * @return int - The value for which the 
 */
int getTone(){
  /* Variabale intialisation */
  int notes[6] = {0,0,0,0,0,0}; 
  int average = 0;
  int total = 0;
  int notesCount = 0;
  
  /// For future use in the case of a neck ///
  int multiplier = 256;
  
  int notePin7 = 1*multiplier;
  int notePin6 = 2*multiplier;
  int notePin5 = 3*multiplier;
  int notePin4 = 4*multiplier;
  int notePin3 = 5*multiplier;
  int notePin2 = 6*multiplier;
  
  /* If a note is being pressed its value is added to the notes array */  
  if(!digitalRead(13)){
    notes[5] = notePin7;
  } 
  if(!digitalRead(12)){
    notes[4] = notePin6;
  } 
  if(!digitalRead(11)){
    notes[3] = notePin5;
  } 
  if(!digitalRead(10)){
    notes[2] = notePin4;  
  }
  if(!digitalRead(9)){
    notes[1] = notePin3;
  }
  if(!digitalRead(8)){
    notes[0] = notePin2;
  }
  
  /* Average of all note values is calculated*/
  for(int i=0; i<=5; i++){
    if(notes[i] != 0){
      total += notes[i];
      notesCount++;
    }
  } 
  average = total/notesCount;

  return average;
}


/**
 * @brief  Checks the states of all the phototransistors and pitch shifting variable 
 * reistors and outputs a corrosponding tone level
 * @param void
 * @return int - The value for which the 
 */
void playTone(int toneLevel){
  //if there's no note meant to be played, don't play it, otherwise play it.
  if(toneLevel<=0){
    noTone(audioOutPin);
  }else{
    tone(audioOutPin, toneLevel);
  }
}


