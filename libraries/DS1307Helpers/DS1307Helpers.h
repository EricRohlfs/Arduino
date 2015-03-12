class DS1307Helpers
{
  public:
    Morse(int pin);
    void setDateDs1307();
    void getDateDs1307();
	byte decToBcd(byte val);
	byte bcdToDec(byte val);
	int command;      // This is the command char, in ascii form, sent from the serial port     
	int i;
	long previousMillis ;        // will store last time Temp was updated
	byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
	byte test;
	byte zero;
	char  *Day[] ;
	char  *Mon[] ;
  private:
    
};