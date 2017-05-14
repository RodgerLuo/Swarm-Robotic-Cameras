


import time
import picamera

import os, sys
import pipan


# center the camera
p = pipan.PiPan()
p.neutral_pan()
p.neutral_tilt()

time.sleep (1)




with picamera.PiCamera() as camera:

    from random import randint
    pan_position = randint(100,160)
    tilt_position = randint(100,160)


    p.do_pan(pan_position)
    time.sleep(0.1)
    p.do_tilt(tilt_position)
    time.sleep(0.1)


    camera.resolution = (1920, 1080)
    camera.hflip = True
    camera.vflip = True
    camera.start_preview()
#     camera warm-up time
    time.sleep(2)
    camera.capture('images/test.jpg')



