@ECHO ON

set path=Validation\
set pathValidation=VT\
set result=result\

call ExecuteClear.bat
md %path%%pathValidation%%result%
copy *.py %path%%pathValidation%
cd %path%%pathValidation%

MainWimosWindows.py VT > %result%ResultUTs.log