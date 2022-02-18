# Last Updated: 2021-12-16
# Coded by: Andersen Ball, Johnathan Sorrentino

# This code retrieves status updates from Elite Dangerous and sends them to a device using a serial port.
import sched, time
import json     # Needed to read the Status.json file
import serial	# Needed for serial communication with Arduino
import time		# Needed for internal delays

def checkFlag(flag,flagToCheck):
    return flag & flagToCheck == flagToCheck

# shields up, fsd charging, is in danger/ being interdicted, over heating, masslocked, hardpoints deployed
def viewActiveFlags(flag): #lits all flags that are currently active
    flags = ["docked","landed","landing Gear Down","shields up", "super cruise", "flight assist off", "hardpoints deployed", "in wing","lights on",
             "cargo scoop deployed","silent running","scooping fuel","srv handbrake","srv using turret view","Srv turret retracted","srv drive assist",
             "fsd masslocked","fsd charging","fsd cooldown","low fuel <25%","over heating >100%","has lat long","is in danger","being interdicted",
             "in mainship","in fighter","in srv","hud in analysis mode","night vision","altitude from average radius","fsd jump","srv High Beam"]
    print("\tactive flags:")
    for i in range(len(flags)):
        if (checkFlag(flag,2**i)):
            print("\t",flags[i],":","flag",i)


def readStatus():
    # Read a file
    f = open("C:\\Users\\Andersen B\\Saved Games\\Frontier Developments\\Elite Dangerous\\Status.json")
    statusContent = f.read()
    # Closing file
    f.close()
    print("\tloaded json:",statusContent)
    info = json.loads(statusContent)
    if(len(info) == 3): #check if length of dic is 3. It is like this when game is turned off. use this to cancel function
        return False
    
    
    print("\tflags active:" + bin(info['Flags'])) #if a 1 is present, there is a flag there. ex. 1010 shows flags at 8, 2
    viewActiveFlags(info['Flags'])
    
    if checkFlag(info['flags'],8): #shields up
    	write_ard(1)
    elif checkFlag(info['flags'],64):#hardpoints
	write_ard(2)
    elif checkFlag(info['flags'],131072): #fsd charging 
	write_ard(3)
    elif checkFlag(info['flags'],4194304): #  is in danger
	write_ard(4)
    else:
	write_ard(5)
    
    #landingGearFlag = 4
    #print("\t check flag 4,landingGear:", checkFlag(info['Flags'],landingGearFlag))
    
    return True

## connect to port
arduino = serial.Serial(port = 'COM3', baudrate = 115200, timeout = .1)
time.sleep(1)

# code to run every 3 seconds using an event 
s = sched.scheduler(time.time, time.sleep) # create schedular
def do_something(sc): 
    print("*****************\nchecking json...")
    if(readStatus() == False): #if reading status file return false, exit loop
        print("ending function")
        return
    else: 
        s.enter(3, 1, do_something, (sc,))#calls its self after waiting 3 seconds

s.enter(1, 1, do_something, (s,)) # 
s.run()


def write_ard(x):
	arduino.write(bytes(x, 'utf-8'))
	
def write_read(x):
	arduino.write(bytes(x, 'utf-8'))
	time.sleep(0.1)
	data = arduino.readline().decode('utf-8').rstrip()
	return data
	
while True:
	num = input("Enter a number: ")
	value = write_read(num)
	print(value)
	



 
