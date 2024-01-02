/*
Replacement Door Lock Combo Keypad Controller by James Carl (@james-carl-git)
*/

#include "MspFlash.h"

#define flash SEGMENT_D

unsigned char combo[] = {0, 0, 0, 0};
unsigned char current_combo[] = {10, 10, 10, 10}; //unprogrammed default values, no combo will work
byte combo_size = 4;      //length of combo that is programmed, max 16
byte button_presses = 0;
int timeout = 0;
bool awake = false;
bool programing_mode = false;
byte correct_combos = 0;
byte lock_pin = 11;
byte unlock_pin = 12;
byte buzzer_pin = 13;
byte program_pin = 5;
byte pin_1_L = 6;
byte pin_1_3_7 = 7;
byte pin_3_5 = 8;
byte pin_5_9_L = 9;
byte pin_7_9 = 10;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize pins
  pinMode(lock_pin, OUTPUT);              //lock
  pinMode(unlock_pin, OUTPUT);            //unlock
  pinMode(buzzer_pin, OUTPUT);            //buzzer
  pinMode(program_pin, INPUT_PULLDOWN);   //programming button
  pinMode(pin_1_L, INPUT_PULLDOWN);       // 1/2 - L
  pinMode(pin_1_3_7, OUTPUT);             // 1/2 - 3/4 - 7/8
  pinMode(pin_3_5, INPUT_PULLDOWN);       // 3/4 - 5/6
  pinMode(pin_5_9_L, OUTPUT);             // 5/6 - 9/0 - L
  pinMode(pin_7_9, INPUT_PULLDOWN);       // 7/8 - 9/0
  digitalWrite(pin_1_3_7, HIGH);          // need an initial state so you can detect when a button is pressed
  digitalWrite(pin_5_9_L, HIGH);
  digitalWrite(lock_pin, LOW);
  digitalWrite(unlock_pin, LOW);
  digitalWrite(buzzer_pin, LOW);
  int j = 0;
  do{                                     //read combo from flash memory and use as correct combo
    Flash.read(flash+j, &combo[j], 1);
  } while (j++ < combo_size);
}

void reset_outputs()
{
  digitalWrite(pin_1_3_7, HIGH);
  digitalWrite(pin_5_9_L, HIGH);
}

int readbutton(int button){               // determines what button is pressed and returns: 1/2=1, 3/4=3, 5/6=5, 7/8=7, 9/0=9, L=0, void=10
  if(button == 0){                        // either 1/2 or L was pressed
    digitalWrite(pin_1_3_7, LOW);    
    delay(5);
    if(digitalRead(pin_1_L)){             // L pressed
      digitalWrite(pin_1_3_7, HIGH);      // need to change state of multiplexed keypad to confirm which button was pressed
      digitalWrite(pin_5_9_L, LOW);
      if(digitalRead(pin_1_L)){           // unexpected, return void
        reset_outputs();
        return 10;
      }else{                              // Confirmed L pressed
        reset_outputs();
        return 0;
      }
    }else{                                // 1/2 pressed
      digitalWrite(pin_1_3_7, HIGH);
      digitalWrite(pin_5_9_L, LOW);
      delay(5);
      if(digitalRead(pin_1_L)){           // Confirmed 1/2 pressed
        reset_outputs();
        return 1;
      }else{                              // unexpected, return void
        reset_outputs();
        return 10;
      }
    }
  }else if(button == 1){                  // either 3/4 or 5/6 was pressed
    digitalWrite(pin_1_3_7, LOW);         // need to change state of multiplexed keypad to confirm which button was pressed
    delay(5);
    if(digitalRead(pin_3_5)){             // 5/6 pressed
      digitalWrite(pin_1_3_7, HIGH);
      digitalWrite(pin_5_9_L, LOW);
      delay(5);
      if(digitalRead(pin_3_5)){           // unexpected, return void
        reset_outputs();
        return 10;
      }else{                              // Confirmed 5/6 press
        reset_outputs();
        return 5;
      }
    }else{                                // 3/4 pressed
      digitalWrite(pin_1_3_7, HIGH);
      digitalWrite(pin_5_9_L, LOW);
      delay(5);
      if(digitalRead(pin_3_5)){           // Confirmed 3/4 pressed
        reset_outputs();
        return 3;
      }else{                              // unexpected, return void
        reset_outputs();
        return 10;
      }
    }
  }else{                                  //either 7/8 or 9/0 was pressed
    digitalWrite(pin_1_3_7, LOW);
    delay(5);
    if(digitalRead(pin_7_9)){             // 9/0 pressed
      digitalWrite(pin_1_3_7, HIGH);
      digitalWrite(pin_5_9_L, LOW);
      delay(5);
      if(digitalRead(pin_7_9)){           // unexpected, return void
        reset_outputs();
        return 10;
      }else{                              // Confirmed 9/0 press
        reset_outputs();
        return 9;
      }
    }else{                                // 7/8 pressed
      digitalWrite(pin_1_3_7, HIGH);
      digitalWrite(pin_5_9_L, LOW);
      delay(5);
      if(digitalRead(pin_7_9)){           // Confirmed 7/8 pressed
        reset_outputs();
        return 7;
      }else{                              // unexpected, return void
        reset_outputs();
        return 10;
      }
    }
  }
}

