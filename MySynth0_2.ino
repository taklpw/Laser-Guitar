int outputPin  = A0;
int attackPin  = A1;
int releasePin = A2;

int notes[6];
int notePin7;
int notePin6;
int notePin5;
int notePin4;
int notePin3;
int notePin2;

int total          = 0;
int average        = 0;
int oldAverage     = 0;
int notesCount     = 0;

int toneLevel      = 0;
int oldToneLevel   = 0;
int sustainTone    = 0;

int attackCounter  = 0;
int attackRate     = 0;
int attackTime     = 100; //Controlled by user input

int releaseCounter = 0;
int releaseRate    = 0;
int releaseTime    = 100; //Controlled by user input

int modulateLevel  = 1; //Controlled by user input
int modulateCount  = 0;

boolean attacking  = false;
boolean releasing  = false;
boolean sustaining = false;
boolean modulating = false;

int sineTable[256] = {1     ,805   ,1608  ,2411  ,3212  ,4012  ,4808  ,5602  ,6393  ,7180  ,7962  ,8740  ,9512  ,10279 ,11040 ,11793 ,
                      12540 ,13279 ,14010 ,14733 ,15447 ,16152 ,16846 ,17531 ,18205 ,18868 ,19520 ,20160 ,20788 ,21403 ,22006 ,22595 ,
                      23171 ,23732 ,24280 ,24812 ,25330 ,25833 ,26320 ,26791 ,27246 ,27684 ,28106 ,28511 ,28899 ,29269 ,29622 ,29957 ,
                      30274 ,30572 ,30853 ,31114 ,31357 ,31581 ,31786 ,31972 ,32138 ,32286 ,32413 ,32522 ,32610 ,32679 ,32729 ,32758 ,
                      32768 ,32758 ,32729 ,32679 ,32610 ,32522 ,32413 ,32286 ,32138 ,31972 ,31786 ,31581 ,31357 ,31114 ,30853 ,30572 ,
                      30274 ,29957 ,29622 ,29269 ,28899 ,28511 ,28106 ,27684 ,27246 ,26791 ,26320 ,25833 ,25330 ,24812 ,24280 ,23732 ,
                      23171 ,22595 ,22006 ,21403 ,20788 ,20160 ,19520 ,18868 ,18205 ,17531 ,16846 ,16152 ,15447 ,14733 ,14010 ,13279 ,
                      12540 ,11793 ,11040 ,10279 ,9512  ,8740  ,7962  ,7180  ,6393  ,5602  ,4808  ,4012  ,3212  ,2411  ,1608  ,805   ,
                      1     ,-804  ,-1607 ,-2410 ,-3211 ,-4011 ,-4807 ,-5601 ,-6392 ,-7179 ,-7961 ,-8739 ,-9511 ,-10278,-11039,-11792,
                      -12539,-13278,-14009,-14732,-15446,-16151,-16845,-17530,-18204,-18867,-19519,-20159,-20787,-21402,-22005,-22594,
                      -23170,-23731,-24279,-24811,-25329,-25832,-26319,-26790,-27245,-27683,-28105,-28510,-28898,-29268,-29621,-29956,
                      -30273,-30571,-30852,-31113,-31356,-31580,-31785,-31971,-32137,-32285,-32412,-32521,-32609,-32678,-32728,-32757,
                      -32767,-32757,-32728,-32678,-32609,-32521,-32412,-32285,-32137,-31971,-31785,-31580,-31356,-31113,-30852,-30571,
                      -30273,-29956,-29621,-29268,-28898,-28510,-28105,-27683,-27245,-26790,-26319,-25832,-25329,-24811,-24279,-23731,
                      -23170,-22594,-22005,-21402,-20787,-20159,-19519,-18867,-18204,-17530,-16845,-16151,-15446,-14732,-14009,-13278,
                      -12539,-11792,-11039,-10278,-9511 ,-8739 ,-7961 ,-7179 ,-6392 ,-5601 ,-4807 ,-4011 ,-3211 ,-2410 ,-1607 ,-804  };

