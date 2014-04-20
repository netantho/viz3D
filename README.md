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

* Document usage
* Have it working
* Android version
* Travis integration
	* Need to modify CMakeLists.txt among other things
