
/**
 * Functions: 
 * Set pins 2 to 7 as inputs
 * Initialise serial communication
 */
void setup() {
  //Set pints 2 to 7 as inputs
  for(int i=2; i<=7; i++){
    pinMode(i, INPUT);
    i++;
  }
  
  Serial.begin(9600);
}

void loop() {
  readNeck();
  selectModes();
  //modulateNote();
  playTone();
}
