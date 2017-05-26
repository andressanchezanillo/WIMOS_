@ECHO ON

set path=Validation\
set pathValidation=VT\
set result=result\
set imagesPath=..\images

call ExecuteClear.bat
md %path%%pathValidation%%result%
copy *.py %path%%pathValidation%
echo %path%%pathValidation%%imagesPath%
echo %imagesPath%
start xcopy %imagesPath% %path%%pathValidation%%imagesPath% /E /C /I /Q /G /H /R /K /Y /Z /J
cd %path%%pathValidation%

MainWimosWindows.py VT > %result%ResultVT.log && %result%ResultUTs.log