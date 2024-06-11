import csv

N=256
lines=list()
with open('csv01.csv') as f:
    for line in csv.reader(f):
        lines.append(line)
        if len(lines) > N:
            lines.pop(0)