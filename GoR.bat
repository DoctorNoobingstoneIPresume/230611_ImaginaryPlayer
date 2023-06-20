@echo off
call "Go.bat"
if errorlevel 1 goto :EOF
call "Run.bat" %*
