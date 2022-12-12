@ECHO OFF

rem -- 7zip checker --
if not exist "C:\Program Files\7-Zip\7z.exe" (
    echo 7zip is required to run this batch file. Install 7zip.
    pause
    exit
)

ECHO Downloading files...
powershell -Command "(New-Object Net.WebClient).DownloadFile('https://lightact.io/uploads/swDevRepository/cinder-blocks.zip', 'cinder-blocks.zip')"

ECHO Unpacking files...
"C:\Program Files\7-Zip\7z.exe" x cinder-blocks.zip 

del cinder-blocks.zip
ECHO Done!
pause