#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SD.h>
#include <SPI.h>
#include <SerialFlash.h>
#include <ResponsiveAnalogRead.h>

AudioInputI2S            i2s2;
AudioEffectDelay         delay1;         //xy=196,238
AudioMixer4              mixer1;         //xy=388,203
AudioMixer4              mixer2;         //xy=389,283
AudioMixer4              mixer3;         //xy=529,114
AudioOutputUSB           usb1; 
AudioConnection          patchCord1(i2s2, 0, delay1, 0);
AudioConnection          patchCord2(i2s2, 0, mixer3, 0);
AudioConnection          patchCord3(delay1, 0, mixer1, 0);
AudioConnection          patchCord4(delay1, 1, mixer1, 1);
AudioConnection          patchCord5(delay1, 2, mixer1, 2);
AudioConnection          patchCord6(delay1, 3, mixer1, 3);
AudioConnection          patchCord7(delay1, 4, mixer2, 0);
AudioConnection          patchCord8(delay1, 5, mixer2, 1);
AudioConnection          patchCord9(delay1, 6, mixer2, 2);
AudioConnection          patchCord10(delay1, 7, mixer2, 3);
AudioConnection          patchCord11(mixer1, 0, mixer3, 1);
AudioConnection          patchCord12(mixer2, 0, mixer3, 2);
AudioConnection          patchCord13(mixer3, 0, usb1, 0);
AudioConnection          patchCord14(mixer3, 0, usb1, 1);
AudioControlSGTL5000     sgtl5000_1;

ResponsiveAnalogRead analogOne(41, true);
ResponsiveAnalogRead analogTwo(40, true);
ResponsiveAnalogRead analogThree(39, true);

void setup() {
  pinMode(32, OUTPUT);      //LED
  pinMode(25,INPUT_PULLUP); //footswitch

  Serial.begin(9600); 
  AudioMemory(160); 
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.7); 
  sgtl5000_1.inputSelect(AUDIO_INPUT_LINEIN); 
  sgtl5000_1.lineInLevel(15);

  mixer1.gain(0, 0.2);
  mixer1.gain(1, 0.2);
  mixer1.gain(2, 0.2);
  mixer1.gain(3, 0.2);

  mixer2.gain(0, 0.2);
  mixer2.gain(1, 0.2);
  mixer2.gain(2, 0.2);
  mixer2.gain(3, 0.2);

  mixer3.gain(0, 0.0); // default = do not listen to direct signal
  mixer3.gain(1, 0.0); // ch1 is output of mixer1
  mixer3.gain(2, 0.0); // ch2 is output of mixer2 

  delay1.delay(0, 0); //were 400
  delay1.delay(1, 0);
  delay1.delay(2, 0);
  delay1.delay(3, 0);
  delay1.delay(4, 0);
  delay1.delay(5, 0);
  delay1.delay(6, 0);
  delay1.delay(7, 0);
  delay(1000);
}

void loop() {
  analogOne.update();
  analogTwo.update();
  analogThree.update();

  int pot1_val_scaled = map(analogOne.getValue(), 0, 1023, 0, 400);
  float pot2_val_scaled = map(analogTwo.getValue(), 0, 1023, 0, 100);
  int pot3_val_scaled = map(analogThree.getValue(), 0, 1023, 0, 400);


  if (digitalRead(25)){
    digitalWrite(32,LOW);
  } else {
    digitalWrite(32,HIGH);
  }

  //dry AND delay volume are both one
  mixer3.gain(0, (pot2_val_scaled/100)); // default = do not listen to direct signal
  mixer3.gain(1, (pot2_val_scaled/100)); // ch1 is output of mixer1
  mixer3.gain(2, (pot2_val_scaled/100)); // ch2 is output of mixer2 

  //pot1 controls first set of delays
  delay1.delay(0, pot1_val_scaled);
  delay1.delay(1, pot1_val_scaled);
  delay1.delay(2, pot1_val_scaled);
  delay1.delay(3, pot1_val_scaled);

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
