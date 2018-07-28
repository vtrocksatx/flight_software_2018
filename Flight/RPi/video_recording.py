import os
import picamera

# Read boot counter to match storage directory
bootFilePath = '/home/pi/FlightLogs/bootCountFile'
bootCountFile = open(bootFilePath, 'r')
bootCount = int(bootCountFile.read())
bootCountFile.close()

# Setup directory for log files
path = '/home/pi/FlightLogs/Boot_Number_' + str(bootCount) + '/'

# Setup Camera
camera = picamera.PiCamera()
camera.resolution = (640,480) # 640x480 view
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


