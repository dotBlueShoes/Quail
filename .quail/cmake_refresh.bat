@echo off

@REM
@REM	VALIDATION FOR ARGUMENTS NEEDED!
@REM

set "project_dir=%~1"
set "vcvarsall_dir=%~2"

@REM
@REM	Compile architectures.
@REM

set "p1=x64-win-debug"
set "p2=x64-win-release"

@REM Calling vcvarsall.bat with architecture as argument
call "%vcvarsall_dir%/vcvarsall.bat" amd64

@REM Calling cmake to create / refresh its preset configuration.

@REM Calling for each profile
call "cmake" --preset %p1% -S "%project_dir%"
call "cmake" --preset %p2% -S "%project_dir%"

goto success

:error

echo "Wrong arguments used!"

:success
