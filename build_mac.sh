rm -rf build_mac
mkdir build_mac
cd build_mac

cmake ../ \
 -G Xcode \
 -DBUILD_PLATFORM=MAC
