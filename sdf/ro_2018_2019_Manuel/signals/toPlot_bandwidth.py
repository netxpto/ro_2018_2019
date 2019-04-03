
fileToTex=open("toTex_bw.txt",'w')

for i in range(10,160,10):
    f=open("BER__"+str(i)+".txt", "r")
    lines=f.readlines()
    ber=float(lines[0][5:])
    ub=float(lines[2][13:])
    lb=float(lines[3][13:])
    f.close()
    fileToTex.write("("+str(i/100.0/.6)+","+str(ber)+") +=(0,"+str(ub-ber)+") -=(0,"+str(ber-lb)+")\n")
fileToTex.close()
            
            
            
            
            
