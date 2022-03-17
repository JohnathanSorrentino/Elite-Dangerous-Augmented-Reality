# Last Updated: 2022-03-16
# Coded by: Andersen Ball, Johnathan Sorrentino

# This code retrieves status updates from Elite Dangerous and sends them to a device using a serial port.
import sched, time
import json     # Needed to read the Status.json file
import serial	# Needed for serial communication with Arduino


def checkFlag(flag,flagToCheck):
    return flag & flagToCheck == flagToCheck


def checkFlagChange(flag,prevflag,flagToCheck): #if flag was previous false and is now true return true.
    if checkFlag(flag,flagToCheck) and False ==checkFlag(prevflag,flagToCheck):
        return True
    else:
        return False
    

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
    # Open the file, store the information to a variable, then close the file
    f = open("C:\\Users\\johna\\Saved Games\\Frontier Developments\\Elite Dangerous\\Status.json")
    statusContent = f.read()
    f.close()
    
    # Save the filetype using its json format
    print("\tloaded json:",statusContent)
    info = json.loads(statusContent)    
    print("\tflags active:" + bin(info['Flags'])) #if a 1 is present, there is a flag there. ex. 1010 shows flags at 8, 2
    viewActiveFlags(info['Flags'])
    
    
arduino = serial.Serial(port = 'COM5', baudrate = 115200, timeout = .1)
def write_ard(x):
	arduino.write(x)
	
def read_ard():
	data = arduino.readline().decode('utf-8').rstrip()
	return data

s = sched.scheduler(time.time, time.sleep)  # create scheduler
def loop(num):
    print("*****************\nchecking json...");
    print(time.time());
    
    readStatus();
    print("The number is")
    print(num);
    write_ard(num);
    time.sleep(0.1);
    number = read_ard();
    
    print("number from the arduino:")
    print(number)
    
    num1 = int(input())
    s.enter(1, 1, loop, argument = (num1,));             #calls its self after waiting 1 seconds

def main():
    ## connect to port
    
    time.sleep(1)
   
    num1 = int(input())
    s.enter(1, 1, loop, argument = (num1,));           
    s.run(); 

main();