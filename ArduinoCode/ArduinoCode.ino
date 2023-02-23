#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define buzzer 5

#define M1 4          //MOSFET1
#define M2 10         //MOSFET2
#define M3 7          //MOSFET3
#define M4 6          //MOSFET4
#define buttonPin1 3
#define buttonPin2 2

#define BAT1AI  0        //BAT1AI analog input
#define BAT2AI  1        //BAT2AI analog input
#define BAT3AI  2        //BAT3AI analog input
#define BAT4AI  3        //BAT4AI analog input 

#define voltage 3700

bool BAT1DC = false;
bool BAT2DC = false;
bool BAT3DC = false;
bool BAT4DC = false;

bool buzzing = false;
bool buzzing2 = false;

uint16_t struja = 0;
uint8_t screen = 1;

uint16_t bat1Voltage = 0;
uint16_t bat2Voltage = 0;
uint16_t bat3Voltage = 0;
uint16_t bat4Voltage = 0;

uint8_t brojacIspis = 1;
uint16_t brojacMjerenje = 1;

uint32_t loopInterval = 50;
uint32_t previousMillis = 0;
uint32_t currentMillis = 0;
uint8_t buzzerBrojac = 0;

bool mjerenje = true;

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define i2c_Address 0x3c //initialize with the I2C addr 0x3C Typically eBay OLED's
#define OLED_RESET -1   //   QT-PY / XIAO
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


void setup() {

  Serial.begin(9600);

  pinMode(buzzer, OUTPUT);
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
  pinMode(M3, OUTPUT);
  pinMode(M4, OUTPUT);

  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  digitalWrite(M1, BAT1DC);
  digitalWrite(M2, BAT2DC);
  digitalWrite(M3, BAT3DC);
  digitalWrite(M4, BAT4DC);

  attachInterrupt(digitalPinToInterrupt(buttonPin1), tipkalo1, FALLING);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), tipkalo2, FALLING);

  if (!display.begin(i2c_Address, true)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.cp437(true);
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.display();
}

void loop() {

  currentMillis = millis();

  if ((currentMillis - previousMillis) >= loopInterval) { // 50 ms

    brojacIspis++;
    brojacMjerenje++;
    buzzerBrojac++;

    if (buzzerBrojac >= 10 && buzzing) {
      if (buzzing2) tone(buzzer, 500);
      else noTone(buzzer);
      buzzing2 = !buzzing2;
      buzzerBrojac = 0;
    }

    if (brojacIspis >= 7) { //300 ms

      bat1Voltage = getVoltage(BAT1AI);
      bat2Voltage = getVoltage(BAT2AI);
      bat3Voltage = getVoltage(BAT3AI);
      bat4Voltage = getVoltage(BAT4AI);

      ispis(screen);
      brojacIspis = 1;
    }
    if (brojacMjerenje >= 1200) { //
      mjerenje = true;

      digitalWrite(M1, false);
      digitalWrite(M2, false);
      digitalWrite(M3, false);
      digitalWrite(M4, false);

      brojacMjerenje = 1;
    }
    if (mjerenje == true && brojacMjerenje > 200) {
      mjerenje = false;
      if (getVoltage(BAT1AI) > voltage) BAT1DC = true;
      else BAT1DC = false;
      if (getVoltage(BAT2AI) > voltage) BAT2DC = true;
      else BAT2DC = false;
      if (getVoltage(BAT3AI) > voltage) BAT3DC = true;
      else BAT3DC = false;
      if (getVoltage(BAT4AI) > voltage) BAT4DC = true;
      else BAT4DC = false;

      if ((!BAT1DC && (getVoltage(BAT1AI) < voltage) && (getVoltage(BAT1AI) > 0)) || (!BAT2DC && (getVoltage(BAT2AI) < voltage) && (getVoltage(BAT2AI) > 0)) || (!BAT3DC && (getVoltage(BAT3AI) < voltage) && (getVoltage(BAT3AI) > 0)) || (!BAT4DC && (getVoltage(BAT4AI) < voltage))  && (getVoltage(BAT4AI) > 0)) {
        buzzing = true;
      }

      digitalWrite(M1, BAT1DC);
      digitalWrite(M2, BAT2DC);
      digitalWrite(M3, BAT3DC);
      digitalWrite(M4, BAT4DC);
    }

    previousMillis = currentMillis;
  }

  //  tone(buzzer, 500); // Send 1KHz sound signal...
  //  delay(100);        // ...for 1 sec
  //  noTone(buzzer);     // Stop sound...
  //  delay(100);        // ...for 1sec


}

int getVoltage (int pin) {
  int napon = 0;
  for (int i = 0; i < 30; i++) {
    napon += analogRead(pin);
  }
  napon = napon / 30.0;
  napon = napon * 5086.0 / 1023.0; //5V regulator napon daje cca 5.086V
  return napon;
}

void tipkalo1() {

  screen++;

  if (screen == 3) screen = 1;
}

void tipkalo2() {

  BAT1DC = false;
  BAT2DC = false;
  BAT3DC = false;
  BAT4DC = false;

  digitalWrite(M1, BAT1DC);
  digitalWrite(M2, BAT2DC);
  digitalWrite(M3, BAT3DC);
  digitalWrite(M4, BAT4DC);

  mjerenje = true;
  brojacMjerenje = 0;

  buzzing = false;

}