// the loop routine runs over and over again forever waiting for buttons to be pressed:
void loop() {
  int lock_or_not;
  if(digitalRead(pin_1_L)){
    awake = true;
    timeout = 0;
    lock_or_not = readbutton(0);            // check to see if either lock or 1/2 was pressed
    if(lock_or_not == 0){                   //Lock button pressed
      button_presses = 0;
      for(int k=0; k<combo_size; k++) current_combo[k] = 10;
      digitalWrite(lock_pin, HIGH);         //lock motor for 2 sec
      delay(1000);
      digitalWrite(lock_pin, LOW);
    }else{
      current_combo[button_presses] = lock_or_not;
      button_presses++;
      tone(buzzer_pin,1319);
      delay(5);
      while(digitalRead(pin_1_L))           //wait until button is released
        delay(5);
      noTone(buzzer_pin);
    }
  }else if(digitalRead(pin_3_5)){
    awake = true;
    timeout = 0;
    current_combo[button_presses] = readbutton(1);  // check to see if either 3/4 or 5/6 was pressed
    button_presses++;
    tone(buzzer_pin,1319);
    delay(5);
    while(digitalRead(pin_3_5))
      delay(5);
    noTone(buzzer_pin);
  }else if(digitalRead(pin_7_9)){
    awake = true;
    timeout = 0;
    current_combo[button_presses] = readbutton(2);  // check to see if either 7/8 or 9/0 was pressed
    button_presses++;
    tone(buzzer_pin,1319);
    delay(5);
    while(digitalRead(pin_7_9))
      delay(5);
    noTone(buzzer_pin);
  }else if(digitalRead(program_pin)){
    delay(5);
    if(digitalRead(program_pin)){
      awake = true;
      programing_mode = true;
      timeout = 0;
    }
  }
  if(awake){
    if(button_presses == combo_size){                           //have 'combo_size' presses been registered
      if(programing_mode){
        for(byte i=0; i<combo_size; i++)                        //change current combo being used in volitile memory
          combo[i] = current_combo[i];
        Flash.erase(flash);
        Flash.write(flash, current_combo, combo_size);          //write new combo to flash(non-volitile)
        delay(100);
        tone(buzzer_pin, 1568);                                 //play Tone acknowledging new combo
        delay(200);
        noTone(buzzer_pin);
        delay(200);
        tone(buzzer_pin, 1865);
        delay(200);
        noTone(buzzer_pin);
        delay(500);
      }else{
        correct_combos = 0;
        for(byte i=0; i<combo_size; i++){                     //check if entered combo matches store combo
          if(current_combo[i]==combo[i])
            correct_combos++;
        }
        if(correct_combos == combo_size){
          digitalWrite(unlock_pin, HIGH);                      //unlock motor 1 second
          delay(1000);
          digitalWrite(unlock_pin, LOW);
        }else{                                                //wrong pin, error Tone
          delay(100);
          tone(buzzer_pin, 587);
          delay(200);
          noTone(buzzer_pin);
          delay(200);
          tone(buzzer_pin, 587);
          delay(200);
          noTone(buzzer_pin);
          delay(500);
        }
      }
      correct_combos=0;
      button_presses=0;
      awake = false;
      timeout = 0; 
      programing_mode = false;
    }
    else if(timeout>1000){     //no user input, timed out (10 sec)
      correct_combos=0;
      button_presses=0;
      awake = false;
      timeout = 0;
      programing_mode = false;
    }
    else{                     //awake and waiting for another button press
      timeout++;
      delay(10);
    }  
  }
  else
    sleep(70);
}
