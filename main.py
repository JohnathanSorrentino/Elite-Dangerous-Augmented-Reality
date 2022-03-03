# Last Updated: 2022-03-2
# Coded by: Andersen Ball, Johnathan Sorrentino

# This code retrieves status updates from Elite Dangerous and sends them to a device using a serial port.
import sched, time
import json     # Needed to read the Status.json file
#import serial	# Needed for serial communication with Arduino
#import time		# Needed for internal delays

def write_ard(x,time_to_wait):
    print("send to ardino:",x)
    if time_to_wait != 0:
        time.sleep(time_to_wait)
	#arduino.write(bytes(x, 'utf-8'))


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
    
    if checkFlagChange(info['Flags'],StaticInfo.prev_info,8): #shields up
    	write_ard(1,3)
    elif checkFlagChange(info['Flags'],StaticInfo.prev_info,64):#hardpoints
        write_ard(2,2)
    elif checkFlagChange(info['Flags'],StaticInfo.prev_info,131072): #fsd charging 
        write_ard(3,6)
    elif checkFlagChange(info['Flags'],StaticInfo.prev_info,4194304): #  is in danger
        write_ard(4,5)
    else:
        write_ard(5,0)
    StaticInfo.prev_info = info['Flags']
    
    return True



class StaticInfo:
        prev_info = 0
        timesRun = 0


def main():
    def do_something(sc): 
        print("*****************\nchecking json...",StaticInfo.timesRun)
        StaticInfo.timesRun += 1
        if(readStatus()==False): #if reading status file return false, exit loop
            print("ending function")
            return
        else: 
            s.enter(.5, 1, do_something, (sc,))#calls its self after waiting 3 seconds
    
    ## connect to port
    #arduino = serial.Serial(port = 'COM3', baudrate = 115200, timeout = .1)
    time.sleep(1)
    
    # code to run every 3 seconds using an event 
    s = sched.scheduler(time.time, time.sleep) # create schedular
    s.enter(1, 1, do_something, (s,)) # 
    s.run()


if __name__=="__main__":
    main()





 
