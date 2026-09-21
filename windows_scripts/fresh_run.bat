
:: Runs a clean build by following these steps:
:: 1. Clears CMake build cache
:: 2. generates build
:: 3. executes build
:: 4. runs application

call "%~dp0clear_build_cache.bat"
call "%~dp0generate_build.bat"
call "%~dp0execute_build.bat"
call "%~dp0deploy.bat"
call "%~dp0run.bat"
