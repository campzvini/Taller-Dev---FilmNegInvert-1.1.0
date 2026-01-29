@echo off
REM ══════════════════════════════════════════════════════════════════════════════
REM  FILM NEG INVERT 1.0 - SCRIPTS/PACKAGE.BAT
REM  Script de empacotamento do plugin Film Neg Invert para distribuição
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
set VERSION=1.0.2
set BUILD_DIR=%~dp0..\build
set PACKAGE_DIR=%~dp0..\package
set DIST_DIR=%PACKAGE_DIR%\%BUNDLE_NAME%
set RELEASE_NAME=%PLUGIN_NAME%-%VERSION%-Win64

echo.
echo ══════════════════════════════════════════════════════════════════════════════
echo   FILM NEG INVERT - Empacotador
echo ══════════════════════════════════════════════════════════════════════════════
echo.

REM ============================================
REM 2. VERIFICAÇÃO DO BUILD # Verifica se o plugin foi compilado
REM ============================================
echo [INFO] Verificando build do plugin...
if not exist "%BUILD_DIR%\%PLUGIN_NAME%.ofx" (
    echo [ERRO] Plugin não encontrado em: %BUILD_DIR%\%PLUGIN_NAME%.ofx
    echo.
    echo O plugin precisa ser compilado antes do empacotamento.
    echo.
    echo Para compilar:
    echo   cd build
    echo   cmake --build . --config Release
    echo.
    pause
    exit /b 1
)

echo [OK] Plugin encontrado.
echo.

REM ============================================
REM 3. LIMPEZA # Remove pacotes anteriores
REM ============================================
echo [INFO] Limpando pacotes anteriores...
if exist "%PACKAGE_DIR%" (
    rmdir /S /Q "%PACKAGE_DIR%"
    echo [OK] Diretório de pacotes limpo.
) else (
    echo [INFO] Nenhum pacote anterior encontrado.
)
echo.

REM ============================================
REM 4. CRIAÇÃO DA ESTRUTURA # Cria estrutura do bundle OFX
REM ============================================
echo [INFO] Criando estrutura do bundle OFX...
echo   %DIST_DIR%\Contents\Win64

mkdir "%DIST_DIR%\Contents\Win64" 2>nul
if errorlevel 1 (
    echo [ERRO] Não foi possível criar a estrutura de diretórios.
    pause
    exit /b 1
)

echo [OK] Estrutura criada.
echo.

REM ============================================
REM 5. CÓPIA DO PLUGIN # Copia o arquivo do plugin
REM ============================================
echo [INFO] Copiando plugin para o bundle...
copy /Y "%BUILD_DIR%\%PLUGIN_NAME%.ofx" "%DIST_DIR%\Contents\Win64\%PLUGIN_NAME%.ofx" >nul 2>&1
if errorlevel 1 (
    echo [ERRO] Falha ao copiar o plugin.
    pause
    exit /b 1
)

echo [OK] Plugin copiado.
echo.

REM ============================================
REM 6. CÓPIA DA DOCUMENTAÇÃO # Copia arquivos de documentação
REM ============================================
echo [INFO] Copiando documentação...

if exist "%~dp0..\README.md" (
    copy "%~dp0..\README.md" "%PACKAGE_DIR%\README.md" >nul 2>&1
    echo   [OK] README.md copiado.
)

if exist "%~dp0..\CHANGELOG.md" (
    copy "%~dp0..\CHANGELOG.md" "%PACKAGE_DIR%\CHANGELOG.md" >nul 2>&1
    echo   [OK] CHANGELOG.md copiado.
)

if exist "%~dp0..\docs" (
    xcopy /E /I /Y "%~dp0..\docs" "%PACKAGE_DIR%\docs" >nul 2>&1
    echo   [OK] Documentação copiada.
)

echo.

REM ============================================
REM 7. CRIAÇÃO DO INSTALADOR # Cria script de instalação portátil
REM ============================================
echo [INFO] Criando instalador portátil...

