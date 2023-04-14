#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

int pot1_val = 0;
int pot2_val = 0;
int pot3_val = 0;

#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=323,196
AudioEffectBitcrusher    bitcrusher1;    //xy=522,175
AudioMixer4              mixer1;         //xy=738,220
AudioOutputUSB           usb1;           //xy=1004,211
AudioConnection          patchCord1(i2s2, 0, bitcrusher1, 0);
AudioConnection          patchCord2(i2s2, 1, mixer1, 1);
AudioConnection          patchCord3(bitcrusher1, 0, mixer1, 0);
AudioConnection          patchCord4(mixer1, 0, usb1, 0);
AudioConnection          patchCord5(mixer1, 0, usb1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=64.5,366
// GUItool: end automatically generated code


void setup() {
  // put your setup code here, to run once:
  pinMode(32, OUTPUT); //LED
  pinMode(25,INPUT_PULLUP); //footswtich

  
  Serial.begin(9600); 
  AudioMemory(40); 
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5); 
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); 
  sgtl5000_1.lineInLevel(15);

  mixer1.gain(0, 0);
  mixer1.gain(1, 0);
  
  bitcrusher1.bits(16);
  bitcrusher1.sampleRate(44100);

}

void loop() {
  // put your main code here, to run repeatedly:
  pot1_val = analogRead(41);
  pot2_val = analogRead(40);
  pot3_val = analogRead(39);

  int pot1_val_scaled = map(pot1_val, 0, 1023, 0, 16);
  float pot2_val_scaled = map(pot2_val, 0, 1023, 0, 10);
  int pot3_val_scaled = map(pot3_val, 0, 1023, 0, 15);

  bitcrusher1.bits(16-pot1_val_scaled);
  mixer1.gain(0, (pot2_val_scaled/10));
  
  if (digitalRead(25)){
    digitalWrite(32,LOW);
  } else {
    digitalWrite(32,HIGH);
  }

}
