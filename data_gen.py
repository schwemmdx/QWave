import numpy as np 


x = np.geomspace(1e-3,1e2,num=int(1e4))

y1 = np.sin(x*np.pi*2)
y2 = np.cos(x)
y3 = np.tan(x)
y4 = np.sin(x)*np.cos(x)
y5 = x**2

with open("data/large_set.csv","w") as pcsv:
    pcsv.write("time,y1,y2,y3,y4,y5\ns,a,b,c,d,e\n")
    for xi,a1,a2,a3,a4,a5 in zip(x,y1,y2,y3,y4,y5):
        pcsv.write(f"{xi},{a1},{a2},{a3},{a4},{a5}\n")


        