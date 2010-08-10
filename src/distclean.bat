@echo off

if not exist .\makefile goto is_clean

echo --- Cleaning Workspace ---
mingw32-make distclean
goto end

:is_clean
echo --- Workspace already clean ---

:end
echo done
echo.

pause

