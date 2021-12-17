# Last Updated: 2021-12-08
# Coded by: Johnathan Sorrentino

# This code retrieves status updates from Elite Dangerous and sends them to a device using a serial port.

import json     # Needed to read the Status.json file

def checkFlag(flag,flagToCheck):
    return flag & flagToCheck == flagToCheck

# Read a file
f = open("C:\\Users\\Andersen B\\Saved Games\\Frontier Developments\\Elite Dangerous\\Status.json")
statusContent = f.read()
# Closing file
f.close()

info = json.loads(statusContent)
landingGearFlag = 4

print(info['Flags'])
print(hex(info['Flags']))
print(checkFlag(info['Flags'],landingGearFlag))
