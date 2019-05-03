import csv
import os
import sys

path = sys.path[0]

with open(path + '\\AVInput.csv', 'rt', encoding="utf-8-sig",newline='') as infile:
    reader = csv.reader(infile, delimiter=',')
    headers = next(reader)
    print (headers)
    #for row in reader:
    #    print(row)

    row = next(reader)
    distance = row[0]
    speed = row[1]

    if distance < 25:
        targetSpeed = speed
        duration = 5


    targetSpeed = 45


myData = [['TargetVelocity','DecelTime'],[targetSpeed,duration]]


with open(path + '\\AVOutput.csv', 'w', encoding="utf-8-sig", newline='') as outfile:
    writer = csv.writer(outfile)
    for row in myData:
        writer.writerow(row)

