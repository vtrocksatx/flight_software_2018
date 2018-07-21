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
numVideos = 30 # Number of videos to take
vidLength = 30 # Length of each video in seconds

# Record videos
for i in range (0 , numVideos) #30 30s segments = 15m of video
	fileName = path + 'video_' + i + '.h264'
	camera.split_recording('fileName')
	camera.wait_recording(vidLength)
camera.stop_recording()


