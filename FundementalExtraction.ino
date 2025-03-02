#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <stdlib.h>

elapsedMillis miliTimer;
elapsedMillis sequencerTimer;


// GUItool: begin automatically generated code
AudioInputI2S i2s1;                  //xy=152,281
AudioAmplifier amp1;                 //xy=292,376
AudioMixer4 mixer1;                  //xy=448,386
AudioOutputI2S i2s2;                 //xy=687,285
AudioAnalyzeNoteFrequency notefreq;  //xy=699,390
AudioConnection patchCord1(i2s1, 0, amp1, 0);
AudioConnection patchCord2(i2s1, 0, i2s2, 0);
AudioConnection patchCord3(amp1, 0, mixer1, 1);
AudioConnection patchCord4(mixer1, notefreq);

// GUItool: end automatically generated code

AudioControlSGTL5000 sgtl5000_1;
//-------------------------------------------------------------------
// clear memory for midi values
int midiNote = 0;

int midiArray[5];
int midiSortArray[5];
int appendIndex = 0;

// ------------------------------------------------------------------
// PINS FOR RELAIS

bool semitone = false;

int relaisPins[6] = { 13, 14, 15, 16 };
// ------------------------------------------------------------------
// FUNCTION DEFINITIONS

bool checkNotes(int midiArray[5]);
int appendNote(int prevMidi);
bool detectSemitones();


void setup() {
  Serial.begin(9600);
  Serial.println("booting up");
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  AudioMemory(30);

  // array to store 5 midi values for

  notefreq.begin(.15);
  // ---------------------------------------------------------------
  // PINMODES
  pinMode(LED_BUILTIN, OUTPUT);
  // PINMODES FOR RELAIS
  for (int i = 0; i < 4; i++) {
    pinMode(relaisPins[i], OUTPUT);
  }

  // FILL THE NOTE ARRAY
  for (int i = 0; i < 5; i++) {
    midiArray[i] = i;
  }
  Serial.println("_______________________DONEBOOTING______________________");
}

void loop() {
  // Get frequency, Calculate midiNote, get Probability
  float freq = notefreq.read();
  int prevMidi = midiNote;
  midiNote = ((12 * log(freq / 220.0) / log(2.0)) + 57.01);
  float prob = notefreq.probability();

  //-----------------------------------------------------------------------------
  //  COMPARE NOTEHISTORY AND CALL CODE IF NOTE HAS BEEN CHANGED
  if (midiNote != prevMidi) {
    // Serial.println("youve changed..");
    miliTimer = 0;
    appendNote(prevMidi);

  } else if (midiNote == prevMidi) {

    Serial.print("timer:.. ");
    Serial.println(miliTimer);
    Serial.println(" ");
    Serial.printf("Note: %3.2f | Probability: %.2f ", freq, prob);
    Serial.print("midiNote: ");
    Serial.print(midiNote);
    Serial.print(" prevmidi: ");
    Serial.println(prevMidi);

    checkNotes(midiArray);  //check the midinote according to the notes before
  }

  // -----------------------------------------------------------------------------
  // RELAISCHECK
  Serial.print("semitoneLoop: ");
  Serial.println(semitone);
  sequencerTimer = sequencerTimer % 200;
  Serial.print("sequencertimer: ");
  Serial.println(sequencerTimer);

  // A TEST CHECK SWITCH PART OF CODE FOR CHECKING the relais
  int testCheck = random(500);
  // if (testCheck == 5) {
  //   semitone = true;
  // } else {
  //   semitone = false;
  // }

  //---------------------------------------------------------------------------
  //trigger random relais in sequence when there's no semitone in the phrase
  if (semitone == true) {
    for(int i = 0; i < 2; i++){
    int relaisRandomizer = random(0, 3);
    digitalWrite(relaisPins[relaisRandomizer], HIGH);
    delay(200);
    Serial.print("Relais: ");
    Serial.println(relaisPins[relaisRandomizer]);
    digitalWrite(relaisPins[relaisRandomizer], LOW);
    }
  }

  delay(1);
}

int comp(const void *a, const void *b) {  // i cannot figure out what this does
  return (*(int *)a - *(int *)b);
}

// checkNotes function: a huge mess
// this function checks the current midiArray, if the appendindex is 4 which is
// pretty often it sorts the array and calls the detectSemitones function
bool checkNotes(int midiArray[5]) {

  Serial.println();

  if (appendIndex == 4) {
    // print the midisortarray
    for (int i = 0; i < 5; i++) {
      midiSortArray[i] = midiArray[i];
      Serial.print(". ");

      // Sort the midiSortArray

      // Serial.println("i am calling you know from inside of the checkNotes function");
      qsort(midiSortArray, 5, sizeof(int), comp);  // sort the given array
      for (int i = 0; i < 5; i++) {
        printf("%d ", midiSortArray[i]);
      }
    }
    for (int i = 0; i < 5; i++) {
      if ((midiSortArray[i - 1] - midiSortArray[i]) == 1) {
        Serial.print("SEMITONE DETECTED");
        semitone = true;
        return semitone;
      } else {
        semitone = false;
      }
      for (int i = 0; i < 5; i++) {
        printf("%d ", midiSortArray[i]);
        Serial.print(". ");
      }
    }
    appendIndex = 0;
  }
  return semitone;  // TODO: this function can be void.
}

// name is selfexplanitory
bool detectSemitones() {
  // Detect semitones. compare the sorted array elements and find a midi distance of one.
  for (int i = 0; i < 5; i++) {
    if ((midiSortArray[i - 1] - midiSortArray[i]) == 1) {
      Serial.print("SEMITONE DETECTED");
      semitone = true;
      return semitone;
    } else {
      semitone = false;
    }
    for (int i = 0; i < 5; i++) {
      printf("%d ", midiSortArray[i]);
      Serial.print(". ");
    }
  }
}

// this function appends a "new" note to the midiArray
int appendNote(int prevMidi) {
  appendIndex = (appendIndex + 1) % 5;  // loop the appending to the array after 5 notes
                                        // Serial.print("appendIndex: ");
                                        // Serial.println(appendIndex);
  midiArray[appendIndex] = prevMidi;
  for (int i = 0; i < 5; i++) {
    Serial.print(midiArray[i]);
    Serial.print(", ");
  }
  Serial.println("done");
  return midiArray;
}
