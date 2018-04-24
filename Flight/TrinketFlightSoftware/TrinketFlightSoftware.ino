#include <Arduino.h>   // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function

#define EXPERIMENT_RX_DATA_RATE 9600 // Rate at which data arrives from experiment (baud/bps)
#define XBEE_TX_DATA_RATE		19200 // Rate at which packets are transmitted to xbee

#define PAYLOAD_LEN 128
#define TX_REQUEST_HEADER_LEN 17
#define TX_REQUEST_CHKSUM_LEN 1

#define PACKET_LEN TX_REQUEST_HEADER_LEN + PAYLOAD_LEN + TX_REQUEST_CHKSUM_LEN

// Constants used in the packet header
#define START_DELIMITER 	0x7E
#define START_DELIM_OFFSET 	0

#define LENGTH_FIELD_VAL	0xE + PAYLOAD_LEN		// the length field in the txRequest packet is the number
#define LENGTH_FIELD_OFFSET	START_DELIM_OFFSET + 1	// of bytes between the length and checksum fields

#define FRAME_TYPE			0x10					// TX Request
#define FRAME_TYPE_OFFSET	LENGTH_FIELD_OFFSET + 2

#define FRAME_ID_NO_RESP	0x00					// Setting frame id to 0 will cause the receiver not to send a response
#define FRAME_ID_OFFSET		FRAME_TYPE_OFFSET + 1

#define DEST_ADDR_64		0x0013A200415C83C0		// Address for Dennis
#define DEST_ADDR_64_OFFSET	FRAME_ID_OFFSET + 1

#define DEST_ADDR_16		0xFFFE					// TODO: Determine appropriate value for this field
#define DEST_ADDR_16_OFFSET	DEST_ADDR_64_OFFSET + 8

#define BROADCAST_RADIUS	0x03					// Maximum allowed hops for a broadcast;
#define BROADCAST_RAD_OFFSET DEST_ADDR_16_OFFSET + 2 // 3 is the maximum hops to the receiver with a linear path

#define OPTIONS				0x00 					// No options in use
#define OPTIONS_OFFSET 		BROADCAST_RAD_OFFSET + 1

#define PAYLOAD_OFFSET		OPTIONS_OFFSET + 1

#define CHECKSUM_OFFSET		PAYLOAD_OFFSET + PAYLOAD_LEN

// Serial2 pin definitions
#define SERIAL_2_RX_PIN 2ul
#define SERIAL_2_TX_PIN 0ul

uint8_t txRequest[PACKET_LEN] = {0};

uint8_t rxBuffer[PAYLOAD_LEN] = {0};
uint8_t rxBufferIndex = 0;

// Setup a second hardware serial port for communicating with XBee
Uart Serial2(&sercom2, SERIAL_2_RX_PIN, SERIAL_2_TX_PIN, SERCOM_RX_PAD_1, UART_TX_PAD_0);
void SERCOM2_Handler()
{
  Serial2.IrqHandler();
}

/* 	Converts n bytes of number to a big-endian sequence of bytes starting at dest
	Valid for n <= 8 (sizeof(int64_t)) */
void nByteNumberToBigEndianByteStream(uint8_t n, int64_t number, uint8_t* dest){
	for(uint8_t i = 0; i < n; ++i){
		*(dest + i) = (number >> (n-(i+1))*8) & 0xFF;
	}
}

/*	Sets the values of fields in the txRequest frame that will remain constant	*/
void initializeTxRequest(){
	txRequest[START_DELIM_OFFSET] = START_DELIMITER;
	nByteNumberToBigEndianByteStream(2, LENGTH_FIELD_VAL, &txRequest[LENGTH_FIELD_OFFSET]);
	txRequest[FRAME_TYPE_OFFSET] = FRAME_TYPE;
	txRequest[FRAME_ID_OFFSET] = FRAME_ID_NO_RESP;
	nByteNumberToBigEndianByteStream(8, DEST_ADDR_64, &txRequest[DEST_ADDR_64_OFFSET]);
	nByteNumberToBigEndianByteStream(2, DEST_ADDR_16, &txRequest[DEST_ADDR_16_OFFSET]);
	txRequest[BROADCAST_RAD_OFFSET] = BROADCAST_RADIUS;
	txRequest[OPTIONS_OFFSET] = OPTIONS;
}
/*	Updates the checksum field in txRequest */
void updateChecksum(){
	uint8_t sum = 0;
	for(uint8_t i = FRAME_TYPE_OFFSET; i < CHECKSUM_OFFSET; ++i){
		sum += txRequest[i];
	}
	txRequest[CHECKSUM_OFFSET] = 0xFF - sum;
}

void sendTxRequest(){
	// Fill in data field
	for(uint8_t i = 0; i < PAYLOAD_LEN; ++i){
		txRequest[PAYLOAD_OFFSET + i] = rxBuffer[i];
	}

	updateChecksum();

	// write out packet
	for(uint8_t i = 0; i < PACKET_LEN; ++i){
		Serial1.write(txRequest[i]);
	}
}

void setup(){
	// Setup serial for receiving 
	Serial2.begin(EXPERIMENT_RX_DATA_RATE);
  	// Assign Serial2 pins SERCOM functionality
  	pinPeripheral(SERIAL_2_RX_PIN, PIO_SERCOM_ALT);
  	pinPeripheral(SERIAL_2_TX_PIN, PIO_SERCOM_ALT);

  	// Setup serial for transmitting xbee frames
	Serial1.begin(XBEE_TX_DATA_RATE);
	initializeTxRequest();
}

void loop(){
	while(Serial2.available()){
		rxBuffer[rxBufferIndex] = Serial2.read();
		if(rxBufferIndex == PAYLOAD_LEN - 1){
			sendTxRequest();
			rxBufferIndex = 0;
		} else {
			rxBufferIndex += 1;
		}
	}
}