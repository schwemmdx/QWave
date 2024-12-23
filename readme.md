
![Qt](https://img.shields.io/badge/Qt-%23217346.svg?style=for-the-badge&logo=Qt&logoColor=white)  ![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black) ![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white) 
![Cpp](https://img.shields.io/badge/Cpp)



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
Im developing on a linux machine only, so this is basically all working out of the box.
```
mkdir build && cd build && cmake .. && make -j$nproc
```
Thats it. 
### Running
- For Linux machines just run the executable
- For Win-devil, usually you would need the tool provided by qt to generate the required dynamic libraries. This is included in the 'CMakeLists.txt'. It searches for the required libraries (I just hardcoded them :D ) Please be aware, that you need to change/or delete the path to your Qt Libraries (in my case i had to, because it didnt worked as expected.
```
 if(WIN32)
    set(Qt6_DIR C:/Qt/6.8.0/mingw_64/lib/cmake/Qt6)
endif()
```
If on windows the libraries are not found in the build directory, CMake will evoke the command to generate them
```
windeployqt path/to/QWave.exe
```
### Overview
![image](https://github.com/user-attachments/assets/224410fd-6fee-4267-89ec-e9b06a3622f8)
The main window of the application. Files are represented as _cyan_ colored nodes, their schildren are datasets. Each Dataset holds also calculated statistical information. For __every__ file, the x vector to plot the data against needs to be specified. Clicking on a valid node (internally holding the data) the data is plotted on the chart. 
![image](https://github.com/user-attachments/assets/d9a9abab-528d-46da-a848-775092bd8e1e)


#### Markers and Inspection
Upon hitting'#' and moving around in the Chart, the cursor changes to a crosshair with highlighted x,y coordinates at the axes of the chart.
![image](https://github.com/user-attachments/assets/08662f09-8d34-4ce6-96ff-1f7ad5fd8562)
This is useful in case of manual inspection and singal integrity checks. In this mode by _LEFT_MOUSE_ a marker is placed at the current _X-Position_. This marker pieces trough every series that is present on the chart and displaying their y-values at the current x-position of the marker. 
![image](https://github.com/user-attachments/assets/2a941f08-d21d-44d0-97b0-1b28bdbf260f)

### Functionality and Planned Features
- [x] Reading .csv files, where every colum is treated as a vector of data. The first cell in every colum is the signals name, the second row is the hnit of the signal.
- [x] Simple Add/Remove functionality to quickly examine data
- [x] Various zooming and panning functionality  
- [x] Taking a snapshot of the chart (Ctrl+P) exports a direct *.png
- [ ] Exporting the chart with its data to an svg
- [ ] Exporting the chart to native Latex
- [ ] Exporting to matplotlib figure (generating .py file)
- [ ] Settings for chart appearance
- [ ] Various export formats like *.hdf5, *.csv,*.json, *.npy ...
- [ ] Various other import formats like *.hdf5, json...
- [ ] Selection of data points to fit to a physical model
- [ ] Settings-file with load and save + apply
- [ ] Settings for chart export
And possibly some more :)

### Contribution
Oh, hell yeah. Please :)


