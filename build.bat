:: Update submodules
git submodule update --init --recursive

:: Build Cinder
cd proj\vc2019
msbuild cinder.sln -t:cinder -property:Configuration=Debug
msbuild cinder.sln -t:cinder -property:Configuration=Release
cd ..\..

pause