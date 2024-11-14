import numpy as np 


x = np.geomspace(1e-3,1e2,num=1000)

y = np.sin(x*np.pi*2);

with open("data/gen2.csv","w") as pcsv:
    pcsv.write("x,y\ns,y\n")
    for xi,yi in zip(x,y):
        pcsv.write(f"{xi},{yi}\n")