# Viz3D

Visualize your points in 3D at scale!

Created by [Anthony Verez](http://www.anthony-verez.fr) ([@netantho](http://twitter.com/netantho)) and Emmanuel Chaboud (amadeus)

## Installation

Clone this repo:
```
git clone --recursive https://github.com/netantho/viz3D.git
```

Install Urho3D:
```
cd Libs/Urho3D
./cmake_gcc.sh -DURHO3D_64BIT=1 -DURHO3D_SAMPLES=1
cd Build
make
```

Install viz3D:
```
sh ./install.sh
cd Bin
./Main
```

## TODO

* Improve CMakeLists.txt based on https://github.com/urho3d/Urho3D/blob/master/Source/CMake/Modules/Urho3D-CMake-common.cmake
* Travis integration
