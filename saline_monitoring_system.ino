#include <HX711_ADC.h> // Library for HX711 load cell
#include <Wire.h>
#include <LiquidCrystal_I2C.h> // Library for I2C LCD
#include <SoftwareSerial.h>

SoftwareSerial gsmSerial(9, 10); // GSM module communication pins
int weightFlag1 = 0, weightFlag2 = 0, weightFlag3 = 1, weightFlag4 = 1, weightFlag5 = 1;
HX711_ADC loadCell(4, 5); // HX711 load cell pins: DT pin = 4, SCK pin = 5
LiquidCrystal_I2C lcd(0x3F, 16, 2); // LCD I2C address: 0x3F, 16 columns, 2 rows

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  float weightValue;

  // Initialize LCD
  lcd.init();
  lcd.begin(16, 2); // 16 columns, 2 rows
  lcd.backlight(); // Turn on the backlight

  // Initialize HX711 load cell
  loadCell.begin();
  loadCell.start(2000); // Wait for the load cell to stabilize (2000ms)
  loadCell.setCalFactor(419); // Calibration factor (adjust this based on your setup)
}

void loop() {
  loadCell.update(); // Update data from the load cell
  float weightValue = loadCell.getData(); // Get load cell output

  // Display weight on LCD
  lcd.setCursor(0, 0); // Set cursor to the first row
  lcd.print("Weight [g]:"); // Print label
  lcd.setCursor(0, 1); // Set cursor to the second row
  lcd.print(weightValue); // Print the weight value

  // Check weight thresholds and send SMS if conditions met
  if (weightValue > 50) {
    weightFlag1 = 1;
  }
  if (weightFlag1) {
    if (weightValue < 55) {
      if (weightFlag3 == 1) {
        lcd.init();
        lcd.print("Threshold reached 50");
        gsmSerial.println("AT+CMGF=1"); // Set GSM Module in Text Mode
        delay(1000);
        gsmSerial.println("AT+CMGS=\"+91123456789\"\r"); // Replace with your mobile number
        delay(1000);
        Serial.println("Sent SMS: Weight exceeded 50 grams");
        gsmSerial.print("Current weight: ");
        gsmSerial.println(weightValue);
        delay(100);
        gsmSerial.println((char)26); // ASCII code of CTRL+Z to end SMS
        delay(1000);
        weightFlag3 = 0;
      }
    }
  }

  if (weightValue > 21) {
    weightFlag2 = 1;
  }
  if (weightFlag2) {
    if (weightValue < 20) {
      if (weightFlag4 == 1) {
        lcd.init();
        lcd.print("Threshold reached 20");
        gsmSerial.println("AT+CMGF=1"); // Set GSM Module in Text Mode
        delay(1000);
        gsmSerial.println("AT+CMGS=\"+91123456789\"\r"); // Replace with your mobile number
        delay(1000);
        Serial.println("Sent SMS: Weight exceeded 20 grams");
        gsmSerial.print("Current weight: ");
        gsmSerial.println(weightValue);
        delay(100);
        gsmSerial.println((char)26); // ASCII code of CTRL+Z to end SMS
        delay(1000);
        weightFlag4 = 0;
      }
    }
  }

  if (weightValue > 10) {
    weightFlag2 = 1;
  }
  if (weightFlag2) {
    if (weightValue < 10) {
      if (weightFlag5 == 1) {
        lcd.init();
        lcd.print("Threshold reached 10");
        gsmSerial.println("AT+CMGF=1"); // Set GSM Module in Text Mode
        delay(1000);
        gsmSerial.println("AT+CMGS=\"+91123456789\"\r"); // Replace with your mobile number
        delay(1000);
        Serial.println("Sent SMS: Weight exceeded 10 grams");
        gsmSerial.println("Threshold reached");
        gsmSerial.print("Current weight: ");
        gsmSerial.println(weightValue);
        gsmSerial.print("Please take action.");
        delay(100);
        gsmSerial.println((char)26); // ASCII code of CTRL+Z to end SMS
        delay(1000);
        weightFlag5 = 0;
      }
    }
  }
}

