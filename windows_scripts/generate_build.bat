
:: Generates the CMake build files
:: Uses ninja as the build generator tool

pushd "%~dp0.."

cmake -S . -B build -G "Ninja" -DCMAKE_MAKE_PROGRAM="C:\ninja\ninja.exe" -DCMAKE_C_COMPILER="C:\msys64\mingw64\bin\gcc.exe" -DCMAKE_CXX_COMPILER="C:\msys64\mingw64\bin\g++.exe" -DCMAKE_PREFIX_PATH="C:\Qt\6.11.1\mingw_64"

popd
