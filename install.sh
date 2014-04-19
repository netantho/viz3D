mkdir -p Build
cd Build
export URHO3D_HOME=../Libs/Urho3D
cmake ../Source -DURHO3D_64BIT=1
make
cd ../Bin
ln -s ../Libs/Urho3D/Bin/CoreData .
ln -s ../Libs/Urho3D/Bin/Data .
