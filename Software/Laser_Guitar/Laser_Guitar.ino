int audioOutPin = A0;

/**
 * Functions: 
 * Set pins 2 to 7 as inputs
 * Initialise serial communication
 */
void setup() {
  // Set pints 2 to 7 as outputs
  // Laser diodes
  for(int i=2; i<=7; i++){
    pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
  //Set pints 8 to 13 as inputs
  // Phototransistors
  for(int i=8; i<=13; i++){
    pinMode(i, INPUT);
  }
  
}

void loop() {
  playTone(getTone());
}

int getTone(){
  int notes[6];
  
  int average = 0;
  int total = 0;
  int notesCount = 0;
  
  // For future use in the case of a neck
  int multiplier = 256;
  
  int notePin7 = 1*multiplier;
  int notePin6 = 2*multiplier;
  int notePin5 = 3*multiplier;
  int notePin4 = 4*multiplier;
  int notePin3 = 5*multiplier;
  int notePin2 = 6*multiplier;
  
  // If a note is being pressed its value is added to the notes array  
  if(digitalRead(13)){
    notes[5] = notePin7;
  } 
  if(digitalRead(12)){
    notes[4] = notePin6;
  } 
  if(digitalRead(11)){
    notes[3] = notePin5;
  } 
  if(digitalRead(10)){
    notes[2] = notePin4;
  }
  if(digitalRead(9)){
    notes[1] = notePin3;
  }
  if(digitalRead(8)){
    notes[0] = notePin2;
  }

  for(int i=0; i<=5; i++){
    if(notes[i] != 0){
      total += notes[i];
      notesCount++;
    }
  } 
  
  average = total/notesCount;
  
  return average;
}

void playTone(int toneLevel){
  //if there's no note meant to be played, don't play it, otherwise play it.
  if(toneLevel<=0){
    noTone(audioOutPin);
  }else{
    tone(audioOutPin, toneLevel);
  }
}


