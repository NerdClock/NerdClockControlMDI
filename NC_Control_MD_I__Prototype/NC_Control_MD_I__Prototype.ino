// Firmware NCControl MD I 
// Version: 0.10
// Copyright: Martin Duerrenberger 2017
// also on github

// Number of Components
int componentCount=6;
int dialPotiNr=6;
boolean dialPotiEnabled=true;

// Selection numbers for Sets
int ccSetCurrent=1;
int ccSetDefaultA=1;
int ccSetDefaultB=2;
int ccSetDefaultC=3;
int ccSetDefaultD=4;
int ccSetStandard=5;

// Poti values. Teensy can use up to 21 analog ins. 10 are connected in this setup.
int potiPin[]={A0,A1,A2,A3,A4,A5,A6,A7,A8,A9};
int potiValue[10];

// Control Change Values
// found in: http://nickfever.com/music/midi-cc-list

// Default Controls
// SET A
int ccDefault14=14; //undefined
int ccDefault15=15; //undefined
int ccDefault16=16; //undefined
int ccDefault17=17; //undefined
int ccDefault18=18; //undefined
int ccDefault19=19; //undefined

// SET B
int ccDefault20=20; //undefined
int ccDefault21=21; //undefined
int ccDefault22=22; //undefined
int ccDefault23=23; //undefined
int ccDefault24=24; //undefined
int ccDefault25=25; //undefined

// SET C
int ccDefault26=26; //undefined
int ccDefault27=27; //undefined
int ccDefault28=28; //undefined
int ccDefault29=29; //undefined
int ccDefault30=30; //undefined
int ccDefault31=31; //undefined

// SET D
int ccDefault85=85; //undefined
int ccDefault86=86; //undefined
int ccDefault87=87; //undefined
int ccDefault88=88; //undefined
int ccDefault89=89; //undefined
int ccDefault90=90; //undefined

// Volume
int ccVolume=7;
int ccBalance=8;
int ccPan=10;
int ccExpression=11; //Percentage of CC7 Volume

// Portamento Controls
int ccPortamentoOnOff=65; // Portamento on/off 0 to 63 = Off, 64 to 127 = On
int ccPortamentoTime=5;
int ccPortamentoAmount=84;

// VCA Controls
int ccResonance=71; //VCA Resonance
int ccRelease=72; //ADSR - R
int ccAttack=73; //ADSR - A
int ccCutoff=74; //VCA Cutoff


// Controler Init Sets
int controlChangeDefaultA[]={ccDefault14,ccDefault15,ccVolume,ccDefault16,ccDefault17,ccDefault19,ccDefault19,ccDefault19,ccDefault19,ccDefault19};
int controlChangeDefaultB[]={ccDefault20,ccDefault21,ccVolume,ccDefault22,ccDefault23,ccDefault25,ccDefault19,ccDefault19,ccDefault19,ccDefault19};
int controlChangeDefaultC[]={ccDefault26,ccDefault27,ccVolume,ccDefault28,ccDefault29,ccDefault31,ccDefault19,ccDefault19,ccDefault19,ccDefault19};
int controlChangeDefaultD[]={ccDefault85,ccDefault86,ccVolume,ccDefault87,ccDefault88,ccDefault90,ccDefault19,ccDefault19,ccDefault19,ccDefault19};
int controlChangeStandard[]={ccCutoff,ccResonance,ccVolume,ccAttack,ccRelease,ccDefault25,ccDefault19,ccDefault19,ccDefault19,ccDefault19};

int controlChangeValue[10];
int controlChangeValuePrev[10];

// MIDI
int midiBaudRate=31250;
//int serialBaudRate=9600;
int midiChannel=1;

// Poti 6 split points. This poti is used to select the cc set.
// NCControl MD I Default
int defaultSetA=32; // Everything < defaultSet sets the default set.
int defaultSetB=64;
int defaultSetC=96;
int defaultSetD=125;
int midiStandardSet=127; //Everything > defaultSet and < midiStandardSet sets the StandardSet. // more sets can be added

//Overrides
int baudRate=midiBaudRate;

