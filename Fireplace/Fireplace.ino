/*
  SD card datalogger
 
 This example shows how to log data from three analog sensors 
 to an SD card using the SD library.
 	
 The circuit:
 * analog sensors on analog ins 0, 1, and 2
 * SD card attached to SPI bus as follows:
 ** MOSI - pin 11
 ** MISO - pin 12
 ** CLK - pin 13
 ** CS - pin 4
 
 created  24 Nov 2010
 modified 9 Apr 2012
 by Tom Igoe
 
 This example code is in the public domain.
 	 
 */

#include <SD.h>

#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h>  // a basic DS1307 library that returns time as a time_t


#include "MAX31855.h" // Amplifier for thermalcouple K type 3.3v


// On the Ethernet Shield, CS is pin 4. Note that even if it's not
// used as the CS pin, the hardware CS pin (10 on most Arduino boards,
// 53 on the Mega) must be left as an output or the SD library
// functions will not work.
const int chipSelect = 4;

const int doPin = 7; //so for max13855
const int csPin = 6; //cs for max13855
const int clPin = 5; //sck max13855

MAX31855 tc(clPin, csPin, doPin); //global var tc for this sketch tc = thermolcouple

//#include <OneWire.h>
//#include <DallasTemperature.h>
// Data wire is plugged into port 2 on the Arduino
//#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
//OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature. 
//DallasTemperature sensors(&oneWire);

void setup()
{
 // Open serial communications and wait for port to open:
  Serial.begin(9600);
   while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
   Serial.println(MAX31855_VERSION);
   tc.begin();
   setSyncProvider(RTC.get);   // the function to get the time from the RTC
   if(timeStatus()!= timeSet) 
     Serial.println("Unable to sync with the RTC");
   else
     Serial.println("RTC has set the system time");   

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
  Serial.println("Send a over a return carrage via serial to see current stats");
}

void loop()
{ 
  ShowMeSomething();
  if(isTimeToLog()){
    logData();
    Serial.print("sleeping for milliseconds: ");  
    delay(33000); //35 seconds
  }
 }

//hopefully cleaner code and might use less battery
void ShowMeSomething(){
  if (Serial.available() > 0) {
          // read the incoming byte:
          int incomingByte = Serial.read();
          float last = tc.getTemperature();
          tc.read();
          int currentTemp = (int) tc.getTemperature();
          int delta = (int) currentTemp - last;
          Serial.print(currentTemp);
          Serial.print(",");
          Serial.print(delta);
          Serial.println();
        }
}

//the main loop method was getting too big.
void logData(){
// open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  float last = tc.getTemperature();
  tc.read();
  int currentTemp = (int) tc.getTemperature();
  int delta = (int) currentTemp - last;
  // if the file is available, write to it:
  if (dataFile) {
    //dataFile.println(GetTemperature());
    writeTimeToFile(dataFile);
    dataFile.print(",");
    dataFile.print(currentTemp);
    dataFile.print(",");
    dataFile.print(delta);
    dataFile.print(",");
    dataFile.print((int)tc.getInternal());
    dataFile.println();
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datalog.txt");
  } 
}

//want to compare across days and hours
//since I am taking 5 minute readings 
//only log minutes that end in 5 or zero
boolean isTimeToLog(){
  String m = String(minute());
  if(m.endsWith("0") || m.endsWith("5")){
    return true;
  }
  return false;
}

void writeTimeToFile(File d){
  if (d) {
      d.print(hour());
      printDigitsToFile(minute(),d);
      //printDigitsToFile(second(),d);
      d.print(",");
      d.print(year()); 
      d.print("-");
      d.print(month());
      d.print("-");
      d.print(day());
    }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening file.");
  } 
}

void printDigitsToFile(int digits,File d){
  // utility function for digital clock display: prints preceding colon and leading 0
  d.print(":");
  if(digits < 10)
    d.print('0');
  d.print(digits);
}

///for onewire
//int GetTemperature(){
//   Serial.println(tc.readFarenheit();
//   return tc.readFarenheit();

  //sensors.requestTemperatures();
  //int temp = (int) sensors.getTempFByIndex(0);
  
 // Serial.println(temp);
  //return temp;
 //}







