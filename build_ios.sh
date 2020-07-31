rm -rf build_ios
mkdir build_ios
cd build_ios

cmake ../ \
 -G Xcode \
 -DCMAKE_TOOLCHAIN_FILE=../deps/iOS/ios.toolchain.cmake \
 -DPLATFORM=OS \
 -DIOS_PLATFORM=OS \
 -DBUILD_ARM64=1 \
 -DCMAKE_IOS_DEVELOPER_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/ \
 -DCMAKE_IOS_SDK_ROOT=/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneOS.platform/Developer/SDKs/iPhoneOS.sdk/
