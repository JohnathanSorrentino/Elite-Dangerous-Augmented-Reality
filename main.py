# Last Updated: 2021-12-08
# Coded by: Johnathan Sorrentino

# This code retrieves status updates from Elite Dangerous and sends them to a device using a serial port.

import json     # Needed to read the Status.json file

# Read a file
f = open("C:\\Users\\johna\\Saved Games\\Frontier Developments\\Elite Dangerous\\Status.json")
statusContent = f.read()
aList = json.loads(statusContent)

print(aList)
# Closing file
f.close()