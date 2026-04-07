@echo off
setlocal EnableExtensions
title Taller Installer

for /f %%e in ('echo prompt $E^| cmd') do set "ESC=%%e"

set "C_RESET=%ESC%[0m"
set "C_BANNER=%ESC%[38;5;45m"
set "C_TITLE=%ESC%[38;5;45m"
set "C_TEXT=%ESC%[38;5;252m"
set "C_DIM=%ESC%[38;5;246m"
set "C_INFO=%ESC%[38;5;81m"
set "C_SUCCESS=%ESC%[38;5;51m"
set "C_WARNING=%ESC%[38;5;220m"
set "C_ERROR=%ESC%[38;5;203m"

set "DIST_OFX_PATH=dist\FilmNegInvert.ofx"
set "BUILD_OFX_PATH=out\build\x64-Release\lib\FilmNegInvert.ofx"
set "ICON_SOURCE_PATH=Resources\com.tallerneg.FilmNegInvert.png"
set "DEST_PLUGIN_PATH=C:\Program Files\Common Files\OFX\Plugins"
set "BUNDLE_NAME=FilmNegInvert.ofx.bundle"
set "BUNDLE_PATH=%DEST_PLUGIN_PATH%\%BUNDLE_NAME%"
set "SOURCE_OFX_PATH="

REM ===========================================
REM 1. PRIVILEGE CHECK
REM ===========================================

>nul 2>&1 "%SYSTEMROOT%\system32\cacls.exe" "%SYSTEMROOT%\system32\config\system"

if "%errorlevel%" NEQ "0" (
    cls
    call :print_banner
    echo %C_WARNING%Requesting administrative privileges...%C_RESET%
    echo.
    echo %C_DIM%The installer needs permission to write into Program Files.%C_RESET%
    goto UACPrompt
) else (
    goto gotAdmin
)

:UACPrompt
echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
set params=%*:"="
echo UAC.ShellExecute "cmd.exe", "/c %~s0 %params%", "", "runas", 1 >> "%temp%\getadmin.vbs"
"%temp%\getadmin.vbs"
del "%temp%\getadmin.vbs"
exit /B

:gotAdmin
if exist "%temp%\getadmin.vbs" del "%temp%\getadmin.vbs"
pushd "%CD%"
CD /D "%~dp0"

REM ===========================================
REM 2. INSTALL FLOW
REM ===========================================

cls
call :print_header

if exist "%DIST_OFX_PATH%" (
    set "SOURCE_OFX_PATH=%DIST_OFX_PATH%"
) else if exist "%BUILD_OFX_PATH%" (
    set "SOURCE_OFX_PATH=%BUILD_OFX_PATH%"
)

if not exist "%SOURCE_OFX_PATH%" (
    call :show_missing_binary_error
    goto end
)

echo %C_INFO%Using plugin binary:%C_RESET% %C_TEXT%%SOURCE_OFX_PATH%%C_RESET%
echo %C_INFO%Removing the previous version, if it exists...%C_RESET%
if exist "%BUNDLE_PATH%" (
    rmdir /s /q "%BUNDLE_PATH%"
    echo %C_WARNING%Previous version removed.%C_RESET%
)

echo %C_INFO%Creating bundle structure at:%C_RESET% %C_TEXT%%BUNDLE_PATH%%C_RESET%
mkdir "%BUNDLE_PATH%" >nul 2>&1
mkdir "%BUNDLE_PATH%\Contents" >nul 2>&1
mkdir "%BUNDLE_PATH%\Contents\Resources" >nul 2>&1
mkdir "%BUNDLE_PATH%\Contents\Win64" >nul 2>&1

echo.
echo %C_INFO%Copying plugin files...%C_RESET%
copy /y "%SOURCE_OFX_PATH%" "%BUNDLE_PATH%\Contents\Win64\FilmNegInvert.ofx" >nul
if errorlevel 1 (
    call :show_binary_copy_error
    goto end
)

