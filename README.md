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

## Usage

Dataset format:

```
x\ty\tz\tcluster\tlabel
0.000000	0.000000	0.000000	0	My awesome label here!
```
The `cluster` and `label` columns are optional.
See `Samples/Datasets` for more examples

When your dataset is ready, copy it with a `.tsv` extension in `Bin/Data/Datasets`.

Enjoy!

## Benchmark

On my macbook pro 2013:
* 100k_cluster_label: 60-70 fps (for 100k points!)

## TODO

* Screenshots

* Bug: sometimes the datasets list is empty
* Travis integration
	* Need to modify CMakeLists.txt among other things
* Key to inverse colors
* Add a demo video
* Android version
