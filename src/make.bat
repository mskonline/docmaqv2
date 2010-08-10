@echo off
echo --- DocmaQ v2 Build ---
echo.

if not exist .\makefile goto build

echo --- Cleaning Workspace ---
mingw32-make distclean

:build
qmake
mingw32-make

if not %errorlevel% == 0 goto error
echo.
echo --- DocmaQ v2 built ---
echo. 
goto end

:error
echo.
echo --- Error Occured. Please check for all dependencies ---
echo.

:end
echo.
echo done
echo.

pause
