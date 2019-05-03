
fileToTex=open("toTex.txt",'w')

for i in {'-14','-15','-16','-17'}:
    for j in [1,2,3,4,5,6,7,8,9]:
        if (i == '-14' and (j==1 or j==2)) or i=='-15' or i=='-16' or (i=='-17' and j!=1):
            f=open("BER_"+str(j)+"E"+i+".txt", "r")
            lines=f.readlines()
            ber=float(lines[0][5:])
            ub=float(lines[2][13:])
            lb=float(lines[3][13:])
            f.close()
            fileToTex.write("("+str(j)+"e"+i+","+str(ber)+") +=(0,"+str(ub-ber)+") -=(0,"+str(ber-lb)+")\n")
fileToTex.close()
            
            
            
            
            
