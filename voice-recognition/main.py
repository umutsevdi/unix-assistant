import speech_recognition as sr
import time
import pyaudio
import wave
import sounddevice as sd
import RPi.GPIO as GPIO
from scipy.io.wavfile import write
import socket
import numpy as np
# Set up the Raspberry Pi's GPIO pins
GPIO.setmode(GPIO.BOARD)
GPIO.setup(7, GPIO.IN, pull_up_down=GPIO.PUD_UP)

print("TEST")
# Set the recording parameters
samplerate = 44100
duration = 5
filename = "recording.wav"
IP = socket.gethostbyname(socket.gethostname())

PORT = 8081

print("BEFORE SOCKET")
# create a socket instance
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
# connect to the server
print("SOCKET CONNECT")
sock.connect((IP, PORT))
r = sr.Recognizer()


# Create a callback function that will be called every time the button is pressed
def record_audio():
  print("RECORDING")
  data = sd.rec(samplerate * duration, samplerate=samplerate, channels=1, blocking=True)
  print("RECORDING DONE")
  abs_max= 2**16 -1 
  data=  ( data * abs_max + abs_max).clip(0,abs_max).astype(np.int16)

  write(filename, samplerate, data)
  print("WRITING DONE")
  time.sleep(2)
  print("WAKE UP")


directory = {
    #komutlar
    
    "list":"ls ",
    "remove":"rm ",
    "make_directory":"mkdir ",
    "process":"ps ",
    "touch":"touch ",
    "echo":"echo ",
    #parameters

    "recursive":"-r ",
    "verbose":"-v ", 
    "help":"-h ",
    "force":"-f ",
    
}
    

def text_processor(string):
    
    new_str = ""
    output = ""
    speacial_cases = ["change","make"]
    for word in string.split():
        new_str+=word
        if word in speacial_cases:
            new_str+="_"  
        else:
            new_str+=" "  


    for word in new_str.split():

        if word in directory:
            word = directory[word]
        output += word
    return output
    


while True:
    if True: # not GPIO.input(7):
        record_audio()
        with sr.WavFile("recording.wav") as source:             
            audio = r.record(source)            

        try:
            text = r.recognize_google(audio)
            print(text)
            message = text_processor(text)
            print("{"+message+"}")
            if len(message) > 0:
                sock.send(message.encode())
        except sr.UnknownValueError:
            print("Sorry, I didn't catch that.")
        except sr.RequestError as e:
            print(f"Sorry, there was an error: {e}")
    break


