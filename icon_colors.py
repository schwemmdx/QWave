from genericpath import isdir

from os import listdir, mkdir
from os.path import isfile, join

import shutil




color = "\"#a93a07\""
targetPath = "./icons"

onlyfiles = [f for f in listdir(targetPath) if isfile(join(targetPath, f))]
if isdir(targetPath+"/modified"):
    shutil.rmtree(targetPath+"/modified")

mkdir(targetPath+"/modified")
modified = None
for file in onlyfiles:
    with open(targetPath+"/"+file,"r") as svg:
        svgLines = svg.read().splitlines(keepends=True)
        buffer = str()

        for ln in svgLines:
            if "fill=" in ln:
                i = ln.find("fill=")
                ln = ln[:i]+f"fill={color} "+ln[i+len(color)+6:]

            elif ln.startswith("<path ") and not "fill=" in ln:
                ln = f"<path fill={color} " +ln[6:]

            elif ln.startswith("<circle "):
                ln = f"<circle fill={color} " +ln[7:]
            
            elif ln.startswith("<ellipse "):
                ln = f"<ellipse fill={color} " +ln[8:]

            buffer+=ln

        with open(targetPath+"/modified/"+file,"w") as mod:
            mod.write(buffer)
    