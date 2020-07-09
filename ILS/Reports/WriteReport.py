import glob

files = []
for file in glob.glob("*.sol"):
    files.append(file)
    
print("Files found: ")
files.sort()
for file in files:
    print(file)
    
    
print(" ")    
print("Reading Files...")

results = list()

dec_origin = "."
dec_target = ","

for file in files:
    fileData = list()

    fileData.append(file.split(".")[0])

    f = open(file,"r")
    
    costLine = f.readline()
    costSplit = costLine.split(": ")
    if costLine == "[INFEASIBLE]" or len(costSplit) < 2:
        fileData.append(-1)
        fileData.append(0)
        fileData.append(0)
    else:
        cost = costSplit[1]
        fileData.append(cost[:-1].replace(dec_origin,dec_target))
        
        vehLine = f.readline()
        veh = vehLine.split(": ")[1]
        fileData.append(veh[:-1].replace(dec_origin,dec_target))
        
        timeLine = f.readline()
        time = timeLine.split(": ")[1]
        fileData.append(time[:-1].replace(dec_origin,dec_target))
    
    results.append(fileData)
    
    f.close()


print("Writing Report File...")

f = open("report.csv","w")
for result in results:
    for col in result:
        f.write(str(col))
        f.write(";")
    f.write("\n")
f.close()

print("Done!")