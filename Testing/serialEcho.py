import serial

serialPort = serial.Serial("/dev/ttyS0", baudrate=9600, timeout=3.0)

current = 0; #keeps track of loop
fileNum = 0; #track file number
threshhold = 256
filename = 'file' + str(fileNum)
logFile = open(filename, 'wb')

while True:
    #rcv = serialPort.readline()
    #print(rcv)
    readCharacters = serialPort.read()
    current += len(readCharacters)
    if current > threshhold:
        logFile.close() #close previous file before starting up a new file
        fileNum += 1
        fileName = 'file' + str(fileNum)
        logFile = open(fileName, 'wb')
        logFile.write(readCharacters) 
        current = len(readCharacters)
    else: 
        logFile.write(readCharacters)