(
echo @echo off
echo REM Instalador portátil do %PLUGIN_NAME% v%VERSION%
echo REM Execute como Administrador
echo.
echo set PLUGIN_NAME=%PLUGIN_NAME%
echo set BUNDLE_NAME=%BUNDLE_NAME%
echo set OFX_PLUGIN_DIR=C:\Program Files\Common Files\OFX\Plugins
echo set INSTALL_DIR=%%OFX_PLUGIN_DIR%%\%%BUNDLE_NAME%%\Contents\Win64
echo.
echo echo Instalando %%PLUGIN_NAME%% v%VERSION%%%
echo.
echo net session ^>nul 2^>^&1
echo if %%errorLevel%% neq 0 (
echo     echo [ERRO] Execute como Administrador!
echo     pause
echo     exit /b 1
echo )
echo.
echo if not exist "%%OFX_PLUGIN_DIR%%" mkdir "%%OFX_PLUGIN_DIR%%"
echo if not exist "%%OFX_PLUGIN_DIR%%\%%BUNDLE_NAME%%" mkdir "%%OFX_PLUGIN_DIR%%\%%BUNDLE_NAME%%"
echo if not exist "%%OFX_PLUGIN_DIR%%\%%BUNDLE_NAME%%\Contents" mkdir "%%OFX_PLUGIN_DIR%%\%%BUNDLE_NAME%%\Contents"
echo if not exist "%%INSTALL_DIR%%" mkdir "%%INSTALL_DIR%%"
echo.
echo copy /Y "%%~dp0%BUNDLE_NAME%\Contents\Win64\%PLUGIN_NAME%.ofx" "%%INSTALL_DIR%%\%PLUGIN_NAME%.ofx"
echo.
echo if exist "%%INSTALL_DIR%%\%PLUGIN_NAME%.ofx" (
echo     echo [SUCESSO] Plugin instalado!
echo     echo.
echo     echo Reinicie o DaVinci Resolve para usar o plugin.
echo ) else (
echo     echo [ERRO] Falha na instalação.
echo )
echo.
echo pause
) > "%PACKAGE_DIR%\INSTALL.bat"

echo [OK] Instalador portátil criado.
echo.

REM ============================================
REM 8. CRIAÇÃO DO ZIP # Compacta o pacote
REM ============================================
echo [INFO] Criando arquivo de distribuição...

cd "%PACKAGE_DIR%"

REM Verifica se o 7-Zip está disponível
set ZIP_TOOL=none
if exist "C:\Program Files\7-Zip\7z.exe" (
    set ZIP_TOOL=7z
    set ZIP_PATH="C:\Program Files\7-Zip\7z.exe"
) else if exist "C:\Program Files (x86)\7-Zip\7z.exe" (
    set ZIP_TOOL=7z
    set ZIP_PATH="C:\Program Files (x86)\7-Zip\7z.exe"
)

if "%ZIP_TOOL%"=="7z" (
    echo   Usando 7-Zip para compressão...
    %ZIP_PATH% a -tzip "%~dp0..\%RELEASE_NAME%.zip" "*" -r >nul 2>&1
    if errorlevel 1 (
        echo   [AVISO] Falha ao criar ZIP com 7-Zip.
        set ZIP_TOOL=none
    ) else (
        echo   [OK] Pacote criado: %RELEASE_NAME%.zip
    )
)

if "%ZIP_TOOL%"=="none" (
    echo   Usando PowerShell para compressão...
    powershell -Command "Compress-Archive -Path '%DIST_DIR%','%PACKAGE_DIR%\README.md','%PACKAGE_DIR%\INSTALL.bat' -DestinationPath '%~dp0..\%RELEASE_NAME%.zip' -Force" >nul 2>&1
    if errorlevel 1 (
        echo   [AVISO] Falha ao criar ZIP com PowerShell.
    ) else (
        echo   [OK] Pacote criado: %RELEASE_NAME%.zip
    )
)

echo.

REM ============================================
REM 9. RESUMO # Exibe resumo do pacote
REM ============================================
echo ══════════════════════════════════════════════════════════════════════════════
echo   EMPACOTAMENTO CONCLUÍDO!
echo ══════════════════════════════════════════════════════════════════════════════
echo.
echo Estrutura do pacote:
echo   %BUNDLE_NAME%/
echo   ├── Contents/echo   │   └── Win64/
echo   │       └── %PLUGIN_NAME%.ofx
echo   ├── docs/
echo   ├── README.md
echo   └── INSTALL.bat
echo.
echo Arquivos gerados:
echo   - package/%BUNDLE_NAME%/       (Bundle OFX)
echo   - %RELEASE_NAME%.zip           (Pacote de distribuição)
echo.
echo Para instalar:
echo   1. Extraia %RELEASE_NAME%.zip
echo   2. Execute INSTALL.bat como Administrador
echo   3. Reinicie o DaVinci Resolve
echo.

pause
exit /b 0
