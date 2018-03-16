#include <Printers.h>
#include <XBee.h>

XBeeAddress64 addr64;
uint8_t payload[256];
uint8_t message[256];
XBee xbee = XBee();

// the setup function runs once when you press reset or power the board
void setup() 
{
  // initialize digital pin 13 as an output.
  pinMode(13, OUTPUT);
  // Create an XBee object at the top of your sketch
  message[1] = 'H';
  message[2] = 'i';
  message[3] = '\n';
  // Start the serial port
  Serial1.begin(9600);
  // Tell XBee to use Hardware Serial. It's also possible to use SoftwareSerial
  xbee.setSerial(Serial1);
  // Specify the address of the remote XBee (this is the SH + SL)
  addr64 = XBeeAddress64(0x0013a200, 0x415c83c0);
}
 
// the loop function runs over and over again forever
void loop() 
{
  // Create an array for holding the data you want to send.
  for( int i = 0; i < sizeof(message)/sizeof(uint8_t); i++ ) 
  {
    payload[i] = {message[i]};
  }
  // Create a TX Request
  ZBTxRequest zbTx = ZBTxRequest(addr64, payload, 3);//sizeof(payload));

  // Send your request
  xbee.send(zbTx);
  // Serial.println("Sent Tx"); -> Commented out as Xbee is using hardware serial
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);              // wait for a second
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);              // wait for a second
}
