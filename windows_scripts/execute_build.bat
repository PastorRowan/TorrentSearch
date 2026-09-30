
:: Executes generated CMake build

pushd "%~dp0.."

cmake --build build --parallel

popd
