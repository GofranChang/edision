rm -rf build_ios
mkdir build_ios
cd build_ios

cmake ../ \
 -G Xcode \
 -DCMAKE_TOOLCHAIN_FILE=../deps/iOS/ios.toolchain.cmake \
 -DPLATFORM=OS \
 -DIOS_PLATFORM=OS \
 -DCMAKE_IOS_DEVELOPER_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/
