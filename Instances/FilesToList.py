import glob

files = []
for file in glob.glob("*.vrp"):
    files.append(file)
    
print("Files found: ")
files.sort()
for file in files:
    print(file)
    
    
print(" ")    
print("Reading Files...")

fileNames = dict()

for file in files:
    fileData = list()

    fileName = file.split(".")[0]
    
    instanceType = fileName[0]
    instanceList = fileNames.get(instanceType)
    if instanceList == None:
        instanceList = list()
        fileNames[instanceType] = instanceList
    instanceList.append(fileName)


print("Writing Report File...")

f = open("vrplists.txt","w")
for instances in fileNames.values():
    
    firstFlag = True
    namesWrote = 0
    for instanceName in instances:
        if firstFlag == True:
            firstFlag = False
        else:
            f.write(",")
            
        if namesWrote >= 5:
            f.write("\n");
            namesWrote = 1
        else:
            namesWrote += 1
        
        f.write('"');
        f.write(str(instanceName));
        f.write('"');
        
    f.write("\n\n");
    
f.close()

print("Done!")