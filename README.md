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
./Viz3D
```

## Benchmark

On my macbook pro 2013:
* 100k_cluster_label: 60-70 fps (for 100k points!)

## TODO

* Bug: sometimes the datasets list is empty
* Document usage
* Travis integration
	* Need to modify CMakeLists.txt among other things
* Code cleanup/refactoring
* Key to inverse colors
* Screenshots

* Add a demo video
* Android version
