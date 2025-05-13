@echo off

@REM
@REM	VALIDATION FOR ARGUMENTS NEEDED!
@REM

set "project_dir=%~1"
set "vcvarsall_dir=%~2"
set "profile=%~3"

echo %profile%

@REM Calling vcvarsall.bat with architecture as argument, We're silencing it's output with '> nul'.
call "%vcvarsall_dir%/vcvarsall.bat" amd64 > nul
call "cmake" --build "%project_dir%\build\%profile%"
@REM }

goto success

:error

echo "Wrong arguments used!"

:success
