@ECHO OFF

echo git fetch origin
git fetch origin
echo git checkout -B master origin/master
git checkout -B master origin/master

echo:
echo Building project...
echo:

build.bat