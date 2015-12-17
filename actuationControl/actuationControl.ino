#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

//pins above 13 are on the servo control board
#define slide_servo_pin 14
#define santa_servo_pin 15
#define santa_light_pin 16
#define slide_light_pin 4
#define top_red_pin 3
#define top_green_pin 5
#define top_blue_pin 6
#define bottom_red_pin 9
#define bottom_green_pin 10
#define bottom_blue_pin 11
#define xmas_light_pin 7
#define money_light_pin 17
#define card_1_servo_pin 18
#define card_2_servo_pin 19
#define acrylic_servo_pin 20
#define debug_light_pin 13
#define button_pin 8
#define random_seed_pin 0

#define change_time 1000

int output_pins[] = {slide_servo_pin, santa_servo_pin, santa_light_pin, 
                     slide_light_pin, top_red_pin, top_green_pin, top_blue_pin, 
                     bottom_red_pin, bottom_green_pin, bottom_blue_pin, 
                     xmas_light_pin, money_light_pin, card_1_servo_pin,
                     card_2_servo_pin, acrylic_servo_pin, debug_light_pin};
int num_output_pins = 13;

int input_pins[] = {button_pin};
int num_input_pins = 1;

struct color{
  int r;  //out of 4096;
  int g;  //out of 4096;
  int b;  //out of 4096;
};

color top_light_vals[] = {{0, 0, 0},
                        {1023, 1023, 1023},
                        {1023, 0, 0},
                        {0, 1023, 0},
                        {0, 0, 1023}};
int top_light_vals_len = 5;

color bottom_light_vals[] = {{0, 0, 0},
                             {2096, 2096, 2069}};
int bottom_light_vals_len = 2;

int slide_servo_still = 366; //out of 4096
int slide_servo_move = 377;
int slide_servo_time = 360; //ms

int slide_light_vals[] = {0, 1}; //binary
int slide_light_vals_len = 2;

int santa_servo_min = 150; //out of 4096
int santa_servo_max = 590; //out of 4096
int santa_servo_vals[] = {0, 120}; //degrees
int santa_servo_vals_len = 2;

int card_1_servo_min = 140;
int card_1_servo_max = 580;
int card_1_servo_vals[] = {0, 180};
int card_1_servo_vals_len = 2;

int card_2_servo_min = 150;
int card_2_servo_max = 590;
int card_2_servo_vals[] = {0, 180};
int card_2_servo_vals_len = 2;

int acrylic_servo_min = 150;
int acrylic_servo_max = 590;
int acrylic_servo_vals[] = {45, 135};
int acrylic_servo_vals_len = 2;

int santa_light_vals[] = {0, 4096, 1024, 2048}; //out of 4096
int santa_light_vals_len = 4;

int money_light_vals[] = {0, 4095};
int money_light_vals_len = 2;

int xmas_light_vals[] = {0, 1};
int xmas_light_vals_len = 2;

bool button_state = false;
bool button_state_old = false;

char key_vals[] = {'a', 'b', 'c', 'd'};
int key_vals_len = 4;

void setup() {  
  //set all needed pins
  for(int i = 0; i < num_output_pins; i++){
    if(output_pins[i] < 14){
      pinMode(output_pins[i], OUTPUT);
    }
  }
  for(int i = 0; i < num_input_pins; i++){
    pinMode(input_pins[i], INPUT);
  }
  randomSeed(analogRead(random_seed_pin));
  Serial.begin(9600);
  
  Keyboard.begin();
  
  Serial.println("setting up pwm and then starting");
  //set up pwm driver
  pwm.begin();
  pwm.setPWMFreq(60);
  yield();
}

void setVal(int pin, int value){
  if(pin > 13){
    pwm.setPWM((pin - 14), 0, value);  
  } else if (pin == 3 || pin == 5 || pin == 6 || pin == 9 ||
             pin == 10 || pin == 11){
    analogWrite(pin, value);
  } else {
    digitalWrite(pin, value);
  }
}

void changeDisplay(){
  digitalWrite(debug_light_pin, 1);
  int val_idx = 0;
  int val = 0;
  
  //slide servo
  setVal(slide_servo_pin, slide_servo_move);
  delay(slide_servo_time);
  setVal(slide_servo_pin, slide_servo_still);

  //keyboard
  val_idx = random(key_vals_len);
  char key_val = key_vals[val_idx];
  Keyboard.print(key_val);
  
  //santa servo
  val_idx = random(santa_servo_vals_len);
  val = map(santa_servo_vals[val_idx], 0, 180, santa_servo_min, santa_servo_max);
  setVal(santa_servo_pin, val);
  
  //santa light
  val_idx = random(santa_light_vals_len);
  val = santa_light_vals[val_idx];
  setVal(santa_light_pin, val);

  //slide light
  val_idx = random(slide_light_vals_len);
  val = slide_light_vals[val_idx];
  setVal(slide_light_pin, val);

  //xmas light
  val_idx = random(xmas_light_vals_len);
  val = xmas_light_vals[val_idx];
  setVal(xmas_light_pin, val);

  //top light
  val_idx = random(top_light_vals_len);
  color color_val = top_light_vals[val_idx];
  setVal(top_red_pin, color_val.r);
  setVal(top_green_pin, color_val.g);
  setVal(top_blue_pin, color_val.b);

  //money light
  val_idx = random(money_light_vals_len);
  val = money_light_vals[val_idx];
  Serial.println(val);
  setVal(money_light_pin, val);

  //card 1 servo
  val_idx = random(card_1_servo_vals_len);
  val = map(card_1_servo_vals[val_idx], 0, 180, 
            card_1_servo_min, card_1_servo_max);
  setVal(card_1_servo_pin, val);

  //card 2 servo
  val_idx = random(card_2_servo_vals_len);
  val = map(card_2_servo_vals[val_idx], 0, 180,
            card_2_servo_min, card_2_servo_max);
  setVal(card_2_servo_pin, val);

  //acrylic servo
  val_idx = random(acrylic_servo_vals_len);
  val = map(acrylic_servo_vals[val_idx], 0, 180,
            acrylic_servo_min, acrylic_servo_max);
  setVal(acrylic_servo_pin, val);
  
  delay(change_time);
  digitalWrite(debug_light_pin, 0);

}

void loop() {
  button_state = digitalRead(button_pin);
  if(button_state && !button_state_old){
    changeDisplay();
  }
  button_state_old = button_state;
}