void setup() {
  Serial.begin(baudRate);
  pinMode(12,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(8,OUTPUT);
  blinkLed(500,12);
  blinkLed(500,11);
  blinkLed(500,10);
  blinkLed(500,9);
  blinkLed(500,8);
  blinkLed(500,8);
  blinkLed(500,8);
}

void loop() { 
  for(int i = 0; i < componentCount; i++){
    potiValue[i] = analogRead(potiPin[i]);
    controlChangeValue[i] = map(potiValue[i],0,1023,0,127);
    int controlValueTest=(controlChangeValue[i]+controlChangeValuePrev[i])/2; //Smooth out the poti values
    
    // Has poti Value changed?
    if(controlChangeValue[i] != controlValueTest){
          // Is dial Poti enabled?
          if (dialPotiEnabled){
            selectDialRange(i);
          }
      
          // Send Midi CC
          sendMidiCC(i);  
      
          //Remember old Value
          controlChangeValuePrev[i] = controlChangeValue[i];
    }
  }
}





// Send midi cc function
void sendMidiCC(int i){
      // No control change for dialPoti
      if(i != (dialPotiNr-1) || dialPotiEnabled==false){
        if (ccSetCurrent==ccSetDefaultA)
        {
          usbMIDI.sendControlChange(controlChangeDefaultA[i], controlChangeValue[i], midiChannel);
        }
        if (ccSetCurrent==ccSetDefaultB)
        {
          usbMIDI.sendControlChange(controlChangeDefaultB[i], controlChangeValue[i], midiChannel);
        }
        if (ccSetCurrent==ccSetDefaultC)
        {
          usbMIDI.sendControlChange(controlChangeDefaultC[i], controlChangeValue[i], midiChannel);
        }
        if (ccSetCurrent==ccSetDefaultD)
        {
          usbMIDI.sendControlChange(controlChangeDefaultD[i], controlChangeValue[i], midiChannel);
        }
        if (ccSetCurrent==ccSetStandard)
        {
          usbMIDI.sendControlChange(controlChangeStandard[i], controlChangeValue[i], midiChannel);
        }
        blinkLed(10,12);
      }
}

// Selecting the dial Range (5 possible sectors)
void selectDialRange(int i){
      //Decide which set is selected. Only if defined Poti was used
      if(i == (dialPotiNr-1))
      {
        if(controlChangeValue[i] >= 0 && controlChangeValue[i] < defaultSetA)
        {
          //Set Default A
          if(ccSetCurrent!=ccSetDefaultA){
            ccSetCurrent=ccSetDefaultA;
            blinkLed(100,8);
          }
        }
        if(controlChangeValue[i] >= defaultSetA && controlChangeValue[i] < defaultSetB)
        {
          //Set Default B
          if(ccSetCurrent!=ccSetDefaultB){
            ccSetCurrent=ccSetDefaultB;
            blinkLed(100,8);
          }
        } 
        if(controlChangeValue[i] >= defaultSetB && controlChangeValue[i] < defaultSetC)
        {
          //Set Default C
          if(ccSetCurrent!=ccSetDefaultC){
            ccSetCurrent=ccSetDefaultC;
            blinkLed(100,8);
          }
        } 
        if(controlChangeValue[i] >= defaultSetC && controlChangeValue[i] < defaultSetD)
        {
          //Set Default C
          if(ccSetCurrent!=ccSetDefaultD){
            ccSetCurrent=ccSetDefaultD;
            blinkLed(100,8);
          }
        } 
        if(controlChangeValue[i] >= defaultSetD && controlChangeValue[i] <= midiStandardSet)
        {
          //Set Standard
          if(ccSetCurrent!=ccSetStandard){
            ccSetCurrent=ccSetStandard;
            blinkLed(100,8);
          }
        }
      }
}

void blinkLed(int timer, int pinNr){
      // turn the pin on:
    digitalWrite(pinNr, HIGH);
    delay(timer/2);
    // turn the pin off:
    digitalWrite(pinNr, LOW);
    delay(timer/2);
}
