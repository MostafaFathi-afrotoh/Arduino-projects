#include <Wire.h>                // I2C Communication
#include <LiquidCrystal_I2C.h>   // LCD I2C Library
#include <OneWire.h>
#include <DallasTemperature.h>
const int startLed = 24; //this led is blue colored
const int stopeLed = 25; // this led is red colored

// const int Finished = ; // this led is red colored


const int upperHeater = 44;
const int bottomHeater = 45;
const int heaterLed1 = 27 ; // this is green colored
const int heaterLed2 = 28; // green color for the second bottom heater

const int Agitator;
const int airGeneratorLed = 26;
const int airGenerator1 = 42;
const int airGenerator2 = 43;

int desiredTemp = 30;

long countdownTime = 300; // Countdown time in seconds (5 minutes)
bool startProcess = false;
bool AIR1 = false;
bool AIR2 = false;


#define ONE_WIRE_BUS 9  // Data wire connected to pin 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

// LCD: Set I2C Address (0x27 or 0x3F depending on the module)
LiquidCrystal_I2C lcd(0x27, 16, 2);  
void setup() {
    String name1 = "Salah Badr";
    String name2 = "Ragab Qasem";
    String name3 = "Mostafa Fathy";
    String names[] = {"Umr Elsayed " ,"Mohamed Essam", "Amr Ibrahim ", "Ahmed Samy", "Omar Ahmed", "Ahmed Wasiefie", "Ahmed Hashad", "Aly Mansour"};
    
    pinMode(startLed, OUTPUT);
    pinMode(stopeLed, OUTPUT);
    
    pinMode(upperHeater, OUTPUT);
    pinMode(bottomHeater, OUTPUT);
    pinMode(heaterLed1, OUTPUT);
    pinMode(heaterLed2, OUTPUT);

    pinMode(airGenerator1, OUTPUT);
    pinMode(airGenerator2, OUTPUT);
    pinMode(airGeneratorLed, OUTPUT);

    pinMode(Agitator, OUTPUT);

    Serial.begin(9600);    // Serial Monitor
    Serial1.begin(9600);   // Bluetooth module communication
    sensors.begin();       // Start the DS18B20 sensor

    // LCD Initialization
    lcd.init();
    lcd.backlight();  // Turn on LCD backlight
    lcd.setCursor(0, 0);
    lcd.print("Initializing...");
    delay(2000);
    lcd.clear();
    lcd.print("Supervisor...");
    delay(2000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Prof/Dr..");
    delay(2000);
    for (int i =0; i < name1.length(); i++){
      lcd.setCursor(i,1);
      lcd.print(name1[i]);
      delay(500);
    }
    delay(2000);
    lcd.clear();
   
    delay(2000);
    lcd.clear();
    lcd.print("Dr/..");
    delay(500);
    for (int i =0; i < name2.length(); i++){
      lcd.setCursor(i,1);
      lcd.print(name2[i]);
      delay(500);
    }
    delay(2000);
    lcd.clear();
    delay(500);
    lcd.print("Mr/..");
    delay(500);
    for (int i =0; i < name3.length(); i++){
      lcd.setCursor(i,1);
      lcd.print(name3[i]);
      delay(500);
    }
    delay(500);
    delay(2000);
    lcd.clear();
    delay(100);
    for (int i = 0; i < sizeof(names) / sizeof(names[0]); i++){
        lcd.print("Students....");
        delay(500);
        lcd.setCursor(0,0);
        lcd.setCursor(0, 1);
        lcd.print(names[i]);
        delay(1500);
        lcd.clear();
    
    }
    // delay();
    lcd.clear();
}
void loop() {
    // Check for Bluetooth commands
    if (Serial1.available()) {
        String command = Serial1.readStringUntil('\n');
        command.trim();
        
        if (command.startsWith("TIME:")) {
            int newTime = command.substring(5).toInt();
            if (newTime > 0) {
                countdownTime = newTime;
                Serial1.print("Time set to: ");
                Serial1.println(countdownTime);
            }
        } else if (command.startsWith("TEMP:")) {
            int newTemp = command.substring(5).toInt();
            desiredTemp = newTemp;
            Serial1.print("Desired Temp set to: ");
            Serial1.println(desiredTemp);
        } else if (command.startsWith("AIR1:ON") ) {
             AIR1 = true;
        } else if (command.startsWith("AIR2:ON") ) {
             AIR2 = true;
        } else if (command.startsWith("AIR1:OFF") ) {
             AIR1 = false;
        } else if (command.startsWith("AIR2:OFF") ) {
             AIR2 = false;
        } else if (command == "START") {
            startProcess = true;
            digitalWrite(startLed, HIGH);
            digitalWrite(stopeLed, LOW);
        } else if (command == "STOP") {
            startProcess = false;
            digitalWrite(startLed, LOW);
            digitalWrite(stopeLed, HIGH);
            digitalWrite(upperHeater, LOW);
            digitalWrite(bottomHeater, LOW);
            digitalWrite(heaterLed1, LOW);
            digitalWrite(heaterLed2, LOW);
            digitalWrite(airGenerator1, LOW);
            digitalWrite(airGenerator2, LOW);
            digitalWrite(airGeneratorLed, LOW);
        }
    }

    if (startProcess) {
        sensors.requestTemperatures();  // Request temperature readings
        
        float temperature1 = sensors.getTempCByIndex(0); // First sensor
        float temperature2 = sensors.getTempCByIndex(1); // Second sensor

        // Display Data on LCD
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("T1:");
        lcd.print(temperature1);
        lcd.print("C ");
        lcd.print("T2:");
        lcd.print(temperature2);
        lcd.print("C");
        lcd.setCursor(13, 1);
        lcd.print(desiredTemp);

        // Countdown Timer Display
        int minutes = countdownTime / 60;
        int seconds = countdownTime % 60;
        
        lcd.setCursor(0, 1);
        lcd.print("Time:");
        lcd.print(minutes);
        lcd.print("m ");
        lcd.print(seconds);
        lcd.print("s");

        // Send data over Bluetooth
        Serial1.print("T1:");
        Serial1.print(temperature1);
        Serial1.print("\n");

        Serial1.print("T2:");
        Serial1.print(temperature2);
        Serial1.print("\n");

        // Debugging on Serial Monitor
        Serial.print("Temperature 1: ");
        Serial.print(temperature1);
        Serial.print(" °C   Temperature 2: ");
        Serial.print(temperature2);
        Serial.println(" °C");
       
        if (AIR1 == true) {
          digitalWrite(airGenerator1, HIGH);
          digitalWrite(airGeneratorLed, HIGH);        }
        else if (AIR1 == false){
          digitalWrite(airGenerator1, LOW);
          digitalWrite(airGeneratorLed, LOW);
        }
        if (AIR2 == true){
          // digitalWrite(airGenerator1, LOW);
          digitalWrite(airGenerator2, HIGH);
          digitalWrite(airGeneratorLed, HIGH);
        }
        else if (AIR2 == false){
          digitalWrite(airGenerator2, LOW);
          digitalWrite(airGeneratorLed, LOW);

        }
        // }
        if (temperature1 < desiredTemp && countdownTime > 0 && startProcess == true )  {
          if (startProcess == true) {
            digitalWrite(upperHeater, HIGH);
            digitalWrite(heaterLed1, HIGH);
          }
          else if (startProcess == false){
            digitalWrite(upperHeater, LOW);
            digitalWrite(heaterLed1, LOW);
          }
        
        // } else {
        //     digitalWrite(upperHeater, LOW);
        //     digitalWrite(heaterLed1, LOW);
        }
        if (temperature2 < desiredTemp && countdownTime > 0 && startProcess == true ) {
          if (startProcess == true) {
            digitalWrite(bottomHeater, HIGH);
            digitalWrite(heaterLed2, HIGH);
          }
          else if (startProcess == false){
            digitalWrite(bottomHeater, LOW);
            digitalWrite(heaterLed2, LOW);
          }
        }
        
        // } else {
        //     digitalWrite(upperHeater, LOW);
        //     digitalWrite(heaterLed2, LOW);
        // }


        if (countdownTime > 0) {
            countdownTime--;
        } else {
            digitalWrite(airGenerator1, LOW);
            digitalWrite(airGenerator2, LOW);
            digitalWrite(airGeneratorLed, LOW);

            digitalWrite(upperHeater, LOW);
            digitalWrite(heaterLed1, LOW);
            digitalWrite(heaterLed2, LOW);
            digitalWrite(bottomHeater, LOW);

            startProcess = false;
        }

        delay(1000);  // Update every second
    }
}
