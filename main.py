# Last Updated: 2022-03-16
# Coded by: Andersen Ball, Johnathan Sorrentino

# This code retrieves status updates from Elite Dangerous and sends them to a device using a serial port.
import sched, time
import json     # Needed to read the Status.json file
import serial	# Needed for serial communication with Arduino
import os       # Needed for file size checking

def readStatus():
    # Open the file, store the information to a variable, then close the file
    file_location = "C:\\Users\\Johnathan\\Saved Games\\Frontier Developments\\Elite Dangerous\\Status.json"    

    f = open(file_location)
    statusContent = f.read()
    f.close()
    
    # Save the filetype using its json format
    if (statusContent == ''):
        #print("\n\n*************Error Reading File***************\n\n")
        return
    else:
        info = json.loads(statusContent)    
        write_ard(str(info['Flags']&4))
    
	
    
    
arduino = serial.Serial(port = 'COM6', baudrate = 115200, timeout = .1)
def write_ard(x):
	arduino.write(bytes(x, 'utf-8'))
	
def read_ard():
	data = arduino.readline().decode('utf-8').rstrip()
	return data




s = sched.scheduler(time.time, time.sleep)  # create scheduler
def loop():
  
    readStatus();
    s.enter(0.02, 1, loop);             #calls its self after waiting 1 seconds

def main():
    ## connect to port
    
    time.sleep(1)
    print("Code Starting...")
    #num1 = input("Enter a number: ")
    s.enter(0.02, 1, loop);           
    s.run(); 

main();