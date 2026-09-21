
:: Deploys the application

pushd "%~dp0.."
pushd build

"C:\Qt\6.11.1\mingw_64\bin\windeployqt.exe" TorrentSearch.exe

popd
popd
