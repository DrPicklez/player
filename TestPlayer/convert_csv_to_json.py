import csv
import json

csv_file = open('Video_SM.csv', 'r')
json_file = open('AnimationClips.json', 'w')

csv_reader = csv.reader(csv_file)
data = []

def calcPosition(_name):
    position = 0
    transition = False
    name = _name.split('_')
    l = "Left"
    f = "Front"
    r = "Right"
    t = "To"
    if(name[0] == l):
        position = 0
    if(name[0] == f):
        position = 1
    if(name[0] == r):
        position = 2
    if(name[1] == t):
        transition = True
        if(name[2] == l):
            targetPosition = 0
        if(name[2] == f):
            targetPosition = 1
        if(name[2] == r):
            targetPosition = 2
    else:
        targetPosition = position

    return position, transition, targetPosition

#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

for row in csv_reader:
    _name =     row[0]  # Extract first value/column
    _startTime = int(row[1])  # Extract second value/column
    _endTime =  int(row[2])  # Extract third value/column
    _length = _endTime - _startTime
    _position, _isTransition, _targetPosition = calcPosition(_name)


    # Create a dictionary for the extracted values
    item = {"Name": _name, "startTime": _startTime, "endTime": _endTime, "length": _length, "position": _position, "targetPosition": _targetPosition, "isTransition": _isTransition}
    data.append(item)


json.dump(data, json_file, indent=4)

csv_file.close()
json_file.close()
