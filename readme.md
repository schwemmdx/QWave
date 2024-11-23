## QWave - Data Visualizer
The purpose of this software is primarily dispaying of 
measurement data aquired from oscilloscopes from the rigol family.
The main data type supported is .csv, because of its simplicity. But future additions for various datatypea 
is planned and development ongoing.

### Building
1. Prerequisites 
- Qt6 libraries 
- Python (if you want to update resources automatically from the materials content)
- CMake 
- GCC
- Make
2. Build Process
'''
mkdir build && cd build && cmake .. && make -j$nproc
'''
Thats it. 

### Functionality and Planned Features
- [x] Reading .csv files, where every colum is treated as a vector of data. The first cell in every colum is the signals name, the second row is the hnit of the signal.
-[] 