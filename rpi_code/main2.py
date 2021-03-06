try:
    import cv2
except Exception as e:
    print("Warning: bitch")
from time import sleep
import thread
import threading
import atexit
import sys
import termios
import contextlib

import imutils
#import gpiozero
import RPi.GPIO as GPIO

GPIO.setmode(GPIO.BCM)
GPIO.setup(14,GPIO.OUT)
GPIO.setup(15,GPIO.OUT)

def raw_mode(file):
	old_attrs = termios.tcgetattr(file.fileno())
        new_attrs = old_attrs[:]
        new_attrs[3] = new_attrs[3] & ~(termios.ECHO | termios.ICANON)
        try:
            termios.tcsetattr(file.fileno(), termios.TCSADRAIN, new_attrs)
            yield
        finally:
            termios.csetattr(file.fileno(), termios.TCSADRAIN, old_attrs)

class VideoUtils(object):
    @staticmethod
    def live_video(camera_port=0):
        video_capture = cv2.VideoCapture(camera_port)
        while True:
            ret, frame = video_capture.read()
            cv2.imshow('Video', frame)
            if cv2.waitKey(1) & 0xFF == ord('q'):
                break
        video_capture.release()
        cv2.destroyAllWindows()
    @staticmethod
    def find_motion(callback, camera_port=0, show_video=False):
        camera = cv2.VideoCapture(camera_port)
        sleep(0.25)
        firstFrame = None
        tempFrame = None
        count = 0
        while True:
            (grabbed, frame) = camera.read()
            if not grabbed:
                break
            frame = imutils.resize(frame, width=500)
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            gray = cv2.GaussianBlur(gray, (21, 21), 0)
            if firstFrame is None:
                print ("Waiting for video to adjust...")
                if tempFrame is None:
                    tempFrame = gray
                    continue
                else:
                    delta = cv2.absdiff(tempFrame, gray)
                    tempFrame = gray
                    tst = cv2.threshold(delta, 5, 255, cv2.THRESH_BINARY)[1]
                    tst = cv2.dilate(tst, None, iterations=2)
                    if count > 30:
                        print ("Done. \n Waiting for motion.")
                        if not cv2.countNonZero(tst) > 0:
                            firstFrame = gray
                        else:
                            continue
                    else:
                       count += 1
                       continue
            frameDelta = cv2.absdiff(firstFrame, gray)
            thresh = cv2.threshold(frameDelta, 25, 255, cv2.THRESH_BINARY)[1]
            thresh = cv2.dilate(thresh, None, iterations=2)
            c = VideoUtils.get_best_contour(thresh.copy(),5000)
            if c is not None:
                (x, y, w, h) = cv2.boundingRect(c)
                cv2.rectangle(frame, (x, y), (x + w, y + h), (0, 255, 0), 2)
                callback(c, frame)
            if show_video:
                cv2.imshow("Security Feed", frame)
                key = cv2.waitKey(1) & 0xFF
                if key == ord("q"):
                    break
        camera.release()
        cv2.destroyAllWindows()
    @staticmethod
    def get_best_contour(imgmask, threshold):
        im, contours, hierarchy = cv2.findContours(imgmask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        best_area = threshold
        best_cnt = None
        for cnt in contours:
            area = cv2.contourArea(cnt)
            if area > best_area:
                best_area = area
                best_cnt = cnt
        return best_cnt
class Turret(object):
    def __init__(self, friendly_mode=True):
        GPIO.output(14,True)
        GPIO.output(15,False)
        sleep(0.1)
        GPIO.output(14,True)
        GPIO.output(15,True)
        sleep(0.5)
        GPIO.output(14,False)
        GPIO.output(15,False)
        sleep(0.1)
        GPIO.output(14,True)
        GPIO.output(15,True)
        print("test inicial")
    def motion_detection(self,show_video=False):
        VideoUtils.find_motion(self.__move_axis, show_video=show_video)
    def __move_axis(self, contour,frame):
        (v_h, v_w) = frame.shape[:2]
        (x, y, w, h) = cv2.boundingRect(contour)
        print "x: %s, %s" % (str(x),str(w))
if __name__ == "__main__":
    t = Turret(friendly_mode = False)
    user_input = raw_input("Presion 1 ")
    if user_input == "1":
        if raw_input("Ver video? (y, n)\n").lower() == "y":
            t.motion_detection(show_video=True)
        else:
            t.motion_detection()
    else:
        print ("presion 1 ptm")
print ("success")
sleep(0.5)
GPIO.output(14,True)
sleep(0.5)
GPIO.output(14,False)
print ("shit")
