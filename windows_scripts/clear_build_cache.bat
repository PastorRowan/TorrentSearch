
:: clears CMake build cache
:: useful to do clean builds and runs

pushd "%~dp0.."

rmdir /s /q build

popd
