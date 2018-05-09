import os
import serial

#Set serial
serialPort = serial.Serial("/dev/ttyS0", baudrate=19200, timeout=3.0)


#Open and increment boot counter
bootCountFile = open('bootCountFile', 'r')
bootCount = int(bootCountFile.read())
bootCountFile.close()
bootCountFile = open('bootCountFile', 'w')
bootCountFile.write(str(bootCount+1))
bootCountFile.close()

#Create directory for log files
path = './Boot_Number_' + str(bootCount) + '/'
os.system('mkdir Boot_Number_' + str(bootCount) )
fileName = path + 'file_0'
logFile = open(fileName, 'wb')

#Variables
current = 0; #keeps track of loop
fileNum = 0; #track file number
threshhold = 271


#Loop to read in data
while True:
    readCharacters = serialPort.read()
    current += len(readCharacters)
    if current > threshhold:
        logFile.close() #close previous file before starting up a new file
        fileNum += 1
        fileName = path + 'file_' + str(fileNum)
        logFile = open(fileName, 'wb')
        logFile.write(readCharacters) 
        current = len(readCharacters)
    else: 
        logFile.write(readCharacters)