void ispis(int screen) {

  display.clearDisplay();

  if (screen == 1) {
    screen1();
  }
  else if (screen == 2) {
    screen2();
  }

  display.display();
}

void screen1() {

  display.setCursor(35, 0);
  display.setTextSize(1);
  display.print("Napon[mV]:");

  //1
  display.drawRect(0, 28, 63, 8, SH110X_WHITE);

  //2
  display.drawRect(65, 28, 63, 8, SH110X_WHITE);

  //3
  display.drawRect(0, 56, 63, 8, SH110X_WHITE);

  //4
  display.drawRect(65, 56, 63, 8, SH110X_WHITE);

  display.setCursor(0, 11);
  display.setTextSize(1);
  if (BAT1DC) display.write(0x10);

  display.setCursor(0, 19);
  display.print("1.");
  display.setCursor(11, 12);
  display.setTextSize(2);
  if (bat1Voltage < 500) bat1Voltage = 0;
  display.print(bat1Voltage);
  float postotak = (49.17 * bat1Voltage / 1000.0) - 147.5;
  if (postotak > 59) postotak = 59;
  else if (postotak < 0) postotak = 0;
  display.fillRect(2, 30, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

  display.setCursor(65, 11);
  display.setTextSize(1);
  if (BAT2DC) display.write(0x10);

  display.setCursor(65, 19);
  display.print("2.");
  display.setCursor(76, 12);
  display.setTextSize(2);
  display.print(bat2Voltage);
  postotak = (49.17 * bat2Voltage / 1000.0) - 147.5;
  if (postotak > 59) postotak = 59;
  else if (postotak < 0) postotak = 0;
  display.fillRect(67, 30, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

  display.setCursor(0, 39);
  display.setTextSize(1);
  if (BAT3DC) display.write(0x10);

  display.setCursor(0, 47);
  display.print("3.");
  display.setCursor(11, 40);
  display.setTextSize(2);
  display.print(bat3Voltage);
  postotak = (49.17 * bat3Voltage / 1000.0) - 147.5;
  if (postotak > 59) postotak = 59;
  else if (postotak < 0) postotak = 0;
  display.fillRect(2, 58, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

  display.setCursor(65, 39);
  display.setTextSize(1);
  if (BAT4DC) display.write(0x10);

  display.setCursor(65, 47);
  display.print("4.");
  display.setCursor(76, 40);
  display.setTextSize(2);
  display.print(bat4Voltage);
  postotak = (49.17 * bat4Voltage / 1000.0) - 147.5;
  if (postotak > 59) postotak = 59;
  else if (postotak < 0) postotak = 0;
  display.fillRect(67, 58, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

}

void screen2() {

  display.setCursor(35, 0);
  display.setTextSize(1);
  display.print("Struja[mA]:");

  //1
  display.drawRect(0, 28, 63, 8, SH110X_WHITE);

  //2
  display.drawRect(65, 28, 63, 8, SH110X_WHITE);

  //3
  display.drawRect(0, 56, 63, 8, SH110X_WHITE);

  //4
  display.drawRect(65, 56, 63, 8, SH110X_WHITE);

  display.setCursor(0, 11);
  display.setTextSize(1);
  if (BAT1DC) display.write(0x10);

  display.setCursor(0, 19);
  display.print("1.");
  display.setCursor(11, 12);
  display.setTextSize(2);
  if (BAT1DC) struja = bat1Voltage / 3.3;
  else struja = 0;
  display.print(struja);
  //  float postotak = (49.17 * napon / 1000.0) - 147.5;
  //  if (postotak > 59) postotak = 59;
  //  else if (postotak < 0) postotak = 0;
  //  display.fillRect(2, 30, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

  display.setCursor(65, 11);
  display.setTextSize(1);
  if (BAT2DC) display.write(0x10);

  display.setCursor(65, 19);
  display.print("2.");
  display.setCursor(76, 12);
  display.setTextSize(2);
  if (BAT2DC) struja = bat2Voltage / 3.3;
  else struja = 0;
  display.print(struja);
  //  postotak = (49.17 * napon / 1000.0) - 147.5;
  //  if (postotak > 59) postotak = 59;
  //  else if (postotak < 0) postotak = 0;
  //  display.fillRect(67, 30, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

  display.setCursor(0, 39);
  display.setTextSize(1);
  if (BAT3DC) display.write(0x10);

  display.setCursor(0, 47);
  display.print("3.");
  display.setCursor(11, 40);
  display.setTextSize(2);
  if (BAT3DC) struja = bat3Voltage / 3.3;
  else struja = 0;
  display.print(struja);
  //  postotak = (49.17 * napon / 1000.0) - 147.5;
  //  if (postotak > 59) postotak = 59;
  //  else if (postotak < 0) postotak = 0;
  //  display.fillRect(2, 58, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

  display.setCursor(65, 39);
  display.setTextSize(1);
  if (BAT4DC) display.write(0x10);

  display.setCursor(65, 47);
  display.print("4.");
  display.setCursor(76, 40);
  display.setTextSize(2);
  if (BAT4DC) struja = bat4Voltage / 3.3;
  else struja = 0;
  display.print(struja);
  //  postotak = (49.17 * napon / 1000.0) - 147.5;
  //  if (postotak > 59) postotak = 59;
  //  else if (postotak < 0) postotak = 0;
  //  display.fillRect(67, 58, postotak, 4, SH110X_WHITE); // pocX, pocY, velX, velY //velX min 0, max 59

}
