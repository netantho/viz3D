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

* url -> label
* Dataset items can have a label and/or a cluster or not
* Change executable name to viz3D
* More samples, some with labels, some without
* Document usage
* Android version
* Travis integration
	* Need to modify CMakeLists.txt among other things
* Code cleanup/refactoring
* Key to inverse colors
* Add a demo video
