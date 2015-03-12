/*
 * Water logger by Eric Rohlfs copywright 2015
 * 
 * 
 */
 
#include <Time.h>  
#include <Wire.h>  
#include <DS1307RTC.h> 

//time to wait between logging events. 
//
int secondsToWait = 1;

void setup(){
  Serial.begin(9600);
 
}

void loop(){

  delay(secondsToWait * 1000);
}

string getDateTime(){
  tmElements_t tm;
  if(!RTC.read(tm){
    ///print and return an error
    Serial.print("Could not connect to the clock.");
  }
  // we want a comma separating the date and time fields so we don't have 
  // to parse in excel.  It is easier to combine two cells in excel than to split
  // terminating with '\0' as per string style guide
  char dateTimeStr[16] = {getFormattedDate(tm), ',' ,getFormattedTime(tm), '\0'};
  return dateTimeStr;
}


string getFormattedDate(tmElements_t tm){
  //Returns the time in the following format
  // yy-mm-dd
  // terminating with '\0' as per string style guide
  char dateStr[7] = {tm.Year, '-', tm.Month, '-' , tm.Day, '\0'};
  return date;
}


string getFormattedTime(tmElements_t tm tm){
  //Returns time in the following format
  // hh:mm:dd
  // terminating with '\0' as per string style guide
  char timeStr[7] = {tm.Hour, ':', tm.Minute, '\0'}
  return timeStr;
}

void printDigits(int digits){
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

