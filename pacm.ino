#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>
//Adafruit_SSD1306 display(128, 64, 9, 10, 11, 13, 12); //SPI
Adafruit_SSD1306 display(128, 64, &Wire, -1);                                         //I2C

byte cursor = 0;
byte lives = 3;
byte pacman_x, pacman_y;
char pacman_direction = 'R';
char pacman_turns = 'R';
unsigned int tick = 0;
unsigned int score = 0;
unsigned int boost = 0;

byte ghosts_x[4] = {0, 0, 0, 0};
byte ghosts_y[4] = {0, 0, 0, 0};
char ghosts_direction[4] = {'D', 'D', 'U', 'U'};
byte ghosts_freeze[4] = {10, 10, 10, 10};


char field[6][16] = {
  {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'},
  {'W', 'G', ' ', ' ', ' ', 'W', ' ', 'W', ' ', 'W', 'o', 'o', 'o', 'o', 'G', 'W'},
  {'W', ' ', 'W', 'W', ' ', ' ', ' ', 'P', ' ', ' ', ' ', 'W', 'W', 'W', 'o', 'W'},
  {' ', ' ', ' ', ' ', ' ', 'W', ' ', 'W', ' ', 'W', ' ', ' ', ' ', ' ', 'o', 'o'},
  {'W', 'G', ' ', 'W', ' ', 'W', ' ', 'W', ' ', ' ', ' ', ' ', 'W', ' ', 'G', 'W'},
  {'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W', 'W'}
};

static const unsigned char PROGMEM ghost[] =
{
  0b00011000,
  0b00111100,
  0b01011010,
  0b01011010,
  0b11011011,
  0b11111111,
  0b11111111,
  0b01011010
};


static const unsigned char PROGMEM oreo[] =
{
  0b01111110,
  0b11111111,
  0b11010101,
  0b10101011,
  0b11010101,
  0b10101011,
  0b11111111,
  0b01111110
};
static const unsigned char PROGMEM ghost_busted[] =
{
  0b00011000,
  0b00111100,
  0b01011010,
  0b01111110,
  0b11000011,
  0b10111101,
  0b11111111,
  0b01011010
};

static const unsigned char PROGMEM logo[] =
{ 0b00001, 0b10000,
  0b00011, 0b11000,
  0b00111, 0b11100,
  0b01111, 0b11110,
  0b11111, 0b11111,
  0b01111, 0b11110,
  0b00111, 0b11100,
  0b00011, 0b11000,
  0b00001, 0b10000,
  0b00011, 0b11000,
  0b00111, 0b11100,
  0b01111, 0b11110,
  0b11111, 0b11111,
  0b01111, 0b11110,
  0b00111, 0b11100,
  0b00011, 0b11000 }; 

void setup() { 
  //.begin(SSD1306_SWITCHCAPVCC);        // SPI
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // I2C
  display.setRotation(2);
  
  pinMode(7, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(4, OUTPUT);
  
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  digitalWrite(5, HIGH);
  
  digitalWrite(4, HIGH);
  menu();
  randomSeed(analogRead(A4));
  byte ghosts_number = 0;
  for (byte y = 0; y < 6; y ++)
    for (byte x = 0; x < 16; x ++)
      if (field[y][x] == 'P') {
        pacman_x = x * 8 + 4;
        pacman_y = y * 8 + 4;
      }
      else if(field[y][x] == 'G'){
        ghosts_x[ghosts_number] = x * 8 + 4;
        ghosts_y[ghosts_number] = y * 8 + 4;
        ghosts_number++;
      }
      else if(field[y][x] == ' '){
        field[y][x] = '.';
      }
}

void loop() {
  switch(pacman_direction){
    case 'R':
      if(round(floor((pacman_x+4)/8)) > 15) pacman_x += 2;
      else if(field[round(floor((pacman_y)/8))][round(floor((pacman_x+4)/8))] != 'W') pacman_x += 2;
      if(pacman_x > 128) pacman_x = 0; 
      break;
    case 'L':
      if(round(floor((pacman_x-6)/8)) < 0) pacman_x -= 2;
     else if(field[round(floor((pacman_y)/8))][round(floor((pacman_x-6)/8))] != 'W') pacman_x -= 2;
     if(pacman_x > 128) pacman_x = 128;
      break;
    case 'U':
      if(field[round(floor((pacman_y-6)/8))][round(floor((pacman_x)/8))] != 'W') pacman_y -= 2;
      break;
    case 'D':
      if(field[round(floor((pacman_y+4)/8))][round(floor((pacman_x)/8))] != 'W') pacman_y += 2;
      break;
  }
  if(tick % 2 == 0)
  for(byte i = 0; i < 4; i++){
   if(ghosts_freeze[i] == 0){
    switch(ghosts_direction[i]){
      case 'R':
        if(field[round(floor((ghosts_y[i])/8))][round(floor((ghosts_x[i]+4)/8))] != 'W') ghosts_x[i] += 2;
        if(ghosts_x[i] > 128) ghosts_x[i] = 0;
        break;  
      case 'L':
        if(field[round(floor((ghosts_y[i])/8))][round(floor((ghosts_x[i]-6)/8))] != 'W') ghosts_x[i] -= 2;
        if(ghosts_x[i] > 128) ghosts_x[i] = 128;
        break;
      case 'U':
        if(field[round(floor((ghosts_y[i]-6)/8))][round(floor((ghosts_x[i])/8))] != 'W') ghosts_y[i] -= 2;
        break;
      case 'D':
        if(field[round(floor((ghosts_y[i]+4)/8))][round(floor((ghosts_x[i])/8))] != 'W') ghosts_y[i] += 2;
        break;
    }
  }
}
  if(field[round(floor((pacman_y)/8))][round(floor((pacman_x)/8))] == '.') {
    field[round(floor((pacman_y)/8))][round(floor((pacman_x)/8))] = ' ';
    score += 10;
  }
  if(field[round(floor((pacman_y)/8))][round(floor((pacman_x)/8))] == 'o'){
     field[round(floor((pacman_y)/8))][round(floor((pacman_x)/8))] = '.';
     boost = 150;
  }

  if (input_up())     pacman_turns = 'U';
  if (input_down())   pacman_turns = 'D';
  if (input_right())  pacman_turns = 'R';
  if (input_left())   pacman_turns = 'L';

  if(pacman_turns == 'U' && pacman_x % 8 == 4 && field[round(floor((pacman_y)/8))-1][round(floor((pacman_x)/8))] != 'W')
    pacman_direction = 'U';
  if(pacman_turns == 'D' && pacman_x % 8 == 4 && field[round(floor((pacman_y)/8))+1][round(floor((pacman_x)/8))] != 'W')
    pacman_direction = 'D';
  if(pacman_turns == 'L' && pacman_y % 8 == 4 && field[round(floor((pacman_y)/8))][round(floor((pacman_x)/8))-1] != 'W')
    pacman_direction = 'L';
  if(pacman_turns == 'R' && pacman_y % 8 == 4 && field[round(floor((pacman_y)/8))][round(floor((pacman_x)/8))+1] != 'W')
    pacman_direction = 'R';

  // ----------------------------УПРАВЛЕНИЕ ПРИЗРАКАМИ-----------------------------------
  for(byte i = 0; i < 4; i++){
    if(ghosts_x[i] % 8 == 4 && ghosts_y[i] % 8 == 4){
      byte ways = 0;
      byte ways_directions[3] = {' ', ' ', ' '};
      if(field[round(floor((ghosts_y[i])/8))-1][round(floor((ghosts_x[i])/8))] != 'W' && ghosts_direction[i] != 'D'){
        ways_directions[ways] = 'U';
        ways++;
      }
      if(field[round(floor((ghosts_y[i])/8))+1][round(floor((ghosts_x[i])/8))] != 'W' && ghosts_direction[i] != 'U'){
        ways_directions[ways] = 'D';
        ways++;
      }
      if(field[round(floor((ghosts_y[i])/8))][round(floor((ghosts_x[i])/8))-1] != 'W' && ghosts_direction[i] != 'R'){
        ways_directions[ways] = 'L';
        ways++;
      }
      if(field[round(floor((ghosts_y[i]-16)/8))][round(floor((ghosts_x[i])/8))+1] != 'W' && ghosts_direction[i] != 'L'){
        ways_directions[ways] = 'R';
        ways++;
      }

      if (ways > 0) {
        ghosts_direction[i] = ways_directions[random(0, ways)];
      } else {
        switch(ghosts_direction[i]){
          case 'U':
            ghosts_direction[i] = 'D';
            break;
          case 'D':
            ghosts_direction[i] = 'U';
            break;
          case 'L':
            ghosts_direction[i] = 'R';
            break;
          case 'R':
            ghosts_direction[i] = 'L';
            break;
        }
      }
    }
  }
  for (byte i = 0; i < 4; i++){
    if(abs(ghosts_x[i]-pacman_x) < 7 && abs(ghosts_y[i]-pacman_y) <7 && boost == 0) {
    if(lives > 0){
       lives --;
      restart();
    } else game_over_screen();
    }else if(abs(ghosts_x[i]-pacman_x) <7 && abs(ghosts_y[i]-pacman_y) < 7 && boost > 0 && ghosts_freeze[i] == 0){
      byte i_g = 0;
      for(byte y = 0; y < 6; y++)
       for(byte x = 0; x < 16; x++)
        if(field[y][x] == 'G'){
         if(i == i_g){
          ghosts_x[i] = x * 8 + 4;
          ghosts_y[i] = y * 8 + 4;
          ghosts_freeze[i] = 30;
          score += 10000;
         }
         i_g ++;
        }
    }
  }
  if(victory()) victory_screen();
  render();
  tick ++;
  if(boost > 0) boost--;
  for(byte i = 0; i < 4; i++){
    if(ghosts_freeze[i] > 0) ghosts_freeze[i]--;
  }
  
}

void restart(){
  byte ghosts_number = 0;
  for(byte y = 0; y < 6; y ++)
   for(byte x = 0; x < 16; x ++){
    if(field[y][x] == 'G'){
     pacman_x = x * 8 + 4;
     pacman_y = y * 8 + 4;
    }
    else if(field[y][x] == 'G'){
      ghosts_x[ghosts_number] = x * 8 + 4;
      ghosts_y[ghosts_number] = y * 8 + 4;
      ghosts_number++;
    }
  }
}

void render(){
  display.clearDisplay();

  display.setTextSize(1); // Draw 2X-scale text
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 56);
  display.print(score);
  display.print(lives);
  byte seconds = round(floor(millis() / 1000));
  byte minutes = round(floor(seconds) / 60);

  display.setCursor(70, 56);
  display.print(minutes);
  display.print(":");
  display.print(seconds);
  display.print(".");
  display.print(millis()%1000);
  for (byte y = 0; y < 6; y ++) {
    for (byte x = 0; x < 16; x ++){
      if (field[y][x] == 'W') draw_wall(x, y);
      if (field[y][x] == '.') display.fillCircle(x*8+4, y*8+4, 1, SSD1306_WHITE);
      if (field[y][x] == 'o') display.drawBitmap(x*8, y*8, oreo, 8, 8, SSD1306_WHITE);
    }
  }
 
  for(byte i = 0; i < 4; i++){
    if(boost > 0)display.drawBitmap(ghosts_x[i]-4, ghosts_y[i]-4, ghost_busted, 8, 8, SSD1306_WHITE);
    display.drawBitmap(ghosts_x[i]-4, ghosts_y[i]-4, ghost, 8, 8, SSD1306_WHITE);
  }

  draw_pacman(pacman_x, pacman_y, pacman_direction, round(floor(tick/4)) % 2);
  display.display();
}

void draw_pacman(byte x, byte y, char direction, byte frame){
  if(frame == 0) {
    display.fillCircle(x, y, 3, SSD1306_WHITE);
  } else {
    display.fillCircle(x, y, 3, SSD1306_WHITE);
    switch (direction){
      case 'U':
        display.fillTriangle(x, y, x-2, y-3, x+2, y-3, SSD1306_BLACK);
        break;
      case 'D':
        display.fillTriangle(x, y, x-2, y+3, x+2, y+3, SSD1306_BLACK);
        break;
      case 'R':
        display.fillTriangle(x, y, x+3, y-2, x+3, y+2, SSD1306_BLACK);
        break;
      case 'L':
        display.fillTriangle(x, y, x-3, y-2, x-3, y+2, SSD1306_BLACK);
        break;
    }
  }
}

void draw_wall(byte x, byte y){
  display.fillRect(x * 8 + 3, y * 8 + 3, 2, 2, SSD1306_WHITE);
  if(y > 0 && field[y-1][x] == 'W')  display.fillRect(x * 8 + 3, y * 8 , 2, 3, SSD1306_WHITE);
  if(y < 5 && field[y+1][x] == 'W')  display.fillRect(x * 8 + 3, y * 8 + 5, 2, 3, SSD1306_WHITE);
  if(x > 0 && field[y][x-1] == 'W')  display.fillRect(x * 8, y * 8 + 3, 3, 2, SSD1306_WHITE);
  if(x < 15 && field[y][x+1] == 'W') display.fillRect(x * 8 + 5, y * 8 + 3, 3, 2, SSD1306_WHITE);
}

void victory_screen() {
  unsigned long time = millis();

  byte seconds = round(floor(millis() / 1000));
  byte minutes = round(floor(seconds) / 60);

  display.setCursor(90, 0);
  display.print(minutes);
  display.print(":");
  display.print(seconds);
  display.print(".");
  display.print(millis()%1000);

  for(byte i = 0; i < lives; i++){
    draw_pacman(120 + i*10, 56,'R', 1);
  }
  
  
  unsigned long best_time;
  EEPROM.get(0, best_time);
  if(best_time > time) EEPROM.put(0, best_time);
  
  display.fillRect(0, 0, 128, 64, SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextColor(SSD1306_BLACK);
  display.setCursor(22, 22);
  display.print("VICTORY");
  display.setTextSize(1);
  
  if(best_time > time){
    display.setCursor(22, 40);
    display.print("BEST TIME: ");

    seconds = round(floor(millis() / 1000));
    minutes = round(floor(seconds) / 60);

    display.setCursor(70, 0);
    display.print(minutes);
    display.print(":");
    display.print(seconds);
    display.print(".");
    display.print(millis()%1000);
  }
    else{
      display.setCursor(22, 40);
      display.print("NEW RECORD!!!");
    }
  
  
  while(true){
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(24, 55);
    display.print("Press RESTART");
    display.display();
    delay(500);

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(24, 48);
    display.print("Press RESTART");
    display.display();
    delay(500);
  }
}

void game_over_screen() {
  display.fillRect(0, 0, 128, 64, SSD1306_BLACK);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(12, 22);
  display.print("GAME OVER");
  display.setTextSize(1);
  
  while(true){
    display.setTextColor(SSD1306_BLACK);
    display.setCursor(24, 48);
    display.print("Press RESTART");
    display.display();
    delay(500);

    display.setTextColor(SSD1306_WHITE);
    display.setCursor(24, 48);
    display.print("Press RESTART");
    display.display();
    delay(500);
  }
}

void menu(){
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.drawBitmap(56, 16, logo, 16, 16, SSD1306_WHITE);
  if(cursor == 0){
    display.setCursor(0, 48);
    display.setTextSize(2);
  } else{
    display.setCursor(16, 52);
    display.setTextSize(1);
  }
  display.print("NEW HELL");

  display.display();
  
  if(cursor == 1){
    display.setCursor(64, 48);
    display.setTextSize(2);
  } else{
    display.setCursor(80, 52);
    display.setTextSize(1);
  }
  display.print("CREDITS");

  display.display();

  if(input_right() || input_left()) {
    cursor = 1 - cursor;
    delay(200);
  }
  if((input_up() || input_down()) && cursor == 0){
    delay(1000);
    return;
  }
  if((input_up() || input_down()) && cursor == 1){
    delay(1000);
    credits();
  }
  menu(); 
}

void credits(){
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(3, 10);
  display.println("credits:");
  display.println("oversouth,coldy,52SG");
  display.println("teapt,arsenal");
  display.println("Mihail");
  display.display();

  if(input_up() || input_down()){
    while(input_up() || input_down());
    return;
  }

  credits();
}

bool victory(){
  for(byte y = 0; y < 6; y++)
    for(byte x = 0; x < 16; x++)
      if(field[y][x] == '.') return false;
  return true;
}

bool input_up() {
  return digitalRead(7);        // ЕСЛИ КНОПКИ
  //return analogRead(A0) > 700;  // ЕСЛИ ДЖОЙСТИК
}

bool input_down() {
  return digitalRead(6);        // ЕСЛИ КНОПКИ
  //return analogRead(A0) < 300;  // ЕСЛИ ДЖОЙСТИК
}

bool input_left() {
  return digitalRead(5);        // ЕСЛИ КНОПКИ
  //return analogRead(A1) < 300;  // ЕСЛИ ДЖОЙСТИК  
}

bool input_right() {
  return digitalRead(4);        // ЕСЛИ КНОПКИ
  //return analogRead(A1) > 700;  // ЕСЛИ ДЖОЙСТИК
}
