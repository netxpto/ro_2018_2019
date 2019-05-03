
fileToTex=open("toTex.txt",'w')

for i in range(1,100,1):
    f=open("midreport"+str(i)+".txt", "r")
    lines=f.readlines()
    ber=float(lines[0][5:])
    ub=float(lines[2][13:])
    lb=float(lines[3][13:])
    rb=float(lines[4][25:])
    f.close()
    fileToTex.write("("+str(rb)+","+str(ber)+") +=(0,"+str(ub-ber)+") -=(0,"+str(ber-lb)+")\n")
fileToTex.close()
            
            
            
            
            
