@echo off
REM ══════════════════════════════════════════════════════════════════════════════
REM  FILM NEG INVERT 1.0 - INSTALL.BAT
REM  Script de instalação manual do plugin Film Neg Invert para DaVinci Resolve
REM  Windows Batch Script
REM ══════════════════════════════════════════════════════════════════════════════
REM Taller Dev - 2026
REM VAI CORINTHIANS!!
REM ══════════════════════════════════════════════════════════════════════════════

REM ============================================
REM 1. CONFIGURAÇÃO # Configurações do script
REM ============================================
set PLUGIN_NAME=FilmNegInvert
set BUNDLE_NAME=%PLUGIN_NAME%.ofx.bundle
set SOURCE_DIR=%~dp0build
set OFX_PLUGIN_DIR=C:\Program Files\Common Files\OFX\Plugins
set INSTALL_DIR=%OFX_PLUGIN_DIR%\%BUNDLE_NAME%\Contents\Win64

echo.
echo ══════════════════════════════════════════════════════════════════════════════
echo   FILM NEG INVERT - Instalador
echo ══════════════════════════════════════════════════════════════════════════════
echo.

REM ============================================
REM 2. VERIFICAÇÃO DE ADMINISTRADOR # Check admin rights
REM ============================================
net session >nul 2>&1
if %errorLevel% neq 0 (
    echo [ERRO] Este script precisa ser executado como Administrador!
    echo.
    echo Para instalar o plugin:
    echo 1. Clique com o botão direito no arquivo install.bat
    echo 2. Selecione "Executar como administrador"
    echo.
    pause
    exit /b 1
)

echo [OK] Privilégios de administrador confirmados.
echo.

REM ============================================
REM 3. VERIFICAÇÃO DO ARQUIVO # Verifica se o plugin foi compilado
REM ============================================
if not exist "%SOURCE_DIR%\%PLUGIN_NAME%.ofx" (
    echo [ERRO] Plugin não encontrado em: %SOURCE_DIR%\%PLUGIN_NAME%.ofx
    echo.
    echo O plugin precisa ser compilado antes da instalação.
    echo.
    echo Para compilar:
    echo 1. mkdir build ^&^& cd build
    echo 2. cmake .. -DOFX_ROOT=C:/dev/openfx
    echo 3. cmake --build . --config Release
    echo.
    pause
    exit /b 1
)

echo [OK] Plugin encontrado: %SOURCE_DIR%\%PLUGIN_NAME%.ofx
echo.

REM ============================================
REM 4. CRIAÇÃO DO DIRETÓRIO # Cria estrutura de diretórios
REM ============================================
echo Criando estrutura de diretórios...
echo   %INSTALL_DIR%

if not exist "%OFX_PLUGIN_DIR%" (
    mkdir "%OFX_PLUGIN_DIR%"
    if errorlevel 1 (
        echo [ERRO] Não foi possível criar: %OFX_PLUGIN_DIR%
        pause
        exit /b 1
    )
)

if not exist "%OFX_PLUGIN_DIR%\%BUNDLE_NAME%" (
    mkdir "%OFX_PLUGIN_DIR%\%BUNDLE_NAME%"
)

if not exist "%OFX_PLUGIN_DIR%\%BUNDLE_NAME%\Contents" (
    mkdir "%OFX_PLUGIN_DIR%\%BUNDLE_NAME%\Contents"
)

if not exist "%INSTALL_DIR%" (
    mkdir "%INSTALL_DIR%"
)

echo [OK] Diretórios criados.
echo.

REM ============================================
REM 5. BACKUP # Cria backup de instalação anterior
REM ============================================
if exist "%INSTALL_DIR%\%PLUGIN_NAME%.ofx" (
    echo Criando backup da instalação anterior...
    set BACKUP_NAME=%PLUGIN_NAME%_backup_%date:~-4,4%%date:~-7,2%%date:~-10,2%_%time:~0,2%%time:~3,2%%time:~6,2%.ofx
    set BACKUP_NAME=%BACKUP_NAME: =0%
    copy "%INSTALL_DIR%\%PLUGIN_NAME%.ofx" "%INSTALL_DIR%\%BACKUP_NAME%" >nul 2>&1
    echo [OK] Backup criado: %BACKUP_NAME%
    echo.
)

REM ============================================
REM 6. INSTALAÇÃO # Copia o plugin
REM ============================================
echo Instalando plugin...
echo   De: %SOURCE_DIR%\%PLUGIN_NAME%.ofx
echo   Para: %INSTALL_DIR%\%PLUGIN_NAME%.ofx

copy /Y "%SOURCE_DIR%\%PLUGIN_NAME%.ofx" "%INSTALL_DIR%\%PLUGIN_NAME%.ofx" >nul 2>&1
if errorlevel 1 (
    echo.
    echo [ERRO] Falha ao copiar o plugin!
    echo Verifique se o DaVinci Resolve está fechado e tente novamente.
    pause
    exit /b 1
)

echo [OK] Plugin instalado com sucesso!
echo.

REM ============================================
REM 7. VERIFICAÇÃO # Confirma instalação
REM ============================================
if exist "%INSTALL_DIR%\%PLUGIN_NAME%.ofx" (
    echo ══════════════════════════════════════════════════════════════════════════════
    echo   INSTALAÇÃO CONCLUÍDA COM SUCESSO!
    echo ══════════════════════════════════════════════════════════════════════════════
    echo.
    echo O plugin foi instalado em:
    echo   %INSTALL_DIR%\%PLUGIN_NAME%.ofx
    echo.
    echo Próximos passos:
    echo 1. Abra o DaVinci Resolve
    echo 2. Vá para a página Color
    echo 3. Adicione o node Film Neg Invert:
    echo    - Clique com botão direito no node editor
    echo    - OpenFX ^> Color ^> Film Neg Invert
    echo.
    echo Para desinstalar, delete o diretório:
    echo   %OFX_PLUGIN_DIR%\%BUNDLE_NAME%
    echo.
) else (
    echo [ERRO] Falha na verificação da instalação!
    pause
    exit /b 1
)

pause
exit /b 0
