rm -rf output
mkdir output
mkdir output/android

mkdir output/android/armeabi
mkdir output/android/armeabi-v7a
mkdir output/android/arm64-v8a
cd build
cmake ../ \
-DCMAKE_BUILD_TYPE=Release \
-DANDROID_ABI=armeabi \
-DANDROID_PLATFORM=android-16 \
-DANDROID_NDK=/Users/zhanggaofan/Documents/workspace/tool/android-ndk-r16b/ \
-DCMAKE_TOOLCHAIN_FILE=/Users/zhanggaofan/Documents/workspace/tool/android-ndk-r16b/build/cmake/android.toolchain.cmake \
-DANDROID_TOOLCHAIN=clang .. \
-DLIBRARY_OUTPUT_PATH=../output/android/armeabi
make

rm -rf ./*
cmake ../ \
-DCMAKE_BUILD_TYPE=Release \
-DANDROID_ABI=armeabi-v7a \
-DANDROID_PLATFORM=android-16 \
-DANDROID_NDK=/Users/zhanggaofan/Documents/workspace/tool/android-ndk-r16b/ \
-DCMAKE_TOOLCHAIN_FILE=/Users/zhanggaofan/Documents/workspace/tool/android-ndk-r16b/build/cmake/android.toolchain.cmake \
-DANDROID_TOOLCHAIN=clang .. \
-DLIBRARY_OUTPUT_PATH=../output/android/armeabi-v7a
make

rm -rf ./*
cmake ../ \
-DCMAKE_BUILD_TYPE=Release \
-DANDROID_ABI=arm64-v8a \
-DANDROID_PLATFORM=android-16 \
-DANDROID_NDK=/Users/zhanggaofan/Documents/workspace/tool/android-ndk-r16b/ \
-DCMAKE_TOOLCHAIN_FILE=/Users/zhanggaofan/Documents/workspace/tool/android-ndk-r16b/build/cmake/android.toolchain.cmake \
-DANDROID_TOOLCHAIN=clang .. \
-DLIBRARY_OUTPUT_PATH=../output/android/arm64-v8a
make
