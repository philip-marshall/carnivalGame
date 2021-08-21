import logging
import asyncio
import platform
import ast

from bleak import BleakClient
from bleak import BleakScanner
from bleak import discover

import serial
import time
from tkinter import *

import pygame

pygame.mixer.init()

RELAY_UUID = '13012F01-F8C3-4F4A-A8F4-15CD926DA146'


relay_on_value = bytearray([0x01])

arduino = serial.Serial(port='/dev/ttyACM0', baudrate=9600, timeout=.1)
serialValue = 0
serialArray = {"", ""}
serialInt = 0

class PythonCarnival:
    def __init__(self,master, labelText, totalText, youWinLoseText):  
        self.master = master 
        master.title("Python Hub")
        #master.iconbitmap("pythonHub_icon.ico")
        master.geometry("800x500")  

        labelText.set("0")
        totalText.set("0")

        self.timertext = DoubleVar()
        
        self.timertext.set(0)
            
        display = Label(master, textvariable = self.timertext, font= ("Calibri", 25), background= "gainsboro").place(x = 200, y = 150)
        self.timeit=False

        scoreVal = Label(master, 
                        textvariable=labelText, font=("Calibri", 25), background= "gainsboro").place(x = 200,
                                                y = 60)
        scoreLabel1 = Label(master, 
                        text = "Score:", font=("Calibri", 25), background= "gainsboro").place(x = 40,
                                                y = 60)
        scoreLabel2 = Label(master, 
                        text = "out of ", font=("Calibri", 20), background= "gainsboro").place(x = 300,
                                                y = 60)
        totalVal = Label(master, 
                        textvariable=totalText, font=("Calibri", 25), background= "gainsboro").place(x = 450,
                                                y = 60)

        timerLabel = Label(master, 
                        text= "Time:", font=("Calibri", 25), background= "gainsboro").place(x = 40,
                                                y = 150)

        winLoseText = Label(master, 
                        textvariable=youWinLoseText, font=("Calibri", 100), background= "gainsboro").place(x = 20,
                                                y = 250)

        master.configure(background= "gainsboro")

    def decrement_timer(self):
        ctr=int(self.timertext.get())
        self.timertext.set(ctr-1)
        if (self.timertext.get() == 0):
            arduino.write(bytes("9", 'utf-8'))
            self.timertext.set(30)
            labelText.set(0)
        elif self.timeit:
            print("move\n")
            arduino.write(bytes("2", 'utf-8'))
            self.master.after(1000, self.decrement_timer)

    def start(self, difficulty):
        self.timeit=True
        print("setting")
        print(str(difficulty))
        if (difficulty == 1):
            self.timertext.set(30)
        elif (difficulty == 2):
            self.timertext.set(25)
        elif (difficulty == 3):
            self.timertext.set(20)

        self.decrement_timer()

    def quitit(self):
        self.timeit=False

root = Tk()
labelText = StringVar()
totalText = StringVar()
youWinLoseText = StringVar()
app = PythonCarnival(root, labelText, totalText, youWinLoseText)

async def shock(client):
    await client.write_gatt_char(RELAY_UUID, relay_on_value)

async def run():
    print('big cringe peripheral service for ya booiiiiii')
    print('looking for peripheral shocker...')
    address = ''
    found = False
    devices = await discover()
    for d in devices:       
        if 'Arduino'in d.name:
            print('Found Arduino Nano 33 BLE Sense Peripheral')
            found = True
            address = d.address
    if (address != ''):
        async with BleakClient(address) as client:
            print(f'Connected to {d.address}')

            while True:
                if (arduino.inWaiting()):

                    # clean up the serial data and turn into and array where array[0] = serial type and array[1] = data
                    serialValue = str(arduino.readline())
                    serialValue = serialValue.replace("b'", "")
                    serialValue = serialValue.replace("'", "")
                    serialValue = serialValue.replace("\\r\\n", "")
                    serialArray = serialValue.split(",")
                    print(serialArray[0])
                    print(serialValue)

                    youWinLoseText.set("")

                    # evaluate serial type and act accordingly
                    if (serialArray[0] == "score"):
                        serialInt = int(serialArray[1])
                        pygame.mixer.music.load("/home/pi/Desktop/carnivalGame/piManager/ding.wav")
                        pygame.mixer.music.play()
                        labelText.set(str(serialInt))
                        print(serialInt)
                    elif (serialArray[0] == "final"):
                        serialInt = int (serialArray[1])
                        if (str(serialInt) >= totalText.get()):
                            print("you win!!!")
                            youWinLoseText.set("You WIN!!")
                            pygame.mixer.music.load("/home/pi/Desktop/carnivalGame/piManager/win.wav")
                            pygame.mixer.music.play()
                        else:
                            print("bluetooth timeeeeee")
                            pygame.mixer.music.load("/home/pi/Desktop/carnivalGame/piManager/lose.wav")
                            pygame.mixer.music.play()
                            await shock(client)
                            youWinLoseText.set("You LOSE!!")
                    elif (str(serialArray[0]) == "difficulty"):
                        print("in start")
                        youWinLoseText.set("")
                        difficulty = int(serialArray[1])
                        print("d = " + str(difficulty))
                        if (difficulty == 1):
                            totalText.set("15")
                        elif (difficulty == 2):
                            totalText.set("20")
                        else:
                            totalText.set("25")
                        app.start(difficulty)

                    time.sleep(0.05)

                root.update_idletasks()
                root.update()

    if not found:
        print('peripheral didnt get found :(')

loop = asyncio.get_event_loop()
try:
    loop.run_until_complete(run())
except KeyboardInterrupt:
    print('\nReceived Keyboard Interrupt')
finally:
    print('Program finished')

