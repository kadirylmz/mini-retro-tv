@echo off
chcp 65001 >nul
REM =============================================================
REM  Mini Retro TV - Hizli Video + Ses Donusturucu (FFmpeg)
REM  Kullanim: Herhangi bir MP4 / MKV / MOV dosyasini bu .bat
REM            dosyasinin uzerine surukleyip birakin.
REM  Cikti   : ayni klasorde "<ad>_f15.mjpeg" + "<ad>_f15.pcm"
REM            Ikisini de MicroSD karttaki /v/ klasorune atin
REM            (veya Wi-Fi web arayuzunden yukleyin).
REM =============================================================

if "%~1"=="" (
    echo Bir video dosyasini bu .bat dosyasinin uzerine surukleyip birakin.
    pause
    exit /b 1
)

where ffmpeg >nul 2>&1
if errorlevel 1 (
    echo [HATA] ffmpeg bulunamadi.
    echo Kurmak icin PowerShell'de: winget install Gyan.FFmpeg
    pause
    exit /b 1
)

set "FPS=15"
set "IN=%~1"
set "OUT_V=%~dpn1_f%FPS%.mjpeg"
set "OUT_A=%~dpn1_f%FPS%.pcm"

echo [1/2] Video donusturuluyor (280x240, %FPS% FPS)...
ffmpeg -y -i "%IN%" -vf "fps=%FPS%,scale=280:240:force_original_aspect_ratio=increase,crop=280:240" -q:v 9 -f mjpeg "%OUT_V%"

echo [2/2] Ses donusturuluyor (16 kHz mono PCM)...
ffmpeg -y -i "%IN%" -vn -ac 1 -ar 16000 -f s16le "%OUT_A%"

echo.
echo [TAMAM] Hazir dosyalar:
echo   Video: "%OUT_V%"
echo   Ses  : "%OUT_A%"
echo Ikisini de MicroSD karttaki /v/ klasorune kopyalayabilirsiniz!
pause
