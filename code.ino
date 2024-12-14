#include <LiquidCrystal.h>
#include <DHT.h>

#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const int trigPin = 2;
const int echoPin = 3;

char value;

LiquidCrystal lcd(5, 6, 7, 8, 9, 10);
// Function to turn the LED on for a short duration (dot)
void dot() {
  digitalWrite(11, HIGH); 
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH);
  delay(250); // Duration of a dot
  digitalWrite(11, LOW); 
  digitalWrite(12, LOW);
  digitalWrite(13, LOW); // Turn off the LED
  delay(250); // Pause between dots
}

// Function to turn the LED on for a longer duration (dash)
void dash() {
  digitalWrite(11, HIGH); 
  digitalWrite(12, HIGH);
  digitalWrite(13, HIGH); // Turn on the LED
  delay(750); // Duration of a dash
  digitalWrite(11, LOW); 
  digitalWrite(12, LOW);
  digitalWrite(13, LOW); // Turn off the LED
  delay(250); // Pause between dashes/dots
    }

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  dht.begin();
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("Welcome");
}

void loop() {
  while (Serial.available() > 0) {
    value = Serial.read();
  }

  if (value == 's') {
    // "S"
    dot();
    dot();
    dot();
    delay(1000); // Pause between letters

    // "O"
    dash();
    dash();
    dash();
    delay(1000); // Pause between letters

    // "S"
    dot();
    dot();
    dot();
    delay(3000); // Pause between SOS signals
    
  }

  if (value == 'b') {
    // Increase brightness
    for (int brightness = 0; brightness <= 255; brightness=brightness+30)
     {
      analogWrite(11, brightness); // Set LED 1 brightness
      // analogWrite(12, brightness); // Set LED 2 brightness
      // analogWrite(13, brightness); // Set LED 3 brightness
      delay(50); // Delay for smooth transition
    }

    // Decrease brightness
    for (int brightness = 255; brightness >= 0; brightness=brightness-30) {
      analogWrite(11, brightness); // Set LED 1 brightness
      // analogWrite(12, brightness); // Set LED 2 brightness
      // analogWrite(13, brightness); // Set LED 3 brightness
      delay(50); // Delay for smooth transition
    }
  }

  if (value == 'r') {
    for (int i = 11; i <= 13; i++) {
      digitalWrite(i, HIGH);
      delay(750);
      digitalWrite(i, LOW);
      delay(800);
    }
    digitalWrite(11, HIGH);
    digitalWrite(13, HIGH);
    delay(750);
    digitalWrite(11, LOW);
    digitalWrite(13, LOW);
    delay(1000);
  }

  if (value == 'd') {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    long duration = pulseIn(echoPin, HIGH);
    int distance = duration * 0.034 / 2;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.setCursor(0, 1);
    lcd.print(distance);
    lcd.print(" cm");
    Serial.println(distance);
    delay(740);
  }

  if (value == 't') {
    delay(1500);

    float h = dht.readHumidity();
    float t = dht.readTemperature();
    float f = dht.readTemperature(true);

    if (isnan(h) || isnan(t) || isnan(f)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return;
    }

    float hif = dht.computeHeatIndex(f, h);
    float hic = dht.computeHeatIndex(t, h, false);

    lcd.setCursor(0, 0);
    lcd.print("Humidity: ");
    lcd.print(h);
    lcd.print("%");

    lcd.setCursor(0, 1);
    lcd.print("Temp:     ");
    lcd.print(t);
    lcd.print("C");

    Serial.print(F("\nHumidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
  }
}
