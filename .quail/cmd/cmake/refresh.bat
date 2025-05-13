@echo off

@REM
@REM	VALIDATION FOR ARGUMENTS NEEDED!
@REM

set "project_dir=%~1"
set "vcvarsall=%~2"

@REM
@REM	Compile architectures.
@REM

set "p1=x64-win-debug"
set "p2=x64-win-release"

@REM
@REM Replaces '`' character with `"` character.
@REM  It's a good practise for passed strings because:
@REM  - Those strings might be passed from other shells (different quote escaping systems)
@REM  Especially when dealing with a string that contains a `quote escape`, spaces like 
@REM  the following where we have a program path and it's argument where program path can
@REM  have spaces and has to be surrounded with quotes that do not interfere with the argument.
@REM
set "vcvarsall=%vcvarsall:`="%"

@REM
@REM Calling vcvarsall.bat with architecture as argument
@REM
call %vcvarsall%

@REM
@REM Calling cmake to create / refresh its preset configuration.
@REM - Calling for each profile
@REM
call "cmake" --preset %p1% -S "%project_dir%"
call "cmake" --preset %p2% -S "%project_dir%"

goto success

:error

echo "Wrong arguments used!"

:success
