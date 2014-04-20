mkdir -p Build
cd Build
export URHO3D_HOME=../Libs/Urho3D
cmake ../Source -DURHO3D_64BIT=1
make
cd ../Bin
cp -rf ../Libs/Urho3D/Bin/CoreData .
cp -rf ../Libs/Urho3D/Bin/Data .
mkdir -p Data/datasets
cp -rf ../Samples/Datasets/* Data/Datasets
cp -rf ../Samples/Materials/* Data/Materials
cp -rf ../Samples/Textures/* Data/Textures
