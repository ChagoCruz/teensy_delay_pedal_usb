#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include <ResponsiveAnalogRead.h>

// GUItool: begin automatically generated code
AudioInputI2S            i2s2;           //xy=323,196
AudioEffectFreeverb      freeverb1;      //xy=544,166
AudioMixer4              mixer1;         //xy=733,215
AudioOutputUSB           usb1;           //xy=1004,211
AudioConnection          patchCord1(i2s2, 0, freeverb1, 0);
AudioConnection          patchCord2(i2s2, 1, mixer1, 1);
AudioConnection          patchCord3(freeverb1, 0, mixer1, 0);
AudioConnection          patchCord4(mixer1, 0, usb1, 0);
AudioConnection          patchCord5(mixer1, 0, usb1, 1);
AudioControlSGTL5000     sgtl5000_1;     //xy=64.5,366
// GUItool: end automatically generated code

ResponsiveAnalogRead analogOne(41, true);
ResponsiveAnalogRead analogTwo(40, true);
ResponsiveAnalogRead analogThree(39, true);

void setup() {
  pinMode(32, OUTPUT); //LED
  pinMode(25, INPUT_PULLUP); //footswitch

  Serial.begin(9600);
  AudioMemory(40);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN);
  sgtl5000_1.lineInLevel(15);

  mixer1.gain(0,0); //mixes in wet
  mixer1.gain(1,0.5); //set dry to half volume always

  //roomsize 0-1
  //damping 0-1
  freeverb1.roomsize(0);
  freeverb1.damping(0);
}

void loop() {
  //reads the pots
  analogOne.update();
  analogTwo.update();
  analogThree.update();

  //scaling for the pots based on effects params
  float pot1_val_scaled = map(analogOne.getValue(), 0, 1023, 0, 100);
  float pot2_val_scaled = map(analogTwo.getValue(), 0, 1023, 0, 100);
  float pot3_val_scaled = map(analogThree.getValue(), 0, 1023, 0, 100);

  freeverb1.roomsize((pot1_val_scaled/100));
  freeverb1.damping((pot3_val_scaled/100));
  mixer1.gain(0, (pot2_val_scaled/100));

  if (digitalRead(25)){
    digitalWrite(32,LOW);
  } else {
    digitalWrite(32,HIGH);
  }

  Serial.print("Knob 1 is: ");
  Serial.print(pot1_val_scaled);
  Serial.print("\t");
  Serial.print("  Knob 2 is: ");
  Serial.print(pot2_val_scaled/100);
  Serial.print("\t");
  Serial.print("  Knob 3 is: ");
  Serial.println(pot3_val_scaled);

  delay(20);
}