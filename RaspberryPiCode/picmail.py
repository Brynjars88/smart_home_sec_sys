#!/usr/bin/python3

import time
from datetime import datetime
import picamera
import smtplib
from smtplib import SMTP, SMTPException
from email.mime.image import MIMEImage
from email.mime.multipart import MIMEMultipart
import RPi.GPIO as GPIO

# Configure PIN 13 on the Raspberry Pi Zero W to be input
PIN = 13
GPIO.setmode(GPIO.BCM)
GPIO.setup(PIN, GPIO.IN)

def take_picture():
	with picamera.PiCamera() as camera:
		camera.resolution = (1024, 768)
		camera.start_preview()
		# Camera warm-up time
		time.sleep(2)
		camera.capture('photo.jpg')

def send_email():
	f_time = datetime.now().strftime('%a %d %b @ %H:%M')

	toaddr = 'iotsecsys@gmail.com' 
	me = 'iotsecsys@gmail.com'
	subject = 'Motion detected: ' + f_time

	msg = MIMEMultipart()
	msg['Subject'] = subject
	msg['From'] = me
	msg['To'] = toaddr
	msg.preamble = "Photo @ " + f_time

	fp = open('photo.jpg', 'rb')
	img = MIMEImage(fp.read())
	fp.close()
	msg.attach(img)

	try:
		s = smtplib.SMTP('smtp.gmail.com',587)
		s.ehlo()
		s.starttls()
		s.ehlo()
		s.login(user = 'iotsecsys', password = 'qathkaubnqvfjksw')
		s.send_message(msg)
		s.sendmail(me, toaddr, msg.as_string())
		s.quit()
	except SMTPException as e:
		print("Error: unable to send email : {err}".format(err=e))

def main():
	while(True):
		if GPIO.input(PIN):
			take_picture()
			send_email()
			time.sleep(2)

if __name__ == '__main__':
	main()
