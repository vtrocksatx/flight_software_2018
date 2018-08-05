import os
import picamera

KEEP_NUM_BOOTS = 5

# Read boot counter to match storage directory
bootFilePath = '/home/pi/FlightLogs/bootCountFile'
bootCountFile = open(bootFilePath, 'r')
bootCount = int(bootCountFile.read()) - 1 # because reasons
bootCountFile.close()

if bootCount >= KEEP_NUM_BOOTS:
        bootToRm = bootCount - KEEP_NUM_BOOTS
        cmd = "sudo rm /home/pi/FlightLogs/Boot_Number_" \
              + str(bootToRm) + "/*.h264"
        os.system(cmd)

# Setup directory for log files
path = '/home/pi/FlightLogs/Boot_Number_' + str(bootCount) + '/'

# Setup Camera
camera = picamera.PiCamera()
camera.resolution = (1280, 720) # 1280x720 view
camera.framerate = 25 # 25 fps

# Variables
numVideos = 0 # Number of videos to take
vidLength = 30 # Length of each video in seconds
# Setup
fileName = path + 'video_' + str(numVideos) + '.h264'
camera.start_recording(fileName)

# Record videos
while True:
	fileName = path + 'video_' + str(numVideos) + '.h264'
	camera.split_recording(fileName)
	camera.wait_recording(vidLength)
	numVideos+=1
camera.stop_recording()


