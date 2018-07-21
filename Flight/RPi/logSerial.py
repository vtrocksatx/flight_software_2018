import os
import serial

#Set serial
serialPort = serial.Serial("/dev/ttyS0", baudrate=19200, timeout=3.0)


#Open and increment boot counter
bootFilePath = '/home/pi/FlightLogs/bootCountFile'
bootCountFile = open(bootFilePath, 'r')
bootCount = int(bootCountFile.read())
bootCountFile.close()
bootCountFile = open(bootFilePath, 'w')
bootCountFile.write(str(bootCount+1))
bootCountFile.close()

#Create directory for log files
path = '/home/pi/FlightLogs/Boot_Number_' + str(bootCount) + '/'
os.system('mkdir /home/pi/FlightLogs/Boot_Number_' + str(bootCount) )
fileName = path + 'file_0'
logFile = open(fileName, 'wb')

#Variables
charCount = 0; #keeps track of loop
fileNum = 0; #track file number
threshhold = 271


#Loop to read in data
while True:
    readCharacters = serialPort.read()
    charCount += len(readCharacters)
    if charCount > threshhold:
        logFile.close() #close previous file before starting up a new file
        fileNum += 1
        fileName = path + 'file_' + str(fileNum)
        logFile = open(fileName, 'wb')
        logFile.write(readCharacters) 
        charCount = len(readCharacters)
    else: 
        logFile.write(readCharacters)

