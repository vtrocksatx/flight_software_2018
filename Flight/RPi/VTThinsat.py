from picamera import PiCamera
from time import sleep
import Adafruit_BMP.BMP085 as BMP085
import serial
import time
from PIL import Image
import os, sys
import math

bmpOk = False
try:
    sensor = BMP085.BMP085(mode=BMP085.BMP085_HIGHRES)
    bmpOk = False
except:
    bmpOk = False

cameraOk = True
try:
    camera = PiCamera()
    camera.resolution = (1280,960)
except:
    cameraOk = False

txOut = serial.Serial('/dev/ttyS0')

IMAGE_PAYLOAD_LEN = 256 + 8

TX_REQUEST_HEADER_LEN = 17
TX_REQUEST_CHKSUM_LEN = 1

IMAGE_PACKET_LEN = TX_REQUEST_HEADER_LEN + IMAGE_PAYLOAD_LEN + TX_REQUEST_CHKSUM_LEN

CHECKSUM_START_IDX = 3

packetBuffer = bytearray(IMAGE_PACKET_LEN)

def ithByteOfN(n, num, i):
    return (num >> (n-(i+1))*8) & 0xFF;

def fillPacketHeader(payloadLen):

    # Constants used in the packet header
    START_DELIMITER =  	    0x7E
    START_DELIM_OFFSET =    0

    LENGTH_FIELD_VAL = 	    0xE + payloadLen		# the length field in the txRequest packet is the number
    LENGTH_FIELD_OFFSET =   START_DELIM_OFFSET + 1	# of bytes between the length and checksum fields

    FRAME_TYPE = 	    0x10			# TX Request
    FRAME_TYPE_OFFSET =	    LENGTH_FIELD_OFFSET + 2

    FRAME_ID_NO_RESP = 	    0x00			# Setting frame id to 0 will cause the receiver not to send a response
    FRAME_ID_OFFSET = 	    FRAME_TYPE_OFFSET + 1

    DEST_ADDR_64 = 	    bytearray([0x00,0x13,0xA2,0x00,0x41,0x77,0xFD,0x81])	# Address for ground station
    DEST_ADDR_64_OFFSET =   FRAME_ID_OFFSET + 1

    DEST_ADDR_16 =          0xFFFE
    DEST_ADDR_16_OFFSET =   DEST_ADDR_64_OFFSET + 8

    BROADCAST_RADIUS =	    0x03					
    BROADCAST_RAD_OFFSET =  DEST_ADDR_16_OFFSET + 2

    OPTIONS =		    0x00 			# No options in use
    OPTIONS_OFFSET =	    BROADCAST_RAD_OFFSET + 1

    PAYLOAD_OFFSET =	    OPTIONS_OFFSET + 1

    CHECKSUM_OFFSET =	    PAYLOAD_OFFSET + payloadLen

    # Fill in header in packet buffer
    for i in range(0, IMAGE_PACKET_LEN):
        packetBuffer[i] = 0x00

    packetBuffer[START_DELIM_OFFSET] = START_DELIMITER
    packetBuffer[LENGTH_FIELD_OFFSET] = ithByteOfN(2, LENGTH_FIELD_VAL, 0)
    packetBuffer[LENGTH_FIELD_OFFSET + 1] = ithByteOfN(2, LENGTH_FIELD_VAL, 1)
    packetBuffer[FRAME_TYPE_OFFSET] = FRAME_TYPE
    packetBuffer[FRAME_ID_OFFSET] = FRAME_ID_NO_RESP
    for i in range(0, len(DEST_ADDR_64)):
        packetBuffer[DEST_ADDR_64_OFFSET + i] = DEST_ADDR_64[i]
    packetBuffer[DEST_ADDR_16_OFFSET] = ithByteOfN(2, DEST_ADDR_16, 0)
    packetBuffer[DEST_ADDR_16_OFFSET + 1] = ithByteOfN(2, DEST_ADDR_16, 1)
    packetBuffer[BROADCAST_RAD_OFFSET] = BROADCAST_RADIUS
    packetBuffer[OPTIONS_OFFSET] = OPTIONS

def updateChecksum(sumIdx):
	checksum = 0
	for i in range(CHECKSUM_START_IDX, sumIdx):
		checksum = (checksum + packetBuffer[i]) % 256;
	packetBuffer[sumIdx] = 0xFF - checksum;

def sendSensorPacket():
    if bmpOk:
        temp = sensor.read_temperature()
        press = sensor.read_pressure()
        payloadString = "{0:0.2f},{1:0.2f}".format(temp, press)
        sendString(payloadString)
    else:
        sendString("BMP180 ERROR")

def sendString(text):
    payload = bytearray(text, 'ascii')
    fillPacketHeader(len(payload))
    for i in range(0, len(payload)):
        packetBuffer[TX_REQUEST_HEADER_LEN + i] = payload[i]
    updateChecksum(TX_REQUEST_HEADER_LEN + len(payload))
    sendPacket(TX_REQUEST_HEADER_LEN + len(payload) + 1)

def sendPacket(length):
        txOut.write(packetBuffer[:length])

while True:
    sendSensorPacket()
    time.sleep(1)

    '''try:
        camera.capture('/home/pi/image.png')
        im = Image.open("/home/pi/image.png")
        bg = Image.new("RGB", im.size, (255,255,255))
        bg.paste(im, (0,0), im)
        bg.save("image.jpeg", quality = 50)
    
        with open('image.jpeg', 'rb') as imageFile:
            f = imageFile.read()
            imageBytes = bytearray(f)

        numPackets = math.ceil(len(imageBytes) / 256)

        for i in range(0,numPackets):
            fillPacketHeader(IMAGE_PAYLOAD_LEN)
            for j in range(0,4):
                packetBuffer[TX_REQUEST_HEADER_LEN + i]'''
            
