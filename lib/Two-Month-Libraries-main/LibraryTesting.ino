#include <SHC_BNO055.h>
#include <Utils.h>
#include <SHC_BME280.h>
#include <SHC_M9N.h>

/*
 * This is what the library was tested with. It runs all of the sensors on their default I2C addresses using the default pins and prints to Serial the data.  
 * The built in LED will blink if there is a wiring error and if the GPS is being tested, it will be on until a lock is achieved. 
 * Note that this also means that if you have any of the sensors not correctly wired - or not wired at all - then it will lock and not return anything.
 * With that in mind, comment out any sensor tests and inits for sensors you do not currently have connected. 
*/

M9N gps = M9N();
BNO055 mybno = BNO055();
SHC_BME280 press = SHC_BME280();

void GPStest();
void GPSinit();
void BNOtest();
void BNOinit();
void BMEtest();
void BMEinit();

void setup() {
  Wire.begin();  


  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);

  GPSinit();
  BNOinit();
  BMEinit();

}

void loop() {
  GPStest();
  BNOtest();
  BMEtest();
  delay(50);
}


// Testing functions to make it easier to put into the setup and loop
void GPSinit() {
  while (gps.init() != NO_ERROR) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.println("M9N Not Found");
  }
}
void GPStest() {
  gps.prefetchData();
  if (gps.getSIV() < 3) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println("Waiting for lock; SIV at " + String(gps.getSIV()));
  } else {
    digitalWrite(LED_BUILTIN, LOW);
  }
  Serial.println("Latitude: " + String(gps.getLatitude()));
  Serial.println("Longitude: " + String(gps.getLongitude()));
  Serial.println("Altitude: " + String(gps.getAltitude()));
  Serial.print("" + String(gps.getYear()));
  Serial.print(", " + String(gps.getMonth()));
  Serial.print(" " + String(gps.getDay()));
  Serial.print(" " + String(gps.getHour()));
  Serial.print(":" + String(gps.getMinute()));
  Serial.println(":" + String(gps.getSecond()));
  Serial.println("SIV: " + String(gps.getSIV()));
  Serial.println("UNIX: " + String(gps.getUnixTime()));

}

void BNOinit() {
  while (mybno.init() != NO_ERROR) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.println("BNO Not Found");
  }
}

void BNOtest() {
  mybno.prefetchData();
  Serial.print("Accel: (" + String(mybno.getAccelerationX()));
  Serial.print(", " + String(mybno.getAccelerationY()));
  Serial.println(", " + String(mybno.getAccelerationZ()) + ")");
  Serial.print("Gyro: (" + String(mybno.getGyroX()));
  Serial.print(", " + String(mybno.getGyroY()));
  Serial.println(", " + String(mybno.getGyroZ()) + ")");
  Serial.print("Orient: (" + String(mybno.getOrientationX()));
  Serial.print(", " + String(mybno.getOrientationY()));
  Serial.println(", " + String(mybno.getOrientationZ()) + ")");
}


void BMEinit() {
  while (press.init() != NO_ERROR) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.println("BME Not Found");
  }
}

void BMEtest() {
  press.prefetchData();
  Serial.println("Pressue: " + String(press.getPressure()));
  Serial.println("Temperature: " + String(press.getTemperature()));
  Serial.println("Altitude: " + String(press.getAltitude()));
  Serial.println("Humidity: " + String(press.getHumidity()));
  
}


