rm -rf build_ios
mkdir build_ios
cd build_ios

cmake ../ \
 -G Xcode \
 -DCMAKE_TOOLCHAIN_FILE=../deps/iOS/ios.toolchain.cmake \
 -DENABLE_BITCODE=false \
 -DPLATFORM=OS \
 -DBUILD_PLATFORM=iOS

#  -DCMAKE_BUILD_WITH_INSTALL_RPATH=TRUE
#  -DCMAKE_SKIP_RPATH=TRUE
#  -DIOS_PLATFORM=OS
#  -DCMAKE_IOS_DEVELOPER_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/