void setup() {
  //Set pints 2 to 7 as inputs
  int i = 2;
  while(i<=7){
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


/**readNeck() reads the analog input on the neck, calculates values for digital inputs and calculates an average**/
void readNeck(){
  oldAverage = average;
  //Initialises values to avoid overflows and other areas
  average    = 0;
  total      = 0;
  notesCount = 0;
  
  //Determine potential sounds based on slider position
  notePin7 = 1*analogRead(A0);
  notePin6 = 2*analogRead(A0);
  notePin5 = 3*analogRead(A0);
  notePin4 = 4*analogRead(A0);
  notePin3 = 5*analogRead(A0);
  notePin2 = 6*analogRead(A0);

  //Set all values in 'notes[]' to zero 
  int k =0;
  while(k<=5){
    notes[k] = 0;
    k++;  
  }
  
  //If a note is being pressed its value is added to the notes array  
  if(digitalRead(7)){
    notes[5] = notePin7;
  } 
  if(digitalRead(6)){
    notes[4] = notePin6;
  } 
  if(digitalRead(5)){
    notes[3] = notePin5;
  } 
  if(digitalRead(4)){
    notes[2] = notePin4;
  }
  if(digitalRead(3)){
    notes[1] = notePin3;
  }
  if(digitalRead(2)){
    notes[0] = notePin2;
  }

  //If a note is played its value is added to 'total' & number of notes played is added to 'notesCount'
  int j=0;
  while(j<=5){
    if(notes[j]!=0){
      total += notes[j];
      notesCount++;
    }
    j++;
  }
  //Calculates the average from the above
  average = total/notesCount;

}

//Selects modes of operation
void selectModes(){  
  //Set to attack mode if the attack counter is less than the average level and the average is greater than 0
  if(average>0){
    if(attackCounter<average){
      attackNote();
    }else{
      //Otherwise the note is sustained
      sustainNote();
    }
  }else{
  //Set to release mode if the release counter is above 60Hz and the average is 0 (or less)
    if(releaseCounter>=60){
      releaseNote();
    }else{
      //Otherwise no tone is played and release and attack counters get set to 0
      noTone(outputPin);
      releaseCounter = 0;
      attackCounter  = 0;
      sustainTone    = 0;
    }
  }


}

void attackNote(){
  //Tone level set to current value of attackLevel
  toneLevel = attackCounter;

  //Output
  ///tone(outputPin,toneLevel);
  
  //creates a time based attackRate
  attackRate = average/attackTime;

  //Prevents a situation where the program flacuates between attack and sustain
  if(attackRate + attackCounter > average){
    attackCounter = average - attackRate;
  }
  
  //Increments the attackCounter by the timebased attackRate
  attackCounter += attackRate;
  releaseCounter = attackCounter;
  //Sets sustainTone to set release if a button is released
  sustainTone    = toneLevel;


  
      Serial.print("\nAttacking \n");
//      Serial.print(attackCounter);
//      Serial.print("\n");
//      Serial.print(average);     
}

//Keeps the current note at a level pitch
void sustainNote(){
  //Tone played is the average note
  toneLevel = average;
  //tone(outputPin,toneLevel); 

  //Sets attack, release and sustain tone to the played tone level
  releaseCounter = toneLevel;
  attackCounter  = toneLevel;   
  sustainTone    = toneLevel;
  Serial.print("Sustaining \n");

  if(modulateLevel > 0){
    modulating =true;
    modulateNote();
    modulating = false;
    Serial.print("\n modulateLevel>0 \n");
  }
  
}

void releaseNote(){
 //Prevents infinite loop when attack level doesn't reach >60
  if(toneLevel<=60){
    releaseCounter = 0;
  }
  
  //toneLevel set to current value of releaseCounter
  toneLevel = releaseCounter;

  //Output
  //tone(outputPin,toneLevel);
  
  //Creates a release rate based on time
  releaseRate = sustainTone/releaseTime;

  //Decrement releaseCounter by releaseRate
  releaseCounter -= releaseRate;
  //Sets the attack counter to current toneLevel in case a higher note is requested
  attackCounter = toneLevel;

  Serial.print("Releasing \n");
 
}

void modulateNote(){
  if(modulating == true){
    Serial.print("\n Modulating \n");
    toneLevel = toneLevel + (modulateLevel*(sineTable[modulateCount]/100));
    modulateCount += modulateCount*modulateLevel;
    if(modulateCount >= 255){
      modulateCount = 0;
    }
  }
  Serial.print("\n Modulating = ");
  Serial.print(modulating);
  Serial.print("\n");
}

//Plays the tone with the value of toneLevel
void playTone(){
  //if there's no note meant to be played, don't play it, otherwise play it.
  if(average<=0 && releaseCounter < 60){
    noTone(outputPin);
  }else{
    tone(outputPin,toneLevel);
  }
}


