@ECHO ON

set path=Validation\
set pathUTs=UTs\
set result=result\
set imagesPath=..\images

call ExecuteClear.bat
md %path%%pathUTs%%result%
copy *.py %path%%pathUTs%
start xcopy %imagesPath% %path%%pathUTs%%imagesPath% /E /C /I /Q /G /H /R /K /Y /Z /J
cd %path%%pathUTs%

MainWimosWindows.py UTs > %result%ResultUTs.log && %result%ResultUTs.log