if exist "%ICON_SOURCE_PATH%" (
    copy /y "%ICON_SOURCE_PATH%" "%BUNDLE_PATH%\Contents\Resources\com.tallerneg.FilmNegInvert.png" >nul
    if errorlevel 1 (
        call :show_icon_copy_error
        goto end
    )
) else (
    echo %C_WARNING%WARNING: Plugin icon not found at %ICON_SOURCE_PATH%%C_RESET%
    echo %C_DIM%The plugin will still install, but the icon resource will be missing.%C_RESET%
)

echo.
echo %C_SUCCESS%-----------------------------------------%C_RESET%
echo %C_SUCCESS%Plugin installed successfully!%C_RESET%
echo %C_SUCCESS%-----------------------------------------%C_RESET%
echo.
echo %C_TEXT%You can now find "Film Neg Invert" in the DaVinci Resolve OpenFX list.%C_RESET%

REM ===========================================
REM 3. SHARED OUTPUT
REM ===========================================

:end
call :print_footer
echo.
pause
echo %C_RESET%
endlocal
exit /b 0

:print_header
call :print_banner
echo %C_INFO%Installing the Film Neg Invert OFX plugin...%C_RESET%
echo.
exit /b 0

:print_banner
echo %C_BANNER% _________  ________  ___       ___       _______   ________            %C_RESET%
echo %C_BANNER%^|\___   ___\\   __  \^|\  \     ^|\  \     ^|\  ___ \ ^|\   __  \           %C_RESET%
echo %C_BANNER%\^|___ \  \_\ \  \^|\  \ \  \    \ \  \    \ \   __/^|\ \  \^|\  \          %C_RESET%
echo %C_BANNER%     \ \  \ \ \   __  \ \  \    \ \  \    \ \  \_^|/_\ \   _  _\         %C_RESET%
echo %C_BANNER%      \ \  \ \ \  \ \  \ \  \____\ \  \____\ \  \_^|\ \ \  \\  \^|        %C_RESET%
echo %C_BANNER%       \ \__\ \ \__\ \__\ \_______\ \_______\ \_______\ \__\\ _\        %C_RESET%
echo %C_BANNER%        \^|__^|  \^|__^|\^|__^|\^|_______^|\^|_______^|\^|_______^|\^|__^|\^|__^|       %C_RESET%
echo.
echo %C_TITLE%                         Research \ Film \ Develop%C_RESET%
echo %C_DIM%-------------------------------------------------------------------------%C_RESET%
echo.
exit /b 0

:print_footer
echo.
echo %C_DIM%Visit us at https://tallerlab.com/%C_RESET%
exit /b 0

:show_missing_binary_error
echo %C_ERROR%ERROR: No distributable plugin binary was found.%C_RESET%
echo %C_DIM%Checked:%C_RESET%
echo %C_TEXT%  %DIST_OFX_PATH%%C_RESET%
echo %C_TEXT%  %BUILD_OFX_PATH%%C_RESET%
echo.
echo %C_WARNING%What to do:%C_RESET%
echo %C_TEXT%- Make sure the repository includes dist\FilmNegInvert.ofx.%C_RESET%
echo %C_TEXT%- If you are a developer, build the project and run the installer again.%C_RESET%
exit /b 0

:show_binary_copy_error
echo %C_ERROR%ERROR: Failed to copy the plugin binary.%C_RESET%
echo.
echo %C_WARNING%What to do:%C_RESET%
echo %C_TEXT%- Close DaVinci Resolve if it is using the plugin.%C_RESET%
echo %C_TEXT%- Run the installer again as administrator.%C_RESET%
echo %C_TEXT%- Confirm that the binary is not blocked or read-only.%C_RESET%
exit /b 0

:show_icon_copy_error
echo %C_ERROR%ERROR: Failed to copy the plugin icon.%C_RESET%
echo.
echo %C_WARNING%What to do:%C_RESET%
echo %C_TEXT%- Confirm that Resources\com.tallerneg.FilmNegInvert.png exists.%C_RESET%
echo %C_TEXT%- Run the installer again as administrator.%C_RESET%
echo %C_TEXT%- Check whether the destination bundle is locked by another process.%C_RESET%
exit /b 0
